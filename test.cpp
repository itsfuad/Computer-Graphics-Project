#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <memory>
#include <chrono>
#include <functional>
#include <random>
#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <unordered_map>

// Define PI if not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// AudioManager class definition
class AudioManager {
private:
    ALCdevice* device;
    ALCcontext* context;
    std::unordered_map<std::string, ALuint> buffers;
    std::unordered_map<std::string, ALuint> sources;
    bool isInitialized;

    void cleanupSource(const std::string& name) {
        if (sources.find(name) != sources.end()) {
            alSourceStop(sources[name]);
            alDeleteSources(1, &sources[name]);
            sources.erase(name);
        }
    }

public:
    AudioManager() : device(nullptr), context(nullptr), isInitialized(false) {}

    bool initialize() {
        if (isInitialized) return true;

        device = alcOpenDevice(nullptr);
        if (!device) {
            std::cerr << "Failed to open audio device" << std::endl;
            return false;
        }

        context = alcCreateContext(device, nullptr);
        if (!context) {
            std::cerr << "Failed to create audio context" << std::endl;
            alcCloseDevice(device);
            return false;
        }

        alcMakeContextCurrent(context);
        ALenum error = alGetError(); // Clear any previous error
        if (error != AL_NO_ERROR) {
            std::cerr << "OpenAL error after making context current: " << alGetString(error) << std::endl;
        }
        isInitialized = true;
        std::cerr << "AudioManager initialized successfully." << std::endl;
        return true;
    }

    bool loadSound(const std::string& name, const std::string& filePath) {
        if (!isInitialized) return false;

        ALuint buffer;
        alGenBuffers(1, &buffer);
        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            std::cerr << "OpenAL error generating buffer: " << alGetString(error) << std::endl;
            return false;
        }

        FILE* file = fopen(filePath.c_str(), "rb");
        if (!file) {
            std::cerr << "Failed to open sound file: " << filePath << std::endl;
            alDeleteBuffers(1, &buffer);
            return false;
        }

        char riffChunkID[4];
        long riffChunkSize;
        char waveFormat[4];
        if (fread(riffChunkID, 1, 4, file) != 4 || memcmp("RIFF", riffChunkID, 4) != 0 ||
            fread(&riffChunkSize, 4, 1, file) != 1 ||
            fread(waveFormat, 1, 4, file) != 4 || memcmp("WAVE", waveFormat, 4) != 0) {
            std::cerr << "Not a valid RIFF/WAVE file: " << filePath << std::endl;
            fclose(file);
            alDeleteBuffers(1, &buffer);
            return false;
        }

        short audioFormat = 0;
        short numChannels = 0;
        int sampleRate = 0;
        short bitsPerSample = 0;
        int dataSize = 0;
        ALenum format = 0;

        while (!feof(file)) {
            char chunkID[4];
            int chunkSize;
            if (fread(chunkID, 1, 4, file) != 4 || fread(&chunkSize, 4, 1, file) != 1) {
                if (feof(file)) break; // Reached end of file cleanly
                std::cerr << "Failed to read WAV chunk header for file: " << filePath << std::endl;
                fclose(file);
                alDeleteBuffers(1, &buffer);
                return false;
            }

            if (memcmp("fmt ", chunkID, 4) == 0) {
                if (chunkSize < 16) { // Minimum fmt chunk size is 16
                    std::cerr << "WAV fmt chunk too small for file: " << filePath << std::endl;
                    fclose(file);
                    alDeleteBuffers(1, &buffer);
                    return false;
                }
                char fmtData[16]; // Only read first 16 bytes for essential info
                if (fread(fmtData, 1, 16, file) != 16) {
                    std::cerr << "Failed to read fmt data for file: " << filePath << std::endl;
                    fclose(file);
                    alDeleteBuffers(1, &buffer);
                    return false;
                }
                audioFormat = *(short*)&fmtData[0];
                numChannels = *(short*)&fmtData[2];
                sampleRate = *(int*)&fmtData[4];
                bitsPerSample = *(short*)&fmtData[14];

                // Determine OpenAL format
                if (audioFormat != 1) { // PCM format
                    std::cerr << "Unsupported WAV audio format (not PCM): " << audioFormat << " for file: " << filePath << std::endl;
                    fclose(file);
                    alDeleteBuffers(1, &buffer);
                    return false;
                }

                if (numChannels == 1 && bitsPerSample == 8) format = AL_FORMAT_MONO8;
                else if (numChannels == 1 && bitsPerSample == 16) format = AL_FORMAT_MONO16;
                else if (numChannels == 2 && bitsPerSample == 8) format = AL_FORMAT_STEREO8;
                else if (numChannels == 2 && bitsPerSample == 16) format = AL_FORMAT_STEREO16;
                else {
                    std::cerr << "Unsupported WAV format: channels=" << numChannels << ", bits=" << bitsPerSample << " for file: " << filePath << std::endl;
                    fclose(file);
                    alDeleteBuffers(1, &buffer);
                    return false;
                }
                // Skip any extra bytes in fmt chunk if chunkSize > 16
                if (chunkSize > 16) {
                    fseek(file, chunkSize - 16, SEEK_CUR);
                }
            } else if (memcmp("data", chunkID, 4) == 0) {
                dataSize = chunkSize;
                break; // Found data chunk, stop searching for chunks
            } else {
                // Skip unknown chunk
                fseek(file, chunkSize, SEEK_CUR);
            }
        }

        if (format == 0 || dataSize == 0) {
            std::cerr << "Missing or invalid 'fmt ' or 'data' chunk in WAV file: " << filePath << std::endl;
            fclose(file);
            alDeleteBuffers(1, &buffer);
            return false;
        }

        std::vector<unsigned char> data(dataSize);
        if (fread(data.data(), 1, dataSize, file) != static_cast<size_t>(dataSize)) {
            std::cerr << "Failed to read audio data from " << filePath << std::endl;
            fclose(file);
            alDeleteBuffers(1, &buffer);
            return false;
        }
        fclose(file);

        std::cerr << "Loading " << name << " - Format: " << format << " (Channels: " << numChannels << ", Bits: " << bitsPerSample << "), Size: " << dataSize << ", Sample Rate: " << sampleRate << std::endl;

        alBufferData(buffer, format, data.data(), dataSize, sampleRate);
        error = alGetError();
        if (error != AL_NO_ERROR) {
            std::cerr << "Failed to buffer audio data from " << filePath << ": " << alGetString(error) << std::endl;
            alDeleteBuffers(1, &buffer);
            return false;
        }

        buffers[name] = buffer;
        std::cerr << "Sound loaded successfully: " << name << " from " << filePath << std::endl;
        return true;
    }

    void playSound(const std::string& name, bool loop = false) {
        if (!isInitialized || buffers.find(name) == buffers.end()) return;

        cleanupSource(name);

        ALuint source;
        alGenSources(1, &source);

        alSourcei(source, AL_BUFFER, buffers[name]);
        alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcef(source, AL_GAIN, 0.5f);

        alSourcePlay(source);
        sources[name] = source;
    }

    void stopSound(const std::string& name) {
        if (!isInitialized) return;
        cleanupSource(name);
    }

    void setVolume(const std::string& name, float volume) {
        if (!isInitialized || sources.find(name) == sources.end()) return;
        alSourcef(sources[name], AL_GAIN, std::max(0.0f, std::min(1.0f, volume)));
    }

    bool isPlaying(const std::string& name) {
        if (!isInitialized || sources.find(name) == sources.end()) return false;
        ALint state;
        alGetSourcei(sources[name], AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    void cleanup() {
        if (!isInitialized) return;

        for (const auto& source : sources) {
            alSourceStop(source.second);
            alDeleteSources(1, &source.second);
        }
        sources.clear();

        for (const auto& buffer : buffers) {
            alDeleteBuffers(1, &buffer.second);
        }
        buffers.clear();

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);
        isInitialized = false;
    }

    ~AudioManager() {
        cleanup();
    }
};

// Global audio manager instance
AudioManager audioManager;

// ===================================================================
//  Constants and Global Variables from Simulation
// ===================================================================
const float USER_CAR_SPEED_BASE = 3.5f;  // Increased from 1.75f to 3.5f
const float USER_HUMAN_SPEED = 2.80f;
const int MIN_TIME_BETWEEN_SPAWNS_CAR = 30;

const int MAX_ACTIVE_HUMANS = 10;
const int MIN_TIME_BETWEEN_SPAWNS_HUMAN = 15;
const float USER_HUMAN_SIDEWALK_SPEED_FACTOR = 1.2f;
const int HUMAN_SPAWN_RATE_SIDEWALK = 120;

const int CAR_SPAWN_RATE = 100;

// Window and Scene Dimensions
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;
const float ROAD_Y_BOTTOM = 150.0f;
const float ROAD_Y_TOP = 250.0f;
const float SIDEWALK_TOP_Y_START = 250.0f;
const float SIDEWALK_TOP_Y_END = 280.0f;
const float SIDEWALK_BOTTOM_Y_START = 120.0f;
const float SIDEWALK_BOTTOM_Y_END = 150.0f;

const float TRAFFIC_LIGHT_X = 600.0f;
const float CAR_STOP_LINE_X = TRAFFIC_LIGHT_X - 20.0f;
const float HUMAN_CROSSING_X_START = TRAFFIC_LIGHT_X + 35.0f;
const float HUMAN_CROSSING_WIDTH = 70.0f;
const float HUMAN_CROSSING_CENTER_X = HUMAN_CROSSING_X_START + HUMAN_CROSSING_WIDTH / 2.0f;

const float MIN_CAR_SPACING_AHEAD = 20.0f;
const float MIN_CAR_SPAWN_DISTANCE = 300.0f;
const float CAR_SAME_LANE_Y_THRESHOLD = 5.0f;
const float HUMAN_SAFETY_BUFFER = 3.0f;
const float DISTANCE_TO_STOP_FROM_SIGNAL = 20.0f;
const float CAR_SPAWN_OFFSCREEN_DISTANCE = 100.0f;

// Day/Night Cycle
const float USER_DAY_NIGHT_CYCLE_SPEED = 0.0008f;
float currentTimeOfDay = 0.3f;
bool isNight = false;
unsigned char frameCount = 0;

// Scene State
bool scenePaused = false;
bool DEBUG_ON = false;

bool showWarningMessage = false;
int lastCarSpawnTime = 0;
int lastHumanSpawnTime = 0;

// Enums for Traffic and Pedestrian Lights
enum class TrafficLightState { RED, GREEN };
enum class VehicleType { CAR, TRUCK, BUS };

TrafficLightState lightState = TrafficLightState::GREEN;

// Add these constants near the top with other constants
const float CAR_PRIORITY_THRESHOLD = 450.0f;  // Distance to check for cars
const int MIN_CARS_FOR_PRIORITY = 2;         // Minimum cars to give them priority
const int MIN_HUMANS_FOR_PRIORITY = 1;       // Minimum humans to give them priority

// Add this near the top of the file with other constants
const float VEHICLE_WHEEL_OFFSET = 5.0f;  // How much to lift vehicles to prevent wheel overlap

// ===================================================================
//  Helper Structs & Classes from Man.cpp and oldCode.cpp
// ===================================================================

// A simple struct to hold RGB color values
struct Color {
    float r, g, b;
};

// Redesigned hairstyles for better visuals and 4-direction visibility
enum class HairStyle {
    Spiky,
    FlatTop,
    Bald
};

// Bounding box struct
struct Rect {
    float x, y, w, h;
};

class Drawable {
public:
    float x, y, width, height;
    Drawable(float _x, float _y, float _w, float _h) : x(_x), y(_y), width(_w), height(_h) {}
    Rect getBounds() {
        return {x, y, width, height};
    }
    virtual void draw() = 0;  // Pure virtual function
    virtual ~Drawable() = default;
};

class Vehicle;
class AdvancedHuman;

// Prototypes for functions that are defined later
void drawText(float x, float y, const char* text, float scale = 0.7f) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);
    glRasterPos2f(0, 0);
    for (const char* p = text; *p; p++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
    }
    glPopMatrix();
}

void drawRect(float x, float y, float width, float height, float translateX = 0.0f, float translateY = 0.0f, int type = GL_QUADS) {
    glBegin(type);
    glVertex2f(x + translateX, y + translateY);
    glVertex2f(x + width + translateX, y + translateY);
    glVertex2f(x + width + translateX, y + height + translateY);
    glVertex2f(x + translateX, y + height + translateY);
    glEnd();
}

void drawBound(float x, float y, float width, float height, float translateX = 0.0f, float translateY = 0.0f) {
    glLineWidth(2.0f);
    drawRect(x, y, width, height, translateX, translateY, GL_LINE_LOOP);
    glLineWidth(1.0f);
}

void drawLine(float x1, float y1, float x2, float y2, float thickness = 1.0f) {
    glLineWidth(thickness);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawCircle(float cx, float cy, float r, int num_segments = 100) {
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta));
    }
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

bool checkAABBCollision(Rect r1, Rect r2) {
    return (r1.x < r2.x + r2.w && r1.x + r1.w > r2.x && r1.y < r2.y + r2.h && r1.y + r1.h > r2.y);
}
int timeNow() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// ===================================================================
//  NEW VEHICLE DRAWING FUNCTIONS
// ===================================================================

void drawModernCar(float x, float y, float width, float height, float r, float g, float b, bool isNight, float current_speed, bool isHonking = false) {
    glPushMatrix();
    glTranslatef(0, VEHICLE_WHEEL_OFFSET, 0);  // Translate entire vehicle up

    // Body
    glColor3f(r, g, b);
    drawRect(x, y, width, height * 0.6f);

    // Roof and Windows
    glColor3f(0.6f, 0.8f, 0.9f); // Light blue for windows
    glBegin(GL_QUADS);
    glVertex2f(x + width * 0.15f, y + height * 0.6f);
    glVertex2f(x + width * 0.85f, y + height * 0.6f);
    glVertex2f(x + width * 0.7f, y + height);
    glVertex2f(x + width * 0.25f, y + height);
    glEnd();

    // Window outlines
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x + width * 0.15f, y + height * 0.6f);
    glVertex2f(x + width * 0.85f, y + height * 0.6f);
    glVertex2f(x + width * 0.7f, y + height);
    glVertex2f(x + width * 0.25f, y + height);
    glEnd();
    drawLine(x + width * 0.5f, y + height * 0.6f, x + width * 0.5f, y + height);

    // Wheels - draw them at y - VEHICLE_WHEEL_OFFSET
    glColor3f(0.1f, 0.1f, 0.1f); // Tire
    drawCircle(x + width * 0.25f, y, height * 0.25f);
    drawCircle(x + width * 0.75f, y, height * 0.25f);
    glColor3f(0.7f, 0.7f, 0.7f); // Rim
    drawCircle(x + width * 0.25f, y, height * 0.15f);
    drawCircle(x + width * 0.75f, y, height * 0.15f);

    // Brake Lights (rear)
    if (current_speed < 0.1f) {
        glColor3f(1.0f, 0.0f, 0.0f);
    } else {
        glColor3f(0.6f, 0.0f, 0.0f);
    }
    drawRect(x, y + height * 0.3f, 4, 6);

    // Headlights (front)
    if (isNight) {
        glColor4f(1.0f, 1.0f, 0.7f, 0.3f); // Glow
        drawCircle(x + width, y + height * 0.4f, 20.0f);
        glColor3f(1.0f, 1.0f, 0.8f); // Light source
    } else {
        glColor3f(0.9f, 0.9f, 0.9f); // Turned off light
    }
    drawRect(x + width - 4, y + height * 0.3f, 4, 6);

    glPopMatrix();  // Restore original matrix
}


void drawTruck(float x, float y, float width, float height, float r, float g, float b, bool isNight, float current_speed, bool isHonking = false) {
    glPushMatrix();
    glTranslatef(0, VEHICLE_WHEEL_OFFSET, 0);  // Translate entire vehicle up
    
    // --- Chassis and Cargo connection
    glColor3f(0.0f, 0.0f, 0.0f);
    drawLine(x + width * 0.65f, y + height * 0.2f, x + width * 0.65f, y + height * 0.8f, 6.0f);
    
    // --- Chassis ---
    glColor3f(0.2f, 0.2f, 0.2f);
    drawRect(x, y, width, 5); // A thin, dark chassis base

    // --- Cargo Bed (Container Style) ---
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f); // Slightly darker shade for the container
    float containerWidth = width * 0.68f;
    float containerHeight = height * 0.9f;
    drawRect(x - 3, y + 5, containerWidth, containerHeight);
    // Add some vertical lines for detail on the container
    glColor3f(r * 0.7f, g * 0.7f, b * 0.7f);
    glLineWidth(2.0f);
    for(int i=1; i<4; ++i){
        drawLine(x + containerWidth * (i/4.0f), y + 5, x + containerWidth * (i/4.0f), y + 5 + containerHeight);
    }


    // --- Cab ---
    float cabWidth = width - containerWidth;
    float cabX = x + containerWidth;
    glColor3f(r, g, b); // Main cab color
    glBegin(GL_QUADS);
    glVertex2f(cabX, y + 5);
    glVertex2f(cabX + cabWidth, y + 5);
    glVertex2f(cabX + cabWidth, y + height * 0.8f);
    glVertex2f(cabX, y + height * 0.9f); // Gives the cab a slight backward tilt
    glEnd();


    // --- Cab Window ---
    glColor3f(0.6f, 0.8f, 0.9f); // Light blue window color
    glBegin(GL_POLYGON);
    glVertex2f(cabX + cabWidth * 0.5f, y + height * 0.42f);
    glVertex2f(cabX + cabWidth * 1.0f, y + height * 0.42f);
    glVertex2f(cabX + cabWidth * 1.0f, y + height * 0.75f);
    glVertex2f(cabX + cabWidth * 0.5f, y + height * 0.8f);
    glEnd();


    // --- Details: Fuel tank and exhaust ---
    glColor3f(0.5f, 0.5f, 0.5f); // Grey for fuel tank
    drawRect(x + containerWidth - 25, y + 5, 20, 10);
    glColor3f(0.3f, 0.3f, 0.3f); // Dark for exhaust
    drawRect(cabX + cabWidth * 0.1f, y + height - 4, 3, 10); // Vertical exhaust pipe


    // --- Wheels (3 sets) ---
    float wheelRadius = height * 0.2f;
    glColor3f(0.1f, 0.1f, 0.1f); // Tire
    drawCircle(x + width * 0.15f, y + wheelRadius, wheelRadius);
    drawCircle(x + width * 0.35f, y + wheelRadius, wheelRadius);
    drawCircle(x + width * 0.85f, y + wheelRadius, wheelRadius);
    glColor3f(0.7f, 0.7f, 0.7f); // Rim
    drawCircle(x + width * 0.15f, y + wheelRadius, wheelRadius * 0.6f);
    drawCircle(x + width * 0.35f, y + wheelRadius, wheelRadius * 0.6f);
    drawCircle(x + width * 0.85f, y + wheelRadius, wheelRadius * 0.6f);


    // --- Lights ---
    // Brake Lights (rear)
    if (current_speed < 0.1f) {
        glColor3f(1.0f, 0.0f, 0.0f);
    } else {
        glColor3f(0.6f, 0.0f, 0.0f);
    }
    drawRect(x - 4, y + height * 0.4f, 4, 5);

    // Headlights (front)
    if (isNight) {
        glColor4f(1.0f, 1.0f, 0.7f, 0.3f); // Glow
        drawCircle(x + width, y + height * 0.2f, 25.0f);
        glColor3f(1.0f, 1.0f, 0.8f); // Light source
    } else {
        glColor3f(0.9f, 0.9f, 0.9f); // Turned off light
    }
    
    drawRect(x + width - 5, y + height * 0.2f, 5, 5);

    glPopMatrix();  // Restore original matrix
}


void drawBus(float x, float y, float width, float height, float r, float g, float b, bool isNight, float current_speed, bool isHonking = false) {
    glPushMatrix();
    glTranslatef(0, VEHICLE_WHEEL_OFFSET, 0);  // Translate entire vehicle up

    // Main Body
    glColor3f(r, g, b);
    drawRect(x, y, width, height);

    // Windows
    glColor3f(0.2f, 0.2f, 0.3f); // Dark windows
    float windowHeight = height * 0.4f;
    float windowY = y + height * 0.4f;
    float windowWidth = width * 0.12f;
    float windowSpacing = width * 0.05f;
    for (int i = 0; i < 5; ++i) {
        float windowX = x + width * 0.1f + i * (windowWidth + windowSpacing);
        drawRect(windowX, windowY, windowWidth, windowHeight);
    }

    // Wheels - draw them at y - VEHICLE_WHEEL_OFFSET
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(x + width * 0.2f, y, height * 0.25f);
    drawCircle(x + width * 0.8f, y, height * 0.25f);
    glColor3f(0.7f, 0.7f, 0.7f);
    drawCircle(x + width * 0.2f, y, height * 0.15f);
    drawCircle(x + width * 0.8f, y, height * 0.15f);

    // Brake Lights
    if (current_speed < 0.1f) glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3f(0.6f, 0.0f, 0.0f);
    drawRect(x, y + height * 0.2f, 4, 8);

    // Headlights
    if (isNight) {
        glColor4f(1.0f, 1.0f, 0.7f, 0.3f);
        drawCircle(x + width, y + height * 0.25f, 30.0f);
        glColor3f(1.0f, 1.0f, 0.8f);
    } else {
        glColor3f(0.9f, 0.9f, 0.9f);
    }
    drawRect(x + width - 5, y + height * 0.25f, 4, 4);

    glPopMatrix();  // Restore original matrix
}

struct Cloud {
    float x, y;
    float speed;
    float scale;
    Cloud(float _x, float _y, float _scale) : x(_x), y(_y), scale(_scale) {
        speed = 0.2f + (rand() % 30) / 100.0f; // Random speed between 0.2 and 0.5
    }
};

// Add this struct after the Cloud struct
struct Star {
    float x, y;
    float size;
    float baseBrightness;
    float blinkPhase;
    Star(float _x, float _y, float _size) :
        x(_x), y(_y), size(_size),
        baseBrightness(0.7f + (rand() % 30) / 100.0f), // Random base brightness between 0.7 and 1.0
        blinkPhase(rand() % 628) // Random starting phase (0 to 2π)
    {}
};

// Global collections of scene objects
std::vector<std::shared_ptr<Vehicle>> vehicles; // Changed from Car
std::vector<std::shared_ptr<AdvancedHuman>> activeHumans;
std::vector<std::shared_ptr<Drawable>> drawableObjects;  // Change back to shared_ptr
std::vector<Cloud> clouds;
std::vector<Star> stars;

// Global vector to store debug drawing calls
std::vector<std::function<void()>> debugCalls;

// --- Function to draw all stored debug calls ---
void drawDebugOverlay() {
    for (const auto& debugCall : debugCalls) {
        debugCall();
    }
    debugCalls.clear(); // Clear calls for the next frame
}

class TrafficSignal : public Drawable {
public:
    TrafficLightState lightState;
    bool yellowLightOn;
    const int YELLOW_BLINK_INTERVAL = 15;
    TrafficSignal(float x, float y) : Drawable(x, y, 20, 60) {
        lightState = TrafficLightState::RED;
        yellowLightOn = false;
    }

    void showGreenLight() {
        lightState = TrafficLightState::GREEN;
    }

    void showRedLight() {
        lightState = TrafficLightState::RED;
    }

    void draw() override {
        const float height = 60.0f;
        const float width = 20.0f;
        const float lightRadius = 8.0f;
        float spacing = height / 3.0f;
        int translateY = 60;
        int translateX = -(width / 4);
        const float gap = 18;

        // Draw three segments (top, middle, bottom)
        for (int i = 0; i < 3; i++) {
            float centerY = y + height - (i + 0.5f) * spacing;
            // Determine bulb color based on segment and current state
            if(i == 0 && lightState == TrafficLightState::RED && !yellowLightOn)
                glColor3f(1.0f, 0.0f, 0.0f);
            else if(i == 1 && yellowLightOn)
                glColor3f(1.0f, 1.0f, 0.0f);
            else if(i == 2 && lightState == TrafficLightState::GREEN && !yellowLightOn)
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(0.3f, 0.3f, 0.3f); // dim bulb when off

            // Draw left and right semi-circles for the bulb
            drawCircle(x + translateX + gap, centerY + translateY - lightRadius, lightRadius);
            drawCircle(x + translateX + width - gap, centerY + translateY - lightRadius, lightRadius);

            bool drawGlow = true;

            if(i == 0 && lightState == TrafficLightState::RED && !yellowLightOn)
                glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
            else if(i == 1 && yellowLightOn)
                glColor4f(1.0f, 1.0f, 0.0f, 0.3f);
            else if(i == 2 && lightState == TrafficLightState::GREEN && !yellowLightOn)
                glColor4f(0.0f, 1.0f, 0.0f, 0.3f);
            else
                drawGlow = false;
        
            if (drawGlow) {
                drawCircle(x + translateX + gap, centerY + translateY - lightRadius, lightRadius * 1.8f);
                drawCircle(x + translateX + width - gap, centerY + translateY - lightRadius, lightRadius * 1.8f);
            }

            // Draw triangle shapes on each side
            glColor3f(0.0f, 0.0f, 0.0f);
            drawTriangle(x + translateX, centerY + translateY, x + translateX - 10, centerY + translateY, x + translateX, centerY - 10 + translateY);
            drawTriangle(x + translateX + width, centerY + translateY, x + translateX + width + 10, centerY + translateY, x + translateX + width, centerY - 10 + translateY);
        }

        // Draw the main black signal box
        glColor3f(0.0f, 0.0f, 0.0f);
        drawRect(x + translateX, y + translateY - 8, width, height);
        
        // Draw the pole (originally missing)
        drawRect(x, y, 10, 100); 

        drawHumanSign();
    }

private:
    void drawHumanShape(float x, float y, float scale, int walkState) {
        glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(scale, scale, 1.0f);
        
        if (walkState == 1) {
            // WALK state - walking figure
            //color 
            glColor3f(0.0f, 0.9f, 0.0f); // Green for "WALK"
            //left hand
            drawLine(-7, -4, -3, 4, 4.0f); // Adjust for visibility
            drawLine(6, 0, 3, 4, 4.0f);    // Adjust for visibility
            drawLine(6, -4, 6, 0, 4.0f);    // Adjust for visibility
            //legs as ^
            drawLine(-5, -10, -1, -2, 4.0f); // Adjust for visibility
            drawLine(5, -10, 1, -2, 4.0f);  // Adjust for visibility
        } else {
            // STOP state - standing figure
            glColor3f(0.9f, 0.0f, 0.0f); // Red for "STOP"
            //left hand
            drawLine(-3, -4, -3, 4, 4.0f); // Adjust for visibility
            //right hand
            drawLine(3.2, -4, 3.2, 4, 4.0f); // Adjust for visibility
            //left leg
            drawLine(-1, -10, -1, -2, 4.0f); // Adjust for visibility
            //right leg
            drawLine(1.2, -10, 1.2, -2, 4.0f); // Adjust for visibility
        }

        drawCircle(0, 7.5, 3);
        // Body - Adjust for centering and visibility
        drawRect(0.0f, 0.0f, 4.0f, 8.0f, -4.0f / 2.0f, -8.0f / 2.0f); 
        
        glPopMatrix();
    }
    void drawHumanSign() {
        // Add blinking logic for yellow light
        if (yellowLightOn) {
            // Blink the DONT_WALK sign
            if ((frameCount / YELLOW_BLINK_INTERVAL) % 2 == 0) { // Draw on alternate blink intervals
                glColor3f(0.9f, 0.0f, 0.0f); // Red for blinking "DONT WALK"
                drawHumanShape(x + 5, y + 85, 0.8, 0); // Draw standing figure (DONT_WALK)
            }
        } else {
            // Normal drawing based on pedestrian light state
            if (lightState == TrafficLightState::RED) {
                glColor3f(0.0f, 0.9f, 0.0f);
            } else {
                glColor3f(0.9f, 0.0f, 0.0f);
            }
            drawHumanShape(x + 5, y + 85, 0.8, lightState == TrafficLightState::RED ? 1 : 0);
        }
    }
};

std::shared_ptr<TrafficSignal> trafficSignal = std::make_shared<TrafficSignal>(TRAFFIC_LIGHT_X, SIDEWALK_TOP_Y_START);

enum class HumanState { 
    WALKING_ON_SIDEWALK,
    WAITING_AT_CROSSING_EDGE,
    CROSSING_ROAD,
    REACHED_OTHER_SIDEWALK,
    WALKING_AWAY_ON_SIDEWALK, 
    DESPAWNED 
};

class AdvancedHuman : public Drawable {
public:
    // Simulation state
    HumanState state;
    float targetX;
    float currentSidewalkY;
    bool onBottomSidewalkInitially;
    bool willCrossRoad;
    float speedFactor;
    float speed;

    // Visual properties
    Color shirtColor;
    Color pantsColor;
    Color hairColor;
    Color skinColor;
    HairStyle hairStyle;
    float scale;

    // Animation state
    float walkCycle;
    bool isWalking;
    int direction;
    float SYNC_CONST;

    AdvancedHuman(float startX, float startY, float visualScale, const Color& shirt, const Color& pants, const Color& skin, const Color& hair, HairStyle style)
        : Drawable(startX, startY, 0.2f, 0.4f),
          state(HumanState::WALKING_ON_SIDEWALK),
          targetX(0.0f),
          currentSidewalkY(0.0f),
          onBottomSidewalkInitially(false),
          willCrossRoad(false),
          speedFactor(1.0f + (rand() % 41) / 100.0f),  // Random factor between 1.0 and 1.4
          speed(0.7f * speedFactor),  // Base speed of 1.0 multiplied by speed factor
          shirtColor(shirt),
          pantsColor(pants),
          hairColor(hair),
          skinColor(skin),
          hairStyle(style),
          scale(visualScale),
          walkCycle(0.0f),
          isWalking(false),
          direction(0) {
        SYNC_CONST = 0.15f;
    }

    // --- Control Methods ---
    void startWalking(int dir) {
        isWalking = true;
        direction = dir;
    }

    void stopWalking() {
        isWalking = false;
        walkCycle = 0.0f;
    }

    // --- Core Public Methods ---
    void update() {
        bool isCurrentlyMoving = false;
        float effectiveSpeed = speed;
        float walkCycleSpeed = 0.2f;  // Base walk cycle speed

        switch (state) {
            case HumanState::WALKING_ON_SIDEWALK:
                isCurrentlyMoving = true;
                y = currentSidewalkY;
                effectiveSpeed *= USER_HUMAN_SIDEWALK_SPEED_FACTOR;
                walkCycleSpeed = effectiveSpeed * SYNC_CONST;  // Scale walk cycle with movement speed
                if (fabs(x - targetX) < effectiveSpeed * 1.5f) {
                    x = targetX;
                    state = willCrossRoad ? HumanState::WAITING_AT_CROSSING_EDGE : HumanState::DESPAWNED;
                } else if (x < targetX) {
                    x += effectiveSpeed;
                    startWalking(0); // Right
                } else {
                    x -= effectiveSpeed;
                    startWalking(1); // Left
                }
                break;

            case HumanState::WAITING_AT_CROSSING_EDGE:
                stopWalking();
                if (trafficSignal->lightState == TrafficLightState::RED && !trafficSignal->yellowLightOn) {
                    state = HumanState::CROSSING_ROAD;
                }
                break;

            case HumanState::CROSSING_ROAD:
                isCurrentlyMoving = true;
                effectiveSpeed *= 0.8f;  // Slightly slower when crossing road
                walkCycleSpeed = effectiveSpeed * SYNC_CONST;  // Scale walk cycle with movement speed
                if (onBottomSidewalkInitially) {
                    y += effectiveSpeed;
                    startWalking(2); // Up
                    if (y >= (SIDEWALK_TOP_Y_START + SIDEWALK_TOP_Y_END) / 2.0f) {
                        y = (SIDEWALK_TOP_Y_START + SIDEWALK_TOP_Y_END) / 2.0f;
                        currentSidewalkY = y;
                        state = HumanState::REACHED_OTHER_SIDEWALK;
                    }
                } else {
                    y -= effectiveSpeed;
                    startWalking(3); // Down
                    if (y <= (SIDEWALK_BOTTOM_Y_START + SIDEWALK_BOTTOM_Y_END) / 2.0f) {
                        y = (SIDEWALK_BOTTOM_Y_START + SIDEWALK_BOTTOM_Y_END) / 2.0f;
                        currentSidewalkY = y;
                        state = HumanState::REACHED_OTHER_SIDEWALK;
                    }
                }
                break;

            case HumanState::REACHED_OTHER_SIDEWALK:
                stopWalking();
                targetX = (x < WINDOW_WIDTH / 2) ? -50.0f : WINDOW_WIDTH + 50.0f;
                state = HumanState::WALKING_AWAY_ON_SIDEWALK;
                break;

            case HumanState::WALKING_AWAY_ON_SIDEWALK:
                isCurrentlyMoving = true;
                y = currentSidewalkY;
                effectiveSpeed *= USER_HUMAN_SIDEWALK_SPEED_FACTOR;
                walkCycleSpeed = effectiveSpeed * SYNC_CONST;  // Scale walk cycle with movement speed
                if ((targetX < 0 && x < 0) || (targetX > WINDOW_WIDTH && x > WINDOW_WIDTH)) {
                    state = HumanState::DESPAWNED;
                } else if (x < targetX) {
                    x += effectiveSpeed;
                    startWalking(0); // Right
                } else {
                    x -= effectiveSpeed;
                    startWalking(1); // Left
                }
                break;
            
            case HumanState::DESPAWNED:
                stopWalking();
                break;
        }

        if (isCurrentlyMoving) {
            walkCycle += walkCycleSpeed;  // Use the speed-adjusted walk cycle increment
            if (walkCycle >= 2.0f * M_PI) {
                walkCycle = 0.0f;  // Reset cycle when it completes
            }
        } else {
            stopWalking();
        }
    }

    void draw() {
        glPushMatrix();

        // The model's origin is its torso center. The simulation's `y` is the feet position.
        // We adjust the drawing position upwards so the feet land on `y`.
        const float model_feet_to_center_offset = 0.19f;
        glTranslatef(x, y + model_feet_to_center_offset * scale, 0.0f);
        glScalef(scale, scale, 1.0f);

        if (direction == 0) { // Flip for right-facing view
            glScalef(-1.0f, 1.0f, 1.0f);
        }

        float armAngle = 0.0f, legAngle = 0.0f, legLift = 0.0f;

        if (isWalking) {
            if (direction == 0 || direction == 1) { // Left/Right
                legAngle = sin(walkCycle) * 25.0f;
                armAngle = -sin(walkCycle) * 20.0f;
            } else { // Up/Down
                armAngle = sin(walkCycle) * 15.0f;
                legLift = std::max(0.0f, (float)sin(walkCycle * 2.0f)) * 0.03f;
            }
        }

        const float torsoTop = 0.1f, torsoBottom = -0.05f, torsoWidth = 0.08f;
        const float headRadius = 0.04f, neckY = torsoTop, headY = neckY + headRadius;
        const float shoulderY = torsoTop - 0.02f, shoulderX = torsoWidth / 2;
        const float hipY = torsoBottom, hipX = torsoWidth / 4;

        // --- ARMS ---
        glColor3f(skinColor.r, skinColor.g, skinColor.b);
        // Left Arm
        glPushMatrix();
        glTranslatef(shoulderX, shoulderY, 0.0f);
        glRotatef(-armAngle, 0, 0, 1);
        drawLine(0, 0, 0, -0.08f, 5.0f); // Explicitly set thickness
        glTranslatef(0, -0.08f, 0);
        glRotatef(-30.0f, 0, 0, 1);
        drawLine(0, 0, 0, -0.07f, 5.0f); // Explicitly set thickness
        drawCircle(0, -0.075f, 0.015f); // No change for hand circle
        glPopMatrix();

        // --- LEGS ---
        glPushMatrix();
        glColor3f(pantsColor.r, pantsColor.g, pantsColor.b);
        // Left Leg
        glPushMatrix();
        glTranslatef(hipX + 0.01f, hipY + 0.02f, 0.0f);
        if(direction == 0 || direction == 1) glRotatef(-legAngle, 0, 0, 1);
        else glTranslatef(0.0f, -legLift, 0.0f);
        drawLine(0, 0, 0, -0.13f, 5.0f); // Explicitly set thickness
        glTranslatef(0, -0.13f, 0);
        glRotatef(std::max(0.0f, -legAngle * 0.5f), 0, 0, 1);
        drawLine(0, 0, 0, -0.06f, 5.0f); // Explicitly set thickness
        glColor3f(0.1f, 0.1f, 0.1f);
        drawRect(-0.01f, -0.06f, 0.04f, 0.02f, -0.04f / 2.0f, -0.02f / 2.0f); // Corrected x-coordinate
        glPopMatrix();
        // Right Leg
        glPushMatrix();
        glColor3f(pantsColor.r, pantsColor.g, pantsColor.b);
        glTranslatef(-hipX - 0.01f, hipY + 0.02f, 0.0f);
        if(direction == 0 || direction == 1) glRotatef(legAngle, 0, 0, 1);
        else glTranslatef(0.0f, legLift, 0.0f);
        drawLine(0, 0, 0, -0.13f, 5.0f); // Explicitly set thickness
        glTranslatef(0, -0.13f, 0);
        glRotatef(std::max(0.0f, legAngle * 0.5f), 0, 0, 1);
        drawLine(0, 0, 0, -0.06f, 5.0f); // Explicitly set thickness
        glColor3f(0.1f, 0.1f, 0.1f);
        drawRect(-0.01f, -0.06f, 0.04f, 0.02f, -0.04f / 2.0f, -0.02f / 2.0f); // Corrected x-coordinate
        glPopMatrix();
        glPopMatrix();

        // --- TORSO ---
        glColor3f(shirtColor.r, shirtColor.g, shirtColor.b);
        drawRect(0.0f, (torsoTop + torsoBottom) / 2, torsoWidth, torsoTop - torsoBottom, -torsoWidth / 2.0f, -(torsoTop - torsoBottom) / 2.0f);

        // --- PANT TOP ---
        glColor3f(pantsColor.r, pantsColor.g, pantsColor.b);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, hipY + 0.01f);
        for(int i = 0; i <= 180; i += 10) {
            float angle = i * M_PI / 180.0f;
            float x = ((torsoWidth + 0.005)/2 - 0.002) * cos(angle);
            float y = hipY + 0.01f - ((torsoWidth + 0.005)/2 - 0.002) * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();

         // --- HEAD, FACE, and HAIR ---
        glColor3f(skinColor.r, skinColor.g, skinColor.b);
        drawCircle(0.0f, headY, headRadius, 20); // Explicitly set segments to 20

        glColor3f(hairColor.r, hairColor.g, hairColor.b);
        switch(hairStyle) {
            case HairStyle::Spiky:
                if (direction == 2) { // Back View
                    glBegin(GL_POLYGON);
                    glVertex2f(-headRadius * 0.9f, headY + headRadius * 0.3f); glVertex2f( headRadius * 0.9f, headY + headRadius * 0.3f);
                    glVertex2f( headRadius * 0.8f, headY + headRadius * 0.8f); glVertex2f(-headRadius * 0.8f, headY + headRadius * 0.8f);
                    glEnd();
                     for (int i = -1; i <= 1; ++i) {
                        float bx = i * 0.025f;
                        glBegin(GL_TRIANGLES);
                        glVertex2f(bx - 0.01f, headY + headRadius * 0.8f); glVertex2f(bx + 0.01f, headY + headRadius * 0.8f); glVertex2f(bx, headY + headRadius * 1.3f);
                        glEnd();
                    }
                    glBegin(GL_POLYGON);
                    glVertex2f(-headRadius * 0.9f, headY + headRadius * 0.4f); glVertex2f( headRadius * 0.9f, headY + headRadius * 0.4f);
                    glVertex2f( headRadius * 0.7f, headY + headRadius * -0.8f); glVertex2f(-headRadius * 0.7f, headY + headRadius * -0.8f);
                    glEnd();
                } else if (direction == 0 || direction == 1) { // Side View
                    glBegin(GL_POLYGON);
                    glVertex2f(headRadius * 0.3f, headY + headRadius * 0.8f); glVertex2f(headRadius * 0.85f, headY + headRadius * 0.8f);
                    glVertex2f(headRadius * 1.0f, headY - headRadius * 0.3f); glVertex2f(headRadius * 0.8f, headY - headRadius * 0.3f);
                    glEnd();
                    for (int i = -1; i <= 1; ++i) {
                        float bx = i * 0.025f;
                        glBegin(GL_TRIANGLES);
                        glVertex2f(bx - 0.01f, headY + headRadius * 0.8f); glVertex2f(bx + 0.01f, headY + headRadius * 0.8f); glVertex2f(bx, headY + headRadius * 1.3f);
                        glEnd();
                    }
                } else { // Front view (direction 3)
                    for (int i = -1; i <= 1; ++i) {
                        float bx = i * 0.025f;
                        glBegin(GL_TRIANGLES);
                        glVertex2f(bx - 0.01f, headY + headRadius * 0.8f); glVertex2f(bx + 0.01f, headY + headRadius * 0.8f); glVertex2f(bx, headY + headRadius * 1.3f);
                        glEnd();
                        glBegin(GL_QUADS);
                        glVertex2f(bx - 0.01f, headY + headRadius * 0.8f); glVertex2f(bx + 0.01f, headY + headRadius * 0.8f);
                        glVertex2f(bx + 0.01f, headY + headRadius * 0.6f); glVertex2f(bx - 0.01f, headY + headRadius * 0.6f);
                        glEnd();
                    }
                }
                break;

            case HairStyle::FlatTop:
                if(direction == 2){ // Back
                    drawRect(0.0f, headY + headRadius * 0.5f, headRadius * 2, headRadius, -(headRadius * 2) / 2.0f, -headRadius / 2.0f);
                    drawCircle(0.0f, headY, headRadius*0.95f, 20); // Explicitly set segments to 20
                } else { // Front and Side
                    drawRect(0.0f, headY + headRadius * 0.8f, headRadius * 1.8f, headRadius * 0.5f, -(headRadius * 1.8f) / 2.0f, -(headRadius * 0.5f) / 2.0f);
                    glBegin(GL_QUADS);
                    glVertex2f(-headRadius * 0.9f, headY + headRadius * 0.55f); glVertex2f( headRadius * 0.9f, headY + headRadius * 0.55f);
                    glVertex2f( headRadius * 1.0f, headY + headRadius * 0.5f); glVertex2f(-headRadius * 1.0f, headY + headRadius * 0.5f);
                    glEnd();
                    if (direction == 0 || direction == 1) {
                        glBegin(GL_POLYGON);
                        glVertex2f(headRadius * 0.3f, headY + headRadius * 0.8f); glVertex2f(headRadius * 0.85f, headY + headRadius * 0.8f);
                        glVertex2f(headRadius * 1.0f, headY - headRadius * 0.3f); glVertex2f(headRadius * 0.8f, headY - headRadius * 0.3f);
                        glEnd();
                    }
                }
                break;
            case HairStyle::Bald: break;
        }

        if (direction != 2) { // Eyes and face details (not for back view)
            glColor3f(0.0f, 0.0f, 0.0f);
            if (direction == 3) { // Front
                drawCircle(-0.012f, headY + 0.01f, 0.005f, 20); drawCircle( 0.012f, headY + 0.01f, 0.005f, 20); // Explicitly set segments to 20
                drawLine(-0.01f, headY - 0.015f, 0.01f, headY - 0.015f, 2.0f); // Explicitly set thickness
            } else { // Side
                drawCircle(-0.015f, headY + 0.005f, 0.004f, 20); // Explicitly set segments to 20
                glColor3f(skinColor.r, skinColor.g, skinColor.b);
                glBegin(GL_POLYGON);
                glVertex2f(-headRadius, headY); glVertex2f(-headRadius, headY - 0.01f); glVertex2f(-headRadius - 0.01f, headY - 0.005f);
                glEnd();
                glColor3f(0.0f, 0.0f, 0.0f);
                drawLine(-0.015f, headY - 0.01f, -0.03f, headY - 0.01f, 2.0f); // Explicitly set thickness
            }
        }

        // --- Right Arm ---
        glColor3f(skinColor.r, skinColor.g, skinColor.b);
        glPushMatrix();
        glTranslatef(-shoulderX, shoulderY, 0.0f);
        glRotatef(armAngle, 0, 0, 1);
        drawLine(0, 0, 0, -0.08f, 5.0f); // Explicitly set thickness
        glTranslatef(0, -0.08f, 0);
        glRotatef(-30.0f, 0, 0, 1);
        drawLine(0, 0, 0, -0.07f, 5.0f); // Explicitly set thickness
        drawCircle(0, -0.075f, 0.015f); // No change for hand circle
        glPopMatrix();
        glPopMatrix();

        // Draw yellow debug bounding box in world coordinates using getBounds()
        if (DEBUG_ON) {  // Removed !scenePaused check
            Rect b = getBounds();
            glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
            drawBound(b.x, b.y, 25, 45, -13, -5);
        }

        glPopMatrix();  // Restore original matrix
    }
};


// ===================================================================
//  Vehicle Class (replaces Car)
// ===================================================================
class Vehicle : public Drawable {
public:
    float r, g, b;
    float speedFactor;
    float current_speed;
    float target_speed;  // The speed we want to reach
    float acceleration_rate;  // How fast we accelerate
    float deceleration_rate;  // How fast we decelerate
    VehicleType type;
    bool isCurrentlyColliding = false;
    bool isHonking = false;
    int honkTimer = 0;
    int honkDuration = 0;
    const int HONK_INTERVAL = 120; // Honk every 2 seconds (assuming 60 FPS)
    const int HONK_DURATION_MAX = 45;  // How long the honk visual effect lasts
    bool isBlocked = false;  // Add this to track if vehicle is blocked

    Vehicle(float _x, float _y, float _w, float _h, float _r, float _g, float _b, VehicleType _type)
        : Drawable(_x, _y, _w, _h), r(_r), g(_g), b(_b),  // Remove the y offset from constructor
          speedFactor(1.0f + (rand() % 41) / 100.0f),
          current_speed(0.0f),
          target_speed(USER_CAR_SPEED_BASE * speedFactor),
          acceleration_rate(0.15f),
          deceleration_rate(0.25f),
          type(_type) {}

    virtual void draw() override = 0;
    virtual ~Vehicle() = default;

    void updateSpeed() {
        if (current_speed < target_speed) {
            // Accelerate
            current_speed = std::min(current_speed + acceleration_rate, target_speed);
        } else if (current_speed > target_speed) {
            // Decelerate
            current_speed = std::max(current_speed - deceleration_rate, target_speed);
        }
    }

    void updateHonk() {
        // Only honk if vehicle is both stopped and blocked
        if (current_speed < 0.1f && isBlocked) {
            if (honkTimer <= 0) {
                // Time to honk
                isHonking = true;
                honkTimer = HONK_INTERVAL;
                honkDuration = HONK_DURATION_MAX;
                // Play honk sound
                std::string soundToPlay;
                switch (type) {
                    case VehicleType::CAR: soundToPlay = "car"; break;
                    case VehicleType::TRUCK: soundToPlay = "truck"; break;
                    case VehicleType::BUS: soundToPlay = "bus"; break;
                    default: soundToPlay = "car";
                }
                audioManager.playSound(soundToPlay, false);
            } else {
                honkTimer--;
            }
        } else {
            // Reset honk timer and state when moving or not blocked
            honkTimer = HONK_INTERVAL;
            isHonking = false;
            honkDuration = 0;
        }

        // Update honk visual effect
        if (isHonking && honkDuration > 0) {
            honkDuration--;
            if (honkDuration <= 0) {
                isHonking = false;
            }
        }
    }

    bool update() {
        target_speed = USER_CAR_SPEED_BASE * speedFactor;  // Set target speed based on vehicle type

        bool stoppedByLight = false;
        if (trafficSignal->lightState == TrafficLightState::RED || trafficSignal->yellowLightOn) {
            if (x + width < CAR_STOP_LINE_X + 5.0f &&
                x + width > CAR_STOP_LINE_X - (DISTANCE_TO_STOP_FROM_SIGNAL + width)) {
                target_speed = 0;
                stoppedByLight = true;
                if (DEBUG_ON) {
                    debugCalls.push_back([=]() {
                        std::string debugText = "Stopped by Light at (" + std::to_string(x) + ", " + std::to_string(y) + ")";
                        drawText(x + width / 2, y - 20, debugText.c_str(), 0.5f);
                    });
                }
            }
        }

        bool blockedByVehicleAhead = false;
        if (!stoppedByLight) {
            for (const auto& v2 : vehicles) {
                if (this == v2.get()) continue;  // Skip self
                if (fabs(y - v2->y) < CAR_SAME_LANE_Y_THRESHOLD) {
                    if (v2->x > x) {
                        float distance = v2->x - (x + width);
                        float minSpacing = std::max(MIN_CAR_SPACING_AHEAD, width * 0.5f);
                        
                        if (distance < minSpacing) {
                            if (v2->current_speed < target_speed) {
                                target_speed = v2->current_speed;
                            }
                            if (distance < width * 0.25f || (v2->current_speed < 0.1f && distance < minSpacing * 0.75f)) {
                                target_speed = 0;
                                blockedByVehicleAhead = true;
                                if (DEBUG_ON) {
                                    debugCalls.push_back([=]() {
                                        std::string debugText = "Blocked by Vehicle Ahead at (" + std::to_string(v2->x) + ", " + std::to_string(v2->y) + ")";
                                        drawText(x + width / 2, y - 20, debugText.c_str(), 0.5f);
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }

        bool blockedByHuman = false;
        if (!stoppedByLight && !blockedByVehicleAhead) {
            Rect vehicleNextBounds = {x + current_speed, y, width, height};
            Rect crossingAreaCheck = {HUMAN_CROSSING_X_START - width / 2.0f, ROAD_Y_BOTTOM, HUMAN_CROSSING_WIDTH + width, ROAD_Y_TOP - ROAD_Y_BOTTOM};

            if (checkAABBCollision(vehicleNextBounds, crossingAreaCheck)) {
                for (const auto& ped : activeHumans) {
                    if (ped->state == HumanState::CROSSING_ROAD) {
                        if (checkAABBCollision(vehicleNextBounds, ped->getBounds())) {
                            blockedByHuman = true;
                            target_speed = 0;  // Set target speed to 0 instead of directly setting current_speed
                            //draw debug text whick caused block
                            if (DEBUG_ON) {  // Removed !scenePaused check
                                debugCalls.push_back([=]() {
                                    std::string debugText = "Blocked by Human at (" + std::to_string(ped->x) + ", " + std::to_string(ped->y) + ")";
                                    drawText(x + width / 2, y - 20, debugText.c_str(), 0.5f);
                                });
                            }
                            break;
                        }
                    }
                }
            }
        }
        
        bool collisionDetectedThisFrame = stoppedByLight || blockedByVehicleAhead || blockedByHuman;
        isBlocked = collisionDetectedThisFrame;  // Update isBlocked state

        if (collisionDetectedThisFrame) {
            target_speed = 0;
            if (!isCurrentlyColliding) {
                // Collision just started, play sound
                std::string soundToPlay;
                switch (type) {
                    case VehicleType::CAR: soundToPlay = "car"; break;
                    case VehicleType::TRUCK: soundToPlay = "truck"; break;
                    case VehicleType::BUS: soundToPlay = "bus"; break;
                    default: soundToPlay = "car";
                }
                audioManager.playSound(soundToPlay, false);
            }
        }
        
        isCurrentlyColliding = collisionDetectedThisFrame;

        updateSpeed();
        x += current_speed;
        updateHonk();

        return x > WINDOW_WIDTH + 100;
    }
};


// ===================================================================
//  Car, Truck, Bus Classes (inherit from Vehicle)
// ===================================================================
class Car : public Vehicle {
public:
    Car(float _x, float _y, float _w, float _h, float _r, float _g, float _b)
        : Vehicle(_x, _y, _w, _h, _r, _g, _b, VehicleType::CAR) {
        // Cars maintain normal speed (speedFactor is already set in base class)
    }

    void draw() override {
        drawModernCar(x, y, width, height, r, g, b, isNight, current_speed, isHonking);
        if (DEBUG_ON) {  // Removed !scenePaused check
            Rect b = getBounds();
            glColor3f(0.0f, 1.0f, 0.0f);
            drawBound(b.x, b.y, b.w, b.h);
        }
    }
};

class Truck : public Vehicle {
public:
    Truck(float _x, float _y, float _w, float _h, float _r, float _g, float _b)
        : Vehicle(_x, _y, _w, _h, _r, _g, _b, VehicleType::TRUCK) {
        // Trucks are slower and have slower acceleration/deceleration
        speedFactor *= 0.9f;
        target_speed = USER_CAR_SPEED_BASE * speedFactor;
        acceleration_rate *= 0.7f;  // Trucks accelerate slower
        deceleration_rate *= 0.7f;  // Trucks decelerate slower
    }

    void draw() override {
        drawTruck(x, y, width, height, r, g, b, isNight, current_speed, isHonking);
        if (DEBUG_ON) {  // Removed !scenePaused check
            Rect b = getBounds();
            glColor3f(0.0f, 1.0f, 0.0f);
            drawBound(b.x, b.y, b.w, b.h);
        }
    }
};

class Bus : public Vehicle {
public:
    Bus(float _x, float _y, float _w, float _h, float _r, float _g, float _b)
        : Vehicle(_x, _y, _w, _h, _r, _g, _b, VehicleType::BUS) {
        // Buses have even slower acceleration/deceleration
        acceleration_rate *= 0.6f;  // Buses accelerate even slower
        deceleration_rate *= 0.6f;  // Buses decelerate even slower
    }

    void draw() override {
        drawBus(x, y, width, height, r, g, b, isNight, current_speed, isHonking);
        if (DEBUG_ON) {  // Removed !scenePaused check
            Rect b = getBounds();
            glColor3f(0.0f, 1.0f, 0.0f);
            drawBound(b.x, b.y, b.w, b.h);
        }
    }
};

unsigned short HumansWaitingToCross() {
    unsigned short count = 0;
    for (const auto& human : activeHumans) {
        if (human->state == HumanState::WAITING_AT_CROSSING_EDGE) {
            count++;
        }
    }
    return count;
}

unsigned short HumansCrossing() {
    unsigned short count = 0;
    for (const auto& human : activeHumans) {
        if (human->state == HumanState::CROSSING_ROAD) {
            count++;
        }
    }
    return count;
}


// Add these new functions before the display() function
void drawSun(float x, float y, float radius) {
    // Calculate sunset effect (more reddish during sunset)
    float sunsetFactor = 0.0f;
    if (currentTimeOfDay >= 0.6f && currentTimeOfDay < 0.8f) {
        // During sunset (0.6 to 0.8), gradually increase redness but cap it
        sunsetFactor = std::min((currentTimeOfDay - 0.6f) / 0.2f, 0.5f);  // Cap at 0.5 to prevent too much red
    }
    
    // Base colors
    float r = 1.0f;  // Red component
    float g = 0.9f - (0.3f * sunsetFactor);  // Green component decreases during sunset
    float b = 0.2f - (0.2f * sunsetFactor);  // Blue component decreases during sunset

    // Draw outer glow
    glColor4f(r, g, b, 0.3f);
    drawCircle(x, y, radius * 1.5f);

    // Draw sun body
    glColor3f(r, g, b);
    drawCircle(x, y, radius);
}

void drawMoon(float x, float y, float radius) {
    // Draw outer glow
    glColor4f(0.9f, 0.9f, 0.95f, 0.3f);
    drawCircle(x, y, radius * 1.5f);

    // Draw moon body
    glColor3f(0.9f, 0.9f, 0.95f);
    drawCircle(x, y, radius);

    // Draw craters
    glColor3f(0.8f, 0.8f, 0.85f);
    drawCircle(x - radius * 0.3f, y + radius * 0.2f, radius * 0.15f);
    drawCircle(x + radius * 0.2f, y - radius * 0.3f, radius * 0.1f);
    drawCircle(x + radius * 0.1f, y + radius * 0.1f, radius * 0.08f);
}


void drawCloud(float x, float y, float scale) {
    glColor3f(0.9f, 0.9f, 0.95f);
    float r = 15.0f * scale;
    drawCircle(x, y, r);
    drawCircle(x + r * 0.8f, y, r * 0.9f);
    drawCircle(x - r * 0.8f, y, r * 0.9f);
    drawCircle(x + r * 0.4f, y + r * 0.3f, r * 0.8f);
    drawCircle(x - r * 0.4f, y + r * 0.3f, r * 0.8f);
}

void drawStars() {
    // Stars should be visible during night (currentTimeOfDay > 0.8 or < 0.2)
    // and fade in/out during transitions
    float starAlpha = 0.0f;

    if (currentTimeOfDay >= 0.85f) {  // Adjusted from 0.7
        // Night time - full visibility
        starAlpha = 1.0f;
    } else if (currentTimeOfDay >= 0.80f) {  // Adjusted from 0.6
        // Smoother transition to night
        starAlpha = (currentTimeOfDay - 0.80f) / 0.05f;
    } else if (currentTimeOfDay <= 0.15f) {  // Adjusted from 0.2
        // Night time - full visibility
        starAlpha = 1.0f;
    } else if (currentTimeOfDay <= 0.20f) {  // Adjusted from 0.3
        // Smoother transition to day
        starAlpha = (0.20f - currentTimeOfDay) / 0.05f;
    }

    if (starAlpha > 0.0f) {
        for (auto& star : stars) {
            // Update blink phase
            star.blinkPhase += 0.05f;
            if (star.blinkPhase > 628.0f) star.blinkPhase -= 628.0f;

            // Calculate blinking brightness
            float blinkFactor = 0.5f + 0.5f * sin(star.blinkPhase);
            float finalBrightness = star.baseBrightness * blinkFactor;

            glColor4f(1.0f, 1.0f, 0.9f, starAlpha * finalBrightness);
            glPointSize(star.size);
            glBegin(GL_POINTS);
            glVertex2f(star.x, star.y);
            glEnd();
        }
    }
}

class Building : public Drawable {
    public:
        Building(float _x, float _y, float _width, float _height) 
            : Drawable(_x, _y, _width, _height) {}
        virtual void draw() = 0;
};
class ModernBuilding : public Building {
    public:
        ModernBuilding(float _x, float _y, float _width, float _height) 
            : Building(_x, _y, _width, _height) {}
        void draw() override {
            // Main building
            glColor3f(0.7f, 0.7f, 0.75f);
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + width, y);
            glVertex2f(x + width, y + height);
            glVertex2f(x, y + height);
            glEnd();

            // Windows
            glColor3f(0.9f, 0.9f, 0.95f);
            float windowWidth = width * 0.15f;
            float windowHeight = height * 0.1f;
            float windowSpacing = width * 0.2f;
            float windowRows = height * 0.15f;

            for (float wx = x + windowSpacing; wx < x + width - windowWidth; wx += windowSpacing) {
                for (float wy = y + windowRows; wy < y + height - windowRows; wy += windowRows) {
                    glBegin(GL_QUADS);
                    glVertex2f(wx, wy);
                    glVertex2f(wx + windowWidth, wy);
                    glVertex2f(wx + windowWidth, wy + windowHeight);
                    glVertex2f(wx, wy + windowHeight);
                    glEnd();
                }
            }
        }
};

class ClassicBuilding : public Building {
    public:
        ClassicBuilding(float _x, float _y, float _width, float _height) 
            : Building(_x, _y, _width, _height) {}

        void draw() override {
            // Main building
            glColor3f(0.8f, 0.6f, 0.5f);
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + width, y);
            glVertex2f(x + width, y + height);
            glVertex2f(x, y + height);
            glEnd();

            // Roof
            glColor3f(0.5f, 0.3f, 0.2f);
            glBegin(GL_TRIANGLES);
            glVertex2f(x - width * 0.1f, y + height);
            glVertex2f(x + width * 0.5f, y + height + height * 0.2f);
            glVertex2f(x + width + width * 0.1f, y + height);
            glEnd();

            // Windows
            glColor3f(0.9f, 0.9f, 0.7f);
            float windowWidth = width * 0.2f;
            float windowHeight = height * 0.15f;
            float windowSpacing = width * 0.3f;
            float windowRows = height * 0.2f;

            for (float wx = x + windowSpacing; wx < x + width - windowWidth; wx += windowSpacing) {
                for (float wy = y + windowRows; wy < y + height - windowRows; wy += windowRows) {
                    glBegin(GL_QUADS);
                    glVertex2f(wx, wy);
                    glVertex2f(wx + windowWidth, wy);
                    glVertex2f(wx + windowWidth, wy + windowHeight);
                    glVertex2f(wx, wy + windowHeight);
                    glEnd();
                }
            }
        }
};

class SkyScraper : public Building {
    public:
        SkyScraper(float _x, float _y, float _width, float _height) 
            : Building(_x, _y, _width, _height) {}


        void draw() {
            // Main building
            glColor3f(0.6f, 0.7f, 0.8f);
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + width, y);
            glVertex2f(x + width, y + height);
            glVertex2f(x, y + height);
            glEnd();

            // Glass windows
            glColor3f(0.8f, 0.9f, 1.0f);
            float windowWidth = width * 0.1f;
            float windowHeight = height * 0.05f;
            float windowSpacing = width * 0.15f;
            float windowRows = height * 0.08f;

            for (float wx = x + windowSpacing; wx < x + width - windowWidth; wx += windowSpacing) {
                for (float wy = y + windowRows; wy < y + height - windowRows; wy += windowRows) {
                    glBegin(GL_QUADS);
                    glVertex2f(wx, wy);
                    glVertex2f(wx + windowWidth, wy);
                    glVertex2f(wx + windowWidth, wy + windowHeight);
                    glVertex2f(wx, wy + windowHeight);
                    glEnd();
                }
            }

            // Antenna
            glColor3f(0.3f, 0.3f, 0.3f);
            glBegin(GL_QUADS);
            glVertex2f(x + width * 0.45f, y + height);
            glVertex2f(x + width * 0.55f, y + height);
            glVertex2f(x + width * 0.55f, y + height + height * 0.1f);
            glVertex2f(x + width * 0.45f, y + height + height * 0.1f);
            glEnd();
        }
};


void drawGrass(float x, float y, float scale = 1.0f) {
    glColor3f(0.2f, 0.7f, 0.2f);
    glBegin(GL_LINES);
    for (int i = -2; i <= 2; ++i) {
        glVertex2f(x, y);
        glVertex2f(x + i * 2 * scale, y + 12 * scale - abs(i) * 2 * scale);
    }
    glEnd();
}

void drawFlower(float x, float y, float scale = 1.0f) {
    // Stem
    glColor3f(0.2f, 0.7f, 0.2f);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x, y + 10 * scale);
    glEnd();
    // Petals
    glColor3f(1.0f, 0.7f, 0.2f); // yellow/orange
    for (int i = 0; i < 5; ++i) {
        float angle = i * 2 * 3.14159f / 5;
        float px = x + cosf(angle) * 4 * scale;
        float py = y + 10 * scale + sinf(angle) * 4 * scale;
        drawCircle(px, py, 2.0f * scale, 8);
    }
    // Center
    glColor3f(1.0f, 0.2f, 0.5f); // pink center
    drawCircle(x, y + 10 * scale, 2.2f * scale, 10);
}

class Tree : public Drawable {
public:
    Tree(float x, float y, float w, float h) : Drawable(x, y, w, h) {}
    virtual void draw() = 0;
    virtual ~Tree() = default;
};

class BasicTree : public Tree {
public:
    BasicTree(float x, float y) : Tree(x, y, 40, 80) {}
    void draw() override {
        glColor3f(0.5f, 0.3f, 0.05f);
        glBegin(GL_QUADS);
        glVertex2f(x - 8, y);  // Reduced from 12
        glVertex2f(x + 8, y);
        glVertex2f(x + 6, y + 50);  // Reduced from 70
        glVertex2f(x - 6, y + 50);
        glEnd();

        glColor3f(0.0f, 0.45f, 0.05f);
        drawCircle(x, y + 75, 25);  // Reduced from 35
        drawCircle(x - 15, y + 65, 20);  // Reduced from 30
        drawCircle(x + 15, y + 65, 20);  // Reduced from 30
        glColor3f(0.1f, 0.55f, 0.1f);
        drawCircle(x, y + 80, 18);  // Reduced from 25
    }
};

class PineTree : public Tree {
public:
    PineTree(float x, float y) : Tree(x, y, 50, 130) {}
    void draw() override {
        // Trunk
        glColor3f(0.5f, 0.3f, 0.05f);
        glBegin(GL_QUADS);
        glVertex2f(x - 6, y);  // Reduced from 8
        glVertex2f(x + 6, y);
        glVertex2f(x + 4, y + 60);  // Reduced from 80
        glVertex2f(x - 4, y + 60);
        glEnd();

        // Pine needles - multiple layers with varying sizes
        glColor3f(0.0f, 0.4f, 0.1f);
        // Bottom layer - widest
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 35, y + 60);  // Reduced from 45
        glVertex2f(x + 35, y + 60);
        glVertex2f(x, y + 85);  // Reduced from 110
        glEnd();
        // Second layer
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 30, y + 75);  // Reduced from 40
        glVertex2f(x + 30, y + 75);
        glVertex2f(x, y + 100);  // Reduced from 130
        glEnd();
        // Third layer
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 25, y + 90);  // Reduced from 35
        glVertex2f(x + 25, y + 90);
        glVertex2f(x, y + 115);  // Reduced from 150
        glEnd();
        // Fourth layer
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 20, y + 105);  // Reduced from 30
        glVertex2f(x + 20, y + 105);
        glVertex2f(x, y + 130);  // Reduced from 170
        glEnd();
        // Top layer - smallest
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 15, y + 120);  // Reduced from 25
        glVertex2f(x + 15, y + 120);
        glVertex2f(x, y + 145);  // Reduced from 190
        glEnd();
        // Add some darker green details
        glColor3f(0.0f, 0.35f, 0.05f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 25, y + 70);  // Reduced from 35
        glVertex2f(x + 25, y + 70);
        glVertex2f(x, y + 95);  // Reduced from 120
        glEnd();
    }
};

class MapleTree : public Tree {
public:
    MapleTree(float x, float y) : Tree(x, y, 70, 120) {}
    void draw() override {
        // Trunk
        glColor3f(0.5f, 0.3f, 0.05f);
        glBegin(GL_QUADS);
        glVertex2f(x - 10, y);
        glVertex2f(x + 10, y);
        glVertex2f(x + 8, y + 80);
        glVertex2f(x - 8, y + 80);
        glEnd();
        // Main branches
        glBegin(GL_QUADS);
        glVertex2f(x - 8, y + 60);
        glVertex2f(x + 8, y + 60);
        glVertex2f(x + 6, y + 80);
        glVertex2f(x - 6, y + 80);
        glEnd();
        // Maple leaves - multiple layers
        glColor3f(0.0f, 0.5f, 0.1f);
        for (int i = 0; i < 3; i++) {
            float yy = y + 80 + i * 20;
            float radius = 35 - i * 5;
            drawCircle(x, yy, radius);
        }
    }
};

class StreetLamp : public Drawable {
public:
    StreetLamp(float x, float y) : Drawable(x, y, 36, 120) {}
    void draw() override {
        glPushMatrix();
        glTranslatef(x, y, 0);

        // Draw the pole with a gradient effect
        glBegin(GL_QUADS);
        // Darker shade at the bottom
        glColor3f(0.3f, 0.3f, 0.3f);
        glVertex2f(-4, 0);
        glVertex2f(4, 0);
        // Lighter shade at the top
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex2f(4, 120);
        glVertex2f(-4, 120);
        glEnd();

        // Draw decorative rings around the pole
        glColor3f(0.4f, 0.4f, 0.4f);
        for (int i = 0; i < 3; i++) {
            float y_pos = 30 + i * 30;
            drawRect(-5, y_pos, 10, 2);
        }

        // Draw the arm with a modern curved design
        glColor3f(0.4f, 0.4f, 0.4f);
        // Main arm
        glBegin(GL_QUADS);
        glVertex2f(0, 115);
        glVertex2f(35, 115);
        glVertex2f(35, 110);
        glVertex2f(0, 110);
        glEnd();

        // Curved end of the arm
        glBegin(GL_QUADS);
        glVertex2f(35, 115);
        glVertex2f(40, 110);
        glVertex2f(40, 105);
        glVertex2f(35, 110);
        glEnd();

        // Draw the lamp housing
        glColor3f(0.2f, 0.2f, 0.2f);
        // Main housing
        glBegin(GL_QUADS);
        glVertex2f(35, 105);
        glVertex2f(45, 105);
        glVertex2f(45, 95);
        glVertex2f(35, 95);
        glEnd();

        // Bottom part of housing
        glBegin(GL_QUADS);
        glVertex2f(38, 95);
        glVertex2f(42, 95);
        glVertex2f(42, 90);
        glVertex2f(38, 90);
        glEnd();

        if (isNight) {
            // Draw the light source with multiple layers of glow
            // Outer glow (largest, most transparent)
            glColor4f(1.0f, 0.95f, 0.8f, 0.15f);
            drawCircle(40, 100, 25);

            // Middle glow
            glColor4f(1.0f, 0.95f, 0.8f, 0.3f);
            drawCircle(40, 100, 15);

            // Inner glow (brightest)
            glColor4f(1.0f, 0.95f, 0.8f, 0.5f);
            drawCircle(40, 100, 8);

            // Light source
            glColor3f(1.0f, 0.95f, 0.8f);
            drawCircle(40, 100, 5);

            // Draw light beam effect
            glColor4f(1.0f, 0.95f, 0.8f, 0.2f);
            glBegin(GL_TRIANGLES);
            glVertex2f(40, 90);  // Start from bottom of lamp
            glVertex2f(20, 0);  // Left edge of beam, moved down
            glVertex2f(60, 0);  // Right edge of beam, moved down
            glEnd();

            // Add some light reflection on the ground
            glColor4f(1.0f, 0.95f, 0.8f, 0.1f);
            drawCircle(40, 0, 25); // Adjusted y and radius for larger reflection
        } else {
            // Draw the lamp when off
            glColor3f(0.6f, 0.6f, 0.5f);
            drawCircle(40, 100, 5);
        }

        // Add some decorative details
        glColor3f(0.3f, 0.3f, 0.3f);
        // Small decorative elements on the arm
        for (int i = 0; i < 3; i++) {
            float x_pos = 10 + i * 10;
            drawRect(x_pos, 112, 2, 6);
        }

        glPopMatrix();
    }
};

void spawnNewVehicle() {
    float spawnW, spawnH;
    VehicleType type;

    // Randomly choose a vehicle type
    int typeRoll = rand() % 20; // 0-5: Car,  6-7: Truck, 8-9: Bus
    if (typeRoll < 10) {
        type = VehicleType::CAR;
        spawnW = 80.0f;  // Increased from 60
        spawnH = 35.0f;  // Increased from 28
    } else if (typeRoll < 13) {
        type = VehicleType::TRUCK;
        spawnW = 150.0f; // Increased for larger trucks
        spawnH = 60.0f;  // Increased for larger trucks
    } else {
        type = VehicleType::BUS;
        spawnW = 160.0f; // Increased from 120
        spawnH = 50.0f;  // Increased from 40
    }

    float spawnX = -spawnW - 150.0f; // Start further back
    float spawnY;

    if (rand() % 2 == 0) {
        spawnY = ROAD_Y_BOTTOM + 8.0f; // Bottom lane
    } else {
        spawnY = ROAD_Y_TOP - 8.0f - spawnH; // Top lane
    }

    // Sort vehicles by lanes to prevent overlap
    std::sort(vehicles.begin(), vehicles.end(), 
        [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
            return a->y < b->y;  // Sort by y-coordinate (lane position)
        });

    // Check for vehicle in the same lane to ensure proper spacing
    for (const auto &v : vehicles) {
        if (fabs(v->y - spawnY) < CAR_SAME_LANE_Y_THRESHOLD) {
            if (v->x < spawnX + MIN_CAR_SPAWN_DISTANCE) {
                // This check is a bit tricky, but we avoid spawning right on top
                return; // Just skip spawning this frame if it's too crowded
            }
        }
    }

    // Create a new vehicle
    std::shared_ptr<Vehicle> newVehicle;
    if (type == VehicleType::CAR) {
        newVehicle = std::make_shared<Car>(spawnX, spawnY, spawnW, spawnH,
            (rand() % 10) / 10.0f,
            (rand() % 10) / 10.0f,
            (rand() % 10) / 10.0f);
    } else if (type == VehicleType::TRUCK) {
        newVehicle = std::make_shared<Truck>(spawnX, spawnY, spawnW, spawnH,
            (rand() % 10) / 10.0f,
            (rand() % 10) / 10.0f,
            (rand() % 10) / 10.0f);
    } else {
        newVehicle = std::make_shared<Bus>(spawnX, spawnY, spawnW, spawnH,
            (rand() % 10) / 10.0f,
            (rand() % 10) / 10.0f,
            (rand() % 10) / 10.0f);
    }

    if (newVehicle->r < 0.1f && newVehicle->g < 0.1f && newVehicle->b < 0.1f) {
        newVehicle->r = 0.2f;
        newVehicle->g = 0.2f;
        newVehicle->b = 0.2f;
    }

    vehicles.push_back(newVehicle);
}


void updateVehicles() {
    for (size_t i = 0; i < vehicles.size(); ++i) {
        if (vehicles[i]->update()) {
            vehicles.erase(vehicles.begin() + i);
            i--; // Decrement i to account for the erased element
        }
    }

    // Spawn new vehicles periodically
    if (vehicles.size() < 8 && rand() % CAR_SPAWN_RATE == 0) {
        spawnNewVehicle();
    }
}


void drawBackgroundScenes() {
        // Draw celestial bodies with smooth transitions
    float sunX = WINDOW_WIDTH * 0.3f;  // Sun on the left
    float moonX = WINDOW_WIDTH * 0.8f; // Moon on the right
    float celestialY = WINDOW_HEIGHT * 1.2f; // Start sun above the window

    if (currentTimeOfDay >= 0.05 && currentTimeOfDay < 0.80) {
        // Day time - sun visible
        // Sun starts high (noon) and moves down
        float sunY = celestialY - (currentTimeOfDay - 0.2f) * WINDOW_HEIGHT * 1.4f;
        drawSun(sunX, sunY, 30.0f);
    }

    if (currentTimeOfDay >= 0.70 || currentTimeOfDay < 0.20) {
        // Night time - moon visible
        // Moon rises from bottom
        float moonY = (WINDOW_HEIGHT/3) + (currentTimeOfDay > 0.5f ?
            (currentTimeOfDay - 0.7f) : (currentTimeOfDay + 0.3f)) * WINDOW_HEIGHT * 1.5;
        drawMoon(moonX, moonY, 25.0f);
    }

    // Draw clouds
    for (const auto& cloud : clouds) {
        drawCloud(cloud.x, cloud.y, cloud.scale);
    }

    // Draw stars
    drawStars();
}


void drawGround() {
    glColor3f(0.35f, 0.7f, 0.25f); // green ground
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, SIDEWALK_BOTTOM_Y_START);
    glVertex2f(0, SIDEWALK_BOTTOM_Y_START);
    glEnd();

    // Draw grass clumps (deterministic, evenly spaced)
    for (int i = 0; i < 25; ++i) {
        float gx = 20 + i * 38;
        float gy = 10 + (i * 13) % (int)(SIDEWALK_BOTTOM_Y_START * 0.5f);
        float scale = 0.8f + ((i * 7) % 10) / 20.0f;
        drawGrass(gx, gy, scale);
    }

    // Draw flowers (deterministic, less frequent)
    for (int i = 0; i < 10; ++i) {
        float fx = 35 + i * 90;
        float fy = 18 + (i * 23) % (int)(SIDEWALK_BOTTOM_Y_START * 0.5f);
        float scale = 0.8f + ((i * 5) % 10) / 20.0f;
        drawFlower(fx, fy, scale);
    }
}


void drawRoadAndSidewalks()
{
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    glVertex2f(0, ROAD_Y_BOTTOM);
    glVertex2f(WINDOW_WIDTH, ROAD_Y_BOTTOM);
    glVertex2f(WINDOW_WIDTH, ROAD_Y_TOP);
    glVertex2f(0, ROAD_Y_TOP);
    glEnd();
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_QUADS);
    glVertex2f(0, SIDEWALK_TOP_Y_START);
    glVertex2f(WINDOW_WIDTH, SIDEWALK_TOP_Y_START);
    glVertex2f(WINDOW_WIDTH, SIDEWALK_TOP_Y_END);
    glVertex2f(0, SIDEWALK_TOP_Y_END);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0, SIDEWALK_BOTTOM_Y_START);
    glVertex2f(WINDOW_WIDTH, SIDEWALK_BOTTOM_Y_START);
    glVertex2f(WINDOW_WIDTH, SIDEWALK_BOTTOM_Y_END);
    glVertex2f(0, SIDEWALK_BOTTOM_Y_END);
    glEnd();
    glColor3f(0.9f, 0.9f, 0.9f);
    float road_center_y = (ROAD_Y_BOTTOM + ROAD_Y_TOP) / 2.0f;
    for (float i = 0; i < WINDOW_WIDTH; i += 60)
    {
        glBegin(GL_QUADS);
        glVertex2f(i, road_center_y - 1);
        glVertex2f(i + 30, road_center_y - 1);
        glVertex2f(i + 30, road_center_y + 1);
        glVertex2f(i, road_center_y + 1);
        glEnd();
    }
}

int countCarsNearCrossing() {
    int count = 0;
    for (const auto& v : vehicles) {
        if (v->x < TRAFFIC_LIGHT_X && v->x > TRAFFIC_LIGHT_X - CAR_PRIORITY_THRESHOLD) {
            count++;
        }
    }
    return count;
}

void drawZebraCrossing(float road_y_bottom, float road_y_top, float crossing_area_x_start, float crossing_area_width)
{
    glColor3f(0.95f, 0.95f, 0.95f);
    float offset = -5.0f;
    //draw 10 stripes on x axis
    const int stripCount = 10;
    const float stripe_height = (road_y_top - road_y_bottom) / stripCount;
    for (int i = 0; i < stripCount; i++)
    {
        if (i % 2 == 0) continue;
        glBegin(GL_QUADS);
        glVertex2f(crossing_area_x_start, (road_y_bottom + i * stripe_height) + offset);
        glVertex2f(crossing_area_x_start + crossing_area_width, (road_y_bottom + i * stripe_height) + offset);
        glVertex2f(crossing_area_x_start + crossing_area_width, (road_y_bottom + (i + 1) * stripe_height) + offset);
        glVertex2f(crossing_area_x_start, (road_y_bottom + (i + 1) * stripe_height) + offset);
        glEnd();
    }

    // Draw debug bounding box for zebra crossing area
    if (DEBUG_ON) {  // Removed !scenePaused check
        // Main crossing area
        glColor3f(1.0f, 0.0f, 1.0f);  // Magenta color for zebra crossing debug box
        drawBound(crossing_area_x_start, road_y_bottom, crossing_area_width, road_y_top - road_y_bottom);
        // Label for main crossing area
        glColor3f(1.0f, 0.0f, 1.0f);
        debugCalls.push_back([=]() {
            drawText(crossing_area_x_start, road_y_top + 10, "Main Crossing Area", 0.5f);
        });
        // Show human count
        char humanCountText[64];
        snprintf(humanCountText, sizeof(humanCountText), "Humans waiting: %d", HumansWaitingToCross());
        debugCalls.push_back([=]() {
            drawText(crossing_area_x_start, road_y_top + 25, humanCountText, 0.5f);
        });

        // Extended waiting area (20 units on each side)
        glColor3f(1.0f, 0.5f, 1.0f);  // Lighter magenta for extended area
        drawBound(crossing_area_x_start - DISTANCE_TO_STOP_FROM_SIGNAL, road_y_bottom, crossing_area_width + DISTANCE_TO_STOP_FROM_SIGNAL * 2, road_y_top - road_y_bottom);
        // Label for extended waiting area
        glColor3f(1.0f, 0.5f, 1.0f);
        debugCalls.push_back([=]() {
            drawText(crossing_area_x_start - DISTANCE_TO_STOP_FROM_SIGNAL, road_y_top + 40, "Extended Waiting Area", 0.5f);
        });

        // Car priority area
        glColor3f(0.0f, 1.0f, 0.5f);  // Light green for car priority area
        drawBound(crossing_area_x_start - CAR_PRIORITY_THRESHOLD, road_y_bottom, CAR_PRIORITY_THRESHOLD, road_y_top - road_y_bottom);
        // Label for car priority area
        glColor3f(0.0f, 1.0f, 0.5f);
        debugCalls.push_back([=]() {
            drawText(crossing_area_x_start - CAR_PRIORITY_THRESHOLD, road_y_top + 55, "Car Priority Area", 0.5f);
        });
        // Show car count
        char carCountText[64];
        snprintf(carCountText, sizeof(carCountText), "Cars in area: %d", countCarsNearCrossing());
        glColor3f(0.0f, 1.0f, 0.5f);
        debugCalls.push_back([=]() {
            drawText(crossing_area_x_start - CAR_PRIORITY_THRESHOLD, road_y_top + 70, carCountText, 0.5f);
        });
    }
}



void drawSceneObjects() {
    // Clear previous objects
    drawableObjects.clear();
    
    // Create and add buildings
    drawableObjects.push_back(std::make_shared<ModernBuilding>(60.0f, SIDEWALK_TOP_Y_END, 80.0f, 120.0f));
    drawableObjects.push_back(std::make_shared<ClassicBuilding>(180.0f, SIDEWALK_TOP_Y_END, 100.0f, 100.0f));
    drawableObjects.push_back(std::make_shared<SkyScraper>(340.0f, SIDEWALK_TOP_Y_END, 60.0f, 180.0f));
    drawableObjects.push_back(std::make_shared<ModernBuilding>(460.0f, SIDEWALK_TOP_Y_END, 90.0f, 140.0f));
    drawableObjects.push_back(std::make_shared<ClassicBuilding>(620.0f, SIDEWALK_TOP_Y_END, 110.0f, 110.0f));
    drawableObjects.push_back(std::make_shared<SkyScraper>(840.0f, SIDEWALK_TOP_Y_END, 70.0f, 200.0f));

    // Create and add vehicles
    for (const auto& vehicle : vehicles) {
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(vehicle));
    }

    // Add trees as objects (replace drawTree calls)
    drawableObjects.push_back(std::make_shared<BasicTree>(180, SIDEWALK_TOP_Y_START + 28));  // -2 from original
    drawableObjects.push_back(std::make_shared<PineTree>(500, SIDEWALK_TOP_Y_START + 28));   // -2 from original
    drawableObjects.push_back(std::make_shared<PineTree>(700, SIDEWALK_TOP_Y_START + 28));   // -2 from original
    drawableObjects.push_back(std::make_shared<BasicTree>(850, SIDEWALK_BOTTOM_Y_START - 2)); // -2 from original
    drawableObjects.push_back(std::make_shared<PineTree>(300, SIDEWALK_BOTTOM_Y_START - 2));  // -2 from original

    // Add street lamps as objects
    const int numLamps = 3; // Define N here, you can change this value
    float spacing;
    float startX;

    if (numLamps > 1) {
        // Spread lamps over 80% of the window width, centered.
        float effectiveSpreadWidth = WINDOW_WIDTH * 0.8f; 
        spacing = effectiveSpreadWidth / (numLamps - 1.0f);
        startX = (WINDOW_WIDTH / 2.0f) - (effectiveSpreadWidth / 2.0f);

    } else { // numLamps == 1
        spacing = 0.0f; // No spacing needed for a single lamp
        startX = WINDOW_WIDTH / 2.0f; // Place single lamp in the center
    }

    // Upper sidewalk lamps
    for (int i = 0; i < numLamps; ++i) {
        float lampX = startX + i * spacing;
        drawableObjects.push_back(std::make_shared<StreetLamp>(lampX, SIDEWALK_TOP_Y_START));
    }

    // Lower sidewalk lamps with a phase shift
    const float phaseShift = 50.0f; // The amount of phase shift
    for (int i = 0; i < numLamps; ++i) {
        float lampX = startX + i * spacing + phaseShift;
        drawableObjects.push_back(std::make_shared<StreetLamp>(lampX, SIDEWALK_BOTTOM_Y_END));
    }

    // Add traffic signal as an object
    drawableObjects.push_back(trafficSignal);

    // Add humans to drawable objects
    for (const auto& human : activeHumans) {
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(human));
    }

    //apply y-sorting
    std::sort(drawableObjects.begin(), drawableObjects.end(), [](const auto& a, const auto& b) {
        return a->y > b->y;
    });

    // Draw all objects
    for (const auto& obj : drawableObjects) {
        obj->draw();
    }
}


void transition() {
    // Base colors for different times of day
    float dayR = 0.5f, dayG = 0.7f, dayB = 1.0f;    // Bright blue sky
    float dawnR = 0.6f, dawnG = 0.5f, dawnB = 0.4f;  // Soft orange dawn
    float nightR = 0.1f, nightG = 0.1f, nightB = 0.3f;  // Dark blue night
    float duskR = 0.6f, duskG = 0.5f, duskB = 0.4f;  // Soft orange dusk
    float r, g, b;

    // Time windows for each phase
    float dawnStart = 0.0f;
    float dawnEnd = 0.2f;
    float dayStart = 0.2f;
    float dayEnd = 0.45f;
    float duskStart = 0.45f;
    float duskEnd = 0.7f;
    float nightStart = 0.7f;
    float nightEnd = 1.0f;

    // Transition periods
    float transitionPeriod = 0.15f;

    // Calculate color based on time of day
    if (currentTimeOfDay >= dawnStart && currentTimeOfDay < dawnEnd) {
        // Dawn transition (night to day)
        float t = (currentTimeOfDay - dawnStart) / transitionPeriod;
        if (t > 1.0f) t = 1.0f;
        r = nightR + (dawnR - nightR) * t;
        g = nightG + (dawnG - nightG) * t;
        b = nightB + (dawnB - nightB) * t;
    }
    else if (currentTimeOfDay >= dayStart && currentTimeOfDay < dayEnd) {
        // Day transition (dawn to day)
        float t = (currentTimeOfDay - dayStart) / transitionPeriod;
        if (t > 1.0f) t = 1.0f;
        r = dawnR + (dayR - dawnR) * t;
        g = dawnG + (dayG - dawnG) * t;
        b = dawnB + (dayB - dawnB) * t;
    }
    else if (currentTimeOfDay >= duskStart && currentTimeOfDay < duskEnd) {
        // Dusk transition (day to dusk)
        float t = (currentTimeOfDay - duskStart) / transitionPeriod;
        if (t > 1.0f) t = 1.0f;
        r = dayR + (duskR - dayR) * t;
        g = dayG + (duskG - dayG) * t;
        b = dayB + (duskB - dayB) * t;
    }
    else if (currentTimeOfDay >= nightStart && currentTimeOfDay < nightEnd) {
        // Night transition (dusk to night)
        float t = (currentTimeOfDay - nightStart) / transitionPeriod;
        if (t > 1.0f) t = 1.0f;
        r = duskR + (nightR - duskR) * t;
        g = duskG + (nightG - duskG) * t;
        b = duskB + (nightB - duskB) * t;
    }
    else {
        // Default to night color
        r = nightR;
        g = nightG;
        b = nightB;
    }

    // Ensure minimum blue component during night to prevent black sky
    if (currentTimeOfDay >= nightStart || currentTimeOfDay < dawnEnd) {
        b = std::max(b, 0.25f);
    }

    // Apply the colors
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void spawnNewHuman() {
    // Original checks for max humans and spawn rate
    if (activeHumans.size() >= MAX_ACTIVE_HUMANS) return;
    if (lastHumanSpawnTime != 0 && (timeNow() - lastHumanSpawnTime < MIN_TIME_BETWEEN_SPAWNS_HUMAN * 100)) return;

    // --- Define some colors for variety ---
    Color redShirt = {0.8f, 0.2f, 0.2f}, tealShirt = {0.1f, 0.5f, 0.5f}, greenShirt = {0.2f, 0.6f, 0.2f};
    Color bluePants = {0.1f, 0.2f, 0.5f}, blackPants = {0.1f, 0.1f, 0.1f}, skyBluePants = {0.3f, 0.5f, 0.9f};
    Color brownHair = {0.4f, 0.2f, 0.1f}, blackHair = {0.1f, 0.1f, 0.1f}, blondeHair = {0.9f, 0.8f, 0.5f};
    Color darkSkin = {0.4f, 0.3f, 0.07f}, lightSkin = {0.9f, 0.8f, 0.7f}, lightBrown = {0.9f, 0.7f, 0.5f};

    std::vector<Color> shirts = {redShirt, tealShirt, greenShirt};
    std::vector<Color> pants = {bluePants, blackPants, skyBluePants};
    std::vector<Color> hairs = {brownHair, blackHair, blondeHair};
    std::vector<Color> skins = {darkSkin, lightSkin, lightBrown};

    // --- Randomize Appearance ---
    Color chosenShirt = shirts[rand() % shirts.size()];
    Color chosenPants = pants[rand() % pants.size()];
    Color chosenHair = hairs[rand() % hairs.size()];
    Color chosenSkin = skins[rand() % skins.size()];
    HairStyle chosenStyle = (HairStyle)(rand() % 3);

    // --- Determine Starting Position and Trajectory ---
    bool startsOnBottomSidewalk = rand() % 2 == 0;
    bool comesFromLeft = rand() % 2 == 0;
    float startX = comesFromLeft ? -50.0f : WINDOW_WIDTH + 50.0f;
    float startY = startsOnBottomSidewalk ? (SIDEWALK_BOTTOM_Y_START + SIDEWALK_BOTTOM_Y_END) / 2.0f : (SIDEWALK_TOP_Y_START + SIDEWALK_TOP_Y_END) / 2.0f;
    
    // Create and configure the new human
    std::shared_ptr<AdvancedHuman> newHuman = std::make_shared<AdvancedHuman>(
        startX, startY, 100.0f,
        chosenShirt, chosenPants, chosenSkin, chosenHair, chosenStyle
    );
    
    newHuman->onBottomSidewalkInitially = startsOnBottomSidewalk;
    newHuman->willCrossRoad = (rand() % 3) == 0;
    newHuman->currentSidewalkY = startY;

    if (!newHuman->willCrossRoad) {
        newHuman->targetX = (startX < WINDOW_WIDTH / 2) ? WINDOW_WIDTH + 50.0f : -50.0f;
    } else {
        newHuman->targetX = HUMAN_CROSSING_CENTER_X + (rand() % (int)(HUMAN_CROSSING_WIDTH / 2) - (int)(HUMAN_CROSSING_WIDTH / 4));
    }
    
    activeHumans.push_back(newHuman);
    lastHumanSpawnTime = timeNow();
}

void updateHumans() {
    for (size_t i = 0; i < activeHumans.size(); ++i) {
        auto& human = activeHumans[i];
        human->update();
        if (human->state == HumanState::DESPAWNED) {
            activeHumans.erase(activeHumans.begin() + i);
            i--; // Decrement i to account for the erased element
        }
    }
    // Periodically spawn new humans
    spawnNewHuman();
}

void updateDayNight() {
    currentTimeOfDay += USER_DAY_NIGHT_CYCLE_SPEED;
    if (currentTimeOfDay >= 1.0f) {
        currentTimeOfDay -= 1.0f;
    }
    isNight = (currentTimeOfDay > 0.75 || currentTimeOfDay < 0.22);
}

void updateClouds() {
    for (auto& cloud : clouds) {
        cloud.x += cloud.speed;
        if (cloud.x > WINDOW_WIDTH + 100.0f) {
            cloud.x = -100.0f;
            cloud.y = WINDOW_HEIGHT * (0.8f + (rand() % 20) / 100.0f); // Random height between 0.8 and 1.0
        }
    }
}

static std::function<void()> g_pendingCallback = nullptr;

void timerCallback(int value) {
    trafficSignal->yellowLightOn = false;  // turn off yellow light after delay
    if (g_pendingCallback) {
        g_pendingCallback();  // call the stored function
        g_pendingCallback = nullptr;  // clear it
    }
    glutPostRedisplay();
}

void showTransitionDelay(std::function<void()> callback, int delay) {

    glutPostRedisplay();

    g_pendingCallback = callback;
    glutTimerFunc(delay, timerCallback, 0);
}

void updateTrafficLights() {
    bool isWaiting = HumansWaitingToCross() > 0;
    bool isCrossing = HumansCrossing() > 0;
    int carsNearCrossing = countCarsNearCrossing();
    
    // If humans are waiting and we're not already in the process of changing
    if (isWaiting && !trafficSignal->yellowLightOn) {
        // Only change to red if:
        // 1. There are enough humans waiting (more than MIN_HUMANS_FOR_PRIORITY)
        // 2. OR there aren't enough cars to give them priority
        if (trafficSignal->lightState == TrafficLightState::GREEN && 
            (HumansWaitingToCross() >= MIN_HUMANS_FOR_PRIORITY || 
             carsNearCrossing < MIN_CARS_FOR_PRIORITY)) {
            trafficSignal->yellowLightOn = true;
            showTransitionDelay([&]() { trafficSignal->showRedLight(); }, 1000);
        }
    }
    
    // If no humans are crossing and we were previously crossing
    if (!isCrossing && !trafficSignal->yellowLightOn) {
        // Only change to green if:
        // 1. There are enough cars to give them priority
        // 2. OR there aren't enough humans waiting
        if (trafficSignal->lightState == TrafficLightState::RED && 
            (carsNearCrossing >= MIN_CARS_FOR_PRIORITY || 
             HumansWaitingToCross() < MIN_HUMANS_FOR_PRIORITY)) {
            trafficSignal->yellowLightOn = true;
            showTransitionDelay([&]() { trafficSignal->showGreenLight(); }, 1000);
        }
    }
}

void updateScene() {
    updateVehicles();
    updateHumans();
    updateDayNight();
    updateClouds();
    updateTrafficLights();
}

// Modify the display function to include the new elements
void display()
{
    
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
    
    transition();
    
    drawBackgroundScenes();
    drawGround();
    drawRoadAndSidewalks();
    drawZebraCrossing(ROAD_Y_BOTTOM, ROAD_Y_TOP, HUMAN_CROSSING_X_START, HUMAN_CROSSING_WIDTH);
    drawSceneObjects();

    if (showWarningMessage)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText(TRAFFIC_LIGHT_X - 100, SIDEWALK_TOP_Y_START + 180, "People are still passing the road", 0.7f);
    }

    // Draw debug overlay last to ensure it's on top
    drawDebugOverlay();
    
    glDisable(GL_BLEND);
    glutSwapBuffers();
}


void timer(int)
{
    if (!scenePaused)
    {
        updateScene();
        frameCount++;
    }

    //std::cout << (int)frameCount << std::endl;

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 't':
        case 'T':
            if (trafficSignal->lightState == TrafficLightState::RED) {
                trafficSignal->yellowLightOn = true;
                showTransitionDelay([&]() { trafficSignal->showGreenLight(); }, 1000);
            }
            else if (trafficSignal->lightState == TrafficLightState::GREEN) {
                trafficSignal->yellowLightOn = true;
                showTransitionDelay([&]() { trafficSignal->showRedLight(); }, 1000);
            }
            break;
        case 'p':
        case 'P':
            scenePaused = !scenePaused;
            break;
        case 'd':
        case 'D':
            DEBUG_ON = !DEBUG_ON;
            break;
        case 'n':
        case 'N':
            if (isNight)
            {
                currentTimeOfDay = 0.3f;
                isNight = false;
            }
            else
            {
                currentTimeOfDay = 0.8f;
                isNight = true;
            }
            break;
        case 'm':
        case 'M':
            // Toggle background traffic sound
            if (audioManager.isPlaying("traffic")) {
                audioManager.stopSound("traffic");
                audioManager.stopSound("people");
            } else {
                audioManager.playSound("traffic", true);
                audioManager.playSound("people", true);
            }
            break;
        case 27: // ESC key
            // Cleanup
            vehicles.clear();
            activeHumans.clear();  // shared_ptr will handle deletion
            drawableObjects.clear();
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void init()
{
    std::cout << "Initializing OpenGL..." << std::endl;
    
    // Set clear color to sky blue
    glClearColor(0.52f, 0.8f, 0.92f, 1.0f);
    
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    
    // Set up the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Initialize random seed
    srand(time(0));

    // Initialize vehicles
    for (int i = 0; i < 4; ++i) {
        spawnNewVehicle();
    }

    // Initialize humans with random properties
    for (int i = 0; i < 5; ++i) { // Create 5 initial humans
        spawnNewHuman();
    }

    // Initialize clouds
    clouds.clear();
    clouds.emplace_back(WINDOW_WIDTH * 0.2f, WINDOW_HEIGHT * 0.85f, 1.0f);
    clouds.emplace_back(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.9f, 1.2f);
    clouds.emplace_back(WINDOW_WIDTH * 0.7f, WINDOW_HEIGHT * 0.82f, 0.9f);

    // Initialize stars
    stars.clear();
    for (int i = 0; i < 100; i++) {
        float x = (rand() % WINDOW_WIDTH);
        float y = WINDOW_HEIGHT * (0.7f + (rand() % 30) / 100.0f);
        float size = 1.0f + (rand() % 30) / 10.0f;
        stars.emplace_back(x, y, size);
    }
    
    std::cout << "OpenGL initialization complete" << std::endl;

    // Initialize traffic signal - add it directly to drawableObjects
    drawableObjects.push_back(trafficSignal);

    // Initialize audio system
    if (!audioManager.initialize()) {
        std::cerr << "Failed to initialize audio system" << std::endl;
    } else {
        // Load sounds - replace paths with your actual sound files
        audioManager.loadSound("traffic", "D:/dev/OpenGL/man_walk/audio/traffic.wav"); // Use forward slashes or escaped backslashes
        audioManager.loadSound("people", "D:/dev/OpenGL/man_walk/audio/people.wav");
        //car
        audioManager.loadSound("car", "D:/dev/OpenGL/man_walk/audio/car.wav");
        //bus
        audioManager.loadSound("bus", "D:/dev/OpenGL/man_walk/audio/bus.wav");
        //truck
        audioManager.loadSound("truck", "D:/dev/OpenGL/man_walk/audio/truck.wav");
        // Start playing background traffic sound
        audioManager.playSound("traffic", true);  // true for looping
        audioManager.playSound("people", true);
    }
}


// --- MAIN FUNCTION ---
int main(int argc, char **argv) {
    std::cout << "Starting application..." << std::endl;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Enhanced Traffic Simulation");

    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    
    std::cout << "--- Controls ---" << std::endl;
    std::cout << "P: Pause/Play" << std::endl;
    std::cout << "D: Toggle Debug Bounding Boxes" << std::endl;
    std::cout << "N: Toggle Day/Night" << std::endl;
    std::cout << "M: Toggle Background Traffic Sound" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    
    std::cout << "Entering main loop..." << std::endl;
    glutMainLoop();

    audioManager.cleanup();
    return 0;
}

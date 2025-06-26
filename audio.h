#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <filesystem>

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
        ALenum error = alGetError(); 
        if (error != AL_NO_ERROR) {
            std::cerr << "OpenAL error after making context current: " << alGetString(error) << std::endl;
        }
        isInitialized = true;
        std::cerr << "AudioManager initialized successfully." << std::endl;
        return true;
    }

    bool loadSound(const std::string& name, std::string filePath) {
        if (!isInitialized) return false;

        ALuint buffer;
        alGenBuffers(1, &buffer);
        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            std::cerr << "OpenAL error generating buffer: " << alGetString(error) << std::endl;
            return false;
        }

        //detect if relative or absolute path
        if (filePath.empty()) {
            std::cerr << "File path is empty for sound: " << name << std::endl;
            alDeleteBuffers(1, &buffer);
            return false;
        }

        // if the file path is relative, convert it to absolute
        if (!std::filesystem::path(filePath).is_absolute()) {
            //get the current working directory of the target file
            std::string filepath = __FILE__;
            std::filesystem::path currentPath(filepath);
            filePath = currentPath.parent_path().string() + filePath;
        }

        if (!std::filesystem::exists(filePath)) {
            std::cerr << "Sound file does not exist: " << filePath << std::endl;
            alDeleteBuffers(1, &buffer);
            return false;
        }

        std::cerr << "Loading sound: " << name << " from " << filePath << std::endl;

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
                if (feof(file)) break; 
                std::cerr << "Failed to read WAV chunk header for file: " << filePath << std::endl;
                fclose(file);
                alDeleteBuffers(1, &buffer);
                return false;
            }

            if (memcmp("fmt ", chunkID, 4) == 0) {
                if (chunkSize < 16) { 
                    std::cerr << "WAV fmt chunk too small for file: " << filePath << std::endl;
                    fclose(file);
                    alDeleteBuffers(1, &buffer);
                    return false;
                }
                char fmtData[16]; 
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

                
                if (audioFormat != 1) { 
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
                
                if (chunkSize > 16) {
                    fseek(file, chunkSize - 16, SEEK_CUR);
                }
            } else if (memcmp("data", chunkID, 4) == 0) {
                dataSize = chunkSize;
                break; 
            } else {
                
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

        std::cout << "Audio manager cleaned!" << std::endl;
    }

    ~AudioManager() {
        cleanup();
    }
};
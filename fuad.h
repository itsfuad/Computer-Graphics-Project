#pragma once
#include <GL/glut.h>
#include<GL/gl.h>
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
#include <string>
#include <unordered_map>
#include "audio.h"

#define M_PI 3.14159265358979323846

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

namespace Fuad {

    const float ANIMATION_SPEED = 1.0f;
    const float USER_CAR_SPEED_BASE = 3.0f * ANIMATION_SPEED;
    const float USER_HUMAN_SPEED = 2.80f * ANIMATION_SPEED;
    const int MIN_TIME_BETWEEN_SPAWNS_CAR = 30;

    const int MAX_ACTIVE_HUMANS = 10;
    const int MAX_ACTIVE_VEHICLES = 8;
    const int MIN_TIME_BETWEEN_SPAWNS_HUMAN = 15;
    const float USER_HUMAN_SIDEWALK_SPEED_FACTOR = 1.2f * ANIMATION_SPEED;
    const int HUMAN_SPAWN_RATE_SIDEWALK = 120;

    const int CAR_SPAWN_RATE = 100;

    const float ROAD_Y_BOTTOM = 150.0f;
    const float ROAD_Y_TOP = 250.0f;
    const float SIDEWALK_TOP_Y_START = 250.0f;
    const float SIDEWALK_TOP_Y_END = 280.0f;
    const float SIDEWALK_BOTTOM_Y_START = 120.0f;
    const float SIDEWALK_BOTTOM_Y_END = 150.0f;

    const float HUMAN_CROSSING_X_START = 600.0f;
    const float CAR_STOP_LINE_X = HUMAN_CROSSING_X_START - 20.0f;
    const float HUMAN_CROSSING_WIDTH = 70.0f;
    const float HUMAN_CROSSING_CENTER_X = HUMAN_CROSSING_X_START + HUMAN_CROSSING_WIDTH / 2.0f;

    const float MIN_CAR_SPACING_AHEAD = 60.0f;
    const float MIN_CAR_SPAWN_DISTANCE = 300.0f;
    const float CAR_SAME_LANE_Y_THRESHOLD = 5.0f;
    const float HUMAN_SAFETY_BUFFER = 3.0f;
    const float DISTANCE_TO_STOP_FROM_SIGNAL = 20.0f;
    const float CAR_SPAWN_OFFSCREEN_DISTANCE = 100.0f;


    const float USER_DAY_NIGHT_CYCLE_SPEED = 0.0008f * ANIMATION_SPEED;
    float currentTimeOfDay = 0.3f;
    bool isNight = false;
    unsigned char frameCount = 0;


    bool IS_PAUSED = false;
    bool DEBUG_ON = false;
    bool MUSIC_ON = true;

    bool showWarningMessage = false;
    bool warningMessageActive = false;  // Add flag to track if warning timer is active
    int lastCarSpawnTime = 0;
    int lastHumanSpawnTime = 0;
    int warningMessageTimer = 0;  // Add timer variable


    const float CAR_PRIORITY_THRESHOLD = 350.0f;  


    const float VEHICLE_WHEEL_OFFSET = 5.0f;  


    struct Color {
        float r, g, b;
    };


    enum class HairStyle {
        Spiky,
        FlatTop,
        Bald
    };


    struct Rect {
        float x, y, w, h;
    };

    struct Point2D {
        float x, y;
    };


    struct SmokeParticle {
        float x, y;
        float size;
        float alpha; 
        float x_drift; 
    };



    std::vector<std::function<void()>> debugCalls;

    AudioManager audioManager;

    class Vehicle;
    class Human;
    class Building;
    class Tree;
    class StreetLamp;
    class BlurrySkyline;
    class Star;
    class ShootingStar;
    class Cloud;
    class Drawable;


    unsigned short HumansWaitingToCross();
    unsigned short HumansCrossing();
    int countCarsNearCrossing();
    void showTransitionDelay(std::function<void()>, int);



    std::vector<std::shared_ptr<Drawable>> drawableObjects;
    std::vector<std::shared_ptr<Drawable>> backgroundObjects;


    std::vector<std::shared_ptr<Vehicle>> vehicles; 
    std::vector<std::shared_ptr<Human>> activeHumans;
    std::vector<std::shared_ptr<Star>> stars;
    std::vector<std::shared_ptr<ShootingStar>> shootingStars;

    void drawDebugOverlay() {
        for (const auto& debugCall : debugCalls) {
            debugCall();
        }
        debugCalls.clear(); 
    }

    void setObjectColor(float r, float g, float b, bool isLightSource = false) {
        
        if (isLightSource) {
            glColor3f(r, g, b);
            return;
        }

        
        const float dayLightLevel = 1.0f;
        const float nightLightLevel = 0.5f; 

        
        const float dawnStartTime = 0.18f;
        const float dawnEndTime = 0.28f;
        const float duskStartTime = 0.62f;
        const float duskEndTime = 0.72f;

        float currentLightLevel;

        if (currentTimeOfDay >= duskStartTime && currentTimeOfDay < duskEndTime) {
            
            
            float transitionProgress = (currentTimeOfDay - duskStartTime) / (duskEndTime - duskStartTime);
            
            currentLightLevel = dayLightLevel - (dayLightLevel - nightLightLevel) * transitionProgress;

        } else if (currentTimeOfDay >= duskEndTime || currentTimeOfDay < dawnStartTime) {
            
            currentLightLevel = nightLightLevel;

        } else if (currentTimeOfDay >= dawnStartTime && currentTimeOfDay < dawnEndTime) {
            
            
            float transitionProgress = (currentTimeOfDay - dawnStartTime) / (dawnEndTime - dawnStartTime);
            
            currentLightLevel = nightLightLevel + (dayLightLevel - nightLightLevel) * transitionProgress;

        } else {
            
            currentLightLevel = dayLightLevel;
        }

        
        glColor3f(
            std::max(0.0f, r * currentLightLevel),
            std::max(0.0f, g * currentLightLevel),
            std::max(0.0f, b * currentLightLevel)
        );
    }


    void setObjectColor(const Color& c, bool isLightSource = false) {
        setObjectColor(c.r, c.g, c.b, isLightSource);
    }


    void drawText(float x, float y, const char* text, float scale = 0.7f) {
        glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
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




    class Drawable {
    public:
        float x, y, width, height;
        Drawable(float _x, float _y, float _w = 0, float _h = 0) : x(_x), y(_y), width(_w), height(_h) {}
        Rect getBounds() {
            return {x, y, width, height};
        }
        virtual void draw() = 0;
        virtual void update() = 0;
        virtual ~Drawable() = default;
    };

    class Star : public Drawable {
    public:
        float size;
        float baseBrightness;
        float blinkPhase;
        Star(float _x, float _y, float _size) :
            Drawable(_x, _y),
            size(_size),
            baseBrightness(0.7f + (rand() % 30) / 100.0f),
            blinkPhase(rand() % 628)
        {}
        void update() override {

        }
        void draw() override {

        }
    };

    class Cloud : public Drawable {
        public:
        float speed;
        float scale;
        Cloud(float _x, float _y, float _scale): Drawable(_x, _y), scale(_scale) {
            speed = 0.2f + (rand() % 30) / 100.0f;
        }
        void draw() override {
            glColor3f(0.9f, 0.9f, 0.95f);
            float r = 15.0f * scale;
            drawCircle(x, y, r);
            drawCircle(x + r * 0.8f, y, r * 0.9f);
            drawCircle(x - r * 0.8f, y, r * 0.9f);
            drawCircle(x + r * 0.4f, y + r * 0.3f, r * 0.8f);
            drawCircle(x - r * 0.4f, y + r * 0.3f, r * 0.8f);
        }
        void update() override {
            if (!IS_PAUSED) {
                
                x += speed;
                if (x > WINDOW_WIDTH + 100.0f) {
                    x = -100.0f; 
                    y = WINDOW_HEIGHT * (0.8f + (rand() % 20) / 100.0f); 
                }
            }
        }
    };

    class ShootingStar : public Drawable {
    public:
        float speed;
        float angle;
        float trailLength;
        float alpha;
        bool active;
        
        ShootingStar() : Drawable(0, 0), speed(0), angle(0), trailLength(0), alpha(0), active(false) {}
        
        void spawn() {
            // Spawn from top of screen
            x = rand() % WINDOW_WIDTH;
            y = WINDOW_HEIGHT + 10.0f; // Reduced from +50 to +10
            
            // Random angle (downward, slightly diagonal)
            angle = (3 * M_PI / 2) + ((rand() % 60) - 30) * M_PI / 180.0f; // Changed from M_PI/2 to 3*M_PI/2
            
            speed = 2.0f + (rand() % 3); // Reduced speed from 3-6 to 2-4
            trailLength = 30.0f + (rand() % 40); // Trail length between 30-70
            alpha = 1.0f;
            active = true;
        }
        
        void update() override {
            if (!active || !isNight) return;
            
            // Move shooting star
            x += cos(angle) * speed;
            y += sin(angle) * speed;
            
            // Fade out as it moves
            alpha -= 0.02f;
            
            // Deactivate if off screen or faded out
            if (x < -100 || x > WINDOW_WIDTH + 100 || y < -100 || alpha <= 0) {
                active = false;
            }
        }
        
        void draw() override {
            if (!active || !isNight) return;
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Draw trail
            glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.3f);
            glLineWidth(2.0f);
            glBegin(GL_LINES);
            float trailX = x - cos(angle) * trailLength;
            float trailY = y - sin(angle) * trailLength;
            glVertex2f(trailX, trailY);
            glVertex2f(x, y);
            glEnd();
            
            // Draw bright head
            glColor4f(1.0f, 1.0f, 1.0f, alpha);
            glPointSize(3.0f);
            glBegin(GL_POINTS);
            glVertex2f(x, y);
            glEnd();
            
            glDisable(GL_BLEND);
        }
    };

    class TrafficSignal : public Drawable {
    public:
        enum class TrafficLightState { RED, GREEN };
        static TrafficLightState lightState;
        static bool yellowLightOn;
        const int YELLOW_BLINK_INTERVAL = 20;
        TrafficSignal(float x, float y) : Drawable(x, y, 20, 60) {
            static bool initialized = false;
            if (!initialized) {
                lightState = TrafficLightState::GREEN;
                yellowLightOn = false;
                initialized = true;
            }
        }

        static void showGreenLight() {
            lightState = TrafficLightState::GREEN;
        }

        static void showRedLight() {
            lightState = TrafficLightState::RED;
        }

        void update() override {

            if (yellowLightOn) return;

            int numOfHumanWaiting = HumansWaitingToCross();
            int numOfHumansCurrentlyCrossing = HumansCrossing();
            int carsNearCrossing = countCarsNearCrossing();

            bool isWaiting = numOfHumanWaiting > 0;
            bool isCrossing = numOfHumansCurrentlyCrossing > 0;


            bool prioritizeHumans = numOfHumanWaiting > carsNearCrossing;
            if (numOfHumanWaiting == carsNearCrossing) prioritizeHumans = false;

            if (lightState == TrafficLightState::GREEN && isWaiting && prioritizeHumans) {
                yellowLightOn = true;
                showTransitionDelay([&]() { showRedLight(); }, 2000);
            }
            else if (lightState == TrafficLightState::RED && !isCrossing && !prioritizeHumans) {
                yellowLightOn = true;
                showTransitionDelay([&]() { showGreenLight(); }, 2000);
            }
        }

        void draw() override {
            const float height = 60.0f;
            const float width = 20.0f;
            const float lightRadius = 8.0f;
            float spacing = height / 3.0f;
            int translateY = 60;
            int translateX = -(width / 4);
            const float gap = 18;

            for (int i = 0; i < 3; i++) {
                float centerY = y + height - (i + 0.5f) * spacing;
                if(i == 0 && lightState == TrafficLightState::RED && !yellowLightOn)
                    glColor3f(1.0f, 0.0f, 0.0f);
                else if(i == 1 && yellowLightOn)
                    glColor3f(1.0f, 1.0f, 0.0f);
                else if(i == 2 && lightState == TrafficLightState::GREEN && !yellowLightOn)
                    glColor3f(0.0f, 1.0f, 0.0f);
                else
                    glColor3f(0.3f, 0.3f, 0.3f);
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
                setObjectColor(0.0f, 0.0f, 0.0f);
                drawTriangle(x + translateX, centerY + translateY, x + translateX - 10, centerY + translateY, x + translateX, centerY - 10 + translateY);
                drawTriangle(x + translateX + width, centerY + translateY, x + translateX + width + 10, centerY + translateY, x + translateX + width, centerY - 10 + translateY);
            }
            setObjectColor(0.0f, 0.0f, 0.0f);
            drawRect(x + translateX, y + translateY - 8, width, height);
            drawRect(x, y, 10, 100);
            drawHumanSign();
        }

    private:
        void drawHumanShape(float x, float y, float scale, int walkState) {
            glPushMatrix();
            glTranslatef(x, y, 0.0f);
            glScalef(scale, scale, 1.0f);
            
            if (walkState == 1) {
                
                
                glColor3f(0.0f, 0.9f, 0.0f); 
                
                drawLine(-7, -4, -3, 4, 4.0f); 
                drawLine(6, 0, 3, 4, 4.0f);    
                drawLine(6, -4, 6, 0, 4.0f);    
                
                drawLine(-5, -10, -1, -2, 4.0f); 
                drawLine(5, -10, 1, -2, 4.0f);  
            } else {
                
                glColor3f(0.9f, 0.0f, 0.0f); 
                
                drawLine(-3, -4, -3, 4, 4.0f); 
                
                drawLine(3.2, -4, 3.2, 4, 4.0f); 
                
                drawLine(-1, -10, -1, -2, 4.0f); 
                
                drawLine(1.2, -10, 1.2, -2, 4.0f); 
            }

            drawCircle(0, 7.5, 3);
            
            drawRect(0.0f, 0.0f, 4.0f, 8.0f, -4.0f / 2.0f, -8.0f / 2.0f); 
            
            glPopMatrix();
        }
        void drawHumanSign() {
            
            if (yellowLightOn) {
                
                if ((frameCount / YELLOW_BLINK_INTERVAL) % 2 == 0) { 
                    audioManager.playSound("beep", true);
                    glColor3f(0.9f, 0.0f, 0.0f); 
                    drawHumanShape(x + 5, y + 85, 0.8, 0); 
                }
            } else {
                if (audioManager.isPlaying("beep")) {
                    audioManager.stopSound("beep");
                }
                if (lightState == TrafficLightState::RED) {
                    glColor3f(0.0f, 0.9f, 0.0f);
                } else {
                    glColor3f(0.9f, 0.0f, 0.0f);
                }
                drawHumanShape(x + 5, y + 85, 0.8, lightState == TrafficLightState::RED ? 1 : 0);
            }
        }
    };

    TrafficSignal::TrafficLightState TrafficSignal::lightState = TrafficSignal::TrafficLightState::GREEN;
    bool TrafficSignal::yellowLightOn = false;

    class Human : public Drawable {
    public:
        
        enum class HumanState { 
            WALKING_ON_SIDEWALK,
            WAITING_AT_CROSSING_EDGE,
            CROSSING_ROAD,
            REACHED_OTHER_SIDEWALK,
            WALKING_AWAY_ON_SIDEWALK, 
            DESPAWNED 
        };
        HumanState state;
        float targetX;
        float currentSidewalkY;
        bool onBottomSidewalkInitially;
        bool willCrossRoad;
        bool cameFromLeft; // Track original spawn direction
        float speedFactor;
        float speed;

        
        Color shirtColor;
        Color pantsColor;
        Color hairColor;
        Color skinColor;
        HairStyle hairStyle;
        float scale;

        
        float walkCycle;
        bool isWalking;
        int direction;
        float SYNC_CONST;

        
        Human() : Drawable(-1000.0f, -1000.0f), 
            state(HumanState::DESPAWNED),
            targetX(0.0f), currentSidewalkY(0.0f),
            onBottomSidewalkInitially(false), willCrossRoad(false),
            speedFactor(1.0f), speed(0.0f), 
            shirtColor({0,0,0}), pantsColor({0,0,0}), hairColor({0,0,0}), skinColor({0,0,0}),
            hairStyle(HairStyle::Bald), 
            scale(1.0f), walkCycle(0.0f), isWalking(false), direction(0) {
            SYNC_CONST = 0.15f;
        }

        Human(float startX, float startY, float visualScale, const Color& shirt, const Color& pants, const Color& skin, const Color& hair, HairStyle style)
            : Drawable(startX, startY, 0.2f, 0.4f),
            state(HumanState::WALKING_ON_SIDEWALK),
            targetX(0.0f),
            currentSidewalkY(0.0f),
            onBottomSidewalkInitially(false),
            willCrossRoad(false),
            speedFactor(1.0f + (rand() % 41) / 100.0f),  
            speed(0.7f * speedFactor),  
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

        
        void reset() {
            state = HumanState::DESPAWNED;
            x = -1000.0f; 
            stopWalking();
        }

        
        void initialize(float startX, float startY, float visualScale, const Color& shirt, const Color& pants, const Color& skin, const Color& hair, HairStyle style, bool bottomSidewalk, bool crossRoad) {
            x = startX;
            y = startY;
            scale = visualScale;
            shirtColor = shirt;
            pantsColor = pants;
            skinColor = skin;
            hairColor = hair;
            hairStyle = style;
            onBottomSidewalkInitially = bottomSidewalk;
            willCrossRoad = crossRoad;
            cameFromLeft = (startX < WINDOW_WIDTH / 2); // Set original spawn direction

            speedFactor = 1.0f + (rand() % 41) / 100.0f;
            speed = 0.7f * speedFactor;
            walkCycle = 0.0f;
            isWalking = false;
            direction = 0; 

            state = HumanState::WALKING_ON_SIDEWALK;
            currentSidewalkY = startY;

            // Set target based on crossing state
            if (willCrossRoad) {
                targetX = HUMAN_CROSSING_CENTER_X + (rand() % (int)(HUMAN_CROSSING_WIDTH / 2) - (int)(HUMAN_CROSSING_WIDTH / 4));
            } else {
                targetX = cameFromLeft ? WINDOW_WIDTH + 50.0f : -50.0f;
            }
        }

        
        void startWalking(int dir) {
            isWalking = true;
            direction = dir;
        }

        void stopWalking() {
            isWalking = false;
            walkCycle = 0.0f;
        }

        
        void update() {
            if (state == HumanState::DESPAWNED) return; 

            bool isCurrentlyMoving = false;
            float effectiveSpeed = speed * USER_HUMAN_SIDEWALK_SPEED_FACTOR;
            float walkCycleSpeed = 0.2f;  

            switch (state) {
                case HumanState::WALKING_ON_SIDEWALK:
                    isCurrentlyMoving = true;
                    y = currentSidewalkY;
                    walkCycleSpeed = effectiveSpeed * SYNC_CONST;  
                    // Removed dynamic targetX update here
                    if (fabs(x - targetX) < effectiveSpeed * 1.5f) {
                        x = targetX;
                        if (willCrossRoad) {
                            state = HumanState::WAITING_AT_CROSSING_EDGE;
                        } else {
                            state = HumanState::DESPAWNED;
                            reset();
                        }
                    } else if (x < targetX) {
                        x += effectiveSpeed;
                        startWalking(0); 
                    } else {
                        x -= effectiveSpeed;
                        startWalking(1); 
                    }
                    break;

                case HumanState::WAITING_AT_CROSSING_EDGE:
                    stopWalking();
                    if (!willCrossRoad) {
                        // Changed mind, go back to walking away
                        // Determine which edge to go to based on current position
                        if (x > WINDOW_WIDTH / 2) {
                            targetX = WINDOW_WIDTH + 50.0f; // Go to right edge
                        } else {
                            targetX = -50.0f; // Go to left edge
                        }
                        state = HumanState::WALKING_ON_SIDEWALK;
                    } else if (TrafficSignal::lightState == TrafficSignal::TrafficLightState::RED && !TrafficSignal::yellowLightOn) {
                        state = HumanState::CROSSING_ROAD;
                    }
                    break;

                case HumanState::CROSSING_ROAD:
                    isCurrentlyMoving = true;
                    effectiveSpeed *= 0.8f;  
                    walkCycleSpeed = effectiveSpeed * SYNC_CONST;  
                    if (onBottomSidewalkInitially) {
                        y += effectiveSpeed;
                        startWalking(2); 
                        if (y >= (SIDEWALK_TOP_Y_START + SIDEWALK_TOP_Y_END) / 2.0f) {
                            y = (SIDEWALK_TOP_Y_START + SIDEWALK_TOP_Y_END) / 2.0f;
                            currentSidewalkY = y;
                            state = HumanState::REACHED_OTHER_SIDEWALK;
                        }
                    } else {
                        y -= effectiveSpeed;
                        startWalking(3); 
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
                    walkCycleSpeed = effectiveSpeed * SYNC_CONST;  
                    if ((targetX < 0 && x < 0) || (targetX > WINDOW_WIDTH && x > WINDOW_WIDTH)) {
                        state = HumanState::DESPAWNED;
                        reset(); 
                    } else if (x < targetX) {
                        x += effectiveSpeed;
                        startWalking(0); 
                    } else { 
                        x -= effectiveSpeed;
                        startWalking(1); 
                    }
                    break;
                
                case HumanState::DESPAWNED:
                    stopWalking();
                    break;
            }

            if (isCurrentlyMoving) {
                walkCycle += walkCycleSpeed;  
                if (walkCycle >= 2.0f * M_PI) {
                    walkCycle = 0.0f;  
                }
            } else {
                stopWalking();
            }
        }

        void draw() {
            if (state == HumanState::DESPAWNED) return; 

            glPushMatrix();

            
            
            const float model_feet_to_center_offset = 0.19f;
            glTranslatef(x, y + model_feet_to_center_offset * scale, 0.0f);
            glScalef(scale, scale, 1.0f);

            if (direction == 0) { 
                glScalef(-1.0f, 1.0f, 1.0f);
            }

            float armAngle = 0.0f, legAngle = 0.0f, legLift = 0.0f;

            if (isWalking) {
                if (direction == 0 || direction == 1) { 
                    legAngle = sin(walkCycle) * 25.0f;
                    armAngle = -sin(walkCycle) * 20.0f;
                } else { 
                    armAngle = sin(walkCycle) * 15.0f;
                    legLift = std::max(0.0f, (float)sin(walkCycle * 2.0f)) * 0.03f;
                }
            }

            const float torsoTop = 0.1f, torsoBottom = -0.05f, torsoWidth = 0.08f;
            const float headRadius = 0.04f, neckY = torsoTop, headY = neckY + headRadius;
            const float shoulderY = torsoTop - 0.02f, shoulderX = torsoWidth / 2;
            const float hipY = torsoBottom, hipX = torsoWidth / 4;

            
            setObjectColor(skinColor.r, skinColor.g, skinColor.b);
            
            glPushMatrix();
            glTranslatef(shoulderX, shoulderY, 0.0f);
            glRotatef(-armAngle, 0, 0, 1);
            drawLine(0, 0, 0, -0.08f, 5.0f); 
            glTranslatef(0, -0.08f, 0);
            glRotatef(-30.0f, 0, 0, 1);
            drawLine(0, 0, 0, -0.07f, 5.0f); 
            drawCircle(0, -0.075f, 0.015f); 
            glPopMatrix();

            
            glPushMatrix();
            setObjectColor(pantsColor.r, pantsColor.g, pantsColor.b);
            
            glPushMatrix();
            glTranslatef(hipX + 0.01f, hipY + 0.02f, 0.0f);
            if(direction == 0 || direction == 1) glRotatef(-legAngle, 0, 0, 1);
            else glTranslatef(0.0f, -legLift, 0.0f);
            drawLine(0, 0, 0, -0.13f, 5.0f); 
            glTranslatef(0, -0.13f, 0);
            glRotatef(std::max(0.0f, -legAngle * 0.5f), 0, 0, 1);
            drawLine(0, 0, 0, -0.06f, 5.0f); 
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawRect(-0.01f, -0.06f, 0.04f, 0.02f, -0.04f / 2.0f, -0.02f / 2.0f); 
            glPopMatrix();
            
            glPushMatrix();
            setObjectColor(pantsColor.r, pantsColor.g, pantsColor.b);
            glTranslatef(-hipX - 0.01f, hipY + 0.02f, 0.0f);
            if(direction == 0 || direction == 1) glRotatef(legAngle, 0, 0, 1);
            else glTranslatef(0.0f, legLift, 0.0f);
            drawLine(0, 0, 0, -0.13f, 5.0f); 
            glTranslatef(0, -0.13f, 0);
            glRotatef(std::max(0.0f, legAngle * 0.5f), 0, 0, 1);
            drawLine(0, 0, 0, -0.06f, 5.0f); 
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawRect(-0.01f, -0.06f, 0.04f, 0.02f, -0.04f / 2.0f, -0.02f / 2.0f); 
            glPopMatrix();
            glPopMatrix();

            
            setObjectColor(shirtColor.r, shirtColor.g, shirtColor.b);
            drawRect(0.0f, (torsoTop + torsoBottom) / 2, torsoWidth, torsoTop - torsoBottom, -torsoWidth / 2.0f, -(torsoTop - torsoBottom) / 2.0f);

            
            setObjectColor(pantsColor.r, pantsColor.g, pantsColor.b);
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, hipY + 0.01f);
            for(int i = 0; i <= 180; i += 10) {
                float angle = i * M_PI / 180.0f;
                float x = ((torsoWidth + 0.005)/2 - 0.002) * cos(angle);
                float y = hipY + 0.01f - ((torsoWidth + 0.005)/2 - 0.002) * sin(angle);
                glVertex2f(x, y);
            }
            glEnd();

            
            setObjectColor(skinColor.r, skinColor.g, skinColor.b);
            drawCircle(0.0f, headY, headRadius, 20); 

            setObjectColor(hairColor.r, hairColor.g, hairColor.b);
            switch(hairStyle) {
                case HairStyle::Spiky:
                    if (direction == 2) { 
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
                    } else if (direction == 0 || direction == 1) { 
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
                    } else { 
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
                    if(direction == 2){ 
                        drawRect(0.0f, headY + headRadius * 0.5f, headRadius * 2, headRadius, -(headRadius * 2) / 2.0f, -headRadius / 2.0f);
                        drawCircle(0.0f, headY, headRadius*0.95f, 20); 
                    } else { 
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

            if (direction != 2) { 
                setObjectColor(0.0f, 0.0f, 0.0f);
                if (direction == 3) { 
                    drawCircle(-0.012f, headY + 0.01f, 0.005f, 20); drawCircle( 0.012f, headY + 0.01f, 0.005f, 20); 
                    drawLine(-0.01f, headY - 0.015f, 0.01f, headY - 0.015f, 2.0f); 
                } else { 
                    drawCircle(-0.015f, headY + 0.005f, 0.004f, 20); 
                    setObjectColor(skinColor.r, skinColor.g, skinColor.b);
                    glBegin(GL_POLYGON);
                    glVertex2f(-headRadius, headY); glVertex2f(-headRadius, headY - 0.01f); glVertex2f(-headRadius - 0.01f, headY - 0.005f);
                    glEnd();
                    setObjectColor(0.0f, 0.0f, 0.0f);
                    drawLine(-0.015f, headY - 0.01f, -0.03f, headY - 0.01f, 2.0f); 
                }
            }

            
            setObjectColor(skinColor.r, skinColor.g, skinColor.b);
            glPushMatrix();
            glTranslatef(-shoulderX, shoulderY, 0.0f);
            glRotatef(armAngle, 0, 0, 1);
            drawLine(0, 0, 0, -0.08f, 5.0f); 
            glTranslatef(0, -0.08f, 0);
            glRotatef(-30.0f, 0, 0, 1);
            drawLine(0, 0, 0, -0.07f, 5.0f); 
            drawCircle(0, -0.075f, 0.015f); 
            glPopMatrix();
            glPopMatrix();

            
            if (DEBUG_ON) {  
                // Draw debug bounding box
                Rect b = getBounds();
                glColor3f(1.0f, 1.0f, 0.0f);  
                drawBound(b.x, b.y, 25, 45, -13, -5);
                
                // Draw crossing indicator triangle above bounding box
                const float triangleSize = 6.0f;
                const float triangleY = b.y + b.h + 10.0f; // Above the bounding box
                
                if (willCrossRoad) {
                    glColor3f(1.0f, 0.0f, 0.0f); // Red triangle for will cross
                } else {
                    glColor3f(0.0f, 1.0f, 0.0f); // Green triangle for won't cross
                }
                
                // Draw downward pointing triangle
                glBegin(GL_TRIANGLES);
                glVertex2f(b.x + b.w/2, triangleY + 30); // Top point (center of bounding box)
                glVertex2f(b.x + b.w/2 - triangleSize, triangleY + 30 + triangleSize); // Bottom left
                glVertex2f(b.x + b.w/2 + triangleSize, triangleY + 30 + triangleSize); // Bottom right
                glEnd();
            }

            glPopMatrix();  
        }
    };



    class Vehicle : public Drawable {
    public:
        enum class VehicleType { CAR, TRUCK, BUS, VAN, SUV };
        enum class VehicleState { ACTIVE, DESPAWNED };
        float r, g, b;
        float speedFactor;
        float current_speed;
        float target_speed;
        float acceleration_rate;
        float deceleration_rate;
        VehicleType type;
        VehicleState state;
        bool isHonking;
        int honkTimer;
        int honkDuration;
        bool isBlocked;

        const int MIN_HONK_INTERVAL = 90;
        const int MAX_HONK_INTERVAL = 240;
        const int HONK_DURATION_MAX = 45;

        Vehicle(float _x, float _y, float _w, float _h, float _r, float _g, float _b, VehicleType _type)
            : Drawable(_x, _y, _w, _h), r(_r), g(_g), b(_b),
            speedFactor(1.0f + (rand() % 41) / 100.0f),
            current_speed(0.0f),
            target_speed(USER_CAR_SPEED_BASE * speedFactor),
            acceleration_rate(0.05f),
            deceleration_rate(0.1f),
            type(_type),
            state(VehicleState::DESPAWNED),
            isHonking(false), honkTimer(0), honkDuration(0), isBlocked(false) {}

        virtual void draw() override = 0;
        virtual ~Vehicle() = default;

        void reset() {
            x = -width - 150.0f; 
            current_speed = 0.0f;
            target_speed = USER_CAR_SPEED_BASE * speedFactor;
            isHonking = false;
            honkTimer = 0;
            honkDuration = 0;
            isBlocked = false;
            state = VehicleState::DESPAWNED;
        }

        void initialize(float startX, float startY, float _r, float _g, float _b) {
            x = startX;
            y = startY;
            r = _r;
            g = _g;
            b = _b;
            if (r < 0.1f && g < 0.1f && b < 0.1f) {
                r = 0.2f; g = 0.2f; b = 0.2f;
            }
            state = VehicleState::ACTIVE;
        }

        void updateSpeed() {
            if (current_speed < target_speed) {
                current_speed = std::min(current_speed + acceleration_rate, target_speed);
            } else if (current_speed > target_speed) {
                current_speed = std::max(current_speed - deceleration_rate, target_speed);
            }
        }

        void updateHonk() {

            if (x < -width || x > WINDOW_WIDTH) {
                return;
            }

            if (honkDuration > 0) {
                honkDuration--;
            } else {
                isHonking = false;
            }

            if (current_speed < 0.1f && isBlocked) {
                if (honkTimer > 0) {
                    honkTimer--;
                }
                if (honkTimer <= 0 && !isHonking) {
                    isHonking = true;
                    honkDuration = HONK_DURATION_MAX;
                    honkTimer = MIN_HONK_INTERVAL + (rand() % (MAX_HONK_INTERVAL - MIN_HONK_INTERVAL + 1));
                    
                    
                    if (MUSIC_ON && !IS_PAUSED) {
                        std::string soundToPlay;
                        switch (type) {
                            case VehicleType::CAR: soundToPlay = "car"; break;
                            case VehicleType::TRUCK: soundToPlay = "truck"; break;
                            case VehicleType::BUS: soundToPlay = "bus"; break;
                            case VehicleType::VAN: soundToPlay = "truck"; break; 
                            case VehicleType::SUV: soundToPlay = "car"; break;   
                            default: soundToPlay = "car";
                        }
                        audioManager.playSound(soundToPlay, false);
                    }
                }
            } else {
                honkTimer = 0;
            }
        }

        bool isOutOfScreen() const {
            // Check if the entire vehicle is off screen (x position is past window width)
            return x > WINDOW_WIDTH;
        }

        void update() override {

            if (state != VehicleState::ACTIVE) {
                return;
            }

            target_speed = USER_CAR_SPEED_BASE * speedFactor;

            bool stoppedByLight = false;
            if (TrafficSignal::lightState == TrafficSignal::TrafficLightState::RED || TrafficSignal::yellowLightOn) {
                if (x + width < CAR_STOP_LINE_X + 5.0f && x + width > CAR_STOP_LINE_X - (DISTANCE_TO_STOP_FROM_SIGNAL + width)) {
                    target_speed = 0;
                    stoppedByLight = true;
                    if (DEBUG_ON) {
                        debugCalls.push_back([=]() {
                            std::string debugText = "Stopped by Light";
                            drawText(x + width / 2, y - 20, debugText.c_str(), 0.5f);
                        });
                    }
                }
            }

            bool blockedByVehicleAhead = false;
            if (!stoppedByLight) {
                for (const auto& v2 : vehicles) {
                    if (this == v2.get()) continue;
                    if (fabs(y - v2->y) < CAR_SAME_LANE_Y_THRESHOLD && v2->x > x) {
                        float distance = v2->x - (x + width);
                        float minSpacing = std::max(MIN_CAR_SPACING_AHEAD, width * 0.5f);
                        if (distance < minSpacing) {
                            target_speed = v2->current_speed < target_speed ? v2->current_speed : target_speed;
                            if (distance < width * 0.25f || (v2->current_speed < 0.1f && distance < minSpacing * 0.75f)) {
                                target_speed = 0;
                                blockedByVehicleAhead = true;
                                if (DEBUG_ON) {
                                    debugCalls.push_back([=]() {
                                        std::string debugText = "Blocked by Vehicle Ahead";
                                        drawText(x + width / 2, y - 20, debugText.c_str(), 0.5f);
                                    });
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
                        if (ped->state == Human::HumanState::CROSSING_ROAD && checkAABBCollision(vehicleNextBounds, ped->getBounds())) {
                            blockedByHuman = true;
                            target_speed = 0;
                            if (DEBUG_ON) {
                                debugCalls.push_back([=]() {
                                    std::string debugText = "Blocked by Human";
                                    drawText(x + width / 2, y - 20, debugText.c_str(), 0.5f);
                                });
                            }
                            break;
                        }
                    }
                }
            }

            isBlocked = stoppedByLight || blockedByVehicleAhead || blockedByHuman;
            if (isBlocked) {
                target_speed = 0;
            }

            updateSpeed();
            x += current_speed;
            updateHonk();
        }
    };





    class Car : public Vehicle {
    public:
        Car(float _x, float _y, float _w, float _h, float _r, float _g, float _b)
            : Vehicle(_x, _y, _w, _h, _r, _g, _b, VehicleType::CAR) {
            
        }

        void draw() override {
            if (state == VehicleState::DESPAWNED) return; 

            glPushMatrix();
            glTranslatef(0, VEHICLE_WHEEL_OFFSET, 0);  
            
            setObjectColor(r, g, b);
            drawRect(x, y, width, height * 0.6f);

            
            setObjectColor(0.6f, 0.8f, 0.9f);
            glBegin(GL_QUADS);
            glVertex2f(x + width * 0.15f, y + height * 0.6f);
            glVertex2f(x + width * 0.85f, y + height * 0.6f);
            glVertex2f(x + width * 0.7f, y + height);
            glVertex2f(x + width * 0.25f, y + height);
            glEnd();

            
            setObjectColor(0.1f, 0.1f, 0.1f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x + width * 0.15f, y + height * 0.6f);
            glVertex2f(x + width * 0.85f, y + height * 0.6f);
            glVertex2f(x + width * 0.7f, y + height);
            glVertex2f(x + width * 0.25f, y + height);
            glEnd();
            drawLine(x + width * 0.5f, y + height * 0.6f, x + width * 0.5f, y + height);

            
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawCircle(x + width * 0.25f, y, height * 0.25f);
            drawCircle(x + width * 0.75f, y, height * 0.25f);
            glColor3f(0.7f, 0.7f, 0.7f); 
            drawCircle(x + width * 0.25f, y, height * 0.15f);
            drawCircle(x + width * 0.75f, y, height * 0.15f);

            
            if (current_speed < 0.1f) {
                glColor3f(1.0f, 0.0f, 0.0f);
            } else {
                glColor3f(0.6f, 0.0f, 0.0f);
            }
            drawRect(x, y + height * 0.3f, 4, 6);

            
            if (isNight) {
                glColor4f(1.0f, 1.0f, 0.7f, 0.3f); 
                drawCircle(x + width, y + height * 0.4f, 20.0f);
                glColor3f(1.0f, 1.0f, 0.8f); 
            } else {
                glColor3f(0.9f, 0.9f, 0.9f); 
            }
            drawRect(x + width - 4, y + height * 0.3f, 4, 6);

            glPopMatrix();  
            if (DEBUG_ON) {  
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
            
            speedFactor *= 0.9f;
            target_speed = USER_CAR_SPEED_BASE * speedFactor;
            acceleration_rate *= 0.7f;  
            deceleration_rate *= 0.7f;  
        }

        void draw() override {
            if (state == VehicleState::DESPAWNED) return; 

            glPushMatrix();
            glTranslatef(0, VEHICLE_WHEEL_OFFSET, 0);  
            
            
            setObjectColor(0.0f, 0.0f, 0.0f);
            drawLine(x + width * 0.65f, y + height * 0.2f, x + width * 0.65f, y + height * 0.8f, 6.0f);

            
            setObjectColor(r * 0.8f, g * 0.8f, b * 0.8f);
            float containerWidth = width * 0.68f;
            float containerHeight = height * 0.9f;
            drawRect(x - 3, y + 5, containerWidth, containerHeight);
            
            setObjectColor(r * 0.7f, g * 0.7f, b * 0.7f);
            glLineWidth(2.0f);
            for(int i=1; i<4; ++i){
                drawLine(x + containerWidth * (i/4.0f), y + 5, x + containerWidth * (i/4.0f), y + 5 + containerHeight);
            }


            
            float cabWidth = width - containerWidth;
            float cabX = x + containerWidth;
            setObjectColor(r, g, b);
            glBegin(GL_QUADS);
            glVertex2f(cabX, y + 5);
            glVertex2f(cabX + cabWidth, y + 5);
            glVertex2f(cabX + cabWidth, y + height * 0.8f);
            glVertex2f(cabX, y + height * 0.9f); 
            glEnd();


            
            setObjectColor(0.6f, 0.8f, 0.9f);
            glBegin(GL_POLYGON);
            glVertex2f(cabX + cabWidth * 0.5f, y + height * 0.42f);
            glVertex2f(cabX + cabWidth * 1.0f, y + height * 0.42f);
            glVertex2f(cabX + cabWidth * 1.0f, y + height * 0.75f);
            glVertex2f(cabX + cabWidth * 0.5f, y + height * 0.8f);
            glEnd();


            
            setObjectColor(0.5f, 0.5f, 0.5f);
            drawRect(x + containerWidth - 25, y + 5, 20, 10);
            setObjectColor(0.3f, 0.3f, 0.3f);
            drawRect(cabX + cabWidth * 0.1f, y + height - 4, 3, 10); 


            
            float wheelRadius = height * 0.2f;
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawCircle(x + width * 0.15f, y + wheelRadius, wheelRadius);
            drawCircle(x + width * 0.35f, y + wheelRadius, wheelRadius);
            drawCircle(x + width * 0.85f, y + wheelRadius, wheelRadius);
            setObjectColor(0.7f, 0.7f, 0.7f);
            drawCircle(x + width * 0.15f, y + wheelRadius, wheelRadius * 0.6f);
            drawCircle(x + width * 0.35f, y + wheelRadius, wheelRadius * 0.6f);
            drawCircle(x + width * 0.85f, y + wheelRadius, wheelRadius * 0.6f);


            
            
            if (current_speed < 0.1f) {
                glColor3f(1.0f, 0.0f, 0.0f);
            } else {
                glColor3f(0.6f, 0.0f, 0.0f);
            }
            drawRect(x - 4, y + height * 0.4f, 4, 5);

            
            if (isNight) {
                glColor4f(1.0f, 1.0f, 0.7f, 0.3f); 
                drawCircle(x + width, y + height * 0.2f, 25.0f);
                glColor3f(1.0f, 1.0f, 0.8f); 
            } else {
                setObjectColor(0.9f, 0.9f, 0.9f);
            }
            
            drawRect(x + width - 5, y + height * 0.2f, 5, 5);

            glPopMatrix();  
            if (DEBUG_ON) {  
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
            
            acceleration_rate *= 0.6f;  
            deceleration_rate *= 0.6f;  
        }

        void draw() override {
            if (state == VehicleState::DESPAWNED) return; 

            glPushMatrix();
            glTranslatef(0, VEHICLE_WHEEL_OFFSET, 0);  

            
            setObjectColor(r, g, b);
            drawRect(x, y, width, height);

            
            setObjectColor(0.2f, 0.2f, 0.3f);
            float windowHeight = height * 0.4f;
            float windowY = y + height * 0.4f;
            float windowWidth = width * 0.12f;
            float windowSpacing = width * 0.05f;
            for (int i = 0; i < 5; ++i) {
                float windowX = x + width * 0.1f + i * (windowWidth + windowSpacing);
                drawRect(windowX, windowY, windowWidth, windowHeight);
            }

            
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawCircle(x + width * 0.2f, y, height * 0.25f);
            drawCircle(x + width * 0.8f, y, height * 0.25f);
            glColor3f(0.7f, 0.7f, 0.7f);
            drawCircle(x + width * 0.2f, y, height * 0.15f);
            drawCircle(x + width * 0.8f, y, height * 0.15f);

            
            if (current_speed < 0.1f) glColor3f(1.0f, 0.0f, 0.0f);
            else glColor3f(0.6f, 0.0f, 0.0f);
            drawRect(x, y + height * 0.2f, 4, 8);

            
            if (isNight) {
                glColor4f(1.0f, 1.0f, 0.7f, 0.3f);
                drawCircle(x + width, y + height * 0.25f, 30.0f);
                glColor3f(1.0f, 1.0f, 0.8f);
            } else {
                setObjectColor(0.9f, 0.9f, 0.9f);
            }
            drawRect(x + width - 5, y + height * 0.25f, 4, 4);

            glPopMatrix();  
            if (DEBUG_ON) {  
                Rect b = getBounds();
                glColor3f(0.0f, 1.0f, 0.0f);
                drawBound(b.x, b.y, b.w, b.h);
            }
        }
    };

    class Van : public Vehicle {
    public:
        Van(float _x, float _y, float _w, float _h, float _r, float _g, float _b)
            : Vehicle(_x, _y, _w, _h, _r, _g, _b, VehicleType::VAN) {
            
            speedFactor *= 0.95f;
            target_speed = USER_CAR_SPEED_BASE * speedFactor;
        }

        void draw() override {
            if (state == VehicleState::DESPAWNED) return; 

            glPushMatrix();
            glTranslatef(0, VEHICLE_WHEEL_OFFSET, 0);

            
            setObjectColor(r, g, b);
            drawRect(x, y, width * 0.95f, height);
            
            drawRect(x + width * 0.7f, y, width * 0.3f, height * 0.8f);

            
            setObjectColor(0.2f, 0.2f, 0.3f);
            
            drawRect(x + width * 0.75f, y + height * 0.3f, width * 0.2f, height * 0.4f);
            
            drawRect(x + width * 0.1f, y + height * 0.4f, width * 0.5f, height * 0.4f);
            
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawBound(x + width * 0.1f, y + height * 0.4f, width * 0.5f, height * 0.4f);


            
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawCircle(x + width * 0.2f, y, height * 0.25f);
            drawCircle(x + width * 0.75f, y, height * 0.25f);
            setObjectColor(0.7f, 0.7f, 0.7f);
            drawCircle(x + width * 0.2f, y, height * 0.15f);
            drawCircle(x + width * 0.75f, y, height * 0.15f);


            
            if (current_speed < 0.1f) glColor3f(1.0f, 0.0f, 0.0f); else glColor3f(0.6f, 0.0f, 0.0f);
            drawRect(x, y + height * 0.2f, 4, 8); 
            if (isNight) {
                glColor4f(1.0f, 1.0f, 0.7f, 0.3f);
                drawCircle(x + width, y + height * 0.25f, 25.0f);
                glColor3f(1.0f, 1.0f, 0.8f);
            } else {
                setObjectColor(0.9f, 0.9f, 0.9f);
            }
            drawRect(x + width - 1, y + height * 0.25f, 4, 4); 

            glPopMatrix();
            if (DEBUG_ON) {
                Rect b = getBounds();
                setObjectColor(0.0f, 1.0f, 0.0f, true); 
                drawBound(b.x, b.y, b.w, b.h);
            }
        }
    };

    class SUV : public Vehicle {
    public:
        SUV(float _x, float _y, float _w, float _h, float _r, float _g, float _b)
            : Vehicle(_x, _y, _w, _h, _r, _g, _b, VehicleType::SUV) {
            
            acceleration_rate *= 0.9f;
            deceleration_rate *= 0.9f;
        }

        void draw() override {
            if (state == VehicleState::DESPAWNED) return; 

            glPushMatrix();
            glTranslatef(0, VEHICLE_WHEEL_OFFSET + 3.0f, 0); 

            
            setObjectColor(r, g, b);
            drawRect(x, y, width, height * 0.7f);

            
            setObjectColor(0.6f, 0.8f, 0.9f);
            drawRect(x + width * 0.1f, y + height * 0.4f, width * 0.8f, height * 0.5f);

            
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawBound(x + width * 0.1f, y + height * 0.4f, width * 0.8f, height * 0.5f);
            drawLine(x + width * 0.45f, y + height * 0.4f, x + width * 0.45f, y + height * 0.9f);
            drawLine(x + width * 0.7f, y + height * 0.4f, x + width * 0.7f, y + height * 0.9f);

            
            float wheelRadius = height * 0.3f;
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawCircle(x + width * 0.25f, y, wheelRadius);
            drawCircle(x + width * 0.75f, y, wheelRadius);
            setObjectColor(0.7f, 0.7f, 0.7f);
            drawCircle(x + width * 0.25f, y, wheelRadius * 0.6f);
            drawCircle(x + width * 0.75f, y, wheelRadius * 0.6f);

            
            setObjectColor(0.15f, 0.15f, 0.15f);
            drawCircle(x, y + height * 0.4f, wheelRadius * 0.8f);
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawCircle(x, y + height * 0.4f, wheelRadius * 0.7f);

            
            if (current_speed < 0.1f) glColor3f(1.0f, 0.0f, 0.0f); else glColor3f(0.6f, 0.0f, 0.0f);
            drawRect(x + 2, y + height * 0.5f, 4, 6); 
            if (isNight) {
                glColor4f(1.0f, 1.0f, 0.7f, 0.3f);
                drawCircle(x + width, y + height * 0.5f, 25.0f);
                glColor3f(1.0f, 1.0f, 0.8f);
            } else {
                setObjectColor(0.9f, 0.9f, 0.9f);
            }
            drawRect(x + width - 4, y + height * 0.5f, 4, 6); 

            glPopMatrix();
            if (DEBUG_ON) {
                Rect b = getBounds();
                setObjectColor(0.0f, 1.0f, 0.0f, true); 
                drawBound(b.x, b.y, b.w, b.h);
            }
        }
    };



    void drawSun(float x, float y, float radius) {
        
        float sunsetFactor = 0.0f;
        if (currentTimeOfDay >= 0.6f && currentTimeOfDay < 0.8f) {
            
            sunsetFactor = std::min((currentTimeOfDay - 0.6f) / 0.2f, 0.5f);  
        }
        
        
        float r = 1.0f;  
        float g = 0.9f - (0.3f * sunsetFactor);  
        float b = 0.2f - (0.2f * sunsetFactor);  

        
        glColor4f(r, g, b, 0.3f);
        drawCircle(x, y, radius * 1.5f);

        
        glColor3f(r, g, b);
        drawCircle(x, y, radius);
    }

    void drawMoon(float x, float y, float radius) {
        
        glColor4f(0.9f, 0.9f, 0.95f, 0.3f);
        drawCircle(x, y, radius * 1.5f);

        
        glColor3f(0.9f, 0.9f, 0.95f);
        drawCircle(x, y, radius);

        
        glColor3f(0.8f, 0.8f, 0.85f);
        drawCircle(x - radius * 0.3f, y + radius * 0.2f, radius * 0.15f);
        drawCircle(x + radius * 0.2f, y - radius * 0.3f, radius * 0.1f);
        drawCircle(x + radius * 0.1f, y + radius * 0.1f, radius * 0.08f);
    }

    void updateStars() {
        
        
        float starAlpha = 0.0f;

        if (currentTimeOfDay >= 0.85f) {  
            
            starAlpha = 1.0f;
        } else if (currentTimeOfDay >= 0.80f) {  
            
            starAlpha = (currentTimeOfDay - 0.80f) / 0.05f;
        } else if (currentTimeOfDay <= 0.15f) {  
            
            starAlpha = 1.0f;
        } else if (currentTimeOfDay <= 0.20f) {  
            
            starAlpha = (0.20f - currentTimeOfDay) / 0.05f;
        }

        if (starAlpha > 0.0f) {
            for (auto& star : stars) {
                
                if (!IS_PAUSED) { 
                    star->blinkPhase += 0.05f;
                    if (star->blinkPhase > 628.0f) star->blinkPhase -= 628.0f; 
                }

                
                float blinkFactor = 0.5f + 0.5f * sin(star->blinkPhase);
                float finalBrightness = star->baseBrightness * blinkFactor;

                glColor4f(1.0f, 1.0f, 0.9f, starAlpha * finalBrightness);
                glPointSize(star->size);
                glBegin(GL_POINTS);
                glVertex2f(star->x, star->y);
                glEnd();
            }
        }
    }

    void updateShootingStars() {
        if (!isNight) return;
        
        // Randomly spawn new shooting stars
        if (rand() % 100 == 0) { // Increased from 300 to 100 (1 in 100 chance per frame)
            // Find an inactive shooting star to reuse
            for (auto& shootingStar : shootingStars) {
                if (!shootingStar->active) {
                    shootingStar->spawn();
                    break;
                }
            }
        }
        
        // Update all shooting stars
        for (auto& shootingStar : shootingStars) {
            shootingStar->update();
        }
    }





    class Building : public Drawable {
        public:
            Building(float _x, float _y, float _width, float _height)
                : Drawable(_x, _y, _width, _height) {}
    };


    struct WindowState {
        float brightness = 0.0f;  
        bool isLitTarget = false; 
    };

    class BrickBuilding : public Building {
    private:
        std::vector<WindowState> windowStates;
        bool wasNightInternal = false; 
        int numWindowRows, numWindowCols;

    public:
        BrickBuilding(float _x, float _y, float _width, float _height)
            : Building(_x, _y, _width, _height) {

            numWindowRows = 3;
            numWindowCols = 3;
            windowStates.resize(numWindowRows * numWindowCols);
        }

        void update() override {
            
            bool transitionToNight = isNight && !wasNightInternal;
            bool transitionToDay = !isNight && wasNightInternal;
            wasNightInternal = isNight;

            if (transitionToNight) {
                
                for (auto& window : windowStates) {
                    window.isLitTarget = (rand() % 4 == 0); 
                }
            } else if (transitionToDay) {
                
                for (auto& window : windowStates) {
                    window.isLitTarget = false;
                }
            }

            
            if (isNight && frameCount % 15 == 0) {
                if (rand() % 5 == 0) { 
                    int index = rand() % windowStates.size();
                    windowStates[index].isLitTarget = !windowStates[index].isLitTarget;
                }
            }

            
            for (auto& window : windowStates) {
                float targetBrightness = window.isLitTarget ? 1.0f : 0.0f;
                window.brightness = targetBrightness; 
            }
        }

        void draw() override {
            
            setObjectColor(0.5f, 0.45f, 0.45f);
            drawRect(x, y, width, 15);
            
            setObjectColor(0.7f, 0.35f, 0.3f);
            drawRect(x, y + 15, width, height - 25);
            
            glColor4f(0.0f, 0.0f, 0.0f, 0.15f);
            for(float lineY = y + 15; lineY < y + height - 10; lineY += 8) {
                drawLine(x, lineY, x + width, lineY);
            }
            
            setObjectColor(0.3f, 0.2f, 0.2f);
            drawRect(x - 5, y + height - 10, width + 10, 10);

            
            float windowWidth = 18.0f;
            float windowHeight = 22.0f;
            float w_gap = (width - (numWindowCols * windowWidth)) / (numWindowCols + 1);
            float h_gap = (height - 35 - (numWindowRows * windowHeight)) / (numWindowRows + 1);

            Color dayColor = {0.5f, 0.6f, 0.7f};
            Color litColor = {1.0f, 0.95f, 0.75f};

            int windowIndex = 0;
            for (int row = 0; row < numWindowRows; ++row) {
                for (int col = 0; col < numWindowCols; ++col) {
                    float wx = x + w_gap + col * (windowWidth + w_gap);
                    float wy = y + 25 + h_gap + row * (windowHeight + h_gap);

                    
                    setObjectColor(0.8f, 0.8f, 0.8f);
                    drawRect(wx-2, wy-2, windowWidth+4, 4);

                    
                    Color unlitBaseColor;
                    
                    unlitBaseColor = dayColor;

                    
                    float r = unlitBaseColor.r + (litColor.r - unlitBaseColor.r) * windowStates[windowIndex].brightness;
                    float g = unlitBaseColor.g + (litColor.g - unlitBaseColor.g) * windowStates[windowIndex].brightness;
                    float b = unlitBaseColor.b + (litColor.b - unlitBaseColor.b) * windowStates[windowIndex].brightness;

                    glColor3f(r, g, b);
                    drawRect(wx, wy, windowWidth, windowHeight);

                    windowIndex++;
                }
            }
        }
    };

    class GlassSkyscraper : public Building {
    private:
        std::vector<WindowState> windowStates;
        bool wasNightInternal = false;
        int numWindowRows, numWindowCols;

    public:
        GlassSkyscraper(float _x, float _y, float _width, float _height)
            : Building(_x, _y, _width, _height) {

            numWindowRows = 25;
            numWindowCols = 5;
            windowStates.resize(numWindowRows * numWindowCols);
        }

        void update() override {
            bool transitionToNight = isNight && !wasNightInternal;
            bool transitionToDay = !isNight && wasNightInternal;
            wasNightInternal = isNight;

            if (transitionToNight) {
                for (auto& window : windowStates) {
                    window.isLitTarget = (rand() % 6 == 0);
                }
            } else if (transitionToDay) {
                for (auto& window : windowStates) {
                    window.isLitTarget = false;
                }
            }

            if (isNight && frameCount % 10 == 0) {
                if (rand() % 3 == 0) {
                    int index = rand() % windowStates.size();
                    windowStates[index].isLitTarget = !windowStates[index].isLitTarget;
                }
            }

            for (auto& window : windowStates) {
                float targetBrightness = window.isLitTarget ? 1.0f : 0.0f;
                window.brightness = targetBrightness; 
            }
        }

        void draw() override {

            setObjectColor(0.3f, 0.35f, 0.4f);
            drawRect(x, y, width, height);
            glBegin(GL_QUADS);
            setObjectColor(0.5f, 0.6f, 0.7f);
            glVertex2f(x + 5, y);
            glVertex2f(x + width, y);
            setObjectColor(0.8f, 0.85f, 0.9f);
            glVertex2f(x + width, y + height);
            glVertex2f(x + 5, y + height);
            glEnd();

            float w_gap = 4.0f;
            float h_gap = 4.0f;
            float windowWidth = (width - 5 - (numWindowCols + 1) * w_gap) / numWindowCols;
            float windowHeight = (height - (numWindowRows + 1) * h_gap) / numWindowRows;

            Color dayColor = {0.4f, 0.5f, 0.6f}; 
            Color litColor = {0.9f, 0.9f, 0.7f}; 

            int windowIndex = 0;
            for (int row = 0; row < numWindowRows; ++row) {
                for (int col = 0; col < numWindowCols; ++col) {
                    float wx = x + 5 + w_gap + col * (windowWidth + w_gap);
                    float wy = y + h_gap + row * (windowHeight + h_gap);

                    float r = dayColor.r + (litColor.r - dayColor.r) * windowStates[windowIndex].brightness;
                    float g = dayColor.g + (litColor.g - dayColor.g) * windowStates[windowIndex].brightness;
                    float b = dayColor.b + (litColor.b - dayColor.b) * windowStates[windowIndex].brightness;

                    glColor3f(r, g, b);
                    drawRect(wx, wy, windowWidth, windowHeight);

                    windowIndex++;
                }
            }
        }
    };

    class ModernOfficeBuilding : public Building {
    private:
        
        std::vector<WindowState> floorStates;
        bool wasNightInternal = false;
        int numFloors = 4; 
        int numWindowsPerFloor = 10; 

    public:
        ModernOfficeBuilding(float _x, float _y, float _width, float _height)
            : Building(_x, _y, _width, _height) {
            floorStates.resize(numFloors);
        }

        void update() override {
            bool transitionToNight = isNight && !wasNightInternal;
            bool transitionToDay = !isNight && wasNightInternal;
            wasNightInternal = isNight;

            if (transitionToNight) {
                
                
                for (auto& floor : floorStates) {
                    floor.isLitTarget = (rand() % 4 == 0); 
                }
            } else if (transitionToDay) {
                
                for (auto& floor : floorStates) {
                    floor.isLitTarget = false;
                }
            }

            
            for (auto& floor : floorStates) {
                float targetBrightness = floor.isLitTarget ? 1.0f : 0.0f;
                
                floor.brightness = targetBrightness;
            }
        }

        void draw() override {
            
            setObjectColor(0.85f, 0.85f, 0.8f);
            drawRect(x, y, width, height);

            
            setObjectColor(0.7f, 0.7f, 0.65f);
            for (int i = 1; i < 5; ++i) {
                float lineX = x + i * (width / 5.0f);
                drawLine(lineX, y, lineX, y + height);
            }

            
            float w_gap = 2.0f;
            float h_gap = 12.0f;
            float windowWidth = (width - (numWindowsPerFloor + 1) * w_gap) / numWindowsPerFloor;
            float windowHeight = (height - (numFloors + 1) * h_gap) / numFloors;

            Color dayColor = {0.4f, 0.5f, 0.6f};
            Color litColor = {0.9f, 0.9f, 0.7f};

            for (int floor = 0; floor < numFloors; ++floor) {
                float currentBrightness = floorStates[floor].brightness;

                float r = dayColor.r + (litColor.r - dayColor.r) * currentBrightness;
                float g = dayColor.g + (litColor.g - dayColor.g) * currentBrightness;
                float b = dayColor.b + (litColor.b - dayColor.b) * currentBrightness;
                glColor3f(r, g, b);

                for (int col = 0; col < numWindowsPerFloor; ++col) {
                    float wx = x + w_gap + col * (windowWidth + w_gap);
                    float wy = y + h_gap + floor * (windowHeight + h_gap);
                    drawRect(wx, wy, windowWidth, windowHeight);
                }
            }
        }
    };


    class ClassicApartment : public Building {
    private:
        std::vector<WindowState> windowStates;
        bool wasNightInternal = false;

        
        std::vector<SmokeParticle> smokeParticles;

    public:
        ClassicApartment(float _x, float _y, float _width, float _height)
            : Building(_x, _y, _width, _height) {
            windowStates.resize(12); 
        }

        void update() override {
            
            bool transitionToNight = isNight && !wasNightInternal;
            bool transitionToDay = !isNight && wasNightInternal;
            wasNightInternal = isNight;

            if (transitionToNight) {
                for (auto& window : windowStates) {
                    window.isLitTarget = (rand() % 2 == 0);
                }
            } else if (transitionToDay) {
                for (auto& window : windowStates) {
                    window.isLitTarget = false;
                }
            }

            for (auto& window : windowStates) {
                float targetBrightness = window.isLitTarget ? 1.0f : 0.0f;
                window.brightness = targetBrightness;
            }

            
            
            if (frameCount % 15 == 0) { 
                float chimneyTopX = x + width - 20;
                float chimneyTopY = y + height + 20;
                smokeParticles.push_back({
                    chimneyTopX,
                    chimneyTopY,
                    3.0f,                  
                    1.0f,                  
                    0.1f + (rand() % 20) / 100.0f 
                });
            }

            
            for (auto& p : smokeParticles) {
                p.y += 0.5f;           
                p.x += p.x_drift;      
                p.size += 0.1f;          
                p.alpha -= 0.008f;     
            }

            
            smokeParticles.erase(
                std::remove_if(smokeParticles.begin(), smokeParticles.end(), [](const SmokeParticle& p) {
                    return p.alpha <= 0.0f;
                }),
                smokeParticles.end()
            );
        }

        void draw() override {
            
            
            setObjectColor(0.9f, 0.85f, 0.7f); 
            drawRect(x, y, width, height);

            
            setObjectColor(0.6f, 0.4f, 0.2f); 
            drawRect(x - 5, y + height - 10, width + 10, 15);

            
            float windowWidth = width / 6.0f;
            float windowHeight = height / 5.0f;
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 4; ++col) {
                    float wx = x + (col + 1) * (width / 5.0f) - windowWidth / 2;
                    float wy = y + (row + 0.5f) * (height / 3.5f) - windowHeight / 2;
                    int windowIndex = row * 4 + col;
                    setObjectColor(0.8f, 0.75f, 0.6f);
                    drawRect(wx - 2, wy - 2, windowWidth + 4, 4);
                    float r = 0.2f + 0.8f * windowStates[windowIndex].brightness;
                    float g = 0.2f + 0.7f * windowStates[windowIndex].brightness;
                    float b = 0.3f + 0.5f * windowStates[windowIndex].brightness;
                    glColor3f(r, g, b);
                    drawRect(wx, wy, windowWidth, windowHeight);
                }
            }

            
            for (const auto& p : smokeParticles) {
                
                glColor4f(0.7f, 0.7f, 0.7f, p.alpha);
                drawCircle(p.x, p.y, p.size);
            }

            setObjectColor(0.5f, 0.35f, 0.3f);
            drawRect(x + width - 25, y + height - 5, 15, 30);

        }
    };

    class Shop : public Building {
    private:
        std::string shopName;
        Color awningColor;
        bool isOpen;
        bool wasNightInternal = false; 

    public:
        Shop(float _x, float _y, float _width, float _height, std::string _name, Color _color)
            : Building(_x, _y, _width, _height), shopName(_name), awningColor(_color), isOpen(true) {}

        void update() override {
            bool transitionToNight = isNight && !wasNightInternal;
            bool transitionToDay = !isNight && wasNightInternal;
            wasNightInternal = isNight;

            if (transitionToNight) {
                
                
                isOpen = (rand() % 4 != 0); 
            } else if (transitionToDay) {
                
                isOpen = true;
            }
        }

        void draw() override {
            
            setObjectColor(0.85f, 0.8f, 0.7f); 
            drawRect(x, y, width, height);

            
            float awningY = y + height * 0.65f;
            setObjectColor(awningColor.r, awningColor.g, awningColor.b);
            drawRect(x, awningY, width, 12);
            setObjectColor(awningColor.r * 0.8f, awningColor.g * 0.8f, awningColor.b * 0.8f);
            for (int i = 0; i < width; i += 10) {
                drawRect(x + i, awningY, 5, 12);
            }

            
            
            float windowX = x + 10;
            float windowY = y + 10;
            float windowW = width - 20;
            float windowH = height * 0.5f - 10;

            if (isOpen && isNight) {
                
                glColor3f(1.0f, 1.0f, 0.85f);
            } else {
                
                setObjectColor(0.3f, 0.3f, 0.4f);
            }
            drawRect(windowX, windowY, windowW, windowH);

            
            
            setObjectColor(0.4f, 0.4f, 0.4f);

            if (shopName == "CAFE") {
                
                drawRect(windowX, windowY, windowW, windowH * 0.25f);
                
                drawRect(windowX + windowW * 0.6f, windowY + windowH * 0.25f, windowW * 0.3f, windowH * 0.4f);
                
                drawLine(windowX + windowW * 0.1f, windowY + windowH * 0.6f, windowX + windowW * 0.4f, windowY + windowH * 0.6f, 2.0f);
                drawCircle(windowX + windowW * 0.15f, windowY + windowH * 0.6f + 5, 3);
                drawCircle(windowX + windowW * 0.25f, windowY + windowH * 0.6f + 5, 3);
                drawCircle(windowX + windowW * 0.35f, windowY + windowH * 0.6f + 5, 3);

            } else if (shopName == "BOOKS") {
                for (int i = 0; i < 3; ++i) {
                    float shelfX = windowX + windowW * 0.1f + i * (windowW * 0.3f);
                    
                    drawRect(shelfX, windowY, windowW * 0.25f, windowH * 0.9f);
                    
                    for (int j = 0; j < 3; ++j) {
                        float bookY = windowY + windowH * 0.1f + j * (windowH * 0.3f);
                        drawRect(shelfX + 2, bookY, windowW * 0.2f, windowH * 0.25f);
                    }
                }
            }
            
            

            
            setObjectColor(0.2f, 0.2f, 0.2f);
            drawBound(windowX, windowY, windowW, windowH);

            
            setObjectColor(0.2f, 0.2f, 0.2f);
            drawRect(x, y + height - 30, width, 30);
            setObjectColor(1.0f, 1.0f, 1.0f);
            drawText(x + 15, y + height - 22, shopName.c_str(), 0.18f);

            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    };

    void initStreetLamps(int numLamps = 3) {
        float spacing;
        float startX;

        if (numLamps > 1) {
            
            float effectiveSpreadWidth = WINDOW_WIDTH * 0.8f; 
            spacing = effectiveSpreadWidth / (numLamps - 1.0f);
            startX = (WINDOW_WIDTH / 2.0f) - (effectiveSpreadWidth / 2.0f);
        } else { 
            spacing = 0.0f; 
            startX = WINDOW_WIDTH / 2.0f; 
        }

        
        for (int i = 0; i < numLamps; ++i) {
            float lampX = startX + i * spacing;
            drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<StreetLamp>(lampX, SIDEWALK_TOP_Y_START)));
        }

        
        const float phaseShift = 50.0f; 
        for (int i = 0; i < numLamps; ++i) {
            float lampX = startX + i * spacing + phaseShift;
            drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<StreetLamp>(lampX, SIDEWALK_BOTTOM_Y_END)));
        }
    }

    void drawGrass(float x, float y, float scale = 1.0f) {
        setObjectColor(0.2f, 0.7f, 0.2f);
        glBegin(GL_LINES);
        for (int i = -2; i <= 2; ++i) {
            glVertex2f(x, y);
            glVertex2f(x + i * 2 * scale, y + 12 * scale - abs(i) * 2 * scale);
        }
        glEnd();
    }

    void drawFlower(float x, float y, float scale = 1.0f) {
        
        setObjectColor(0.2f, 0.7f, 0.2f);
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x, y + 10 * scale);
        glEnd();
        
        setObjectColor(1.0f, 0.7f, 0.2f); 
        for (int i = 0; i < 5; ++i) {
            float angle = i * 2 * 3.14159f / 5;
            float px = x + cosf(angle) * 4 * scale;
            float py = y + 10 * scale + sinf(angle) * 4 * scale;
            drawCircle(px, py, 2.0f * scale, 8);
        }
        
        setObjectColor(1.0f, 0.2f, 0.5f); 
        drawCircle(x, y + 10 * scale, 2.2f * scale, 10);
    }

    class Tree : public Drawable {
    public:
        Tree(float x, float y, float w, float h) : Drawable(x, y, w, h) {}
        virtual ~Tree() = default;
    };

    class BasicTree : public Tree {
    public:
        BasicTree(float x, float y) : Tree(x, y, 40, 80) {}
        void draw() override {
            setObjectColor(0.5f, 0.3f, 0.05f);
            glBegin(GL_QUADS);
            glVertex2f(x - 8, y);  
            glVertex2f(x + 8, y);
            glVertex2f(x + 6, y + 50);  
            glVertex2f(x - 6, y + 50);
            glEnd();

            setObjectColor(0.0f, 0.45f, 0.05f);
            drawCircle(x, y + 75, 25);  
            drawCircle(x - 15, y + 65, 20);  
            drawCircle(x + 15, y + 65, 20);  
            setObjectColor(0.1f, 0.55f, 0.1f);
            drawCircle(x, y + 80, 18);  
        }
        void update() override {
            
        }
    };

    class PineTree : public Tree {
    public:
        PineTree(float x, float y) : Tree(x, y, 50, 130) {}
        void draw() override {
            
            setObjectColor(0.5f, 0.3f, 0.05f);
            glBegin(GL_QUADS);
            glVertex2f(x - 6, y);  
            glVertex2f(x + 6, y);
            glVertex2f(x + 4, y + 60);  
            glVertex2f(x - 4, y + 60);
            glEnd();

            
            setObjectColor(0.0f, 0.4f, 0.1f);
            
            glBegin(GL_TRIANGLES);
            glVertex2f(x - 35, y + 60);  
            glVertex2f(x + 35, y + 60);
            glVertex2f(x, y + 85);  
            glEnd();
            
            glBegin(GL_TRIANGLES);
            glVertex2f(x - 30, y + 75);  
            glVertex2f(x + 30, y + 75);
            glVertex2f(x, y + 100);  
            glEnd();
            
            glBegin(GL_TRIANGLES);
            glVertex2f(x - 25, y + 90);  
            glVertex2f(x + 25, y + 90);
            glVertex2f(x, y + 115);  
            glEnd();
            
            glBegin(GL_TRIANGLES);
            glVertex2f(x - 20, y + 105);  
            glVertex2f(x + 20, y + 105);
            glVertex2f(x, y + 130);  
            glEnd();
            
            glBegin(GL_TRIANGLES);
            glVertex2f(x - 15, y + 120);  
            glVertex2f(x + 15, y + 120);
            glVertex2f(x, y + 145);  
            glEnd();
            
            setObjectColor(0.0f, 0.35f, 0.05f);
            glBegin(GL_TRIANGLES);
            glVertex2f(x - 25, y + 70);  
            glVertex2f(x + 25, y + 70);
            glVertex2f(x, y + 95);  
            glEnd();
        }
        void update() override {
            
        }
    };

    class MapleTree : public Tree {
    public:
        MapleTree(float x, float y) : Tree(x, y, 70, 120) {}
        void draw() override {
            
            setObjectColor(0.5f, 0.3f, 0.05f);
            glBegin(GL_QUADS);
            glVertex2f(x - 10, y);
            glVertex2f(x + 10, y);
            glVertex2f(x + 8, y + 80);
            glVertex2f(x - 8, y + 80);
            glEnd();
            
            glBegin(GL_QUADS);
            glVertex2f(x - 8, y + 60);
            glVertex2f(x + 8, y + 60);
            glVertex2f(x + 6, y + 80);
            glVertex2f(x - 6, y + 80);
            glEnd();
            
            setObjectColor(0.0f, 0.5f, 0.1f);
            for (int i = 0; i < 3; i++) {
                float yy = y + 80 + i * 20;
                float radius = 35 - i * 5;
                drawCircle(x, yy, radius);
            }
        }
        void update() override {
            
        }
    };

    class BlurrySkyline : public Drawable {
    private:
        struct BuildingProps {
            float xOffset;
            float height;
            float width;
            int topType;
            float topBlockWidth;
            float topBlockHeight;
        };
        std::vector<BuildingProps> buildings;

    public:
        BlurrySkyline(float x, float y, float w, float h) : Drawable(x, y+15, w, h) {
            float currentX = 0.0f;
            while (currentX < width) {
                BuildingProps newBuilding;
                newBuilding.xOffset = currentX;
                newBuilding.height = 50.0f + (rand() % 150); 
                newBuilding.width = 40.0f; 

                newBuilding.topType = rand() % 4; 
                if (newBuilding.topType == 1) {
                    newBuilding.topBlockWidth = 20.0f + (rand() % 15);
                    newBuilding.topBlockHeight = 20.0f + (rand() % 30);
                }

                buildings.push_back(newBuilding);

                
                currentX += newBuilding.width; 
            }
        }

        void draw() override {

            glPushMatrix();
            glTranslatef(0, -15.0f, 0); 

            
            glColor3f(0.35f, 0.35f, 0.35f); 

            for (const auto& building : buildings) {
                
                drawRect(x + building.xOffset, y, building.width, building.height);

                
                if (building.topType == 1) {
                    drawRect(x + building.xOffset + (building.width - building.topBlockWidth) / 2, y + building.height, building.topBlockWidth, building.topBlockHeight);
                } else if (building.topType == 2) {
                    drawTriangle(x + building.xOffset, y + building.height,
                                x + building.xOffset + building.width, y + building.height,
                                x + building.xOffset, y + building.height + (building.width / 2.0f));
                } else if (building.topType == 3) {
                    drawTriangle(x + building.xOffset, y + building.height,
                                x + building.xOffset + building.width, y + building.height,
                                x + building.xOffset + building.width, y + building.height + (building.width / 2.0f));
                }
            }
            glPopMatrix();
        }

        void update() override {

        }
    };

    class StreetLamp : public Drawable {
    private:
        bool isLid = false;
        int timePassed = 0;
        int randomDelay = 0;  // Random delay before state change
        bool isTurningOff = false;  // Track if we're in the process of turning off

    public:
        StreetLamp(float x, float y) : Drawable(x, y, 36, 120) {
            // Set initial random delay (10-100 frames)
            randomDelay = 10 + (rand() % 91);
        }

        void update() override {
            if (isNight) {
                if (!isLid && !isTurningOff) {
                    // Turning on
                    timePassed++;
                    if (timePassed >= randomDelay) {
                        isLid = true;
                        timePassed = 0;
                    }
                }
            } else {
                if (isLid && !isTurningOff) {
                    // Start turning off
                    isTurningOff = true;
                    timePassed = 0;
                    randomDelay = 10 + (rand() % 91);  // New random delay for turning off
                } else if (isTurningOff) {
                    // In process of turning off
                    timePassed++;
                    if (timePassed >= randomDelay) {
                        isLid = false;
                        isTurningOff = false;
                        timePassed = 0;
                        randomDelay = 10 + (rand() % 91);  // New random delay for next turn on
                    }
                }
            }
        }

        void draw() override {
            glPushMatrix();
            glTranslatef(x, y, 0);

            
            glBegin(GL_QUADS);
            
            setObjectColor(0.3f, 0.3f, 0.3f);
            glVertex2f(-4, 0);
            glVertex2f(4, 0);
            
            setObjectColor(0.5f, 0.5f, 0.5f);
            glVertex2f(4, 120);
            glVertex2f(-4, 120);
            glEnd();

            
            setObjectColor(0.4f, 0.4f, 0.4f);
            for (int i = 0; i < 3; i++) {
                float y_pos = 30 + i * 30;
                drawRect(-5, y_pos, 10, 2);
            }

            
            setObjectColor(0.4f, 0.4f, 0.4f);
            
            glBegin(GL_QUADS);
            glVertex2f(0, 115);
            glVertex2f(35, 115);
            glVertex2f(35, 110);
            glVertex2f(0, 110);
            glEnd();

            
            glBegin(GL_QUADS);
            glVertex2f(35, 115);
            glVertex2f(40, 110);
            glVertex2f(40, 105);
            glVertex2f(35, 110);
            glEnd();

            
            setObjectColor(0.2f, 0.2f, 0.2f);
            
            glBegin(GL_QUADS);
            glVertex2f(35, 105);
            glVertex2f(45, 105);
            glVertex2f(45, 95);
            glVertex2f(35, 95);
            glEnd();

            
            glBegin(GL_QUADS);
            glVertex2f(38, 95);
            glVertex2f(42, 95);
            glVertex2f(42, 90);
            glVertex2f(38, 90);
            glEnd();

            
            setObjectColor(0.3f, 0.3f, 0.3f);
            
            for (int i = 0; i < 3; i++) {
                float x_pos = 10 + i * 10;
                drawRect(x_pos, 112, 2, 6);
            }

            
            if (isNight || isTurningOff) {
                if (isLid) {
                    drawLights();
                } else {
                    setObjectColor(0.6f, 0.6f, 0.5f);
                    drawCircle(40, 100, 5);
                }
            } else {
                setObjectColor(0.6f, 0.6f, 0.5f);
                drawCircle(40, 100, 5);
            }

            glPopMatrix();
        }

        void drawLights() {
            glColor4f(1.0f, 0.95f, 0.8f, 0.15f);
            drawCircle(40, 100, 25);

            
            glColor4f(1.0f, 0.95f, 0.8f, 0.3f);
            drawCircle(40, 100, 15);

            
            glColor4f(1.0f, 0.95f, 0.8f, 0.5f);
            drawCircle(40, 100, 8);

            
            glColor3f(1.0f, 0.95f, 0.8f);
            drawCircle(40, 100, 5);

            
            glColor4f(1.0f, 0.95f, 0.8f, 0.2f);
            glBegin(GL_TRIANGLES);
            glVertex2f(40, 90);  
            glVertex2f(20, 0);  
            glVertex2f(60, 0);  
            glEnd();

            
            glColor4f(1.0f, 0.95f, 0.8f, 0.1f);
            drawCircle(40, 0, 25); 
        }
    };


    class PostBox : public Drawable {
    public:
        PostBox(float x, float y) : Drawable(x, y, 18, 40) {}
        void draw() override {
            
            setObjectColor(0.85f, 0.1f, 0.1f); 
            drawRect(x, y, width, height - 8);
            
            setObjectColor(0.7f, 0.07f, 0.07f);
            drawCircle(x + width / 2, y + height - 8, width / 2, 16);
            
            setObjectColor(0.1f, 0.1f, 0.1f);
            drawRect(x + 4, y + height - 18, width - 8, 4);
            
            setObjectColor(0.3f, 0.1f, 0.1f);
            drawRect(x + 2, y - 4, width - 4, 6);
        }
        void update() override {
            
        }
    };


    class Bench : public Drawable {
    public:
        Bench(float x, float y) : Drawable(x, y, 50, 12) {}
        void draw() override {
            
            setObjectColor(0.6f, 0.4f, 0.2f); 
            drawRect(x, y, width, 4);
            
            setObjectColor(0.6f, 0.4, 0.2f); 
            drawRect(x, y + 4, width, 4);
            
            setObjectColor(0.5f, 0.3f, 0.15f);
            drawRect(x, y + 8, width, 3);
            
            setObjectColor(0.2f, 0.2f, 0.2f); 
            drawRect(x + 2, y - 6, 4, 10);
            drawRect(x + width - 6, y - 6, 4, 10);
        }
        void update() override {
            
        }
    };

    void spawnNewVehicle() {
        
        std::shared_ptr<Vehicle> vehicleToReuse = nullptr;
        for (const auto& vehicle : vehicles) {
            if (vehicle->state == Vehicle::VehicleState::DESPAWNED) {
                vehicleToReuse = vehicle;
                break;
            }
        }

        
        if (!vehicleToReuse) return;

        float spawnW, spawnH;
        float wheel_radius;

        
        switch (vehicleToReuse->type) {
            case Vehicle::VehicleType::CAR:
                spawnW = 80.0f; spawnH = 35.0f;
                break;
            case Vehicle::VehicleType::SUV:
                spawnW = 90.0f; spawnH = 45.0f;
                break;
            case Vehicle::VehicleType::VAN:
                spawnW = 100.0f; spawnH = 50.0f;
                break;
            case Vehicle::VehicleType::TRUCK:
                spawnW = 150.0f; spawnH = 60.0f;
                break;
            case Vehicle::VehicleType::BUS:
                spawnW = 160.0f; spawnH = 50.0f;
                break;
        }

        
        if (vehicleToReuse->type == Vehicle::VehicleType::SUV) {
            
            wheel_radius = spawnH * 0.3f;
        } else {
            
            wheel_radius = spawnH * 0.25f;
        }

        float spawnX = -spawnW - 150.0f; 
        float spawnY;

        if (rand() % 2 == 0) {
            
            
            
            
            spawnY = ROAD_Y_BOTTOM + wheel_radius - VEHICLE_WHEEL_OFFSET;
        } else {
            
            
            
            
            spawnY = ROAD_Y_TOP - spawnH - VEHICLE_WHEEL_OFFSET - 5.0f;
        }

        
        for (const auto &v : vehicles) {
            if (v->state == Vehicle::VehicleState::ACTIVE && fabs(v->y - spawnY) < CAR_SAME_LANE_Y_THRESHOLD) {
                if (v->x < spawnX + MIN_CAR_SPAWN_DISTANCE) {
                    return; 
                }
            }
        }

        float r = 0.4f + (rand() % 7) / 10.0f; 
        float g = 0.4f + (rand() % 7) / 10.0f; 
        float b = 0.4f + (rand() % 7) / 10.0f;

        
        vehicleToReuse->initialize(spawnX, spawnY, r, g, b);
    }

    void updateVehiclesStates() {
        
        for (auto& vehicle : vehicles) {
            if (vehicle->state == Vehicle::VehicleState::ACTIVE) {
                vehicle->update(); 
                if (vehicle->isOutOfScreen()) {
                    vehicle->reset(); 
                }
            }
        }

        
        int activeCount = 0;
        for (const auto& vehicle : vehicles) {
            if (vehicle->state == Vehicle::VehicleState::ACTIVE) {
                activeCount++;
            }
        }

        
        if (activeCount < MAX_ACTIVE_VEHICLES && rand() % CAR_SPAWN_RATE == 0) {
            spawnNewVehicle();
        }
    }

    void updateSky() {
            
        float sunX = WINDOW_WIDTH * 0.3f;  
        float moonX = WINDOW_WIDTH * 0.8f; 
        float celestialY = WINDOW_HEIGHT * 1.2f; 

        if (currentTimeOfDay >= 0.05 && currentTimeOfDay < 0.80) {
            
            
            float sunY = celestialY - (currentTimeOfDay - 0.2f) * WINDOW_HEIGHT * 1.4f;
            drawSun(sunX, sunY, 30.0f);
        }

        if (currentTimeOfDay >= 0.70 || currentTimeOfDay < 0.20) {
            
            
            float moonY = (WINDOW_HEIGHT/3) + (currentTimeOfDay > 0.5f ?
                (currentTimeOfDay - 0.7f) : (currentTimeOfDay + 0.3f)) * WINDOW_HEIGHT * 1.5;
            drawMoon(moonX, moonY, 25.0f);
        }

        updateStars();
        updateShootingStars();
        
        // Draw shooting stars
        for (auto& shootingStar : shootingStars) {
            shootingStar->draw();
        }
    }



    class Bush : public Drawable {
    private:
        
        struct Puff {
            float offsetX, offsetY;
            float radius;
            bool isDarker; 
        };

        
        std::vector<Puff> puffs;

    public:
        Bush(float x, float y, float scale) : Drawable(x, y, 50 * scale, 30 * scale) {
            
            int num_puffs = 5 + (rand() % 4); 
            for (int i = 0; i < num_puffs; ++i) {
                
                puffs.push_back({
                    (float)(rand() % (int)(30 * scale) - 15 * scale), 
                    (float)(rand() % (int)(15 * scale) - 7 * scale),  
                    (10.0f + (rand() % 5)) * scale,                  
                    (i % 2 == 0)                                     
                });
            }
        }

        void draw() override {
            
            
            for (const auto& puff : puffs) {
                if (puff.isDarker) {
                    setObjectColor(0.1f, 0.45f, 0.1f);
                } else {
                    setObjectColor(0.15f, 0.5f, 0.15f);
                }
                
                drawCircle(x + puff.offsetX, y + puff.offsetY, puff.radius);
            }
        }

        void update() override {
            
        }
    };

    void initGroundDecorator() {
        for (int i = 0; i < 6; ++i) {  
            
            float x = 80.0f + (rand() % (WINDOW_WIDTH - 160));  
            
            
            float y = SIDEWALK_BOTTOM_Y_START - (40.0f + (rand() % 80));  
            
            float scale = 1.0f + (rand() % 3) / 10.0f;  
            drawableObjects.push_back(std::make_shared<Bush>(x, y, scale));
        }
    }

    void initBuildings() {
        backgroundObjects.push_back(std::make_shared<BlurrySkyline>(0, SIDEWALK_TOP_Y_END, WINDOW_WIDTH, 100));
        backgroundObjects.push_back(std::make_shared<BrickBuilding>(80.0f, SIDEWALK_TOP_Y_END, 120.0f, 140.0f));
        backgroundObjects.push_back(std::make_shared<Shop>(220.0f, SIDEWALK_TOP_Y_END, 90.0f, 100.0f, "CAFE", Color{0.8f, 0.2f, 0.2f}));
        backgroundObjects.push_back(std::make_shared<Shop>(320.0f, SIDEWALK_TOP_Y_END, 100.0f, 100.0f, "BOOKS", Color{0.2f, 0.2f, 0.8f}));
        backgroundObjects.push_back(std::make_shared<GlassSkyscraper>(570.0f, SIDEWALK_TOP_Y_END, 80.0f, 280.0f));
        backgroundObjects.push_back(std::make_shared<ModernOfficeBuilding>(680.0f, SIDEWALK_TOP_Y_END, 100.0f, 200.0f));
        backgroundObjects.push_back(std::make_shared<ClassicApartment>(440.0f, SIDEWALK_TOP_Y_END, 110.0f, 160.0f));
        backgroundObjects.push_back(std::make_shared<BrickBuilding>(800.0f, SIDEWALK_TOP_Y_END, 150.0f, 160.0f));
    }

    void drawGround() {
        setObjectColor(0.35f, 0.7f, 0.25f); 
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(WINDOW_WIDTH, 0);
        glVertex2f(WINDOW_WIDTH, SIDEWALK_BOTTOM_Y_START);
        glVertex2f(0, SIDEWALK_BOTTOM_Y_START);
        glEnd();

        
        for (int i = 0; i < 25; ++i) {
            float gx = 20 + i * 38;
            float gy = 10 + (i * 13) % (int)(SIDEWALK_BOTTOM_Y_START * 0.5f);
            float scale = 0.8f + ((i * 7) % 10) / 20.0f;
            drawGrass(gx, gy, scale);
        }

        
        for (int i = 0; i < 10; ++i) {
            float fx = 35 + i * 90;
            float fy = 18 + (i * 23) % (int)(SIDEWALK_BOTTOM_Y_START * 0.5f);
            float scale = 0.8f + ((i * 5) % 10) / 20.0f;
            drawFlower(fx, fy, scale);
        }
    }


    void drawRoadAndSidewalks()
    {
        setObjectColor(0.4f, 0.4f, 0.4f);
        glBegin(GL_QUADS);
        glVertex2f(0, ROAD_Y_BOTTOM);
        glVertex2f(WINDOW_WIDTH, ROAD_Y_BOTTOM);
        glVertex2f(WINDOW_WIDTH, ROAD_Y_TOP);
        glVertex2f(0, ROAD_Y_TOP);
        glEnd();
        setObjectColor(0.75f, 0.75f, 0.75f);
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
        setObjectColor(0.9f, 0.9f, 0.9f);
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
            if (v->state == Vehicle::VehicleState::ACTIVE && v->x < HUMAN_CROSSING_X_START && v->x > HUMAN_CROSSING_X_START - CAR_PRIORITY_THRESHOLD) {
                count++;
            }
        }
        return count;
    }

    void drawZebraCrossing(float road_y_bottom, float road_y_top, float crossing_area_x_start, float crossing_area_width)
    {
        setObjectColor(0.95f, 0.95f, 0.95f);
        float offset = -5.0f;
        
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

        
        if (DEBUG_ON) {  
            
            glColor3f(1.0f, 0.0f, 1.0f);  
            drawBound(crossing_area_x_start, road_y_bottom, crossing_area_width, road_y_top - road_y_bottom);
            
            glColor3f(1.0f, 0.0f, 1.0f);
            debugCalls.push_back([=]() {
                drawText(crossing_area_x_start, road_y_top + 10, "Main Crossing Area", 0.5f);
            });
            
            char humanCountText[64];
            snprintf(humanCountText, sizeof(humanCountText), "Humans waiting: %d", HumansWaitingToCross());
            debugCalls.push_back([=]() {
                drawText(crossing_area_x_start, road_y_top + 25, humanCountText, 0.5f);
            });

            
            glColor3f(1.0f, 0.5f, 1.0f);  
            drawBound(crossing_area_x_start - DISTANCE_TO_STOP_FROM_SIGNAL, road_y_bottom, crossing_area_width + DISTANCE_TO_STOP_FROM_SIGNAL * 2, road_y_top - road_y_bottom);
            
            glColor3f(1.0f, 0.5f, 1.0f);
            debugCalls.push_back([=]() {
                drawText(crossing_area_x_start - DISTANCE_TO_STOP_FROM_SIGNAL, road_y_top + 40, "Extended Waiting Area", 0.5f);
            });

            
            glColor3f(0.0f, 1.0f, 0.5f);  
            drawBound(crossing_area_x_start - CAR_PRIORITY_THRESHOLD, road_y_bottom, CAR_PRIORITY_THRESHOLD, road_y_top - road_y_bottom);
            
            glColor3f(0.0f, 1.0f, 0.5f);
            debugCalls.push_back([=]() {
                drawText(crossing_area_x_start - CAR_PRIORITY_THRESHOLD, road_y_top + 55, "Car Priority Area", 0.5f);
            });
            
            char carCountText[64];
            snprintf(carCountText, sizeof(carCountText), "Cars in area: %d", countCarsNearCrossing());
            glColor3f(0.0f, 1.0f, 0.5f);
            debugCalls.push_back([=]() {
                drawText(crossing_area_x_start - CAR_PRIORITY_THRESHOLD, road_y_top + 70, carCountText, 0.5f);
            });
        }
    }

    void updateSkyColor() {
        
        float dayR = 0.5f, dayG = 0.7f, dayB = 1.0f;    
        float dawnR = 0.6f, dawnG = 0.5f, dawnB = 0.4f;  
        float nightR = 0.1f, nightG = 0.1f, nightB = 0.3f;  
        float duskR = 0.6f, duskG = 0.5f, duskB = 0.4f;  
        float r, g, b;

        
        float dawnStart = 0.0f;
        float dawnEnd = 0.2f;
        float dayStart = 0.2f;
        float dayEnd = 0.45f;
        float duskStart = 0.45f;
        float duskEnd = 0.7f;
        float nightStart = 0.7f;
        float nightEnd = 1.0f;

        
        float transitionPeriod = 0.15f;

        
        if (currentTimeOfDay >= dawnStart && currentTimeOfDay < dawnEnd) {
            
            float t = (currentTimeOfDay - dawnStart) / transitionPeriod;
            if (t > 1.0f) t = 1.0f;
            r = nightR + (dawnR - nightR) * t;
            g = nightG + (dawnG - nightG) * t;
            b = nightB + (dawnB - nightB) * t;
        }
        else if (currentTimeOfDay >= dayStart && currentTimeOfDay < dayEnd) {
            
            float t = (currentTimeOfDay - dayStart) / transitionPeriod;
            if (t > 1.0f) t = 1.0f;
            r = dawnR + (dayR - dawnR) * t;
            g = dawnG + (dayG - dawnG) * t;
            b = dawnB + (dayB - dawnB) * t;
        }
        else if (currentTimeOfDay >= duskStart && currentTimeOfDay < duskEnd) {
            
            float t = (currentTimeOfDay - duskStart) / transitionPeriod;
            if (t > 1.0f) t = 1.0f;
            r = dayR + (duskR - dayR) * t;
            g = dayG + (duskG - dayG) * t;
            b = dayB + (duskB - dayB) * t;
        }
        else if (currentTimeOfDay >= nightStart && currentTimeOfDay < nightEnd) {
            
            float t = (currentTimeOfDay - nightStart) / transitionPeriod;
            if (t > 1.0f) t = 1.0f;
            r = duskR + (nightR - duskR) * t;
            g = duskG + (nightG - duskG) * t;
            b = duskB + (nightB - duskB) * t;
        }
        else {
            
            r = nightR;
            g = nightG;
            b = nightB;
        }

        
        if (currentTimeOfDay >= nightStart || currentTimeOfDay < dawnEnd) {
            b = std::max(b, 0.25f);
        }

        
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void humanRunner() {
        
        if (lastHumanSpawnTime != 0 && (timeNow() - lastHumanSpawnTime < MIN_TIME_BETWEEN_SPAWNS_HUMAN * 100)) return;

        
        std::shared_ptr<Human> humanToReuse = nullptr;
        for (const auto& human : activeHumans) {
            if (human->state == Human::HumanState::DESPAWNED) {
                humanToReuse = human;
                break;
            }
        }

        
        if (!humanToReuse) return;

        
        Color redShirt = {0.8f, 0.2f, 0.2f}, tealShirt = {0.1f, 0.5f, 0.5f}, greenShirt = {0.2f, 0.6f, 0.2f};
        Color bluePants = {0.1f, 0.2f, 0.5f}, blackPants = {0.1f, 0.1f, 0.1f}, skyBluePants = {0.3f, 0.5f, 0.9f};
        Color brownHair = {0.4f, 0.2f, 0.1f}, blackHair = {0.1f, 0.1f, 0.1f}, blondeHair = {0.9f, 0.8f, 0.5f};
        Color darkSkin = {0.4f, 0.3f, 0.07f}, lightSkin = {0.9f, 0.8f, 0.7f}, lightBrown = {0.9f, 0.7f, 0.5f};

        std::vector<Color> shirts = {redShirt, tealShirt, greenShirt};
        std::vector<Color> pants = {bluePants, blackPants, skyBluePants};
        std::vector<Color> hairs = {brownHair, blackHair, blondeHair};
        std::vector<Color> skins = {darkSkin, lightSkin, lightBrown};

        
        Color chosenShirt = shirts[rand() % shirts.size()];
        Color chosenPants = pants[rand() % pants.size()];
        Color chosenHair = hairs[rand() % hairs.size()];
        Color chosenSkin = skins[rand() % skins.size()];
        HairStyle chosenStyle = (HairStyle)(rand() % 3);

        
        bool startsOnBottomSidewalk = rand() % 2 == 0;
        bool comesFromLeft = rand() % 2 == 0;
        float startX = comesFromLeft ? -50.0f : WINDOW_WIDTH + 50.0f;
        float startY = startsOnBottomSidewalk ? (SIDEWALK_BOTTOM_Y_START + SIDEWALK_BOTTOM_Y_END) / 2.0f : (SIDEWALK_TOP_Y_START + SIDEWALK_TOP_Y_END) / 2.0f;
        bool willCross = (rand() % 3) == 0;
        
        
        humanToReuse->initialize(startX, startY, 100.0f, chosenShirt, chosenPants, chosenSkin, chosenHair, chosenStyle, startsOnBottomSidewalk, willCross);

        lastHumanSpawnTime = timeNow();
    }


    void updateTime() {
        currentTimeOfDay += USER_DAY_NIGHT_CYCLE_SPEED;
        if (currentTimeOfDay >= 1.0f) {
            currentTimeOfDay -= 1.0f;
        }
        isNight = (currentTimeOfDay > 0.65 || currentTimeOfDay < 0.25);
    }

    static std::function<void()> g_pendingCallback = nullptr;

    void timerCallback(int value) {
        TrafficSignal::yellowLightOn = false;  
        if (g_pendingCallback) {
            g_pendingCallback();  
            g_pendingCallback = nullptr;  
        }
        glutPostRedisplay();
    }

    void showTransitionDelay(std::function<void()> callback, int delay) {

        glutPostRedisplay();

        g_pendingCallback = callback;
        glutTimerFunc(delay, timerCallback, 0);
    }



    unsigned short HumansWaitingToCross() {
        unsigned short count = 0;
        for (const auto& human : activeHumans) {
            if (human->state == Human::HumanState::WAITING_AT_CROSSING_EDGE) {
                count++;
            }
        }
        return count;
    }

    unsigned short HumansCrossing() {
        unsigned short count = 0;
        for (const auto& human : activeHumans) {
            if (human->state == Human::HumanState::CROSSING_ROAD) {
                count++;
            }
        }
        return count;
    }

        void initClouds() {
        for (int i = 0; i < 3; ++i) {
            float x = (rand() % WINDOW_WIDTH);
            float y = WINDOW_HEIGHT * (0.7f + (rand() % 30) / 100.0f);
            float size = 0.5f + (rand() % 20) / 100.0f;
            backgroundObjects.push_back(std::make_shared<Cloud>(x, y, size));
        }
    }

    void initStars() {
        stars.clear();
        for (int i = 0; i < 100; i++) {
            float x = (rand() % WINDOW_WIDTH);
            float y = WINDOW_HEIGHT * (0.7f + (rand() % 30) / 100.0f);
            float size = 1.0f + (rand() % 30) / 10.0f;
            stars.push_back(std::make_shared<Star>(x, y, size));
        }
        
        // Initialize shooting stars
        shootingStars.clear();
        for (int i = 0; i < 5; i++) {
            shootingStars.push_back(std::make_shared<ShootingStar>());
        }
    }

    void initHumans() {
        activeHumans.reserve(MAX_ACTIVE_HUMANS); 
        for (int i = 0; i < MAX_ACTIVE_HUMANS; ++i) {
            std::shared_ptr<Human> newHuman = std::make_shared<Human>();
            activeHumans.push_back(newHuman);
            drawableObjects.push_back(std::static_pointer_cast<Drawable>(newHuman)); 
        }
    }

    void initAudio() {
        if (!audioManager.initialize()) {
            std::cerr << "Failed to initialize audio system" << std::endl;
        } else {
            
            audioManager.loadSound("traffic", "./audio/traffic.wav"); 
            audioManager.loadSound("people", "./audio/people.wav");
            
            audioManager.loadSound("car", "./audio/car.wav");
            
            audioManager.loadSound("bus", "./audio/bus.wav");
            
            audioManager.loadSound("truck", "./audio/truck.wav");

            audioManager.loadSound("beep", "./audio/beep.wav");
            
            audioManager.playSound("traffic", true);
            audioManager.playSound("people", true);
        }
    }

    void initVehicles() {
        vehicles.clear(); 
        vehicles.reserve(MAX_ACTIVE_VEHICLES); 
        
        
        for (int i = 0; i < MAX_ACTIVE_VEHICLES; ++i) {
            std::shared_ptr<Vehicle> newVehicle;
            float r = (rand() % 10) / 10.0f;
            float g = (rand() % 10) / 10.0f;
            float b = (rand() % 10) / 10.0f;
            
            
            switch(i % 5) {
                case 0:
                    newVehicle = std::make_shared<Car>(-1000.0f, 0.0f, 80.0f, 35.0f, r, g, b);
                    break;
                case 1:
                    newVehicle = std::make_shared<Truck>(-1000.0f, 0.0f, 150.0f, 60.0f, r, g, b);
                    break;
                case 2:
                    newVehicle = std::make_shared<Bus>(-1000.0f, 0.0f, 160.0f, 50.0f, r, g, b);
                    break;
                case 3:
                    newVehicle = std::make_shared<Van>(-1000.0f, 0.0f, 100.0f, 50.0f, r, g, b);
                    break;
                case 4:
                    newVehicle = std::make_shared<SUV>(-1000.0f, 0.0f, 90.0f, 45.0f, r, g, b);
                    break;
            }
            vehicles.push_back(newVehicle);
            drawableObjects.push_back(newVehicle);
        }
        
        
        if (!vehicles.empty()) {
            spawnNewVehicle();
        }
    }

    void initTrees() {
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<BasicTree>(180, SIDEWALK_TOP_Y_START + 28)));  
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<PineTree>(560, SIDEWALK_TOP_Y_START + 28)));   
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<PineTree>(700, SIDEWALK_TOP_Y_START + 28)));   
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<BasicTree>(850, SIDEWALK_BOTTOM_Y_START - 2))); 
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<PineTree>(300, SIDEWALK_BOTTOM_Y_START - 2)));  
    }

    void initRoadDecorators() {
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<TrafficSignal>(HUMAN_CROSSING_CENTER_X - 60, SIDEWALK_TOP_Y_START)));
        drawableObjects.push_back(std::static_pointer_cast<Drawable>(std::make_shared<TrafficSignal>(HUMAN_CROSSING_CENTER_X + 60, SIDEWALK_TOP_Y_START - 100)));
        drawableObjects.push_back(std::make_shared<PostBox>(130, SIDEWALK_TOP_Y_END - 8));
        drawableObjects.push_back(std::make_shared<PostBox>(800, SIDEWALK_BOTTOM_Y_START + 8));
        drawableObjects.push_back(std::make_shared<Bench>(250, SIDEWALK_TOP_Y_END - 8));
        drawableObjects.push_back(std::make_shared<Bench>(700, SIDEWALK_BOTTOM_Y_START + 8));
    }

    void updateScene() {
        updateTime();
        updateSkyColor();
        updateSky();

        
        updateVehiclesStates();
        humanRunner();
        
        for (const auto& obj : backgroundObjects) {
            obj->update();
        }
        for (const auto& obj : drawableObjects) {
            obj->update();
        }

        if (!IS_PAUSED) {
            //updateScene();
            frameCount++;
            
            if (MUSIC_ON && !IS_PAUSED && !audioManager.isPlaying("traffic")) {
                audioManager.playSound("traffic", true);
                audioManager.playSound("people", true);
            }
        } else {
            
            audioManager.stopSound("traffic");
            audioManager.stopSound("people");
        }

        glutPostRedisplay();
    }

    void display() {

        drawGround();
        drawRoadAndSidewalks();
        drawZebraCrossing(ROAD_Y_BOTTOM, ROAD_Y_TOP, HUMAN_CROSSING_X_START, HUMAN_CROSSING_WIDTH);

        
        std::sort(drawableObjects.begin(), drawableObjects.end(), [](const auto& a, const auto& b) {
            return a->y > b->y;
        });

        
        for (const auto& obj : backgroundObjects) {
            obj->draw();
        }
        
        for (const auto& obj : drawableObjects) {
            obj->draw();
        }

        if (showWarningMessage)
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            drawText(HUMAN_CROSSING_X_START - 100, SIDEWALK_TOP_Y_START + 100, "People are still passing the road", 0.7f);
        }
        
        drawDebugOverlay();
    }

    void hideWarningMessage(int value) {
        showWarningMessage = false;
        warningMessageActive = false;
        glutPostRedisplay();
    }

    void keyboard(unsigned char key, int x, int y) {
        switch (key) {
            case 't':
            case 'T':
                if (TrafficSignal::lightState == TrafficSignal::TrafficLightState::RED) {
                    // show warning message if people are still crossing
                    if (HumansCrossing() > 0) {
                        showWarningMessage = true;
                        // Cancel any existing timer by setting the flag
                        warningMessageActive = false;
                        // Set new timer to hide message after 3 seconds
                        warningMessageActive = true;
                        glutTimerFunc(3000, hideWarningMessage, 0);
                    } else {
                        showWarningMessage = false;
                        warningMessageActive = false;
                    }
                    TrafficSignal::yellowLightOn = true;
                    showTransitionDelay([&]() { TrafficSignal::showGreenLight(); }, 1000);
                }
                else if (TrafficSignal::lightState == TrafficSignal::TrafficLightState::GREEN) {
                    TrafficSignal::yellowLightOn = true;
                    showTransitionDelay([&]() { TrafficSignal::showRedLight(); }, 1000);
                }
                break;
            case 'p':
            case 'P':
                IS_PAUSED = !IS_PAUSED;
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
                
                if (MUSIC_ON) { 
                    audioManager.stopSound("traffic");
                    audioManager.stopSound("people");
                    MUSIC_ON = false;
                } else { 
                    audioManager.playSound("traffic", true);
                    audioManager.playSound("people", true);
                    MUSIC_ON = true;
                }
                break;
            case 27: 
                
                vehicles.clear();
                activeHumans.clear();  
                drawableObjects.clear();
                backgroundObjects.clear();
                stars.clear();
                shootingStars.clear();
                exit(0);
                break;
        }
        glutPostRedisplay();
    }

    bool hasInit = false;

    void initScene() {

        if (hasInit) return;

        initVehicles(); 

        initHumans();

        initClouds();

        initStars();
        
        initBuildings();

        initStreetLamps();

        initRoadDecorators();

        initTrees();

        initGroundDecorator();  

        initAudio();

        hasInit = true;

        std::cout << "Fuad's scene initialized" << std::endl;

        std::cout << "--- Controls ---" << std::endl;
        std::cout << "P: Pause/Play" << std::endl;
        std::cout << "D: Toggle Debug Bounding Boxes" << std::endl;
        std::cout << "N: Toggle Day/Night" << std::endl;
        std::cout << "M: Toggle Background Traffic Sound" << std::endl;
        std::cout << "Left Click: Toggle human's crossing behavior" << std::endl;
        std::cout << "ESC: Exit" << std::endl;
    }

    void cleanupScene() {

        audioManager.cleanup();
                
        // Clear all objects
        vehicles.clear();
        activeHumans.clear();
        drawableObjects.clear();
        backgroundObjects.clear();
        stars.clear();
        shootingStars.clear();
        
        // Reset state
        hasInit = false;
        IS_PAUSED = false;
        DEBUG_ON = false;
        MUSIC_ON = true;
        showWarningMessage = false;
        warningMessageActive = false;
        lastCarSpawnTime = 0;
        lastHumanSpawnTime = 0;
        currentTimeOfDay = 0.3f;
        isNight = false;
        frameCount = 0;

        std::cout << "Fuad's scene cleaned up" << std::endl;
    }

    void mouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // Convert screen coordinates to world coordinates
            // Note: OpenGL uses bottom-left origin, but mouse coordinates are top-left
            float worldY = WINDOW_HEIGHT - y;
            
            // Check if click is inside any human's bounding box
            for (auto& human : activeHumans) {
                if (human->state != Human::HumanState::DESPAWNED) {
                    Rect bounds = human->getBounds();
                    // Adjust bounds to match the debug bounding box
                    Rect debugBounds = {bounds.x - 13, bounds.y - 5, 25, 45};
                    
                    if (x >= debugBounds.x && x <= debugBounds.x + debugBounds.w &&
                        worldY >= debugBounds.y && worldY <= debugBounds.y + debugBounds.h) {
                        // Toggle willCrossRoad
                        human->willCrossRoad = !human->willCrossRoad;
                        
                        // Update target based on new willCrossRoad state
                        if (human->willCrossRoad) {
                            // Should go to crossing
                            human->targetX = HUMAN_CROSSING_CENTER_X + (rand() % (int)(HUMAN_CROSSING_WIDTH / 2) - (int)(HUMAN_CROSSING_WIDTH / 4));
                        } else {
                            // Should go to edge of screen - use original spawn direction
                            human->targetX = human->cameFromLeft ? WINDOW_WIDTH + 50.0f : -50.0f;
                        }
                        
                        break; // Only toggle one human per click
                    }
                }
            }
        }
    }
}
#pragma once

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>

namespace Shoab {
    // Global variables
    bool isDay = true;
    bool isRaining = false;
    const int NUM_DROPS = 200;
    float dropsX[NUM_DROPS];
    float dropsY[NUM_DROPS];
    float cloud1X = 150, cloud2X = 400, cloud3X = 700;
    float nightCloud1X = 180, nightCloud2X = 450;
    float boatX = 0.0f;
    float truckX = -200.0f;
    float smokeY1 = 520.0f;
    float smokeY2 = 455.0f;
    float smokeOffset = 0.0f;

    // Draw filled circle
    void drawCircle(float cx, float cy, float r) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++) {
            float theta = 2.0f * 3.14159f * i / 100;
            glVertex2f(cx + r * cos(theta), cy + r * sin(theta));
        }
        glEnd();
    }

    // Initialize raindrop positions
    void initRain() {
        for (int i = 0; i < NUM_DROPS; i++) {
            dropsX[i] = rand() % 1000;
            dropsY[i] = rand() % 600;
        }
    }

    void drawSun(float x, float y, float radius) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        drawCircle(x, y, radius);
    }

    // Render rain
    void renderRain() {
        glColor3f(0.7f, 0.7f, 1.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < NUM_DROPS; i++) {
            glVertex2f(dropsX[i], dropsY[i]);
            glVertex2f(dropsX[i], dropsY[i] - 15);
        }
        glEnd();
    }

    // Update raindrops
    void updateRain() {
        for (int i = 0; i < NUM_DROPS; i++) {
            dropsY[i] -= 10;
            if (dropsY[i] < 0) {
                dropsY[i] = 600;
                dropsX[i] = rand() % 1000;
            }
        }
    }

    // Cloud drawing for day
    void drawCloud(float x, float y) {
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(x, y, 20);
        drawCircle(x + 20, y + 10, 25);
        drawCircle(x + 40, y, 20);
        drawCircle(x + 10, y - 10, 20);
    }

    // Cloud drawing for night
    void drawNightCloud(float x, float y) {
        glColor3f(0.4f, 0.4f, 0.5f);
        drawCircle(x, y, 20);
        drawCircle(x + 20, y + 10, 25);
        drawCircle(x + 40, y, 20);
        drawCircle(x + 10, y - 10, 20);
    }

    void river() {
        glColor3f(0.2f, 0.4f, 0.9f);
        glBegin(GL_POLYGON);
        glVertex2f(0,0);
        glVertex2f(1000,0);
        glVertex2f(1000,150);
        glVertex2f(0,150);
        glEnd();
    }

    void ground() {
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(0,150);
        glVertex2f(1000,150);
        glVertex2f(1000,225);
        glVertex2f(0,225);
        glEnd();

        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex2f(0,225);
        glVertex2f(1000,225);
        glVertex2f(1000,250);
        glVertex2f(0,250);
        glEnd();
    }

    void drawSmoke(float x, float y, float r) {
        glColor4f(0.4f, 0.4f, 0.4f, 0.5f); // Gray with transparency
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++) {
            float angle = 2 * 3.1416f * i / 100;
            glVertex2f(x + r * cos(angle), y + r * sin(angle));
        }
        glEnd();
    }

    void burner() {
        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Burner on main building
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(90, 467.5);
        glVertex2f(140, 467.5);
        glVertex2f(130, 520);
        glVertex2f(100, 520);
        glEnd();

        // Fume from main burner
        drawSmoke(115 + sin(smokeOffset) * 5, smokeY1, 12);

        // Burner on side building
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(330, 400);
        glVertex2f(370, 400);
        glVertex2f(360, 455);
        glVertex2f(340, 455);
        glEnd();

        // Fume from side burner
        drawSmoke(350 + cos(smokeOffset) * 4, smokeY2, 10);

        glDisable(GL_BLEND);
    }



    void factory1(){
        //main building
        glColor3f(0.6f, 0.6f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(50,250);
        glVertex2f(250,250);
        glVertex2f(250,375);
        glVertex2f(50,375);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(50,312.5);
        glVertex2f(250,312.5);
        glEnd();

        //roof
        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_POLYGON);
        glVertex2f(40,375);
        glVertex2f(260,375);
        glVertex2f(260,390);
        glVertex2f(40,390);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(50,250);
        glVertex2f(50,375);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(250,250);
        glVertex2f(250,375);
        glEnd();

        //windows for 1st flor
        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(70,275);
        glVertex2f(105,275);
        glVertex2f(105,300);
        glVertex2f(70,300);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(125,275);
        glVertex2f(160,275);
        glVertex2f(160,300);
        glVertex2f(125,300);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(180,275);
        glVertex2f(215,275);
        glVertex2f(215,300);
        glVertex2f(180,300);
        glEnd();

        //windows for 2nd floor
        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(70, 325);
        glVertex2f(105, 325);
        glVertex2f(105, 350);
        glVertex2f(70, 350);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(125, 325);
        glVertex2f(160, 325);
        glVertex2f(160, 350);
        glVertex2f(125, 350);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(180, 325);
        glVertex2f(215, 325);
        glVertex2f(215, 350);
        glVertex2f(180, 350);
        glEnd();

        //3rd flor
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(50,390);
        glVertex2f(175,390);
        glVertex2f(175,452.5);
        glVertex2f(50,452.5);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_LINES);
        glVertex2f(50,390);
        glVertex2f(50,452.5);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_LINES);
        glVertex2f(175,390);
        glVertex2f(175,452.5);
        glEnd();

        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_POLYGON);
        glVertex2f(40,452.5);
        glVertex2f(185,452.5);
        glVertex2f(185,467.5);
        glVertex2f(40,467.5);
        glEnd();

        //windows
        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(70,405);
        glVertex2f(105,405);
        glVertex2f(105,435);
        glVertex2f(70,435);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(125,405);
        glVertex2f(160,405);
        glVertex2f(160,435);
        glVertex2f(125,435);
        glEnd();

        //side building
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(250,250);
        glVertex2f(250,350);
        glVertex2f(400,350);
        glVertex2f(400,250);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(400,250);
        glVertex2f(400,350);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(250,350);
        glVertex2f(410,350);
        glVertex2f(410,355);
        glVertex2f(250,355);
        glEnd();

        glColor3f(0.6f, 0.6f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(325,355);
        glVertex2f(400,355);
        glVertex2f(400,400);
        glVertex2f(325,400);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(400,355);
        glVertex2f(400,400);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(325,355);
        glVertex2f(325,400);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(325,400);
        glVertex2f(400,400);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(330,250);
        glVertex2f(330,310);
        glVertex2f(370,310);
        glVertex2f(370,250);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(50,250);
        glVertex2f(400,250);
        glEnd();

        burner();
    }

    void factory2(){
        //main building
        glColor3f(0.6f, 0.6f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(50,250);
        glVertex2f(250,250);
        glVertex2f(250,375);
        glVertex2f(50,375);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(50,312.5);
        glVertex2f(250,312.5);
        glEnd();

        //roof
        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_POLYGON);
        glVertex2f(40,375);
        glVertex2f(260,375);
        glVertex2f(260,390);
        glVertex2f(40,390);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(50,250);
        glVertex2f(50,375);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(250,250);
        glVertex2f(250,375);
        glEnd();

        //windows for 1st flor
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(70,275);
        glVertex2f(105,275);
        glVertex2f(105,300);
        glVertex2f(70,300);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(125,275);
        glVertex2f(160,275);
        glVertex2f(160,300);
        glVertex2f(125,300);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(180,275);
        glVertex2f(215,275);
        glVertex2f(215,300);
        glVertex2f(180,300);
        glEnd();

        //windows for 2nd floor
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(70, 325);
        glVertex2f(105, 325);
        glVertex2f(105, 350);
        glVertex2f(70, 350);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(125, 325);
        glVertex2f(160, 325);
        glVertex2f(160, 350);
        glVertex2f(125, 350);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(180, 325);
        glVertex2f(215, 325);
        glVertex2f(215, 350);
        glVertex2f(180, 350);
        glEnd();

        //3rd flor
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(50,390);
        glVertex2f(175,390);
        glVertex2f(175,452.5);
        glVertex2f(50,452.5);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_LINES);
        glVertex2f(50,390);
        glVertex2f(50,452.5);
        glEnd();

        glColor3f(0.2f, 0.4f, 0.6f);
        glBegin(GL_LINES);
        glVertex2f(175,390);
        glVertex2f(175,452.5);
        glEnd();

        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_POLYGON);
        glVertex2f(40,452.5);
        glVertex2f(185,452.5);
        glVertex2f(185,467.5);
        glVertex2f(40,467.5);
        glEnd();

        //windows
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(70,405);
        glVertex2f(105,405);
        glVertex2f(105,435);
        glVertex2f(70,435);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(125,405);
        glVertex2f(160,405);
        glVertex2f(160,435);
        glVertex2f(125,435);
        glEnd();

        //side building
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(250,250);
        glVertex2f(250,350);
        glVertex2f(400,350);
        glVertex2f(400,250);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(400,250);
        glVertex2f(400,350);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(250,350);
        glVertex2f(410,350);
        glVertex2f(410,355);
        glVertex2f(250,355);
        glEnd();

        glColor3f(0.6f, 0.6f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(325,355);
        glVertex2f(400,355);
        glVertex2f(400,400);
        glVertex2f(325,400);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(400,355);
        glVertex2f(400,400);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(325,355);
        glVertex2f(325,400);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(325,400);
        glVertex2f(400,400);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(330,250);
        glVertex2f(330,310);
        glVertex2f(370,310);
        glVertex2f(370,250);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(50,250);
        glVertex2f(400,250);
        glEnd();

        burner();
    }


    void dBuilding1() {
        glColor3f(0.3f,0.4f,0.8f);
        glBegin(GL_POLYGON);
        glVertex2f(0,250);
        glVertex2f(50,250);
        glVertex2f(50,500);
        glVertex2f(0,500);
        glEnd();

        // Roof
        glColor3f(0.3f,0.4f,0.8f);
        glBegin(GL_POLYGON);
        glVertex2f(-1, 500);
        glVertex2f(55, 500);
        glVertex2f(55, 510);
        glVertex2f(-1, 510);
        glEnd();

        //border
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(1,250);
        glVertex2f(1,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(50,250);
        glVertex2f(50,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,500);
        glVertex2f(55,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,510);
        glVertex2f(55,510);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(55,510);
        glVertex2f(55,500);
        glEnd();


        //1st floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,300);
        glVertex2f(50,300);
        glEnd();

        //1st floor window
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 260);
        glVertex2f(32, 260);
        glVertex2f(32, 290);
        glVertex2f(18, 290);
        glEnd();

        //2nd floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,350);
        glVertex2f(50,350);
        glEnd();

        //2nd floor window
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 310);
        glVertex2f(32, 310);
        glVertex2f(32, 340);
        glVertex2f(18, 340);
        glEnd();

        //3rd floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,400);
        glVertex2f(50,400);
        glEnd();

        //3rd floor window
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 360);
        glVertex2f(32, 360);
        glVertex2f(32, 390);
        glVertex2f(18, 390);
        glEnd();

        //4th floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,450);
        glVertex2f(50,450);
        glEnd();

        //4th floor window
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 410);
        glVertex2f(32, 410);
        glVertex2f(32, 440);
        glVertex2f(18, 440);
        glEnd();

        //5th floor window

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 460);
        glVertex2f(32, 460);
        glVertex2f(32, 490);
        glVertex2f(18, 490);
        glEnd();
    }

    void nbuilding1(){
        glColor3f(0.1f,0.1f,0.4f);
        glBegin(GL_POLYGON);
        glVertex2f(0,250);
        glVertex2f(50,250);
        glVertex2f(50,500);
        glVertex2f(0,500);
        glEnd();

        // Roof
        glColor3f(0.1f,0.1f,0.4f);
        glBegin(GL_POLYGON);
        glVertex2f(-5, 500);
        glVertex2f(55, 500);
        glVertex2f(55, 510);
        glVertex2f(-5, 510);
        glEnd();

        //bordeer
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(1,250);
        glVertex2f(1,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(50,250);
        glVertex2f(50,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,500);
        glVertex2f(55,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,510);
        glVertex2f(55,510);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(55,510);
        glVertex2f(55,500);
        glEnd();

        //1st floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,300);
        glVertex2f(50,300);
        glEnd();

        //1st floor window
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 260);
        glVertex2f(32, 260);
        glVertex2f(32, 290);
        glVertex2f(18, 290);
        glEnd();

        //2nd floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,350);
        glVertex2f(50,350);
        glEnd();

        //2nd floor window
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 310);
        glVertex2f(32, 310);
        glVertex2f(32, 340);
        glVertex2f(18, 340);
        glEnd();

        //3rd floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,400);
        glVertex2f(50,400);
        glEnd();

        //3rd floor window
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 360);
        glVertex2f(32, 360);
        glVertex2f(32, 390);
        glVertex2f(18, 390);
        glEnd();

        //4th floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(0,450);
        glVertex2f(50,450);
        glEnd();

        //4th floor window
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 410);
        glVertex2f(32, 410);
        glVertex2f(32, 440);
        glVertex2f(18, 440);
        glEnd();

        //5th floor window

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(18, 460);
        glVertex2f(32, 460);
        glVertex2f(32, 490);
        glVertex2f(18, 490);
        glEnd();

    }

    void dbuilding2(){

        //building
        glColor3f(0.8f,0.7f,0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(390,250);
        glVertex2f(390,460);
        glVertex2f(560,460);
        glVertex2f(560,250);
        glEnd();

        //roof
        glColor3f(0.8f,0.7f,0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(380,460);
        glVertex2f(380,470);
        glVertex2f(570,470);
        glVertex2f(570,460);
        glEnd();

        //border
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(390,250);
        glVertex2f(390,460);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,250);
        glVertex2f(560,460);
        glEnd();


        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(380,460);
        glVertex2f(570,460);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(380,470);
        glVertex2f(570,470);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(380,460);
        glVertex2f(380,470);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(570,460);
        glVertex2f(570,470);
        glEnd();

        //1st floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(390,320);
        glVertex2f(560,320);
        glEnd();

        //1st floor door and right window
        glColor3f(0.0f, 0.0f, 0.0f); // Black door
        glBegin(GL_POLYGON);
        glVertex2f(410,250);   // Bottom-left
        glVertex2f(440,250);   // Bottom-right
        glVertex2f(440,300);   // Top-right
        glVertex2f(410,300);   // Top-left
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f); // Black window
        glBegin(GL_POLYGON);
        glVertex2f(490,270);   // Bottom-left
        glVertex2f(520,270);   // Bottom-right
        glVertex2f(520,300);   // Top-right
        glVertex2f(490,300);   // Top-left
        glEnd();

        //2nd floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(390,390);
        glVertex2f(560,390);
        glEnd();

        //2nd floors window

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(420,330);
        glVertex2f(450,330);
        glVertex2f(450,360);
        glVertex2f(420,360);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(490,330);
        glVertex2f(520,330);
        glVertex2f(520,360);
        glVertex2f(490,360);
        glEnd();


        //3rd floors windows

        glColor3f(0.0f,0.0f,0.f);
        glBegin(GL_POLYGON);
        glVertex2f(420,400);
        glVertex2f(450,400);
        glVertex2f(450,430);
        glVertex2f(420,430);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(490,400);
        glVertex2f(520,400);
        glVertex2f(520,430);
        glVertex2f(490,430);
        glEnd();


    }

    void nbuilding2(){
        glColor3f(0.4f,0.3f,0.2f);
        glBegin(GL_POLYGON);
        glVertex2f(390,250);
        glVertex2f(390,460);
        glVertex2f(560,460);
        glVertex2f(560,250);
        glEnd();

        //roof
        glColor3f(0.4f,0.3f,0.2f);
        glBegin(GL_POLYGON);
        glVertex2f(380,460);
        glVertex2f(380,470);
        glVertex2f(570,470);
        glVertex2f(570,460);
        glEnd();

        //border
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(390,250);
        glVertex2f(390,460);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,250);
        glVertex2f(560,460);
        glEnd();


        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(380,460);
        glVertex2f(570,460);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(380,470);
        glVertex2f(570,470);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(380,460);
        glVertex2f(380,470);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(570,460);
        glVertex2f(570,470);
        glEnd();

        //1st floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(390,320);
        glVertex2f(560,320);
        glEnd();

        //1st floor door and right window
        glColor3f(1.0f, 1.0f, 0.0f); // Black door
        glBegin(GL_POLYGON);
        glVertex2f(410,250);   // Bottom-left
        glVertex2f(440,250);   // Bottom-right
        glVertex2f(440,300);   // Top-right
        glVertex2f(410,300);   // Top-left
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f); // Black window
        glBegin(GL_POLYGON);
        glVertex2f(490,270);   // Bottom-left
        glVertex2f(520,270);   // Bottom-right
        glVertex2f(520,300);   // Top-right
        glVertex2f(490,300);   // Top-left
        glEnd();

        //2nd floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(390,390);
        glVertex2f(560,390);
        glEnd();

        //2nd floors window

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(420,330);
        glVertex2f(450,330);
        glVertex2f(450,360);
        glVertex2f(420,360);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(490,330);
        glVertex2f(520,330);
        glVertex2f(520,360);
        glVertex2f(490,360);
        glEnd();


        //3rd floors windows

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(420,400);
        glVertex2f(450,400);
        glVertex2f(450,430);
        glVertex2f(420,430);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(490,400);
        glVertex2f(520,400);
        glVertex2f(520,430);
        glVertex2f(490,430);
        glEnd();

    }

    void dbuilding3(){

        //building
        glColor3f(0.5f,0.6f,0.4f);
        glBegin(GL_POLYGON);
        glVertex2f(560,250);
        glVertex2f(560,500);
        glVertex2f(710,500);
        glVertex2f(710,250);
        glEnd();

        //roof
        glColor3f(0.5f,0.6f,0.4f);
        glBegin(GL_POLYGON);
        glVertex2f(550,500);
        glVertex2f(550,510);
        glVertex2f(720,510);
        glVertex2f(720,500);
        glEnd();

        //border
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,250);
        glVertex2f(560,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,250);
        glVertex2f(710,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(550,500);
        glVertex2f(550,510);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(720,500);
        glVertex2f(720,510);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(550,500);
        glVertex2f(720,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(550,510);
        glVertex2f(720,510);
        glEnd();

        //1st floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,312.5);
        glVertex2f(710,312.5);
        glEnd();

        //1st floor door in middle
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(620, 250);    // Bottom-left
        glVertex2f(650, 250);    // Bottom-right
        glVertex2f(650, 300);    // Top-right
        glVertex2f(620, 300);    // Top-left
        glEnd();

        //2ns floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,375);
        glVertex2f(710,375);
        glEnd();

        //2nd floor windows
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(580, 325);    // Left window
        glVertex2f(610, 325);
        glVertex2f(610, 355);
        glVertex2f(580, 355);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(660, 325);    // Right window
        glVertex2f(690, 325);
        glVertex2f(690, 355);
        glVertex2f(660, 355);
        glEnd();

        //3rd floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,437.5);
        glVertex2f(710,437.5);
        glEnd();

        //3rd floor windows

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(580, 387.5);    // Left window
        glVertex2f(610, 387.5);
        glVertex2f(610, 417.5);
        glVertex2f(580, 417.5);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(660, 387.5);    // Right window
        glVertex2f(690, 387.5);
        glVertex2f(690, 417.5);
        glVertex2f(660, 417.5);
        glEnd();

        //4th floor windows

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(580, 450);    // Left window
        glVertex2f(610, 450);
        glVertex2f(610, 480);
        glVertex2f(580, 480);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(660, 450);    // Right window
        glVertex2f(690, 450);
        glVertex2f(690, 480);
        glVertex2f(660, 480);
        glEnd();
    }

    void nbuilding3(){
        glColor3f(0.3f,0.4f,0.2f);
        glBegin(GL_POLYGON);
        glVertex2f(560,250);
        glVertex2f(560,500);
        glVertex2f(710,500);
        glVertex2f(710,250);
        glEnd();

        //roof
        glColor3f(0.3f,0.4f,0.2f);
        glBegin(GL_POLYGON);
        glVertex2f(550,500);
        glVertex2f(550,510);
        glVertex2f(720,510);
        glVertex2f(720,500);
        glEnd();

        //border

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,250);
        glVertex2f(560,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,250);
        glVertex2f(710,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(550,500);
        glVertex2f(550,510);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(720,500);
        glVertex2f(720,510);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(550,500);
        glVertex2f(720,500);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(550,510);
        glVertex2f(720,510);
        glEnd();

        //1st floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,312.5);
        glVertex2f(710,312.5);
        glEnd();

        //1st floor door in middle
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(620, 250);    // Bottom-left
        glVertex2f(650, 250);    // Bottom-right
        glVertex2f(650, 300);    // Top-right
        glVertex2f(620, 300);    // Top-left
        glEnd();

        //2ns floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,375);
        glVertex2f(710,375);
        glEnd();

        //2nd floor windows
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(580, 325);    // Left window
        glVertex2f(610, 325);
        glVertex2f(610, 355);
        glVertex2f(580, 355);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(660, 325);    // Right window
        glVertex2f(690, 325);
        glVertex2f(690, 355);
        glVertex2f(660, 355);
        glEnd();

        //3rd floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(560,437.5);
        glVertex2f(710,437.5);
        glEnd();

        //3rd floor windows

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(580, 387.5);    // Left window
        glVertex2f(610, 387.5);
        glVertex2f(610, 417.5);
        glVertex2f(580, 417.5);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(660, 387.5);    // Right window
        glVertex2f(690, 387.5);
        glVertex2f(690, 417.5);
        glVertex2f(660, 417.5);
        glEnd();

        //4th floor windows

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(580, 450);    // Left window
        glVertex2f(610, 450);
        glVertex2f(610, 480);
        glVertex2f(580, 480);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(660, 450);    // Right window
        glVertex2f(690, 450);
        glVertex2f(690, 480);
        glVertex2f(660, 480);
        glEnd();
    }

    void dbuilding4(){

        //building
        glColor3f(0.2f,0.5f,0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(710,250);
        glVertex2f(710,450);
        glVertex2f(880,450);
        glVertex2f(880,250);
        glEnd();

        //roof
        glColor3f(0.2f,0.5f,0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(700,450);
        glVertex2f(890,450);
        glVertex2f(890,460);
        glVertex2f(700,460);
        glEnd();

        //border
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,250);
        glVertex2f(710,450);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(880,250);
        glVertex2f(880,450);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(890,450);
        glVertex2f(890,460);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(700,450);
        glVertex2f(700,460);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(700,450);
        glVertex2f(890,450);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(700,460);
        glVertex2f(890,460);
        glEnd();

        //1st floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,300);
        glVertex2f(880,300);
        glEnd();

        //1st floor door

        glColor3f(0.0f, .0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(775,250);   // bottom-left
        glVertex2f(805,250);   // bottom-right
        glVertex2f(805,290);   // top-right
        glVertex2f(775,290);   // top-left
        glEnd();

        //2nd floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,350);
        glVertex2f(880,350);
        glEnd();

        //2nd floor window
        glColor3f(0.0f, .0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(730,310);    // Left window
        glVertex2f(760,310);
        glVertex2f(760,340);
        glVertex2f(730,340);
        glEnd();

        glColor3f(0.0f, .0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(830,310);    // Right window
        glVertex2f(860,310);
        glVertex2f(860,340);
        glVertex2f(830,340);
        glEnd();

        //3rd floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,400);
        glVertex2f(880,400);
        glEnd();

        //3rd floor windows
        glColor3f(0.0f, .0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(730,360);    // Left window
        glVertex2f(760,360);
        glVertex2f(760,390);
        glVertex2f(730,390);
        glEnd();

        glColor3f(0.0f, .0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(830,360);    // Right window
        glVertex2f(860,360);
        glVertex2f(860,390);
        glVertex2f(830,390);
        glEnd();

        //4th floor windows

        glColor3f(0.0f, .0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(730,410);    // Left window
        glVertex2f(760,410);
        glVertex2f(760,440);
        glVertex2f(730,440);
        glEnd();

        glColor3f(0.0f, .0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(830,410);    // Right window
        glVertex2f(860,410);
        glVertex2f(860,440);
        glVertex2f(830,440);
        glEnd();
    }

    void nbuilding4(){
        glColor3f(0.1f,0.2f,0.4f);
        glBegin(GL_POLYGON);
        glVertex2f(710,250);
        glVertex2f(710,450);
        glVertex2f(880,450);
        glVertex2f(880,250);
        glEnd();

        //roof
        glColor3f(0.1f,0.2f,0.4f);
        glBegin(GL_POLYGON);
        glVertex2f(700,450);
        glVertex2f(890,450);
        glVertex2f(890,460);
        glVertex2f(700,460);
        glEnd();

        //border
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,250);
        glVertex2f(710,450);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(880,250);
        glVertex2f(880,450);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(890,450);
        glVertex2f(890,460);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(700,450);
        glVertex2f(700,460);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(700,450);
        glVertex2f(890,450);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(700,460);
        glVertex2f(890,460);
        glEnd();

        //1st floor
        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,300);
        glVertex2f(880,300);
        glEnd();

        //1st floor door

        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(775,250);   // bottom-left
        glVertex2f(805,250);   // bottom-right
        glVertex2f(805,290);   // top-right
        glVertex2f(775,290);   // top-left
        glEnd();

        //2nd floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,350);
        glVertex2f(880,350);
        glEnd();

        //2nd floor window
        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(730,310);    // Left window
        glVertex2f(760,310);
        glVertex2f(760,340);
        glVertex2f(730,340);
        glEnd();

        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(830,310);    // Right window
        glVertex2f(860,310);
        glVertex2f(860,340);
        glVertex2f(830,340);
        glEnd();

        //3rd floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(710,400);
        glVertex2f(880,400);
        glEnd();

        //3rd floor windows
        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(730,360);    // Left window
        glVertex2f(760,360);
        glVertex2f(760,390);
        glVertex2f(730,390);
        glEnd();

        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(830,360);    // Right window
        glVertex2f(860,360);
        glVertex2f(860,390);
        glVertex2f(830,390);
        glEnd();

        //4th floor windows

        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(730,410);    // Left window
        glVertex2f(760,410);
        glVertex2f(760,440);
        glVertex2f(730,440);
        glEnd();

        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(830,410);    // Right window
        glVertex2f(860,410);
        glVertex2f(860,440);
        glVertex2f(830,440);
        glEnd();
    }

    void dbuilding5(){
        //building
        glColor3f(0.4f,0.9f,0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(880,250);
        glVertex2f(880,375);
        glVertex2f(1000,375);
        glVertex2f(1000,250);
        glEnd();

        //roof
        glColor3f(0.4f,0.9f,0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(880,375);
        glVertex2f(1000,375);
        glVertex2f(1000,385);
        glVertex2f(880,385);
        glEnd();

        //border

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(880,375);
        glVertex2f(1000,375);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(880,385);
        glVertex2f(1000,385);
        glEnd();

        //1st floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(880,312.5);
        glVertex2f(1000,312.5);
        glEnd();

        //1st floor door
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(930, 250);   // Bottom-left
        glVertex2f(960, 250);   // Bottom-right
        glVertex2f(960, 300);   // Top-right
        glVertex2f(930, 300);   // Top-left
        glEnd();

        //2nd floor windows
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(900, 320);   // Bottom-left
        glVertex2f(925, 320);   // Bottom-right
        glVertex2f(925, 350);   // Top-right
        glVertex2f(900, 350);   // Top-left
        glEnd();

        // Right window
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(955, 320);   // Bottom-left
        glVertex2f(980, 320);   // Bottom-right
        glVertex2f(980, 350);   // Top-right
        glVertex2f(955, 350);   // Top-left
        glEnd();
    }

    void nbuilding5(){

        glColor3f(0.1f,0.5f,0.3f);
        glBegin(GL_POLYGON);
        glVertex2f(880,250);
        glVertex2f(880,375);
        glVertex2f(1000,375);
        glVertex2f(1000,250);
        glEnd();

        glColor3f(0.1f,0.5f,0.3f);
        glBegin(GL_POLYGON);
        glVertex2f(880,375);
        glVertex2f(1000,375);
        glVertex2f(1000,385);
        glVertex2f(880,385);
        glEnd();

        //border

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(880,375);
        glVertex2f(1000,375);
        glEnd();

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(880,385);
        glVertex2f(1000,385);
        glEnd();

        //1st floor

        glColor3f(0.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f(880,312.5);
        glVertex2f(1000,312.5);
        glEnd();

        //1st floor door
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(930, 250);   // Bottom-left
        glVertex2f(960, 250);   // Bottom-right
        glVertex2f(960, 300);   // Top-right
        glVertex2f(930, 300);   // Top-left
        glEnd();

        //2nd floor windows
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(900, 320);   // Bottom-left
        glVertex2f(925, 320);   // Bottom-right
        glVertex2f(925, 350);   // Top-right
        glVertex2f(900, 350);   // Top-left
        glEnd();

        // Right window
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(955, 320);   // Bottom-left
        glVertex2f(980, 320);   // Bottom-right
        glVertex2f(980, 350);   // Top-right
        glVertex2f(955, 350);   // Top-left
        glEnd();
    }

    void drain() {
        // Large circular drain
        glColor3f(0.2f, 0.2f, 0.2f); // Dark gray drain
        drawCircle(420, 180, 25); // Centered around y=180, large size radius=25

        // Inner circle to simulate depth
        glColor3f(0.1f, 0.1f, 0.1f); // Darker inner hole
        drawCircle(420, 180, 18);
    }

    void truck() {
        glColor3f(0.6f, 0.1f, 0.1f); // Dark red cargo
        glBegin(GL_POLYGON);
        glVertex2f(420 + truckX, 250);
        glVertex2f(560 + truckX, 250);
        glVertex2f(560 + truckX, 310);
        glVertex2f(420 + truckX, 310);
        glEnd();

        // Cabin base
        glColor3f(0.4f, 0.4f, 0.4f); // Gray cabin
        glBegin(GL_POLYGON);
        glVertex2f(560 + truckX, 250);
        glVertex2f(630 + truckX, 250);
        glVertex2f(630 + truckX, 325);
        glVertex2f(560 + truckX, 325);
        glEnd();

        // Cabin roof
        glColor3f(0.2f, 0.2f, 0.2f); // Darker roof
        glBegin(GL_POLYGON);
        glVertex2f(560 + truckX, 325);
        glVertex2f(630 + truckX, 325);
        glVertex2f(620 + truckX, 335);
        glVertex2f(550 + truckX, 335);
        glEnd();

        // Window
        glColor3f(0.7f, 0.9f, 1.0f); // Light blue
        glBegin(GL_POLYGON);
        glVertex2f(575 + truckX, 285);
        glVertex2f(615 + truckX, 285);
        glVertex2f(615 + truckX, 320);
        glVertex2f(575 + truckX, 320);
        glEnd();

        // Wheels
        glColor3f(0.0f, 0.0f, 0.0f); // Outer wheels
        drawCircle(440 + truckX, 240, 15); // Rear wheel 1
        drawCircle(520 + truckX, 240, 15); // Rear wheel 2
        drawCircle(570 + truckX, 240, 15); // Front wheel 1
        drawCircle(615 + truckX, 240, 15); // Front wheel 2

        // Wheel centers
        glColor3f(0.3f, 0.3f, 0.3f); // Inner hub
        drawCircle(440 + truckX, 240, 6);
        drawCircle(520 + truckX, 240, 6);
        drawCircle(570 + truckX, 240, 6);
        drawCircle(615 + truckX, 240, 6);
    }

    void boat() {
        float offset = boatX;
        // Boat hull (lowered)
        glColor3f(0.4f, 0.2f, 0.0f);  // Dark wood color
        glBegin(GL_POLYGON);
        glVertex2f(650 + offset, 135);
        glVertex2f(875 + offset, 135);
        glVertex2f(840 + offset, 100);
        glVertex2f(685 + offset, 100);
        glEnd();

        // Boat body (lowered)
        glColor3f(0.9f, 0.9f, 0.9f);  // Light gray upper part
        glBegin(GL_POLYGON);
        glVertex2f(700 + offset, 135);
        glVertex2f(825 + offset, 135);
        glVertex2f(805 + offset, 170);
        glVertex2f(720 + offset, 170);
        glEnd();

        // Boat roof (lowered)
        glColor3f(0.3f, 0.3f, 0.3f);  // Dark gray
        glBegin(GL_POLYGON);
        glVertex2f(735 + offset, 170);
        glVertex2f(790 + offset, 170);
        glVertex2f(780 + offset, 190);
        glVertex2f(745 + offset, 190);
        glEnd();

        // Flag pole (lowered)
        glColor3f(0.1f, 0.1f, 0.1f); // Black pole
        glBegin(GL_LINES);
        glVertex2f(762 + offset, 190);
        glVertex2f(762 + offset, 220);
        glEnd();

        // Flag (lowered)
        glColor3f(1.0f, 0.0f, 0.0f); // Red flag
        glBegin(GL_TRIANGLES);
        glVertex2f(762 + offset, 220);
        glVertex2f(782 + offset, 210);
        glVertex2f(762 + offset, 200);
        glEnd();
    }
    void moon(float x, float y, float radius) {
        glColor3f(1.0f, 1.0f, 0.9f); // Pale yellowish white
        drawCircle(x, y, radius);
    }


    void stars() {
        glColor3f(1.0f, 1.0f, 1.0f); // White color for stars
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        glVertex2f(50, 550);
        glVertex2f(100, 580);
        glVertex2f(150, 540);
        glVertex2f(220, 570);
        glVertex2f(300, 590);
        glVertex2f(400, 560);
        glVertex2f(500, 580);
        glVertex2f(600, 550);
        glVertex2f(700, 570);
        glVertex2f(800, 590);
        glVertex2f(900, 560);
        glVertex2f(950, 580);
        glVertex2f(980, 540);
        glEnd();
    }

    void garbage() {
        // Floating plastic bottle 1
        glColor3f(0.2f, 0.8f, 0.2f);  // Green bottle
        glBegin(GL_POLYGON);
        glVertex2f(120, 50);
        glVertex2f(140, 45);
        glVertex2f(145, 80);
        glVertex2f(125, 85);
        glEnd();

        // Bottle neck
        glColor3f(0.7f, 0.9f, 0.7f);
        glBegin(GL_POLYGON);
        glVertex2f(130, 85);
        glVertex2f(140, 80);
        glVertex2f(138, 95);
        glVertex2f(132, 95);
        glEnd();

        // Broken wooden crate
        glColor3f(0.6f, 0.4f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex2f(220, 40);
        glVertex2f(270, 45);
        glVertex2f(265, 75);
        glVertex2f(215, 70);
        glEnd();

        // Crate hole
        glColor3f(0.3f, 0.2f, 0.1f);
        glBegin(GL_POLYGON);
        glVertex2f(235, 55);
        glVertex2f(255, 58);
        glVertex2f(253, 68);
        glVertex2f(233, 65);
        glEnd();

        // Soda can
        glColor3f(0.9f, 0.1f, 0.1f);  // Red can
        glBegin(GL_POLYGON);
        glVertex2f(350, 55);
        glVertex2f(380, 50);
        glVertex2f(385, 75);
        glVertex2f(355, 80);
        glEnd();

        // Can top
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_POLYGON);
        glVertex2f(360, 80);
        glVertex2f(380, 75);
        glVertex2f(378, 85);
        glVertex2f(362, 87);
        glEnd();

        // Plastic bag
        glColor4f(0.9f, 0.9f, 0.9f, 0.6f);
        glBegin(GL_POLYGON);
        glVertex2f(450, 60);
        glVertex2f(490, 55);
        glVertex2f(500, 70);
        glVertex2f(495, 85);
        glVertex2f(460, 90);
        glVertex2f(445, 75);
        glEnd();

        // Banana peel
        glColor3f(0.9f, 0.9f, 0.3f);
        glBegin(GL_POLYGON);
        glVertex2f(550, 50);
        glVertex2f(590, 55);
        glVertex2f(580, 60);
        glVertex2f(570, 65);
        glVertex2f(560, 62);
        glVertex2f(555, 55);
        glEnd();

        // Banana details
        glColor3f(0.7f, 0.7f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex2f(565, 55);
        glVertex2f(570, 58);
        glVertex2f(568, 60);
        glVertex2f(563, 57);
        glEnd();

        // Tire
        glColor3f(0.1f, 0.1f, 0.1f);
        glBegin(GL_POLYGON);
        glVertex2f(650, 45);
        glVertex2f(700, 50);
        glVertex2f(695, 85);
        glVertex2f(645, 80);
        glEnd();

        // Tire inner
        glColor3f(0.4f, 0.4f, 0.4f);
        glBegin(GL_POLYGON);
        glVertex2f(665, 60);
        glVertex2f(685, 62);
        glVertex2f(683, 72);
        glVertex2f(663, 70);
        glEnd();

        // Milk carton
        glColor3f(0.95f, 0.95f, 0.95f);
        glBegin(GL_POLYGON);
        glVertex2f(750, 50);
        glVertex2f(780, 48);
        glVertex2f(785, 80);
        glVertex2f(755, 82);
        glEnd();

        // Carton top
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_POLYGON);
        glVertex2f(755, 82);
        glVertex2f(785, 80);
        glVertex2f(782, 90);
        glVertex2f(758, 92);
        glEnd();
    }
    void updateScene() {
        if (isRaining) {
            updateRain();
        }

        // Move clouds
        cloud1X += 0.2f;
        cloud2X += 0.2f;
        cloud3X += 0.2f;
        nightCloud1X += 0.2f;
        nightCloud2X += 0.2f;

        if (cloud1X > 1100) cloud1X = -100;
        if (cloud2X > 1100) cloud2X = -100;
        if (cloud3X > 1100) cloud3X = -100;
        if (nightCloud1X > 1100) nightCloud1X = -100;
        if (nightCloud2X > 1100) nightCloud2X = -100;

        // Synchronized movement of boat and truck
        static float scenePosition = 0.0f;
        scenePosition += 0.5f;  // Master speed control

        // Boat moves right to left (opposite direction)
        boatX = -scenePosition;

        // Truck moves left to right
        truckX = scenePosition * 1.6f;  // 1.6x faster than boat

        smokeY1 += 0.5f;
        smokeY2 += 0.5f;
        smokeOffset += 0.2f;

        if (smokeY1 > 600) smokeY1 = 520;
        if (smokeY2 > 600) smokeY2 = 455;

        // Reset when both go off-screen
        if (scenePosition > 1000) {
            scenePosition = -200.0f;
            boatX = -scenePosition;
            truckX = scenePosition * 1.6f;
        }

        glutPostRedisplay();
    }


    // Day Scene
    void renderDayScene() {
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Clouds
        drawCloud(cloud1X, 520);
        drawCloud(cloud2X, 560);
        drawCloud(cloud3X, 530);


        drawSun(900, 520, 40);
        river();
        ground();
        dBuilding1();
        dbuilding4();
        dbuilding3();
        dbuilding2();
        dbuilding5();
        factory1();
        drain();
        truck();
        boat();
        garbage();

        if (isRaining) {
            renderRain();
        }

        glFlush();
    }

    // Night Scene
    void renderNightScene() {
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Night Clouds
        drawNightCloud(nightCloud1X, 520);
        drawNightCloud(nightCloud2X, 550);

        river();
        ground();
        nbuilding1();
        nbuilding4();
        nbuilding3();
        nbuilding2();
        nbuilding5();
        factory2();
        drain();
        truck();
        boat();
        moon(900, 520, 40);
        stars();
        garbage();

        if (isRaining) {
            renderRain();
        }

        glFlush();
    }

    // Display function
    void display() {
        if (isDay) {
            renderDayScene();
        } else {
            renderNightScene();
        }
    }

    // Keyboard input
    void keyboard(unsigned char key, int x, int y) {
        if (key == 'd' || key == 'D') isDay = true;
        else if (key == 'n' || key == 'N') isDay = false;
        else if (key == 'r' || key == 'R') isRaining = !isRaining;
        glutPostRedisplay();
    }

    // Initialization
    void initScene() {

        initRain();

        std::cout << "Shoabs's scene initialized." << std::endl;

        std::cout <<  "--- Controls ---" << std::endl;
        std::cout << "For night press : n" << std::endl;
        std::cout << "For day press   : d" << std::endl;
        std::cout << "For rain press  : r" << std::endl;
        std::cout << std::endl;
        std::cout << "To close the animation press : Esc button" << std::endl;
    }

    void cleanupScene() {

        std::cout << "Shoab's scene cleaned up." << std::endl;
    }
}

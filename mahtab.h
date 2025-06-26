#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>

namespace Mahtab {
    // Global variables
    bool day = true;
    bool moveSunDown = false;
    bool moveSunUp = false;
    bool show_vehicle = true;
    bool isRaining = false;
    bool isFlooded = false;
    float rainY[100];  // Y positions for rain drops
    float rainX[100];  // X positions for rain drops
    float floodLevel = 0.0f;  // For flood animation

    float cloudX = 0.0f;
    float sunY = 550.0f;
    float sunSpeed = 1.0f;
    float carX = 1000.0f;  // Starting position for car1
    float carX2 = -800.0f;  // Starting position for car2
    float busX = -80.0f;  // For bus1
    float busX2 = 0.0f;  // For bus2
    float planeX = 0.0f;
    float position = 0.0f;
    float speed = 1.7f;
    float carSpeed = 2.0f;  // Reduced speed for smoother movement
    float busSpeed = 2.5f;  // Reduced speed for smoother movement
    float waterFlow = 0.0f;  // For water animation
    float boatX = -200.0f;  // Starting position for boat
    float boatSpeed = 1.5f; // Boat speed, adjustable by mouse click

    // Function declarations
    void init();
    void drawCircle(int x, int y, int radius);
    void day_sky();
    void night_sky();
    void sun();
    void tree();
    void house_day();
    void house_night();
    void car1();
    void car2();
    void bus1();
    void bus2();
    void boat();
    void road();
    void greenPart_bush();
    void display();
    void keyboard(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);

    void drawCircle(int x, int y, int radius)
    {
        float theta;
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++)
        {
            theta = i * 3.142 / 180;
            glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
        }
        glEnd();
    }

    void road()
    {
        glBegin(GL_QUADS);
        glColor3f(0.392f, 0.392f, 0.392f);  //Dark Grey Road Middle Part of road
        glVertex2i(0, 127);
        glVertex2i(1000, 127);
        glVertex2i(1000, 347);
        glVertex2i(0, 347);
        glEnd();
        glBegin(GL_QUADS);
        glColor3f(0.890f, 0.870f, 0.870f); //Grey Border
        glVertex2i(0, 140);
        glVertex2i(1000, 140);
        glVertex2i(1000, 130);
        glVertex2i(0, 130);
        glEnd();
        glBegin(GL_QUADS);
        glColor3f(0.698f, 0.745f, 0.709f); //Grey Border
        glVertex2i(0, 340);
        glVertex2i(1000, 340);
        glVertex2i(1000, 343);
        glVertex2i(0, 343);
        glEnd();
        glBegin(GL_QUADS);
        glColor3f(0.698f, 0.745f, 0.709f); //ash road divider
        glVertex2i(100, 240);
        glVertex2i(300, 240);
        glVertex2i(300, 245);
        glVertex2i(100, 245);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2i(400, 240);
        glVertex2i(600, 240);
        glVertex2i(600, 245);
        glVertex2i(400, 245);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2i(700, 240);
        glVertex2i(900, 240);
        glVertex2i(900, 245);
        glVertex2i(700, 245);
        glEnd();
        glColor3f(0.392f, 0.392f, 0.392f);  //road from the house
        glBegin(GL_QUADS);
        glVertex2f(195, 400);
        glVertex2f(235, 400);
        glVertex2i(200, 347);
        glVertex2i(160, 347);
        glEnd();
    }
    void day_sky()
    {
        glBegin(GL_QUADS);
        glColor3f(0.784f, 0.96f, 0.96f);  //blue
        glVertex2i(0,600);
        glVertex2i(1000, 600);
        glVertex2i(1000, 300);
        glVertex2i(0, 300);
        glEnd();
    }
    void night_sky()
    {
        glBegin(GL_QUADS);
        glColor3f(isRaining ? 0.02f : 0.05f, isRaining ? 0.02f : 0.05f, isRaining ? 0.15f : 0.2f);
        glVertex2i(0, 600);
        glVertex2i(1000, 600);
        glVertex2i(1000, 300);
        glVertex2i(0, 300);
        glEnd();

        if (!isRaining) {
            const int num_stars = 16;
            int star_positions[num_stars][2] = {
                {50, 550}, {150, 500}, {200, 560}, {350, 520},
                {300, 590}, {420, 550}, {450, 480}, {500, 560},
                {600, 500}, {650, 570}, {700, 500}, {750, 470},
                {800, 550}, {850, 530}, {900, 560}, {950, 480}
            };

            glPointSize(3.0f);
            glBegin(GL_POINTS);
            for (int i = 0; i < num_stars; ++i) {
                float brightness = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.5f));
                glColor3f(brightness, brightness, brightness);
                glVertex2i(star_positions[i][0], star_positions[i][1]);
            }
            glEnd();
        }
    }
    void sun()
    {
        glColor3f(1.0f, 1.0f, 0.0f);  // yellow
        drawCircle(750, sunY, 30);
    }
    void lamp_daylight()
    {
        // Deep blue-tinted light for daytime
        glColor3f(0.4f, 0.6f, 1.0f);

        // Left lamp light cone
        glBegin(GL_TRIANGLES);
        glColor4f(0.4f, 0.6f, 1.0f, 0.8f);  // Deep blue at top
        glVertex2i(540, 420);  // Moved left from 590
        glColor4f(0.2f, 0.4f, 0.9f, 0.4f);  // Darker blue at bottom
        glVertex2i(510, 347);  // Moved left from 560
        glVertex2i(570, 347);  // Moved left from 620
        glEnd();

        // Right lamp light cone
        glBegin(GL_TRIANGLES);
        glColor4f(0.4f, 0.6f, 1.0f, 0.8f);  // Deep blue at top
        glVertex2i(890, 420);
        glColor4f(0.2f, 0.4f, 0.9f, 0.4f);  // Darker blue at bottom
        glVertex2i(860, 347);
        glVertex2i(920, 347);
        glEnd();

        // Bright center of the lamps - intense blue-white
        glColor3f(0.6f, 0.8f, 1.0f);
        drawCircle(540, 440, 8);  // Moved left from 590
        drawCircle(890, 440, 8);
    }
    void lamp_nightlight()
    {
        // Deep magenta light for night
        glColor3f(0.8f, 0.0f, 0.8f);

        // Left lamp light cone with gradient
        glBegin(GL_TRIANGLES);
        glColor3f(0.8f, 0.0f, 0.8f);  // Deep magenta at top
        glVertex2i(540, 420);  // Moved left from 590
        glColor3f(0.5f, 0.0f, 0.5f);  // Dark purple at bottom
        glVertex2i(510, 347);  // Moved left from 560
        glVertex2i(570, 347);  // Moved left from 620
        glEnd();

        // Right lamp light cone with gradient
        glBegin(GL_TRIANGLES);
        glColor3f(0.8f, 0.0f, 0.8f);  // Deep magenta at top
        glVertex2i(890, 420);
        glColor3f(0.5f, 0.0f, 0.5f);  // Dark purple at bottom
        glVertex2i(860, 347);
        glVertex2i(920, 347);
        glEnd();

        // Inner bright center of the lamps - intense magenta
        glColor3f(1.0f, 0.2f, 1.0f);
        drawCircle(540, 440, 6);  // Moved left from 590
        drawCircle(890, 440, 6);

        // Middle layer glow
        glColor3f(0.9f, 0.1f, 0.9f);
        drawCircle(540, 440, 9);  // Moved left from 590
        drawCircle(890, 440, 9);

        // Outer glow for the lamp centers - deep magenta
        glColor3f(0.8f, 0.0f, 0.8f);
        drawCircle(540, 440, 12);  // Moved left from 590
        drawCircle(890, 440, 12);
    }
    void lamp_stand()
    {
        glColor3f(0.65f, 0.65f, 0.65f);  // Original grey color
        glBegin(GL_QUADS);//1 - Left lamp
        glVertex2i(535, 347);  // Moved left from 585
        glVertex2i(545, 347);  // Moved left from 595
        glVertex2i(545, 420);  // Moved left from 595
        glVertex2i(535, 420);  // Moved left from 585
        glEnd();

        glBegin(GL_QUADS);//1 - Left lamp head
        glVertex2i(530, 440);  // Moved left from 580
        glVertex2i(550, 440);  // Moved left from 600
        glVertex2i(550, 420);  // Moved left from 600
        glVertex2i(530, 420);  // Moved left from 580
        glEnd();

        glBegin(GL_QUADS);//2 - Right lamp
        glVertex2i(885, 347);
        glVertex2i(895, 347);
        glVertex2i(895, 420);
        glVertex2i(885, 420);
        glEnd();

        glBegin(GL_QUADS);//2 - Right lamp head
        glVertex2i(880, 440);
        glVertex2i(900, 440);
        glVertex2i(900, 420);
        glVertex2i(880, 420);
        glEnd();
    }
    void tree()
    {
        glColor3f(0.60f, 0.43f, 0.04f);  // slightly darker brown
        glBegin(GL_QUADS);//1
        glVertex2i(585, 400);
        glVertex2i(595, 400);
        glVertex2i(595, 490);
        glVertex2i(585, 490);
        glEnd();
        glBegin(GL_QUADS);//2
        glVertex2i(785, 400);
        glVertex2i(795, 400);
        glVertex2i(795, 490);
        glVertex2i(785, 490);
        glEnd();
        glBegin(GL_QUADS);//3 - rightmost tree trunk moved further right
        glVertex2i(935, 400);
        glVertex2i(945, 400);
        glVertex2i(945, 490);
        glVertex2i(935, 490);
        glEnd();

        // tree leaves
        glColor3f(0.0f, 0.5f, 0.0f);  // dark green
        glBegin(GL_TRIANGLES);//1
        glVertex2i(540, 490);
        glVertex2i(590, 550);
        glVertex2i(640, 490);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(540, 520);
        glVertex2i(590, 610);
        glVertex2i(640, 520);
        glEnd();
        glBegin(GL_TRIANGLES);//2
        glVertex2i(740, 490);
        glVertex2i(790, 550);
        glVertex2i(840, 490);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(740, 520);
        glVertex2i(790, 610);
        glVertex2i(840, 520);
        glEnd();
        glBegin(GL_TRIANGLES);//3 - rightmost tree leaves moved further right
        glVertex2i(890, 490);
        glVertex2i(940, 550);
        glVertex2i(990, 490);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(890, 520);
        glVertex2i(940, 610);
        glVertex2i(990, 520);
        glEnd();
    }
    void greenPart_bush()
    {
        glBegin(GL_QUADS);
        glColor3f(0.380f, 0.670f, 0.380f);  //green bottom
        glVertex2i(0, 127);
        glVertex2i(1000, 127);
        glVertex2i(1000, 100);
        glVertex2i(0, 100);
        glEnd();

        glColor3f(0.0f, 0.3f, 0.0f);  // dark green bush
        drawCircle(450, 400, 25);
        drawCircle(490, 415, 25);
        drawCircle(530, 400, 25);
        drawCircle(650, 400, 25);
        drawCircle(690, 415, 25);
        drawCircle(730, 400, 25);

        glBegin(GL_QUADS);
        glColor3f(0.235f, 0.596f, 0.216f);  //green up
        glVertex2i(0, 347);
        glVertex2i(1000, 347);
        glVertex2i(1000, 400);
        glVertex2i(0, 400);
        glEnd();

        // Add grass pattern - original simple triangles
        glColor3f(0.0f, 0.3f, 0.0f);  // dark green grass

        // First cluster
        glBegin(GL_TRIANGLES);
        glVertex2i(20, 347);
        glVertex2i(25, 380);
        glVertex2i(30, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(15, 347);
        glVertex2i(10, 380);
        glVertex2i(25, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(25, 347);
        glVertex2i(40, 380);
        glVertex2i(35, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(40, 347);
        glVertex2i(35, 380);
        glVertex2i(30, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(45, 347);
        glVertex2i(50, 380);
        glVertex2i(35, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(35, 347);
        glVertex2i(20, 380);
        glVertex2i(25, 347);
        glEnd();

        // Second cluster
        glBegin(GL_TRIANGLES);
        glVertex2i(280, 347);
        glVertex2i(275, 380);
        glVertex2i(270, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(285, 347);
        glVertex2i(290, 380);
        glVertex2i(275, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(275, 347);
        glVertex2i(260, 380);
        glVertex2i(265, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(300, 347);
        glVertex2i(295, 380);
        glVertex2i(290, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(305, 347);
        glVertex2i(310, 380);
        glVertex2i(295, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(295, 347);
        glVertex2i(280, 380);
        glVertex2i(285, 347);
        glEnd();

        // Third cluster
        glBegin(GL_TRIANGLES);
        glVertex2i(480, 347);
        glVertex2i(475, 380);
        glVertex2i(470, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(485, 347);
        glVertex2i(490, 380);
        glVertex2i(475, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(475, 347);
        glVertex2i(460, 380);
        glVertex2i(465, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(500, 347);
        glVertex2i(495, 380);
        glVertex2i(490, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(505, 347);
        glVertex2i(510, 380);
        glVertex2i(495, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(495, 347);
        glVertex2i(480, 380);
        glVertex2i(485, 347);
        glEnd();

        // Fourth cluster
        glBegin(GL_TRIANGLES);
        glVertex2i(780, 347);
        glVertex2i(775, 380);
        glVertex2i(770, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(785, 347);
        glVertex2i(790, 380);
        glVertex2i(775, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(775, 347);
        glVertex2i(760, 380);
        glVertex2i(765, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(800, 347);
        glVertex2i(795, 380);
        glVertex2i(790, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(805, 347);
        glVertex2i(810, 380);
        glVertex2i(795, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(795, 347);
        glVertex2i(780, 380);
        glVertex2i(785, 347);
        glEnd();

        // Fifth cluster
        glBegin(GL_TRIANGLES);
        glVertex2i(980, 347);
        glVertex2i(975, 380);
        glVertex2i(970, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(985, 347);
        glVertex2i(990, 380);
        glVertex2i(975, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(975, 347);
        glVertex2i(960, 380);
        glVertex2i(965, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(960, 347);
        glVertex2i(955, 380);
        glVertex2i(950, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(965, 347);
        glVertex2i(970, 380);
        glVertex2i(955, 347);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(955, 347);
        glVertex2i(940, 380);
        glVertex2i(945, 347);
        glEnd();
    }
    void day_cloud()
    {
        glColor3f(1.0f, 1.0f, 1.0f);  // light grey
        drawCircle(90, 550, 20); // Cloud 1
        drawCircle(120, 550, 25);
        drawCircle(150, 550, 20);
        drawCircle(515, 570, 15); // Cloud 2
        drawCircle(540, 570, 20);
        drawCircle(565, 570, 15);
        drawCircle(745, 520, 15); // Cloud 3
        drawCircle(770, 520, 20);
        drawCircle(795, 520, 15);
        drawCircle(300, 490, 15); // Cloud 4
        drawCircle(330, 490, 20);
        drawCircle(360, 490, 15);
        drawCircle(905, 480, 15);  // Cloud 5
        drawCircle(930, 480, 20);
        drawCircle(955, 480, 15);
    }
    void night_cloud()
    {
        glColor3f(0.698f, 0.745f, 0.709f);
        drawCircle(90, 550, 20); // Cloud 1
        drawCircle(120, 550, 25);
        drawCircle(150, 550, 20);
        drawCircle(515, 570, 15); // Cloud 2
        drawCircle(540, 570, 20);
        drawCircle(565, 570, 15);
        drawCircle(745, 520, 15); // Cloud 3
        drawCircle(770, 520, 20);
        drawCircle(795, 520, 15);
        drawCircle(300, 490, 15); // Cloud 4
        drawCircle(330, 490, 20);
        drawCircle(360, 490, 15);
        drawCircle(905, 480, 15);  // Cloud 5
        drawCircle(930, 480, 20);
        drawCircle(955, 480, 15);
    }
    void car1()
    {
        // A red car, with a unique design, shifted to the upper lane.
        int y_offset = 100;

        // Body
        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.0f, 0.0f); // Red
        glVertex2i(150, 170 + y_offset);
        glVertex2i(400, 170 + y_offset);
        glVertex2i(400, 225 + y_offset);
        glVertex2i(150, 225 + y_offset);
        glEnd();

        // Cabin
        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.0f, 0.0f); // Red
        glVertex2i(190, 225 + y_offset);
        glVertex2i(360, 225 + y_offset);
        glVertex2i(310, 260 + y_offset);
        glVertex2i(240, 260 + y_offset);
        glEnd();

        // Headlight
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        glVertex2i(145, 210 + y_offset);
        glVertex2i(150, 210 + y_offset);
        glVertex2i(150, 225 + y_offset);
        glVertex2i(145, 225 + y_offset);
        glEnd();

        // Window
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.0f); // Black
        glVertex2i(197, 225 + y_offset);
        glVertex2i(347, 225 + y_offset);
        glVertex2i(310, 255 + y_offset);
        glVertex2i(240, 255 + y_offset);
        glEnd();

        // Window Divider
        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.0f, 0.0f); // Red
        glVertex2i(270, 225 + y_offset);
        glVertex2i(275, 225 + y_offset);
        glVertex2i(278, 255 + y_offset);
        glVertex2i(273, 255 + y_offset);
        glEnd();

        // Wheels
        glColor3f(0.0f, 0.0f, 0.0f); // Black
        drawCircle(200, 170 + y_offset, 20);
        drawCircle(350, 170 + y_offset, 20);
        glColor3f(0.698f, 0.745f, 0.709f); // Grey
        drawCircle(200, 170 + y_offset, 10);
        drawCircle(350, 170 + y_offset, 10);
    }
    void car2()
    {
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2i(450, 170);
        glVertex2i(700, 170);
        glVertex2i(700, 225);
        glVertex2i(450, 225);
        glEnd();
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2i(540, 260);
        glVertex2i(610, 260);
        glVertex2i(655, 225);
        glVertex2i(490, 225);
        glEnd();
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2i(575, 260);
        glVertex2i(580, 260);
        glVertex2i(580, 225);
        glVertex2i(575, 225);
        glEnd();
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2i(705, 210);
        glVertex2i(700, 210);
        glVertex2i(700, 225);
        glVertex2i(705, 225);
        glEnd();
        //window
        glBegin(GL_QUADS);
        glVertex2i(540, 255);
        glVertex2i(610, 255);
        glVertex2i(647, 225);
        glVertex2i(497, 225);
        glEnd();
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2i(575, 260);
        glVertex2i(580, 260);
        glVertex2i(580, 225);
        glVertex2i(575, 225);
        glEnd();
        //car wheel
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(500, 170, 20);
        drawCircle(650, 170, 20);
        glColor3f(0.698f, 0.745f, 0.709f);
        drawCircle(500, 170, 10);
        drawCircle(650, 170, 10);
    }
    void bus1()
    {
        glBegin(GL_POLYGON);
        glColor3f(0.8f, 0.3f, 0.0f);  // Orange color to distinguish from bus2
        glVertex2i(200, 230);  // Positioned higher than bus2
        glVertex2i(200, 290);
        glVertex2i(230, 360);
        glVertex2i(550, 360);
        glVertex2i(550, 230);
        glEnd();
        // bus door
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.0);
        glVertex2i(240, 240);
        glVertex2i(240, 350);
        glVertex2i(290, 350);
        glVertex2i(290, 240);
        glEnd();
        //bus window
        glBegin(GL_TRIANGLES);
        glVertex2i(210, 300);
        glVertex2i(235, 350);
        glVertex2i(235, 300);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2i(320, 300);
        glVertex2i(320, 350);
        glVertex2i(370, 350);
        glVertex2i(370, 300);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2i(400, 300);
        glVertex2i(400, 350);
        glVertex2i(450, 350);
        glVertex2i(450, 300);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2i(480, 300);
        glVertex2i(480, 350);
        glVertex2i(530, 350);
        glVertex2i(530, 300);
        glEnd();
        drawCircle(315, 230, 25);
        drawCircle(490, 230, 25);
        glColor3f(0.698f, 0.745f, 0.709f);
        drawCircle(315, 230, 15);
        drawCircle(490, 230, 15);
    }
    void bus2()
    {
        glBegin(GL_POLYGON);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2i(200, 165);
        glVertex2i(200, 225);
        glVertex2i(230, 295);
        glVertex2i(550, 295);
        glVertex2i(550, 165);
        glEnd();
        // bus door
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.0);
        glVertex2i(240, 175);
        glVertex2i(240, 285);
        glVertex2i(290, 285);
        glVertex2i(290, 175);
        glEnd();
        //bus window
        glBegin(GL_TRIANGLES);
        glVertex2i(210, 235);
        glVertex2i(235, 285);
        glVertex2i(235, 235);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2i(320, 235);
        glVertex2i(320, 285);
        glVertex2i(370, 285);
        glVertex2i(370, 235);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2i(400, 235);
        glVertex2i(400, 285);
        glVertex2i(450, 285);
        glVertex2i(450, 235);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2i(480, 235);
        glVertex2i(480, 285);
        glVertex2i(530, 285);
        glVertex2i(530, 235);
        glEnd();
        drawCircle(315, 165, 25);
        drawCircle(490, 165, 25);
        glColor3f(0.698f, 0.745f, 0.709f);
        drawCircle(315, 165, 15);
        drawCircle(490, 165, 15);
    }
    void plane()
    {
        glColor3f(0.392f, 0.392f, 0.392f);   //Dark Grey
        glBegin(GL_QUADS);
        glVertex2f(330, 550);
        glVertex2f(400, 550);
        glVertex2f(400, 550 + 15);
        glVertex2f(330, 550 + 15);
        glEnd();
        drawCircle(400, 558,7);
        glBegin(GL_QUADS); // tail
        glVertex2f(330, 550 );
        glVertex2f(345, 550 );
        glVertex2f(325, 550 + 35);
        glVertex2f(310, 550 + 35);
        glEnd();
        glColor3f(0.27f, 0.39f, 0.55f); //plane inside blue part
        glBegin(GL_QUADS);
        glVertex2f(335, 552);
        glVertex2f(395, 552);
        glVertex2f(395, 550 + 13);
        glVertex2f(335, 550 + 13);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(333, 552 );
        glVertex2f(342, 552 );
        glVertex2f(322, 550 + 33);
        glVertex2f(313, 550 + 33);
        glEnd();
        drawCircle(400, 558,4);
        // Wing
        glColor3f(0.392f, 0.392f, 0.392f);  //Dark Grey
        glBegin(GL_QUADS);
        glVertex2f(370, 550 + 15);
        glVertex2f(380, 550 + 15);
        glVertex2f(360, 550 + 35);
        glVertex2f(350, 550 + 35);
        glEnd();
        glColor3f(0.392f, 0.392f, 0.392f);  //Dark Grey
        glBegin(GL_QUADS);
        glVertex2f(370, 550);
        glVertex2f(380, 550);
        glVertex2f(360, 550 - 20);
        glVertex2f(350, 550 - 20);
        glEnd();
    }
    void house_day()
    {
        glColor3f(0.60f, 0.40f, 0.30f);  //house 1
        glBegin(GL_QUADS);
        glVertex2f(10, 400);
        glVertex2f(140, 400);
        glVertex2f(140, 580);
        glVertex2f(10, 580);
        glEnd();
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(20, 570);
        glVertex2f(67, 570);
        glVertex2f(67, 520);
        glVertex2f(20, 520);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(130, 570);
        glVertex2f(78, 570);
        glVertex2f(78, 520);
        glVertex2f(130, 520);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(20, 510);
        glVertex2f(67, 510);
        glVertex2f(67, 460);
        glVertex2f(20, 460);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(130, 460);
        glVertex2f(78, 460);
        glVertex2f(78, 510);
        glVertex2f(130, 510);
        glEnd();
        //door 1
        glBegin(GL_QUADS);
        glVertex2f(60, 400);
        glVertex2f(85, 400);
        glVertex2f(85, 450);
        glVertex2f(60, 450);
        glEnd();

        glColor3f(0.33f, 0.60f, 0.50f);   //house2
        glBegin(GL_QUADS);
        glVertex2f(280, 400);
        glVertex2f(150, 400);
        glVertex2f(150, 580);
        glVertex2f(280, 580);
        glEnd();
        //window 2
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(270, 550);
        glVertex2f(160, 550);
        glVertex2f(160, 570);
        glVertex2f(270, 570);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(270, 540);
        glVertex2f(160, 540);
        glVertex2f(160, 480);
        glVertex2f(270, 480);
        glEnd();
        //door 2
        glBegin(GL_QUADS);
        glVertex2f(195, 400);
        glVertex2f(235, 400);
        glVertex2f(235, 470);
        glVertex2f(195, 470);
        glEnd();

        glColor3f(0.27f, 0.39f, 0.55f); //house 3
        glBegin(GL_QUADS);
        glVertex2f(290, 400);
        glVertex2f(420, 400);
        glVertex2f(420, 585);
        glVertex2f(290, 585);
        glEnd();
        //window 3
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(300, 570);
        glVertex2f(350, 570);
        glVertex2f(350, 520);
        glVertex2f(300, 520);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(410, 570);
        glVertex2f(358, 570);
        glVertex2f(358, 520);
        glVertex2f(410, 520);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(300, 510);
        glVertex2f(350, 510);
        glVertex2f(350, 460);
        glVertex2f(300, 460);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(410, 460);
        glVertex2f(358, 460);
        glVertex2f(358, 510);
        glVertex2f(410, 510);
        glEnd();
        //door 3
        glBegin(GL_QUADS);
        glVertex2f(370, 400);
        glVertex2f(335, 400);
        glVertex2f(335, 450);
        glVertex2f(370, 450);
        glEnd();
    }
    void house_night()
    {
        // House 1
        glColor3f(0.60f, 0.40f, 0.30f);
        glBegin(GL_QUADS);
        glVertex2f(10, 400);
        glVertex2f(140, 400);
        glVertex2f(140, 580);
        glVertex2f(10, 580);
        glEnd();

        // House 1, second floor windows (cool white)
        glColor3f(0.9f, 0.9f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(20, 570);
        glVertex2f(67, 570);
        glVertex2f(67, 520);
        glVertex2f(20, 520);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(130, 570);
        glVertex2f(78, 570);
        glVertex2f(78, 520);
        glVertex2f(130, 520);
        glEnd();

        // House 1, first floor windows (pale yellow)
        glColor3f(1.0f, 1.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(20, 510);
        glVertex2f(67, 510);
        glVertex2f(67, 460);
        glVertex2f(20, 460);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(130, 460);
        glVertex2f(78, 460);
        glVertex2f(78, 510);
        glVertex2f(130, 510);
        glEnd();

        // Door 1
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.2f, 0.2f);
        glVertex2f(60, 400);
        glVertex2f(85, 400);
        glVertex2f(85, 450);
        glVertex2f(60, 450);
        glEnd();

        // House 2
        glColor3f(0.33f, 0.60f, 0.50f);
        glBegin(GL_QUADS);
        glVertex2f(280, 400);
        glVertex2f(150, 400);
        glVertex2f(150, 580);
        glVertex2f(280, 580);
        glEnd();

        // House 2, windows (warm white)
        glColor3f(1.0f, 0.87f, 0.73f);
        glBegin(GL_QUADS);
        glVertex2f(270, 550);
        glVertex2f(160, 550);
        glVertex2f(160, 570);
        glVertex2f(270, 570);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(270, 540);
        glVertex2f(160, 540);
        glVertex2f(160, 480);
        glVertex2f(270, 480);
        glEnd();

        // Door 2
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.2f, 0.2f);
        glVertex2f(195, 400);
        glVertex2f(235, 400);
        glVertex2f(235, 470);
        glVertex2f(195, 470);
        glEnd();

        // House 3
        glColor3f(0.27f, 0.39f, 0.55f);
        glBegin(GL_QUADS);
        glVertex2f(290, 400);
        glVertex2f(420, 400);
        glVertex2f(420, 585);
        glVertex2f(290, 585);
        glEnd();

        // House 3, second floor windows (cool white)
        glColor3f(0.9f, 0.9f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(300, 570);
        glVertex2f(350, 570);
        glVertex2f(350, 520);
        glVertex2f(300, 520);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(410, 570);
        glVertex2f(358, 570);
        glVertex2f(358, 520);
        glVertex2f(410, 520);
        glEnd();

        // House 3, first floor windows (pinkish white)
        glColor3f(1.0f, 0.9f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(300, 510);
        glVertex2f(350, 510);
        glVertex2f(350, 460);
        glVertex2f(300, 460);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(410, 460);
        glVertex2f(358, 460);
        glVertex2f(358, 510);
        glVertex2f(410, 510);
        glEnd();

        // Door 3
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.2f, 0.2f);
        glVertex2f(370, 400);
        glVertex2f(335, 400);
        glVertex2f(335, 450);
        glVertex2f(370, 450);
        glEnd();
    }
    void bird()
    {
        glColor3f(0.392f, 0.392f, 0.392f);
        glBegin(GL_TRIANGLES);  //1
        glVertex2d(350,550);
        glVertex2d(353,555);
        glVertex2d(343,570);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2d(350,550);
        glVertex2d(353,555);
        glVertex2d(358,575);
        glEnd();
        glBegin(GL_TRIANGLES);   //2
        glVertex2d(400,500);
        glVertex2d(403,505);
        glVertex2d(393,520);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2d(400,500);
        glVertex2d(403,505);
        glVertex2d(408,525);
        glEnd();
        glBegin(GL_TRIANGLES);   //3
        glVertex2d(450,550);
        glVertex2d(453,555);
        glVertex2d(443,570);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2d(450,550);
        glVertex2d(453,555);
        glVertex2d(458,575);
        glEnd();
    }
    void bench()
    {
        // Seat
        glColor3f(0.4f, 0.2f, 0.0f);  // Brown wood color
        glBegin(GL_QUADS);
        glVertex2i(665, 360);
        glVertex2i(765, 360);
        glVertex2i(765, 375);
        glVertex2i(665, 375);
        glEnd();

        // Backrest
        glBegin(GL_QUADS);
        glVertex2i(665, 375);
        glVertex2i(765, 375);
        glVertex2i(765, 395);
        glVertex2i(665, 395);
        glEnd();

        // Legs
        glColor3f(0.3f, 0.3f, 0.3f);  // Dark gray for metal legs
        // Left leg
        glBegin(GL_QUADS);
        glVertex2i(675, 347);
        glVertex2i(685, 347);
        glVertex2i(685, 360);
        glVertex2i(675, 360);
        glEnd();

        // Right leg
        glBegin(GL_QUADS);
        glVertex2i(745, 347);
        glVertex2i(755, 347);
        glVertex2i(755, 360);
        glVertex2i(745, 360);
        glEnd();

        // Simple decorative lines on seat and backrest
        glColor3f(0.3f, 0.15f, 0.0f);  // Darker brown for lines
        glBegin(GL_LINES);
        // Seat lines
        glVertex2i(665, 368);
        glVertex2i(765, 368);
        // Backrest lines
        glVertex2i(665, 385);
        glVertex2i(765, 385);
        glEnd();
    }
    void init() {
        // Initialize rain drops at random positions
        for(int i = 0; i < 100; i++) {
            rainY[i] = rand() % 600;  // Random Y position
            rainX[i] = rand() % 1000; // Random X position
        }
    }
    void drawRain() {
        glColor4f(0.7f, 0.7f, 1.0f, 0.5f);  // Light blue-white, semi-transparent
        glLineWidth(1.5f);

        for(int i = 0; i < 100; i++) {
        glBegin(GL_LINES);
            glVertex2f(rainX[i], rainY[i]);
            glVertex2f(rainX[i] - 3, rainY[i] - 10);  // Slanted rain drops
        glEnd();

            // Update rain drop position
            rainY[i] -= 20.0f;  // Move down
            rainX[i] -= 2.5f;  // Move slightly left for wind effect

            // Reset rain drop if it goes off screen
            if(rainY[i] < 0) {
                rainY[i] = rand() % 600 + 600;  // Reset to top
                rainX[i] = rand() % 1000;
            }
            if(rainX[i] < 0) {
                rainX[i] = 1000;
            }
        }
    }
    void updateFlood(int value) {
        if(isRaining) {
            floodLevel += 0.3f;
            if(floodLevel > 50.0f) floodLevel = 50.0f;
        } else {
            floodLevel -= 0.2f;
            if(floodLevel < 0.0f) floodLevel = 0.0f;
        }
        glutPostRedisplay();
    }
    void river() {
        float currentHeight = 100.0f + floodLevel;  // Base height plus flood level

        // Main river body with flood level
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.3f, 0.7f);  // Darker blue for flood water
        glVertex2i(0, 0);
        glVertex2i(1000, 0);
        glVertex2i(1000, currentHeight);
        glVertex2i(0, currentHeight);
        glEnd();

        // Flood water effect (more turbulent)
        glBegin(GL_LINES);
        glColor4f(0.2f, 0.4f, 0.8f, 0.7f);  // Slightly different blue for turbulence

        int offset;
        for(int i = 0; i < 1000; i += 30) {  // More frequent waves during flood
            offset = (i + (int)waterFlow) % 1000;

            // Multiple layers of choppy waves
            for(float y = 20; y < currentHeight; y += 25) {
                glVertex2i(offset, y);
                glVertex2i(offset + 20, y + (isRaining ? 5 : 0));  // Choppier waves when raining
            }
        }
        glEnd();

        // Rain splash effects on water
        if(isRaining) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
            for(int i = 0; i < 20; i++) {
                float splashX = rand() % 1000;
                float splashY = rand() % (int)currentHeight;
                drawCircle(splashX, splashY, 2);
            }
        }
    }
    void boat() {
        glPushMatrix();
        glTranslatef(boatX, 25.0f, 0.0f);  // Lowered Y position to sit lower in river
        glScalef(8.0f, 8.0f, 1.0f);  // Maintained the same scale

        // Upper red part
        glColor3f(1.0f, 0.0f, 0.0f);  // Red
        glBegin(GL_POLYGON);
        glVertex2f(3.5, 5.5);
        glVertex2f(3.5, 8.5);
        glVertex2f(19.5, 8.5);
        glVertex2f(19.5, 5.5);
        glEnd();

        // Lower hull - dark brown
        glColor3f(0.36f, 0.25f, 0.20f);  // Rich dark brown color
        glBegin(GL_POLYGON);
        glVertex2f(1.0, 5.5);
        glVertex2f(4.0, 1.0);
        glVertex2f(19.0, 1.0);
        glVertex2f(21.5, 5.5);
        glEnd();

        // Brown mast
        glColor3f(0.7f, 0.4f, 0.2f);  // Brown
        glBegin(GL_POLYGON);
        glVertex2f(4.0, 5.5);
        glVertex2f(4.0, 8.0);
        glVertex2f(5.0, 8.0);
        glVertex2f(5.0, 5.5);
        glEnd();

        // Windows (white rectangles)
        glColor3f(0.9f, 0.9f, 0.9f);  // White
        // First window
        glBegin(GL_POLYGON);
        glVertex2f(7.0, 7.5);
        glVertex2f(7.0, 8.0);
        glVertex2f(10.0, 8.0);
        glVertex2f(10.0, 7.5);
        glEnd();
        // Second window
        glBegin(GL_POLYGON);
        glVertex2f(11.5, 7.5);
        glVertex2f(11.5, 8.0);
        glVertex2f(15.0, 8.0);
        glVertex2f(15.0, 7.5);
        glEnd();
        // Third window
        glBegin(GL_POLYGON);
        glVertex2f(16.0, 7.5);
        glVertex2f(16.0, 8.0);
        glVertex2f(19.0, 8.0);
        glVertex2f(19.0, 7.5);
        glEnd();

        // Grey upper deck
        glColor3f(0.5f, 0.5f, 0.5f);  // Grey
        glBegin(GL_POLYGON);
        glVertex2f(4.5, 8.5);
        glVertex2f(4.5, 10.0);
        glVertex2f(18.5, 10.0);
        glVertex2f(18.5, 8.5);
        glEnd();

        // Four chimneys with orange tops
        for(float x = 5.5f; x <= 14.5f; x += 3.0f) {
            // Black chimney
            glColor3f(0.0f, 0.0f, 0.0f);
            glBegin(GL_POLYGON);
            glVertex2f(x, 10.0);
            glVertex2f(x, 12.9);
            glVertex2f(x + 1.5f, 12.9);
            glVertex2f(x + 1.5f, 10.0);
            glEnd();

            // Orange top
            glColor3f(0.9f, 0.5f, 0.2f);
            glBegin(GL_POLYGON);
            glVertex2f(x, 12.9);
            glVertex2f(x, 13.5);
            glVertex2f(x + 1.5f, 13.5);
            glVertex2f(x + 1.5f, 12.9);
        glEnd();
    }

        // Hull details (black lines)
        glColor3f(0.0f, 0.0f, 0.0f);
        for(float x = 1.5f; x < 20.0f; x += 2.0f) {
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, 5.5);
            glVertex2f(x, 6.5);
            glVertex2f(x + 0.5f, 6.2);
            glVertex2f(x + 1.5f, 6.2);
            glVertex2f(x + 2.0f, 6.5);
            glVertex2f(x + 2.0f, 5.5);
            glEnd();
        }

        // Window details (white lines)
        glColor3f(1.0f, 1.0f, 1.0f);
        for(float x = 4.5f; x < 18.0f; x += 2.5f) {
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, 3.5);
            glVertex2f(x, 4.5);
            glVertex2f(x + 1.0f, 4.5);
            glVertex2f(x + 1.0f, 3.5);
            glEnd();
        }

        glPopMatrix();
    }
    void night_rain_clouds()
    {
        if (!isRaining) return;

        glColor3f(0.1f, 0.1f, 0.15f);
        // Cloud clusters
        drawCircle(90, 550, 25);  drawCircle(120, 550, 30);  drawCircle(150, 550, 25);
        drawCircle(515, 570, 20); drawCircle(540, 570, 25);  drawCircle(565, 570, 20);
        drawCircle(745, 520, 20); drawCircle(770, 520, 25);  drawCircle(795, 520, 20);
        drawCircle(300, 490, 20); drawCircle(330, 490, 25);  drawCircle(360, 490, 20);
        drawCircle(905, 480, 20); drawCircle(930, 480, 25);  drawCircle(955, 480, 20);
        drawCircle(200, 520, 22); drawCircle(400, 540, 28);  drawCircle(600, 510, 24);
        drawCircle(800, 530, 26);
    }
    void display()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (day) {
            if(isRaining) {
                glColor3f(0.7f, 0.7f, 0.7f);
                glBegin(GL_QUADS);
                glVertex2i(0,600); glVertex2i(1000, 600);
                glVertex2i(1000, 300); glVertex2i(0, 300);
                glEnd();
            } else {
            day_sky();
            sun();
            }

            lamp_daylight();
            glPushMatrix();
            glTranslatef(cloudX, 0.0f, 0.0f);
            day_cloud();
            glPopMatrix();

            if(!isRaining) {
            glPushMatrix();
            glTranslatef(position, position, 0.0f);
            bird();
            glPopMatrix();
            }
            house_day();
            greenPart_bush();
        } else {
            night_sky();
            sun();
            night_rain_clouds();

            if(!isRaining) {
            glPushMatrix();
            glTranslatef(planeX, 0.0f, 0.0f);
            plane();
            glPopMatrix();
            }

            lamp_nightlight();
            glPushMatrix();
            glTranslatef(cloudX, 0.0f, 0.0f);
            night_cloud();
            glPopMatrix();
            greenPart_bush();
            house_night();
        }

        road();
        tree();
        lamp_stand();
        bench();
        river();
        boat();

        if(isRaining) {
            drawRain();
        }

        if (show_vehicle) {
            if (day) {
                glPushMatrix();
                glTranslatef(400, 0.0f, 0.0f);
                glPushMatrix();
                glTranslatef(busX, 0.0f, 0.0f);
                bus1();
                glPopMatrix();
                glPushMatrix();
                glTranslatef(busX2, 0.0f, 0.0f);
                bus2();
                glPopMatrix();
                glPopMatrix();
            } else {
                glPushMatrix();
                glPushMatrix();
                glTranslatef(carX, 0.0f, 0.0f);
                car1();
                glPopMatrix();
                glPushMatrix();
                glTranslatef(carX2, 0.0f, 0.0f);
                car2();
                glPopMatrix();
                glPopMatrix();
            }
        }
        glFlush();
        glutSwapBuffers();
    }
    void updateCloud(int value)
    {
        cloudX += 1.5f;
        if (cloudX > 1000)
        {
            cloudX = -1000.0f;
        }
        glutPostRedisplay();
    }
    void updatePlane(int value)
    {
        planeX += 2.0f;
        if (planeX > 1000)
        {
            planeX = -500.0f;
        }
        glutPostRedisplay();
    }
    void updateSun(int value)
    {
        if (moveSunDown)
        {
            sunY -= sunSpeed;
            if(sunY <= 280)
            {
                day= false;
            }
            if (sunY < 260)
            {
                sunY = 260;
            }
        }
        else if (moveSunUp)
        {
            sunY += sunSpeed;
            if (sunY > 280)
            {
                day = true;
            }
            if (sunY > 550)
            {
                sunY = 550;
            }
        }
        glutPostRedisplay();
    }
    void updateBird(int value)
    {
        if(position > 550.0f)
        {
            position = -200.0f;  // Reset position to start from off-screen
        }
        position += speed; // Move along both x & y axes

        glutPostRedisplay(); // Notify GLUT that the display has changed
    }
    void updateWater(int value)
    {
        waterFlow += 1.0f;
        if(waterFlow >= 1000.0f) {
            waterFlow = 0.0f;
        }
        glutPostRedisplay();
    }
    void updateBoat(int value) {
        boatX += boatSpeed;
        if(boatX > 1000) {
            boatX = -200.0f;
            boatSpeed = 1.5f; // Reset speed when boat restarts
        }
        glutPostRedisplay();
    }
    void updateCar1(int value) {
        carX -= 3.0f;  // Moving left to right
        if (carX < -700) {
            carX = 1000.0f;  // Reset to left side
        }
        glutPostRedisplay();
    }
    void updateCar2(int value) {
        carX2 += 3.5f;  // Same speed as car1
        if (carX2 > 1000) {
            carX2 = -800.0f;  // Reset to left side with spacing
        }
        glutPostRedisplay();
    }
    void updateBus1(int value) {
        busX -= 3.0f;
        if (busX < -1000.0f) {
            busX = 1000.0f;
        }
        glutPostRedisplay();
    }
    void updateBus2(int value) {
        busX2 -= 3.8f;
        if (busX2 < -1000.0f) {
            busX2 = 1000.0f;
        }
        glutPostRedisplay();
    }
    void mouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // Convert window y to OpenGL y
            int oglY = 600 - y;
            // Boat's bounding box in world coordinates
            float boatLeft = boatX;
            float boatRight = boatX + 21.5f * 8.0f; // boat width * scale
            float boatBottom = 25.0f;
            float boatTop = 25.0f + 13.5f * 8.0f; // boat height * scale
            if (x >= boatLeft && x <= boatRight && oglY >= boatBottom && oglY <= boatTop) {
                boatSpeed += 1.0f; // Increase speed
                if (boatSpeed > 10.0f) boatSpeed = 10.0f; // Cap speed
            }
        }
    }
    void keyboard(unsigned char key, int x, int y)
    {
        switch (key)
        {
        case 'e':
        case 'E':
            show_vehicle = true;
            glutPostRedisplay();
            break;
        case 'f':
        case 'F':
            show_vehicle = false;
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            moveSunDown = true;
            moveSunUp = false;
            glutPostRedisplay();
            break;
        case 'u':
        case 'U':
            moveSunUp = true;
            moveSunDown = false;
            glutPostRedisplay();
            break;
        case 'r':
            isRaining = !isRaining;
            if (isRaining) {
                isFlooded = true;
                floodLevel = 0.0f;
            } else {
                isFlooded = false;  // Stop flood when rain stops
                floodLevel = 0.0f;  // Reset flood level
            }
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        }
    }

    void initScene() {
        std::cout << "Mahtab's scene initialized." << std::endl;

        cout<< "--- Controls ---" <<endl;
        cout<<"For rain press : r"<<endl;
    }
    void cleanupScene() {
        std::cout << "Mahhtab's scene cleaned up." << std::endl;
    }
    void updateScene() {
        updateCloud(0);
        updateCar1(0);
        updateCar2(0);
        updateBus1(0);
        updateBus2(0);
        updatePlane(0);
        updateWater(0);
        updateBoat(0);
        updateFlood(0);
        updateSun(0);
        updateBird(0);
    }
}
#pragma once
#include <GL/glut.h>
#include<GL/gl.h>
#include <iostream>
#include<windows.h>
#include<math.h>

using namespace std;

namespace Masud {

    bool FisDay=true;

    void Fcircle(int centerX, int centerY, int radius)
    {
        glBegin(GL_POLYGON);
        for(int i = 0; i < 360; i++)
        {
            float pi = 3.1416;
            float A  = (i*2*pi)/180 ;
            float r  = radius;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x + centerX, y + centerY );
        }
        glEnd();
    }
    void FRiver() {
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.6f, 1.0f);
        glVertex2f(0, 0);
        glVertex2d(1000, 0);             //river
        glVertex2d(1000, 300);
        glVertex2d(0, 300);
        glEnd();

    }

    void FRiverNight() {
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.4f);
        glVertex2f(0, 0);
        glVertex2d(1000, 0);             //riverNight
        glVertex2d(1000, 300);
        glVertex2d(0, 300);
        glEnd();

    }

    void FRiverBank() {
        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.6f, 0.7f);
        glVertex2f(0, 290);
        glVertex2d(1000, 290);             //riverBank
        glVertex2d(1010, 310);
        glVertex2d(0, 310);
        glEnd();

    }

    void FRoad() {
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(0, 310);
        glVertex2f(1000, 310);      //road
        glVertex2f(1000, 400);
        glVertex2f(0, 400);
        glEnd();

        glLineWidth(4);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int x = 10; x < 1000; x += 90) {      //divider
            glVertex2f(x, 350);
            glVertex2f(x + 50, 350);
        }
        glEnd();

    }

    void FSidewalk() {
        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.4f, 0.0f);
        glVertex2f(0, 400);
        glVertex2f(1000, 400);
        glVertex2f(1000, 420);
        glVertex2f(0, 420);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.6f, 0.7f);
        glVertex2f(0, 400);
        glVertex2f(1000, 400);
        glVertex2f(1000, 405);
        glVertex2f(0, 405);
        glEnd();


    }

    void FSky() {
        glBegin(GL_QUADS);
        glColor3f(.63f, 0.85f, 1.0f);
        glVertex2f(0, 420);
        glVertex2d(1000, 420);                 //sky
        glVertex2d(1000, 600);
        glVertex2d(0, 600);
        glEnd();


    }

    void FSkyNight() {
        glBegin(GL_QUADS);
        glColor3f(.0f, 0.2f, 0.4f);
        glVertex2f(0, 420);
        glVertex2d(1000, 420);               //night sky
        glVertex2d(1000, 600);
        glVertex2d(0, 600);
        glEnd();


    }

    void FBuilding3() {
        glBegin(GL_QUADS);
        glColor3f(.7f, .8f, .9f);
        glVertex2f(200, 420);
        glVertex2d(295, 420);
        glVertex2d(295, 520);
        glVertex2d(200, 520);
        glEnd();

        for (int i = 0; i < 4; i++) {
            glBegin(GL_QUADS);
            glColor3f(0.75f, 0.75f, 0.75f);
            glVertex2f(210 + i * 20, 430);
            glVertex2d(220 + i * 20, 430);
            glVertex2d(220 + i * 20, 510);
            glVertex2d(210 + i * 20, 510);
            glEnd();
        }

    }

    void FBuilding3Night() {
        glBegin(GL_QUADS);
        glColor3f(.7f, .8f, .9f);
        glVertex2f(200, 420);
        glVertex2d(295, 420);
        glVertex2d(295, 520);
        glVertex2d(200, 520);
        glEnd();

        for (int i = 0; i < 4; i++) {
            glBegin(GL_QUADS);
            glColor3f(1.f, 1.0f, 0.0f);
            glVertex2f(210 + i * 20, 430);
            glVertex2d(220 + i * 20, 430);
            glVertex2d(220 + i * 20, 510);
            glVertex2d(210 + i * 20, 510);
            glEnd();
        }

    }

    void FBuilding1() {
        glBegin(GL_POLYGON);
        glColor3f(0.4f, 0.5f, 0.2f);
        glVertex2f(0, 420);
        glVertex2f(100, 420);
        glVertex2f(100, 500);
        glVertex2f(50, 550);
        glVertex2f(0, 500);
        glEnd();

        // Windows
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(10 + i * 20, 430 + j * 30);
                glVertex2f(18 + i * 20, 430 + j * 30);
                glVertex2f(18 + i * 20, 440 + j * 30);
                glVertex2f(10 + i * 20, 440 + j * 30);
                glEnd();
            }
        }

    }


    void FBuilding1Night() {
        glBegin(GL_POLYGON);
        glColor3f(0.0f, 0.2f, 0.1f);
        glVertex2f(0, 420);
        glVertex2f(100, 420);
        glVertex2f(100, 500);
        glVertex2f(50, 550);
        glVertex2f(0, 500);
        glEnd();

        // Windows
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(10 + i * 20, 430 + j * 30);
                glVertex2f(18 + i * 20, 430 + j * 30);
                glVertex2f(18 + i * 20, 440 + j * 30);
                glVertex2f(10 + i * 20, 440 + j * 30);
                glEnd();
            }
        }

    }


    void FBuilding2() {
        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.6f, 0.6f);
        glVertex2f(100, 420);
        glVertex2f(200, 420);
        glVertex2f(200, 570);
        glVertex2f(100, 570);
        glEnd();

        // Windows
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 6; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(115 + i * 20, 430 + j * 25);
                glVertex2f(125 + i * 20, 430 + j * 25);
                glVertex2f(125 + i * 20, 440 + j * 25);
                glVertex2f(115 + i * 20, 440 + j * 25);
                glEnd();
            }
        }

    }

    void FBuilding2Night() {
        glBegin(GL_QUADS);
        glColor3f(0.1f, 0.24f, 0.4f);
        glVertex2f(100, 420);
        glVertex2f(200, 420);
        glVertex2f(200, 570);
        glVertex2f(100, 570);
        glEnd();

        // Windows
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 6; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(115 + i * 20, 430 + j * 25);
                glVertex2f(125 + i * 20, 430 + j * 25);
                glVertex2f(125 + i * 20, 440 + j * 25);
                glVertex2f(115 + i * 20, 440 + j * 25);
                glEnd();
            }
        }

    }

    void FBuilding4() {
        glBegin(GL_POLYGON);
        glColor3f(0.8f, 0.5f, 0.2f);
        glVertex2f(300, 420);
        glVertex2f(400, 420);
        glVertex2f(400, 470);
        glVertex2f(300, 470);
        glEnd();

        // Windows
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 2; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(305 + i * 18, 430 + j * 20);
                glVertex2f(315 + i * 18, 430 + j * 20);
                glVertex2f(315 + i * 18, 440 + j * 20);
                glVertex2f(305 + i * 18, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding4Night() {
        glBegin(GL_POLYGON);
        glColor3f(0.58f, 0.3f, 0.2f);
        glVertex2f(300, 420);
        glVertex2f(400, 420);
        glVertex2f(400, 470);
        glVertex2f(300, 470);
        glEnd();


        // Windows
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 2; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(305 + i * 18, 430 + j * 20);
                glVertex2f(315 + i * 18, 430 + j * 20);
                glVertex2f(315 + i * 18, 440 + j * 20);
                glVertex2f(305 + i * 18, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding5()
    {
    glBegin(GL_POLYGON);
        glColor3f(1.f, 1.f, 1.f);
        glVertex2d(400,   420);
        glVertex2d(450,   420);
        glVertex2d(450,   550);
        glVertex2d(400,   550);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.f,0.5f,0.5f);
        glVertex2f(450,   420);
        glVertex2d(480,   420);
        glVertex2d(480,   550);
        glVertex2d(450,   550);
        glEnd();


        // Windows
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 6; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(405 + i * 15, 430 + j * 20);
                glVertex2f(415 + i * 15, 430 + j * 20);
                glVertex2f(415 + i * 15, 440 + j * 20);
                glVertex2f(405 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }



    void FBuilding5Night()
    {
    glBegin(GL_POLYGON);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex2d(400,   420);
        glVertex2d(450,   420);
        glVertex2d(450,   550);
        glVertex2d(400,   550);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(.45f,0.f,0.f);
        glVertex2f(450,   420);
        glVertex2d(480,   420);
        glVertex2d(480,   550);
        glVertex2d(450,   550);
        glEnd();


        // Windows
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 6; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(405 + i * 15, 430 + j * 20);
                glVertex2f(415 + i * 15, 430 + j * 20);
                glVertex2f(415 + i * 15, 440 + j * 20);
                glVertex2f(405 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding6() {
        glBegin(GL_POLYGON);
        glColor3f(0.8f, 0.5f, 0.8f);
        glVertex2f(480, 420);
        glVertex2f(550, 420);
        glVertex2f(550, 520);
        glVertex2f(480, 520);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(490, 520);
        glVertex2f(540, 520);
        glVertex2f(540, 530);
        glVertex2f(490, 530);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.2f, 0.6f, 0.4f);
        glVertex2f(500, 530);
        glVertex2f(530, 530);
        glVertex2f(530, 540);
        glVertex2f(500, 540);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.5f, 0.5f, 0.8f);
        glVertex2f(514, 540);
        glVertex2f(516, 540);
        glVertex2f(516, 570);
        glVertex2f(514, 570);
        glEnd();

        // Windows
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(485 + i * 15, 430 + j * 20);
                glVertex2f(495 + i * 15, 430 + j * 20);
                glVertex2f(495 + i * 15, 440 + j * 20);
                glVertex2f(485 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding6Night() {
        glBegin(GL_POLYGON);
        glColor3f(0.1f, 0.1f, 0.5f);
        glVertex2f(480, 420);
        glVertex2f(550, 420);
        glVertex2f(550, 520);
        glVertex2f(480, 520);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(490, 520);
        glVertex2f(540, 520);
        glVertex2f(540, 530);
        glVertex2f(490, 530);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.2f, 0.6f, 0.4f);
        glVertex2f(500, 530);
        glVertex2f(530, 530);
        glVertex2f(530, 540);
        glVertex2f(500, 540);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.5f, 0.5f, 0.8f);
        glVertex2f(514, 540);
        glVertex2f(516, 540);
        glVertex2f(516, 570);
        glVertex2f(514, 570);
        glEnd();

        // Windows
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(485 + i * 15, 430 + j * 20);
                glVertex2f(495 + i * 15, 430 + j * 20);
                glVertex2f(495 + i * 15, 440 + j * 20);
                glVertex2f(485 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding7() {
        glBegin(GL_POLYGON);
        glColor3f(0.6f, 0.4f, 0.3f);
        glVertex2f(550, 420);
        glVertex2f(650, 420);
        glVertex2f(650, 500);
        glVertex2f(550, 500);
        glEnd();


        // Windows
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 3; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(555 + i * 15, 430 + j * 20);
                glVertex2f(565 + i * 15, 430 + j * 20);
                glVertex2f(565 + i * 15, 440 + j * 20);
                glVertex2f(555 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }



    void FBuilding7Night() {
        glBegin(GL_POLYGON);
        glColor3f(0.4f, 0.2f, 0.f);
        glVertex2f(550, 420);
        glVertex2f(650, 420);
        glVertex2f(650, 500);
        glVertex2f(550, 500);
        glEnd();

        // Windows
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 3; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(555 + i * 15, 430 + j * 20);
                glVertex2f(565 + i * 15, 430 + j * 20);
                glVertex2f(565 + i * 15, 440 + j * 20);
                glVertex2f(555 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding8() {
        glBegin(GL_POLYGON);
        glColor3f(0.8f, 0.5f, 0.2f);
        glVertex2f(650, 420);
        glVertex2f(750, 420);
        glVertex2f(750, 470);
        glVertex2f(650, 470);
        glEnd();

        // Windows
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 2; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(655 + i * 18, 430 + j * 20);
                glVertex2f(665 + i * 18, 430 + j * 20);
                glVertex2f(665 + i * 18, 440 + j * 20);
                glVertex2f(655 + i * 18, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding8Night() {
        glBegin(GL_POLYGON);
        glColor3f(0.6f, 0.3f, 0.f);
        glVertex2f(650, 420);
        glVertex2f(750, 420);
        glVertex2f(750, 470);
        glVertex2f(650, 470);
        glEnd();

        // Windows
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 2; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(655 + i * 18, 430 + j * 20);
                glVertex2f(665 + i * 18, 430 + j * 20);
                glVertex2f(665 + i * 18, 440 + j * 20);
                glVertex2f(655 + i * 18, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding9() {
        glBegin(GL_POLYGON);
        glColor3f(0.7f, 0.7f, 0.2f);
        glVertex2f(800, 420);
        glVertex2f(900, 420);
        glVertex2f(900, 500);
        glVertex2f(850, 550);
        glVertex2f(800, 500);
        glEnd();


        // Windows
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(810 + i * 20, 430 + j * 30);
                glVertex2f(820 + i * 20, 430 + j * 30);
                glVertex2f(820 + i * 20, 440 + j * 30);
                glVertex2f(810 + i * 20, 440 + j * 30);
                glEnd();
            }
        }

    }

    void FBuilding9Night() {
        glBegin(GL_POLYGON);
        glColor3f(0.0f, 0.4f, 0.2f);
        glVertex2f(800, 420);
        glVertex2f(900, 420);
        glVertex2f(900, 500);
        glVertex2f(850, 550);
        glVertex2f(800, 500);
        glEnd();


        // Windows
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(810 + i * 20, 430 + j * 30);
                glVertex2f(820 + i * 20, 430 + j * 30);
                glVertex2f(820 + i * 20, 440 + j * 30);
                glVertex2f(810 + i * 20, 440 + j * 30);
                glEnd();
            }
        }

    }

    void FBuilding10() {
        glBegin(GL_POLYGON);
        glColor3f(0.6f, 0.2f, 0.3f);
        glVertex2f(900, 420);
        glVertex2f(1000, 420);
        glVertex2f(1000, 480);
        glVertex2f(900, 480);
        glEnd();


        // Windows
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 2; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(905 + i * 15, 430 + j * 20);
                glVertex2f(915 + i * 15, 430 + j * 20);
                glVertex2f(915 + i * 15, 440 + j * 20);
                glVertex2f(905 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuilding10Night() {
        glBegin(GL_POLYGON);
        glColor3f(0.4f, 0.f, 0.f);
        glVertex2f(900, 420);
        glVertex2f(1000, 420);
        glVertex2f(1000, 480);
        glVertex2f(900, 480);
        glEnd();

        // Windows
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 2; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(905 + i * 15, 430 + j * 20);
                glVertex2f(915 + i * 15, 430 + j * 20);
                glVertex2f(915 + i * 15, 440 + j * 20);
                glVertex2f(905 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuildingB1() {
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.7f, 0.7f);
        glVertex2f(250, 420);
        glVertex2f(350, 420);
        glVertex2f(350, 555);
        glVertex2f(250, 555);
        glEnd();

        // Windows
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(255 + i * 18, 430 + j * 20);
                glVertex2f(265 + i * 18, 430 + j * 20);
                glVertex2f(265 + i * 18, 440 + j * 20);
                glVertex2f(255 + i * 18, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuildingB2() {
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.7f, 0.7f);
        glVertex2f(580, 420);
        glVertex2f(680, 420);
        glVertex2f(680, 555);
        glVertex2f(580, 555);
        glEnd();

        // Windows
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(585 + i * 15, 430 + j * 20);
                glVertex2f(595 + i * 15, 430 + j * 20);
                glVertex2f(595 + i * 15, 440 + j * 20);
                glVertex2f(585 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }


    void FBuildingB3() {
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.7f, 0.7f);
        glVertex2f(720, 420);
        glVertex2f(820, 420);
        glVertex2f(820, 530);
        glVertex2f(720, 530);
        glEnd();

        // Windows
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 5; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(725 + i * 15, 430 + j * 20);
                glVertex2f(735 + i * 15, 430 + j * 20);
                glVertex2f(735 + i * 15, 440 + j * 20);
                glVertex2f(725 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuildingB4() {
        glBegin(GL_QUADS);
        glColor3f(0.2f, 0.7f, 0.7f);
        glVertex2f(950, 420);
        glVertex2f(1000, 420);
        glVertex2f(1000, 570);
        glVertex2f(950, 570);
        glEnd();


        // Windows
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 7; j++) {
                glBegin(GL_QUADS);
                glColor3f(0.75f, 0.75f, 0.75f);
                glVertex2f(955 + i * 15, 430 + j * 20);
                glVertex2f(965 + i * 15, 430 + j * 20);
                glVertex2f(965 + i * 15, 440 + j * 20);
                glVertex2f(955 + i * 15, 440 + j * 20);
                glEnd();
            }
        }
        glFlush();
    }

    void FBuildingB1Night() {
        glBegin(GL_QUADS);
        glColor3f(0.25f, 0.25f, 0.25f);
        glVertex2f(250, 420);
        glVertex2f(350, 420);
        glVertex2f(350, 555);
        glVertex2f(250, 555);
        glEnd();


        // Windows
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(255 + i * 18, 430 + j * 20);
                glVertex2f(265 + i * 18, 430 + j * 20);
                glVertex2f(265 + i * 18, 440 + j * 20);
                glVertex2f(255 + i * 18, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuildingB2Night() {
        glBegin(GL_QUADS);
        glColor3f(0.25f, 0.25f, 0.25f);
        glVertex2f(580, 420);
        glVertex2f(680, 420);
        glVertex2f(680, 555);
        glVertex2f(580, 555);
        glEnd();

        // Windows
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(585 + i * 15, 430 + j * 20);
                glVertex2f(595 + i * 15, 430 + j * 20);
                glVertex2f(595 + i * 15, 440 + j * 20);
                glVertex2f(585 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuildingB3Night() {
        glBegin(GL_QUADS);
        glColor3f(0.25f, 0.25f, 0.25f);
        glVertex2f(720, 420);
        glVertex2f(820, 420);
        glVertex2f(820, 530);
        glVertex2f(720, 530);
        glEnd();

        // Windows
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 5; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(725 + i * 15, 430 + j * 20);
                glVertex2f(735 + i * 15, 430 + j * 20);
                glVertex2f(735 + i * 15, 440 + j * 20);
                glVertex2f(725 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBuildingB4Night() {
        glBegin(GL_QUADS);
        glColor3f(0.25f, 0.25f, 0.25f);
        glVertex2f(950, 420);
        glVertex2f(1000, 420);
        glVertex2f(1000, 570);
        glVertex2f(950, 570);
        glEnd();

        // Windows
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 7; j++) {
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(955 + i * 15, 430 + j * 20);
                glVertex2f(965 + i * 15, 430 + j * 20);
                glVertex2f(965 + i * 15, 440 + j * 20);
                glVertex2f(955 + i * 15, 440 + j * 20);
                glEnd();
            }
        }

    }

    void FBridge() {
        int shift = 200;
        for (int i = 0; i < 5; i++) {
            // Bridge supports (pillars)
            glBegin(GL_QUADS);
            glColor3f(0.61f, 0.35f, 0.18f); // Brown  supports
            glVertex2f(0   + shift * i, 100);
            glVertex2f(10  + shift * i, 100);
            glVertex2f(10  + shift * i, 200);
            glVertex2f(0   + shift * i, 200);
            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.61f, 0.35f, 0.18f); // Brown  supports
            glVertex2f(190   + shift * i, 100);
            glVertex2f(200  + shift * i, 100);
            glVertex2f(200  + shift * i, 200);
            glVertex2f(190   + shift * i, 200);
            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.61f, 0.35f, 0.18f); // Brown supports
            glVertex2f(10   + shift * i, 175);
            glVertex2f(190  + shift * i, 175);
            glVertex2f(190  + shift * i, 185);
            glVertex2f(10   + shift * i, 185);
            glEnd();

            // Bridge planks
            glBegin(GL_QUADS);
            glColor3f(0.7f, 0.7f, 0.7f); // Gray  planks
            glVertex2f(0 + shift * i, 200);
            glVertex2f(200 + shift * i, 200);
            glVertex2f(200 + shift * i, 220);
            glVertex2f(0 + shift * i, 220);
            glEnd();
        }


    }

    void Frailline()
    {
        glColor3f(0.803, 0.690, 0.675);
        glBegin(GL_QUADS);
        glVertex2f(0, 185);
        glVertex2f(1000,185);
        glVertex2f(1000,230);
        glVertex2f(0, 230);

        glEnd();
        glBegin(GL_QUADS);//down black railline
        glColor3f(0, 0, 0);
        glVertex2f(0, 190);
        glVertex2f(1000, 190);
        glVertex2f(1000, 195);
        glVertex2f(0, 195);
        glEnd();

        glBegin(GL_QUADS);//upper black railline
        glColor3f(0, 0, 0);
        glVertex2f(0, 220);
        glVertex2f(1000, 220);
        glVertex2f(1000, 225);
        glVertex2f(0, 225);
        glEnd();

        glColor3f(0.3, 0.3, 0.3); // Dark gray  rail line
        glBegin(GL_QUADS);
        for (int i =0 ; i <= 1000; i = i + 30) {
            glVertex2f(i, 190);        // Bottom left corner of the rail
            glVertex2f(i + 5, 190);    // Bottom right corner of the rail
            glVertex2f(i + 20, 225);   // Top right corner of the rail
            glVertex2f(i + 25, 225);   // Top left corner of the rail
        }
        glEnd();

    }

    void Ftree() {
        int shift = 200;
        for (int i = 0; i < 5; i++) {
            // Tree trunk
            glBegin(GL_QUADS);
            glColor3f(0.61, 0.35, 0.18);
            glVertex2f(100  + shift * i, 300);
            glVertex2f(110  + shift * i, 300);
            glVertex2f(110  + shift * i, 370);
            glVertex2f(100  + shift * i, 370);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(0.04, 0.36, 0.12);
            glVertex2f(70  + shift * i, 370);
            glVertex2f(140  + shift * i, 370);
            glVertex2f(105  + shift * i, 420);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(0.09, 0.8, 0.2);
            glVertex2f(75  + shift * i, 390);
            glVertex2f(135  + shift * i, 390);
            glVertex2f(105  + shift * i, 430);
            glEnd();
        }

    }

    void FtreeNight() {
        int shift = 200;
        for (int i = 0; i < 5; i++) {
            // Tree trunk
            glBegin(GL_QUADS);
            glColor3f(0.61, 0.35, 0.18);
            glVertex2f(100  + shift * i, 300);
            glVertex2f(110  + shift * i, 300);
            glVertex2f(110  + shift * i, 370);
            glVertex2f(100  + shift * i, 370);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(0.0, 0.2, 0.0);
            glVertex2f(70  + shift * i, 370);
            glVertex2f(140  + shift * i, 370);
            glVertex2f(105  + shift * i, 420);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(0.2, 0.4, 0.0);
            glVertex2f(75  + shift * i, 390);
            glVertex2f(135  + shift * i, 390);
            glVertex2f(105  + shift * i, 430);
            glEnd();
        }

    }

    void FtreeUp() {
        int shift = 200;
        for (int i = 0; i < 5; i++) {
            // Tree trunk
            glBegin(GL_QUADS);
            glColor3f(0.61, 0.35, 0.18);
            glVertex2f(50  + shift * i, 400);
            glVertex2f(55  + shift * i, 400);
            glVertex2f(55  + shift * i, 440);
            glVertex2f(50  + shift * i, 440);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(0.04, 0.36, 0.12);
            glVertex2f(35   + shift * i, 440);
            glVertex2f(70  + shift * i, 440);
            glVertex2f(52.5  + shift * i, 480);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(0.09, 0.8, 0.2);
            glVertex2f(38   + shift * i, 455);
            glVertex2f(68  + shift * i, 455);
            glVertex2f(52.5  + shift * i, 485);
            glEnd();
        }

    }

    void FtreeUpNight() {
        int shift = 200;
        for (int i = 0; i < 5; i++) {
            // Tree trunk
            glBegin(GL_QUADS);
            glColor3f(0.61, 0.35, 0.18);
            glVertex2f(50  + shift * i, 400);
            glVertex2f(55  + shift * i, 400);
            glVertex2f(55  + shift * i, 440);
            glVertex2f(50  + shift * i, 440);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(0.0, 0.2, 0.0);
            glVertex2f(35   + shift * i, 440);
            glVertex2f(70  + shift * i, 440);
            glVertex2f(52.5  + shift * i, 480);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(0.2, 0.4, 0.0);
            glVertex2f(38   + shift * i, 455);
            glVertex2f(68  + shift * i, 455);
            glVertex2f(52.5  + shift * i, 485);
            glEnd();
        }

    }

    int Birdx =10.0f;
    void FupdateBird(int value) {
        Birdx += 2;
        if (Birdx > 1000) {
            Birdx = -300;
        }
        glutPostRedisplay();
    }

    void FBird() {

        glPushMatrix();
        glTranslatef(Birdx, 0.0f, 0.0f);

        int shift = 40;
        for (int i = 0; i < 5; i++) {
            // Left wing
            glBegin(GL_TRIANGLES);
            glColor3f(0, 0, 0);
            glVertex2f(80 + shift * i, 570);
            glVertex2f(95 + shift * i, 555);
            glVertex2f(100 + shift * i, 555);
            glEnd();

            // Right wing
            glBegin(GL_TRIANGLES);
            glColor3f(0, 0, 0);
            glVertex2f(110 + shift * i, 570);
            glVertex2f(95 + shift * i, 555);
            glVertex2f(100 + shift * i, 555);
            glEnd();
        }
        glPopMatrix();

    }

    float busX = 0.0f;
    float car1X = 0.0f;
    float car2X = 0.0f;
    float car3X = 0.0f;
    float truckX = 0.0f;
    float truck2X = 0.0f;
    float boatX = 0.0f;
    float boatY = 0.0f;
    float cloudX = 0.0f;
    float vehicleSpeed = 1.0f;
    float FTrainX=-900.0f;
    float sTrainSpeed=2.0f;


    void FupdateCloud(int value) {
        cloudX += 0.5f;
        if (cloudX > 1300) cloudX = -850;
        glutPostRedisplay();
    }

    void FCloud(int centerX, int centerY, int radius) {
        glPushMatrix();
        glTranslatef(cloudX, 0.0f, 0.0f);

        glColor3f(.75f, .75f, .75f);
        Fcircle(centerX, centerY, radius); // center circle
        Fcircle(centerX + radius, centerY, radius); // Right circle
        Fcircle(centerX - radius, centerY, radius); // Left circle
        Fcircle(centerX, centerY + radius, radius); // Top circle

        glPopMatrix();
    }

    void FCloudNight(int centerX, int centerY, int radius) {
        glPushMatrix();
        glTranslatef(cloudX, 0.0f, 0.0f);

        glColor3f(.85f, .85f, .85f);
        Fcircle(centerX, centerY, radius); // center circle
        Fcircle(centerX + radius, centerY, radius); // Right circle
        Fcircle(centerX - radius, centerY, radius); // Left circle
        Fcircle(centerX, centerY + radius, radius); // Top circle

        glPopMatrix();
    }

    void FupdateVehicles(int value) {
        // Move right
        busX += vehicleSpeed;
        if (busX > 1050) busX = -180;

        car1X += vehicleSpeed;
        if (car1X > 900) car1X = -340;

        car2X += vehicleSpeed;
        if (car2X > 680) car2X = -540;

        // Move left
        car3X -= vehicleSpeed;
        if (car3X < -180) car3X = 1000;

        truckX += vehicleSpeed;
        if (truckX > 300) truckX = -900;

        truck2X -= vehicleSpeed;
        if (truck2X < -620) truck2X = 550;

        glutPostRedisplay();
    }

    void FBus() {
        glPushMatrix();
        glTranslatef(busX, 0.0f, 0.0f);

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.5f, 0.1f);
        glVertex2f(80, 380);
        glVertex2f(180, 380);
        glVertex2f(180, 400);
        glVertex2f(160, 420);
        glVertex2f(80, 420);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.25f, 0.25f, 0.25f);
        glVertex2f(90, 400);
        glVertex2f(160, 400);
        glVertex2f(160, 410);
        glVertex2f(90, 410);
        glEnd();

        glColor3f(0.25f, 0.25f, 0.25f);
        Fcircle(105, 380, 7);
        Fcircle(155, 380, 7);

        glPopMatrix();
    }

    void FBusnight() {
        glPushMatrix();
        glTranslatef(busX, 0.0f, 0.0f);

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.5f, 0.1f);
        glVertex2f(80, 380);
        glVertex2f(180, 380);
        glVertex2f(180, 400);
        glVertex2f(160, 420);
        glVertex2f(80, 420);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(90, 400);
        glVertex2f(160, 400);
        glVertex2f(160, 410);
        glVertex2f(90, 410);
        glEnd();

        glColor3f(0.25f, 0.25f, 0.25f);
        Fcircle(105, 380, 7);
        Fcircle(155, 380, 7);

        glPopMatrix();
    }

    void FCar1() {
        glPushMatrix();
        glTranslatef(car1X, 0.0f, 0.0f);

        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(300, 380);
        glVertex2f(360, 380);
        glVertex2f(360, 395);
        glVertex2f(300, 395);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(310, 395);
        glVertex2f(350, 395);
        glVertex2f(340, 405);
        glVertex2f(315, 405);
        glEnd();

        glColor3f(0.5f, 0.0f, 0.0f);
        Fcircle(314, 380, 5);
        Fcircle(345, 380, 5);

        glPopMatrix();
    }

    void FCarNight() {
        glPushMatrix();
        glTranslatef(car1X, 0.0f, 0.0f);

        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.4f, 0.0f);
        glVertex2f(300, 380);
        glVertex2f(360, 380);
        glVertex2f(360, 395);
        glVertex2f(300, 395);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(310, 395);
        glVertex2f(350, 395);
        glVertex2f(340, 405);
        glVertex2f(315, 405);
        glEnd();

        glColor3f(0.5f, 0.0f, 0.0f);
        Fcircle(314, 380, 5);
        Fcircle(345, 380, 5);

        glPopMatrix();
    }

    void FCar2() {
        glPushMatrix();
        glTranslatef(car2X, 0.0f, 0.0f);

        glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(500, 380);
        glVertex2f(560, 380);
        glVertex2f(560, 395);
        glVertex2f(500, 395);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.25f, .25f, .25f);
        glVertex2f(510, 395);
        glVertex2f(550, 395);
        glVertex2f(540, 405);
        glVertex2f(515, 405);
        glEnd();

        glColor3f(0.5f, 0.0f, 0.0f);
        Fcircle(514, 380, 5);
        Fcircle(545, 380, 5);

        glPopMatrix();
    }

    void FCar3() {
        glPushMatrix();
        glTranslatef(car3X, 0.0f, 0.0f);

        glBegin(GL_QUADS);
        glColor3f(.0f, 0.0f, 1.0f);
        glVertex2f(100, 330);
        glVertex2f(180, 330);
        glVertex2f(180, 355);
        glVertex2f(100, 355);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.0f, 4.0f, 6.0f);
        glVertex2f(110, 355);
        glVertex2f(170, 355);
        glVertex2f(165, 372);
        glVertex2f(120, 372);
        glEnd();

        glColor3f(0.5f, 0.0f, 0.0f);
        Fcircle(120, 330, 6);
        Fcircle(160, 330, 6);

        glPopMatrix();
    }

    void FTruck() {
        glPushMatrix();
        glTranslatef(truckX, 0.0f, 0.0f);

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.5f, 0.1f);
        glVertex2f(802, 380);
        glVertex2f(830, 380);
        glVertex2f(830, 400);
        glVertex2f(820, 420);
        glVertex2f(802, 420);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.2f, 0.3f);
        glVertex2f(700, 380);
        glVertex2f(800, 380);
        glVertex2f(800, 420);
        glVertex2f(700, 420);
        glEnd();

        glColor3f(0.25f, 0.25f, 0.25f);
        Fcircle(815, 380, 7);
        Fcircle(720, 380, 7);
        Fcircle(775, 380, 7);

        glPopMatrix();
    }

    void FTruckNight() {
        glPushMatrix();
        glTranslatef(truck2X, 0.0f, 0.0f);

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.5f, 0.1f);
        glVertex2f(498, 330);
        glVertex2f(460, 330);
        glVertex2f(460, 370);
        glVertex2f(475, 390);
        glVertex2f(498, 390);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.4f, 0.3f, 0.5f);
        glVertex2f(500, 330);
        glVertex2f(620, 330);
        glVertex2f(620, 390);
        glVertex2f(500, 390);
        glEnd();

        glColor3f(0.25f, 0.25f, 0.25f);
        Fcircle(480, 330, 7);
        Fcircle(530, 330, 7);
        Fcircle(590, 330, 7);

        glPopMatrix();
    }

    void FTruck2() {
        glPushMatrix();
        glTranslatef(truck2X, 0.0f, 0.0f);

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.5f, 0.1f);
        glVertex2f(498, 330);
        glVertex2f(460, 330);
        glVertex2f(460, 370);
        glVertex2f(475, 390);
        glVertex2f(498, 390);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.2f, 0.3f);
        glVertex2f(500, 330);
        glVertex2f(620, 330);
        glVertex2f(620, 390);
        glVertex2f(500, 390);
        glEnd();

        glColor3f(0.25f, 0.25f, 0.25f);
        Fcircle(480, 330, 7);
        Fcircle(530, 330, 7);
        Fcircle(590, 330, 7);

        glPopMatrix();
    }

    void FupdateBoat(int value) {
        boatX += 1.0f;
        if (boatX > 1250) boatX = -650;
        glutPostRedisplay();
    }
    float moveUp=0.0f;

    void FBoat() {
        glPushMatrix();
        glTranslatef(boatX, moveUp, 0.0f);

        glBegin(GL_POLYGON);
        glColor3f(0.5f, 0.2f, 0.0f);
        glVertex2f(400, 50);
        glVertex2f(600, 50);
        glVertex2f(550, 100);
        glVertex2f(450, 100);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(250, 50);
        glVertex2f(650, 50);
        glVertex2f(600, 20);
        glVertex2f(300, 20);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(470, 100);
        glVertex2f(530, 100);
        glVertex2f(530, 125);
        glVertex2f(470, 125);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.6f, 0.6f, 0.6f);
        glVertex2f(470, 125);
        glVertex2f(530, 125);
        glVertex2f(500, 155);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.3f, 0.3f, 0.3f);
        glVertex2f(490, 125);
        glVertex2f(500, 125);
        glVertex2f(500, 160);
        glVertex2f(490, 160);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.7f, 0.9f, 1.0f);
        glVertex2f(480, 110);
        glVertex2f(490, 110);
        glVertex2f(490, 120);
        glVertex2f(480, 120);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.7f, 0.9f, 1.0f);
        glVertex2f(510, 110);
        glVertex2f(520, 110);
        glVertex2f(520, 120);
        glVertex2f(510, 120);
        glEnd();

        glPopMatrix();
    }

    void FupdateBoatNight(int value) {
        boatY -= 1.0f;
        if (boatY < -650) boatY = 1200;

        glutPostRedisplay();
    }

    void FBoatNight() {
        glPushMatrix();
        glTranslatef(boatY, moveUp, 0.0f);

        glBegin(GL_POLYGON);
        glColor3f(0.5f, 0.2f, 0.0f);
        glVertex2f(400, 250);
        glVertex2f(600, 250);
        glVertex2f(550, 300);
        glVertex2f(450, 300);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(250, 250);
        glVertex2f(650, 250);
        glVertex2f(600, 220);
        glVertex2f(300, 220);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(470, 300);
        glVertex2f(530, 300);
        glVertex2f(530, 325);
        glVertex2f(470, 325);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.6f, 0.6f, 0.6f);
        glVertex2f(470, 325);
        glVertex2f(530, 325);
        glVertex2f(500, 355);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.3f, 0.3f, 0.3f);
        glVertex2f(490, 325);
        glVertex2f(500, 325);
        glVertex2f(500, 360);
        glVertex2f(490, 360);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.f, 1.f, 0.0f);
        glVertex2f(480, 310);
        glVertex2f(490, 310);
        glVertex2f(490, 320);
        glVertex2f(480, 320);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(1.f, 1.f, 0.0f);
        glVertex2f(510, 310);
        glVertex2f(520, 310);
        glVertex2f(520, 320);
        glVertex2f(510, 320);
        glEnd();

        glPopMatrix();
    }

    void FSun() {
        int centerX = 228;
        int centerY = 550;
        int radius = 20;

        glColor3f(1.0f, 0.5f, 0.0f); // Orange sun
        Fcircle(centerX, centerY, radius);
    }

    void FMoon() {
        int centerX = 730;
        int centerY = 570;
        int radius = 20;

        glColor3f(1.0f, 1.0f, 1.0f); // white moon
        Fcircle(centerX, centerY, radius);
    }



    void Ftrain(){

        //left first
        glPushMatrix(); // Save the current transformation matrix
        glTranslatef(FTrainX, 0, 0.0f);
        glColor3f(0.0, 0.2,0.7);
        glBegin(GL_QUADS);
        glVertex2f(0,200);
        glVertex2f(200,200);
        glVertex2f(200,300);
        glVertex2f(0,300);
        glEnd();
        //design
        glColor3f(1.0, 0.6, 0.2);
        glBegin(GL_QUADS);
        glVertex2f(0,210);
        glVertex2f(200,210);
        glVertex2f(200,220);
        glVertex2f(0,220);
        glEnd();

        glColor3f(0.0,0.0,0.1);
        glBegin(GL_QUADS);
        glVertex2f(0,300);
        glVertex2f(200,300);
        glVertex2f(220,320);
        glVertex2f(30,320);
        glEnd();

        glColor3f(0.0,0.0,0.2);
        glBegin(GL_QUADS);
        glVertex2f(200,300);
        glVertex2f(220,320);
        glVertex2f(220,230);
        glVertex2f(200,200);
        glEnd();


            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(10,250);
        glVertex2f(40,250);
        glVertex2f(40,280);
        glVertex2f(10,280);
        glEnd();
        //2nd
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(50,250);
        glVertex2f(80,250);
        glVertex2f(80,280);
        glVertex2f(50,280);
        glEnd();
        //3rd
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(90,250);
        glVertex2f(120,250);
        glVertex2f(120,280);
        glVertex2f(90,280);
        glEnd();

        //1st door
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            // glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(130,220);
        glVertex2f(165,220);
        glVertex2f(165,280);
        glVertex2f(130,280);
        glEnd();



        //left second
        glColor3f(0.0, 0.2, 0.7);
        glBegin(GL_QUADS);
        glVertex2f(220,200);
        glVertex2f(420,200);
        glVertex2f(420,300);
        glVertex2f(220,300);
        glEnd();

        glColor3f(1.0, 0.6, 0.2);
        glBegin(GL_QUADS);
        glVertex2f(220,210);
        glVertex2f(420,210);
        glVertex2f(420,220);
        glVertex2f(220,220);
        glEnd();

        glColor3f(0.0,0.0,0.1);
        glBegin(GL_QUADS);
        glVertex2f(220,300);
        glVertex2f(420,300);
        glVertex2f(440,320);
        glVertex2f(250,320);
        glEnd();

        glColor3f(0.0,0.0,0.2);
        glBegin(GL_QUADS);
        glVertex2f(420,300);
        glVertex2f(440,320);
        glVertex2f(440,230);
        glVertex2f(420,200);
        glEnd();


        //1st window
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            // glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(230,250);
        glVertex2f(260,250);
        glVertex2f(260,280);
        glVertex2f(230,280);
        glEnd();
        //2nd
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            // glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(270,250);
        glVertex2f(300,250);
        glVertex2f(300,280);
        glVertex2f(270,280);
        glEnd();
        //3rd
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(310,250);
        glVertex2f(340,250);
        glVertex2f(340,280);
        glVertex2f(310,280);
        glEnd();

        // door
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(355,220);
        glVertex2f(390,220);
        glVertex2f(390,280);
        glVertex2f(355,280);
        glEnd();


        //left 3rd
        glColor3f(0.0, 0.2, 0.7);
        glBegin(GL_QUADS);
        glVertex2f(440,200);
        glVertex2f(640,200);
        glVertex2f(640,300);
        glVertex2f(440,300);
        glEnd();

        glColor3f(1.0, 0.6, 0.2);
        glBegin(GL_QUADS);
        glVertex2f(440,210);
        glVertex2f(640,210);
        glVertex2f(640,220);
        glVertex2f(440,220);
        glEnd();


        glColor3f(0.0,0.0,0.1);
        glBegin(GL_QUADS);
        glVertex2f(440,300);
        glVertex2f(640,300);
        glVertex2f(660,320);
        glVertex2f(470,320);
        glEnd();

        glColor3f(0.0,0.0,0.2);
        glBegin(GL_QUADS);
        glVertex2f(640,300);
        glVertex2f(660,320);
        glVertex2f(660,230);
        glVertex2f(640,200);
        glEnd();


        //1st window
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(450,250);
        glVertex2f(480,250);
        glVertex2f(480,280);
        glVertex2f(450,280);
        glEnd();
        //2nd
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            // glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(490,250);
        glVertex2f(520,250);
        glVertex2f(520,280);
        glVertex2f(490,280);
        glEnd();
        //3rd
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(530,250);
        glVertex2f(560,250);
        glVertex2f(560,280);
        glVertex2f(530,280);
        glEnd();

        // door
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(580,220);
        glVertex2f(615,220);
        glVertex2f(615,280);
        glVertex2f(580,280);
        glEnd();


        //head eng
        glColor3f(0.0,0.2,0.7);
        glBegin(GL_POLYGON);
        glVertex2f(660,200);
        glVertex2f(860,200);
        glVertex2f(860,250);
        glVertex2f(760,300);
        glVertex2f(660,300);
        glEnd();

        glColor3f(1.0, 0.6, 0.2);
        glBegin(GL_QUADS);
        glVertex2f(660,210);
        glVertex2f(860,210);
        glVertex2f(860,220);
        glVertex2f(660,220);
        glEnd();


        glColor3f(0.0,0.0,0.1);
        glBegin(GL_POLYGON);
        glVertex2f(660,300);
        glVertex2f(760,300);
        glVertex2f(790,320);
        glVertex2f(690,320);
        glEnd();

        glColor3f(0.0,0.6,0.7);
        glBegin(GL_POLYGON);
        glVertex2f(760,300);
        glVertex2f(790,320);
        glVertex2f(880,270);
        glVertex2f(860,250);
        glEnd();

        glColor3f(0.7,0.0,0.0);
        glBegin(GL_POLYGON);
        glVertex2f(860,200);
        glVertex2f(860,250);
        glVertex2f(880,270);
        glVertex2f(880,225);
        glEnd();

        // door
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(680,220);
        glVertex2f(720,220);
        glVertex2f(720,280);
        glVertex2f(680,280);
        glEnd();

        //window
            glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            //glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(730,250);
        glVertex2f(760,250);
        glVertex2f(760,280);
        glVertex2f(730,280);
        glEnd();

        //chaka
        glColor3f(0.0, 0.0, 0.0);
        Fcircle(10, 200, 10);
        Fcircle(60, 200, 10);
        Fcircle(110, 200,10);
        Fcircle( 170, 200,10);

        Fcircle( 250, 200,10);
        Fcircle( 300, 200,10);
        Fcircle( 350, 200,10);
        Fcircle(400, 200,10);


        Fcircle( 460, 200,10);
        Fcircle( 510, 200,10);
        Fcircle( 560, 200,10);
        Fcircle(620, 200,10);

        Fcircle( 680, 200,10);
        Fcircle( 730, 200,10);
        Fcircle(780, 200,10);
        Fcircle(830, 200,10);

        glPopMatrix(); // Restore the transformation matrix


            // Update the boat's position for movement
            FTrainX += sTrainSpeed;
            if (FTrainX > 1200.0f) FTrainX = -1080.0f;



    }





    void FtrainNight(){

        //left first
        glPushMatrix(); // Save the current transformation matrix
        glTranslatef(FTrainX, 0, 0.0f);
        glColor3f(0.0, 0.2,0.7);
        glBegin(GL_QUADS);
        glVertex2f(0,200);
        glVertex2f(200,200);
        glVertex2f(200,300);
        glVertex2f(0,300);
        glEnd();
        //design
        glColor3f(1.0, 0.6, 0.2);
        glBegin(GL_QUADS);
        glVertex2f(0,210);
        glVertex2f(200,210);
        glVertex2f(200,220);
        glVertex2f(0,220);
        glEnd();

        glColor3f(0.0,0.0,0.1);
        glBegin(GL_QUADS);
        glVertex2f(0,300);
        glVertex2f(200,300);
        glVertex2f(220,320);
        glVertex2f(30,320);
        glEnd();

        glColor3f(0.0,0.0,0.2);
        glBegin(GL_QUADS);
        glVertex2f(200,300);
        glVertex2f(220,320);
        glVertex2f(220,230);
        glVertex2f(200,200);
        glEnd();


            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(10,250);
        glVertex2f(40,250);
        glVertex2f(40,280);
        glVertex2f(10,280);
        glEnd();
        //2nd
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(50,250);
        glVertex2f(80,250);
        glVertex2f(80,280);
        glVertex2f(50,280);
        glEnd();
        //3rd
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(90,250);
        glVertex2f(120,250);
        glVertex2f(120,280);
        glVertex2f(90,280);
        glEnd();

        //1st door
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(130,220);
        glVertex2f(165,220);
        glVertex2f(165,280);
        glVertex2f(130,280);
        glEnd();



        //left second
        glColor3f(0.0, 0.2, 0.7);
        glBegin(GL_QUADS);
        glVertex2f(220,200);
        glVertex2f(420,200);
        glVertex2f(420,300);
        glVertex2f(220,300);
        glEnd();

        glColor3f(1.0, 0.6, 0.2);
        glBegin(GL_QUADS);
        glVertex2f(220,210);
        glVertex2f(420,210);
        glVertex2f(420,220);
        glVertex2f(220,220);
        glEnd();

        glColor3f(0.0,0.0,0.1);
        glBegin(GL_QUADS);
        glVertex2f(220,300);
        glVertex2f(420,300);
        glVertex2f(440,320);
        glVertex2f(250,320);
        glEnd();

        glColor3f(0.0,0.0,0.2);
        glBegin(GL_QUADS);
        glVertex2f(420,300);
        glVertex2f(440,320);
        glVertex2f(440,230);
        glVertex2f(420,200);
        glEnd();


        //1st window
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(230,250);
        glVertex2f(260,250);
        glVertex2f(260,280);
        glVertex2f(230,280);
        glEnd();
        //2nd
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(270,250);
        glVertex2f(300,250);
        glVertex2f(300,280);
        glVertex2f(270,280);
        glEnd();
        //3rd
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(310,250);
        glVertex2f(340,250);
        glVertex2f(340,280);
        glVertex2f(310,280);
        glEnd();

        // door
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(355,220);
        glVertex2f(390,220);
        glVertex2f(390,280);
        glVertex2f(355,280);
        glEnd();


        //left 3rd
        glColor3f(0.0, 0.2, 0.7);
        glBegin(GL_QUADS);
        glVertex2f(440,200);
        glVertex2f(640,200);
        glVertex2f(640,300);
        glVertex2f(440,300);
        glEnd();

        glColor3f(1.0, 0.6, 0.2);
        glBegin(GL_QUADS);
        glVertex2f(440,210);
        glVertex2f(640,210);
        glVertex2f(640,220);
        glVertex2f(440,220);
        glEnd();


        glColor3f(0.0,0.0,0.1);
        glBegin(GL_QUADS);
        glVertex2f(440,300);
        glVertex2f(640,300);
        glVertex2f(660,320);
        glVertex2f(470,320);
        glEnd();

        glColor3f(0.0,0.0,0.2);
        glBegin(GL_QUADS);
        glVertex2f(640,300);
        glVertex2f(660,320);
        glVertex2f(660,230);
        glVertex2f(640,200);
        glEnd();


        //1st window
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(450,250);
        glVertex2f(480,250);
        glVertex2f(480,280);
        glVertex2f(450,280);
        glEnd();
        //2nd
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(490,250);
        glVertex2f(520,250);
        glVertex2f(520,280);
        glVertex2f(490,280);
        glEnd();
        //3rd
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(530,250);
        glVertex2f(560,250);
        glVertex2f(560,280);
        glVertex2f(530,280);
        glEnd();

        // door
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(580,220);
        glVertex2f(615,220);
        glVertex2f(615,280);
        glVertex2f(580,280);
        glEnd();


        //head eng
        glColor3f(0.0,0.2,0.7);
        glBegin(GL_POLYGON);
        glVertex2f(660,200);
        glVertex2f(860,200);
        glVertex2f(860,250);
        glVertex2f(760,300);
        glVertex2f(660,300);
        glEnd();

        glColor3f(1.0, 0.6, 0.2);
        glBegin(GL_QUADS);
        glVertex2f(660,210);
        glVertex2f(860,210);
        glVertex2f(860,220);
        glVertex2f(660,220);
        glEnd();


        glColor3f(0.0,0.0,0.1);
        glBegin(GL_POLYGON);
        glVertex2f(660,300);
        glVertex2f(760,300);
        glVertex2f(790,320);
        glVertex2f(690,320);
        glEnd();

        glColor3f(0.0,0.6,0.7);
        glBegin(GL_POLYGON);
        glVertex2f(760,300);
        glVertex2f(790,320);
        glVertex2f(880,270);
        glVertex2f(860,250);
        glEnd();

        glColor3f(0.7,0.0,0.0);
        glBegin(GL_POLYGON);
        glVertex2f(860,200);
        glVertex2f(860,250);
        glVertex2f(880,270);
        glVertex2f(880,225);
        glEnd();

        // door
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(680,220);
        glVertex2f(720,220);
        glVertex2f(720,280);
        glVertex2f(680,280);
        glEnd();

        //window
            //glColor3f(1.0f, 1.0f, 1.0f);  // Sky blue (Day)

            glColor3f(1.0f, 0.9f, 0.55f);   // Warm yellow glow (Night)

        glBegin(GL_QUADS);
        glVertex2f(730,250);
        glVertex2f(760,250);
        glVertex2f(760,280);
        glVertex2f(730,280);
        glEnd();

        //chaka
        glColor3f(0.0, 0.0, 0.0);
        Fcircle(10, 200, 10);
        Fcircle(60, 200, 10);
        Fcircle(110, 200,10);
        Fcircle( 170, 200,10);

        Fcircle( 250, 200,10);
        Fcircle( 300, 200,10);
        Fcircle( 350, 200,10);
        Fcircle(400, 200,10);


        Fcircle( 460, 200,10);
        Fcircle( 510, 200,10);
        Fcircle( 560, 200,10);
        Fcircle(620, 200,10);

        Fcircle( 680, 200,10);
        Fcircle( 730, 200,10);
        Fcircle(780, 200,10);
        Fcircle(830, 200,10);

        glPopMatrix(); // Restore the transformation matrix


            // Update the boat's position for movement
            FTrainX += sTrainSpeed;
            if (FTrainX > 1200.0f) FTrainX = -1080.0f;



    }




    // ...  ...


    void FDay()
    {
        glClear(GL_COLOR_BUFFER_BIT);  // Just clear the buffer
        // glLoadIdentity();  // Remove this since we want to preserve transformations

        FRiver();
        FRiverBank();
        FRoad();
        FSidewalk();
        FSky();
        FSun();

        // clouds
        FCloud(200, 560, 8);
        FCloud(800, 530, 17);
        FCloud(400, 540, 13);
        FCloud(600, 540, 11);

        FBird();

        //background buildings
        FBuildingB1();
        FBuildingB2();
        FBuildingB3();
        FBuildingB4();

        //front buildings
        FBuilding1();
        FBuilding2();
        FBuilding3();
        FBuilding4();
        FBuilding5();
        FBuilding6();
        FBuilding7();
        FBuilding8();
        FBuilding9();
        FBuilding10();
        FtreeUp();
        FBus();
        FCar1();
        FCar2();
        FCar3();
        FTruck();
        FTruck2();
        Ftree();
        FBridge();
        Frailline();
        FBoat();
        Ftrain();

    }

    void FNight()
    {
        glClear(GL_COLOR_BUFFER_BIT);  // Just clear the buffer
        // glLoadIdentity();  // Remove this since we want to preserve transformations

        FRiverNight();
        FRiverBank();
        FRoad();
        FSidewalk();
        FSkyNight();
        FMoon();

        // clouds
        FCloudNight(100, 560, 8);
        FCloudNight(800, 530, 17);
        FCloudNight(400, 540, 13);
    // Bird();

        //background buildings
        FBuildingB1Night();
        FBuildingB2Night();
        FBuildingB3Night();
        FBuildingB4Night();

        //front buildings
        FBuilding1Night();
        FBuilding2Night();
        FBuilding3Night();
        FBuilding4Night();
        FBuilding5Night();
        FBuilding6Night();
        FBuilding7Night();
        FBuilding8Night();
        FBuilding9Night();
        FBuilding10Night();
        FtreeUpNight();
        FCarNight();
        FBusnight();
        FTruckNight();
        FtreeNight();
        FBoatNight();
        FBridge();
        Frailline();
        FtrainNight();

    }

    void display() {
        if (FisDay) {
            FDay();  // Call the day setup
        } else {
            FNight();  // Call the night setup
        }
        glutSwapBuffers();  // Swap buffers to update the display
    }


    void keyboard(unsigned char key, int x, int y) {
        switch (key) {
            case 27:     // ESC key
                exit(0);
                break;
            case 'n':
            case 'N':
                FisDay = false;
                glClearColor(0.0f, 0.0f, 0.2f, 1.0f);  // Dark blue for night mode
                break;
            case 'd':
            case 'D':
                FisDay = true;
                glClearColor(0.52f, 0.8f, 0.92f, 1.0f);  // Sky blue for day mode
                break;
        }
    }

    void specialKeyboard(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_UP: // Up arrow key
                moveUp += 5.0f; // Move the boat up
                if (moveUp>50.0f)
                    moveUp=0.0f;
                break;
            case GLUT_KEY_DOWN: // Down arrow key
                moveUp -= 5.0f; // Move the boat down
                if (moveUp<-70.0f)
                    moveUp=0.0f;
                break;
        }
    }

    void mouse(int button, int state, int x, int y) {
        if (state == GLUT_DOWN) {
            if (button == GLUT_LEFT_BUTTON) {
                vehicleSpeed += 0.5f;
                if (vehicleSpeed > 10.0f)
                    vehicleSpeed = 1.0f;
            } else if (button == GLUT_RIGHT_BUTTON) {
                vehicleSpeed -= 0.5f;
                if (vehicleSpeed < 0.0f) vehicleSpeed = 0.0f;
            }
        }
    }

    void updateScene() {
        
        glutPostRedisplay();

        FupdateBird(0);
        FupdateVehicles(0);
        FupdateBoat(0);
        FupdateBoatNight(0);
        FupdateCloud(0);
    }

    bool hasInit = false;
    
    void initScene() {
        if (hasInit) return;

        FisDay=true;
        busX = 0.0f;
        car1X = 0.0f;
        car2X = 0.0f;
        car3X = 0.0f;
        truckX = 0.0f;
        truck2X = 0.0f;
        boatX = 0.0f;
        boatY = 0.0f;
        cloudX = 0.0f;
        vehicleSpeed = 1.0f;
        FTrainX=-900.0f;
        sTrainSpeed=2.0f;
        moveUp = 0.0f;
        hasInit = true;
        
        // Initialize any Masud scene specific resources here
        glClearColor(0.52f, 0.8f, 0.92f, 1.0f);  // Sky blue for day mode
        
        std::cout << "Masud's scene initialized." << std::endl;

        cout<< "--- Controls ---" <<endl;
        cout<<"For night press : n"<<endl;
        cout<<"For day press   : d"<<endl;
        cout<<endl;
        cout<<"To move the boat up press   : up button"<<endl;
        cout<<"To move the boat down press : down button"<<endl;
        cout<<endl;
        cout<<"To close the animation press : Esc button"<<endl;
    }
    
    void cleanupScene() {
        hasInit = false;
        std::cout << "Masud's scene cleaned up." << std::endl;
    }
}
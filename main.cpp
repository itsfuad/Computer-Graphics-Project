#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm> // For std::max

// Define PI if not available in cmath
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// ===================================================================
//  Helper Structs
// ===================================================================

// A simple struct to hold RGB color values
struct Color {
    float r, g, b;
};

// ===================================================================
//  Man Class Definition (with inline methods)
// ===================================================================

// Redesigned hairstyles for better visuals and 4-direction visibility
enum class HairStyle {
    Normal,
    Spiky,
    FlatTop,
    Ponytail
};

class Man {
private:
    // --- Member Variables (State for each Man) ---
    float x, y;          // Position
    float walkCycle;
    bool isWalking;
    float speed;

    // Style properties
    Color shirtColor;
    Color pantsColor;
    Color hairColor;
    Color skinColor;
    HairStyle hairStyle;

    // --- Private Helper Drawing Functions ---
    static void drawCircle(float cx, float cy, float radius, int segments = 20) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * M_PI * i / segments;
            glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
        }
        glEnd();
    }

    static void drawLine(float x1, float y1, float x2, float y2, float width = 5.0f) {
        glLineWidth(width);
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }

    static void drawRectangle(float cx, float cy, float width, float height) {
        glBegin(GL_QUADS);
        glVertex2f(cx - width / 2, cy - height / 2);
        glVertex2f(cx + width / 2, cy - height / 2);
        glVertex2f(cx + width / 2, cy + height / 2);
        glVertex2f(cx - width / 2, cy + height / 2);
        glEnd();
    }

public:
    int direction;       // 0=right, 1=left, 2=up, 3=down
    // --- Constructor ---
    // Updated constructor to accept hair style and color
    Man(float startX, float startY, float moveSpeed, const Color& shirt, const Color& pants, const Color& skin, const Color& hair, HairStyle style) {
        x = startX;
        y = startY;
        speed = moveSpeed;
        walkCycle = 0.0f;
        isWalking = false;
        direction = 1;
        shirtColor = shirt;
        pantsColor = pants;
        hairColor = hair;
        skinColor = skin;
        hairStyle = style;
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

    void resetPosition(float newX, float newY) {
        x = newX;
        y = newY;
        isWalking = false;
        walkCycle = 0.0f;
    }

    // --- Core Public Methods ---
    void update() {
        if (isWalking) {
            walkCycle += 0.2f;

            switch (direction) {
                case 0: x += speed; break; // Right
                case 1: x -= speed; break; // Left
                case 2: y += speed; break; // Up
                case 3: y -= speed; break; // Down
            }

            // Screen wrap
            if (x > 1.2f) x = -1.2f;
            if (x < -1.2f) x = 1.2f;
            if (y > 1.2f) y = -1.2f;
            if (y < -1.2f) y = 1.2f;
        }
    }

    void draw() {
        glPushMatrix();
        glTranslatef(x, y, 0.0f);

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
        const float hipY = torsoBottom, hipX = torsoWidth / 3;

        // --- LEGS ---
        glPushMatrix();
        glColor3f(pantsColor.r, pantsColor.g, pantsColor.b);
        // Left Leg
        glPushMatrix();
        glTranslatef(hipX, hipY, 0.0f);
        if(direction == 0 || direction == 1) glRotatef(-legAngle, 0, 0, 1);
        else glTranslatef(0.0f, -legLift, 0.0f);
        drawLine(0, 0, 0, -0.1f);
        glTranslatef(0, -0.1f, 0);
        glRotatef(std::max(0.0f, -legAngle * 0.5f), 0, 0, 1);
        drawLine(0, 0, 0, -0.09f);
        glColor3f(0.1f, 0.1f, 0.1f);
        drawRectangle(0.0f, -0.1f, 0.04f, 0.02f);
        glPopMatrix();
        // Right Leg
        glPushMatrix();
        glColor3f(pantsColor.r, pantsColor.g, pantsColor.b);
        glTranslatef(-hipX, hipY, 0.0f);
        if(direction == 0 || direction == 1) glRotatef(legAngle, 0, 0, 1);
        else glTranslatef(0.0f, legLift, 0.0f);
        drawLine(0, 0, 0, -0.1f);
        glTranslatef(0, -0.1f, 0);
        glRotatef(std::max(0.0f, legAngle * 0.5f), 0, 0, 1);
        drawLine(0, 0, 0, -0.09f);
        glColor3f(0.1f, 0.1f, 0.1f);
        drawRectangle(0.0f, -0.1f, 0.04f, 0.02f);
        glPopMatrix();
        glPopMatrix();

        // --- ARMS ---
        glColor3f(skinColor.r, skinColor.g, skinColor.b);
        // Left Arm
        glPushMatrix();
        glTranslatef(shoulderX, shoulderY, 0.0f);
        glRotatef(-armAngle, 0, 0, 1);
        drawLine(0, 0, 0, -0.08f);
        glTranslatef(0, -0.08f, 0);
        glRotatef(-30.0f, 0, 0, 1);
        drawLine(0, 0, 0, -0.07f);
        drawCircle(0, -0.075f, 0.015f);
        glPopMatrix();

        // --- TORSO ---
        glColor3f(shirtColor.r, shirtColor.g, shirtColor.b);
        drawRectangle(0.0f, (torsoTop + torsoBottom) / 2, torsoWidth, torsoTop - torsoBottom);

         // --- HEAD, FACE, and HAIR ---
        // 1. Draw Skin
        glColor3f(skinColor.r, skinColor.g, skinColor.b);
        drawCircle(0.0f, headY, headRadius, 20);

        // 3. Draw New 4-Directional Hair
        glColor3f(hairColor.r, hairColor.g, hairColor.b);
        switch(hairStyle) {
            case HairStyle::Normal:
                // Front and Side: A simple semicircle
                // Back: A full circle
                if(direction == 2){
                    drawCircle(0.0f, headY, headRadius, 20);
                } else {
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(0.0f, headY);
                    for (int i = 0; i <= 12; i++) {
                        float angle = M_PI * i / 12.0f;
                        glVertex2f(cos(angle) * headRadius, sin(angle) * headRadius + headY);
                    }
                    glEnd();
                }
                break;

            case HairStyle::Spiky:
                // Draws spikes on top of a base hair shape in all directions.
                if (direction == 2) { // Back View
                     drawCircle(0.0f, headY, headRadius, 20); // Base hair
                     for (int i = -1; i <= 1; ++i) { // Fewer spikes for back
                        float bx = i * 0.025f;
                        glBegin(GL_TRIANGLES);
                        glVertex2f(bx - 0.01f, headY + headRadius * 0.9f);
                        glVertex2f(bx + 0.01f, headY + headRadius * 0.9f);
                        glVertex2f(bx, headY + headRadius * 1.5f);
                        glEnd();
                    }
                } else { // Front and Side View
                    // Base hair shape
                    glBegin(GL_POLYGON);
                    glVertex2f(-headRadius, headY); glVertex2f(headRadius, headY);
                    glVertex2f(headRadius, headY+0.02f); glVertex2f(-headRadius, headY+0.02f);
                    glEnd();
                    // Spikes
                    for (int i = -2; i <= 2; ++i) {
                        if (direction != 3 && i > 0) continue; // Fewer spikes on side view
                        float bx = i * 0.018f;
                        glBegin(GL_TRIANGLES);
                        glVertex2f(bx - 0.01f, headY + 0.02f);
                        glVertex2f(bx + 0.01f, headY + 0.02f);
                        glVertex2f(bx, headY + 0.06f);
                        glEnd();
                    }
                }
                break;

            case HairStyle::FlatTop:
                // A classic flat-top haircut
                if(direction == 2){ // Back
                    drawRectangle(0.0f, headY + headRadius * 0.5f, headRadius * 2, headRadius);
                    drawCircle(0.0f, headY, headRadius*0.95f, 20);
                } else { // Front and Side
                    drawRectangle(0.0f, headY + headRadius, headRadius * 2, headRadius*0.4f);
                    drawRectangle(0.0f, headY + headRadius * 0.4f, headRadius * 2 * 0.9f, headRadius);
                }
                break;

            case HairStyle::Ponytail:
                // Normal hair on top, with a ponytail visible from sides and back.
                if(direction != 2) { // Front and Side
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(0.0f, headY);
                    for (int i = 0; i <= 12; i++) {
                        float angle = M_PI * i / 12.0f;
                        glVertex2f(cos(angle) * headRadius, sin(angle) * headRadius + headY);
                    }
                    glEnd();
                } else { // Back
                    drawCircle(0.0f, headY, headRadius, 20);
                }
                // Draw the ponytail itself if not facing front
                if(direction != 3) {
                    drawCircle(-headRadius, headY - 0.01f, 0.015f, 20); // The hair tie
                    glLineWidth(10.0f);
                    drawLine(-headRadius - 0.01f, headY - 0.02f, -headRadius - 0.02f, headY - 0.08f, 10.0f);
                }
                break;
        }

                // 2. Eyes
        if (direction != 2) {
            glColor3f(0.0f, 0.0f, 0.0f);
            if (direction == 3) { // Front
                drawCircle(-0.012f, headY + 0.01f, 0.005f, 20);
                drawCircle( 0.012f, headY + 0.01f, 0.005f, 20);
                drawLine(-0.01f, headY - 0.015f, 0.01f, headY - 0.015f, 2.0f);
            } else { // Side
                drawCircle(-0.015f, headY + 0.005f, 0.004f, 20);
                //nose
                glColor3f(skinColor.r, skinColor.g, skinColor.b);
                glBegin(GL_POLYGON);
                    glVertex2f(-headRadius, headY); 
                    glVertex2f(-headRadius, headY - 0.01f); 
                    glVertex2f(-headRadius - 0.01f, headY - 0.005f);
                glEnd();
                //mouth
                glColor3f(0.0f, 0.0f, 0.0f);
                drawLine(-0.015f, headY - 0.01f, -0.03f, headY - 0.01f, 2.0f);
            }
        }

        // --- Right Arm ---
        glColor3f(skinColor.r, skinColor.g, skinColor.b); // Skin
        glPushMatrix();
        glTranslatef(-shoulderX, shoulderY, 0.0f);
        glRotatef(armAngle, 0, 0, 1);
        drawLine(0, 0, 0, -0.08f);
        glTranslatef(0, -0.08f, 0);
        glRotatef(-30.0f, 0, 0, 1);
        drawLine(0, 0, 0, -0.07f);
        drawCircle(0, -0.075f, 0.015f);
        glPopMatrix();

        glPopMatrix(); // End of man's transformation
    }
};

// ===================================================================
//  Global Scope: Scene Management and GLUT Callbacks
// ===================================================================

std::vector<Man*> men;
int activeManIndex = 0;

void drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.9f, 1.0f);
    glVertex2f(-2.0f, 2.0f);
    glVertex2f(2.0f, 2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(2.0f, -0.5f);
    glVertex2f(-2.0f, -0.5f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.6f, 0.2f);
    glVertex2f(-2.0f, -0.5f);
    glVertex2f(2.0f, -0.5f);
    glColor3f(0.3f, 0.5f, 0.1f);
    glVertex2f(2.0f, -2.0f);
    glVertex2f(-2.0f, -2.0f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawBackground();
    for (Man* man : men) {
        man->draw();
    }
    glutSwapBuffers();
}

void update(int value) {
    for (Man* man : men) {
        man->update();
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void specialKeys(int key, int x, int y) {
    if (activeManIndex < men.size()) {
        int dir = men[activeManIndex]->direction; // Keep current direction if key is not an arrow
        switch (key) {
            case GLUT_KEY_RIGHT: dir = 0; break;
            case GLUT_KEY_LEFT:  dir = 1; break;
            case GLUT_KEY_UP:    dir = 2; break;
            case GLUT_KEY_DOWN:  dir = 3; break;
        }
        men[activeManIndex]->startWalking(dir);
    }
}

void specialKeysUp(int key, int x, int y) {
    if (activeManIndex < men.size()) {
        men[activeManIndex]->stopWalking();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '9') {
        int index = key - '1';
        if (index < men.size()) {
            if(activeManIndex < men.size()) men[activeManIndex]->stopWalking();
            activeManIndex = index;
            std::cout << "Control switched to Man " << activeManIndex + 1 << std::endl;
        }
    }

    switch (key) {
        case 27:
            for (Man* man : men) { delete man; }
            men.clear();
            exit(0);
        case 'r': case 'R':
            for (Man* man : men) {
                man->resetPosition(0.0f, -0.2f);
            }
            std::cout << "All positions reset." << std::endl;
            break;
    }
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w >= h) {
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

// ===================================================================
//  Main Function
// ===================================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Advanced Walking Men");

    init();

    // --- Define some colors ---
    Color redShirt   = {0.8f, 0.2f, 0.2f};
    Color bluePants  = {0.1f, 0.2f, 0.5f};
    Color tealShirt = {0.1f, 0.5f, 0.5f};
    Color blackPants  = {0.1f, 0.1f, 0.1f};
    Color brownHair  = {0.4f, 0.2f, 0.1f};
    Color blackHair  = {0.1f, 0.1f, 0.1f};
    Color darkSkin   = {0.2f, 0.1f, 0.05f};
    Color lightSkin  = {0.9f, 0.8f, 0.7f};
    Color lightBrown = {0.9f, 0.7f, 0.5f};

    // --- Create customized Man objects ---
    men.push_back(new Man(0.2f, -0.2f, 0.01f,
        redShirt, bluePants, lightSkin, brownHair, HairStyle::Spiky));

    men.push_back(new Man(-0.5f, -0.3f, 0.015f,
        tealShirt, blackPants, darkSkin, blackHair, HairStyle::Ponytail));

    men.push_back(new Man(-0.8f, -0.4f, 0.015f,
        tealShirt, blackPants, lightBrown, blackHair, HairStyle::FlatTop));

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    std::cout << "--- Controls ---" << std::endl;
    std::cout << "Arrow Keys: Move the active man" << std::endl;
    std::cout << "Keys '1' & '2': Switch control between men" << std::endl;
    std::cout << "'R': Reset all positions" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "Control is on Man 1" << std::endl;

    glutMainLoop();

    return 0;
}

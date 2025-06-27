#pragma once

#include <GL/glut.h>
#include <GL/gl.h>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>

void switchScene(int newScene);

namespace Welcome {
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 800;
    const float TITLE_SCALE = 2.0f;
    const float BUTTON_WIDTH = 200.0f;
    const float BUTTON_HEIGHT = 50.0f;

    bool isHoveringStart = false;
    float titleY = WINDOW_HEIGHT * 0.7f;
    float titleAlpha = 0.0f;
    float buttonAlpha = 0.0f;
    float instructionsAlpha = 0.0f;

    struct Color {
        float r, g, b, a;
        Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    };

    const Color TITLE_COLOR(0.2f, 0.4f, 0.8f);
    const Color BUTTON_COLOR(0.2f, 0.6f, 0.9f);
    const Color BUTTON_HOVER_COLOR(0.3f, 0.7f, 1.0f);
    const Color TEXT_COLOR(0.9f, 0.9f, 0.9f);

    void drawText(float x, float y, const std::string& text, float scale = 1.0f, const Color& color = TEXT_COLOR) {
        glColor4f(color.r, color.g, color.b, color.a);
        glRasterPos2f(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

        
    unsigned char* backgroundImage = nullptr;

    void loadBackgroundImage(const char* filename) {
        FILE* file = fopen(filename, "rb");
        if (!file) return;

        unsigned char header[54];
        fread(header, 1, 54, file); // BMP header

        int width = *(int*)&header[18];
        int height = *(int*)&header[22];

        int imageSize = 3 * width * height;
        backgroundImage = new unsigned char[imageSize];

        fread(backgroundImage, 1, imageSize, file);
        fclose(file);
    }

    void drawSmallText(float x, float y, const std::string& text, const Color& color = TEXT_COLOR) {
        glColor4f(color.r, color.g, color.b, color.a);
        glRasterPos2f(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }

    void drawCenteredText(float y, const std::string& text, float scale = 1.0f, const Color& color = TEXT_COLOR) {
        float width = 0;
        for (char c : text) {
            width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, c);
        }
        float x = (WINDOW_WIDTH - width * scale) / 2.0f;
        drawText(x, y, text, scale, color);
    }

    void drawTitleWithShadow(float y, const std::string& text) {
        drawCenteredText(y - 2, text, TITLE_SCALE, Color(0.0f, 0.0f, 0.0f, titleAlpha * 0.5f));
        drawCenteredText(y + 2, text, TITLE_SCALE, Color(0.0f, 0.0f, 0.0f, titleAlpha * 0.5f));
        drawCenteredText(y, text, TITLE_SCALE, TITLE_COLOR);
    }

    void drawButton(float x, float y, float width, float height, const std::string& text, bool isHovered) {
        static float pulse = 0.0f;
        pulse += 0.05f;

        float hoverScale = isHovered ? 1.0f + 0.05f * sinf(pulse) : 1.0f;
        float effectiveWidth = width * hoverScale;
        float effectiveHeight = height * hoverScale;
        float centerX = x + width / 2;
        float centerY = y - height / 2;

        Color buttonColor = isHovered ? BUTTON_HOVER_COLOR : BUTTON_COLOR;
        glColor4f(buttonColor.r, buttonColor.g, buttonColor.b, buttonAlpha);

        glBegin(GL_QUADS);
        glVertex2f(centerX - effectiveWidth / 2, centerY + effectiveHeight / 2);
        glVertex2f(centerX + effectiveWidth / 2, centerY + effectiveHeight / 2);
        glVertex2f(centerX + effectiveWidth / 2, centerY - effectiveHeight / 2);
        glVertex2f(centerX - effectiveWidth / 2, centerY - effectiveHeight / 2);
        glEnd();

        float textWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text.c_str());
        float textX = centerX - textWidth / 2.0f;
        float textY = centerY - 6;
        drawText(textX, textY, text, 1.0f, Color(1.0f, 1.0f, 1.0f, buttonAlpha));
    }

    void drawBackground() {
        if (backgroundImage) {
            glRasterPos2i(0, 0);
            glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, backgroundImage);
        } else {
            // fallback gradient
            glBegin(GL_QUADS);
            glColor3f(0.05f, 0.05f, 0.15f);
            glVertex2f(0, WINDOW_HEIGHT);
            glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
            glColor3f(0.1f, 0.1f, 0.2f);
            glVertex2f(WINDOW_WIDTH, 0);
            glVertex2f(0, 0);
            glEnd();
        }
    }


    void initScene() {
        titleY = WINDOW_HEIGHT * 0.7f;
        titleAlpha = 0.0f;
        buttonAlpha = 0.0f;
        instructionsAlpha = 0.0f;
    }

    void cleanupScene() {
        // Nothing to clean up yet
    }

    void updateScene() {
        static int frameCount = 0;
        frameCount++;

        if (titleY > WINDOW_HEIGHT * 0.6f) {
            titleY -= 2.0f;
        }

        if (frameCount > 20 && titleAlpha < 1.0f) {
            titleAlpha += 0.02f;
        }
        if (frameCount > 40 && buttonAlpha < 1.0f) {
            buttonAlpha += 0.02f;
        }
        if (frameCount > 60 && instructionsAlpha < 1.0f) {
            instructionsAlpha += 0.02f;
        }

        glutPostRedisplay();
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        drawBackground();

        drawTitleWithShadow(titleY, "OpenGL City View Project");

        float buttonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
        float buttonY = WINDOW_HEIGHT * 0.4f;
        drawButton(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, "Start", isHoveringStart);

    std::vector<std::string> instructions = {
        "Instructor: Anim Al Ahsan Rupai",
        "Contributors: ",
        " - Md. Fuad Hasan",
        " - Md. Masud Alam",
        " - Md. Mahtab Habib",
        " - Abdullah Shoab",
    };


        float instructionY = WINDOW_HEIGHT * 0.25f;
        for (const auto& instruction : instructions) {
            float width = 0;
            for (char c : instruction) {
                width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, c);
            }
            float x = (WINDOW_WIDTH - width) / 2.0f;
            drawSmallText(x, instructionY, instruction, Color(0.8f, 0.8f, 0.8f, instructionsAlpha));
            instructionY -= 20.0f;
        }
    }

    void mouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            float buttonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
            float buttonY = WINDOW_HEIGHT * 0.4f;

            if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
                y >= buttonY - BUTTON_HEIGHT && y <= buttonY) {
                ::switchScene(1);
            }
        }
    }

    void mouseMotion(int x, int y) {
        float buttonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
        float buttonY = WINDOW_HEIGHT * 0.4f;

        isHoveringStart = (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
                          y >= buttonY - BUTTON_HEIGHT && y <= buttonY);

        glutPostRedisplay();
    }

    void keyboard(unsigned char key, int x, int y) {
        if (key == 27) { // ESC
            exit(0);
        }
    }
}

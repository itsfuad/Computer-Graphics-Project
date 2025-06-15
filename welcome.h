#pragma once

#include <GL/glut.h>
#include <string>
#include <vector>
#include <cmath>

// Forward declare the global switchScene function
void switchScene(int newScene);

namespace Welcome {
    // Constants
    const int WINDOW_WIDTH = 1200;  // Match your window size
    const int WINDOW_HEIGHT = 800;
    const float TITLE_SCALE = 2.0f;
    const float BUTTON_WIDTH = 200.0f;
    const float BUTTON_HEIGHT = 50.0f;
    
    // State variables
    bool isHoveringStart = false;
    float titleY = WINDOW_HEIGHT * 0.7f;
    float titleAlpha = 0.0f;
    float buttonAlpha = 0.0f;
    float instructionsAlpha = 0.0f;
    
    // Colors
    struct Color {
        float r, g, b, a;
        Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    };
    
    const Color TITLE_COLOR(0.2f, 0.4f, 0.8f);
    const Color BUTTON_COLOR(0.2f, 0.6f, 0.9f);
    const Color BUTTON_HOVER_COLOR(0.3f, 0.7f, 1.0f);
    const Color TEXT_COLOR(0.9f, 0.9f, 0.9f);
    
    // Helper function to draw text
    void drawText(float x, float y, const std::string& text, float scale = 1.0f, const Color& color = TEXT_COLOR) {
        glColor4f(color.r, color.g, color.b, color.a);
        glRasterPos2f(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }
    
    // Helper function to draw centered text
    void drawCenteredText(float y, const std::string& text, float scale = 1.0f, const Color& color = TEXT_COLOR) {
        float width = 0;
        for (char c : text) {
            width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, c);
        }
        float x = (WINDOW_WIDTH - width * scale) / 2.0f;
        drawText(x, y, text, scale, color);
    }
    
    // Draw a button
    void drawButton(float x, float y, float width, float height, const std::string& text, bool isHovered) {
        // Draw button background
        Color buttonColor = isHovered ? BUTTON_HOVER_COLOR : BUTTON_COLOR;
        glColor4f(buttonColor.r, buttonColor.g, buttonColor.b, buttonAlpha);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y - height);
        glEnd();
        
        // Draw button text
        float textX = x + (width - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text.c_str())) / 2.0f;
        float textY = y - height/2 + 6;
        drawText(textX, textY, text, 1.0f, Color(1.0f, 1.0f, 1.0f, buttonAlpha));
    }
    
    // Initialize the welcome screen
    void initScene() {
        titleY = WINDOW_HEIGHT * 0.7f;
        titleAlpha = 0.0f;
        buttonAlpha = 0.0f;
        instructionsAlpha = 0.0f;
    }
    
    // Clean up resources
    void cleanupScene() {
        // Nothing to clean up for now
    }
    
    // Update animation and state
    void updateScene(int value) {
        // Animate title entrance
        if (titleY > WINDOW_HEIGHT * 0.6f) {
            titleY -= 2.0f;
        }
        
        // Fade in elements
        if (titleAlpha < 1.0f) {
            titleAlpha += 0.02f;
        }
        if (titleAlpha >= 0.5f && buttonAlpha < 1.0f) {
            buttonAlpha += 0.02f;
        }
        if (buttonAlpha >= 0.5f && instructionsAlpha < 1.0f) {
            instructionsAlpha += 0.02f;
        }
        
        glutPostRedisplay();
    }
    
    // Display the welcome screen
    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        
        // Draw title
        glColor4f(TITLE_COLOR.r, TITLE_COLOR.g, TITLE_COLOR.b, titleAlpha);
        drawCenteredText(titleY, "OpenGL Traffic Simulation", TITLE_SCALE);
        
        // Draw start button
        float buttonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
        float buttonY = WINDOW_HEIGHT * 0.4f;
        drawButton(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, "Start Simulation", isHoveringStart);
        
        // Draw instructions
        std::vector<std::string> instructions = {
            "Controls:",
            "P: Pause/Play",
            "D: Toggle Debug Bounding Boxes",
            "N: Toggle Day/Night",
            "M: Toggle Background Traffic Sound",
            "ESC: Exit"
        };
        
        float instructionY = WINDOW_HEIGHT * 0.25f;
        for (const auto& instruction : instructions) {
            drawCenteredText(instructionY, instruction, 1.0f, Color(0.8f, 0.8f, 0.8f, instructionsAlpha));
            instructionY -= 25.0f;
        }
        
        glutSwapBuffers();
    }
    
    // Handle mouse input
    void mouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            float buttonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
            float buttonY = WINDOW_HEIGHT * 0.4f;
            
            // Check if click is within button bounds
            if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
                y >= buttonY - BUTTON_HEIGHT && y <= buttonY) {
                // Switch to first scene using the global function
                ::switchScene(1);
            }
        }
    }
    
    // Handle mouse motion for hover effects
    void mouseMotion(int x, int y) {
        float buttonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
        float buttonY = WINDOW_HEIGHT * 0.4f;
        
        isHoveringStart = (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
                          y >= buttonY - BUTTON_HEIGHT && y <= buttonY);
        
        glutPostRedisplay();
    }
    
    // Handle keyboard input
    void keyboard(unsigned char key, int x, int y) {
        if (key == 27) { // ESC key
            exit(0);
        }
    }
}

#include "shoab.h"
#include "masud.h"
#include "mahtab.h"
#include "fuad.h"
#include "welcome.h"

int currentScene = 0;  // Start with welcome screen
int numOfScene = 4;    // Total number of scenes (excluding welcome)

// Global scene management
void cleanupCurrentScene() {
    switch (currentScene) {
        case 0:
            Welcome::cleanupScene();
            break;
        case 1:
            Fuad::cleanupScene();
            break;
        case 2:
            Masud::cleanupScene();
            break;
        case 3:
            Mahtab::cleanupScene();
            break;
        case 4:
            Shoab::cleanupScene();
            break;
    }
}

void initCurrentScene() {
    switch (currentScene) {
        case 0:
            Welcome::initScene();
            break;
        case 1:
            Fuad::initScene();
            break;
        case 2:
            Masud::initScene();
            break;
        case 3:
            Mahtab::initScene();
            break;
        case 4:
            Shoab::initScene();
            break;
    }
}

void updateWindowTitle() {
    switch (currentScene) {
        case 0:
            glutSetWindowTitle("OpenGL City Scene Animation");
            break;
        case 1:
            glutSetWindowTitle("Fuad's Scene - Busy traffic Simulation");
            break;
        case 2:
            glutSetWindowTitle("Masud's Scene - City view with river and road");
            break;
        case 3:
            glutSetWindowTitle("Mahtab's Scene - City view with train, bridge and vehicles");
            break;
        case 4:
            glutSetWindowTitle("Shoab's Scene - Factory with river and vehicles");
            break;
    }
}

void switchScene(int newScene) {
    if (newScene < 0 || newScene > numOfScene) return;
    
    // Cleanup current scene
    cleanupCurrentScene();
    
    // Switch to new scene
    currentScene = newScene;
    
    // Initialize new scene
    initCurrentScene();
    
    // Update window title
    updateWindowTitle();
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            if (currentScene < numOfScene) {
                switchScene(currentScene + 1);
            }
            break;
        case GLUT_KEY_LEFT:
            if (currentScene > 0) {
                switchScene(currentScene - 1);
            }
            break;
    }
    
    // Handle scene-specific special keys
    switch (currentScene) {
        case 1:
            // Fuad's scene doesn't use special keys
            break;
        case 2:
            Masud::specialKeyboard(key, x, y);
            break;
    }
}

void handleKeyboard(unsigned char key, int x, int y) {
    // Handle keyboard input
    switch (currentScene) {
        case 0:
            Welcome::keyboard(key, x, y);
            break;
        case 1:
            Fuad::keyboard(key, x, y);
            break;
        case 2:
            Masud::keyboard(key, x, y);
            break;
        case 3:
            Mahtab::keyboard(key, x, y);
            break;
        case 4:
            Shoab::keyboard(key, x, y);
            break;
    }
}

void handleMouse(int button, int state, int x, int y) {
    // Handle mouse input
    switch (currentScene) {
        case 0:
            Welcome::mouse(button, state, x, y);
            break;
        case 1:
            Fuad::mouse(button, state, x, y);
            break;
        case 2:
            Masud::mouse(button, state, x, y);
            break;
        case 3:
            Mahtab::mouse(button, state, x, y);
            break;
    }
}

void handleMouseMotion(int x, int y) {
    if (currentScene == 0) {
        Welcome::mouseMotion(x, y);
    }
}

void updateFunc(int) {
    // Update the scene
    switch (currentScene) {
        case 0:
            Welcome::updateScene();
            break;
        case 1:
            Fuad::updateScene();
            break;
        case 2:
            Masud::updateScene();
            break;
        case 3:
            Mahtab::updateScene();
            break;
        case 4:
            Shoab::updateScene();
            break;
    }
    glutTimerFunc(1000 / 60, updateFunc, 0);
}

void displayFunc() {
    switch (currentScene) {
        case 0:
            Welcome::display();
            break;
        case 1:
            Fuad::display();
            break;
        case 2:
            Masud::display();
            break;
        case 3:
            Mahtab::display();
            break;
        case 4:
            Shoab::display();
            break;
    }
    glutSwapBuffers(); 
}


void initGLUT(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Computer Graphics Project - City View Animation");
    
    // Set initial window title
    updateWindowTitle();

    std::cout << "Initializing OpenGL..." << std::endl;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    srand(time(0));

    std::cout << "OpenGL initialization complete" << std::endl;
}


int main(int argc, char **argv) {
    
    initGLUT(argc, argv);

    std::cout << "Initializing scene..." << std::endl;

    initCurrentScene();

    glutDisplayFunc(displayFunc);
    glutTimerFunc(0, updateFunc, 0);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(handleMouse);

    glutMotionFunc(handleMouseMotion);  // for welcome screen
    
    std::cout << "Entering main loop..." << std::endl;
    glutMainLoop();

    return 0;
}
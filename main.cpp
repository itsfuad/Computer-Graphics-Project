#include "masud.h"
#include "fuad.h"
#include "welcome.h"

int currentScene = 0;  // Start with welcome screen
int numOfScene = 2;    // Total number of scenes (excluding welcome)

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
    }
}

void handleMouse(int button, int state, int x, int y) {
    // Handle mouse input
    switch (currentScene) {
        case 0:
            Welcome::mouse(button, state, x, y);
            break;
        case 1:
            //Fuad::mouse(button, state, x, y);
            break;
        case 2:
            Masud::mouse(button, state, x, y);
            break;
    }
}

void handleMouseMotion(int x, int y) {
    if (currentScene == 0) {
        Welcome::mouseMotion(x, y);
    }
}

void updateFunc(int value) {
    // Update the scene
    switch (currentScene) {
        case 0:
            Welcome::updateScene(value);
            break;
        case 1:
            Fuad::updateScene(value);
            break;
        case 2:
            Masud::updateScene(value);
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
    }
}


void initGLUT(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Traffic Simulation");
    
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
    glutMotionFunc(handleMouseMotion);  // Add mouse motion handler
    
    std::cout << "--- Controls ---" << std::endl;
    std::cout << "P: Pause/Play" << std::endl;
    std::cout << "D: Toggle Debug Bounding Boxes" << std::endl;
    std::cout << "N: Toggle Day/Night" << std::endl;
    std::cout << "M: Toggle Background Traffic Sound" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    
    std::cout << "Entering main loop..." << std::endl;
    glutMainLoop();

    return 0;
}
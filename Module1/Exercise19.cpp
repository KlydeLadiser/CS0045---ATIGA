#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

float colors[6][3] = {
    { 1.0f, 0.0f, 0.0f }, // Red
    { 0.0f, 1.0f, 0.0f }, // Green
    { 0.0f, 0.0f, 1.0f }, // Blue
    { 1.0f, 1.0f, 0.0f }, // Yellow
    { 1.0f, 0.5f, 0.0f }, // Orange
    { 0.5f, 0.0f, 0.5f }  // Purple
};

string colorNames[6] = {
    "Red",
    "Green",
    "Blue",
    "Yellow",
    "Orange",
    "Purple"
};

int selectedColor = 0;
void display() {
    glClearColor(
        colors[selectedColor][0],
        colors[selectedColor][1],
        colors[selectedColor][2],
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {

    if (key >= '1' && key <= '6') {
        selectedColor = key - '1';

        cout << "Selected color: "
            << colorNames[selectedColor]
            << endl;


        glutPostRedisplay();
    }


    if (key == 27) {
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Background Color Picker");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    cout << "Press keys 1-6 to change the background." << endl;
    cout << "Press ESC to exit." << endl;

    glutMainLoop();
    return 0;
}
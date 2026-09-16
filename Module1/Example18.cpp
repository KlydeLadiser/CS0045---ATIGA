#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 18 - Responding to Keyboard Input
 * ------------------------------------
 * Concept: glutKeyboardFunc registers a callback that GLUT invokes
 * whenever a key is pressed. Changing a global variable and calling
 * glutPostRedisplay() tells GLUT "the scene changed, please call
 * display() again."
 */

float shapeColor[3] = { 1.0f, 0.0f, 0.0f };  // starts red

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(shapeColor[0], shapeColor[1], shapeColor[2]);

    glBegin(GL_POLYGON);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r') { shapeColor[0] = 1; shapeColor[1] = 0; shapeColor[2] = 0; }
    if (key == 'g') { shapeColor[0] = 0; shapeColor[1] = 1; shapeColor[2] = 0; }
    if (key == 'b') { shapeColor[0] = 0; shapeColor[1] = 0; shapeColor[2] = 1; }
    if (key == 27) { exit(0); }  // ESC key quits

    glutPostRedisplay();  // ask GLUT to redraw with the new color
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex18 - Press r/g/b to change color, ESC to quit");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

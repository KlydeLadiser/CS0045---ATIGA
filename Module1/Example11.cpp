#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 11 - Controlling Window Size and Position
 * ------------------------------------
 * Concept: glutInitWindowSize / glutInitWindowPosition let you
 * control how big the window is and where it appears on screen
 * BEFORE glutCreateWindow is called.
 */

void display() {
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.9f, -0.9f);
    glVertex2f(-0.9f, 0.9f);
    glVertex2f(0.9f, 0.9f);
    glVertex2f(0.9f, -0.9f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1024, 768);    // wide window
    glutInitWindowPosition(50, 50);   // near the top-left of the screen
    glutCreateWindow("Ex11 - Custom Size (1024x768) and Position");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

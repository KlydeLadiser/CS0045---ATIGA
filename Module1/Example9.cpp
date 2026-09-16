#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 09 - Coloring a Shape (glColor3f)
 * ------------------------------------
 * Concept: glColor3f(r, g, b) sets the CURRENT color. Every vertex
 * drawn afterward uses that color until glColor3f is called again.
 * This is a simplified stand-in for what a fragment shader would do
 * in the modern programmable pipeline.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.9f, 0.1f, 0.1f);  // red square, left side
    glBegin(GL_POLYGON);
    glVertex2f(-0.8f, -0.3f);
    glVertex2f(-0.8f, 0.3f);
    glVertex2f(-0.2f, 0.3f);
    glVertex2f(-0.2f, -0.3f);
    glEnd();

    glColor3f(0.1f, 0.3f, 0.9f);  // blue square, right side
    glBegin(GL_POLYGON);
    glVertex2f(0.2f, -0.3f);
    glVertex2f(0.2f, 0.3f);
    glVertex2f(0.8f, 0.3f);
    glVertex2f(0.8f, -0.3f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex09 - Two Colored Squares");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 12 - Visualizing the OpenGL Coordinate System
 * ------------------------------------
 * Concept: By default, OpenGL's visible area (before any projection
 * changes) spans from -1.0 to 1.0 on both X and Y, with (0,0) at the
 * CENTER of the window. This program draws the X and Y axes plus tick
 * marks so students can see that layout directly.
 */

void drawAxes() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X axis
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    // Y axis
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    // small tick marks every 0.25 units on the X axis
    glBegin(GL_LINES);
    for (float x = -1.0f; x <= 1.0f; x += 0.25f) {
        glVertex2f(x, -0.02f);
        glVertex2f(x, 0.02f);
    }
    glEnd();
}

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    // a point marking a sample coordinate (0.5, 0.5)
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.5f, 0.5f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex12 - Coordinate System Axes");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

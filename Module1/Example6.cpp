#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 06 - A Filled Triangle
 * ------------------------------------
 * Concept: GL_TRIANGLES primitive mode + Rasterization.
 * Three vertices become one filled triangle. Rasterization is the
 * pipeline stage that converts this triangle into fragments (pixels).
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 0.0f);  // green

    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.6f);
    glVertex2f(-0.6f, -0.4f);
    glVertex2f(0.6f, -0.4f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex06 - Filled Triangle");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

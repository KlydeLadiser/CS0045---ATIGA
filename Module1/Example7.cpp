#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 07 - A Filled Square (GL_POLYGON)
 * ------------------------------------
 * Concept: GL_POLYGON fills the interior bounded by an ordered list
 * of vertices. This is the exact shape from the Module 1 slides.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex07 - Filled Square");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

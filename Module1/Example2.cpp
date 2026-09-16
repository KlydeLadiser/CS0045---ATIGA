#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 02 - Drawing a Single Point
 * ------------------------------------
 * Concept: Vertex Specification + Primitive Assembly (GL_POINTS).
 * A "vertex" is just a coordinate. GL_POINTS tells the Primitive
 * Assembly stage to treat every vertex as its own primitive (a dot).
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);   // white point
    glPointSize(10.0f);            // make the point big enough to see

    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);    // the origin, dead center of the window
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex02 - Single Point");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

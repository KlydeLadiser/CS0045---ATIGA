#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 03 - Drawing a Single Line
 * ------------------------------------
 * Concept: GL_LINES primitive mode.
 * GL_LINES groups vertices in PAIRS: (v0,v1), (v2,v3), ...
 * Each pair becomes one independent line segment.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);  // yellow
    glLineWidth(3.0f);

    glBegin(GL_LINES);
    glVertex2f(-0.7f, -0.5f);
    glVertex2f(0.7f, 0.5f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex03 - Single Line");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

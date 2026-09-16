#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 13 - Multiple Independent Shapes in One Scene
 * ------------------------------------
 * Concept: display() may contain several glBegin()/glEnd() blocks.
 * Each block is submitted through the pipeline separately, but they
 * all land in the same framebuffer before glFlush() is called.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // a triangle on the left
    glColor3f(0.9f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.9f, -0.3f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(-0.1f, -0.3f);
    glEnd();

    // a square in the middle
    glColor3f(0.0f, 0.6f, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.2f, -0.3f);
    glVertex2f(-0.2f, 0.3f);
    glVertex2f(0.2f, 0.3f);
    glVertex2f(0.2f, -0.3f);
    glEnd();

    // a line on the right
    glColor3f(0.2f, 0.2f, 0.9f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex2f(0.4f, -0.4f);
    glVertex2f(0.9f, 0.4f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(700, 500);
    glutCreateWindow("Ex13 - Multiple Shapes in One Window");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

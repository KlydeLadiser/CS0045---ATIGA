#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 19 - Simple Animation with glutIdleFunc
 * ------------------------------------
 * Concept: glutIdleFunc registers a callback that GLUT calls
 * continuously whenever there are no other events to process. This
 * is how classic GLUT programs animate: update a variable, then
 * request a redraw.
 */

float squareX = -0.8f;
float speed = 0.01f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2f, 0.7f, 0.9f);

    glBegin(GL_POLYGON);
    glVertex2f(squareX - 0.1f, -0.1f);
    glVertex2f(squareX - 0.1f, 0.1f);
    glVertex2f(squareX + 0.1f, 0.1f);
    glVertex2f(squareX + 0.1f, -0.1f);
    glEnd();

    glFlush();
}

void idle() {
    squareX += speed;
    if (squareX > 0.8f || squareX < -0.8f) {
        speed = -speed;  // bounce off the edges
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 300);
    glutCreateWindow("Ex19 - Bouncing Square (glutIdleFunc)");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

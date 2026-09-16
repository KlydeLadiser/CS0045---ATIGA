#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;


void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black background
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.6f, 0.0f);
    glVertex2f(-0.3f, 0.5f);
    glVertex2f(0.3f, 0.5f);
    glVertex2f(0.6f, 0.0f);
    glVertex2f(0.3f, -0.5f);
    glVertex2f(-0.3f, -0.5f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex08 - Custom Background Color");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
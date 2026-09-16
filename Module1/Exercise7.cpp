#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.8f, -0.4f);
    glVertex2f(-0.2f, -0.4f);
    glVertex2f(-0.5f, 0.4f);
    glEnd();


    glColor3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    glVertex2f(0.2f, -0.4f);
    glVertex2f(0.8f, -0.4f);
    glVertex2f(0.5f, 0.4f);
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
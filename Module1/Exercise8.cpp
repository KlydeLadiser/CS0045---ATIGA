#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f); // red
    glVertex2f(0.0f, 0.7f);       // top

    glColor3f(0.0f, 1.0f, 0.0f); // green
    glVertex2f(0.7f, 0.0f);       // right

    glColor3f(0.0f, 0.0f, 1.0f); // blue
    glVertex2f(0.0f, -0.7f);      // bottom

    glColor3f(1.0f, 1.0f, 0.0f); // yellow
    glVertex2f(-0.7f, 0.0f);      // left
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex10 - Smooth Shaded Triangle");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
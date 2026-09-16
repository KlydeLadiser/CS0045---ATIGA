#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 08 - Changing the Background Color
 * ------------------------------------
 * Concept: glClearColor(r, g, b, a) sets the color used the NEXT
 * time glClear(GL_COLOR_BUFFER_BIT) runs. It does not draw anything
 * by itself - it just "loads the paint" for the clear operation.
 */

void display() {
    glClearColor(1.0f, 0.85f, 0.6f, 1.0f);  // warm peach background
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.2f, 0.2f, 0.2f);  // dark gray square
    glBegin(GL_POLYGON);
    glVertex2f(-0.4f, -0.4f);
    glVertex2f(-0.4f, 0.4f);
    glVertex2f(0.4f, 0.4f);
    glVertex2f(0.4f, -0.4f);
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

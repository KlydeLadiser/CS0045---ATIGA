#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 15 - glLineWidth and a Line Grid
 * ------------------------------------
 * Concept: glLineWidth changes how thick GL_LINES primitives are
 * once rasterized. Loops let us generate many vertices programmatically
 * instead of typing each glVertex2f call by hand.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);
    for (float x = -0.8f; x <= 0.8f; x += 0.2f) {
        glVertex2f(x, -0.8f);
        glVertex2f(x, 0.8f);
    }
    for (float y = -0.8f; y <= 0.8f; y += 0.2f) {
        glVertex2f(-0.8f, y);
        glVertex2f(0.8f, y);
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex15 - Line Grid");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

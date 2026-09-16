#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 14 - glPointSize and a Field of Points
 * ------------------------------------
 * Concept: glPointSize controls how many pixels (fragments) each
 * GL_POINTS vertex covers after Rasterization.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.1f, 0.5f, 1.0f);
    glPointSize(6.0f);

    glBegin(GL_POINTS);
    for (float x = -0.8f; x <= 0.8f; x += 0.2f) {
        for (float y = -0.8f; y <= 0.8f; y += 0.2f) {
            glVertex2f(x, y);
        }
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex14 - Grid of Points");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

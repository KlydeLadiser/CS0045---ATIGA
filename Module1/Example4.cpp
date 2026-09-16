#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 04 - GL_LINE_STRIP
 * ------------------------------------
 * Concept: GL_LINE_STRIP connects each vertex to the NEXT one in a
 * continuous chain (v0-v1, v1-v2, v2-v3, ...). The shape does NOT
 * close back to the first vertex.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 1.0f);  // cyan
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.8f, -0.4f);
    glVertex2f(-0.4f, 0.4f);
    glVertex2f(0.0f, -0.4f);
    glVertex2f(0.4f, 0.4f);
    glVertex2f(0.8f, -0.4f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex04 - Line Strip (Zig-Zag)");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

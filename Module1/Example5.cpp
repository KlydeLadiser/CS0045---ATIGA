#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 05 - GL_LINE_LOOP
 * ------------------------------------
 * Concept: GL_LINE_LOOP behaves like GL_LINE_STRIP, but it ADDS one
 * extra segment connecting the last vertex back to the first,
 * producing a closed outline (here, a triangle outline).
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.4f, 0.7f);  // pink
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(0.0f, 0.6f);
    glVertex2f(-0.6f, -0.4f);
    glVertex2f(0.6f, -0.4f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex05 - Line Loop (Triangle Outline)");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

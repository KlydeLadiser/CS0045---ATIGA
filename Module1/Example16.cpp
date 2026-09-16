#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 16 - A Regular Pentagon
 * ------------------------------------
 * Concept: Any regular polygon can be built by placing vertices
 * around a circle at equal angle steps, then feeding them to
 * GL_POLYGON in order.
 */
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.6f, 0.2f, 0.8f);

    const int sides = 5;
    const float radius = 0.6f;

    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; i++) {
        float angle = (float)i / sides * 2.0f * 3.14159265f + 1.5707963f; // start pointing up
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex16 - Regular Pentagon");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

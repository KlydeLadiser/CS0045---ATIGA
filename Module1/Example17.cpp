#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 17 - Approximating a Circle with GL_TRIANGLE_FAN
 * ------------------------------------
 * Concept: GL_TRIANGLE_FAN treats the first vertex as a shared
 * "center" vertex; every pair of following vertices forms a triangle
 * with that center. Enough thin triangles around a center point look
 * like a filled circle once rasterized.
 */
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.6f, 0.0f);

    const int segments = 60;
    const float radius = 0.6f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);  // center of the fan
    for (int i = 0; i <= segments; i++) {
        float angle = (float)i / segments * 2.0f * 3.14159265f;
        glVertex2f(radius * cosf(angle), radius * sinf(angle));
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex17 - Circle via Triangle Fan");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

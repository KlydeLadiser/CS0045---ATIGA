#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 10 - Per-Vertex Color / Smooth Shading
 * ------------------------------------
 * Concept: When each vertex of a primitive gets a DIFFERENT color,
 * OpenGL interpolates the colors across the interior during
 * Rasterization. This mimics how the Vertex Shader stage passes
 * varying data down to the Fragment Shader stage in the modern
 * pipeline.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.6f); // red top
    glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(-0.6f, -0.4f); // green left
    glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.6f, -0.4f); // blue right
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

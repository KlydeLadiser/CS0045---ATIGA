#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

/*
 * Example 20 - Capstone: A Small Composed Scene
 * ------------------------------------
 * Concept: Combines everything from this module - background color,
 * multiple primitive types, per-vertex color, and a circle built
 * from a triangle fan - into one simple "landscape" scene: sky,
 * ground, sun, and a house.
 */
#include <cmath>

void drawSun() {
    glColor3f(1.0f, 0.85f, 0.2f);
    const int segments = 40;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.6f, 0.6f);
    for (int i = 0; i <= segments; i++) {
        float angle = (float)i / segments * 2.0f * 3.14159265f;
        glVertex2f(0.6f + 0.18f * cosf(angle), 0.6f + 0.18f * sinf(angle));
    }
    glEnd();
}

void drawGround() {
    glColor3f(0.2f, 0.6f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.0f, -0.3f);
    glVertex2f(1.0f, -0.3f);
    glVertex2f(1.0f, -1.0f);
    glEnd();
}

void drawHouse() {
    // walls
    glColor3f(0.8f, 0.5f, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.3f, -0.3f);
    glVertex2f(-0.3f, 0.1f);
    glVertex2f(0.3f, 0.1f);
    glVertex2f(0.3f, -0.3f);
    glEnd();

    // roof
    glColor3f(0.6f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.35f, 0.1f);
    glVertex2f(0.0f, 0.4f);
    glVertex2f(0.35f, 0.1f);
    glEnd();
}

void display() {
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);  // sky blue
    glClear(GL_COLOR_BUFFER_BIT);

    drawSun();
    drawGround();
    drawHouse();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Ex20 - Mini Scene");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

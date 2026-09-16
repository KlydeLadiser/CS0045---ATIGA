#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;
 
/*
 * Example 01 - The Empty GLUT Window
 * -----------------------------------
 * Concept: OpenGL / GLUT program skeleton.
 * Pipeline stage highlighted: Per-Sample Operations (the framebuffer
 * is cleared to a background color before anything else happens).
 *
 * This is the smallest possible OpenGL/GLUT program. It opens a
 * window and clears it to a solid color. There is no geometry yet -
 * this proves that the pipeline runs even with zero primitives.
 */
 
void display() {
    glClearColor(0.1f, 0.1f, 0.35f, 1.0f); // dark blue background
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}
 
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex01 - Empty Window");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif


void drawBitmapString(void* font, const char* text) {
    for (const char* character = text; *character != '\0'; character++) {
        glutBitmapCharacter(font, *character);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Title
    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos2f(-0.50f, 0.70f);
    drawBitmapString(
        GLUT_BITMAP_TIMES_ROMAN_24,
        "User Profile"
    );
    
    // Student number
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.60f, 0.450f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "Student Number: 202411971"
    );

    // Name
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.60f, 0.30f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "First Name: JREX LESTER"
    );

    // Middle Name
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.60f, 0.15f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "Middle Name: PADERES"
    );

    //Last Name
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.60f, 0.00f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "Last Name: ATIGA"
    );

    // Program
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.60f, -0.15f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "Personal Email Address: dotsalot992@gmail.com"
    );

    // Section
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.60f, -0.30f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "School Email Address: jpatiga@fit.edu.ph"
    );

    // Citizenship
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.60f, -0.45f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "CitizenShip: Filipino"
    );

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.60f, -0.60f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "Program: BSCSSE"
    );

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.10f, 0.45f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "ENROLLMENT STATUS: TERM 1 SY 20262027"
    );

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.10f, 0.30f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "YEAR_LEVEL: 3"
    );

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.10f, 0.15f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "REGISTRATION STATUS: 21 UNITS ENROLLED"
    );

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.10f, 0.00f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "STUDENT TYPE: IRREGULAR"
    );

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.10f, -0.15f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "SCHOLARSHIP: NONE"
    );

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.10f, -0.30f);
    drawBitmapString(
        GLUT_BITMAP_HELVETICA_18,
        "GRADES VIEWING: CLOSED"
    );

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 500);
    glutCreateWindow("Student Information");

    // NOTE Dark-blue background
    glClearColor(31/255.0f, 135/255.0f, 26/255.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif
#include <iostream>
using namespace std;


void drawBitmapString(void* font, const char* str) {
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    
   glColor3f(3.0f/255.0f, 128.0f/255.0f, 2.0f/255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.98f,  0.333f);   
        glVertex2f( 0.98f,  0.333f);   
        glVertex2f( 0.98f,  0.209f);   
        glVertex2f(-0.98f,  0.209f); 
    glEnd();

    // CS0011 
    glColor3f(165.0f/255.0f, 240.0f/255.0f, 161.0f/255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.98f,  0.209f);
        glVertex2f( 0.98f,  0.209f);
        glVertex2f( 0.98f,  0.076f);
        glVertex2f(-0.98f,  0.076f);
    glEnd();

    // CS0016 
    glColor3f(252.0f/255.0f, 251.0f/255.0f, 143.0f/255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.98f,  0.076f);
        glVertex2f( 0.98f,  0.076f);
        glVertex2f( 0.98f, -0.058f);
        glVertex2f(-0.98f, -0.058f);
    glEnd();

    // CS0019 
    glColor3f(165.0f/255.0f, 240.0f/255.0f, 161.0f/255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.98f, -0.058f);
        glVertex2f( 0.98f, -0.058f);
        glVertex2f( 0.98f, -0.191f);
        glVertex2f(-0.98f, -0.191f);
    glEnd();

    // CS0025 
    glColor3f(252.0f/255.0f, 251.0f/255.0f, 143.0f/255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.98f, -0.191f);
        glVertex2f( 0.98f, -0.191f);
        glVertex2f( 0.98f, -0.324f);
        glVertex2f(-0.98f, -0.324f);
    glEnd();

    // CS0045 
    glColor3f(165.0f/255.0f, 240.0f/255.0f, 161.0f/255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.98f, -0.324f);
        glVertex2f( 0.98f, -0.324f);
        glVertex2f( 0.98f, -0.458f);
        glVertex2f(-0.98f, -0.458f);
    glEnd();

    // CS0053
    glColor3f(252.0f/255.0f, 251.0f/255.0f, 143.0f/255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.98f, -0.458f);
        glVertex2f( 0.98f, -0.458f);
        glVertex2f( 0.98f, -0.591f);
        glVertex2f(-0.98f, -0.591f);
    glEnd();

    // GED0075 
    glColor3f(165.0f/255.0f, 240.0f/255.0f, 161.0f/255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.98f, -0.591f);
        glVertex2f( 0.98f, -0.591f);
        glVertex2f( 0.98f, -0.724f);
        glVertex2f(-0.98f, -0.724f);
    glEnd();

    // Total units 
    glColor3f(3.0f/255.0f, 128.0f/255.0f, 2.0f/255.0f);
    glRectf(-0.98f, -0.724f, 0.98f, -0.858f);

    // Table border
    glColor3f(0.55f, 0.55f, 0.55f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.98f,  0.333f);
        glVertex2f( 0.98f,  0.333f);
        glVertex2f( 0.98f, -0.858f);
        glVertex2f(-0.98f, -0.858f);
    glEnd();

    
    glColor3f(0.1f, 0.1f, 0.1f);

    // Student information (left)
    glRasterPos2f(-0.98f, 0.87f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Enrollment Status : ENROLLED");

    glRasterPos2f(-0.98f, 0.76f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Student # : 202411971");

    glRasterPos2f(-0.98f, 0.66f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Name : ATIGA, JREX LESTER PADERES");

    glRasterPos2f(-0.98f, 0.56f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Phone : +639102367130    Zip Code : 1100");

    glRasterPos2f(-0.98f, 0.46f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Classification :");

    // Student information (right)
    glRasterPos2f(0.03f, 0.76f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "College : COMPUTER STUDIES    Program : BSCSSE");

    glRasterPos2f(0.03f, 0.66f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Year Level : 3    1st term, SY 26-27");

    glRasterPos2f(0.03f, 0.56f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Address : 118 Dama de Noche st.");

    glColor3f(1.0f, 1.0f, 1.0f);

    // Table Header
    glRasterPos2f(-0.95f, 0.25f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Courses");

    glRasterPos2f(-0.82f, 0.25f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Title");

    glRasterPos2f(0.00f, 0.25f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Section");

    glRasterPos2f(0.125f, 0.25f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Units");

    glRasterPos2f(0.22f, 0.25f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Days");

    glRasterPos2f(0.35f, 0.25f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Time");

    glRasterPos2f(0.75f, 0.25f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Room");

    glColor3f(0.1f, 0.1f, 0.1f);

    // CS0011 
    glRasterPos2f(-0.95f, 0.125f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "CS0011");

    glRasterPos2f(-0.82f, 0.125f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "MOBILE PROGRAMMING");

    glRasterPos2f(0.00f, 0.125f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "TA31");

    glRasterPos2f(0.125f, 0.125f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "3");

    glRasterPos2f(0.22f, 0.125f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "S / W");

    glRasterPos2f(0.35f, 0.125f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "09:00:00-10:50:00 / 09:00:00-10:50:00");

    glRasterPos2f(0.75f, 0.125f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "F1209 / F608");

    // CS0016
    glRasterPos2f(-0.95f, -0.01f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "CS0016");

    glRasterPos2f(-0.82f, -0.01f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "NETWORK AND COMMUNICATIONS 2A");

    glRasterPos2f(0.00f, -0.01f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "TN31");

    glRasterPos2f(0.125f, -0.01f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "3");

    glRasterPos2f(0.22f, -0.01f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "T / TH");

    glRasterPos2f(0.35f, -0.01f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "09:00:00-10:50:00 / 09:00:00-10:50:00");

    glRasterPos2f(0.75f, -0.01f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "ONLINE / F1101");

    // CS0019
    glRasterPos2f(-0.95f, -0.142f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "CS0019");

    glRasterPos2f(-0.82f, -0.142f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "MODELING AND SIMULATION");

    glRasterPos2f(0.00f, -0.142f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "TN33");

    glRasterPos2f(0.125f, -0.142f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "3");

    glRasterPos2f(0.22f, -0.142f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "F / T");

    glRasterPos2f(0.35f, -0.142f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "17:00:00-18:50:00 / 17:00:00-18:50:00");

    glRasterPos2f(0.75f, -0.142f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "ONLINE / ONLINE");

    // CS0025
    glRasterPos2f(-0.95f, -0.275f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "CS0025");

    glRasterPos2f(-0.82f, -0.275f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "SOFTWARE ENGINEERING 1");

    glRasterPos2f(0.00f, -0.275f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "TN37");

    glRasterPos2f(0.125f, -0.275f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "3");

    glRasterPos2f(0.22f, -0.275f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "F / S");

    glRasterPos2f(0.35f, -0.275f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "13:00:00-14:50:00 / 13:00:00-14:50:00");

    glRasterPos2f(0.75f, -0.275f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "ONLINE / F702");

    // CS0045 
    glRasterPos2f(-0.95f, -0.408f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "CS0045");

    glRasterPos2f(-0.82f, -0.408f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "CS ELECTIVE - COMPUTER GRAPHICS AND VISUAL COMPUTING");

    glRasterPos2f(0.00f, -0.408f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "TN35");

    glRasterPos2f(0.125f, -0.408f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "3");

    glRasterPos2f(0.22f, -0.408f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "M / TH");

    glRasterPos2f(0.35f, -0.408f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "13:00:00-14:50:00 / 13:00:00-14:50:00");

    glRasterPos2f(0.75f, -0.408f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "E601 / E610");

    // CS0053
    glRasterPos2f(-0.95f, -0.542f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "CS0053");

    glRasterPos2f(-0.82f, -0.542f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "CS SPECIALIZATION 2 - PROGRAMMING TOOLS AND TECHNIQUES");

    glRasterPos2f(0.00f, -0.542f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "TN35");

    glRasterPos2f(0.125f, -0.542f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "3");

    glRasterPos2f(0.22f, -0.542f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "M / TH");

    glRasterPos2f(0.35f, -0.542f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "07:00:00-08:50:00 / 07:00:00-08:50:00");

    glRasterPos2f(0.75f, -0.542f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "F702 / F609");

    // GED0075
    glRasterPos2f(-0.95f, -0.675f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "GED0075");

    glRasterPos2f(-0.82f, -0.675f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "LINEAR ALGEBRA");

    glRasterPos2f(0.00f, -0.675f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "TN25");

    glRasterPos2f(0.125f, -0.675f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "3");

    glRasterPos2f(0.22f, -0.675f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "F / T");

    glRasterPos2f(0.35f, -0.675f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "07:00:00-08:50:00 / 07:00:00-08:50:00");

    glRasterPos2f(0.75f, -0.675f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "ONLINE / ONLINE");

    glColor3f(1.0f, 1.0f, 1.0f);

    // Total units
    glRasterPos2f(-0.97f, -0.808f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_12, "TOTAL UNITS 21");

    

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1200, 450);
    glutCreateWindow("Schedule Assessment");
    glClearColor(0.96f, 0.96f, 0.98f, 1.0f);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
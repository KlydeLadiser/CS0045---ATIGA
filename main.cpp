/*
 * SANTA'S SURPRISE WORKSHOP
 * Fixed coordinates, ordinary functions, and explicit OpenGL arrays.
 * Build and run: double-click run.cmd in this folder.
 *
 * MODULE GUIDE (page numbers refer to the supplied PDFs):
 * 05, pp. 9-32: initBuffers, drawGift, drawGear, updateSnow, cleanup.
 *     VBOs, indexed drawing, static/dynamic data, mapping and unmapping.
 * 06, pp. 7-28: matrixExamples and translateSnowPoint.
 *     Matrix arithmetic, homogeneous coordinates and transformation order.
 * 06, pp. 29-46: giftColors, drawBackdrop and drawToyCube.
 *     Immediate drawing, vertex/color arrays, buffer offsets, array elements.
 * 07, pp. 3-41: display, reshape and drawTableToy.
 *     Matrix stacks, translation/rotation/scaling, viewport, projections,
 *     depth testing and gluLookAt.
 * 08, pp. 6-33: initTexture and drawTableToy.
 *     Normals, materials, ambient/diffuse/specular light, directional/point/
 *     spot lights, texture coordinates, filtering, wrapping and mipmaps.
 *     The image is an 8x8 RGB array below, so no external image is needed.
 *
 * Left/right click: turn the gear and open/close the box.
 * Mouse wheel: resize the robot. Space: pause gifts. R: reset everything.
 * L: lighting. T: wrapping texture. P: change the 3D toy's projection.
 * 1/2/3: directional/point/spot light. F: fullscreen/window. Esc or X: exit.
 */

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

const float PI = 3.14159265f;
const float SCENE_W = 1672.0f, SCENE_H = 941.0f;
const float GEAR_X = 445.0f, GEAR_Y = 278.0f;
const float BOX_X0 = 660.0f, BOX_X1 = 908.0f;
const float BOX_TOP = 612.0f, BOX_BOT = 735.0f;
const float ROBOT_X = 783.0f;

int windowWidth = 1280, windowHeight = 720;
int viewX = 0, viewY = 0, viewWidth = 1280, viewHeight = 720;
float pixelScale = 1.0f;
float beltOffset = 0.0f, gearAngle = 0.0f, gearTarget = 0.0f;
float lidAmount = 1.0f, lidTarget = 1.0f, robotScale = 1.0f;
float toyAngle = 25.0f;
bool beltPaused = false, fullscreen = true;
bool lightingOn = true, textureOn = true;
bool directionalOn = true, pointOn = true, spotOn = true;
int projectionMode = 0; // 0: perspective, 1: orthographic, 2: frustum

GLuint giftVBO[3] = {0, 0, 0};
GLuint gearVBO = 0, snowVBO = 0, toyVBO = 0, toyEBO = 0;
GLuint wrappingTexture = 0;
int glErrorCount = 0;

// A color is just three numbers; all drawing positions are written below.
struct Col { float r, g, b; };
Col rgb(int r, int g, int b) { return {r / 255.0f, g / 255.0f, b / 255.0f}; }
void color(Col c) { glColor3f(c.r, c.g, c.b); }

const Col CREAM = rgb(251, 240, 216), NAVY = rgb(22, 38, 84);
const Col NAVYD = rgb(28, 43, 80), RED = rgb(224, 32, 42);
const Col REDL = rgb(238, 66, 72), YELLOW = rgb(253, 200, 20);
const Col GREEN = rgb(27, 98, 54), WOODD = rgb(150, 92, 46);
const Col SKIN = rgb(248, 190, 145), WHITE = rgb(255, 255, 255);
const Col TEAL = rgb(38, 167, 188), GRAY = rgb(122, 124, 130);
const Col BROWN = rgb(150, 90, 45);

// Small drawing functions: no mesh objects or generated scene graph.
void rect(float x0, float y0, float x1, float y1, Col c) {
    color(c);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0); glVertex2f(x1, y0);
    glVertex2f(x1, y1); glVertex2f(x0, y1);
    glEnd();
}

void tri(float x0, float y0, float x1, float y1, float x2, float y2, Col c) {
    color(c);
    glBegin(GL_TRIANGLES);
    glVertex2f(x0, y0); glVertex2f(x1, y1); glVertex2f(x2, y2);
    glEnd();
}

void sector(float cx, float cy, float rx, float ry, float a0, float a1,
            Col c, int segments) {
    color(c);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; ++i) {
        float a = (a0 + (a1 - a0) * i / segments) * PI / 180.0f;
        glVertex2f(cx + rx * std::cos(a), cy + ry * std::sin(a));
    }
    glEnd();
}

void circle(float x, float y, float radius, Col c, int segments = 28) {
    sector(x, y, radius, radius, 0, 360, c, segments);
}

void ellipse(float x, float y, float rx, float ry, Col c, int segments = 28) {
    sector(x, y, rx, ry, 0, 360, c, segments);
}

void rrect(float x0, float y0, float x1, float y1, float r, Col c) {
    rect(x0 + r, y0, x1 - r, y1, c);
    rect(x0, y0 + r, x0 + r, y1 - r, c);
    rect(x1 - r, y0 + r, x1, y1 - r, c);
    sector(x0 + r, y0 + r, r, r, 180, 270, c, 8);
    sector(x1 - r, y0 + r, r, r, 270, 360, c, 8);
    sector(x1 - r, y1 - r, r, r, 0, 90, c, 8);
    sector(x0 + r, y1 - r, r, r, 90, 180, c, 8);
}

void thickLine(float x0, float y0, float x1, float y1, float width, Col c) {
    float dx = x1 - x0, dy = y1 - y0;
    float length = std::sqrt(dx * dx + dy * dy);
    if (length == 0.0f) return;
    float nx = -dy * width / (2.0f * length);
    float ny = dx * width / (2.0f * length);
    color(c);
    glBegin(GL_QUADS);
    glVertex2f(x0 + nx, y0 + ny); glVertex2f(x1 + nx, y1 + ny);
    glVertex2f(x1 - nx, y1 - ny); glVertex2f(x0 - nx, y0 - ny);
    glEnd();
}

void rotRect(float x, float y, float w, float h, float angle, Col c) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1);
    rect(-w / 2, -h / 2, w / 2, h / 2, c);
    glPopMatrix();
}

void star5(float x, float y, float outer, float inner, Col c) {
    color(c);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 10; ++i) {
        float angle = -PI / 2 + i * PI / 5;
        float radius = (i % 2 == 0) ? outer : inner;
        glVertex2f(x + radius * std::cos(angle), y + radius * std::sin(angle));
    }
    glEnd();
}

void point(float x, float y, Col c, float size) {
    color(c);
    glPointSize(std::max(1.0f, size * pixelScale));
    glBegin(GL_POINTS); glVertex2f(x, y); glEnd();
}

void line(float x0, float y0, float x1, float y1, Col c, float width) {
    color(c);
    glLineWidth(std::max(1.0f, width * pixelScale));
    glBegin(GL_LINES); glVertex2f(x0, y0); glVertex2f(x1, y1); glEnd();
}

void hbar(float x0, float x1, float y0, float y1, Col c) {
    color(c);
    glBegin(GL_QUAD_STRIP);
    glVertex2f(x0, y0); glVertex2f(x0, y1);
    glVertex2f((x0 + x1) / 2, y0); glVertex2f((x0 + x1) / 2, y1);
    glVertex2f(x1, y0); glVertex2f(x1, y1);
    glEnd();
}

void drawBackdrop() {
    // wall, floor, wainscot
    rect(0, 0, SCENE_W, 706, CREAM);
    rect(0, 706, SCENE_W, SCENE_H, rgb(240, 228, 205));
    rect(215, 575, SCENE_W, 706, rgb(175, 214, 208));

    // ---- pegboard + tools
    rect(0, 143, 218, 460, rgb(247, 224, 188));
    for (float x = 22; x < 218; x += 32)
        for (float y = 165; y < 450; y += 34) point(x, y, rgb(190, 125, 70), 4);
    rect(66, 258, 88, 390, rgb(240, 52, 46));                           // hammer
    rect(33, 250, 116, 290, rgb(86, 88, 98));
    tri(33, 250, 33, 290, 14, 272, rgb(86, 88, 98));
    circle(160, 283, 27, rgb(160, 164, 170));                           // wrench
    rect(149, 290, 172, 362, rgb(160, 164, 170));
    circle(161, 374, 21, rgb(160, 164, 170));
    circle(161, 374, 8, rgb(247, 224, 188));
    circle(160, 276, 11, rgb(247, 224, 188));
    rect(153, 252, 167, 276, rgb(247, 224, 188));

    // ---- wooden cabinet
    rect(0, 488, 220, 706, rgb(184, 119, 62));
    rect(0, 488, 222, 516, WOODD);
    rect(20, 528, 190, 590, rgb(208, 140, 72));
    rect(20, 604, 190, 664, rgb(208, 140, 72));
    circle(104, 560, 12, rgb(92, 52, 28));
    circle(104, 632, 12, rgb(92, 52, 28));
    // Module 6: these outlines use plain CPU vertex arrays.
    const GLfloat drawerOutlines[] = {
        20,528, 190,528, 190,590, 20,590, 20,528,
        20,604, 190,604, 190,664, 20,664, 20,604
    };
    color(WOODD);
    glLineWidth(std::max(1.0f, 2 * pixelScale));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, drawerOutlines);
    glDrawArrays(GL_LINE_STRIP, 0, 5);
    glDrawArrays(GL_LINE_STRIP, 5, 5);
    glDisableClientState(GL_VERTEX_ARRAY);
    line(0, 516, 222, 516, rgb(120, 72, 34), 3);                        // cabinet top edge (GL_LINES)

    // ---- green gear machine (the gear itself is a separate VBO object)
    rrect(272, 195, 610, 706, 28, GREEN);
    rrect(225, 520, 598, 706, 42, rgb(58, 130, 78));                    // arch
    rrect(252, 545, 598, 706, 30, rgb(12, 58, 40));                     // dark opening
    rrect(350, 442, 543, 500, 10, rgb(62, 120, 72));                    // control panel
    circle(396, 471, 15, rgb(232, 50, 50));
    circle(446, 471, 15, rgb(250, 165, 35));
    circle(497, 471, 15, rgb(252, 208, 30));

    // ---- shelf with teddy, toy train and ball
    rect(673, 195, 1094, 222, rgb(176, 108, 56));
    rect(673, 195, 1094, 201, rgb(196, 128, 70));
    rect(703, 222, 726, 252, rgb(176, 108, 56));
    rect(1040, 222, 1063, 252, rgb(176, 108, 56));
    Col tan = rgb(236, 196, 150), pink = rgb(216, 156, 110), dk = rgb(60, 36, 22);
    circle(712, 110, 16, BROWN); circle(779, 110, 16, BROWN);        // teddy
    circle(712, 110, 8, pink);   circle(779, 110, 8, pink);
    ellipse(745, 176, 50, 30, BROWN);
    circle(706, 178, 14, BROWN);  circle(784, 178, 14, BROWN);
    circle(716, 192, 16, BROWN);  circle(774, 192, 16, BROWN);
    ellipse(745, 180, 25, 18, tan);
    circle(745, 145, 46, BROWN);
    circle(745, 160, 21, tan);
    ellipse(745, 153, 8, 5, dk);
    circle(729, 136, 5, dk); circle(761, 136, 5, dk);
    color(dk);
    glLineWidth(std::max(1.0f, 2 * pixelScale));
    glBegin(GL_LINE_STRIP);
    glVertex2f(735,166);
    glVertex2f(745,172);
    glVertex2f(755,166);
    glEnd();
    rect(828, 135, 950, 172, rgb(232, 40, 40));                         // toy train
    rect(893, 112, 940, 138, rgb(30, 130, 60));
    rect(888, 102, 948, 116, rgb(252, 205, 30));
    rect(843, 108, 857, 137, rgb(252, 205, 30));
    rect(838, 103, 862, 112, rgb(252, 205, 30));
    float wx[3] = { 848, 890, 930 };
    for (int i = 0; i < 3; i++) {
        circle(wx[i], 182, 16, rgb(30, 120, 55), 20);
        circle(wx[i], 182, 11, rgb(252, 205, 30), 20);
        circle(wx[i], 182, 4, dk, 12);
    }
    circle(1026, 158, 40, rgb(230, 50, 45), 32);                        // beach ball
    sector(1026, 158, 40, 40, -105, -55, YELLOW, 8);
    sector(1026, 158, 40, 40, 75, 125, YELLOW, 8);

    // ---- window (frame, night sky panes, snow banks)
    rect(1250, 117, 1472, 289, rgb(140, 86, 46));
    Col sky = rgb(13, 27, 70);
    rect(1263, 130, 1364, 237, sky); rect(1372, 130, 1460, 237, sky);
    rect(1263, 247, 1364, 278, sky); rect(1372, 247, 1460, 278, sky);
    Col snow = rgb(240, 246, 250);
    color(snow);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1315.667f, 266.667f);
    glVertex2f(1263,278);
    glVertex2f(1263,264);
    glVertex2f(1300,254);
    glVertex2f(1340,258);
    glVertex2f(1364,268);
    glVertex2f(1364,278);
    glVertex2f(1263,278);
    glEnd();
    color(snow);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1419.000f, 266.333f);
    glVertex2f(1372,278);
    glVertex2f(1372,268);
    glVertex2f(1410,257);
    glVertex2f(1440,254);
    glVertex2f(1460,263);
    glVertex2f(1460,278);
    glVertex2f(1372,278);
    glEnd();
    float stars[][2] = { {1285,150},{1330,176},{1310,214},{1395,158},{1432,198},{1415,226},{1280,230},{1445,145} };
    for (int i = 0; i < 8; ++i) point(stars[i][0], stars[i][1], WHITE, 4);

    // ---- table + christmas tree
    rect(1273, 535, 1665, 567, rgb(176, 108, 56));
    rect(1300, 567, 1325, 706, rgb(176, 108, 56));
    rect(1598, 567, 1623, 706, rgb(176, 108, 56));
    rect(1325, 652, 1598, 678, WOODD);
    rect(1540, 495, 1582, 538, WOODD);
    tri(1455, 498, 1665, 498, 1560, 398, rgb(24, 122, 58));
    tri(1478, 424, 1642, 424, 1560, 326, rgb(30, 130, 62));
    tri(1500, 352, 1620, 352, 1560, 284, rgb(36, 138, 66));
    star5(1560, 270, 32, 14, rgb(252, 205, 30));
    circle(1572, 342, 14, rgb(252, 208, 30)); circle(1534, 393, 14, rgb(224, 40, 45));
    circle(1590, 405, 14, rgb(252, 208, 30)); circle(1523, 462, 14, rgb(252, 208, 30));
    circle(1595, 462, 14, rgb(224, 40, 45));

    // ---- conveyor belt
    Col belt = rgb(44, 62, 110);
    rrect(50, 712, 1622, 812, 50, belt);
    rrect(72, 730, 1600, 796, 33, rgb(32, 48, 90));
    hbar(100, 1572, 717, 724, rgb(86, 106, 160));                       // belt highlight (GL_QUAD_STRIP)
    for (int i = 0; i < 9; i++) {
        circle(97 + i * 184.f, 770, 23, rgb(200, 202, 208), 24);
        circle(97 + i * 184.f, 770, 7, rgb(160, 162, 170), 12);
    }
    rect(108, 812, 165, 877, NAVYD);
    rect(1507, 812, 1564, 877, NAVYD);
}

// window cross-bars (drawn AFTER the falling snow so the flakes pass behind them)
void drawWindowBars() {
    Col f = rgb(140, 86, 46);
    rect(1360, 130, 1372, 278, f);
    rect(1263, 236, 1460, 248, f);
}

// arm of the gear machine (drawn in front of the gear)
void drawArm() {
    rect(232, 285, 255, 520, GRAY);
    rect(255, 270, 328, 296, GRAY);
    rect(232, 505, 272, 522, GRAY);
    circle(242, 282, 26, GRAY);  circle(242, 282, 10, rgb(70, 72, 78), 16);
    circle(242, 460, 20, GRAY);  circle(242, 460, 8, rgb(70, 72, 78), 16);
}

// Box front, elf, string lights, and the bottom banner.
void drawForeground() {
    // surprise box front + ribbon
    rect(BOX_X0, BOX_TOP, BOX_X1, BOX_BOT, RED);
    rect(756, BOX_TOP, 812, BOX_BOT, YELLOW);

    // ---- the elf -----------------------------------------------------------
    Col green = rgb(25, 112, 52), hair = rgb(112, 62, 30), dk = rgb(40, 24, 14);
    Col skin2 = rgb(244, 172, 128);
    // body + arms
    color(green);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1132.250f, 617.500f);
    glVertex2f(1062,518);
    glVertex2f(1205,540);
    glVertex2f(1232,706);
    glVertex2f(1030,706);
    glVertex2f(1062,518);
    glEnd();
    thickLine(1205, 555, 1262, 690, 48, green);                        // resting arm
    thickLine(1252, 672, 1262, 690, 52, RED);
    circle(1262, 704, 26, SKIN);
    color(RED);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1133.833f, 545.333f);
    glVertex2f(1062,514);
    glVertex2f(1130,526);
    glVertex2f(1205,544);
    glVertex2f(1190,560);
    glVertex2f(1140,578);
    glVertex2f(1076,550);
    glVertex2f(1062,514);
    glEnd();   // collar
    circle(1121, 592, 10, YELLOW); circle(1117, 628, 10, YELLOW);
    rect(1034, 650, 1192, 696, rgb(24, 46, 34));                       // belt
    rect(1084, 648, 1142, 698, YELLOW);
    rect(1098, 662, 1128, 684, rgb(24, 46, 34));
    // raised arm holding a gift
    thickLine(1078, 566, 998, 470, 46, green);
    thickLine(1008, 488, 996, 472, 52, RED);
    circle(985, 452, 26, SKIN);
    rotRect(936, 382, 196, 72, 22, rgb(160, 24, 34));                  // gift (shadow)
    rotRect(936, 372, 196, 66, 22, REDL);
    rotRect(950, 368, 38, 72, 22, YELLOW);
    circle(925, 318, 25, YELLOW); circle(925, 318, 12, CREAM);
    circle(990, 336, 27, YELLOW); circle(990, 336, 13, CREAM);
    circle(958, 330, 13, YELLOW);
    // head
    tri(1040, 392, 1086, 416, 1084, 474, skin2);                       // ears
    tri(1280, 438, 1216, 428, 1218, 490, skin2);
    circle(1145, 458, 82, SKIN, 36);
    color(hair);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1168.000f, 445.000f);
    glVertex2f(1094,428);
    glVertex2f(1240,428);
    glVertex2f(1226,458);
    glVertex2f(1190,442);
    glVertex2f(1150,462);
    glVertex2f(1108,452);
    glVertex2f(1094,428);
    glEnd();   // fringe
    color(rgb(222, 36, 42));
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1208.571f, 330.000f);
    glVertex2f(1088,376);
    glVertex2f(1130,322);
    glVertex2f(1190,288);
    glVertex2f(1240,282);
    glVertex2f(1272,312);
    glVertex2f(1292,352);
    glVertex2f(1248,378);
    glVertex2f(1088,376);
    glEnd();
    color(WHITE);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1168.500f, 399.000f);
    glVertex2f(1082,372);
    glVertex2f(1252,372);
    glVertex2f(1250,420);
    glVertex2f(1090,432);
    glVertex2f(1082,372);
    glEnd();   // hat band
    circle(1288, 352, 25, WHITE);                                      // pom-pom
    circle(1108, 440, 21, WHITE); circle(1172, 450, 21, WHITE);     // eyes
    circle(1110, 442, 11, dk, 16); circle(1170, 452, 11, dk, 16);
    circle(1114, 437, 4, WHITE, 10); circle(1174, 447, 4, WHITE, 10);
    circle(1138, 464, 13, rgb(230, 40, 40), 16);                       // nose
    color(dk);
    glLineWidth(std::max(1.0f, 4 * pixelScale));
    glBegin(GL_LINE_STRIP);
    glVertex2f(1108,484);
    glVertex2f(1120,494);
    glVertex2f(1138,498);
    glVertex2f(1156,494);
    glVertex2f(1170,486);
    glEnd();   // smile

    // ---- string lights (top corners)
    Col wire = rgb(40, 50, 50);
    color(wire);
    glLineWidth(std::max(1.0f, 3 * pixelScale));
    glBegin(GL_LINE_STRIP);
    glVertex2f(0,22);
    glVertex2f(60,44);
    glVertex2f(110,60);
    glVertex2f(152,64);
    glVertex2f(200,56);
    glVertex2f(250,34);
    glVertex2f(300,-4);
    glEnd();
    color(wire);
    glLineWidth(std::max(1.0f, 3 * pixelScale));
    glBegin(GL_LINE_STRIP);
    glVertex2f(1672,22);
    glVertex2f(1612,44);
    glVertex2f(1562,60);
    glVertex2f(1520,64);
    glVertex2f(1470,56);
    glVertex2f(1420,34);
    glVertex2f(1372,-4);
    glEnd();
    float bx[6] = { 65, 152, 232, 1433, 1520, 1608 };
    float by[6] = { 66, 82, 62, 62, 82, 64 };
    Col bc[6] = { rgb(220, 40, 40), rgb(252, 205, 30), rgb(34, 150, 56),
                  rgb(34, 150, 56), rgb(220, 40, 40), rgb(252, 205, 30) };
    for (int i = 0; i < 6; i++) {
        rect(bx[i] - 6, by[i] - 28, bx[i] + 6, by[i] - 12, rgb(60, 66, 66));
        ellipse(bx[i], by[i], 17, 20, bc[i], 20);
        circle(bx[i] - 6, by[i] - 6, 4, WHITE, 8);
    }

    // ---- bottom banner (text is drawn on top with stroke fonts)
    rect(0, 877, SCENE_W, SCENE_H, NAVY);
}

void drawRobot() {
    Col yel = rgb(253, 214, 30), navy = rgb(30, 45, 80);
    rect(780, 395, 787, 428, navy);                                    // antenna
    circle(783, 381, 15, yel, 20);
    rect(705, 447, 726, 487, yel);                                     // ears
    rect(842, 447, 862, 487, yel);
    rect(693, 528, 729, 596, TEAL);                                    // arms
    rect(838, 528, 876, 596, TEAL);
    rect(693, 576, 729, 598, yel);
    rect(838, 576, 876, 598, yel);
    rrect(728, 520, 838, 610, 8, TEAL);                                // torso
    rrect(750, 538, 816, 592, 6, yel);
    rect(757, 500, 810, 524, yel);                                     // neck
    rrect(724, 425, 843, 502, 12, TEAL);                               // head
    circle(757, 458, 21, WHITE, 20); circle(810, 458, 21, WHITE, 20);
    circle(759, 460, 11, navy, 14);  circle(808, 460, 11, navy, 14);
    circle(763, 454, 4, WHITE, 8);   circle(812, 454, 4, WHITE, 8);
    color(navy);
    glLineWidth(std::max(1.0f, 3 * pixelScale));
    glBegin(GL_LINE_STRIP);
    glVertex2f(764,480);
    glVertex2f(772,488);
    glVertex2f(783,491);
    glVertex2f(794,488);
    glVertex2f(802,480);
    glEnd();
}

// Module 5/6: hardcoded positions and matching RGB colors for each gift.
const GLfloat giftVertices[3][48] = {
    {
        -95.00f, -126.00f, 0.00f, 95.00f, -126.00f, 0.00f, 95.00f, 0.00f, 0.00f, -95.00f, 0.00f, 0.00f,
        -101.00f, -126.00f, 0.00f, 101.00f, -126.00f, 0.00f, 101.00f, -102.00f, 0.00f, -101.00f, -102.00f, 0.00f,
        -95.00f, -65.52f, 0.00f, 95.00f, -65.52f, 0.00f, 95.00f, -35.28f, 0.00f, -95.00f, -35.28f, 0.00f,
        -18.05f, -126.00f, 0.00f, 18.05f, -126.00f, 0.00f, 18.05f, 0.00f, 0.00f, -18.05f, 0.00f, 0.00f,
    },
    {
        -75.00f, -112.00f, 0.00f, 75.00f, -112.00f, 0.00f, 75.00f, 0.00f, 0.00f, -75.00f, 0.00f, 0.00f,
        -81.00f, -112.00f, 0.00f, 81.00f, -112.00f, 0.00f, 81.00f, -88.00f, 0.00f, -81.00f, -88.00f, 0.00f,
        -75.00f, -58.24f, 0.00f, 75.00f, -58.24f, 0.00f, 75.00f, -31.36f, 0.00f, -75.00f, -31.36f, 0.00f,
        -14.25f, -112.00f, 0.00f, 14.25f, -112.00f, 0.00f, 14.25f, 0.00f, 0.00f, -14.25f, 0.00f, 0.00f,
    },
    {
        -85.00f, -100.00f, 0.00f, 85.00f, -100.00f, 0.00f, 85.00f, 0.00f, 0.00f, -85.00f, 0.00f, 0.00f,
        -91.00f, -100.00f, 0.00f, 91.00f, -100.00f, 0.00f, 91.00f, -76.00f, 0.00f, -91.00f, -76.00f, 0.00f,
        -85.00f, -52.00f, 0.00f, 85.00f, -52.00f, 0.00f, 85.00f, -28.00f, 0.00f, -85.00f, -28.00f, 0.00f,
        -16.15f, -100.00f, 0.00f, 16.15f, -100.00f, 0.00f, 16.15f, 0.00f, 0.00f, -16.15f, 0.00f, 0.00f,
    },
};

const GLfloat giftColors[3][48] = {
    {
        0.1922f, 0.5569f, 0.2588f, 0.1922f, 0.5569f, 0.2588f, 0.1922f, 0.5569f, 0.2588f, 0.1922f, 0.5569f, 0.2588f,
        0.1576f, 0.4566f, 0.2122f, 0.1576f, 0.4566f, 0.2122f, 0.1576f, 0.4566f, 0.2122f, 0.1576f, 0.4566f, 0.2122f,
        0.9333f, 0.2510f, 0.2824f, 0.9333f, 0.2510f, 0.2824f, 0.9333f, 0.2510f, 0.2824f, 0.9333f, 0.2510f, 0.2824f,
        0.9333f, 0.2510f, 0.2824f, 0.9333f, 0.2510f, 0.2824f, 0.9333f, 0.2510f, 0.2824f, 0.9333f, 0.2510f, 0.2824f,
    },
    {
        0.2275f, 0.4314f, 0.8039f, 0.2275f, 0.4314f, 0.8039f, 0.2275f, 0.4314f, 0.8039f, 0.2275f, 0.4314f, 0.8039f,
        0.1865f, 0.3537f, 0.6592f, 0.1865f, 0.3537f, 0.6592f, 0.1865f, 0.3537f, 0.6592f, 0.1865f, 0.3537f, 0.6592f,
        0.9922f, 0.8039f, 0.1569f, 0.9922f, 0.8039f, 0.1569f, 0.9922f, 0.8039f, 0.1569f, 0.9922f, 0.8039f, 0.1569f,
        0.9922f, 0.8039f, 0.1569f, 0.9922f, 0.8039f, 0.1569f, 0.9922f, 0.8039f, 0.1569f, 0.9922f, 0.8039f, 0.1569f,
    },
    {
        0.5490f, 0.2824f, 0.6980f, 0.5490f, 0.2824f, 0.6980f, 0.5490f, 0.2824f, 0.6980f, 0.5490f, 0.2824f, 0.6980f,
        0.4502f, 0.2315f, 0.5724f, 0.4502f, 0.2315f, 0.5724f, 0.4502f, 0.2315f, 0.5724f, 0.4502f, 0.2315f, 0.5724f,
        0.9804f, 0.7451f, 0.2353f, 0.9804f, 0.7451f, 0.2353f, 0.9804f, 0.7451f, 0.2353f, 0.9804f, 0.7451f, 0.2353f,
        0.9804f, 0.7451f, 0.2353f, 0.9804f, 0.7451f, 0.2353f, 0.9804f, 0.7451f, 0.2353f, 0.9804f, 0.7451f, 0.2353f,
    },
};

// Twelve separate teeth, four fixed vertices per tooth.
const GLfloat gearTeeth[] = {
    97.815f, -20.791f, 0.000f, 97.815f, 20.791f, 0.000f, 147.550f, 20.737f, 0.000f, 147.550f, -20.737f, 0.000f,
    95.106f, 30.902f, 0.000f, 74.314f, 66.913f, 0.000f, 117.414f, 91.734f, 0.000f, 138.150f, 55.816f, 0.000f,
    66.913f, 74.314f, 0.000f, 30.902f, 95.106f, 0.000f, 55.816f, 138.150f, 0.000f, 91.734f, 117.414f, 0.000f,
    20.791f, 97.815f, 0.000f, -20.791f, 97.815f, 0.000f, -20.737f, 147.550f, 0.000f, 20.737f, 147.550f, 0.000f,
    -30.902f, 95.106f, 0.000f, -66.913f, 74.314f, 0.000f, -91.734f, 117.414f, 0.000f, -55.816f, 138.150f, 0.000f,
    -74.314f, 66.913f, 0.000f, -95.106f, 30.902f, 0.000f, -138.150f, 55.816f, 0.000f, -117.414f, 91.734f, 0.000f,
    -97.815f, 20.791f, 0.000f, -97.815f, -20.791f, 0.000f, -147.550f, -20.737f, 0.000f, -147.550f, 20.737f, 0.000f,
    -95.106f, -30.902f, 0.000f, -74.314f, -66.913f, 0.000f, -117.414f, -91.734f, 0.000f, -138.150f, -55.816f, 0.000f,
    -66.913f, -74.314f, 0.000f, -30.902f, -95.106f, 0.000f, -55.816f, -138.150f, 0.000f, -91.734f, -117.414f, 0.000f,
    -20.791f, -97.815f, 0.000f, 20.791f, -97.815f, 0.000f, 20.737f, -147.550f, 0.000f, -20.737f, -147.550f, 0.000f,
    30.902f, -95.106f, 0.000f, 66.913f, -74.314f, 0.000f, 91.734f, -117.414f, 0.000f, 55.816f, -138.150f, 0.000f,
    74.314f, -66.913f, 0.000f, 95.106f, -30.902f, 0.000f, 138.150f, -55.816f, 0.000f, 117.414f, -91.734f, 0.000f,
};

// The 24 snowflakes start at fixed positions inside the window.
const GLfloat initialSnow[] = {
    1280,143,0, 1305,155,0, 1332,140,0, 1350,168,0,
    1384,146,0, 1410,160,0, 1435,139,0, 1450,183,0,
    1275,192,0, 1295,217,0, 1320,190,0, 1340,232,0,
    1378,205,0, 1400,229,0, 1425,198,0, 1445,245,0,
    1285,254,0, 1310,267,0, 1337,250,0, 1355,205,0,
    1389,266,0, 1418,251,0, 1440,220,0, 1452,267,0
};
const float snowSpeed[24] = {
    .5f,.7f,.4f,.8f,.6f,.5f,.9f,.4f, .7f,.5f,.8f,.6f,
    .4f,.9f,.5f,.7f,.6f,.4f,.8f,.5f, .7f,.6f,.4f,.8f
};

// Each cube face has its own four vertices: corners need different normals
// and texture coordinates on adjacent faces. Indices select these vertices.
const GLfloat toyVertices[] = {
    -1,-1, 1,  1,-1, 1,  1, 1, 1, -1, 1, 1, // front
     1,-1,-1, -1,-1,-1, -1, 1,-1,  1, 1,-1, // back
    -1,-1,-1, -1,-1, 1, -1, 1, 1, -1, 1,-1, // left
     1,-1, 1,  1,-1,-1,  1, 1,-1,  1, 1, 1, // right
    -1, 1, 1,  1, 1, 1,  1, 1,-1, -1, 1,-1, // top
    -1,-1,-1,  1,-1,-1,  1,-1, 1, -1,-1, 1  // bottom
};
const GLfloat toyNormals[] = {
     0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,
     0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,
    -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
     1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,
     0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,
     0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0
};
const GLfloat toyTexCoords[] = {
    0,0, 1,0, 1,1, 0,1,   0,0, 1,0, 1,1, 0,1,
    0,0, 1,0, 1,1, 0,1,   0,0, 1,0, 1,1, 0,1,
    0,0, 1,0, 1,1, 0,1,   0,0, 1,0, 1,1, 0,1
};
const GLubyte toyIndices[] = {
     0, 1, 2,  0, 2, 3,   4, 5, 6,  4, 6, 7,
     8, 9,10,  8,10,11,  12,13,14, 12,14,15,
    16,17,18, 16,18,19,  20,21,22, 20,22,23
};

// Module 8: an actual image, stored as RGB pixels in the source.
// Gold stripes cross red wrapping paper. Eight is a power of two.
const GLubyte wrappingPixels[8][8][3] = {
    {{190,32,46},{222,49,60},{190,32,46},{250,205,65},{250,205,65},{222,49,60},{190,32,46},{222,49,60}},
    {{222,49,60},{190,32,46},{222,49,60},{250,205,65},{250,205,65},{190,32,46},{222,49,60},{190,32,46}},
    {{190,32,46},{222,49,60},{190,32,46},{250,205,65},{250,205,65},{222,49,60},{190,32,46},{222,49,60}},
    {{250,205,65},{250,205,65},{250,205,65},{255,226,95},{255,226,95},{250,205,65},{250,205,65},{250,205,65}},
    {{250,205,65},{250,205,65},{250,205,65},{255,226,95},{255,226,95},{250,205,65},{250,205,65},{250,205,65}},
    {{222,49,60},{190,32,46},{222,49,60},{250,205,65},{250,205,65},{190,32,46},{222,49,60},{190,32,46}},
    {{190,32,46},{222,49,60},{190,32,46},{250,205,65},{250,205,65},{222,49,60},{190,32,46},{222,49,60}},
    {{222,49,60},{190,32,46},{222,49,60},{250,205,65},{250,205,65},{190,32,46},{222,49,60},{190,32,46}}
};

// Module 6: ordinary row-major matrix multiplication, C = A * B.
void multiplyMatrices(const GLfloat a[4][4], const GLfloat b[4][4], GLfloat c[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            c[row][column] = 0;
            for (int k = 0; k < 4; ++k)
                c[row][column] += a[row][k] * b[k][column];
        }
    }
}

void matrixExamples() {
    // Arithmetic examples from Module 6, pp. 7-10, printed once on startup.
    const GLfloat a[2][2] = {{1,2},{3,4}};
    const GLfloat b[2][2] = {{5,6},{7,8}};
    GLfloat sum[2][2], difference[2][2], scaled[2][2];
    for (int row = 0; row < 2; ++row) {
        for (int column = 0; column < 2; ++column) {
            sum[row][column] = a[row][column] + b[row][column];
            difference[row][column] = a[row][column] - b[row][column];
            scaled[row][column] = 2 * a[row][column];
        }
    }
    std::printf("Matrix addition:    [%.0f %.0f; %.0f %.0f]\n",
                sum[0][0], sum[0][1], sum[1][0], sum[1][1]);
    std::printf("Matrix subtraction: [%.0f %.0f; %.0f %.0f]\n",
                difference[0][0], difference[0][1], difference[1][0], difference[1][1]);
    std::printf("Scalar times matrix:[%.0f %.0f; %.0f %.0f]\n",
                scaled[0][0], scaled[0][1], scaled[1][0], scaled[1][1]);

    const GLfloat translation[4][4] = {
        {1,0,0,1}, {0,1,0,2}, {0,0,1,3}, {0,0,0,1}
    };
    const GLfloat scaling[4][4] = {
        {2,0,0,0}, {0,2,0,0}, {0,0,2,0}, {0,0,0,1}
    };
    // A 90-degree rotation around Z: x becomes -y, y becomes x.
    const GLfloat rotation[4][4] = {
        {0,-1,0,0}, {1,0,0,0}, {0,0,1,0}, {0,0,0,1}
    };
    GLfloat rotatedScale[4][4], combined[4][4];
    multiplyMatrices(rotation, scaling, rotatedScale);
    multiplyMatrices(translation, rotatedScale, combined); // T * R * S
    const GLfloat position[4] = {1,1,1,1}; // w=1: affected by translation
    const GLfloat direction[4] = {1,1,1,0}; // w=0: unaffected by translation
    GLfloat transformedPosition[4] = {}, transformedDirection[4] = {};
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            transformedPosition[row] += combined[row][column] * position[column];
            transformedDirection[row] += combined[row][column] * direction[column];
        }
    }
    std::printf("T*R*S position: (%.0f, %.0f, %.0f, %.0f)\n",
                transformedPosition[0], transformedPosition[1],
                transformedPosition[2], transformedPosition[3]);
    std::printf("T*R*S direction: (%.0f, %.0f, %.0f, %.0f)\n",
                transformedDirection[0], transformedDirection[1],
                transformedDirection[2], transformedDirection[3]);
}

// The same 4x4 times 4x1 operation moves the actual snow vertices.
// This is row-major CPU math, not an array passed to OpenGL.
void translateSnowPoint(GLfloat* vertex, float x, float y, float z) {
    const GLfloat translation[4][4] = {
        {1,0,0,x}, {0,1,0,y}, {0,0,1,z}, {0,0,0,1}
    };
    const GLfloat position[4] = {vertex[0], vertex[1], vertex[2], 1};
    GLfloat result[4] = {};
    for (int row = 0; row < 4; ++row)
        for (int column = 0; column < 4; ++column)
            result[row] += translation[row][column] * position[column];
    vertex[0] = result[0];
    vertex[1] = result[1];
    vertex[2] = result[2];
}

void initBuffers() {
    // Allocate once. Position data and color data occupy different byte ranges.
    glGenBuffers(3, giftVBO);
    for (int i = 0; i < 3; ++i) {
        glBindBuffer(GL_ARRAY_BUFFER, giftVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(giftVertices[i]) + sizeof(giftColors[i]),
                     NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(giftVertices[i]), giftVertices[i]);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(giftVertices[i]), sizeof(giftColors[i]), giftColors[i]);
    }

    glGenBuffers(1, &gearVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gearVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gearTeeth), gearTeeth, GL_STATIC_DRAW);

    glGenBuffers(1, &snowVBO);
    glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(initialSnow), initialSnow, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &toyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, toyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(toyVertices) + sizeof(toyNormals) + sizeof(toyTexCoords),
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(toyVertices), toyVertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(toyVertices), sizeof(toyNormals), toyNormals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(toyVertices) + sizeof(toyNormals),
                    sizeof(toyTexCoords), toyTexCoords);

    // Element indices and normal vectors are different data, in different arrays.
    glGenBuffers(1, &toyEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, toyEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(toyIndices), toyIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initTexture() {
    glGenTextures(1, &wrappingTexture);
    glBindTexture(GL_TEXTURE_2D, wrappingTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, wrappingPixels);
    if (GLEW_VERSION_3_0 || GLEW_ARB_framebuffer_object) {
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        // GLU supplies the same mip levels on older compatibility contexts.
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 8, 8, GL_RGB, GL_UNSIGNED_BYTE, wrappingPixels);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawGift(int i) {
    glBindBuffer(GL_ARRAY_BUFFER, giftVBO[i]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glColorPointer(3, GL_FLOAT, 0, reinterpret_cast<const void*>(sizeof(giftVertices[i])));
    // The count is 16 vertices, not the number of floats or bytes.
    glDrawArrays(GL_QUADS, 0, 16);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const float bowX[3] = {24.7f, 19.5f, 22.1f};
    const float bowY[3] = {-142, -128, -116};
    const Col ribbons[3] = {rgb(238,64,72), rgb(253,205,40), rgb(250,190,60)};
    const Col inner[3] = {rgb(167,45,50), rgb(177,144,28), rgb(175,133,42)};
    circle(-bowX[i], bowY[i], 23, ribbons[i], 20);
    circle( bowX[i], bowY[i], 23, ribbons[i], 20);
    circle(-bowX[i], bowY[i], 11, inner[i], 14);
    circle( bowX[i], bowY[i], 11, inner[i], 14);
    circle(0, bowY[i] + 10, 11, ribbons[i], 14);
}

void drawGear() {
    color(rgb(252,190,22));
    glBindBuffer(GL_ARRAY_BUFFER, gearVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glDrawArrays(GL_QUADS, 0, 48);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    circle(0, 0, 118, rgb(252,190,22), 48);
    circle(0, 0, 42, NAVYD, 28);
    color(rgb(214,150,10));
    glLineWidth(std::max(1.0f, 3 * pixelScale));
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 40; ++i) {
        float a = 2 * PI * i / 40;
        glVertex2f(78 * std::cos(a), 78 * std::sin(a));
    }
    glEnd();
    rect(-23, -166, 24, -98, rgb(226,36,46));
}

void drawFlap() {
    rect(0, -34, 124, 0, rgb(236,74,80));
    rect(0, 0, 124, 8, rgb(170,24,34));
    rect(96, -34, 124, 8, YELLOW);
}

void drawSnow() {
    color(WHITE);
    glPointSize(std::max(1.0f, 4 * pixelScale));
    glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glDrawArrays(GL_POINTS, 0, 24);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool updateSnow() {
    glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
    GLfloat* mapped = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));
    if (!mapped) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        std::fprintf(stderr, "Could not map the snow buffer.\n");
        return false;
    }
    for (int i = 0; i < 24; ++i) {
        translateSnowPoint(mapped + i * 3, 0, snowSpeed[i], 0);
        if (mapped[i * 3 + 1] > 272) mapped[i * 3 + 1] = 133;
    }
    GLboolean valid = glUnmapBuffer(GL_ARRAY_BUFFER);
    if (!valid) {
        // An unsuccessful unmap invalidates the buffer; restore known data.
        glBufferData(GL_ARRAY_BUFFER, sizeof(initialSnow), initialSnow, GL_DYNAMIC_DRAW);
        std::fprintf(stderr, "Snow buffer restored after an unsuccessful unmap.\n");
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return valid == GL_TRUE;
}

void drawToyCube() {
    glBindBuffer(GL_ARRAY_BUFFER, toyVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, toyEBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glNormalPointer(GL_FLOAT, 0, reinterpret_cast<const void*>(sizeof(toyVertices)));
    glTexCoordPointer(2, GL_FLOAT, 0,
                     reinterpret_cast<const void*>(sizeof(toyVertices) + sizeof(toyNormals)));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawTableToy() {
    // A small 3D display on the table. Its camera is separate from the 2D room.
    int x = viewX + static_cast<int>(1270 * pixelScale);
    int y = viewY + static_cast<int>((SCENE_H - 600) * pixelScale);
    int w = std::max(1, static_cast<int>(205 * pixelScale));
    int h = std::max(1, static_cast<int>(215 * pixelScale));
    glViewport(x, y, w, h);
    float aspect = static_cast<float>(w) / h;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    if (projectionMode == 0) {
        gluPerspective(42, aspect, 1, 30);
    } else if (projectionMode == 1) {
        glOrtho(-2.4 * aspect, 2.4 * aspect, -2.4, 2.4, 1, 30);
    } else {
        // Same field of view, with a small horizontal lens shift.
        double top = std::tan(21.0 * PI / 180.0);
        glFrustum(-top * aspect + .06, top * aspect + .06, -top, top, 1, 30);
    }
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(3.8, 3.0, 5.8, 0, 0, 0, 0, 1, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE); // keep transformed normals at unit length

    // Set lights after the view matrix, before moving individual objects.
    const GLfloat globalAmbient[] = {.20f,.20f,.23f,1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    const GLfloat keyDirection[] = {-2,3,4,0}; // w=0 means directional
    const GLfloat keyAmbient[] = {.08f,.07f,.06f,1};
    const GLfloat keyDiffuse[] = {.78f,.73f,.65f,1};
    const GLfloat keySpecular[] = {.8f,.8f,.8f,1};
    glLightfv(GL_LIGHT0, GL_POSITION, keyDirection);
    glLightfv(GL_LIGHT0, GL_AMBIENT, keyAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, keyDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, keySpecular);

    const GLfloat pointPosition[] = {3,1,2,1}; // w=1 means a local position
    const GLfloat pointDiffuse[] = {.22f,.38f,.65f,1};
    glLightfv(GL_LIGHT1, GL_POSITION, pointPosition);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, pointDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, pointDiffuse);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, .04f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, .01f);

    const GLfloat spotPosition[] = {0,5,2,1};
    const GLfloat spotDirection[] = {0,-5,-2};
    const GLfloat spotDiffuse[] = {.75f,.30f,.15f,1};
    glLightfv(GL_LIGHT2, GL_POSITION, spotPosition);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spotDiffuse);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10);

    if (directionalOn) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
    if (pointOn) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);
    if (spotOn) glEnable(GL_LIGHT2); else glDisable(GL_LIGHT2);
    if (lightingOn) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);

    const GLfloat materialAmbient[] = {.5f,.5f,.5f,1};
    const GLfloat materialDiffuse[] = {1,1,1,1};
    const GLfloat materialSpecular[] = {.7f,.7f,.7f,1};
    const GLfloat noEmission[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60);

    glPushMatrix();
    glRotatef(toyAngle, 0, 1, 0);
    glScalef(.8f, .8f, .8f);
    glColor3f(1,1,1);
    if (textureOn) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, wrappingTexture);
    }
    drawToyCube();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // A glowing gold ornament on the gift demonstrates material emission
    // and GLUT's solid geometry, including its automatically supplied normals.
    const GLfloat gold[] = {1,.72f,.12f,1};
    const GLfloat goldGlow[] = {.16f,.08f,.01f,1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gold);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, goldGlow);
    glColor3f(1,.72f,.12f);
    glPushMatrix();
    glTranslatef(0,1.05f,0);
    glutSolidSphere(.25, 20, 16);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0); glDisable(GL_LIGHT1); glDisable(GL_LIGHT2);
    glDisable(GL_NORMALIZE);
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glViewport(viewX, viewY, viewWidth, viewHeight);
}

void strokeText(const char* text, float cx, float baseline, float width, Col c, float lineWidth) {
    float length = static_cast<float>(glutStrokeLength(GLUT_STROKE_ROMAN,
                                         reinterpret_cast<const unsigned char*>(text)));
    if (length == 0) return;
    float scale = width / length;
    color(c);
    glLineWidth(std::max(1.0f, lineWidth * pixelScale));
    glPushMatrix();
    glTranslatef(cx - width / 2, baseline, 0);
    glScalef(scale, -scale, 1);
    for (const char* p = text; *p; ++p) glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}

void drawText() {
    strokeText("SANTA'S SURPRISE WORKSHOP", 836, 66, 1023, NAVY, 5);
    strokeText("AUTO: MOVE GIFTS", 285, 908, 343, WHITE, 3);
    strokeText("CLICK: TURN GEAR + OPEN", 835, 908, 465, WHITE, 3);
    strokeText("SCROLL: RESIZE TOY", 1387, 908, 370, WHITE, 3);
    char toyStatus[128];
    const char* projections[] = {"PERSPECTIVE", "ORTHOGRAPHIC", "FRUSTUM"};
    std::snprintf(toyStatus, sizeof(toyStatus), "L LIGHT:%s  T WRAP:%s  P %s  1/2/3 LIGHTS  F WINDOW  R RESET  ESC EXIT",
                  lightingOn ? "ON" : "OFF", textureOn ? "ON" : "OFF", projections[projectionMode]);
    strokeText(toyStatus, 836, 936, 1200, WHITE, 1.5f);
}

void display() {
    glClearColor(NAVY.r, NAVY.g, NAVY.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    drawBackdrop();
    drawSnow();
    drawWindowBars();

    // Module 7: rotate around the gear's local origin.
    glPushMatrix();
    glTranslatef(445,278,0);
    glRotatef(gearAngle,0,0,1);
    drawGear();
    glPopMatrix();
    drawArm();

    // Translate the three gifts on the conveyor, clipping the machine opening.
    const GLdouble giftPlane[] = {1,0,0,-252};
    glClipPlane(GL_CLIP_PLANE0, giftPlane);
    glEnable(GL_CLIP_PLANE0);
    for (int i = 0; i < 3; ++i) {
        float x = 150 + std::fmod(beltOffset + i * 1630.0f / 3, 1630.0f);
        glPushMatrix();
        glTranslatef(x,733,0);
        drawGift(i);
        glPopMatrix();
    }
    glDisable(GL_CLIP_PLANE0);

    glPushMatrix();
    glTranslatef(660,612,0);
    glRotatef(-118 * lidAmount,0,0,1);
    drawFlap();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(908,612,0);
    glRotatef(118 * lidAmount,0,0,1);
    glScalef(-1,1,1); // mirror the right flap
    drawFlap();
    glPopMatrix();

    const GLdouble robotPlane[] = {0,1,0,-(612 - lidAmount * 700.0)};
    glClipPlane(GL_CLIP_PLANE1, robotPlane);
    if (lidAmount < .999f) glEnable(GL_CLIP_PLANE1);
    glPushMatrix();
    // Applied right-to-left: move the feet to zero, scale, move them back.
    glTranslatef(783,612,0);
    glScalef(robotScale,robotScale,1);
    glTranslatef(-783,-612,0);
    drawRobot();
    glPopMatrix();
    glDisable(GL_CLIP_PLANE1);

    ellipse(1372, 536, 42, 6, rgb(133,78,43));
    drawTableToy();
    drawForeground();

    // Module 6: glArrayElement with an explicit CPU vertex/color array.
    // A small colored triangle decorates the front of the gift box.
    const GLfloat badgeVertices[] = {775,663,0, 793,663,0, 784,644,0};
    const GLfloat badgeColors[] = {1,.35f,.15f, 1,.85f,.2f, 1,1,.8f};
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, badgeVertices);
    glColorPointer(3, GL_FLOAT, 0, badgeColors);
    glBegin(GL_TRIANGLES);
    glArrayElement(0); glArrayElement(1); glArrayElement(2);
    glEnd();
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    drawText();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    windowWidth = std::max(1, width);
    windowHeight = std::max(1, height);
    float aspect = SCENE_W / SCENE_H;
    viewWidth = windowWidth;
    viewHeight = windowHeight;
    if (static_cast<float>(windowWidth) / windowHeight > aspect)
        viewWidth = std::max(1, static_cast<int>(windowHeight * aspect));
    else
        viewHeight = std::max(1, static_cast<int>(windowWidth / aspect));
    viewX = (windowWidth - viewWidth) / 2;
    viewY = (windowHeight - viewHeight) / 2;
    pixelScale = viewWidth / SCENE_W;
    glViewport(viewX, viewY, viewWidth, viewHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Module 7: orthographic camera for the room and text.
    gluOrtho2D(0, SCENE_W, SCENE_H, 0);
    glMatrixMode(GL_MODELVIEW);
}

void resetAll() {
    beltOffset = 0; beltPaused = false;
    gearAngle = gearTarget = 0;
    lidAmount = lidTarget = 1;
    robotScale = 1; toyAngle = 25;
    lightingOn = textureOn = true;
    directionalOn = pointOn = spotOn = true;
    projectionMode = 0;
    glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(initialSnow), initialSnow);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cleanup() {
    glDeleteBuffers(3, giftVBO);
    glDeleteBuffers(1, &gearVBO);
    glDeleteBuffers(1, &snowVBO);
    glDeleteBuffers(1, &toyVBO);
    glDeleteBuffers(1, &toyEBO);
    glDeleteTextures(1, &wrappingTexture);
    giftVBO[0] = giftVBO[1] = giftVBO[2] = 0;
    gearVBO = snowVBO = toyVBO = toyEBO = wrappingTexture = 0;
}

void keyboard(unsigned char key, int, int) {
    if (key == 27 || key == 'x' || key == 'X') {
        // FreeGLUT invokes the close callback while the GL context still exists.
        glutLeaveMainLoop();
        return;
    }
    if (key == ' ') beltPaused = !beltPaused;
    if (key == 'r' || key == 'R') resetAll();
    if (key == 'l' || key == 'L') lightingOn = !lightingOn;
    if (key == 't' || key == 'T') textureOn = !textureOn;
    if (key == 'p' || key == 'P') projectionMode = (projectionMode + 1) % 3;
    if (key == '1') directionalOn = !directionalOn;
    if (key == '2') pointOn = !pointOn;
    if (key == '3') spotOn = !spotOn;
    if (key == 'f' || key == 'F') {
        fullscreen = !fullscreen;
        if (fullscreen) glutFullScreen();
        else {
            glutReshapeWindow(1280,720);
            glutPositionWindow(40,40);
        }
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int, int) {
    if (state != GLUT_DOWN) return;
    if (button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON) {
        gearTarget += 90;
        lidTarget = 1 - lidTarget;
    }
    if (button == GLUT_RIGHT_BUTTON) {
        gearTarget -= 90;
        lidTarget = 1 - lidTarget;
    }
    glutPostRedisplay();
}

void mouseWheel(int, int direction, int, int) {
    if (direction > 0) robotScale = std::min(1.8f, robotScale * 1.08f);
    if (direction < 0) robotScale = std::max(.4f, robotScale / 1.08f);
    glutPostRedisplay();
}

void beltTimer(int) {
    if (!beltPaused) beltOffset = std::fmod(beltOffset + 4, 1630.0f);
    glutPostRedisplay();
    glutTimerFunc(30, beltTimer, 0);
}

void effectsTimer(int) {
    float delta = gearTarget - gearAngle;
    if (std::fabs(delta) <= 2.5f) gearAngle = gearTarget;
    else gearAngle += (delta > 0 ? 2.5f : -2.5f);
    float lidDelta = lidTarget - lidAmount;
    if (std::fabs(lidDelta) <= .04f) lidAmount = lidTarget;
    else lidAmount += (lidDelta > 0 ? .04f : -.04f);
    toyAngle = std::fmod(toyAngle + .35f, 360.0f);
    updateSnow();
    glutPostRedisplay();
    glutTimerFunc(16, effectsTimer, 0);
}

// Module 6: only enable this callback when the context supports debug output.
void GLAPIENTRY MessageCallback(GLenum, GLenum type, GLuint, GLenum,
                                GLsizei, const GLchar* message, const void*) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        ++glErrorCount;
        std::fprintf(stderr, "OpenGL error: %s\n", message);
    }
}

bool initialize() {
    GLenum result = glewInit();
    if (result != GLEW_OK) {
        std::fprintf(stderr, "GLEW initialization failed: %s\n", glewGetErrorString(result));
        return false;
    }
    if (!GLEW_VERSION_1_5) {
        std::fprintf(stderr, "This workshop needs OpenGL 1.5 or newer for vertex buffers.\n");
        return false;
    }
    if ((GLEW_VERSION_4_3 || GLEW_KHR_debug) && glDebugMessageCallback) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, NULL);
    }
    GLint maxLights = 0;
    glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
    std::printf("OpenGL: %s\nVendor: %s\nAvailable lights: %d\n",
                glGetString(GL_VERSION), glGetString(GL_VENDOR), maxLights);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    initBuffers();
    initTexture();
    matrixExamples();
    reshape(windowWidth, windowHeight);
    return true;
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i)
        if (std::strcmp(argv[i], "--windowed") == 0) fullscreen = false;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1280,720);
    glutInitWindowPosition(40,40);
    int window = glutCreateWindow("Santa's Surprise Workshop");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    if (!initialize()) {
        glutDestroyWindow(window);
        return 1;
    }
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMouseWheelFunc(mouseWheel);
    glutCloseFunc(cleanup);
    if (fullscreen) glutFullScreen();
    // Schedule each timer once; each callback schedules its next invocation.
    glutTimerFunc(30, beltTimer, 0);
    glutTimerFunc(16, effectsTimer, 0);
    glutMainLoop();
    return glErrorCount == 0 ? 0 : 1;
}

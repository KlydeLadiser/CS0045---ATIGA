// Module 5 machine problem. C++17, GLEW, FreeGLUT, compatibility OpenGL.
// Input: focus this graphics window, type a choice, then press Enter.
#ifndef NOMINMAX
#define NOMINMAX // Keep Windows macros from overriding std::min/max.
#endif
#include <GL/glew.h>

#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const int WindowWidth = 1000;
const int WindowHeight = 700;
string Input;
string Message;
bool Closing = false;

void Cleanup();
void SubmitInput();

// A complete line must contain exactly one number, with optional whitespace.
template <typename T> bool ParseNumber(const string &Text, T &Value) {
  istringstream Stream(Text);
  if (!(Stream >> Value))
    return false;
  Stream >> ws;
  return Stream.eof();
}

void SetMessage(const string &Text) {
  Message = Text;
  cout << Text << '\n';
}

void Text(float X, float Y, const string &Value, bool Small = false) {
  glColor3f(0.13f, 0.16f, 0.20f);
  glRasterPos2f(X, Y);
  for (unsigned char Character : Value)
    glutBitmapCharacter(
        Small ? GLUT_BITMAP_HELVETICA_12 : GLUT_BITMAP_HELVETICA_18, Character);
}

void DrawInput(const string &Prompt = "Choice") {
  Text(665, 190, "Type a number, then Enter.", true);
  Text(665, 168, "Backspace edits. Esc exits.", true);
  Text(665, 130, Prompt + ": " + Input + "_", true);
  // Wrap status text so even validation messages fit within the window.
  istringstream Words(Message);
  string Word, Line;
  float Y = 92;
  while (Words >> Word) {
    if (Line.size() + Word.size() + 1 > 85) {
      Text(40, Y, Line, true);
      Y -= 18;
      Line.clear();
    }
    if (!Line.empty())
      Line += ' ';
    Line += Word;
  }
  Text(40, Y, Line, true);
}

// Keep the original scene proportions when resizing the window.
void Reshape(int Width, int Height) {
  Width = max(Width, 1);
  Height = max(Height, 1);
  float Scale = min(Width / 1000.0f, Height / 700.0f);
  int ViewWidth = max(1, static_cast<int>(1000 * Scale));
  int ViewHeight = max(1, static_cast<int>(700 * Scale));
  glViewport((Width - ViewWidth) / 2, (Height - ViewHeight) / 2, ViewWidth,
             ViewHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1000, 0, 700, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void ExitProgram() {
  if (Closing)
    return;
  Closing = true;
  Cleanup(); // The OpenGL context is still current here.
  glutLeaveMainLoop();
}

void CloseWindow() {
  Closing = true;
  Cleanup(); // Handles clicking the window's X button.
}

void Keyboard(unsigned char Key, int, int) {
  if (Key == 27) {
    ExitProgram();
    return;
  }
  if (Key == 13 || Key == 10) {
    cout << "Input: " << Input << '\n';
    SubmitInput();
    Input.clear();
  } else if (Key == 8 || Key == 127) {
    if (!Input.empty())
      Input.pop_back();
  } else if (Key >= 32 && Key <= 126) {
    if (Input.size() < 24)
      Input += static_cast<char>(Key);
    else
      SetMessage("Input is too long. Press Backspace to edit it.");
  }
  if (!Closing)
    glutPostRedisplay();
}

bool OpenWindow(int &Argc, char **Argv, const char *Title) {
  glutInit(&Argc, Argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutCreateWindow(Title);
  GLenum Result = glewInit();
  if (Result != GLEW_OK || !GLEW_VERSION_1_5) {
    cerr << "OpenGL 1.5 buffer support and GLEW are required.\n";
    return false;
  }
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutKeyboardFunc(Keyboard);
  glutReshapeFunc(Reshape);
  glutCloseFunc(CloseWindow);
  glClearColor(0.96f, 0.97f, 0.95f, 1.0f);
  return true;
}

// References: PDF Ex02 (VBO + glDrawArrays), Ex07 (usage hint),
// Ex15 (cleanup), Ex19 (many triangles in one VBO).
GLuint SceneVBO = 0;
int ActiveSignal = 1; // Initial state and Reset both mean Red / Stop.
struct Shape {
  GLint First;
  GLsizei Count;
};
Shape Housing, Pole, Base, Signals[3];
vector<GLfloat> Positions;

void AddVertex(float X, float Y) {
  Positions.push_back(X);
  Positions.push_back(Y);
}

Shape AddRectangle(float Left, float Bottom, float Right, float Top) {
  Shape Result{static_cast<GLint>(Positions.size() / 2), 6};
  AddVertex(Left, Bottom);
  AddVertex(Right, Bottom);
  AddVertex(Right, Top);
  AddVertex(Left, Bottom);
  AddVertex(Right, Top);
  AddVertex(Left, Top);
  return Result;
}

Shape AddIndicator(float X, float Y, float Radius) {
  const int Segments = 48;
  const float Pi = 3.14159265358979323846f;
  Shape Result{static_cast<GLint>(Positions.size() / 2), Segments * 3};
  for (int I = 0; I < Segments; ++I) {
    float A = 2 * Pi * I / Segments;
    float B = 2 * Pi * (I + 1) / Segments;
    AddVertex(X, Y);
    AddVertex(X + Radius * cos(A), Y + Radius * sin(A));
    AddVertex(X + Radius * cos(B), Y + Radius * sin(B));
  }
  return Result;
}

void BuildScene() {
  Housing = AddRectangle(180, 235, 385, 590);
  Pole = AddRectangle(267, 155, 298, 235);
  Base = AddRectangle(220, 140, 345, 160);
  Signals[0] = AddIndicator(282, 525, 44);
  Signals[1] = AddIndicator(282, 413, 44);
  Signals[2] = AddIndicator(282, 301, 44);
  glGenBuffers(1, &SceneVBO);
  glBindBuffer(GL_ARRAY_BUFFER, SceneVBO);
  // Positions never change when the signal color changes.
  glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(GLfloat),
               Positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawShape(Shape Value, float R, float G, float B) {
  glColor3f(R, G, B);
  glDrawArrays(GL_TRIANGLES, Value.First,
               Value.Count); // Vertex count, not float count.
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glBindBuffer(GL_ARRAY_BUFFER, SceneVBO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, nullptr);
  DrawShape(Pole, 0.37f, 0.40f, 0.43f);
  DrawShape(Base, 0.23f, 0.26f, 0.30f);
  DrawShape(Housing, 0.12f, 0.15f, 0.18f);
  for (int I = 0; I < 3; ++I) {
    float R = 0.24f, G = 0.27f, B = 0.28f;
    if (ActiveSignal == I + 1) {
      R = I == 2 ? 0.12f : 1.0f;
      G = I == 0 ? 0.17f : (I == 1 ? 0.76f : 0.84f);
      B = I == 2 ? 0.35f : 0.12f;
    }
    DrawShape(Signals[I], R, G, B);
  }
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  Text(40, 650, "CAMPUS TRAFFIC SIGNAL");
  Text(40, 620, "MP01  |  Static VBO and glDrawArrays", true);
  Text(420, 520, ActiveSignal == 1 ? "STOP  [ON]" : "STOP");
  Text(420, 408, ActiveSignal == 2 ? "CAUTION [ON]" : "CAUTION");
  Text(420, 296, ActiveSignal == 3 ? "GO  [ON]" : "GO");
  Text(665, 585, "OPERATOR MENU");
  Text(665, 540, "1  Red / Stop");
  Text(665, 505, "2  Yellow / Caution");
  Text(665, 470, "3  Green / Go");
  Text(665, 435, "4  All Off");
  Text(665, 400, "5  Reset to Red");
  Text(665, 365, "0  Exit");
  DrawInput();
  glutSwapBuffers();
}

void SubmitInput() {
  int Choice;
  if (!ParseNumber(Input, Choice) || Choice < 0 || Choice > 5) {
    SetMessage("Invalid choice. Enter one whole number from 0 to 5.");
    return;
  }
  if (Choice == 0) {
    ExitProgram();
    return;
  }
  ActiveSignal = Choice == 5 ? 1 : (Choice == 4 ? 0 : Choice);
  const char *States[] = {"ALL OFF", "RED / STOP", "YELLOW / CAUTION",
                          "GREEN / GO"};
  SetMessage(string(Choice == 5 ? "Reset. Active state: " : "Active state: ") +
             States[ActiveSignal]);
}

void Cleanup() {
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (SceneVBO) {
    glDeleteBuffers(1, &SceneVBO);
    SceneVBO = 0;
  }
}

int main(int Argc, char **Argv) {
  if (!OpenWindow(Argc, Argv, "MP01 - Campus Traffic Signal Visualizer"))
    return 1;
  BuildScene();
  glutDisplayFunc(Display);
  cout << "1 Red | 2 Yellow | 3 Green | 4 All Off | 5 Reset | 0 Exit\n";
  SetMessage("Active state: RED / STOP. Focus this window, type a choice, and "
             "press Enter.");
  glutMainLoop();
  return 0;
}

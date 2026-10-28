
// Input: focus this graphics window, type a choice, then press Enter.
#ifndef NOMINMAX
#define NOMINMAX
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

// References: PDF Ex09 and Ex10 (position VBO + element buffer),
// Ex07 (static usage), Ex15 (cleanup).
GLuint PositionVBO = 0, IndexEBO = 0;
int CurrentLayout = 0;
bool ShowPlayground = true, ShowGarden = true, ShowSeating = true;
const int RectanglesPerLayout = 7;

struct ParkRectangle {
  float Left, Bottom, Right, Top;
};
const ParkRectangle Layouts[2][RectanglesPerLayout] = {{{50, 150, 610, 585},
                                                        {58, 158, 602, 577},
                                                        {58, 330, 602, 370},
                                                        {300, 158, 340, 577},
                                                        {85, 410, 250, 545},
                                                        {380, 410, 560, 545},
                                                        {380, 195, 560, 290}},
                                                       {{50, 150, 610, 585},
                                                        {58, 158, 602, 577},
                                                        {58, 330, 602, 370},
                                                        {300, 158, 340, 577},
                                                        {380, 195, 560, 290},
                                                        {85, 410, 250, 545},
                                                        {380, 410, 560, 545}}};

void BuildScene() {
  vector<GLfloat> Vertices;
  vector<GLuint> Indices;
  for (const auto &Layout : Layouts) {
    for (const Rectangle &R : Layout) {
      GLuint First = static_cast<GLuint>(Vertices.size() / 2);
      GLfloat Points[] = {R.Left,  R.Bottom, R.Right, R.Bottom,
                          R.Right, R.Top,    R.Left,  R.Top};
      Vertices.insert(Vertices.end(), Points, Points + 8);
      GLuint Corners[] = {First, First + 1, First + 2,
                          First, First + 2, First + 3};
      Indices.insert(Indices.end(), Corners, Corners + 6);
    }
  }
  // Preload both layouts once; toggling/layout selection needs no re-upload.
  glGenBuffers(1, &PositionVBO);
  glBindBuffer(GL_ARRAY_BUFFER, PositionVBO);
  glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(GLfloat),
               Vertices.data(), GL_STATIC_DRAW);
  glGenBuffers(1, &IndexEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint),
               Indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DrawRectangle(int Number, float R, float G, float B) {
  glColor3f(R, G, B);
  // With an EBO bound, the final argument is a BYTE OFFSET, not a CPU address.
  uintptr_t Offset =
      (CurrentLayout * RectanglesPerLayout + Number) * 6 * sizeof(GLuint);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                 reinterpret_cast<const void *>(Offset));
}

void FeatureLabel(int Number, const string &Label) {
  const Rectangle &R = Layouts[CurrentLayout][Number];
  Text(R.Left + 12, (R.Bottom + R.Top) / 2, Label, true);
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glBindBuffer(GL_ARRAY_BUFFER, PositionVBO);
  glVertexPointer(2, GL_FLOAT, 0, nullptr);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexEBO);
  DrawRectangle(0, 0.20f, 0.34f, 0.25f);
  DrawRectangle(1, 0.82f, 0.89f, 0.76f);
  DrawRectangle(2, 0.91f, 0.83f, 0.66f);
  DrawRectangle(3, 0.91f, 0.83f, 0.66f);
  if (ShowPlayground)
    DrawRectangle(4, 0.98f, 0.63f, 0.30f);
  if (ShowGarden)
    DrawRectangle(5, 0.39f, 0.71f, 0.42f);
  if (ShowSeating)
    DrawRectangle(6, 0.46f, 0.70f, 0.88f);
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  if (ShowPlayground)
    FeatureLabel(4, "PLAYGROUND");
  if (ShowGarden)
    FeatureLabel(5, "GARDEN");
  if (ShowSeating)
    FeatureLabel(6, "SEATING");
  Text(85, 345, "WALKING PATH", true);
  Text(40, 650, "COMMUNITY PARK PLANNER");
  Text(40, 620,
       string("MP02  |  Indexed triangles  |  Layout ") +
           (CurrentLayout == 0 ? "A" : "B"),
       true);
  Text(665, 585, "PLANNER MENU");
  Text(665, 545, "1  Show All");
  Text(665, 510, "2  Toggle Playground");
  Text(665, 475, "3  Toggle Garden");
  Text(665, 440, "4  Toggle Seating");
  Text(665, 405, "5  Layout A");
  Text(665, 370, "6  Layout B");
  Text(665, 335, "0  Exit");
  Text(665, 285, "Orange: playground | Green: garden", true);
  Text(665, 263, "Blue: seating | Tan: walking path", true);
  Text(665, 241, "Dark green: park boundary", true);
  DrawInput();
  glutSwapBuffers();
}

void SubmitInput() {
  int Choice;
  if (!ParseNumber(Input, Choice) || Choice < 0 || Choice > 6) {
    SetMessage("Invalid choice. Enter one whole number from 0 to 6.");
    return;
  }
  if (Choice == 0) {
    ExitProgram();
    return;
  }
  if (Choice == 1)
    ShowPlayground = ShowGarden = ShowSeating = true;
  if (Choice == 2)
    ShowPlayground = !ShowPlayground;
  if (Choice == 3)
    ShowGarden = !ShowGarden;
  if (Choice == 4)
    ShowSeating = !ShowSeating;
  if (Choice == 5)
    CurrentLayout = 0;
  if (Choice == 6)
    CurrentLayout = 1;
  SetMessage(string("Layout ") + (CurrentLayout == 0 ? "A" : "B") +
             " | Playground: " + (ShowPlayground ? "shown" : "hidden") +
             " | Garden: " + (ShowGarden ? "shown" : "hidden") +
             " | Seating: " + (ShowSeating ? "shown" : "hidden"));
}

void Cleanup() {
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  if (PositionVBO) {
    glDeleteBuffers(1, &PositionVBO);
    PositionVBO = 0;
  }
  if (IndexEBO) {
    glDeleteBuffers(1, &IndexEBO);
    IndexEBO = 0;
  }
}

int main(int Argc, char **Argv) {
  if (!OpenWindow(Argc, Argv, "MP02 - Community Park Layout Planner"))
    return 1;
  BuildScene();
  glutDisplayFunc(Display);
  cout << "1 Show All | 2 Playground | 3 Garden | 4 Seating | 5 A | 6 B | "
          "0 Exit\n";
  cout << "Legend: orange playground, green garden, blue seating, tan path.\n";
  SetMessage("Layout A. All features are shown. Layout changes preserve "
             "visibility choices.");
  glutMainLoop();
  return 0;
}

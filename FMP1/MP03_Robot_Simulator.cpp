// Module 5 machine problem. C++17, GLEW, FreeGLUT, compatibility OpenGL.
// Input: focus this graphics window, type a choice, then press Enter.
#ifndef NOMINMAX
#define NOMINMAX // Keep Windows macros from overriding std::min/max.
#endif
#include <GL/freeglut.h>
#include <GL/glew.h> // GLEW must be included BEFORE FreeGLUT.
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int WindowWidth = 1000;
const int WindowHeight = 700;
std::string Input;
std::string Message;
bool Closing = false;

void Cleanup();
void SubmitInput();

// A complete line must contain exactly one number, with optional whitespace.
template <typename T> bool ParseNumber(const std::string &Text, T &Value) {
  std::istringstream Stream(Text);
  if (!(Stream >> Value))
    return false;
  Stream >> std::ws;
  return Stream.eof();
}

void SetMessage(const std::string &Text) {
  Message = Text;
  std::cout << Text << '\n';
}

void Text(float X, float Y, const std::string &Value, bool Small = false) {
  glColor3f(0.13f, 0.16f, 0.20f);
  glRasterPos2f(X, Y);
  for (unsigned char Character : Value)
    glutBitmapCharacter(
        Small ? GLUT_BITMAP_HELVETICA_12 : GLUT_BITMAP_HELVETICA_18, Character);
}

void DrawInput(const std::string &Prompt = "Choice") {
  Text(665, 190, "Type a number, then Enter.", true);
  Text(665, 168, "Backspace edits. Esc exits.", true);
  Text(665, 130, Prompt + ": " + Input + "_", true);
  // Wrap status text so even validation messages fit within the window.
  std::istringstream Words(Message);
  std::string Word, Line;
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
  Width = std::max(Width, 1);
  Height = std::max(Height, 1);
  float Scale = std::min(Width / 1000.0f, Height / 700.0f);
  int ViewWidth = std::max(1, static_cast<int>(1000 * Scale));
  int ViewHeight = std::max(1, static_cast<int>(700 * Scale));
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
    std::cout << "Input: " << Input << '\n';
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
    std::cerr << "OpenGL 1.5 buffer support and GLEW are required.\n";
    return false;
  }
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutKeyboardFunc(Keyboard);
  glutReshapeFunc(Reshape);
  glutCloseFunc(CloseWindow);
  glClearColor(0.96f, 0.97f, 0.95f, 1.0f);
  return true;
}

// References: PDF Ex14 (mapped positions), Ex16 (timer callback),
// Ex20 (static scene + animated VBO + cleanup).
GLuint MapVBO = 0, RobotVBO = 0;
const float Left = 50, Right = 610, Bottom = 150, Top = 585;
const float StartX = 120, StartY = 205;
const float HalfWidth = 16, LowerHeight = 14, UpperHeight = 20;
float RobotX = StartX, RobotY = StartY;
double StepSize = 20;
bool ReadingStep = false, AutoMoving = false;
int Target = 0;
GLfloat RobotVertices[6];
const GLfloat Destinations[3][2] = {{155, 490}, {340, 490}, {525, 490}};

std::string Number(double Value) {
  std::ostringstream Stream;
  Stream << std::setprecision(5) << Value;
  return Stream.str();
}

void FillRobotVertices(float X, float Y, GLfloat *Data) {
  Data[0] = X;
  Data[1] = Y + UpperHeight;
  Data[2] = X - HalfWidth;
  Data[3] = Y - LowerHeight;
  Data[4] = X + HalfWidth;
  Data[5] = Y - LowerHeight;
}

void BuildScene() {
  std::vector<GLfloat> Map;
  auto AddRect = [&](float X1, float Y1, float X2, float Y2) {
    GLfloat P[] = {X1, Y1, X2, Y1, X2, Y2, X1, Y1, X2, Y2, X1, Y2};
    Map.insert(Map.end(), P, P + 12);
  };
  AddRect(Left, Bottom, Right, Top);
  AddRect(Left + 5, Bottom + 5, Right - 5, Top - 5);
  for (const auto &Zone : Destinations)
    AddRect(Zone[0] - 65, Zone[1] - 50, Zone[0] + 65, Zone[1] + 50);
  glGenBuffers(1, &MapVBO);
  glBindBuffer(GL_ARRAY_BUFFER, MapVBO);
  glBufferData(GL_ARRAY_BUFFER, Map.size() * sizeof(GLfloat), Map.data(),
               GL_STATIC_DRAW);
  FillRobotVertices(RobotX, RobotY, RobotVertices);
  glGenBuffers(1, &RobotVBO);
  glBindBuffer(GL_ARRAY_BUFFER, RobotVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(RobotVertices), RobotVertices,
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Return true when a requested move was limited by a warehouse wall.
bool MoveRobot(double Dx, double Dy) {
  // Clamp the WHOLE triangle, not just its center, inside the boundary.
  double RequestedX = RobotX + Dx, RequestedY = RobotY + Dy;
  float NewX = static_cast<float>(
      std::clamp(RequestedX, static_cast<double>(Left + HalfWidth),
                 static_cast<double>(Right - HalfWidth)));
  float NewY = static_cast<float>(
      std::clamp(RequestedY, static_cast<double>(Bottom + LowerHeight),
                 static_cast<double>(Top - UpperHeight)));
  float ActualDx = NewX - RobotX, ActualDy = NewY - RobotY;
  glBindBuffer(GL_ARRAY_BUFFER, RobotVBO);
  // The buffer contains [x0,y0, x1,y1, x2,y2], so X uses even indices.
  GLfloat *Mapped =
      static_cast<GLfloat *>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));
  bool Uploaded = false;
  if (Mapped != nullptr) {
    for (int I = 0; I < 3; ++I) {
      Mapped[I * 2] += ActualDx;
      Mapped[I * 2 + 1] += ActualDy;
    }
    Uploaded = glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE;
  }
  FillRobotVertices(NewX, NewY, RobotVertices); // Authoritative CPU backup.
  if (!Uploaded) {
    // Safe fallback: a null mapping was never written. A failed unmap makes
    // the old store undefined, so replace ALL data, retaining the same VBO ID.
    glBufferData(GL_ARRAY_BUFFER, sizeof(RobotVertices), RobotVertices,
                 GL_DYNAMIC_DRAW);
    std::cout << "Mapping unavailable/invalid; restored the entire VBO from "
                 "CPU data.\n";
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  RobotX = NewX;
  RobotY = NewY;
  return std::abs(RequestedX - NewX) > 0.001 ||
         std::abs(RequestedY - NewY) > 0.001;
}

// Exactly ONE timer chain is started in main. Display() never schedules timers.
void Timer(int) {
  if (Closing)
    return;
  if (AutoMoving) {
    double Dx = Destinations[Target][0] - RobotX;
    double Dy = Destinations[Target][1] - RobotY;
    double Distance = std::hypot(Dx, Dy);
    if (Distance > 0.001) {
      double Travel = std::min(StepSize, Distance);
      MoveRobot(Dx / Distance * Travel, Dy / Distance * Travel);
    }
    if (std::hypot(Destinations[Target][0] - RobotX,
                   Destinations[Target][1] - RobotY) <= 0.01) {
      SetMessage(std::string("Auto demo reached Zone ") + char('A' + Target) +
                 ".");
      ++Target;
      if (Target == 3) {
        AutoMoving = false;
        SetMessage("Auto demo complete. Visited Zones A, B and C.");
      }
    }
    glutPostRedisplay();
  }
  glutTimerFunc(100, Timer, 0);
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, MapVBO);
  glVertexPointer(2, GL_FLOAT, 0, nullptr);
  glColor3f(0.25f, 0.30f, 0.34f);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glColor3f(0.88f, 0.90f, 0.90f);
  glDrawArrays(GL_TRIANGLES, 6, 6);
  glColor3f(0.70f, 0.83f, 0.95f);
  glDrawArrays(GL_TRIANGLES, 12, 6);
  glColor3f(0.72f, 0.88f, 0.70f);
  glDrawArrays(GL_TRIANGLES, 18, 6);
  glColor3f(0.98f, 0.82f, 0.53f);
  glDrawArrays(GL_TRIANGLES, 24, 6);
  glBindBuffer(GL_ARRAY_BUFFER, RobotVBO);
  glVertexPointer(2, GL_FLOAT, 0, nullptr);
  glColor3f(0.78f, 0.16f, 0.20f);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  for (int I = 0; I < 3; ++I)
    Text(Destinations[I][0] - 28, 550, std::string("ZONE ") + char('A' + I),
         true);
  Text(80, 170, "START", true);
  Text(40, 650, "WAREHOUSE ROBOT ROUTE");
  Text(40, 620, "MP03  |  Dynamic VBO and glMapBuffer", true);
  Text(665, 585, "ROBOT MENU");
  Text(665, 550, "1  Move Left");
  Text(665, 520, "2  Move Right");
  Text(665, 490, "3  Move Up");
  Text(665, 460, "4  Move Down");
  Text(665, 430, "5  Set Step Size");
  Text(665, 400, "6  Return to Start");
  Text(665, 370, "7  Auto-Move Demo");
  Text(665, 340, "0  Exit");
  Text(665, 295, "Step: " + Number(StepSize) + " units", true);
  Text(665, 273, "Position: " + Number(RobotX) + ", " + Number(RobotY), true);
  Text(665, 251,
       std::string("Auto demo: ") + (AutoMoving ? "RUNNING" : "STOPPED"), true);
  DrawInput(ReadingStep ? "Step" : "Choice");
  glutSwapBuffers();
}

void SubmitInput() {
  if (ReadingStep) {
    double Value;
    if (!ParseNumber(Input, Value) || !std::isfinite(Value) || Value <= 0) {
      SetMessage("Invalid step. Enter a finite positive number, such as 10 or "
                 "25.5. Esc exits.");
      return;
    }
    StepSize = Value;
    ReadingStep = false;
    SetMessage("Step size set to " + Number(StepSize) +
               " units per move / timer tick.");
    return;
  }
  int Choice;
  if (!ParseNumber(Input, Choice) || Choice < 0 || Choice > 7) {
    SetMessage("Invalid choice. Enter one whole number from 0 to 7.");
    return;
  }
  if (Choice == 0) {
    ExitProgram();
    return;
  }
  AutoMoving = false; // Manual commands and step editing pause the demo.
  if (Choice == 5) {
    ReadingStep = true;
    SetMessage("Enter a positive step size, then press Enter. Larger steps "
               "move faster in the demo.");
    return;
  }
  if (Choice == 6) {
    MoveRobot(StartX - RobotX, StartY - RobotY);
    Target = 0;
    SetMessage("Robot returned to its starting position.");
    return;
  }
  if (Choice == 7) {
    Target = 0;
    AutoMoving = true;
    SetMessage("Auto demo started: Zone A, then B, then C. A manual command "
               "stops it.");
    return;
  }
  bool Limited =
      MoveRobot(Choice == 1 ? -StepSize : (Choice == 2 ? StepSize : 0),
                Choice == 4 ? -StepSize : (Choice == 3 ? StepSize : 0));
  SetMessage(std::string(Limited ? "Boundary reached. " : "Moved. ") +
             "Robot position: (" + Number(RobotX) + ", " + Number(RobotY) +
             ").");
}

void Cleanup() {
  AutoMoving = false;
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (MapVBO) {
    glDeleteBuffers(1, &MapVBO);
    MapVBO = 0;
  }
  if (RobotVBO) {
    glDeleteBuffers(1, &RobotVBO);
    RobotVBO = 0;
  }
}

int main(int Argc, char **Argv) {
  if (!OpenWindow(Argc, Argv, "MP03 - Warehouse Robot Route Simulator"))
    return 1;
  BuildScene(); // Create buffers BEFORE the first timer can run.
  glutDisplayFunc(Display);
  glutTimerFunc(100, Timer, 0);
  std::cout << "1 Left | 2 Right | 3 Up | 4 Down | 5 Step | 6 Start | 7 Auto | "
               "0 Exit\n";
  SetMessage(
      "Robot ready. Default step: 20. Auto demo visits Zones A, B and C.");
  glutMainLoop();
  return 0;
}

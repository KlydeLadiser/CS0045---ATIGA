#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/freeglut_ext.h>
#include <cmath>
#include <iostream>
#include <string>
using namespace std;

const float PI = 3.14159265359f;
const float WIDTH = 1280.0f, HEIGHT = 800.0f;
const int BODY_COUNT = 9, PAGE_COUNT = 5, STAR_COUNT = 80;
const float MAP_X = 640.0f, MAP_Y = 452.0f;
const float CLOSE_X = 358.0f, CLOSE_Y = 464.0f, CLOSE_R = 170.0f;

struct Color {
  float r, g, b;
};
struct Vec2 {
  float x, y;
};
struct Rect {
  float x, y, w, h;
};
struct Body {
  const char *name;
  const char *type;
  Color color;
  float radius, orbit, startAngle, orbitSpeed;
  const char *stats;
  const char *facts[4];
};

const Color BLACK = {0.0f, 0.0f, 0.0f};
const Color DARKBLUE = {11 / 255.0f, 22 / 255.0f, 34 / 255.0f};
const Color BLUE = {36 / 255.0f, 74 / 255.0f, 97 / 255.0f};
const Color CYAN = {82 / 255.0f, 224 / 255.0f, 235 / 255.0f};
const Color WHITE = {1.0f, 1.0f, 1.0f};
const Color GREYBLUE = {130 / 255.0f, 168 / 255.0f, 186 / 255.0f};
const Color GOLD = {255 / 255.0f, 184 / 255.0f, 79 / 255.0f};

const Body bodies[BODY_COUNT] = {
    {"Sun",
     "STAR",
     {1.0f, .64f, .16f},
     32,
     0,
     0,
     0,
     "Diameter: about 1.4 million km\nRotation: about 25 days at "
     "equator\nGravity: about 274 m/s^2\nType: G-type main-sequence star",
     {"The Sun is plasma, a hot electrically charged gas. Fusion in its core "
      "releases the energy that eventually reaches Earth as sunlight.",
      "The photosphere is the visible layer we call the Sun's surface. It is "
      "roughly 5,500 degrees Celsius and is not solid ground.",
      "The Sun travels around the Milky Way's center. One trip takes roughly "
      "230 million Earth years.",
      "Its equator and poles do not rotate together: about 25 Earth days at "
      "the equator and 36 near the poles."}},
    {"Mercury",
     "ROCKY PLANET",
     {.60f, .59f, .57f},
     9,
     70,
     40,
     23,
     "Diameter: 4,879 km\nRotation: 58.6 Earth days\nYear: 88 Earth "
     "days\nGravity: 3.7 m/s^2",
     {"Mercury is the nearest planet to the Sun and the smallest of the eight "
      "planets. Its rocky surface is covered in impact craters.",
      "Its very thin exosphere cannot hold much heat. Sunlit ground can reach "
      "about 430 degrees Celsius, while night temperatures can fall near -180.",
      "A solar day, from one noon to the next, lasts about 176 Earth days. "
      "That is two Mercury years, despite a rotation taking about 59 days.",
      "Permanently shaded polar craters can shelter water ice, even on the "
      "planet closest to the Sun."}},
    {"Venus",
     "ROCKY PLANET",
     {.91f, .70f, .39f},
     14,
     113,
     158,
     18,
     "Diameter: 12,104 km\nRotation: 243 days, retrograde\nYear: 224.7 Earth "
     "days\nGravity: 8.9 m/s^2",
     {"Venus is the hottest planet. Its thick carbon-dioxide atmosphere traps "
      "heat through a powerful greenhouse effect.",
      "Clouds rich in sulfuric acid hide the surface. Atmospheric pressure at "
      "ground level is about 92 times Earth's.",
      "Venus spins in the opposite direction to Earth. From its surface, the "
      "Sun would appear to rise in the west.",
      "One complete spin takes longer than one Venus year. A solar day is "
      "different: roughly 117 Earth days."}},
    {"Earth",
     "ROCKY PLANET",
     {.20f, .54f, .89f},
     16,
     159,
     290,
     14,
     "Diameter: 12,756 km\nRotation: about 23.9 hours\nYear: about 365.2 "
     "days\nGravity: 9.8 m/s^2",
     {"Liquid water covers about 71 percent of Earth's surface. Earth is the "
      "only world currently known to support life.",
      "The atmosphere near the surface is about 78 percent nitrogen and 21 "
      "percent oxygen, with small amounts of other gases.",
      "Earth's magnetic field helps deflect charged particles from the Sun. "
      "Interactions near the poles can produce auroras.",
      "Earth's longest mountain range lies mostly under the ocean. The "
      "mid-ocean ridge system stretches across the planet."}},
    {"Mars",
     "ROCKY PLANET",
     {.86f, .36f, .21f},
     12,
     209,
     210,
     11,
     "Diameter: 6,792 km\nRotation: about 24.6 hours\nYear: 687 Earth "
     "days\nGravity: 3.7 m/s^2",
     {"Iron minerals in Martian rocks and dust rust, giving the planet its "
      "familiar reddish appearance.",
      "Mars has a thin atmosphere, mostly carbon dioxide. Its surface has "
      "polar ice, extinct volcanoes, valleys and impact craters.",
      "Mars has two small, irregularly shaped moons, Phobos and Deimos. They "
      "are too small for gravity to pull them into round shapes.",
      "Phobos is slowly moving closer to Mars. In roughly 50 million years it "
      "could break apart or hit the planet."}},
    {"Jupiter",
     "GAS GIANT",
     {.79f, .60f, .43f},
     27,
     278,
     35,
     8,
     "Diameter: 142,984 km\nRotation: about 9.9 hours\nYear: about 11.9 Earth "
     "years\nGravity: 23.1 m/s^2 (1 bar)",
     {"Jupiter is the largest planet. Its atmosphere is mostly hydrogen and "
      "helium, with bright zones and darker cloud belts.",
      "The Great Red Spot is a huge storm that has persisted for centuries. "
      "Jupiter has no solid surface to stand on.",
      "Ganymede, one of Jupiter's moons, is larger than Mercury. Europa likely "
      "hides a liquid-water ocean below its icy crust.",
      "Jupiter has rings too. They are faint and dusty, unlike Saturn's bright "
      "rings, and were discovered by Voyager 1 in 1979."}},
    {"Saturn",
     "GAS GIANT",
     {.86f, .76f, .51f},
     23,
     355,
     151,
     6,
     "Diameter: 120,536 km\nRotation: about 10.7 hours\nYear: about 29.4 Earth "
     "years\nGravity: 9.0 m/s^2 (1 bar)",
     {"Saturn's rings contain countless pieces of ice and rock. They are "
      "separate orbiting particles, not one solid disk.",
      "Saturn is made mainly of hydrogen and helium. Its average density is "
      "lower than that of water.",
      "Titan has a thick atmosphere and surface lakes of liquid methane and "
      "ethane. Enceladus ejects icy material from near its south pole.",
      "A vast six-sided jet-stream pattern surrounds Saturn's north pole. This "
      "atmospheric hexagon is about 30,000 kilometers across."}},
    {"Uranus",
     "ICE GIANT",
     {.43f, .81f, .85f},
     19,
     437,
     320,
     4,
     "Diameter: 51,118 km\nRotation: 17.2 hours, retrograde\nYear: about 84 "
     "Earth years\nGravity: 8.7 m/s^2 (1 bar)",
     {"Uranus is an ice giant. Methane in its atmosphere absorbs red light, "
      "contributing to its blue-green appearance.",
      "Its axis is tilted by about 98 degrees. The planet rotates nearly on "
      "its side and experiences extreme seasons.",
      "Uranus has faint rings. It was the first planet discovered with a "
      "telescope, by William Herschel in 1781.",
      "Its moons take names from characters in works by William Shakespeare "
      "and Alexander Pope, rather than only classical mythology."}},
    {"Neptune",
     "ICE GIANT",
     {.24f, .39f, .86f},
     19,
     520,
     60,
     3,
     "Diameter: 49,528 km\nRotation: about 16.1 hours\nYear: about 164 Earth "
     "years\nGravity: 11.0 m/s^2 (1 bar)",
     {"Neptune is the farthest of the eight planets. This cold ice giant has "
      "strong winds and a hydrogen-helium atmosphere containing methane.",
      "Neptune was located in 1846 using mathematical predictions based on "
      "unexpected changes in Uranus' orbit.",
      "Neptune has faint rings and concentrations of ring material called "
      "arcs. It receives much less sunlight than Earth.",
      "Triton, Neptune's largest moon, orbits opposite to Neptune's rotation. "
      "This unusual motion suggests it was captured."}}};

enum ViewState { OVERVIEW, ZOOM_IN, DETAIL, ZOOM_OUT };
ViewState view = OVERVIEW;
int selectedBody = -1, factPage = 0, hoveredBody = -1;
bool paused = false, showOrbits = true, controlsOpen = true, logOpen = false;
bool visited[BODY_COUNT] = {};
bool pagesRead[BODY_COUNT][PAGE_COUNT] = {};
float angles[BODY_COUNT] = {}, spin = 0, speed = 1, zoomProgress = 0;
Vec2 zoomStart = {0, 0};
GLfloat starVertices[STAR_COUNT][2] = {
    {-.94f, .87f},  {-.81f, .62f},  {-.67f, .91f},  {-.52f, .72f},
    {-.39f, .84f},  {-.22f, .66f},  {-.08f, .93f},  {.07f, .75f},
    {.23f, .88f},   {.38f, .61f},   {.53f, .94f},   {.69f, .79f},
    {.84f, .89f},   {.96f, .65f},   {-.97f, .45f},  {-.83f, .29f},
    {-.71f, .53f},  {-.57f, .36f},  {-.43f, .59f},  {-.29f, .42f},
    {-.14f, .51f},  {.02f, .33f},   {.17f, .55f},   {.31f, .41f},
    {.46f, .57f},   {.62f, .32f},   {.76f, .48f},   {.92f, .39f},
    {-.91f, .13f},  {-.77f, -.04f}, {-.63f, .20f},  {-.49f, .02f},
    {-.35f, .17f},  {-.20f, -.08f}, {-.05f, .09f},  {.10f, -.02f},
    {.26f, .22f},   {.41f, .05f},   {.56f, .16f},   {.72f, -.06f},
    {.87f, .19f},   {.98f, .01f},   {-.96f, -.22f}, {-.82f, -.38f},
    {-.68f, -.18f}, {-.54f, -.43f}, {-.40f, -.26f}, {-.25f, -.48f},
    {-.10f, -.30f}, {.05f, -.45f},  {.20f, -.23f},  {.36f, -.39f},
    {.51f, -.17f},  {.66f, -.46f},  {.81f, -.29f},  {.94f, -.42f},
    {-.90f, -.59f}, {-.74f, -.75f}, {-.60f, -.56f}, {-.45f, -.79f},
    {-.30f, -.63f}, {-.15f, -.84f}, {.01f, -.67f},  {.16f, -.81f},
    {.32f, -.58f},  {.47f, -.76f},  {.62f, -.61f},  {.78f, -.83f},
    {.92f, -.64f},  {-.99f, -.91f}, {-.86f, -.85f}, {-.68f, -.95f},
    {-.51f, -.89f}, {-.33f, -.97f}, {-.16f, -.92f}, {.04f, -.96f},
    {.25f, -.90f},  {.45f, -.95f},  {.67f, -.91f},  {.88f, -.96f}};
int mousePixelX = -1, mousePixelY = -1;

const Rect CONTROL_TAB = {30, 26, 324, 40};
const Rect CONTROL_PANEL = {30, 66, 324, 255};
const Rect LOG_TAB = {943, 26, 307, 40};
const Rect LOG_PANEL = {943, 66, 307, 328};
const Rect BACK_BUTTON = {1090, 732, 160, 36};
const Rect FACT_PANEL = {710, 246, 540, 445};

void color(Color c) { glColor3f(c.r, c.g, c.b); }

void vertex(float x, float y) {
  float openGLX = 2.0f * x / WIDTH - 1.0f;
  float openGLY = 2.0f * y / HEIGHT - 1.0f;
  glVertex2f(openGLX, openGLY);
}

void rectangle(Rect r, Color c) {
  color(c);
  glBegin(GL_QUADS);
  vertex(r.x, r.y);
  vertex(r.x + r.w, r.y);
  vertex(r.x + r.w, r.y + r.h);
  vertex(r.x, r.y + r.h);
  glEnd();
}

void outline(Rect r, Color c) {
  color(c);
  glBegin(GL_LINE_LOOP);
  vertex(r.x, r.y);
  vertex(r.x + r.w, r.y);
  vertex(r.x + r.w, r.y + r.h);
  vertex(r.x, r.y + r.h);
  glEnd();
}

void line(float x1, float y1, float x2, float y2, Color c) {
  color(c);
  glBegin(GL_LINES);
  vertex(x1, y1);
  vertex(x2, y2);
  glEnd();
}

void circle(float x, float y, float rx, float ry, Color c, bool filled) {
  color(c);
  glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
  if (filled)
    vertex(x, y);
  const int segments = 96;
  for (int i = 0; i <= segments; ++i) {
    float angle = 2 * PI * i / segments;
    vertex(x + rx * cos(angle), y + ry * sin(angle));
  }
  glEnd();
}

float textWidth(const string &s, float size) {
  if (size < 14) {
    return s.length() * 8.0f;
  }
  return s.length() * 9.0f;
}

void text(float x, float y, const string &s, float size = 14, Color c = WHITE) {
  unsigned char letters[512];
  int i = 0;
  while (i < 511 && i < (int)s.length()) {
    letters[i] = s[i];
    i++;
  }
  letters[i] = '\0';
  color(c);
  glRasterPos2f(2.0f * x / WIDTH - 1.0f, 2.0f * y / HEIGHT - 1.0f);
  if (size < 14) {
    glutBitmapString(GLUT_BITMAP_8_BY_13, letters);
  } else {
    glutBitmapString(GLUT_BITMAP_9_BY_15, letters);
  }
}

void centeredText(float x, float y, const string &s, float size, Color c) {
  text(x - textWidth(s, size) / 2, y, s, size, c);
}

void wrappedText(float x, float y, float width, const string &s,
                 float size = 19, float spacing = 31) {
  string word, row;

  for (size_t i = 0; i <= s.size(); ++i) {
    if (i < s.size() && s[i] != ' ' && s[i] != '\n' && s[i] != '\t' &&
        s[i] != '\r' && s[i] != '\f' && s[i] != '\v') {
      word += s[i];
      continue;
    }
    if (word.empty())
      continue;
    string next = row.empty() ? word : row + " " + word;
    if (!row.empty() && textWidth(next, size) > width) {
      text(x, y, row, size);
      y -= spacing;
      row = word;
    } else
      row = next;
    word.clear();
  }
  if (!row.empty())
    text(x, y, row, size);
}

void button(Rect r, const string &label, bool active = false, float size = 13) {
  rectangle(r, active ? Color{.09f, .23f, .28f} : DARKBLUE);
  outline(r, active ? CYAN : BLUE);
  centeredText(r.x + r.w / 2, r.y + (r.h - size) / 2 + 2, label, size,
               active ? CYAN : WHITE);
}

Rect bodyButton(int i) { return {30.0f + i * 137.0f, 137, 124, 38}; }
Rect pageButton(int i) { return {730.0f + i * 101.0f, 599, 91, 36}; }

void drawStars() {
  color(GREYBLUE);
  glPointSize(2);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, starVertices);
  glDrawArrays(GL_POINTS, 0, STAR_COUNT);
  glDisableClientState(GL_VERTEX_ARRAY);
  glPointSize(1);
}

Vec2 bodyPosition(int i) {
  float angle = angles[i] * PI / 180;
  return {MAP_X + bodies[i].orbit * cos(angle),
          MAP_Y + bodies[i].orbit * .423f * sin(angle)};
}

void saturnRing(float x, float y, float radius, bool front) {
  color({.80f, .73f, .55f});
  glBegin(GL_QUAD_STRIP);
  for (int i = 0; i <= 64; i++) {
    float angle = PI * i / 64;
    if (front) {
      angle += PI;
    }
    vertex(x + radius * 1.75f * cos(angle), y + radius * .53f * sin(angle));
    vertex(x + radius * 1.34f * cos(angle), y + radius * .36f * sin(angle));
  }
  glEnd();
}

void drawBody(int i, float x, float y, float r) {
  Color c = bodies[i].color;
  if (i == 6) {
    saturnRing(x, y, r, false);
  }
  if (i == 0) {
    for (int j = 4; j >= 1; j--) {
      float glowRadius = r + j * r * .10f;
      circle(x, y, glowRadius, glowRadius,
             {.16f + .04f * (4 - j), .10f + .02f * (4 - j), .04f}, true);
    }
  }
  circle(x, y, r, r, c, true);
  if (r > 20 && (i == 2 || i == 5 || i == 6 || i == 7 || i == 8)) {
    for (int j = -3; j <= 3; j++) {
      float bandY = j * r * .20f;
      float edgeY = bandY;
      if (edgeY < 0) {
        edgeY = -edgeY;
      }
      edgeY += r * .025f;
      float halfWidth = sqrt(r * r - edgeY * edgeY) * .98f;
      rectangle({x - halfWidth, y + bandY - r * .025f, halfWidth * 2, r * .05f},
                {c.r * .78f, c.g * .78f, c.b * .78f});
    }
  }
  if (i == 3) {
    color({.29f, .64f, .43f});
    glBegin(GL_POLYGON);
    vertex(x - .65f * r, y + .25f * r);
    vertex(x - .38f * r, y + .67f * r);
    vertex(x + .04f * r, y + .54f * r);
    vertex(x + .17f * r, y + .22f * r);
    vertex(x - .18f * r, y - .12f * r);
    vertex(x - .54f * r, y - .03f * r);
    glEnd();
    circle(x + .27f * r, y - .40f * r, .19f * r, .30f * r, {.29f, .64f, .43f},
           true);
  }
  if (i == 1 || i == 4 || i == 0) {
    const Vec2 marks[] = {
        {-.42f, .30f}, {.23f, .45f}, {.35f, -.25f}, {-.20f, -.40f}};
    for (int j = 0; j < 4; j++) {
      circle(x + marks[j].x * r, y + marks[j].y * r, r * .09f, r * .075f,
             {c.r * .63f, c.g * .63f, c.b * .63f}, true);
    }
  }
  if (i == 5) {
    float spotX = x + sin(spin * PI / 180) * r * .40f;
    circle(spotX, y - r * .24f, r * .19f, r * .10f, {.65f, .28f, .16f}, true);
  }
  if (i == 6) {
    saturnRing(x, y, r, true);
  }
}

int visitedCount() {
  int count = 0;
  for (int i = 0; i < BODY_COUNT; ++i)
    if (visited[i])
      ++count;
  return count;
}

int readCount(int i) {
  int count = 0;
  for (int j = 0; j < PAGE_COUNT; ++j)
    if (pagesRead[i][j])
      ++count;
  return count;
}

void drawOverview() {
  if (showOrbits)
    for (int i = 1; i < BODY_COUNT; ++i)
      circle(MAP_X, MAP_Y, bodies[i].orbit, bodies[i].orbit * .423f, BLUE,
             false);
  for (int i = 0; i < BODY_COUNT; ++i) {
    Vec2 p = bodyPosition(i);
    drawBody(i, p.x, p.y, bodies[i].radius);
    if (hoveredBody == i)
      circle(p.x, p.y, bodies[i].radius + 7, bodies[i].radius + 7, CYAN, false);
    centeredText(p.x, p.y - bodies[i].radius - 22, bodies[i].name, 13,
                 hoveredBody == i ? CYAN : GREYBLUE);
  }
  centeredText(640, 208, "SELECT A WORLD OR USE THE DESTINATION BUTTONS", 12,
               GREYBLUE);
}

void drawCockpit() {

  color({.065f, .12f, .18f});
  glBegin(GL_POLYGON);
  vertex(0, 0);
  vertex(0, 212);
  vertex(120, 180);
  vertex(355, 105);
  vertex(470, 0);
  glEnd();
  glBegin(GL_POLYGON);
  vertex(810, 0);
  vertex(925, 105);
  vertex(1160, 180);
  vertex(1280, 212);
  vertex(1280, 0);
  glEnd();
  rectangle({0, 0, 1280, 105}, DARKBLUE);
  line(0, 212, 120, 180, BLUE);
  line(120, 180, 355, 105, BLUE);
  line(1160, 180, 1280, 212, BLUE);
  line(925, 105, 1160, 180, BLUE);
  for (int i = 0; i < BODY_COUNT; ++i)
    rectangle({486.0f + i * 36, 38, 20, 5}, i < visitedCount() ? CYAN : BLUE);
  centeredText(640, 71, to_string(visitedCount()) + " / 9 WORLDS LOGGED", 14,
               CYAN);
  centeredText(640, 15, "KEEP THE ORIGINAL WINDOW SIZE: 1280 X 800", 10,
               GREYBLUE);
}

void drawFactPanel() {
  const Body &b = bodies[selectedBody];
  rectangle(FACT_PANEL, DARKBLUE);
  outline(FACT_PANEL, BLUE);
  text(732, 657, b.name, 27, WHITE);
  text(1020, 661, b.type, 12, CYAN);
  const char *labels[] = {"1 BASICS", "2 WORLD", "3 DETAILS", "4 DISCOVER",
                          "5 HIDDEN"};
  for (int i = 0; i < PAGE_COUNT; ++i)
    button(pageButton(i), labels[i], i == factPage, 11);
  line(732, 579, 1227, 579, BLUE);
  const char *titles[] = {"COMMON FACTS", "ABOUT THIS WORLD", "A CLOSER LOOK",
                          "DISCOVERY NOTE", "LESSER-KNOWN FACT"};
  text(735, 549, titles[factPage], 13, GOLD);
  if (factPage == 0) {
    string row;
    float y = 510;

    for (int i = 0; b.stats[i] != '\0'; ++i) {
      if (b.stats[i] == '\n') {
        text(735, y, row, 17);
        y -= 39;
        row.clear();
      } else
        row += b.stats[i];
    }
    if (!row.empty())
      text(735, y, row, 17);
    text(735, 322,
         selectedBody >= 5 ? "1 bar = a reference level in the atmosphere."
                           : "Rotation = one spin, relative to the stars.",
         12, GREYBLUE);
    text(735, 300, "Values are rounded; art and motion are illustrative.", 12,
         GREYBLUE);
  } else
    wrappedText(735, 510, 486, b.facts[factPage - 1]);
  text(735, 266, "FACTS READ: " + to_string(readCount(selectedBody)) + " / 5",
       12, CYAN);
  text(1092, 266, "SOURCE: NASA", 11, GREYBLUE);
}

void drawDetail() {
  circle(CLOSE_X, CLOSE_Y, CLOSE_R + 32, CLOSE_R + 32, BLUE, false);
  line(142, 464, 162, 464, CYAN);
  line(554, 464, 574, 464, CYAN);
  line(358, 248, 358, 268, CYAN);
  line(358, 660, 358, 680, CYAN);
  drawBody(selectedBody, CLOSE_X, CLOSE_Y, CLOSE_R);
  centeredText(CLOSE_X, 220,
               "OBSERVATION LOCK  /  " + string(bodies[selectedBody].name), 14,
               CYAN);
  drawFactPanel();
}

void drawFlightControls() {
  button(CONTROL_TAB, "FLIGHT CONTROLS  [F]", controlsOpen, 14);

  color(CYAN);
  glBegin(GL_TRIANGLES);
  vertex(332, controlsOpen ? 51 : 40);
  vertex(342, controlsOpen ? 51 : 40);
  vertex(337, controlsOpen ? 42 : 49);
  glEnd();
  if (!controlsOpen)
    return;
  rectangle(CONTROL_PANEL, DARKBLUE);
  outline(CONTROL_PANEL, BLUE);
  const char *instructions[] = {
      "CLICK  Select a world / button", "1 - 5  Read facts in cockpit",
      "ESC    Return to map",           "SPACE  Pause",
      "+ / -  Adjust animation speed",  "O      Show / hide paths",
      "L      Toggle explorer's log",   "R      Reset view; keep log"};
  for (int i = 0; i < 8; ++i)
    text(47, 294 - i * 23.0f, instructions[i], 12, GREYBLUE);
  button({47, 80, 104, 28}, paused ? "RESUME" : "PAUSE", paused, 11);
  button({160, 80, 77, 28}, "SLOWER", false, 10);
  button({246, 80, 91, 28}, "FASTER", false, 10);
}

void drawLog() {
  button(LOG_TAB, "EXPLORER'S LOG  [L]", logOpen, 14);
  if (!logOpen)
    return;
  rectangle(LOG_PANEL, DARKBLUE);
  outline(LOG_PANEL, BLUE);
  text(961, 369, "DESTINATION", 12, GREYBLUE);
  text(1137, 369, "FACTS", 12, GREYBLUE);
  for (int i = 0; i < BODY_COUNT; ++i) {
    float y = 334 - i * 28.0f;
    circle(968, y + 5, 3, 3, visited[i] ? CYAN : BLUE, true);
    text(982, y, bodies[i].name, 14, visited[i] ? WHITE : GREYBLUE);
    text(1143, y, visited[i] ? to_string(readCount(i)) + " / 5" : "--", 13,
         visited[i] ? CYAN : GREYBLUE);
  }
  text(961, 79, "Entries unlock when you arrive.", 11, GREYBLUE);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  rectangle({0, 0, WIDTH, HEIGHT}, BLACK);
  drawStars();
  if (view == OVERVIEW)
    drawOverview();
  else if (view == DETAIL)
    drawDetail();
  else {
    float t = zoomProgress * zoomProgress * (3 - 2 * zoomProgress);
    drawBody(selectedBody, zoomStart.x + (CLOSE_X - zoomStart.x) * t,
             zoomStart.y + (CLOSE_Y - zoomStart.y) * t,
             bodies[selectedBody].radius +
                 (CLOSE_R - bodies[selectedBody].radius) * t);
    centeredText(640, 216,
                 view == ZOOM_IN ? "APPROACHING DESTINATION..."
                                 : "RETURNING TO MAP...",
                 14, CYAN);
  }
  drawCockpit();
  rectangle({0, 715, 1280, 85}, BLACK);
  line(30, 715, 1250, 715, BLUE);
  text(30, 758, "SOLAR SPACESHIP", 27, WHITE);

  string speedText = to_string(speed);
  speedText = speedText.substr(0, speedText.find('.') + 3);
  string status = paused ? "MOTION PAUSED" : "SYSTEM ONLINE";
  status += "   /   " + speedText + "x";
  text(677, 755, status, 13, CYAN);
  if (view != OVERVIEW)
    button(BACK_BUTTON, "ESC  /  MAP", false, 12);
  for (int i = 0; i < BODY_COUNT; ++i)
    button(bodyButton(i), bodies[i].name, selectedBody == i || hoveredBody == i,
           13);
  drawFlightControls();
  drawLog();
  glFlush();
}

bool inside(float x, float y, Rect r) {
  return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
}

bool transitioning() { return view == ZOOM_IN || view == ZOOM_OUT; }

bool toScene(int px, int py, float &x, float &y) {
  if (px < 0 || py < 0 || px >= WIDTH || py >= HEIGHT) {
    return false;
  }
  x = px;
  y = HEIGHT - py;
  return true;
}

bool coveredByPanel(float x, float y) {
  return inside(x, y, CONTROL_TAB) || inside(x, y, LOG_TAB) ||
         (controlsOpen && inside(x, y, CONTROL_PANEL)) ||
         (logOpen && inside(x, y, LOG_PANEL));
}

int pickBody(float x, float y) {

  for (int i = 0; i < BODY_COUNT; ++i)
    if (inside(x, y, bodyButton(i)))
      return i;
  if (view != OVERVIEW)
    return -1;

  for (int i = BODY_COUNT - 1; i >= 0; --i) {
    Vec2 p = bodyPosition(i);
    float dx = x - p.x;
    float dy = y - p.y;
    float hitRadius = bodies[i].radius;
    if (hitRadius < 14) {
      hitRadius = 14;
    }
    if (dx * dx + dy * dy <= hitRadius * hitRadius)
      return i;
  }
  return -1;
}

void selectBody(int i) {
  if (i < 0 || i >= BODY_COUNT || transitioning())
    return;
  if (view == DETAIL && selectedBody == i)
    return;
  selectedBody = i;
  factPage = 0;
  hoveredBody = -1;
  zoomStart = bodyPosition(i);
  zoomProgress = 0;
  view = ZOOM_IN;
}

void returnToMap() {
  if (view == DETAIL || view == ZOOM_IN)
    view = ZOOM_OUT;
}

void changePage(int page) {
  if (view != DETAIL || page < 0 || page >= PAGE_COUNT)
    return;
  factPage = page;
  pagesRead[selectedBody][page] = true;
}

void changeSpeed(float amount) {
  speed += amount;
  if (speed < .25f) {
    speed = .25f;
  } else if (speed > 4.0f) {
    speed = 4.0f;
  }
}

void resetView() {
  view = OVERVIEW;
  selectedBody = -1;
  hoveredBody = -1;
  factPage = 0;
  zoomProgress = 0;
  spin = 0;
  speed = 1;
  paused = false;
  for (int i = 0; i < BODY_COUNT; ++i)
    angles[i] = bodies[i].startAngle;
}

void keyboard(unsigned char key, int, int) {
  if (key >= '1' && key <= '5')
    changePage(key - '1');
  else
    switch (key) {
    case 27:
      returnToMap();
      break;
    case ' ':
      paused = !paused;
      break;
    case 'f':
    case 'F':
      controlsOpen = !controlsOpen;
      break;
    case 'l':
    case 'L':
      logOpen = !logOpen;
      break;
    case 'o':
    case 'O':
      showOrbits = !showOrbits;
      break;
    case '+':
    case '=':
      changeSpeed(.25f);
      break;
    case '-':
    case '_':
      changeSpeed(-.25f);
      break;
    case 'r':
    case 'R':
      resetView();
      break;
    default:
      break;
    }
  glutPostRedisplay();
}

void mouse(int buttonId, int state, int px, int py) {
  if (buttonId != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
    return;
  mousePixelX = px;
  mousePixelY = py;
  float x, y;
  if (!toScene(px, py, x, y))
    return;

  if (inside(x, y, CONTROL_TAB))
    controlsOpen = !controlsOpen;
  else if (inside(x, y, LOG_TAB))
    logOpen = !logOpen;
  else if (controlsOpen && inside(x, y, CONTROL_PANEL)) {
    if (inside(x, y, {47, 80, 104, 28}))
      paused = !paused;
    else if (inside(x, y, {160, 80, 77, 28}))
      changeSpeed(-.25f);
    else if (inside(x, y, {246, 80, 91, 28}))
      changeSpeed(.25f);
  } else if (logOpen && inside(x, y, LOG_PANEL)) {
  } else if (view != OVERVIEW && inside(x, y, BACK_BUTTON))
    returnToMap();
  else if (view == DETAIL && inside(x, y, FACT_PANEL)) {
    for (int i = 0; i < PAGE_COUNT; ++i)
      if (inside(x, y, pageButton(i)))
        changePage(i);
  } else if (!transitioning())
    selectBody(pickBody(x, y));
  glutPostRedisplay();
}

void updateHover() {
  float x, y;
  hoveredBody = -1;
  if (!transitioning() && toScene(mousePixelX, mousePixelY, x, y) &&
      !coveredByPanel(x, y))
    hoveredBody = pickBody(x, y);
}

void passiveMotion(int px, int py) {
  mousePixelX = px;
  mousePixelY = py;
  updateHover();
  glutPostRedisplay();
}

void mouseEntry(int state) {
  if (state == GLUT_LEFT) {
    mousePixelX = -1;
    mousePixelY = -1;
    hoveredBody = -1;
  }
}

void update() {
  if (!paused) {
    spin += .32f * speed;
    if (spin >= 360) {
      spin -= 360;
    }
    if (view == OVERVIEW) {
      for (int i = 1; i < BODY_COUNT; i++) {
        angles[i] += .016f * bodies[i].orbitSpeed * speed;
        if (angles[i] >= 360) {
          angles[i] -= 360;
        }
      }
    }
  }
  if (view == ZOOM_IN) {
    zoomProgress += .02f;
    if (zoomProgress >= 1) {
      zoomProgress = 1;
      view = DETAIL;
      visited[selectedBody] = true;
      pagesRead[selectedBody][0] = true;
    }
  } else if (view == ZOOM_OUT) {
    zoomProgress -= .02f;
    if (zoomProgress <= 0) {
      zoomProgress = 0;
      view = OVERVIEW;
      selectedBody = -1;
    }
  }
  updateHover();
}

void timer(int) {
  update();
  glutPostRedisplay();
  glutTimerFunc(16, timer, 0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(1280, 800);
  glutInitWindowPosition(20, 20);
  glutCreateWindow("Solar Spaceship");
  glLineWidth(1);
  resetView();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutPassiveMotionFunc(passiveMotion);
  glutMotionFunc(passiveMotion);
  glutEntryFunc(mouseEntry);
  glutTimerFunc(16, timer, 0);
  glutMainLoop();
  return 0;
}

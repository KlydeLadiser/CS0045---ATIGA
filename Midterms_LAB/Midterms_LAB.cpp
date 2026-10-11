
#define GL_SILENCE_DEPRECATION
#ifdef _WIN32
#define NOMINMAX
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>
#include <string>

const float PI = 3.14159265359f;
const float WIDTH = 1280.0f, HEIGHT = 800.0f;
const int BODY_COUNT = 9, PAGE_COUNT = 5, STAR_COUNT = 360;
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
  float radius, orbit, startAngle, orbitSpeed; // visual units, degrees/second
  const char *stats;                           // page 1: common facts
  const char *facts[4];                        // pages 2-5
};

const Color INK = {0.025f, 0.045f, 0.080f};
const Color PANEL = {0.045f, 0.085f, 0.135f};
const Color EDGE = {0.14f, 0.29f, 0.38f};
const Color CYAN = {0.32f, 0.88f, 0.92f};
const Color WHITE = {0.88f, 0.94f, 0.96f};
const Color MUTED = {0.51f, 0.66f, 0.73f};
const Color GOLD = {1.0f, 0.72f, 0.31f};

// Equatorial diameters and gravity including rotation: NASA metric fact sheet.
// Gravity for giant planets is at the 1-bar atmosphere level, not solid ground.
// Signs of rotation are written as words so a negative time isn't misleading.
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
    {"Uranus", "ICE GIANT", {.43f, .81f, .85f}, 19, 437, 320, 4,
     "Diameter: 51,118 km\nRotation: 17.2 hours, retrograde\nYear: about 84 "
     "Earth years\nGravity: 8.7 m/s^2 (1 bar)",
     {"Uranus is an ice giant. Methane in its atmosphere absorbs red light, "
      "contributing to its blue-green appearance.",
      "Its axis is tilted by about 98 degrees. The planet rotates nearly on "
      "its side and experiences extreme seasons.",
      "Uranus has faint rings. It was the first planet discovered with a "
      "telescope, by William Herschel in 1781.",
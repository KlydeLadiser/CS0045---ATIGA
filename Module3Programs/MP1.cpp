#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

// Ship position
float ShipX = 0.0f;
float ShipY = -0.65f;

// Asteroid 1
float AsteroidAX = 0.0f;
float AsteroidAY = 0.70f;
float AsteroidASize = 0.11f;
float AsteroidASpeed = 0.008f;

// Asteroid 2
float AsteroidBX = 0.45f;
float AsteroidBY = 1.10f;
float AsteroidBSize = 0.14f;
float AsteroidBSpeed = 0.014f;

// Asteroid 3
float AsteroidCX = -0.45f;
float AsteroidCY = 1.30f;
float AsteroidCSize = 0.09f;
float AsteroidCSpeed = 0.010f;

// Game information
int score = 0;
int lives = 3;
bool GameRunning = true;


void drawText(float x, float y, const string& text)
{
    glRasterPos2f(x, y);

    for (char character : text)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_HELVETICA_18,
            character
        );
    }
}



void drawBackground()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);

    glBegin(GL_POINTS);
        glVertex2f(-0.80f,  0.50f);
        glVertex2f(-0.70f,  0.10f);
        glVertex2f(-0.75f, -0.40f);
        glVertex2f(-0.50f,  0.35f);
        glVertex2f(-0.40f, -0.20f);
        glVertex2f(-0.15f,  0.45f);
        glVertex2f( 0.10f,  0.25f);
        glVertex2f( 0.25f, -0.45f);
        glVertex2f( 0.45f,  0.40f);
        glVertex2f( 0.65f,  0.10f);
        glVertex2f( 0.80f, -0.55f);
        glVertex2f(-0.55f, -0.70f);
    glEnd();

    // Play-area border
    glColor3f(0.2f, 0.5f, 0.8f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.90f, -0.90f);
        glVertex2f( 0.90f, -0.90f);
        glVertex2f( 0.90f,  0.68f);
        glVertex2f(-0.90f,  0.68f);
    glEnd();
}


// Draw the player's spaceship
void drawShip()
{
    // Main triangular body
    glColor3f(0.0f, 0.6f, 1.0f);

    glBegin(GL_TRIANGLES);
        glVertex2f(ShipX,         ShipY + 0.18f);
        glVertex2f(ShipX - 0.10f, ShipY - 0.13f);
        glVertex2f(ShipX + 0.10f, ShipY - 0.13f);
    glEnd();

    // Left and right wings
    glColor3f(0.1f, 0.3f, 0.9f);

    glBegin(GL_QUADS);
        // Left wing
        glVertex2f(ShipX - 0.05f, ShipY + 0.02f);
        glVertex2f(ShipX - 0.16f, ShipY - 0.10f);
        glVertex2f(ShipX - 0.16f, ShipY - 0.17f);
        glVertex2f(ShipX - 0.05f, ShipY - 0.11f);

        // Right wing
        glVertex2f(ShipX + 0.05f, ShipY + 0.02f);
        glVertex2f(ShipX + 0.16f, ShipY - 0.10f);
        glVertex2f(ShipX + 0.16f, ShipY - 0.17f);
        glVertex2f(ShipX + 0.05f, ShipY - 0.11f);
    glEnd();

    // Cockpit
    glColor3f(0.8f, 0.9f, 1.0f);

    glBegin(GL_QUADS);
        glVertex2f(ShipX - 0.035f, ShipY + 0.07f);
        glVertex2f(ShipX + 0.035f, ShipY + 0.07f);
        glVertex2f(ShipX + 0.035f, ShipY - 0.01f);
        glVertex2f(ShipX - 0.035f, ShipY - 0.01f);
    glEnd();

    // Engine fire
    glColor3f(1.0f, 0.5f, 0.0f);

    glBegin(GL_TRIANGLES);
        glVertex2f(ShipX - 0.045f, ShipY - 0.12f);
        glVertex2f(ShipX + 0.045f, ShipY - 0.12f);
        glVertex2f(ShipX, ShipY - 0.22f);
    glEnd();
}


// Draw a hard-coded asteroid shape
void drawAsteroid(float x, float y, float size)
{
    glColor3f(0.55f, 0.50f, 0.45f);

    glBegin(GL_POLYGON);
        glVertex2f(x - size, y + size * 0.30f);
        glVertex2f(x - size * 0.50f, y + size);
        glVertex2f(x + size * 0.40f, y + size);
        glVertex2f(x + size, y + size * 0.40f);
        glVertex2f(x + size * 0.80f, y - size * 0.50f);
        glVertex2f(x + size * 0.20f, y - size);
        glVertex2f(x - size * 0.60f, y - size * 0.80f);
        glVertex2f(x - size, y - size * 0.20f);
    glEnd();

    // Simple crater
    glColor3f(0.30f, 0.27f, 0.25f);
    glPointSize(8.0f);

    glBegin(GL_POINTS);
        glVertex2f(x - size * 0.25f, y + size * 0.20f);
    glEnd();
}


// Display the score, lives, and controls
void drawHUD()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(-0.88f, 0.90f, "Score: " + to_string(score));
    drawText(-0.25f, 0.90f, "Lives: " + to_string(lives));
    drawText(0.22f, 0.90f, "Move: W/A/S/D   ESC: Exit");
}


// Check if the ship is touching an asteroid
bool checkCollision(float asteroidX, float asteroidY, float asteroidSize)
{
    float shipLeft = ShipX - 0.16f;
    float shipRight = ShipX + 0.16f;
    float shipBottom = ShipY - 0.18f;
    float shipTop = ShipY + 0.18f;

    float asteroidLeft = asteroidX - asteroidSize;
    float asteroidRight = asteroidX + asteroidSize;
    float asteroidBottom = asteroidY - asteroidSize;
    float asteroidTop = asteroidY + asteroidSize;

    if (shipRight > asteroidLeft &&
        shipLeft < asteroidRight &&
        shipTop > asteroidBottom &&
        shipBottom < asteroidTop)
    {
        return true;
    }

    return false;
}


// Return everything to its original value
void resetGame()
{
    ShipX = 0.0f;
    ShipY = -0.65f;

    AsteroidAX = 0.0f;
    AsteroidAY = 0.70f;

    AsteroidBX = 0.45f;
    AsteroidBY = 1.10f;

    score = 0;
    lives = 3;
    GameRunning = true;

    cout << "Game restarted!" << endl;

    glutPostRedisplay();
}


// Automatically update the game
void updateGame(int value)
{
    if (GameRunning)
    {
        // Move the two asteroids at different speeds
        AsteroidAY -= AsteroidASpeed;
        AsteroidBY -= AsteroidBSpeed;
        AsteroidCY -= AsteroidCSpeed;

        // Collision with asteroid 1
        if (checkCollision(
                AsteroidAX,
                AsteroidAY,
                AsteroidASize))
        {
            lives--;

            cout << "Asteroid 1 hit the ship! Lives remaining: "
                 << lives << endl;

            // Reset asteroid 1 to a fixed position
            AsteroidAX = 0.0f;
            AsteroidAY = 1.00f;

            if (lives <= 0)
            {
                lives = 0;
                GameRunning = false;

                cout << "GAME OVER!" << endl;
            }
        }

        // Collision with asteroid 2
        if (GameRunning &&
            checkCollision(
                AsteroidBX,
                AsteroidBY,
                AsteroidBSize))
        {
            lives--;

            cout << "Asteroid 2 hit the ship! Lives remaining: "
                 << lives << endl;

            // Reset asteroid 2 to a fixed position
            AsteroidBX = 0.45f;
            AsteroidBY = 1.20f;

            if (lives <= 0)
            {
                lives = 0;
                GameRunning = false;

                cout << "GAME OVER!" << endl;
            }
        }

        // Collision with asteroid 3
        if (GameRunning &&
            checkCollision(
                AsteroidCX,
                AsteroidCY,
                AsteroidCSize))
        {
            lives--;

            cout << "Asteroid 3 hit the ship! Lives remaining: "
                 << lives << endl;

            // Reset asteroid 3 to a fixed position
            AsteroidCX = -0.45f;
            AsteroidCY = 1.30f;

            if (lives <= 0)
            {
                lives = 0;
                GameRunning = false;

                cout << "GAME OVER!" << endl;
            }
        }

        // Asteroid 1 passed the player
        if (GameRunning &&
            AsteroidAY + AsteroidASize < -0.90f)
        {
            score += 10;

            AsteroidAX = 0.0f;
            AsteroidAY = 1.00f;
        }

        // Asteroid 2 passed the player
        if (GameRunning &&
            AsteroidBY + AsteroidBSize < -0.90f)
        {
            score += 10;

            AsteroidBX = 0.45f;
            AsteroidBY = 1.20f;
        }

        // Asteroid 3 passed the player
        if (GameRunning &&
            AsteroidCY + AsteroidCSize < -0.90f)
        {
            score += 10;

            AsteroidCX = -0.45f;
            AsteroidCY = 1.30f;
        }

        glutPostRedisplay();
    }

    // Run this function again after 16 milliseconds
    glutTimerFunc(16, updateGame, 0);
}


// Keyboard controls
void keyboard(unsigned char key, int x, int y)
{
    if (GameRunning)
    {
        switch (key)
        {
            case 'w':
            case 'W':
                ShipY += 0.06f;
                break;

            case 's':
            case 'S':
                ShipY -= 0.06f;
                break;

            case 'a':
            case 'A':
                ShipX -= 0.06f;
                break;

            case 'd':
            case 'D':
                ShipX += 0.06f;
                break;
        }
    }

    // Restart works even during game over
    if (key == 'r' || key == 'R')
    {
        resetGame();
    }

    // ESC exits the program
    if (key == 27)
    {
        exit(0);
    }

    // Hard-coded movement boundaries
    if (ShipX > 0.74f)
    {
        ShipX = 0.74f;
    }

    if (ShipX < -0.74f)
    {
        ShipX = -0.74f;
    }

    if (ShipY > 0.50f)
    {
        ShipY = 0.50f;
    }

    if (ShipY < -0.72f)
    {
        ShipY = -0.72f;
    }

    glutPostRedisplay();
}



void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    if (GameRunning)
    {
        drawShip();

        drawAsteroid(
            AsteroidAX,
            AsteroidAY,
            AsteroidASize
        );

        drawAsteroid(
            AsteroidBX,
            AsteroidBY,
            AsteroidBSize
        );
        
        drawAsteroid(
            AsteroidCX,
            AsteroidCY,
            AsteroidCSize
        );

        drawHUD();
    }
    else
    {
        glColor3f(1.0f, 0.2f, 0.2f);

        drawText(
            -0.34f,
            0.08f,
            "GAME OVER - Press R to Restart"
        );

        glColor3f(1.0f, 1.0f, 1.0f);

        drawText(
            -0.13f,
            -0.03f,
            "Final Score: " + to_string(score)
        );
    }

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 600);

    glutCreateWindow("MP1");

    glClearColor(0.02f, 0.02f, 0.10f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, updateGame, 0);

    glutMainLoop();

    return 0;
}

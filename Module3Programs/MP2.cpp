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


int PlayerColor = 1;
int BarrierColor = 2;

float BarrierX = 1.0f;
float BarrierSpeed = 0.005f;

int Score = 0;
bool GameOver = false;



void drawText(float X, float Y, const string& Text)
{
    glRasterPos2f(X, Y);

    for (char Character : Text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Character);
    }
}



void drawPlayer()
{
    if (PlayerColor == 1)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else if (PlayerColor == 2)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else if (PlayerColor == 3)
    {
        glColor3f(0.0f, 0.0f, 1.0f);
    }

    glBegin(GL_QUADS);
        glVertex2f(-0.13f, -0.33f);
        glVertex2f( 0.13f, -0.33f);
        glVertex2f( 0.13f, -0.07f);
        glVertex2f(-0.13f, -0.07f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.09f, -0.43f, "PLAYER");
}



void drawBarrier()
{
    if (BarrierColor == 1)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else if (BarrierColor == 2)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else if (BarrierColor == 3)
    {
        glColor3f(0.0f, 0.0f, 1.0f);
    }

    glBegin(GL_QUADS);
        glVertex2f(BarrierX - 0.08f, -0.50f);
        glVertex2f(BarrierX + 0.08f, -0.50f);
        glVertex2f(BarrierX + 0.08f,  0.10f);
        glVertex2f(BarrierX - 0.08f,  0.10f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    if (BarrierColor == 1)
    {
        drawText(BarrierX - 0.025f, -0.22f, "R");
    }
    else if (BarrierColor == 2)
    {
        drawText(BarrierX - 0.025f, -0.22f, "G");
    }
    else if (BarrierColor == 3)
    {
        drawText(BarrierX - 0.025f, -0.22f, "B");
    }
}



void drawDashboard()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(-0.90f, 0.90f, "COLOR MATCH BARRIER");
    drawText(-0.90f, 0.80f, "Score: " + to_string(Score));

    if (PlayerColor == 1)
    {
        drawText(-0.40f, 0.80f, "Player: RED");
    }
    else if (PlayerColor == 2)
    {
        drawText(-0.40f, 0.80f, "Player: GREEN");
    }
    else if (PlayerColor == 3)
    {
        drawText(-0.40f, 0.80f, "Player: BLUE");
    }

    if (BarrierColor == 1)
    {
        drawText(0.25f, 0.80f, "Barrier: RED");
    }
    else if (BarrierColor == 2)
    {
        drawText(0.25f, 0.80f, "Barrier: GREEN");
    }
    else if (BarrierColor == 3)
    {
        drawText(0.25f, 0.80f, "Barrier: BLUE");
    }

    drawText(
        -0.90f,
        0.68f,
        "[R/G/B] Change Color  [SPACE] Restart  [ESC] Exit"
    );

    drawText(
        -0.90f,
        0.58f,
        "Rule: A wrong color immediately ends the game."
    );
}



bool colorsMatch()
{
    if (PlayerColor == BarrierColor)
    {
        return true;
    }

    return false;
}



bool checkGateCollision()
{
    if (BarrierX - 0.08f <= 0.13f)
    {
        return true;
    }

    return false;
}



void spawnNextBarrier()
{
    BarrierX = 1.0f;

     
    if (BarrierColor == 1)
    {
        BarrierColor = 2;
        cout << "Next barrier: GREEN" << endl;
    }
    else if (BarrierColor == 2)
    {
        BarrierColor = 3;
        cout << "Next barrier: BLUE" << endl;
    }
    else if (BarrierColor == 3)
    {
        BarrierColor = 1;
        cout << "Next barrier: RED" << endl;
    }
}



void resetGame()
{
    PlayerColor = 1;
    BarrierColor = 2;

    BarrierX = 1.0f;
    BarrierSpeed = 0.005f;

    Score = 0;
    GameOver = false;

    cout << "Game restarted!" << endl;

    glutPostRedisplay();
}


void updateBarrier(int Value)
{
    if (GameOver == false)
    {
        BarrierX = BarrierX - BarrierSpeed;

        if (checkGateCollision())
        {
            if (colorsMatch())
            {
                Score = Score + 1;

                cout << "Correct match!" << endl;
                cout << "Score: " << Score << endl;

                BarrierSpeed = BarrierSpeed + 0.001f;

                if (BarrierSpeed > 0.018f)
                {
                    BarrierSpeed = 0.018f;
                }

                spawnNextBarrier();
            }
            else
            {
                GameOver = true;

                cout << "Wrong color!" << endl;
                cout << "GAME OVER!" << endl;
            }
        }

        glutPostRedisplay();
    }

    glutTimerFunc(16, updateBarrier, 0);
}


// Keyboard controls
void keyboard(unsigned char Key, int X, int Y)
{
    switch (Key)
    {
        case 'r':
        case 'R':
            PlayerColor = 1;
            cout << "Player color: RED" << endl;
            break;

        case 'g':
        case 'G':
            PlayerColor = 2;
            cout << "Player color: GREEN" << endl;
            break;

        case 'b':
        case 'B':
            PlayerColor = 3;
            cout << "Player color: BLUE" << endl;
            break;

        case ' ':
            resetGame();
            break;

        case 27:
            exit(0);
    }

    glutPostRedisplay();
}


// Display everything
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawPlayer();
    drawBarrier();
    drawDashboard();

    if (GameOver)
    {
        glColor3f(1.0f, 0.2f, 0.2f);

        drawText(
            -0.35f,
            0.35f,
            "GAME OVER - Press SPACE to Restart"
        );

        glColor3f(1.0f, 1.0f, 1.0f);

        drawText(
            -0.12f,
            0.25f,
            "Final Score: " + to_string(Score)
        );
    }

    glutSwapBuffers();
}


int main(int Argc, char** Argv)
{
    glutInit(&Argc, Argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 600);

    glutCreateWindow("Color Match Barrier");

    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, updateBarrier, 0);

    glutMainLoop();

    return 0;
}

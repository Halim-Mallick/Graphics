#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

double Txval = 0, Tyval = 0, Tzval = 0;
double windowHeight = 700, windowWidth = 700;
GLfloat alpha = 0.0, theta = 0.0, axis_x = 0.0, axis_y = 0.0;
GLboolean bRotate = false, uRotate = false;

static GLfloat v_cube[8][3] = {
    {0.0, 0.0, 0.0},
    {3.0, 0.0, 0.0},
    {3.0, 3.0, 0.0},
    {0.0, 3.0, 0.0},
    {0.0, 0.0, 3.0},
    {3.0, 0.0, 3.0},
    {3.0, 3.0, 3.0},
    {0.0, 3.0, 3.0}
};

static GLubyte cubeIndices[6][4] = {
    {0, 1, 2, 3}, // Front face
    {4, 5, 6, 7}, // Back face
    {0, 1, 5, 4}, // Bottom face
    {3, 2, 6, 7}, // Top face
    {0, 3, 7, 4}, // Left face
    {1, 2, 6, 5}  // Right face
};

static GLfloat colors[6][3] = {
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 1.0, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 1.0}
};

void drawCube()
{
    glBegin(GL_QUADS);
    for (GLint i = 0; i < 6; i++)
    {
        glColor3fv(colors[i]);
        for (GLint j = 0; j < 4; j++)
        {
            glVertex3fv(v_cube[cubeIndices[i][j]]);
        }
    }
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-3, 3, -3, 3, 1, 20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 5, 10, 1.5, 1.5, 1.5, 0, 1, 0);

    glViewport(0, 0, windowHeight, windowWidth);
    glPushMatrix();
    glTranslatef(0, 0, Tzval);
    glRotatef(alpha, axis_x, axis_y, 0.0);
    glRotatef(theta, axis_x, axis_y, 0.0);
    drawCube();
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 's':
    case 'S':
        bRotate = !bRotate;
        uRotate = false;
        axis_x = 0.0;
        axis_y = 1.0;
        break;

    case 'r':
    case 'R':
        uRotate = !uRotate;
        bRotate = false;
        axis_x = 1.0;
        axis_y = 0.0;
        break;

    case '+':
        Tzval += 0.2;
        break;

    case '-':
        Tzval -= 0.2;
        break;

    case 27: // Escape key
        exit(1);
    }
}

void animate()
{
    if (bRotate)
    {
        theta += 0.2;
        if (theta > 360.0)
            theta -= 360.0;
    }

    if (uRotate)
    {
        alpha += 0.2;
        if (alpha > 360.0)
            alpha -= 360.0;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Simplified Cube Demo");

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);

    glutMainLoop();
    return 0;
}

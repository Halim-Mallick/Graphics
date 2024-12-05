#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<math.h>

// ট্রান্সলেশন এবং রোটেশন ভ্যালু
double Txval=0, Tyval=0, Tzval=0;
double windowHeight=700, windowWidth=700;
GLfloat alpha=0.0, theta=0.0,  axis_x=0.0, axis_y=0.0;
GLboolean bRotate=false, uRotate=false;

// কিউব এর শীর্ষ ৮টি শীর্ষস্থান (ভেরটেক্স)
static GLfloat v_cube[8][3]=
{
    {-1.0, -1.0, -1.0},
    { 1.0, -1.0, -1.0},
    { 1.0,  1.0, -1.0},
    {-1.0,  1.0, -1.0},
    {-1.0, -1.0,  1.0},
    { 1.0, -1.0,  1.0},
    { 1.0,  1.0,  1.0},
    {-1.0,  1.0,  1.0}
};

// কিউবের পৃষ্ঠগুলো তৈরি করার জন্য ইনডেক্স
static GLubyte cubeIndices[6][4] =
{
    {0, 1, 2, 3},  // পিছনের পৃষ্ঠ
    {4, 5, 6, 7},  // সামনে
    {0, 1, 5, 4},  // নীচে
    {3, 2, 6, 7},  // উপরে
    {0, 3, 7, 4},  // বাম
    {1, 2, 6, 5}   // ডান
};

// কিউবের পৃষ্ঠের রং
static GLfloat colors[6][3]=
{
    {1.0, 0.0, 0.0}, // লাল
    {0.0, 1.0, 0.0}, // সবুজ
    {0.0, 0.0, 1.0}, // নীল
    {1.0, 1.0, 0.0}, // হলুদ
    {1.0, 0.0, 1.0}, // গোলাপী
    {0.0, 1.0, 1.0}  // আকাশি
};

// নরমাল ভেক্টর হিসাব করার ফাংশন
void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2 - x1;
    Uy = y2 - y1;
    Uz = z2 - z1;

    Vx = x3 - x2;
    Vy = y3 - y2;
    Vz = z3 - z2;

    Nx = Uy * Vz - Uz * Vy;
    Ny = Uz * Vx - Ux * Vz;
    Nz = Ux * Vy - Uy * Vx;

    glNormal3f(Nx, Ny, Nz);  // নরমাল ভেক্টর সেট
}

// কিউব আঁকার ফাংশন
void drawCube()
{
    glBegin(GL_QUADS); // পৃষ্ঠ আঁকতে GL_QUADS ব্যবহার করা হয়
    for (GLint i = 0; i < 6; i++) // ৬টি পৃষ্ঠের জন্য লুপ
    {
        glColor3f(colors[i][0], colors[i][1], colors[i][2]); // পৃষ্ঠের রং
        // পৃষ্ঠের ৪টি কোণের জন্য ইনডেক্স ব্যবহার
        getNormal3p(v_cube[cubeIndices[i][0]][0], v_cube[cubeIndices[i][0]][1], v_cube[cubeIndices[i][0]][2],
                    v_cube[cubeIndices[i][1]][0], v_cube[cubeIndices[i][1]][1], v_cube[cubeIndices[i][1]][2],
                    v_cube[cubeIndices[i][2]][0], v_cube[cubeIndices[i][2]][1], v_cube[cubeIndices[i][2]][2]);

        glVertex3fv(&v_cube[cubeIndices[i][0]][0]);
        glVertex3fv(&v_cube[cubeIndices[i][1]][0]);
        glVertex3fv(&v_cube[cubeIndices[i][2]][0]);
        glVertex3fv(&v_cube[cubeIndices[i][3]][0]);
    }
    glEnd();
}

// ডিসপ্লে ফাংশন
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // স্ক্রীন পরিষ্কার করা

    glMatrixMode(GL_PROJECTION);  // প্রজেকশন ম্যাট্রিক্স সেট
    glLoadIdentity();
    glFrustum(-5, 5, -5, 5, 4, 50);  // দৃশ্যের ক্ষেত্র

    glMatrixMode(GL_MODELVIEW);  // মডেলভিউ ম্যাট্রিক্স সেট
    glLoadIdentity();
    gluLookAt(2, 3, 10, 2, 0, 0, 0, 1, 0);  // ক্যামেরার অবস্থান ও দিক

    glViewport(0, 0, windowHeight, windowWidth);  // উইন্ডোর আকার

    glPushMatrix();
    glTranslatef(0, 0, Tzval);  // Z-axis এ ট্রান্সলেট করা
    glRotatef(alpha, axis_x, axis_y, 0.0);  // রোটেশন প্রয়োগ
    glRotatef(theta, axis_x, axis_y, 0.0);  // দ্বিতীয় রোটেশন প্রয়োগ
    drawCube();  // কিউব আঁকা

    glPopMatrix();
    glFlush();  // OpenGL কমান্ড কার্যকরী করা
    glutSwapBuffers();  // বাফার এক্সচেঞ্জ (ডাবল বাফারিং)
}

// কীবোর্ড ইনপুটের জন্য ফাংশন
void myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 's':
    case 'S':
        bRotate = !bRotate;
        uRotate = false;
        axis_x = 0.0;
        axis_y = 0.0;
        break;

    case 'r':
    case 'R':
        uRotate = !uRotate;
        bRotate = false;
        axis_x = 1.0;
        axis_y = 0.0;
        break;
    case '+':
        Tzval += 0.2;  // Z-axis zoom in
        break;
    case '-':
        Tzval -= 0.2;  // Z-axis zoom out
        break;

    case 27:  // Escape key
        exit(1);  // প্রোগ্রাম বন্ধ করা
    }
}

// অ্যানিমেশন ফাংশন
void animate()
{
    if (bRotate == true)
    {
        theta += 0.2;
        if (theta > 360.0)
            theta -= 360.0 * floor(theta / 360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.2;
        if (alpha > 360.0)
            alpha -= 360.0 * floor(alpha / 360.0);
    }
    glutPostRedisplay();  // ডিসপ্লে পুনরায় রেন্ডার করা
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // ডাবল বাফারিং এবং ডিপথ টেস্টিং

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowHeight, windowWidth);  // উইন্ডোর আকার
    glutCreateWindow("Cube-Demo");

    glShadeModel(GL_SMOOTH);  // স্মুথ শেডিং
    glEnable(GL_DEPTH_TEST);  // ডিপথ টেস্টিং
    glEnable(GL_NORMALIZE);  // নরমালাইজেশন চালু করা

    glutKeyboardFunc(myKeyboardFunc);  // কীবোর্ড ইনপুট সেট করা
    glutDisplayFunc(display);  // ডিসপ্লে ফাংশন কল করা
    glutIdleFunc(animate);  // অ্যানিমেশন ফাংশন কল করা

    glutMainLoop();  // মেইন লুপে চলে যাওয়া
}

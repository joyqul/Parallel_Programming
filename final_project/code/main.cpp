#include <iostream>
#include <cstdlib> // standard definitions
#include <cmath> // math definitions
#include <cstdio> // standard I/O
#include <vector>
#include "scene.h"

// include files are in a slightly different location for MacOS
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// escape key (for exit)
#define ESC 27

// Camera position
bool pause=false;
float x = 0.0, y = -16.0; // initially 16 units south of origin
float deltaMove = 0.0; // initially camera doesn't move
float deltaLRshift=0.0;

// Camera direction
float lx = 0.0, ly = 1.0; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

// Balls
std::vector<BALL> ball;
std::vector<PLANE> plane;
std::vector<CYLINDER> cylinder;

void changeSize(int w, int h) {
    float ratio =  ((float) w) / ((float) h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}
float balldx=-0.003;
float balldy=-0.003;
float balldz=-0.003;
void moveBall() {
    if(pause)
        return;
    // ball-ball collapse
    for(int j=0;j<ball.size();++j)
        ball[j].wirte_tmp_v();
    // sync
    for(int j=0;j<ball.size();++j)
    {
        ball[j].detectBalls(j,ball);
    }
    // sync
    for(int j=0;j<ball.size();++j)
        ball[j].update_speed();
    // sync

    // plane-ball collapse
    for(int j=0; j < plane.size(); ++j) // plane
    {
        for(int i=0; i < ball.size(); ++i) // ball
        {
            plane[j].collapse(ball[i]);
        }
    }

    for(int j=0; j < cylinder.size(); ++j)
    {
        for(int i=0; i < ball.size(); ++i) // ball
        {
            cylinder[j].collapse(ball[i]);
        }
    }

    // ball move
    for (int i = 0; i < ball.size(); ++i) {
        ball[i].x += ball[i].v[0];
        ball[i].y += ball[i].v[1];
        ball[i].z += ball[i].v[2];
    }
}

void drawBall() {
    for (int i = 0; i < ball.size(); ++i) {
        glColor3f(ball[i].r, ball[i].g, ball[i].b); // set drawing color to white
        glPushMatrix();
        glTranslatef(ball[i].x, ball[i].y, ball[i].z);
        glutSolidSphere(ball[i].radius, 10, 10);
        glPopMatrix();
    }
}

void drawGround()
{
    if(!plane.size())
    {
        PLANE tmp;
        tmp.init();
        plane.push_back(tmp);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glColor4f(plane[0].r,plane[0].g,plane[0].b,plane[0].alpha); // set drawing color
    glTranslatef(plane[0].x,plane[0].y,plane[0].z); // location

    glBegin(GL_QUADS);
        int y;
        for(y=0;y<4;++y)
            glVertex3f(plane[0].loc[y][0],plane[0].loc[y][1],plane[0].loc[y][2]);
        /*/
		//bl
		glVertex3f(-10.0,-10.0,0.0);
		//br
		glVertex3f(10.0,-10.0,0.0);
		//tr
		glVertex3f(10.0,10.0,0.0);
		//tl
		glVertex3f(-10.0,10.0,0.0);// */

	glEnd();
}
GLUquadricObj *quadratic;
void drawCylinder()
{
    if(!cylinder.size())
    {
        CYLINDER tmp;
        tmp.init();
        cylinder.push_back(tmp);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glColor4f(cylinder[0].r,cylinder[0].g,cylinder[0].b,cylinder[0].alpha); // set drawing color
    glTranslatef(cylinder[0].bx,cylinder[0].by,cylinder[0].bz); // location

    quadratic = gluNewQuadric();
    //glRotatef(0.0, 0.0, 1.0, 0.0);
    gluCylinder(quadratic,cylinder[0].radius,cylinder[0].radius,cylinder[0].height,32,32); // GLUquadricObj*, baseR, topR, height,
}

void update(void) {
    if (deltaMove!=0.0f)
    { // update camera position
        x += deltaMove * lx * 0.125;
        y += deltaMove * ly * 0.125;
    }
    if (deltaLRshift)
    { // update camera position
        x -= deltaLRshift * ly;
        y += deltaLRshift * lx;
    }
    glutPostRedisplay(); // redisplay everything
}
float up_dx=0.0;
float up_dy=0.0;
float up_dz=1.0;
void renderScene(void) {
    int i, j;

    // Clear color and depth buffers
    glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt( x,      y,        1.0,
               x + lx, y + ly,   1.0,
               up_dx ,  up_dy, up_dz);

    moveBall();
    drawGround();
    drawBall();
    drawCylinder();


    glutSwapBuffers(); // Make it all visible
}


void processNormalKeys(unsigned char key, int xx, int yy) {
    if (key == ESC || key == 'q' || key == 'Q') exit(0);
    if(key == 'p')
    {
        pause=!pause;
    }
    if (key == 'a') {
        BALL tmp;
        tmp.init();
        tmp.v[0]=balldx;
        tmp.v[1]=balldy;
        tmp.v[2]=balldz;
        ball.push_back(tmp);
    }
    if (key == 'A') {
        for (int i = 0; i < 1000; ++i) {
            BALL tmp;
            tmp.init();
            tmp.v[0]=balldx + 0.0001*i;
            tmp.v[1]=balldy + 0.0001*i;
            tmp.v[2]=balldz + 0.0001*i;
            ball.push_back(tmp);
        }
    }
    if(key=='x')
    {
        balldx-=0.001;
        printf("dx: %f\t\n",balldx);
    }
    if(key=='c')
    {
        balldy-=0.001;
        printf("dy: %f\t\n",balldy);
    }
    if(key=='v')
    {
        balldz-=0.001;
        printf("dz: %f\t\n",balldz);
    }
    if(key=='s')
    {
        balldx+=0.001;
        printf("dx: %f\t\n",balldx);
    }
    if(key=='d')
    {
        balldy+=0.001;
        printf("dy: %f\t\n",balldy);
    }
    if(key=='f')
    {
        balldz+=0.001;
        printf("dz: %f\t\n",balldz);
    }
    if(key=='g')
    {
        lx+=0.125;
        printf("lx: %f\t\n",lx);
    }
    if(key=='b')
    {
        lx-=0.125;
        printf("lx: %f\t\n",lx);
    }
    if(key=='h')
    {
        ly+=0.125;
        printf("ly: %f\t\n",ly);
    }
    if(key=='n')
    {
        ly-=0.125;
        printf("ly: %f\t\n",ly);
    }
    if(key=='j')
    {
        x+=0.125;
        printf("x: %f\t\n",x);
    }
    if(key=='m')
    {
        x-=0.125;
        printf("x: %f\t\n",x);
    }
    if(key=='k')
    {
        y+=0.125;
        printf("y: %f\t\n",y);
    }
    if(key==',')
    {
        y-=0.125;
        printf("y: %f\t\n",y);
    }
    if(key=='X')
    {
        up_dx-=0.125;
        printf("up_dx: %f\t\n",up_dx);
    }
    if(key=='C')
    {
        up_dy-=0.125;
        printf("up_dy: %f\t\n",up_dy);
    }
    if(key=='V')
    {
        up_dz-=0.125;
        printf("up_dz: %f\t\n",up_dz);
    }
    if(key=='S')
    {
        up_dx+=0.125;
        printf("up_dx: %f\t\n",up_dx);
    }
    if(key=='D')
    {
        up_dy+=0.125;
        printf("up_dy: %f\t\n",up_dy);
    }
    if(key=='F')
    {
        up_dz+=0.125;
        printf("up_dz: %f\t\n",up_dz);
    }
    if(key=='i')
    {
        printf("ball: %d\n",ball.size());
        if(ball.size())
        {
            double p[3]={0.0,0.0,0.0};
            for(int i=0;i<ball.size();++i)
            {
                for(int j=0;j<3;++j)
                {
                    p[j]+=ball[i].v[j];
                }
            }
            printf("P: %f %f %f\t\n",p[0],p[1],p[2]);
        }
    }
}


float horizon_rotate = 0;
void pressSpecialKey(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP : deltaMove = 0.125; break;
        case GLUT_KEY_DOWN : deltaMove = -0.125; break;
        case GLUT_KEY_LEFT : deltaLRshift = 0.0625; break;
            //lx = -sin(angle + (horizon_rotate+=0.005) );
            //ly = cos(angle + (horizon_rotate+=0.005) ); // rotate
        case GLUT_KEY_RIGHT : deltaLRshift = -0.0625; break;
            // lx = -sin(angle + (horizon_rotate-=0.005) );
            // ly = cos(angle + (horizon_rotate-=0.005) ); // rotate
    }
}

void releaseSpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP : deltaMove = 0.0; break;
        case GLUT_KEY_DOWN : deltaMove = 0.0; break;
        case GLUT_KEY_LEFT : deltaLRshift = 0.0; break;
        case GLUT_KEY_RIGHT : deltaLRshift = 0.0; break;
    }
}

void mouseMove(int x, int y) {
    if (isDragging) { // only when dragging
        // update the change in angle
        deltaAngle = (x - xDragStart) * 0.005;

        // camera's direction is set to angle + deltaAngle
        lx = -sin(angle + deltaAngle);
        ly = cos(angle + deltaAngle);
    }
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) { // left mouse button pressed
            isDragging = 1; // start dragging
            xDragStart = x; // save x where button first pressed
        }
        else  { /* (state = GLUT_UP) */
            angle += deltaAngle; // update camera turning angle
            isDragging = 0; // no longer dragging
        }
    }
}

int main(int argc, char **argv) {
    // general initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Free Collision");

    // register callbacks
    glutReshapeFunc(changeSize); // window reshape callback
    glutDisplayFunc(renderScene); // (re)display callback
    glutIdleFunc(update); // incremental update
    glutIgnoreKeyRepeat(0); // ignore key repeat when holding key down
    glutMouseFunc(mouseButton); // process mouse button push/release
    glutMotionFunc(mouseMove); // process mouse dragging motion
    glutKeyboardFunc(processNormalKeys); // process standard key clicks
    glutSpecialFunc(pressSpecialKey); // process special key pressed
                        // Warning: Nonstandard function! Delete if desired.
    glutSpecialUpFunc(releaseSpecialKey); // process special key release

    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0; // this is just to keep the compiler happy
}

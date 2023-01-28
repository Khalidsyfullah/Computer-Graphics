#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>


double windowHeight=800, windowWidth=800;
GLfloat alpha = 0.0, theta = 0.0, axis_x= 0, axis_y= 0, axis_z = 0;
GLboolean bRotate = false, uRotate = false;
float eyeX = 5.5, eyeY = 3, eyeZ = 13, centerX = 4, centerY = 0, centerZ = 0, upX = 0, upY = 1, upZ = 0;
int nearP = 5, farP = 50;
float zoomX = 1, zoomY = 1, zoomZ = 1, speed = 0;
int c1=0, c2 = 0;
double yAngle = 0;

GLboolean fan = false, bView = false, spotLight = false, Light1 = false, Light2 = false, ambL = true, diffL = true, specL = true;
GLboolean ambL1 = false, ambL2 = false, ambL3 = false, diffL1 = false, diffL2 = false, diffL3 = false, specL1 = false, specL2 = false, specL3 = false;

GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 }, light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient1[]  = { 0.6, 0.8, 0.7, 1.0 }, light_diffuse1[]  = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient2[]  = {0.0, 0.5, 0.5, 1.0}, light_diffuse2[]  = { 0.0, 1.0, 0.0, 1.0 };
GLfloat light_ambient3[]  = {0.2, 0.2, 0.2, 1.0}, light_diffuse3[]  = { 1.0, 1.0, 1.0, 1.0 };


void drawcube(float x, float y, float z, float a, float r, float g, float b)
{
    GLfloat points[8][3] =
    {
        {x, y, z},
        {x+a, y, z},
        {x+a, y, z+a},
        {x+a, y+a, z+a},
        {x, y+a, z+a},
        {x, y+a, z},
        {x, y, z+a},
        {x+a, y+a, z}
    };

    GLubyte quadIndices[6][4] =
    {
        {0, 1, 7, 5},
        {0, 6, 4, 5},
        {1, 2, 3, 7},
        {0, 1, 2, 6},
        {5, 7, 3, 4},
        {6, 2, 3, 4}
    };

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5*r, 0.5*g, 0.5*b, 1.0 };
    GLfloat mat_diffuse[] = { 1.0*r, 1.0*g, 1.0*b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 60 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_QUADS);
    for(GLint i=0; i<6; i++)
    {
        glVertex3fv(&points[quadIndices[i][0]][0]);
        glVertex3fv(&points[quadIndices[i][1]][0]);
        glVertex3fv(&points[quadIndices[i][2]][0]);
        glVertex3fv(&points[quadIndices[i][3]][0]);
    }
    glEnd();
}


void Light_Source1()
{
    glPushMatrix();
    GLfloat light_position[] = { 5, 5, 18, 0 };
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
}

void Light_Source2()
{
    glPushMatrix();
    GLfloat light_position[] = { 5, 13, 5.0, 0 };
    glLightfv( GL_LIGHT1, GL_POSITION, light_position);
    glPopMatrix();
}

void spot_light()
{
    glPushMatrix();
    GLfloat light_position[] = { 10, 0, 5, 6 };
    glLightfv( GL_LIGHT2, GL_POSITION, light_position);
    GLfloat spot_direction[] = { 10, 8, -5 };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 25.0);
    glPopMatrix();
}

void drawWall()
{

    //Floor
    glPushMatrix();
    glScalef(20, 0.5, 10);
    drawcube(0, 0, 0, 0.5, 0.0, 0.5, 0.7);
    glPopMatrix();

    //middle wall
    glPushMatrix();
    glScalef(21, 10, 0.5);
    glTranslatef(-0.012, 0, -0.5);
    drawcube(0, 0, 0, 0.5, 1.0, 0.5, 0.0);
    glPopMatrix();


    //left wall first part of fan
    glPushMatrix();
    glScalef(0.25, 5, 1);
    glTranslatef(-1, 0, 0);
    drawcube(0, 0, 0, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left wall lower part of Fan
    glPushMatrix();
    glScalef(0.25, 3.0, 4);
    drawcube(-1, 0, 0.25, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left Fan top wall
    glPushMatrix();
    glScalef(0.25, 0.8, 2.5);
    drawcube(-1, 5.25, 0.4, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left Fan side wall
    glPushMatrix();
    glScalef(0.25, 2, 2.5);
    drawcube(-1, 1.5, 1, 1, 1.0, 0.5, 0.0);
    glPopMatrix();


    //fan top border
    glPushMatrix();
    glScalef(0.125, 0.1, 1.5);
    drawcube(-2, 41, 0.66, 1, 0.5,.02,.02);
    glPopMatrix();

    //fan left border
    glPushMatrix();
    glScalef(0.125, 1.2, 0.1);
    drawcube(-2, 2.5, 9.95, 1, 0.5, 0.02, 0.02);
    glPopMatrix();

    //fan right border
    glPushMatrix();
    glScalef(0.125, 1.2, 0.1);
    drawcube(-2, 2.5, 23.95, 1, 0.5,.02,.02);
    glPopMatrix();

    //fan bottom border
    glPushMatrix();
    glScalef(0.125, 0.1, 1.5);
    drawcube(-2, 30, 0.66, 1, 0.5,.02,.02);
    glPopMatrix();

    //Right wall first part
    glPushMatrix();
    glScalef(0.5, 10, 4);
    glTranslatef(20, 0, 0);
    drawcube(0, 0, 0, 0.5, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Right wall lower part of window
    glPushMatrix();
    glScalef(0.25, 3.0, 3);
    drawcube(40, 0, 0.66, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Window top wall
    glPushMatrix();
    glScalef(0.25, 0.8, 1.5);
    drawcube(40, 5.25, 1.33, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Window side wall
    glPushMatrix();
    glScalef(0.25, 2, 1.5);
    drawcube(40, 1.5, 2.33, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

}

void drawfridge()
{
    //body
    glPushMatrix();
    glScalef(1, 4, 2);
    glTranslatef(0.2,0.0625, 1.5);
    drawcube(0,0,0, 1, 0.0, 0.0, 1.0);
    glPopMatrix();

    //Upper door
    glPushMatrix();
    glScalef(0.25, 1.6, 2);
    glTranslatef(5, 1.65, 1.5);
    drawcube(0,0,0, 1, 0.0, 1.0, 0.5);
    glPopMatrix();

    //Lower door
    glPushMatrix();
    glScalef(0.25, 2.3, 2);
    glTranslatef(5, 0.125, 1.5);
    drawcube(0,0,0, 1, 0.0, 1.0, 0.5);
    glPopMatrix();


    //Handle
    glPushMatrix();
    glScalef(0.1, 0.8, 0.12);
    glTranslatef(14.5, 3.4, 40.5);
    drawcube(0,0,0, 1, 0.1, 0.1, 0.0);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.8, 0.12);
    glTranslatef(14.5, 2.1, 40.5);
    drawcube(0,0,0, 1, 0.1, 0.1, 0.0);
    glPopMatrix();

}

void drawWallshelfDoor(float x)
{
    //draw door
    glPushMatrix();
    glScalef(1.75, 1.2, 0.01);
    glTranslatef(x, 2.85, 101);
    drawcube(0,0,0, 1, 0.1, 0.0, 0.1);
    glPopMatrix();

    //draw handle
    float y = 0.12;
    x = x* (1.75 / y) + 0.3;
    glPushMatrix();
    glScalef(y, 0.6, 0.01);
    drawcube(x,5.8, 102, 1, 1.0, 1.0, 1.0);
    glPopMatrix();
}


void drawWallshelf()
{
    //draw Body
    glPushMatrix();
    glScalef(8, 1.3,1);
    glTranslatef(0.125, 2.6, 0);
    drawcube(0,0,0, 1, 0.5, 0.5, 0.5);
    glPopMatrix();
    drawWallshelfDoor(0.68);
    drawWallshelfDoor(1.8);
    drawWallshelfDoor(2.92);
    drawWallshelfDoor(4.04);
}

void drawCabardDoor(float x, int a)
{
    //draw door
    glPushMatrix();
    glScalef(2, 1.6, 0.02);
    glTranslatef(x, 0.2, 51);
    drawcube(0,0,0, 1, 0.8,0.5, 0);
    glPopMatrix();

    //draw handle
    float y = 0.12;
    if(a == 0)
    {
        x+=1;
        x = x* (2 / y) - 2;
    }
    else
    {
        x = x* (2 / y) + 2;
    }
    glPushMatrix();
    glScalef(y, 0.8, 0.02);
    drawcube(x, 1, 54, 1, 0.1, 0.1, 0.0);
    glPopMatrix();
}


void drawCabard()
{
    //Backside
    glPushMatrix();
    glScalef(9, 2,1);
    drawcube(0, 0, 0, 1, 0.7,.08,.02);
    glPopMatrix();

    //Right Side half
    glPushMatrix();
    glScalef(1, 2, 2.5);
    glTranslatef(9, 0, 0);
    drawcube(0,0,0, 1, 0.7,.08,.02);
    glPopMatrix();


    //Sink Bottom
    glPushMatrix();
    glScalef(1, 1.25, 2);
    glTranslatef(9, 0, 1.25);
    drawcube(0,0,0, 1, 0, 1.0, 1.0);
    glPopMatrix();

    //outer cover
    glPushMatrix();
    glScalef(0.01, 2, 2);
    glTranslatef(899, 0, 1.25);
    drawcube(0,0,0, 1, 0.5,0.6,0.6);
    glPopMatrix();

    //top outer cover
    glPushMatrix();
    glScalef(1, 2, 0.025);
    glTranslatef(9, 0, 180);
    drawcube(0,0,0, 1, 0.5,0.6,0.6);
    glPopMatrix();

    //Sink left corner part
    glPushMatrix();
    glScalef(0.125, 0.75, 2);
    glTranslatef(72, 1.67, 1.25);
    drawcube(0,0,0, 1,  1.0, 1.0, 0.0);
    glPopMatrix();

    //left cover
    glPushMatrix();
    glScalef(0.125, 0.075, 2);
    glTranslatef(72, 26, 1.25);
    drawcube(0,0,0, 1, 0.5,0.6,0.6);
    glPopMatrix();

    //Sink right corner part
    glPushMatrix();
    glScalef(0.3, 0.75, 2);
    glTranslatef(32.3, 1.67, 1.25);
    drawcube(0,0,0, 1, 1.0, 1.0, 0.0);
    glPopMatrix();

    //right cover
    glPushMatrix();
    glScalef(0.3, 0.075, 2);
    glTranslatef(32.3, 26, 1.25);
    drawcube(0,0,0, 1, 0.5,0.6,0.6);
    glPopMatrix();

    //Sink top corner part
    glPushMatrix();
    glScalef(1, 0.75, 0.25);
    glTranslatef(9, 1.67, 10);
    drawcube(0,0,0, 1, 1.0, 1.0, 0.0);
    glPopMatrix();

    //top cover
    glPushMatrix();
    glScalef(1, 0.075, 0.25);
    glTranslatef(9, 26, 10);
    drawcube(0,0,0, 1, 0.5,0.6,0.6);
    glPopMatrix();

    //Sink bottom corner part
    glPushMatrix();
    glScalef(1, 0.75, 0.25);
    glTranslatef(9, 1.67, 17);
    drawcube(0,0,0, 1, 1.0, 1.0, 1.0);
    glPopMatrix();

    //bottom cover
    glPushMatrix();
    glScalef(1, 0.075, 0.25);
    glTranslatef(9, 26, 17);
    drawcube(0,0,0, 1, 0.5,0.6,0.6);
    glPopMatrix();

    //Water Tap body
    glPushMatrix();
    glScalef(0.1, 0.5, 0.1);
    glTranslatef(98.4, 4, 35);
    drawcube(0,0,0, 1, 0.1, 0.1, 0.0);
    glPopMatrix();

    //Water Tap head
    glPushMatrix();
    glScalef(0.4, 0.1, 0.1);
    glTranslatef(23.85, 25, 35);
    drawcube(0,0,0, 1, 0.1, 0.1, 0.0);
    glPopMatrix();

}

void drawDrawer(float x)
{
    //draw lower drawer
    glPushMatrix();
    glScalef(2, 0.7, 0.02);
    glTranslatef(x, 0.5, 51);
    drawcube(0,0,0, 1, 1.5,.02,.02);
    glPopMatrix();

    //draw upper drawer
    glPushMatrix();
    glScalef(2, 0.7, 0.02);
    glTranslatef(x, 1.65, 51);
    drawcube(0,0,0, 1, 1.5,.02,.02);
    glPopMatrix();

    //draw handle low
    float y = (2 * x) / 0.8 + 0.6;
    float z = (0.35 / 0.12) + 1;
    glPushMatrix();
    glScalef(0.8, 0.12, 0.02);
    drawcube(y, z, 54, 1, 0.1f, 0.1f, 0.0f);
    glPopMatrix();

    //draw handle high
    z = z+ 7.5;
    glPushMatrix();
    glScalef(0.8, 0.12, 0.02);
    drawcube(y, z, 54, 1, 0.1, 0.1, 0.0);
    glPopMatrix();
}


void drawCabardDesign()
{
    //Middle Top cover
    glPushMatrix();
    glScalef(10, 0.05,1.1);
    drawcube(0, 40, 0, 1, 1.7, 2, 0.02);
    glPopMatrix();



    drawCabardDoor(0.125, 0);
    drawCabardDoor(1.25, 1);
    drawDrawer(2.375);
    drawDrawer(3.45);
}


void drawWindow()
{
    //Window Body
    glPushMatrix();
    glScalef(0.125, 1.2, 1.5);
    drawcube(81, 2.5, 1.33, 1, 0.0, 1.0, 1.0);
    glPopMatrix();

    //top border
    glPushMatrix();
    glScalef(0.125, 0.1, 1.5);
    drawcube(80, 41, 1.33, 1, 0.5,.02,.02);
    glPopMatrix();

    //left border
    glPushMatrix();
    glScalef(0.125, 1.2, 0.1);
    drawcube(80, 2.5, 19.95, 1, 0.5, 0.02, 0.02);
    glPopMatrix();

    //right border
    glPushMatrix();
    glScalef(0.125, 1.2, 0.1);
    drawcube(80, 2.5, 33.95, 1, 0.5,.02,.02);
    glPopMatrix();

    //bottom border
    glPushMatrix();
    glScalef(0.125, 0.1, 1.5);
    drawcube(80, 30, 1.33, 1, 0.5,.02,.02);
    glPopMatrix();

    //middle border
    glPushMatrix();
    glScalef(0.125, 0.1, 1.5);
    drawcube(80, 36, 1.33, 1, 0.5,.02,.02);
    glPopMatrix();

    //middle boarder
    glPushMatrix();
    glScalef(0.125, 1.2, 0.1);
    drawcube(80, 2.5, 26.45, 1, 0.5,.02,.02);
    glPopMatrix();
}

void drawMicrowaveOven()
{
    //body
    GLfloat M[] = {0, 1,1, 1};

    glPushMatrix();
    glScalef(1, 0.75, 0.5);
    drawcube(7, 2.66, 1, 1, 0.5, 0.5, 0.5);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, M);
    glPushMatrix();
    glTranslatef(5, 2.4, 0.6);
    glutSolidTeapot(0.5);
    glPopMatrix();

    //Front
    glPushMatrix();
    glScalef(0.8, 0.6, 0.1);
    drawcube(8.8, 3.5, 10.1, 1, 0.7, 1.08,1.02);
    glPopMatrix();
}


void drawSnowMan()
{

    glPushMatrix();
    glTranslatef(5, 0.75, 3);
    glutSolidSphere(0.75, 10, 10);


    glTranslatef(0, 0.75, 0);
    glutSolidSphere(0.50, 10, 10);
    glPopMatrix();
}


void drawFan()
{
    glTranslatef(0.25, -3.55, -1.695);
    glRotatef(speed, 0, 0, 1);
    glTranslatef(-0.25, 3.55, 1.695);

    glPushMatrix();
    glScalef(0.125, 0.125, 0.125);
    drawcube(-2, 28.4, 13.56, 1, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.0625, 0.5, 0.1);
    drawcube(-3.8, 7.1, 16.95, 1, 0.0, 0.0, 0.5);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.0625, 0.5, 0.1);
    glTranslatef(-3.8, 6.1, 16.95);
    drawcube(0,0,0, 1, 0.0, 0.0, 0.5);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.0625, 0.1, 0.5);
    glTranslatef(-3.8, 35.5, 3.39);
    drawcube(0,0,0, 1, 0.0, 0.0, 0.5);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.0625, 0.1, 0.5);
    glTranslatef(-3.8, 35.5, 2.39);
    drawcube(0,0,0, 1, 0.0, 0.0, 0.5);
    glPopMatrix();


}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5, 5, -5, 5, nearP, farP);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    upX = sin(yAngle/180.0);
    upY = cos(yAngle/180.0);
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX , upY , upZ);
    glViewport(c1, c2, windowHeight, windowWidth);
    glScalef(zoomX, zoomY, zoomZ);
    glRotatef( alpha, axis_x, axis_y, axis_z );
    glRotatef( theta, axis_x, axis_y, axis_z );
    glEnable(GL_LIGHTING);
    Light_Source1();
    Light_Source2();
    spot_light();
    drawWall();
    drawfridge();
    drawWallshelf();
    drawCabard();
    drawWindow();
    drawCabardDesign();
    drawMicrowaveOven();
    drawSnowMan();
    drawFan();
    glFlush();
    glutSwapBuffers();
}





void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'q':
        eyeX--;
        break;
    case 'w':
        eyeX++;
        break;

    case 'e':
        eyeY++;
        break;
    case 'r':
        eyeY--;
        break;
    case 't':
        eyeZ--;
        break;
    case 'y':
        eyeZ++;
        break;
    case 'z':
        zoomX+= 0.1;
        zoomY+= 0.1;
        zoomZ+= 0.1;
        break;
    case 'x':
        zoomX-= 0.1;
        zoomY-= 0.1;
        zoomZ-= 0.1;
        break;
    case 'c':
        eyeX++;
        eyeZ++;
        break;
    case 'v':
        eyeX--;
        eyeZ--;
        break;
    case 'u':
        nearP++;
        break;
    case 'i':
        nearP--;
        break;

    case 'o':
        c1+= 5;
        break;
    case 'p':
        c1-= 5;
        break;
    case 'k':
        c2+= 5;
        break;
    case 'l':
        c2-= 5;
        break;

    case 'g':
        theta = 0;
        alpha = 0;
        bRotate = false;
        uRotate = false;
        break;



    case 'a':
        bRotate = !bRotate;
        uRotate = true;
        axis_x= 0;
        axis_y= 0;
        axis_z = 1;
        break;

    case 'A':
        uRotate = !uRotate;
        bRotate = true;
        axis_x = 0;
        axis_y = 1;
        axis_z = 0;
        break;

    case 's':
        uRotate = !uRotate;
        bRotate = true;
        axis_x = 1;
        axis_y = 0;
        axis_z = 0;
        break;

    case 'd':
        yAngle += 1;
        break;
    case 'D':
        yAngle -= 1;
        break;
    case 'h':
        centerY+=0.07;
        centerZ=-sqrt(eyeZ*eyeZ-(centerX-eyeX)*(centerX-eyeX)-(centerY-eyeY)*(centerY-eyeZ))+eyeY;
        break;
    case 'H':
        centerY-=0.07;
        centerZ=-sqrt(eyeZ*eyeZ-(centerX-eyeX)*(centerX-eyeX)-(centerY-eyeY)*(centerY-eyeY))+eyeY;
        break;

    case 'j':
        centerX-=0.08;
        centerZ=-sqrt(eyeZ*eyeZ-(centerX-eyeX)*(centerX-eyeX)-(centerY-eyeY)*(centerY-eyeY))+eyeZ;
        break;
    case 'J':
        centerX+=0.08;
        centerZ=-sqrt(eyeZ*eyeZ-(centerX-eyeX)*(centerX-eyeX)-(centerY-eyeY)*(centerY-eyeY))+eyeZ;
        break;

    case 'b':
        bView = !bView;
        if(bView){
            eyeX = -5;
            eyeY = 20;
            eyeZ = 5;
        }
        else{
            eyeX = 5.5;
            eyeY = 3;
            eyeZ = 13;
        }
        break;

    case '1':
        Light1 = !Light1;
        ambL1 = Light1;
        diffL1 = Light1;
        specL1 = Light1;
        if(Light1 == true)
        {
            glEnable( GL_LIGHT0);
        }
        else
        {
            glDisable( GL_LIGHT0);
        }
        break;
    case '2':
        Light2 = !Light2;
        ambL2 = Light2;
        diffL2 = Light2;
        specL2 = Light2;
        if(Light2 == true)
        {
            glEnable( GL_LIGHT1);
        }
        else
        {
            glDisable( GL_LIGHT1);
        }
        break;
    case '3':
        spotLight = !spotLight;
        ambL3 = spotLight;
        diffL3 = spotLight;
        specL3 = spotLight;
        if(spotLight == true)
        {
            glEnable( GL_LIGHT2);
        }
        else
        {
            glDisable( GL_LIGHT2);
        }
        break;

    case '4':
        ambL1 =! ambL1;
        break;
    case '7':
        ambL2 =! ambL2;
        break;
    case '0':
        ambL3 =! ambL3;
        break;
    case '5':
        diffL1 =! diffL1;
        break;
    case '8':
        diffL2 =! diffL2;
        break;
    case '-':
        diffL3 =! diffL3;
        break;
    case '6':
        specL1 =! specL1;
        break;
    case '9':
        specL2 =! specL2;
        break;
    case '=':
        specL3 =! specL3;
        break;

    case 'f':
        if(fan) fan = false;
        else fan = true;
        break;

    case 27:
        exit(1);
    }
}



void animate()
{
    if (bRotate == true)
    {
        theta += 0.05;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.05;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }

    if(ambL1 == true)
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient1);
    }
    else
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
    }
    if(diffL1 == true)
    {
        glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
    }
    else
    {
        glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
    }
    if(specL1 == true)
    {
        glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    }
    else
    {
        glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
    }

    if(ambL2 == true)
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient2);
    }
    else
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
    }
    if(diffL2 == true)
    {
        glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    }
    else
    {
        glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
    }
    if(specL2 == true)
    {
        glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
    }
    else
    {
        glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
    }

    if(ambL3 == true)
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient3);
    }
    else if(ambL3 == false)
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
    }
    if(diffL3 == true)
    {
        glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse3);
    }
    else
    {
        glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
    }
    if(specL3 == true)
    {
        glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);
    }
    else
    {
        glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
    }

    if(bView){
        eyeX+= 0.005;
    }

    if(fan) speed += 90;



    glutPostRedisplay();

}
int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Kitchen Room");
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutMainLoop();

    return 0;
}

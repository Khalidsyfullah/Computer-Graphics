#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <bits/stdc++.h>
#include "BmpLoader.h"

double Txval=0,Tyval=0,Tzval=0;
double windowHeight=1200, windowWidth=1200;
GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0, axis_z = 0;
GLboolean bRotate = false, uRotate = false;
float zoomX = 1, zoomY = 1, zoomZ = 1;
int wheel=0,angle=0;
double yAngle = 0;

unsigned int ID[100];

GLboolean fan = false, bView = false, spotLight = false, Light1 = false, Light2 = false, ambL = true, diffL = true, specL = true;
GLboolean ambL1 = false, ambL2 = false, ambL3 = false, diffL1 = false, diffL2 = false, diffL3 = false, specL1 = false, specL2 = false, specL3 = false;

GLboolean roof1 = true, roof2 = true, roof3 = true;
int doorangle = 0;
float eyeX = 15.5, eyeY = 13, eyeZ = 30, centerX = 4, centerY = 0, centerZ = 0, upX = 0, upY = 1, upZ = 0;


GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 }, light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient1[]  = { 0.6, 0.8, 0.7, 1.0 }, light_diffuse1[]  = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient2[]  = {0.0, 0.5, 0.5, 1.0}, light_diffuse2[]  = { 0.0, 1.0, 0.0, 1.0 };
GLfloat light_ambient3[]  = {0.2, 0.2, 0.2, 1.0}, light_diffuse3[]  = { 1.0, 1.0, 1.0, 1.0 };




static GLfloat Cube[8][3] =
{
    {0.0, 0.0, 0.0},    //0
    {1.0, 0.0, 0.0},    //1
    {1.0, 1.0, 0.0},    //2
    {0.0, 1.0, 0.0},    //3
    {0.0, 0.0, 1.0},    //4
    {1.0, 0.0, 1.0},    //5
    {1.0, 1.0, 1.0},    //6
    {0.0, 1.0, 1.0}     //7
};

static GLubyte Cube_indeces[6][4] =
{
    {1,5,4,0},   //bottom clockwise
    {3,7,6,2},   //top
    {0,4,7,3},   //left clockwise
    {2,6,5,1},   //right
    {4,5,6,7},   //front
    {3,2,1,0}

};

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void LoadTexture(const char*filename,int x)
{
    glGenTextures(1, &ID[x]);
    glBindTexture(GL_TEXTURE_2D, ID[x]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID[x]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

void Material(GLfloat rCol, GLfloat gCol, GLfloat bCol)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { rCol*0.5, gCol*0.5, bCol*0.5, 1.0 };
    GLfloat mat_diffuse[] = { rCol, gCol, bCol, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 100 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
}


void new_Cube()
{
    glBegin(GL_QUADS);

    GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(Cube[Cube_indeces[i][0]][0], Cube[Cube_indeces[i][0]][1], Cube[Cube_indeces[i][0]][2],
                    Cube[Cube_indeces[i][1]][0], Cube[Cube_indeces[i][1]][1], Cube[Cube_indeces[i][1]][2],
                    Cube[Cube_indeces[i][2]][0], Cube[Cube_indeces[i][2]][1], Cube[Cube_indeces[i][2]][2]);

        glVertex3fv(&Cube[Cube_indeces[i][0]][0]);
        glTexCoord2f(0,1);
        glVertex3fv(&Cube[Cube_indeces[i][1]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&Cube[Cube_indeces[i][2]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&Cube[Cube_indeces[i][3]][0]);
        glTexCoord2f(0,0);


    }

    glEnd();
}



static GLfloat p_vertex[5][3] =
{
    {0,0,0},
    {1,0,0},
    {0,0,1},
    {1,0,1},
    {0.5,1,0.5}
};


static GLubyte p_quadIndices[1][4] =
{
    {1,5,4,0}
};

static GLubyte p_triangleIndices[4][3] =
{
    {0,4,1}, //front
    {1,4,3}, //right
    {2,3,4}, //back
    {0,2,4}  //left
};

void draw_Pyramid()
{
    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <4; i++)
    {
        getNormal3p(p_vertex[p_triangleIndices[i][0]][0], p_vertex[p_triangleIndices[i][0]][1], p_vertex[p_triangleIndices[i][0]][2],
                    p_vertex[p_triangleIndices[i][1]][0], p_vertex[p_triangleIndices[i][1]][1], p_vertex[p_triangleIndices[i][1]][2],
                    p_vertex[p_triangleIndices[i][2]][0], p_vertex[p_triangleIndices[i][2]][1], p_vertex[p_triangleIndices[i][2]][2]);

        glVertex3fv(&p_vertex[p_triangleIndices[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&p_vertex[p_triangleIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&p_vertex[p_triangleIndices[i][2]][0]);
        glTexCoord2f(1,1);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (GLint i = 0; i <1; i++)
    {
        getNormal3p(p_vertex[p_quadIndices[i][0]][0], p_vertex[p_quadIndices[i][0]][1], p_vertex[p_quadIndices[i][0]][2],
                    p_vertex[p_quadIndices[i][1]][0], p_vertex[p_quadIndices[i][1]][1], p_vertex[p_quadIndices[i][1]][2],
                    p_vertex[p_quadIndices[i][2]][0], p_vertex[p_quadIndices[i][2]][1], p_vertex[p_quadIndices[i][2]][2]);

        glVertex3fv(&p_vertex[p_quadIndices[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&p_vertex[p_quadIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&p_vertex[p_quadIndices[i][2]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&p_vertex[p_quadIndices[i][3]][0]);
        glTexCoord2f(0,1);
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



void draw_park_land()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glScalef(20,0.3,20);
    glTranslatef(-0.2,-10,-0.5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glScalef(20,0.3,2);
    glTranslatef(-0.2,-10,-7.5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //Extended
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glScalef(30,0.3,20);
    glTranslatef(0.533,-10,-0.5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glScalef(30,0.3,2);
    glTranslatef(0.533,-10,-7.5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //Swimming pool area
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,12);
    glPushMatrix();
    glScalef(22,0.3,20);
    glTranslatef(0.91,-10,-1.75);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //boundary
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glScalef(26,0.3, 2);
    glTranslatef(0.6923,-10, -18.5);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(2,0.3, 22);
    glTranslatef(9,-10, -1.68);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(2,0.3, 22);
    glTranslatef(21,-10, -1.68);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void draw_park_river()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,13);
    glPushMatrix();
    glScalef(20,0.3,3);
    glTranslatef(-0.2,-10,-4.33);
    new_Cube();
    glPopMatrix();



    //Extended
    glPushMatrix();
    glScalef(30,0.3,3);
    glTranslatef(0.533,-10,-4.33);
    new_Cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);

    glPushMatrix();
    glScalef(30,0.3,2);
    glTranslatef(0.533,-10,5);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(20,0.3,2);
    glTranslatef(-0.2,-10,5);
    new_Cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}


void draw_park_sky()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(-1000,475,-400);
    glTexCoord2f(1, 1);
    glVertex3f(-1000,475,1100);
    glTexCoord2f(1, 0);
    glVertex3f(1000,475,1100);
    glTexCoord2f(0, 0);
    glVertex3f(1000,475,-400);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //right anticlockwise
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,20);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(1000,475,-400);
    glTexCoord2f(1, 1);
    glVertex3f(1000,475,1100);
    glTexCoord2f(1, 0);
    glVertex3f(1000,-125,1100);
    glTexCoord2f(0, 0);
    glVertex3f(1000,-125,-400);
    glEnd();

    //left clockwise
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);
    glVertex3f(-1000,-125,-400);
    glTexCoord2f(0, 0);
    glVertex3f(-1000,-125,1100);
    glTexCoord2f(0, 1);
    glVertex3f(-1000,475,1100);
    glTexCoord2f(1, 1);
    glVertex3f(-1000,475,-400);
    glEnd();

    //back clockwise
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(-1000,475,-400);
    glTexCoord2f(1, 1);
    glVertex3f(1000,475,-400);
    glTexCoord2f(1, 0);
    glVertex3f(1000,-125,-400);
    glTexCoord2f(0, 0);
    glVertex3f(-1000,-125,-400);
    glEnd();

    //front anticlockwise
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);
    glVertex3f(-1000,-125,1100);
    glTexCoord2f(0, 0);
    glVertex3f(1000,-125,1100);
    glTexCoord2f(0, 1);
    glVertex3f(1000,475,1100);
    glTexCoord2f(1, 1);
    glVertex3f(-1000,475,1100);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void draw_road()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 4);
    glPushMatrix();
    glScalef(1,0.2,20);
    glTranslatef(5,-14,-0.5);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(20,0.2,1);
    glTranslatef(-0.2,-13,-0.7);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(1.5,0.2,3);
    glTranslatef(3.2,-14,3.2);
    new_Cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 11);

    glPushMatrix();
    glScalef(20,0.3,2);
    glTranslatef(-0.2,-9,6);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(30,0.3,2);
    glTranslatef(0.533,-9,6);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(2,0.3, 31);
    glTranslatef(-3,-9, -0.5484);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(20,0.3,2);
    glTranslatef(-0.2,-9,-8.5);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(30,0.3,2);
    glTranslatef(0.53,-9,-19.5);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(2,0.3, 22);
    glTranslatef(8,-10, -1.68);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(2,0.3, 22);
    glTranslatef(22,-10, -1.68);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(2,0.3, 30);
    glTranslatef(23,-10, -0.533);
    new_Cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}


void draw_park_tree()
{
    GLUquadricObj *quad;
    quad = gluNewQuadric();
    glPushMatrix();
    Material(0.2,0.1,0.1);
    glRotatef(90,1,0,0);
    gluCylinder(quad, 0.5, 0.5, 5, 10, 10);
    glPopMatrix();

    float tx = 0, ty = 0, tz = 0, sx = 1, sy = 1, sz = 1;
    for(int i=0; i<10; i++)
    {
        glPushMatrix();
        glTranslatef(tx,ty,tz);
        Material(0, 0.5,0);
        glRotatef(-90,1,0,0);
        glScalef(sx, sy, sz);
        glutSolidCone(3-i,3,10,10);
        glPopMatrix();
        ty += 1.5;// sx -= 0.08;
    }
}

void tree()

{

    int tx=0;
    for(int i=0; i<6; i++)
    {
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(-10+tx,-3.5,-45);
        draw_park_tree();
        glPopMatrix();
        tx+=5;
    }

    tx=0;
    for(int i=0; i<6; i++)
    {
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(22+tx,-3.5,-45);
        draw_park_tree();
        glPopMatrix();
        tx+=5;
    }

    tx=0;
    for(int i=0; i<14; i++)
    {
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(50,-3.5,-28+tx);
        draw_park_tree();
        glPopMatrix();
        tx+=5;
    }

    tx=0;
    for(int i=0; i<16; i++)
    {
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(-10,-3.5,-38+tx);
        draw_park_tree();
        glPopMatrix();
        tx+=5;
    }

}




void draw_gate()
{
    glPushMatrix();
    GLUquadricObj *quad;
    quad = gluNewQuadric();
    glRotatef(90,1,0,0);
    Material(0,0,0);
    gluCylinder(quad, 0.5, 0.5, 3, 10, 10);
    glPopMatrix();
}

void gate()
{
    glPushMatrix();
    glScalef(1,2,1);
    glTranslatef(3,1.5,13);
    draw_gate();
    glPopMatrix();

    glPushMatrix();
    glScalef(1,2,1);
    glTranslatef(7,1.5,13);
    draw_gate();
    glPopMatrix();


    glPushMatrix();
    glScalef(1,2,1);
    glTranslatef(47,1.5,13);
    draw_gate();
    glPopMatrix();

    glPushMatrix();
    glScalef(1,2,1);
    glTranslatef(47,1.5, 5);
    draw_gate();
    glPopMatrix();
}

float door1 = 0.75, door2 = -0.5;

void draw_wall()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);

    glPushMatrix();
    glScalef(8,6,0.5);
    glTranslatef(-0.625,-0.5,25);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,6, 29);
    glTranslatef(-10,-0.5, -0.569);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,6, 23);
    glTranslatef(33,-0.5, -1.7);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,6, 23);
    glTranslatef(90,-0.5, -1.65);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(0.5,6, 20);
    glTranslatef(93, -0.5, -0.75);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(40,6,0.5);
    glTranslatef(0.18,-0.5,25);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(22,6,0.5);
    glTranslatef(-0.227,-0.5, -32);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(2, 6,0.5);
    glTranslatef(23,-0.5, -32);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(29,6,0.5);
    glTranslatef(0.552,-0.5, -76);
    new_Cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,19);
    glPushMatrix();
    glScalef(4,6,0.5);
    glTranslatef(door1,-0.5,26.5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 21);
    glPushMatrix();
    glScalef(0.5,6, 8);
    glTranslatef(94, door2, 0.625);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}





void drawWallValu(float x, float y, float z, float a)
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

    glBegin(GL_QUADS);

    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    for(GLint i=0; i<6; i++)
    {
        getNormal3p(points[quadIndices[i][0]][0], points[quadIndices[i][0]][1], points[quadIndices[i][0]][2],
                    points[quadIndices[i][1]][0], points[quadIndices[i][1]][1], points[quadIndices[i][1]][2],
                    points[quadIndices[i][2]][0], points[quadIndices[i][2]][1], points[quadIndices[i][2]][2]);

        glVertex3fv(&points[quadIndices[i][0]][0]);
        glTexCoord2f(0,1);
        glVertex3fv(&points[quadIndices[i][1]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&points[quadIndices[i][2]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&points[quadIndices[i][3]][0]);
        glTexCoord2f(0,0);
    }
    glEnd();
}

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
        getNormal3p(points[quadIndices[i][0]][0], points[quadIndices[i][0]][1], points[quadIndices[i][0]][2],
                    points[quadIndices[i][1]][0], points[quadIndices[i][1]][1], points[quadIndices[i][1]][2],
                    points[quadIndices[i][2]][0], points[quadIndices[i][2]][1], points[quadIndices[i][2]][2]);

        glVertex3fv(&points[quadIndices[i][0]][0]);
        glVertex3fv(&points[quadIndices[i][1]][0]);
        glVertex3fv(&points[quadIndices[i][2]][0]);
        glVertex3fv(&points[quadIndices[i][3]][0]);
    }
    glEnd();
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


void drawMainHouse()
{
    //Floor
    glPushMatrix();
    glScalef(20, 0.5, 14);
    drawWallValu(0, 0, 0, 0.5);
    glPopMatrix();


    //Roof
    glPushMatrix();
    if(roof1){
        glScalef(20, 0.5, 14);
    }
    else{
        glScalef(0,0, 0);
    }
    glTranslatef(0, 10, 0);
    drawWallValu(0, 0, 0, 0.5);
    glPopMatrix();

    //middle wall
    glPushMatrix();
    glScalef(21, 10, 0.5);
    glTranslatef(-0.012, 0, -0.5);
    drawWallValu(0, 0, 0, 0.5);
    glPopMatrix();


    glPushMatrix();
    glScalef(21, 10, 0.5);
    glTranslatef(-0.012, 0, 13.5);
    drawcube(0,0, 0, 0.5, 1.0, 0.5, 0.0);
    glPopMatrix();

    //left wall first part of fan
    glPushMatrix();
    glScalef(0.25, 5, 1);
    glTranslatef(-1, 0, 0);
    drawcube(0, 0, 0, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left wall lower part of Fan
    glPushMatrix();
    glScalef(0.25, 3.0, 6);
    drawcube(-1, 0, 0.167, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left Fan top wall
    glPushMatrix();
    glScalef(0.25, 0.8, 6);
    drawcube(-1, 5.25, 0.167, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left Fan side wall
    glPushMatrix();
    glScalef(0.25, 2, 4.5);
    drawcube(-1, 1.5, 0.556, 1, 1.0, 0.5, 0.0);
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
    glScalef(0.25, 0.8, 5);
    drawcube(40, 5.25, 0.4, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Window side wall
    glPushMatrix();
    glScalef(0.25, 2, 1.5);
    drawcube(40, 1.5, 2.33, 1, 1.0, 0.5, 0.0);
    glPopMatrix();


    //Door
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 4);
    glPushMatrix();
    glScalef(0.25, 4.2, 2);
    glTranslatef(40, 0, 2.5);
    glRotatef(doorangle, 0, 1, 0);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
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

void drawKitchen()
{
    //Floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,14);
    glPushMatrix();
    glScalef(20, 0.5, 14);
    drawWallValu(0, 0, 0, 0.5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    if(roof3){
        glScalef(20, 0.5, 14);
    }
    else{
        glScalef(0, 0, 0);
    }

    glTranslatef(0, 10, 0);
    drawWallValu(0, 0, 0, 0.5);
    glPopMatrix();

    //middle wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,15);
    glPushMatrix();
    glScalef(21, 10, 0.5);
    glTranslatef(-0.012, 0, -0.5);
    drawWallValu(0, 0, 0, 0.5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    glScalef(21, 10, 0.5);
    glTranslatef(-0.012, 0, 13.5);
    drawcube(0,0, 0, 0.5, 1.0, 0.5, 0.0);
    glPopMatrix();

    //left wall first part of fan
    glPushMatrix();
    glScalef(0.25, 5, 1);
    glTranslatef(-1, 0, 0);
    drawcube(0, 0, 0, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left wall lower part of Fan
    glPushMatrix();
    glScalef(0.25, 3.0, 6);
    drawcube(-1, 0, 0.167, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left Fan top wall
    glPushMatrix();
    glScalef(0.25, 0.8, 6);
    drawcube(-1, 5.25, 0.167, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Left Fan side wall
    glPushMatrix();
    glScalef(0.25, 2, 4.5);
    drawcube(-1, 1.5, 0.556, 1, 1.0, 0.5, 0.0);
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
    glScalef(0.25, 0.8, 5);
    drawcube(40, 5.25, 0.4, 1, 1.0, 0.5, 0.0);
    glPopMatrix();

    //Window side wall
    glPushMatrix();
    glScalef(0.25, 2, 1.5);
    drawcube(40, 1.5, 2.33, 1, 1.0, 0.5, 0.0);
    glPopMatrix();


    //Door
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 19);
    glPushMatrix();
    glScalef(0.25, 4.2, 2);
    glTranslatef(40, 0, 2.5);
    glRotatef(doorangle, 0, 1, 0);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //Fridge
    //body
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,16);
    glPushMatrix();
    glScalef(1, 4, 2);
    glTranslatef(0.2,0.0625, 1.5);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();

    //Upper door
    glPushMatrix();
    glScalef(0.25, 1.6, 2);
    glTranslatef(5, 1.65, 1.5);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();

    //Lower door
    glPushMatrix();
    glScalef(0.25, 2.3, 2);
    glTranslatef(5, 0.125, 1.5);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //Handle
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,17);
    glPushMatrix();
    glScalef(0.1, 0.8, 0.12);
    glTranslatef(14.5, 3.4, 40.5);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1, 0.8, 0.12);
    glTranslatef(14.5, 2.1, 40.5);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //Wall Shelf

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


    //Door



    //bottom
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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,18);
    glPushMatrix();
    glScalef(0.01, 2, 2);
    glTranslatef(899, 0, 1.25);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();

    //top outer cover
    glPushMatrix();
    glScalef(1, 2, 0.025);
    glTranslatef(9, 0, 180);
    drawWallValu(0, 0, 0, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

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

    //drawer
    //Middle Top cover
    glPushMatrix();
    glScalef(10, 0.05,1.1);
    drawcube(0, 40, 0, 1, 1.7, 2, 0.02);
    glPopMatrix();



    drawCabardDoor(0.125, 0);
    drawCabardDoor(1.25, 1);
    drawDrawer(2.375);
    drawDrawer(3.45);




    //Window
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


    //Microwave oven
    //body
    GLfloat M[] = {0, 1,1, 1};

    glPushMatrix();
    glScalef(1, 0.75, 0.5);
    drawcube(7, 2.66, 1, 1, 0.5, 0.5, 0.5);
    glPopMatrix();


    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AUX0, M);
    glTranslatef(5, 2.4, 0.6);
    glutSolidTeapot(0.5);
    glPopMatrix();

    //Front
    glPushMatrix();
    glScalef(0.8, 0.6, 0.1);
    drawcube(8.8, 3.5, 10.1, 1, 0.7, 1.08,1.02);
    glPopMatrix();

}

void drawKitchenadjusted()
{
    glTranslatef(42, 0, -18);
    glRotatef(-90, 0, 1, 0);
    drawKitchen();
}


void drawHome()
{
    glTranslatef(16, 0, -5);
    drawMainHouse();
}


void drawBedRoom()
{
    //middle wall
    glPushMatrix();
    glScalef(15,7,0.3);
    glTranslatef(-0.2,0,0);
    drawcube(0, 0, 0, 1, 0.5,.02,.02);
    glPopMatrix();

    //top wall
    glPushMatrix();
    glScalef(15,7,0.3);
    glTranslatef(-0.2,0,33.33);
    drawcube(0, 0, 0, 1, 0.5,.02,.02);
    glPopMatrix();

    //left wall
    glPushMatrix();
    glScalef(.3,7,10);
    glTranslatef(-10,0,0);
    drawcube(0, 0, 0, 1, 0.5,.02,.02);
    glPopMatrix();

    //Right wall
    glPushMatrix();
    glScalef(.3,7, 10);
    glTranslatef(40,0,0);
    drawcube(0, 0, 0, 1, 0.5,.5,.5);
    glPopMatrix();

    //Floor
    glPushMatrix();
    glScalef(15,0.3,10);
    glTranslatef(-0.2,0,0);
    drawcube(0, 0, 0, 1, 0.5, 1,.02);
    glPopMatrix();



    //Roof
    glPushMatrix();
    if(roof2){
        glScalef(15,0.3,10);
    }
    else{
        glScalef(0, 0, 0);
    }
    glTranslatef(-0.2, 20, 0);
    drawcube(0, 0, 0, 1, 0.5, 1,.02);
    glPopMatrix();

    //big
    glPushMatrix();
    glScalef(4,5,.7);
    glTranslatef(-.5,0,0);
    drawWallValu(0,0,0,1);
    glPopMatrix();

    //door
    glPushMatrix();
    glScalef(1.5,4.3,.8);
    glTranslatef(-1,.1,0);
    drawcube(0, 0, 0, 1, 0.5,.02,.02);
    glPopMatrix();

    glPushMatrix();
    glScalef(1.5,4.3,.8);
    glTranslatef(.1,0.1,0);
    drawcube(0, 0, 0, 1, 0.5,.02,.02);
    glPopMatrix();

    //Handle
    glPushMatrix();
    glScalef(.07,.7,1);
    glTranslatef(-7,3,0);
    drawcube(0, 0, 0, 1, 1,1,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(.07,.7,1);
    glTranslatef(7,3,0);
    drawcube(0, 0, 0, 1, 1,1,1);
    glPopMatrix();

    //floor
    glPushMatrix();
    glScalef(2,.3,3);
    glTranslatef(5,8,.7);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    //lags
    glPushMatrix();
    glScalef(.2,2.5,.2);
    glTranslatef(59,0,10);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,2.5,.2);
    glTranslatef(50.5,0,10);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,2.5,.2);
    glTranslatef(59,0,24.5);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,2.5,.2);
    glTranslatef(50.5,0,24.5);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    //table part
    glPushMatrix();
    glScaled(1,.3,1.2);
    glTranslatef(10.2,8.5,2.8);
    drawcube(0, 0, 0, 1, 0,0,0);
    glPopMatrix();

    //upper part
    glPushMatrix();
    glScaled(.2,1,1.2);
    glTranslatef(55,2.55,2.8);
    drawcube(0, 0, 0, 1, 0,0,0);
    glPopMatrix();

    //screen
    glPushMatrix();
    glScaled(.1,.98,1);
    glTranslatef(109.5,2.55,3.55);
    drawcube(0, 0, 0, 1, 1,1,1);
    glPopMatrix();

    //chair floor
    glPushMatrix();
    glScalef(1,0.2,1.5);
    glTranslatef(8.5,7,2);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    //lags
    glPushMatrix();
    glScalef(.2,1.5,.2);
    glTranslatef(46.5,0,15);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,1.5,.2);
    glTranslatef(46.5,0,21.5);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,2.9,.2);
    glTranslatef(42.5,0,21.5);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,2.9,.2);
    glTranslatef(42.5,0,15);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    //back
    glPushMatrix();
    glScalef(0.2,.7,1.5);
    glTranslatef(42.5,2.8,2);
    drawcube(0, 0, 0, 1, 0.3,.01,.01);
    glPopMatrix();

    glScalef(1,1,1.5);

    glPushMatrix();
    glScalef(7, 0.7, 2.0);
    glTranslatef(-.3,1,.7);
    drawcube(0, 0, 0, 1, 0.5f, 0.0f, 0.0f);
    glPopMatrix();


    glPushMatrix();
    glScalef(.3, 2, .3);
    glTranslatef(-7,0,5);
    drawcube(0, 0, 0, 1, 0.5f, 0.0f, 0.0f);
    glPopMatrix();


    glPushMatrix();
    glScalef(.3, 2, .3);
    glTranslatef(-7,0,10.5);
    drawcube(0, 0, 0, 1, 0.5f, 0.0f, 0.0f);
    glPopMatrix();


    glPushMatrix();
    glScalef(.3, 2, .3);
    glTranslatef(15,0,10.5);
    drawcube(0, 0, 0, 1, 0.5f, 0.0f, 0.0f);
    glPopMatrix();


    glPushMatrix();
    glScalef(.3, 2, .3);
    glTranslatef(15,0,5);
    drawcube(0, 0, 0, 1, 0.5f, 0.0f, 0.0f);
    glPopMatrix();


    glPushMatrix();
    glScalef(7, 0.7, 2.0);
    glTranslatef(-.3,1.3,.6);
    drawcube(0, 0, 0, 1, 1, 1, 1);
    glPopMatrix();



    glPushMatrix();
    glScalef(.8, 0.5, 1.2);
    glTranslatef(-2,2.5,1.5);
    drawcube(0, 0, 0, 1, 0.5f, 0.5f, 0.5f);
    glPopMatrix();


    glPushMatrix();
    glScalef(5, 0.3, 2.0);
    glTranslatef(-.1,4.5,.7);
    drawcube(0, 0, 0, 1, 0.2f, 2.3f, 0.9f);
    glPopMatrix();

}

void drawbedroomAdjusted()
{
    glScalef(0.8, 0.8, 0.8);
    glTranslatef(22.5, 0, -22.5);
    drawBedRoom();
}

GLfloat radius=2;

void drawWheel()
{
    //Torus
    glPushMatrix();
    Material(0.604, 0.804, 0.196);
    glutSolidTorus(0.1,radius,10,10);
    glPopMatrix();

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //line
    for (int theta = 0; theta <= 360; theta += 36)
    {
        glPushMatrix();
        Material(0.420, 0.557, 0.137);
        glRotatef(theta,0,0,1);
        glRotatef(90,0,1,0);
        gluCylinder(quadratic, 0.1, 0.1, radius, 10, 10);
        glPopMatrix();
    }

}

void wheelSeat()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //flat disk
    glPushMatrix();
    Material(0, 0.5, 0.8);
    //glTranslatef(0,-10,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,0,0.3,32,32);
    glPopMatrix();
    //basket
    glPushMatrix();
    Material(0, 0.5, 0.5);
    //glTranslatef(0,-10,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic, 0.3,0.3, 0.5, 32, 32);
    glPopMatrix();
    //seat head
    glPushMatrix();
    Material(0.416, 0.353, 0.804);
    glTranslatef(0,1,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.05,0.3, 0.3, 32, 32);
    glPopMatrix();
    //attached line
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glTranslatef(0,1.5,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.05,0.05, 1.5, 32, 32);
    glPopMatrix();
}

void wheelStand()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glScalef(2,3,2);
    Material(1,1,1);
    glTranslatef(-0.5,-1,-0.5);
    draw_Pyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void draw_wheel()
{
    glPushMatrix();
    glTranslatef(10,0,-7);
    glPushMatrix();
    glRotatef(angle,0,1,0);
    glPushMatrix();
    glRotatef(90,1,0,0);
    drawWheel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-1.5,2);
    wheelSeat();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-1.5,-2);
    wheelSeat();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,-1.5,0);
    wheelSeat();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2,-1.5,0);
    wheelSeat();
    glPopMatrix();
    glPopMatrix();

    wheelStand();
    glPopMatrix();
}

int angle2=0,d=0,dir=1;

void dolna()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(0.3,5,0.3);
    glTranslatef(0,-0.6,0);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(0.3,5,0.3);
    glTranslatef(10,-0.6,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(3.3,0.3,0.3);
    glTranslatef(0,6,0);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    glRotatef(angle2,1,0,0);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glScalef(2,0.3,1);
    glTranslatef(0.3,-4,0);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(0.1,3,0.1);
    glTranslatef(5.2,-.35,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(0.1,3,0.1);
    glTranslatef(25,-.35,0);
    new_Cube();
    glPopMatrix();
    glPopMatrix();
}

void draw_dolna()
{
    glPushMatrix();
    glTranslatef(-3,0,-17);
    dolna();
    glPopMatrix();
}

GLfloat angle3=0,s=0,ts=30,sd=1;

void draw_ride1()
{
    //stand
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    Material(1,1,1);
    glScalef(1,2,2);
    glTranslatef(0,-1.5,-0.25);
    draw_Pyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //rotating part
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    Material(1,1,1);
    glPushMatrix();
    glRotatef(angle3, 0,0,1);
    //seat
    glPushMatrix();
    glScalef(4,0.3,1);
    glTranslatef(-0.35,-4,0);
    new_Cube();
    glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void stand()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    glScalef(0.5,9,0.5);
    glTranslatef(0,-0.33,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,9,0.5);
    glTranslatef(6,-0.33,0);
    new_Cube();
    glPopMatrix();



    int ty=0;
    for(int i=0; i<11; i++)
    {
        glPushMatrix();
        glScalef(3.5,0.3,0.3);
        glTranslatef(0,-10+ty,0);
        new_Cube();
        glPopMatrix();
        ty+=3;
    }
    glDisable(GL_TEXTURE_2D);

}

GLfloat ty=0,up_speed=1,u=0;
GLint ucon=0;

void seat()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);

    glPushMatrix();
    glScalef(3.5,0.2,1.5);
    glTranslatef(0,-12,0.3);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,1,1.5);
    glTranslatef(0,-2.5,0.3);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,1,1.5);
    glTranslatef(6,-2.5,0.3);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(3.5,1,0.2);
    glTranslatef(-0,-2.5,2 );
    new_Cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}


void draw_ride2()
{
    glPushMatrix();
    glTranslatef(1.4,0,-18);
    glPushMatrix();
    stand();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,ty,0);
    seat();
    glPopMatrix();
    glPopMatrix();
}


void plot()
{
    glPushMatrix();
    glScalef(2,0.2,2);
    glTranslatef(-1,3,0.5);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,3.5,0.2);
    glTranslatef(-1,-.8,5);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(0.2,3.5,0.2);
    glTranslatef(-10,-.8,5);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,3.5,0.2);
    glTranslatef(-10,-.8,14);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,3.5,0.2);
    glTranslatef(-1,-.8,14);
    new_Cube();
    glPopMatrix();
}


void move_stand()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,6,0.2);
    glTranslatef(-20,-0.5,-5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,6,0.2);
    glTranslatef(-20,-0.5,-90);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix();
    glScalef(0.2,0.2,17);
    glTranslatef(-20,15,-1.05);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void move_seat()
{
    glPushMatrix();
    //glScalef(0.5,0.5,0.5);
    glTranslatef(-4,-2.5,-17);
    wheelSeat();
    glPopMatrix();
}

GLfloat tz1=0,ty1=0,k1=0,upcon=0,k2=0,scon=0;

void draw_ride3()
{
    glPushMatrix();
    glTranslatef(0,ty1,tz1);
    move_seat();
    glPopMatrix();
    move_stand();
}

void disk()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //flat disk
    glPushMatrix();
    Material(1, 1, 1);
    glTranslatef(0,-2.5,0);
    glRotatef(90,1,0,0);
    //glTranslatef(0,-6,0);
    gluDisk(quadratic,0,2,32,32);
    glPopMatrix();
}

void chair()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //flat disk
    glPushMatrix();
    Material(0, 0.5, 0.8);
    //glTranslatef(0,-10,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,0,0.3,32,32);
    glPopMatrix();
    //basket
    glPushMatrix();
    Material(0, 0.5, 0.5);
    //glTranslatef(0,-10,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic, 0.3,0.3, 0.5, 32, 32);
    glPopMatrix();
}

void draw_chair()
{
    glPushMatrix();
    glTranslatef(0,-2.5,-1.5);
    chair();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-2.5,1.5);
    chair();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5,-2.5,0);
    chair();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5,-2.5,0);
    chair();
    glPopMatrix();

}

void head()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    glPushMatrix();
    Material(0.416, 0.353, 0.804);
    glTranslatef(0,2,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.05,0.3, 5, 32, 32);
    glPopMatrix();
    //attached line
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glTranslatef(0,3,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.01,3, 3, 32, 32);
    glPopMatrix();
}

GLfloat angle4=0,r4=0;

void draw_ride4()
{

    glPushMatrix();
    glTranslatef(9,0,-15);
    glPushMatrix();
    glRotatef(angle4,0,1,0);
    disk();
    draw_chair();
    glPopMatrix();
    head();
    glPopMatrix();
}


void wheel5()
{
    glPushMatrix();
    //glTranslatef(10,0,-15);
    //glPushMatrix();
    glScalef(1,1,1);
    glTranslatef(0.2,1,0);
    glRotatef(angle,0,1,0);

    // glRotatef(90,1,0,0);
    drawWheel();
    glPopMatrix();
}

void stand5()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,4,0.2);
    glTranslatef(0,-0.75,0);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,4,0.2);
    glTranslatef(0,-0.75,-10);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,0.2,2);
    glTranslatef(0,4,-1);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

GLfloat r5=0,angle5=0;

void draw_ride5()
{
    glPushMatrix();
    glPushMatrix();
    glScalef(1,1,1);
    glTranslatef(0.2,1,-1);
    glRotatef(-angle5,0,0,1);

    //glRotatef(angle,0,1,0);
    drawWheel();
    glPopMatrix();
    stand5();
    glPopMatrix();
}


void table()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glScalef(3,0.2,3);
    glTranslatef(0,-5,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,2,0.2);
    glTranslatef(0,-1.5,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,2,0.2);
    glTranslatef(0,-1.5,14);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,2,0.2);
    glTranslatef(14,-1.5,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,2,0.2);
    glTranslatef(14,-1.5,14);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void table_seat()
{

    glPushMatrix();
    glTranslatef(1,0,0);
    head();
    glPopMatrix();
}

void draw_table()
{
    glPushMatrix();
    glTranslatef(-2,0,-7);
    table();
    table_seat();
    glPopMatrix();
}





int k=-5,c1=0,c2=0;
int rr =10;
float asd1 = 0, asd2 = 0, asd3 = 0;

void changeSize(int w, int h)
{
    if (h == 0)
        h = 1;
    float r = (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);
    gluPerspective(60, r, 1, 200);
    glMatrixMode(GL_MODELVIEW);
}

GLfloat zoomx=0,zoomz=0,zoomy=0;
int dooropen1 = 0, dooropen2 = 0;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60,1,1,10000);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    upX = sin(yAngle/180.0);
    upY = cos(yAngle/180.0);
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    glScalef(zoomX, zoomY, zoomZ);
    glRotatef( alpha, axis_x, axis_y, axis_z );
    glRotatef( theta, axis_x, axis_y, axis_z );

    glPushMatrix();
    glTranslatef(zoomx,zoomy,zoomz);

    glPushMatrix();
    glTranslatef(-2,0,-10);
    glPushMatrix();
    draw_park_land();
    draw_park_river();
    draw_park_sky();
    tree();
    gate();

    draw_road();
    draw_wall();

    glPopMatrix();
    glPopMatrix();

    draw_wheel();

    draw_dolna();

    glPushMatrix();
    glTranslatef(5,0,-4);
    glRotatef(90,0,1,0);
    draw_ride1();
    glPopMatrix();

    draw_ride2();

    draw_ride3();

    draw_ride4();

    glPushMatrix();
    glTranslatef(10,0,-1);
    draw_ride5();
    glPopMatrix();

    draw_table();

    glPopMatrix();

    glPushMatrix();
    drawHome();
    glPopMatrix();

    glPushMatrix();
    drawKitchenadjusted();
    glPopMatrix();

    glPushMatrix();
    drawbedroomAdjusted();
    glPopMatrix();

    glFlush();
    glutSwapBuffers();

    glEnable(GL_LIGHTING);

    Light_Source1();
    Light_Source2();
    spot_light();
}
int r=0;
int doorflag = 0, switchingdoor = 0;

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'q':
        eyeY+= 3;
        break;
    case 'w':
        eyeY-= 3;
        break;
    case 'e':
        if(wheel==0)
            wheel=1;
        else
            wheel=0;
        break;
    case 'r':
        if(u==0)
            u=1;
        else
            u=0;
        break;
    case 't':
        if(r4==0)
            r4=1;
        else
            r4=0;
        break;
    case 'y':
        if(r5==0)
            r5=1;
        else
            r5=0;
        break;

    case 'u':
        if(doorflag == 0)
        {
            doorangle = 0;
            doorflag = 0;
        }
        else
        {
            doorangle = 90;
            doorflag = 1;
        }
        switchingdoor = 1;
        break;
    case 'i':
        roof1 = !roof1;
        break;

    case 'o':
        roof2 = !roof2;
        break;

    case 'p':
        roof3 = !roof3;
        break;

    case 'n':
        eyeX = 15.5;
        eyeY = 13;
        eyeZ = 30;
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


    case 'k':
        dooropen1 = !dooropen1;
        break;
    case 'l':
        dooropen2 = !dooropen2;
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
        yAngle += 2;
        break;
    case 'D':
        yAngle -= 2;
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
        if(bView)
        {
            eyeX = -25;
            eyeY = 40;
            eyeZ = -5;
        }
        else
        {
            eyeX = 15.5;
            eyeY = 13;
            eyeZ = 30;
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

    case 27:
        exit(1);
    }
}



int speed=1,tangle=30;


void animate()
{

    if(dooropen1){
        if(door1 < 1.7)
            door1+= 0.01;
    }
    else{
        if(door1 > 0.75)
            door1-= 0.01;
    }

    if(!dooropen2){
        if(door2 > -0.5)
            door2-= 0.01;
    }
    else{
        if(door2 < 0.5)
            door2+= 0.01;
    }
    if(doorflag == 0 && switchingdoor ==1)
    {
        doorangle+= 2;
        if(doorangle >= 90)
        {
            doorflag = 1;
            switchingdoor = 0;
        }
    }
    else if(switchingdoor == 1)
    {
        doorangle-= 2;
        if(doorangle <=0)
        {
            doorflag = 0;
            switchingdoor = 0;
        }
    }
    if (bRotate == true)
    {
        theta += 0.2;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.2;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }

    if(wheel==1)
    {
        angle += speed;
        if(angle > 360)
            angle -=360;
    }

    if(d == 1)
    {
        if(angle2 >= 20 || angle2 <= -20 )
        {
            dir *= -1;
        }
        angle2 += dir * 0.01;
    }


    if(s == 1)
    {
        if(angle3 >= 20 || angle3 <= -20 )
        {
            sd *= -1;
        }
        angle3 += sd * 0.1;
    }

    if(u==1)
    {
        if(ucon == 0)
        {
            ty+=0.01;
            if(ty>= 8) ucon = 1;
        }

        else
        {
            ty-=0.01;
            if(ty<= 0) ucon = 0;
        }

    }


    if(k1==1)
    {
        if(upcon==0)
        {
            ty1+=0.01;
            if(ty1>=4)
            {
                upcon=1;
                k1=0;
            }
        }
        else
        {
            ty1-=0.01;
            if(ty1<=0)
            {
                upcon=0;
                k1=0;
            }
        }
    }


    if(k2==1)
    {
        if(scon==0)
        {
            tz1+=0.01;
            if(tz1>=15)
            {
                scon=1;
            }
        }
        else
        {
            tz1-=0.01;
            if(tz1<=0)
            {
                scon=0;
                k2=0;
            }
        }
    }

    if(r4==1)
    {
        angle4 += 1;
        if(angle4 > 360)
            angle4 -=360;
    }
    if(r5==1)
    {
        angle5 += 1;
        if(angle5 > 360)
            angle5 -=360;
    }


//For Light1
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

    if(bView)
    {
        eyeX+= 0.1;
    }


    glutPostRedisplay();

}


void specialKey(int key, int x, int y)
{

    switch (key)
    {
    case GLUT_KEY_RIGHT:
        eyeX+=3;
        break;
    case GLUT_KEY_LEFT:
        eyeX-=3;
        break;
    case GLUT_KEY_UP:
        eyeZ-= 3;
        break;
    case GLUT_KEY_DOWN:
        eyeZ+=3;
        break;

    case 27:
        exit(1);
    }

    glutPostRedisplay();
}



int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Resort Courtyard Scenario");


    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\grass3.bmp",1);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\water.bmp",2);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\skyup.bmp",3);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\road.bmp",4);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\brick.bmp",5);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\wood.bmp",6);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\steel.bmp",7);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\steel2.bmp",8);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\rope.bmp",9);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\green-grass.bmp",10);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\concrete.bmp",11);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\pool.bmp",12);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\roadhome.bmp",13);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\floor.bmp",14);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\wall.bmp",15);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\fridge.bmp",16);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\handle.bmp",17);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\basin.bmp",18);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\doorview.bmp",19);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\hill.bmp",20);
    LoadTexture("C:\\Users\\ROG\\Desktop\\Texture\\gate.bmp",21);


    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(specialKey);
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutIdleFunc(animate);
    glutMainLoop();

    return 0;
}


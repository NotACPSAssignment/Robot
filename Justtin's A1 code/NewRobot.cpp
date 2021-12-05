//CPS511 Assignment 1
//Justtin Hoang 500884336
//Section 011
//Bonus: Additional Cannon arms

#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "cube.h"
#include "QuadMesh.h"


//

const int vWidth = 650;    // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

// just by changing robot body scale
float robotBodyWidth = 9.0;
float robotBodyLength = 10.0;
float robotBodyDepth = 6.0;
float headDimension = 0.5 * robotBodyLength;
float upperArmLength = robotBodyLength / 2;
float upperArmWidth = 0.125*robotBodyWidth;
float gunLength = upperArmLength / 3.0;
float gunWidth = upperArmWidth * 2;
float gunDepth = upperArmWidth * 2;
float HeadLength = 0.5 * robotBodyLength;
float lowerBodyD = 0.5 * robotBodyLength;
float baseWidth = 2 * robotBodyWidth;
float baseLength = 0.25*lowerBodyD;


// Control Robot body rotation on base
float robotAngle = 0.0;

// Control arm rotation
float shoulderAngle = -40.0;
float gunAngle = -25.0;

// Spin Cube Mesh
float cubeAngle = 0.0;

//Robot movement
const float robotMove = 1;

// Robot position
float xPos = 0;
float yPos = 0;
float zPos = 0;

//Wheel Rotation angle
float wAngle = 2;

float xPosCan = 0;
float yPosCan = 0;
float wheelRot = 0;

//Laser variables
struct laserValues {
	float laserXPos, laserYPos, laserZPos, laserTimer, laserAngle;
}laserV;

int fired = 0;
// Lighting/shading and material properties for robot - upcoming lecture - just copy for now

GLfloat robotBody_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotBody_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat robotBody_mat_diffuse[] = { 0.1f,0.35f,0.1f,1.0f };
GLfloat robotBody_mat_shininess[] = { 32.0F };


GLfloat robotArm_mat_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 0.55f };
GLfloat robotArm_mat_diffuse[] = { 0.5f,0.0f,0.0f,1.0f };
GLfloat robotArm_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat robotArm_mat_shininess[] = { 32.0F };

GLfloat gun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat gun_mat_diffuse[] = { 0.01f,0.0f,0.01f,0.01f };
GLfloat gun_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat gun_mat_shininess[] = { 100.0F };

GLfloat robotLowerBody_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotLowerBody_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotLowerBody_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat robotLowerBody_mat_shininess[] = { 76.8F };


// Light properties
GLfloat light_position0[] = { -4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_position1[] = { 4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };


// Mouse button
int currentButton;

// A template cube mesh
CubeMesh *cubeMesh = createCubeMesh();

// A flat open mesh
QuadMesh *groundMesh = NULL;

// Structure defining a bounding box, currently unused
typedef struct BoundingBox {
	VECTOR3D min;
	VECTOR3D max;
} BBox;

// Default Mesh Size
int meshSize = 16;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void drawRobot();
void drawBody();
void drawHead();
void drawLowerBody();
void drawCannon();
void drawTower();
void drawTowerCannon();
void aimUp(void);
void aimDown(void);
void fireLaser(int on);
void drawLaser(int on);

void makeTextureMap();
void makeTextures();
void assignColor(GLfloat col[3], GLfloat r, GLfloat g, GLfloat b);
static GLfloat textureMap1[64][64][3];
static GLfloat textureMap2[64][64][3];
static GLfloat textureMap3[64][64][3];
static GLuint tex[3];



int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("3D Hierarchical Example");

	// Initialize GL
	initOpenGL(vWidth, vHeight);

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);

	// Start event loop, never returns
	glutMainLoop();

	return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). 
void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);   // This second light is currently off
	glEnable(GL_TEXTURE_2D);

	

	makeTextureMap();
	makeTextures();

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Other initializatuion
	// Set up ground quad mesh
	VECTOR3D origin = VECTOR3D(-16.0f, 0.0f, 16.0f);
	VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
	VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 32.0);
	groundMesh->InitMesh(meshSize, origin, 32.0, 32.0, dir1v, dir2v);

	VECTOR3D ambient = VECTOR3D(0.0f, 0.05f, 0.0f);
	VECTOR3D diffuse = VECTOR3D(0.4f, 0.8f, 0.4f);
	VECTOR3D specular = VECTOR3D(0.04f, 0.04f, 0.04f);
	float shininess = 0.2;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);
}

void assignColor(GLfloat col[3], GLfloat r, GLfloat g, GLfloat b) {
	col[0] = r;
	col[1] = g;
	col[2] = b;
}

void makeTextureMap()
{
	double range;
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			range = ((double)(rand() % 200 + -100) / (double)1000);
			assignColor(textureMap1[i][j], 0.500 + range, 0.500 + range, 0.500 + range);
		}
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			range = ((double)(rand() % 200 + -100) / (double)1000);
			assignColor(textureMap2[i][j], 0.0, 0.500 + range, 0.0);
		}
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			range = ((double)(rand() % 200 + -100) / (double)1000);
			assignColor(textureMap3[i][j], 0.500 + range, 0.0, 0.0);
		}
}


void makeTextures()
{

	glGenTextures(2, tex);

	//Texture mapping for the Quadmesh
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMap1);

	//Texture mapping for the Player submarine
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMap2);

	//Texture mapping for the Player submarine
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMap3);
}

// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// Create Viewing Matrix V
	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	//gluLookAt(0, -4.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt(0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	fireLaser(fired);
	drawLaser(fired);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	// Draw ground
	glPushMatrix();
	glTranslatef(0.0, -20.0, 0.0);
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, tex[1]);
	drawRobot();

	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(robotAngle, 0, 1, 0);
	glTranslatef(0, -2, 0);
	glRotatef(90, 1, 0, 0);

	drawTower();
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glutSwapBuffers();   // Double buffering, swap buffers
	/*
	drawRobot();
	glPushMatrix();

	
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(robotAngle, 0, 1, 0);
	glTranslatef(0, -2, 0);
	glRotatef(90, 1, 0, 0);

	drawTower();
	glPopMatrix();
	glPopMatrix();

	//Laser Funcs
	for (int i = 0; i < 100; i++) {
		lasers[i].laserXPos = xPos;
		lasers[i].laserYPos = yPos;
		lasers[i].laserZPos = zPos;
		lasers[i].laserAngle = robotAngle;
		fireLaser(fired, lasers[i].laserXPos, lasers[i].laserYPos, lasers[i].laserZPos, lasers[i].laserAngle);
		drawLaser(fired, lasers[i].laserXPos, lasers[i].laserYPos, lasers[i].laserZPos);
		
	}


	// Draw ground
	glPushMatrix();
	glTranslatef(0.0, -20.0, 0.0);
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();

	glutSwapBuffers();   // Double buffering, swap buffers
	*/
}

void drawRobot()
{

	glPushMatrix();
	
	
	glPushMatrix();
	drawBody();
	drawLowerBody();
	glPushMatrix();
	drawHead();
	glPushMatrix();
	drawCannon();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}


void drawBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	//Waist
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glScalef(robotBodyWidth * 0.5, robotBodyLength * 0.25, robotBodyDepth * 0.4);
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 1, 12, 12);
	glPopMatrix();

	//upper body
	glPushMatrix();
	glTranslatef(0.0, 6, 0.0); // this will be done last
	glScalef(robotBodyWidth, robotBodyLength*0.5, robotBodyDepth * 0.7);
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 1, 12, 12);
	glTranslatef(2, 1, 0);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 0.5, 12, 12);
	glPopMatrix();
	glPopMatrix();

	//Neck
	glPushMatrix();
	glTranslatef(0.0, 7, 0.0);
	glScalef(robotBodyWidth * 0.1, robotBodyLength * 0.4, robotBodyDepth * 0.2);
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 1, 12, 12);
	glPopMatrix();



}

void drawHead()
{
	// Set robot material properties per body part. Can have seperate material properties for each part
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	// Position head with respect to parent (body)
	glTranslatef(0, 0.65*robotBodyLength + 0.4*headDimension, -1.5); // this will be done last

	// Build Head
	glPushMatrix();
	glScalef(headDimension, headDimension, headDimension);
	glRotatef(15.0, 1.0, 0.0, 0.0);
	gluCylinder(gluNewQuadric(), 0.4, 0.15, 1.0, 20, 10);
	glPopMatrix();

	//Back of Head
	glPushMatrix();
	glTranslatef(0.0, 0, -0.015);
	glScalef(headDimension, headDimension, headDimension);
	glRotatef(180, 0, 1.0, 0);
	glRotatef(15.0, -1.0, 0.0, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.4, 20, 10);
	glPopMatrix();

	//Front of Head
	glPushMatrix();
	glTranslatef(0.0, -1.30, 4.8);
	glScalef(headDimension, headDimension, headDimension);
	glRotatef(15.0, 1.0, 0.0, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.15, 20, 10);
	glPopMatrix();
	glPopMatrix();

}

void drawLowerBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);

	glPushMatrix();
	glTranslatef(0, -0.75*robotBodyLength, 0.0); // this will be done last

	// Bolt
	glPushMatrix();
	// Position base with respect to parent stanchion
	glTranslatef(0, 0.0, 0.0); // this will be done last
	glScalef(lowerBodyD * 0.75, lowerBodyD, lowerBodyD);
	glRotatef(90, 0, 1.0, 0);
	glPopMatrix();

	// Beams
	glPushMatrix();
	glTranslatef(0.0, 7, 0.0); // this will be done last
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.7, 0.7, 6, 12, 12);
	glPopMatrix();


	glPushMatrix();
	glRotatef(wAngle, 1.0, 0.0, 0.0);
	// wheel
	glTranslatef(1.3, 0, 0);
	glPushMatrix();
	glScalef(lowerBodyD, lowerBodyD, lowerBodyD);
	glRotatef(-90, 0, 1.0, 0);
	gluCylinder(gluNewQuadric(), 0.7, 0.7, 0.5, 20, 20);
	glPopMatrix();

	// side wheel1
	glPushMatrix();// this will be done last
	glScalef(lowerBodyD, lowerBodyD, lowerBodyD);
	glRotatef(90, 0, 1.0, 0);
	gluDisk(gluNewQuadric(), 0, 0.7, 20, 10);
	glPopMatrix();

	// side wheel2 & bolt
	glPushMatrix();
	glTranslatef(-2.5, 0, 0.0);// this will be done last
	glScalef(lowerBodyD, lowerBodyD, lowerBodyD);
	glRotatef(-90, 0, 1.0, 0);
	gluDisk(gluNewQuadric(), 0, 0.7, 20, 10);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}


void drawCannon()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	//  Gun
	glTranslatef(-1.5, 4, 2.5);
	glRotatef(shoulderAngle, 1, 0, 0);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	gluCylinder(gluNewQuadric(), 1.5, 1.5, 3, 20, 20);
	glTranslatef(0, 0, 3);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 1.5, 20, 20);
	glPopMatrix();
	glTranslatef(0, 0, -3);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 1.5, 20, 20);
	glPopMatrix();
	glTranslatef(-0.5, 0, 2);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 5, 20, 20);
	glTranslatef(0, 0, 4.5);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glTranslatef(0, 0, -1);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 5, 20, 20);
	glTranslatef(0, 0, 4.5);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glTranslatef(1, 0, 0.5);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 5, 20, 20);
	glTranslatef(0, 0, 4.5);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 0.5, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, gun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gun_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, gun_mat_shininess);

}

void drawTower() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);
	glRotatef(-45, 0, 0, 1);
	glPushMatrix();
	gluCylinder(gluNewQuadric(), 3, 3, 1, 4, 4);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 3, 4, 4);
	glPushMatrix();
	glTranslatef(0, 0, -4);

	glPushMatrix();
	glRotatef(180, 0, 0, 1);
	drawTowerCannon();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.5, 3.5, 0.5);
	glRotatef(90, 1, 1, 0);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 10, 20, 20);
	glPushMatrix();
	glRotatef(robotAngle, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 2, 2, 1, 12, 12);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 2, 12, 12);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1);
	gluDisk(gluNewQuadric(), 0, 2, 12, 12);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 9);
	glRotatef(robotAngle, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 2, 2, 1, 12, 12);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 2, 12, 12);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1);
	gluDisk(gluNewQuadric(), 0, 2, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void drawTowerCannon() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	glPushMatrix();
	gluCylinder(gluNewQuadric(), 1, 3, 4, 4, 4);
	glRotatef(wheelRot, 1, 1, 0);
	glPushMatrix();
	glTranslatef(0, 0, -1);
	gluSphere(gluNewQuadric(), 1, 20, 20);
	glPushMatrix();
	glTranslatef(2, -2, -1);
	glRotatef(-90, 1, 1, 0);
	gluCylinder(gluNewQuadric(), 2, 1.5, 5, 4, 4);
	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 2, 4, 4);
	glPushMatrix();
	glTranslatef(1, -1, -1);
	glRotatef(-90, 1, 1, 0);
	gluCylinder(gluNewQuadric(), 3, 3, 2.8, 20, 20);

	glPushMatrix();
	gluDisk(gluNewQuadric(), 0, 3, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 2.9);
	gluDisk(gluNewQuadric(), 0, 3, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 5);
	gluDisk(gluNewQuadric(), 0, 1.5, 4, 4);
	glPushMatrix();
	gluCylinder(gluNewQuadric(), 1, 1, 0.5, 20, 20);
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	gluDisk(gluNewQuadric(), 0, 1, 20, 20);
	glPushMatrix();
	glTranslatef(-0.4, 0.4, 0);
	gluCylinder(gluNewQuadric(), 0.3, 0.3, 1, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.4, -0.4, 0);
	gluCylinder(gluNewQuadric(), 0.3, 0.3, 1, 20, 20);
	glPopMatrix();
	glPushMatrix();

	gluCylinder(gluNewQuadric(), 0.3, 0.3, 1, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}


// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
	// Set up viewport, projection, then change to modelview matrix mode - 
	// display function will then set up camera and do modeling transforms.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	gluLookAt(0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

bool stop = false;

void aimDown(void) {
	wheelRot += 1;
	if (wheelRot > 30)
		wheelRot -= 1;
	glutPostRedisplay();
}
void aimUp(void) {
	wheelRot -= 1;
	if (wheelRot < -20)
		wheelRot += 1;
	glutPostRedisplay();
}
// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 't':


		break;
	case 'f':
		laserV.laserXPos = xPos;
		laserV.laserYPos = yPos;
		laserV.laserZPos = zPos;
		laserV.laserAngle = robotAngle;
		fired = 1;

		break;
	case 'a':
		robotAngle += 2.0;
		if (robotAngle >= 360) {
			robotAngle = 0;
		}
		break;
	case 'd':
		robotAngle -= 2.0;
		if (robotAngle <= -360) {
			robotAngle = 0;
		}
		break;
	case 'z':
		if (shoulderAngle >= -10) {
			gunAngle = gunAngle;
			shoulderAngle = shoulderAngle;
		}
		else {
			gunAngle -= 1.0;
			shoulderAngle += 2.0;
		}
		break;
	case 'x':
		if (shoulderAngle <= -90) {
			gunAngle = gunAngle;
			shoulderAngle = shoulderAngle;
		}
		else {
			gunAngle += 1.0;
			shoulderAngle -= 2.0;
		}
		break;
	case 'w':
		xPos += robotMove * sin((robotAngle * M_PI) / 180);
		zPos += robotMove * cos((robotAngle * M_PI) / 180);
		wAngle += 5;
		break;
	case 's':
		xPos -= robotMove * sin((robotAngle * M_PI) / 180);
		zPos -= robotMove * cos((robotAngle * M_PI) / 180);
		wAngle -= 5;
		break;
	case 'v':
		glutTimerFunc(10, animationHandler, 0);
		break;
	case 'V':
		stop = true;
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


void animationHandler(int param)
{
	if (!stop)
	{
		cubeAngle += 2.0;
		glutPostRedisplay();
		glutTimerFunc(10, animationHandler, 0);
	}
}



// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
	// Help key
	if (key == GLUT_KEY_F1)
	{
		printf("Controls:\n");
		printf("w: Move Forward\n");
		printf("s: Move Backwards\n");
		printf("a: Turn Left\n");
		printf("d: Turn Right\n");
		printf("z: Lower Arms\n");
		printf("x: Raise Arms\n");
		printf("v: Spin Cube\n");
		printf("V: Stop Cube Spin\n");
	}
	// Do transformations with arrow keys
	//else if (...)   // GLUT_KEY_DOWN, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
	//{
	//}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			glutIdleFunc(aimUp);
		}
		if (state == GLUT_UP) {
			glutIdleFunc(NULL);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			glutIdleFunc(aimDown);
		}
		if (state == GLUT_UP) {
			glutIdleFunc(NULL);
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

void drawLaser(int on) {

	if (on == 1) {
		glPushMatrix();
		glTranslatef(laserV.laserXPos, laserV.laserYPos, laserV.laserZPos);
		glRotatef(laserV.laserAngle, 0, 1, 0);
		glScalef(0.5, 0.5, 1);
		glutSolidCone(2, 3, 4, 4);
		glPopMatrix();
	}
}

void fireLaser(int on) {

	if (on == 1) {

		if (laserV.laserTimer > 0) {
			laserV.laserXPos -= 0.5 * sin((laserV.laserAngle * M_PI) / 180);
			laserV.laserZPos -= 0.5 * cos((laserV.laserAngle * M_PI) / 180);
			laserV.laserTimer -= 0.01;

		}
		else {
			laserV.laserTimer = 100;
			laserV.laserXPos = xPos;
			laserV.laserYPos = yPos;
			laserV.laserZPos = zPos;
			fired = 0;
		}

	}
	glutPostRedisplay();
}
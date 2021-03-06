//CPS511 Assignment 3
//Justtin Hoang 500884336, Majuth Kirakalaprathapan 500878999, Al Sagun 500882340
//Section 011

#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <utility>
#include <vector>
#include <time.h> 
#include "VECTOR3D.h"
#include "mechBotAnimator.h"
#include "subdivcurve.h"
#include "QuadMesh.h"
#include "RGBpixmap.h"

int test = 1;

const int vWidth = 650;    // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

// just by changing robot body scale
float robotBodyWidth = 9.0;
float robotBodyLength = 10.0;
float robotBodyDepth = 6.0;
float headDimension = 0.5 * robotBodyLength;
float upperArmLength = robotBodyLength / 2;
float upperArmWidth = 0.125 * robotBodyWidth;
float gunLength = upperArmLength / 3.0;
float gunWidth = upperArmWidth * 2;
float gunDepth = upperArmWidth * 2;
float HeadLength = 0.5 * robotBodyLength;
float lowerBodyD = 0.5 * robotBodyLength;
float baseWidth = 2 * robotBodyWidth;
float baseLength = 0.25 * lowerBodyD;

float bot1Scale = 1.0;
float bot2Scale = 1.0;
float bot3Scale = 1.0;
float bot4Scale = 1.0;
time_t timer;


// Control Robot body rotation on base
float robotAngle = 0.0;

// Control arm rotation
float shoulderAngle = -90.0;
float gunAngle = -25.0;

// Spin Cube Mesh
float cubeAngle = 0.0;

//Robot movement
const float robotMove = 7;

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
	int fired;
}laserV;

struct laserEn1 {
	float lEnXPos, lEnYPos, lEnZPos, lEnTimer, lEnAngle;
	int En1fired;
}laserE1;

struct laserEn2 {
	float lEn2XPos, lEn2YPos, lEn2ZPos, lEn2Timer, lEn2Angle;
	int En2fired;
}laserE2;

struct laserEn3 {
	float lEn3XPos, lEn3YPos, lEn3ZPos, lEn3Timer, lEn3Angle;
	int En3fired;
}laserE3;

struct laserEn4 {
	float lEn4XPos, lEn4YPos, lEn4ZPos, lEn4Timer, lEn4Angle;
	int En4fired;
}laserE4;


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
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };
static GLfloat model_ambient[] = { 0.5, 0.5, 0.5, 1.0 };

boolean camera = false;


// Mouse button
int currentButton;

// A template cube mesh


// A flat open mesh
QuadMesh* groundMesh = NULL;

// Structure defining a bounding box, currently unused
typedef struct BoundingBox {
	VECTOR3D min;
	VECTOR3D max;
} BBox;


// Default Mesh Size
int meshSize = 16;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void animationHandler(int param);
void drawBody();
void drawHead();
void drawLowerBody();
void drawCannon();
void drawTower();
void drawTowerCannon();
void aimUp(void);
void aimDown(void);
void detectEnemyCollision();
void detectTowerCollision();

void drawGround();
void fireLaser(int on);
void drawLaser(int on);
void makeTextureMap();
void makeTextures();
void assignColor(GLfloat col[3], GLfloat r, GLfloat g, GLfloat b);


void drawEnLaser1(int on);
void fireAni1(int value);

void drawEnLaser2(int on);
void fireAni2(int value);

void drawEnLaser3(int on);
void fireAni3(int value);

void drawEnLaser4(int on);
void fireAni4(int value);

void fireEnLaser(int on);

static GLfloat textureMap1[64][64][3];
static GLfloat textureMap2[64][64][3];
static GLfloat textureMap3[64][64][3];
static GLfloat textureMap4[64][64][3];
static GLuint tex[4];

RGBpixmap pix1[4];
GLuint textureId;
int towerDMG = 0;

GLint glutWindowWidth = 800;
GLint glutWindowHeight = 600;
GLint viewportWidth = glutWindowWidth;
GLint viewportHeight = glutWindowHeight;

SubdivisionCurve subcurve;

// Use circles to **draw** (i.e. visualize) subdivision curve control points
Circle circles[MAXCONTROLPOINTS];

int numCirclePoints = 30;
double circleRadius = 0.2;
int hoveredCircle = -1;
int curveIndex = 0;
int currentCurvePoint = 0;
float angle = 0.0;
float nonConvertedAngle = 0.0;
int animate = 0;
int delay = 15; // milliseconds
float cubeSize = 1.0;
float sphereRadius = 1.0;
float wheelRadius = 1.0;
float wheelThickness = 1.0;
float towerX = -5.0;
float towerY = 1.0;
float towerZ = 12.0;
float wheelRotation = 0.0;
float nonConvertedRobotAngle = 0.0;
float robotAngle2 = 0.0;
const int towerAngle = 0;

// If collisions happen
int col1 = 0;
int col2 = 0;
int col3 = 0;
int col4 = 0;

int towerCol = 0;

GLdouble worldLeft = -12;
GLdouble worldRight = 12;
GLdouble worldBottom = -9;
GLdouble worldTop = 9;
GLdouble worldCenterX = 0.0;
GLdouble worldCenterY = 0.0;
GLdouble wvLeft = -12;
GLdouble wvRight = 12;
GLdouble wvBottom = -9;
GLdouble wvTop = 9;

GLfloat threeDCurve_ambient[] = { 0.6172f,0.0f,0.0f,0.0f };


int lastMouseX;
int lastMouseY;
int window2D, window3D;
int window3DSizeX = 800, window3DSizeY = 600;
GLdouble aspect = (GLdouble)window3DSizeX / window3DSizeY;
GLdouble eyeX = 0.0, eyeY = 6.0, eyeZ = 22.0;
GLdouble zNear = 0.1, zFar = 40.0;
GLdouble fov = 60.0;

// Ground Mesh material
GLfloat groundMat_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat groundMat_specular[] = { 0.01, 0.01, 0.01, 1.0 };
GLfloat groundMat_diffuse[] = { 0.4, 0.4, 0.7, 1.0 };
GLfloat groundMat_shininess[] = { 1.0 };

GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };


// 
GLdouble spin = 0.0;

int main(int argc, char** argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(glutWindowWidth, glutWindowHeight);
	glutInitWindowPosition(200, 30);
	window3D = glutCreateWindow("Assignment 3");

	initSubdivisionCurve();
	initControlPoints();


	// Initialize GL
	initOpenGL(glutWindowWidth, glutWindowHeight);

	// Register callback functions
	glutPositionWindow(200, 30);
	glutDisplayFunc(display3D);
	glutReshapeFunc(reshape3D);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyHandler);

	// Initialize the 3D system
	init3DSurfaceWindow();


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

	/*
	//Texture mapping for the Quadmesh
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMap1); */

	pix1[2].readBMPFile("grass.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
	glBindTexture(GL_TEXTURE_2D, tex[0]); // select current texture (0)
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D( // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0, // resolution level 0
		GL_RGB, // internal format
		pix1[2].nCols, // image width
		pix1[2].nRows, // image height
		0, // no border
		GL_RGB, // my format
		GL_UNSIGNED_BYTE, // my type
		pix1[2].pixel); // the pixels*/

	/*
	//Texture mapping for the Player submarine
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMap2);
	*/

	pix1[1].readBMPFile("redstone.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
	glBindTexture(GL_TEXTURE_2D, tex[1]); // select current texture (0)
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D( // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0, // resolution level 0
		GL_RGB, // internal format
		pix1[1].nCols, // image width
		pix1[1].nRows, // image height
		0, // no border
		GL_RGB, // my format
		GL_UNSIGNED_BYTE, // my type
		pix1[1].pixel); // the pixels*/


	pix1[0].readBMPFile("diamond.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
	glBindTexture(GL_TEXTURE_2D, tex[2]); // select current texture (0)
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D( // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0, // resolution level 0
		GL_RGB, // internal format
		pix1[0].nCols, // image width
		pix1[0].nRows, // image height
		0, // no border
		GL_RGB, // my format
		GL_UNSIGNED_BYTE, // my type
		pix1[0].pixel); // the pixels*/
}

// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.


void drawGround() {
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-120.0f, -1.0f, -120.0f);
	glVertex3f(-120.0f, -1.0f, 120.0f);
	glVertex3f(120.0f, -1.0f, 120.0f);
	glVertex3f(120.0f, -1.0f, -120.0f);
	glEnd();
	glPopMatrix();
}

void drawBody()
{

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
	glScalef(robotBodyWidth, robotBodyLength * 0.5, robotBodyDepth * 0.7);
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 1, 12, 12);
	glTranslatef(2, 1, 0);
	glPushMatrix();
	//gluDisk(gluNewQuadric(), 0, 0.5, 12, 12);
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

//Robot1 Body
void drawBot1();
void drawRobot1();
void drawBot1()
{
	glPushMatrix();

	glTranslatef(0, 1.0, 0);


	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0, 1.0, 0.0);
	glScalef(0.2, 0.2, 0.2);
	drawRobot1();
	glPopMatrix();


	glPopMatrix();

}
boolean shot1 = false;
float backwards1 = 0;
float yang1 = 0, xang1 = 0;
void drawRobot1()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);
	drawLowerBody();
	glPopMatrix();
	glPushMatrix();

	if (shot1) {
		backwards1 += 1.5;

		glTranslatef(backwards1, 0.0, 0.0);
		glRotatef(yang1, 1.0, 1.0, 0.0);
		glTranslatef(-0.4, 0.25, 0.0);
		glRotatef(xang1, 0.0, 0.0, 1.0);

		yang1 = yang1 + 1.35;
		if (yang1 > 360.0)
			yang1 = 0.0;
		xang1 = xang1 - 1.15;
		if (xang1 < 0.0)
			xang1 = 360.0;
	}
	else if (shot1 == false) {
		backwards1 = 0;
	}

	glRotatef(robotAngle - 50, 0.0, 1.0, 0.0);
	drawBody();
	glPushMatrix();
	drawHead();
	glPushMatrix();
	drawCannon();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}

//Robot2 Body
void drawBot2();
void drawRobot2();
void drawBot2()
{
	glPushMatrix();

	glTranslatef(0, 1.0, 0);


	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0, 1.0, 0.0);
	glScalef(0.2, 0.2, 0.2);
	drawRobot2();
	glPopMatrix();


	glPopMatrix();

}
boolean shot2 = false;
float backwards2 = 0;
float yang2 = 0, xang2 = 0;
void drawRobot2()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);
	drawLowerBody();
	glPopMatrix();
	glPushMatrix();

	if (shot2) {
		backwards2 += 1.5;

		glTranslatef(backwards2, 0.0, 0.0);
		glRotatef(yang2, 1.0, 1.0, 0.0);
		glTranslatef(-0.4, 0.25, 0.0);
		glRotatef(xang2, 0.0, 0.0, 1.0);

		yang2 = yang2 + 1.35;
		if (yang2 > 360.0)
			yang2 = 0.0;
		xang2 = xang2 - 1.15;
		if (xang2 < 0.0)
			xang2 = 360.0;
	}
	else if (shot2 == false) {
		backwards2 = 0;
	}

	glRotatef(robotAngle - 90, 0.0, 1.0, 0.0);
	drawBody();
	glPushMatrix();
	drawHead();
	glPushMatrix();
	drawCannon();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}

//Robot3 Body
void drawBot3();
void drawRobot3();
void drawBot3()
{
	glPushMatrix();

	glTranslatef(0, 1.0, 0);


	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0, 1.0, 0.0);
	glScalef(0.2, 0.2, 0.2);
	drawRobot3();
	glPopMatrix();


	glPopMatrix();

}
boolean shot3 = false;
float backwards3 = 0;
float yang3 = 0, xang3 = 0;
void drawRobot3()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);
	drawLowerBody();
	glPopMatrix();
	glPushMatrix();

	if (shot3) {
		backwards3 += 1.5;

		glTranslatef(backwards3, 0.0, 0.0);
		glRotatef(yang3, 1.0, 1.0, 0.0);
		glTranslatef(-0.4, 0.25, 0.0);
		glRotatef(xang3, 0.0, 0.0, 1.0);

		yang3 = yang3 + 1.35;
		if (yang3 > 360.0)
			yang3 = 0.0;
		xang3 = xang3 - 1.15;
		if (xang3 < 0.0)
			xang3 = 360.0;
	}
	else if (shot3 == false) {
		backwards3 = 0;
	}

	glRotatef(robotAngle - 120, 0.0, 1.0, 0.0);
	drawBody();
	glPushMatrix();
	drawHead();
	glPushMatrix();
	drawCannon();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}

//Robot4 Body
void drawBot4();
void drawRobot4();
void drawBot4()
{
	glPushMatrix();

	glTranslatef(0, 1.0, 0);


	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0, 1.0, 0.0);
	glScalef(0.2, 0.2, 0.2);
	drawRobot4();
	glPopMatrix();


	glPopMatrix();

}
boolean shot4 = false;
float backwards4 = 0;
float yang4 = 0, xang4 = 0;
void drawRobot4()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);
	drawLowerBody();
	glPopMatrix();
	glPushMatrix();

	if (shot4) {
		backwards4 += 1.5;

		glTranslatef(backwards4, 0.0, 0.0);
		glRotatef(yang4, 1.0, 1.0, 0.0);
		glTranslatef(-0.4, 0.25, 0.0);
		glRotatef(xang4, 0.0, 0.0, 1.0);

		yang4 = yang4 + 1.35;
		if (yang4 > 360.0)
			yang4 = 0.0;
		xang4 = xang4 - 1.15;
		if (xang4 < 0.0)
			xang4 = 360.0;
	}
	else if (shot4 == false) {
		backwards4 = 0;
	}

	glRotatef(robotAngle - 140, 0.0, 1.0, 0.0);
	drawBody();
	glPushMatrix();
	drawHead();
	glPushMatrix();
	drawCannon();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}

void drawHead()
{


	glPushMatrix();
	// Position head with respect to parent (body)
	glTranslatef(0, 0.65 * robotBodyLength + 0.4 * headDimension, -1.5); // this will be done last

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


	glPushMatrix();
	glTranslatef(0, -0.75 * robotBodyLength, 0.0); // this will be done last

	// Bolt
	glPushMatrix();
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


}

void drawTower() {

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
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
	glRotatef(180, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 0.5, 0.5, 10, 20, 20);
	glPushMatrix();
	//glRotatef(robotAngle, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 2, 2, 1, 12, 12);
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	gluDisk(gluNewQuadric(), 0, 2, 12, 12);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1);
	gluDisk(gluNewQuadric(), 0, 2, 12, 12);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 9);
	//glRotatef(robotAngle, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 2, 2, 1, 12, 12);
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
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


	glPushMatrix();
	gluCylinder(gluNewQuadric(), 1, 3, 4, 4, 4);
	glRotatef(wheelRot, 1, 1, 0);
	glPushMatrix();
	if (towerDMG == 1) {
		glRotatef(25, 1, -1, 0);

	}
	else if (towerDMG == 14) {
		glRotatef(-45, 1, -1, 0);

	}
	else if (towerDMG == 21) {
		glRotatef(-90, 1, -1, 0);

	}
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

void animationHandler(int param)
{
	if (currentCurvePoint < (subcurve.numCurvePoints - 2)) {

		currentCurvePoint += 1;



		nonConvertedAngle = atan(((1.0 * subcurve.curvePoints[currentCurvePoint + 1].y) - (1.0 * subcurve.curvePoints[currentCurvePoint].y)) / ((1.0 * subcurve.curvePoints[currentCurvePoint + 1].x) - (1.0 * subcurve.curvePoints[currentCurvePoint].x)));

		angle = (nonConvertedAngle / M_PI) * 180.0;
		if (angle > 0) {
			angle = -(180 - angle);
		}

		nonConvertedRobotAngle = atan(((towerX)-(1.0 * subcurve.curvePoints[currentCurvePoint].y)) / ((towerZ)-(1.0 * subcurve.curvePoints[currentCurvePoint].x)));
		robotAngle = ((nonConvertedRobotAngle / M_PI) * 180.0);

		glutSetWindow(window3D);
		glutPostRedisplay();
		glutTimerFunc(100, animationHandler, 0);
	}
}

void fireAni1(int value) {
	float rTime = rand() % 5 + 3;
	if (col3 == 0) {
		laserE1.lEnXPos = subcurve.curvePoints[currentCurvePoint].x;
		laserE1.lEnZPos = -subcurve.curvePoints[currentCurvePoint].y;
		laserE1.lEnAngle = robotAngle + 140;
		laserE1.En1fired = 1;
	}
	if (currentCurvePoint == 0) {
		laserE1.En1fired = 0;
	}
	glutTimerFunc(rTime * 1000, fireAni1, 0);
}
void fireAni2(int value) {
	float rTime = rand() % 5 + 3;
	if (col2 == 0) {
		laserE2.lEn2XPos = subcurve.curvePoints[currentCurvePoint].x - 6.0;
		laserE2.lEn2ZPos = -subcurve.curvePoints[currentCurvePoint].y - 3;
		laserE2.lEn2Angle = robotAngle + 180;
		laserE2.En2fired = 1;
	}
	if (currentCurvePoint == 0) {
		laserE2.En2fired = 0;
	}
	glutTimerFunc(rTime * 1000, fireAni2, 0);
}
void fireAni3(int value) {
	float rTime = rand() % 5 + 3;
	if (col4 == 0) {
		laserE3.lEn3XPos = subcurve.curvePoints[currentCurvePoint].x + 8.0;
		laserE3.lEn3ZPos = -subcurve.curvePoints[currentCurvePoint].y + 2;
		laserE3.lEn3Angle = robotAngle + 120;
		laserE3.En3fired = 1;
	}
	if (currentCurvePoint == 0) {
		laserE3.En3fired = 0;
	}
	glutTimerFunc(rTime * 1100, fireAni3, 0);
}
void fireAni4(int value) {
	float rTime = rand() % 4 + 2;
	if (col1 == 0) {
		laserE4.lEn4XPos = subcurve.curvePoints[currentCurvePoint].x - 10;
		laserE4.lEn4ZPos = -subcurve.curvePoints[currentCurvePoint].y + 4;
		laserE4.lEn4Angle = robotAngle + 230;
		laserE4.En4fired = 1;
	}
	if (currentCurvePoint == 0) {
		laserE4.En4fired = 0;
	}
	glutTimerFunc(rTime * 1100, fireAni4, 0);

}




// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:

		break;
	case GLUT_RIGHT_BUTTON:

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
		glScalef(0.1, 0.1, 1);
		gluSphere(gluNewQuadric(), 2, 12, 12);
		glPopMatrix();
	}
}

void fireLaser(int on) {

	if (on == 1) {

		if (laserV.laserTimer > 0) {
			laserV.laserXPos -= 0.5 * sin((laserV.laserAngle * M_PI) / 180);
			laserV.laserZPos -= 0.5 * cos((laserV.laserAngle * M_PI) / 180);
			laserV.laserTimer -= 0.01;

			detectEnemyCollision();
		}
		else {
			laserV.laserTimer = 100;
			laserV.laserXPos = xPos;
			laserV.laserYPos = yPos;
			laserV.laserZPos = zPos;
			laserV.fired = 0;
		}

	}
	glutPostRedisplay();
}

void drawEnLaser1(int on) {

	if (on == 1) {
		glPushMatrix();
		glTranslatef(laserE1.lEnXPos, laserE1.lEnYPos, laserE1.lEnZPos);
		glRotatef(laserE1.lEnAngle, 0, 1, 0);
		glScalef(0.5, 0.5, 1);
		gluSphere(gluNewQuadric(), 0.2, 20, 20);
		glPopMatrix();

		detectTowerCollision();
	}
}

void drawEnLaser2(int on) {

	if (on == 1) {
		glPushMatrix();
		glTranslatef(laserE2.lEn2XPos, laserE2.lEn2YPos, laserE2.lEn2ZPos);
		glRotatef(laserE2.lEn2Angle, 0, 1, 0);
		glScalef(0.5, 0.5, 1);
		gluSphere(gluNewQuadric(), 0.2, 20, 20);
		glPopMatrix();

		detectTowerCollision();
	}
}
void drawEnLaser3(int on) {

	if (on == 1) {
		glPushMatrix();
		glTranslatef(laserE3.lEn3XPos, laserE3.lEn3YPos, laserE3.lEn3ZPos);
		glRotatef(laserE3.lEn3Angle, 0, 1, 0);
		glScalef(0.5, 0.5, 1);
		gluSphere(gluNewQuadric(), 0.2, 20, 20);
		glPopMatrix();

		detectTowerCollision();
	}
}

void drawEnLaser4(int on) {

	if (on == 1) {
		glPushMatrix();
		glTranslatef(laserE4.lEn4XPos, laserE4.lEn4YPos, laserE4.lEn4ZPos);
		glRotatef(laserE4.lEn4Angle, 0, 1, 0);
		glScalef(0.5, 0.5, 1);
		gluSphere(gluNewQuadric(), 0.2, 20, 20);
		glPopMatrix();

		detectTowerCollision();
	}
}



void fireEnLaser(int on) {

	if (on == 1) {

		if (laserE1.lEnTimer > 0) {
			laserE1.lEnXPos -= 0.05 * sin((laserE1.lEnAngle * M_PI) / 180);
			laserE1.lEnZPos -= 0.05 * cos((laserE1.lEnAngle * M_PI) / 180);
			laserE1.lEnTimer -= 0.01;

		}
		else {
			laserE1.lEnTimer = 100;
			laserE1.lEnXPos = xPos;
			laserE1.lEnYPos = yPos;
			laserE1.lEnZPos = zPos;
			laserE1.En1fired = 0;
		}

		if (laserE2.lEn2Timer > 0) {
			laserE2.lEn2XPos -= 0.05 * sin((laserE2.lEn2Angle * M_PI) / 180);
			laserE2.lEn2ZPos -= 0.05 * cos((laserE2.lEn2Angle * M_PI) / 180);
			laserE2.lEn2Timer -= 0.01;

		}
		else {
			laserE2.lEn2Timer = 100;
			laserE2.lEn2XPos = xPos;
			laserE2.lEn2YPos = yPos;
			laserE2.lEn2ZPos = zPos;
			laserE2.En2fired = 0;
		}

		if (laserE3.lEn3Timer > 0) {
			laserE3.lEn3XPos -= 0.05 * sin((laserE3.lEn3Angle * M_PI) / 180);
			laserE3.lEn3ZPos -= 0.05 * cos((laserE3.lEn3Angle * M_PI) / 180);
			laserE3.lEn3Timer -= 0.01;

		}
		else {
			laserE3.lEn3Timer = 100;
			laserE3.lEn3XPos = xPos;
			laserE3.lEn3YPos = yPos;
			laserE3.lEn3ZPos = zPos;
			laserE3.En3fired = 0;
		}

		if (laserE4.lEn4Timer > 0) {
			laserE4.lEn4XPos -= 0.05 * sin((laserE4.lEn4Angle * M_PI) / 180);
			laserE4.lEn4ZPos -= 0.05 * cos((laserE4.lEn4Angle * M_PI) / 180);
			laserE4.lEn4Timer -= 0.01;

		}
		else {
			laserE4.lEn4Timer = 100;
			laserE4.lEn4XPos = xPos;
			laserE4.lEn4YPos = yPos;
			laserE4.lEn4ZPos = zPos;
			laserE4.En4fired = 0;
		}

	}
	glutPostRedisplay();
}


//3D functions
void screenToWorldCoordinates(int xScreen, int yScreen, GLdouble* xw, GLdouble* yw)
{
	GLdouble xView, yView;
	screenToCameraCoordinates(xScreen, yScreen, &xView, &yView);
	cameraToWorldCoordinates(xView, yView, xw, yw);
}

void screenToCameraCoordinates(int xScreen, int yScreen, GLdouble* xCamera, GLdouble* yCamera)
{
	*xCamera = ((wvRight - wvLeft) / glutWindowWidth) * xScreen;
	*yCamera = ((wvTop - wvBottom) / glutWindowHeight) * (glutWindowHeight - yScreen);
}

void cameraToWorldCoordinates(GLdouble xcam, GLdouble ycam, GLdouble* xw, GLdouble* yw)
{
	*xw = xcam + wvLeft;
	*yw = ycam + wvBottom;
}

void worldToCameraCoordiantes(GLdouble xWorld, GLdouble yWorld, GLdouble* xcam, GLdouble* ycam)
{
	double wvCenterX = wvLeft + (wvRight - wvLeft) / 2.0;
	double wvCenterY = wvBottom + (wvTop - wvBottom) / 2.0;
	*xcam = worldCenterX - wvCenterX + xWorld;
	*ycam = worldCenterY - wvCenterY + yWorld;
}

// The 2D animation path curve is a subdivision curve

void initSubdivisionCurve() {
	// Initialize 3 control points of the subdivision curve

	GLdouble x, y;

	x = 4 * cos(M_PI * 3);
	y = 4 * sin(M_PI * 0.5);

	subcurve.controlPoints[0].x = 1.5;
	subcurve.controlPoints[0].y = 24;

	x = 4 * cos(M_PI * 0.5);
	y = 4 * sin(M_PI * 0.5);

	subcurve.controlPoints[1].x = 0;
	subcurve.controlPoints[1].y = 12;

	x = 4 * cos(M_PI * 0.3);
	y = 4 * sin(M_PI * 0.3);

	subcurve.controlPoints[2].x = 2.35;
	subcurve.controlPoints[2].y = 6.4;

	x = 4 * cos(-M_PI * 0.2);
	y = 4 * sin(-M_PI * 0.2);

	subcurve.controlPoints[3].x = 5;
	subcurve.controlPoints[3].y = -0.6;

	x = 4 * cos(-M_PI * 0.28);
	y = 4 * sin(-M_PI * 0.28);

	subcurve.controlPoints[4].x = 3.2;
	subcurve.controlPoints[4].y = -3;

	subcurve.numControlPoints = 5;
	subcurve.subdivisionSteps = 4;
}

void initControlPoints() {
	int i;
	int num = subcurve.numControlPoints;
	for (i = 0; i < num; i++) {
		constructCircle(circleRadius, numCirclePoints, circles[i].circlePoints);
		circles[i].circleCenter = subcurve.controlPoints[i];
	}
}

void reshape3D(int w, int h)
{
	glutWindowWidth = (GLsizei)w;
	glutWindowHeight = (GLsizei)h;
	glViewport(0, 0, glutWindowWidth, glutWindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void drawEnemyBots() {

}

void display3D()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (camera) {
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	else {
		gluLookAt(towerX, yPos + 5, towerZ, towerX, 0.0, 0.0, 0.0, 1.0, 0.0);
	}


	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	// Draw ground
	glPushMatrix();
	glScalef(100, 100, 100);
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();

	computeSubdivisionCurve(&subcurve);
	//draw3DSubdivisionCurve();
	//draw3DControlPoints();


	// Lasers firing -------------------------------------------------------------------------------------------------------------------------------------
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glTranslatef(0, 2.7, 3);
	drawLaser(laserV.fired);
	fireLaser(laserV.fired);
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTranslatef(0, 2.7, 0);
	drawEnLaser1(laserE1.En1fired);
	fireEnLaser(laserE1.En1fired);
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTranslatef(0, 2.7, 0);
	drawEnLaser2(laserE2.En2fired);
	fireEnLaser(laserE2.En2fired);
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTranslatef(0, 2.7, 0);
	drawEnLaser3(laserE3.En3fired);
	fireEnLaser(laserE3.En3fired);
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTranslatef(0, 2.7, 0);
	drawEnLaser4(laserE4.En4fired);
	fireEnLaser(laserE4.En4fired);
	glPopMatrix();

	// Enemy Bots --------------------------------------------------------------------------------------------------------------------------------------
	glPushMatrix();
	glScalef(bot3Scale, bot3Scale, bot3Scale);
	glTranslatef(subcurve.curvePoints[currentCurvePoint].x, 0, -subcurve.curvePoints[currentCurvePoint].y);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	drawBot3();
	glPopMatrix();


	glPushMatrix();
	glScalef(bot2Scale, bot2Scale, bot2Scale);
	glTranslatef(subcurve.curvePoints[currentCurvePoint].x - 6.0, 0, -subcurve.curvePoints[currentCurvePoint].y - 3.0);

	glBindTexture(GL_TEXTURE_2D, tex[1]);

	drawBot2();
	glPopMatrix();

	glPushMatrix();
	glScalef(bot4Scale, bot4Scale, bot4Scale);
	glTranslatef(subcurve.curvePoints[currentCurvePoint].x + 8.0, 0, -subcurve.curvePoints[currentCurvePoint].y + 2.0);

	glBindTexture(GL_TEXTURE_2D, tex[1]);
	drawBot4();

	glPopMatrix();

	glPushMatrix();
	glScalef(bot1Scale, bot1Scale, bot1Scale);
	glTranslatef(subcurve.curvePoints[currentCurvePoint].x - 10.0, 0, -subcurve.curvePoints[currentCurvePoint].y + 4.0);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	drawBot1();
	glPopMatrix();

	nonConvertedRobotAngle = atan(((towerX)-(1.0 * subcurve.curvePoints[currentCurvePoint].y)) / ((towerZ)-(1.0 * subcurve.curvePoints[currentCurvePoint].x)));
	robotAngle = ((nonConvertedRobotAngle / M_PI) * 180.0);

	// Player Tower ----------------------------------------------------------------------------------------------------------------------------------
	glPushMatrix();
	glTranslatef(towerX, 0.9, 10);
	glScalef(0.4, 0.4, 0.4);
	glRotatef(90, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	drawTower();
	glPopMatrix();
	glutSwapBuffers();
}


void init3DSurfaceWindow()
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear

	glViewport(0, 0, (GLsizei)window3DSizeX, (GLsizei)window3DSizeY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}


void mouseButtonHandler3D(int button, int state, int x, int y)
{

	currentButton = button;
	lastMouseX = x;
	lastMouseY = y;
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
	case GLUT_MIDDLE_BUTTON:

		break;
	default:
		break;
	}
}


void mouseMotionHandler3D(int x, int y)
{
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;
	if (currentButton == GLUT_LEFT_BUTTON) {
		if (dy > 0) {
			eyeZ += 0.05;

		}
		else {
			eyeZ -= 0.05;
		}
		glutSetWindow(window3D);
		glutPostRedisplay();
	}
	if (currentButton == GLUT_RIGHT_BUTTON)
	{
	}
	else if (currentButton == GLUT_MIDDLE_BUTTON)
	{
	}
	lastMouseX = x;
	lastMouseY = y;
	glutPostRedisplay();
}


// Some Utility Functions

Vector3D crossProduct(Vector3D a, Vector3D b) {
	Vector3D cross;

	cross.x = a.y * b.z - b.y * a.z;
	cross.y = a.x * b.z - b.x * a.z;
	cross.z = a.x * b.y - b.x * a.y;

	return cross;
}

Vector3D normalize(Vector3D a) {
	GLdouble norm = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	Vector3D normalized;
	normalized.x = a.x / norm;
	normalized.y = a.y / norm;
	normalized.z = a.z / norm;
	return normalized;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 'Q':
	case 27:
		// Esc, q, or Q key = Quit 
		exit(0);
		break;
	case 32:
		laserV.laserXPos = towerX;
		laserV.laserZPos = zPos;
		laserV.laserAngle = towerAngle;
		laserV.fired = 1;
		if (towerDMG == 21)
		{
			laserV.fired = 0;
		}
		break;
		break;
	case 'a':
		// Add code to create timer and call animation handler
		glutSetWindow(window3D);
		animationHandler(1);
		fireAni1(1);
		fireAni2(1);
		fireAni3(1);
		fireAni4(1);
		// Use this to set to 3D window and redraw it
		glutSetWindow(window3D);
		glutPostRedisplay();
		break;
	case 'r':
		// reset object position at beginning of curve
		shot1 = false;
		shot2 = false;
		shot3 = false;
		shot4 = false;
		currentCurvePoint = 0;
		towerDMG = 0;
		bot1Scale = 1.0;
		bot2Scale = 1.0;
		bot3Scale = 1.0;
		bot4Scale = 1.0;
		col1 = 0;
		col2 = 0;
		col3 = 0;
		col4 = 0;
		glutSetWindow(window3D);
		glutPostRedisplay();
		break;
	default:
		break;
	
	case 'p':
		camera = !camera;
		break;
	
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


void specialKeyHandler(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1:
		printf("Controls:\n");
		printf("Left Arrow Key: Move Left\n");
		printf("Right Arrow Key: Move Right\n");
		printf("Space bar: Shoot projectile\n");
		break;
	case GLUT_KEY_LEFT:
		if (towerDMG < 21) {
			// add code here
			if (towerX > -9) {
				towerX -= 0.5;
				robotAngle2 -= 30;
			}
			else {
				towerX = -9;
			}

			//zPos = zPos;
			glutSetWindow(window3D);
			glutPostRedisplay();
		}

		break;
	case GLUT_KEY_RIGHT:
		if (towerDMG < 21) {
			// add code here;
			if (towerX < 14) {
				towerX += 0.5;
				robotAngle2 += 30;
			}
			else {
				towerX = 14;
			}
			zPos = zPos;
			glutSetWindow(window3D);
			glutPostRedisplay();
		}

		break;
	}
	glutPostRedisplay();
}

void detectEnemyCollision() {

	float bot1XDis = abs(laserV.laserXPos - (subcurve.curvePoints[currentCurvePoint].x - 10.0));
	float bot1ZDis = laserV.laserZPos - (-subcurve.curvePoints[currentCurvePoint].y + 4.0);

	float bot2XDis = abs(laserV.laserXPos - (subcurve.curvePoints[currentCurvePoint].x - 6.0));
	float bot2ZDis = laserV.laserZPos - (-subcurve.curvePoints[currentCurvePoint].y - 3.0);

	float bot3XDis = abs(laserV.laserXPos - (subcurve.curvePoints[currentCurvePoint].x));
	float bot3ZDis = laserV.laserZPos - (-subcurve.curvePoints[currentCurvePoint].y);

	float bot4XDis = abs(laserV.laserXPos - (subcurve.curvePoints[currentCurvePoint].x + 8.0));
	float bot4ZDis = laserV.laserZPos - (-subcurve.curvePoints[currentCurvePoint].y + 2.0);

	if (1 == 0) {}
	else if (bot1XDis < 1.5 && bot1ZDis < 1 && bot1ZDis > -1) {
		shot1 = true;
		//bot1Scale = 0.1;
		col1 = 1;

	}
	else if (bot2XDis < 1.5 && bot2ZDis < 1 && bot2ZDis > -1) {
		shot2 = true;
		//bot2Scale = 0.1;
		col2 = 1;

	}
	else if (bot3XDis < 1.5 && bot3ZDis < 1 && bot3ZDis > -1) {
		shot3 = true;
		col3 = 1;

	}
	else if (bot4XDis < 1.5 && bot4ZDis < 1 && bot4ZDis > -1) {
		shot4 = true;
		col4 = 1;

	}
	else if (bot1XDis < 1.5 && bot1ZDis < 1 && bot1ZDis > -9) {
		shot1 = true;
		col1 = 1;

	}
	else if (bot2XDis < 1.5 && bot2ZDis < 1 && bot2ZDis > -7 && (-subcurve.curvePoints[currentCurvePoint].y - 3.0) < 4) {
		shot2 = true;
		col2 = 1;

	}
	else if (bot3XDis < 1.5 && bot3ZDis < 1 && bot3ZDis > -7 && (-subcurve.curvePoints[currentCurvePoint].y) < 4) {
		shot3 = true;
		col3 = 1;

	}
	else if (bot4XDis < 1.5 && bot4ZDis < 1 && bot4ZDis > -7 && (-subcurve.curvePoints[currentCurvePoint].y + 2.0) < 5) {
		shot4 = true;
		col4 = 1;

	}

}

void detectTowerCollision() {
	float laser1XDis = abs(towerX - laserE1.lEnXPos);
	float laser1YDis = towerZ - laserE1.lEnZPos;
	//printf("laser1y: %f", laser1YDis);

	float laser2XDis = abs(towerX - laserE2.lEn2XPos);
	float laser2YDis = towerZ - laserE2.lEn2ZPos;
	//printf("laser2y: %f", laser2YDis);
	
	float laser3XDis = abs(towerX - laserE3.lEn3XPos);
	float laser3YDis = towerZ - laserE3.lEn3ZPos;
	//printf("laser3y: %f", laser3YDis);

	float laser4XDis = abs(towerX - laserE4.lEn4XPos);
	float laser4YDis = towerZ - laserE4.lEn4ZPos;
	//printf("laser4y: %f", laser4YDis);
	
	if (laser1XDis < 2 && laser1YDis < 1 && laser1YDis > -1 || laser2XDis < 2 && laser2YDis < 1 && laser2YDis > -1 || laser3XDis < 2 && laser3YDis < 1 && laser3YDis > -1 || laser4XDis < 2 && laser4YDis < 1 && laser4YDis > -1) {
		towerCol = 1;
		towerDMG++;
		if (towerDMG > 21) {
			towerDMG = 21;
		}
		
		printf("Tower collision detected\n");
	}else {

	}
}
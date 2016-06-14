//For windows
//#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//#include "RgbImage.h"
#include "model.hpp"

#define CURLING_STONE_TEXTURE "mesa.bmp"
#define CURLING_STONE_OBJ "stone_model.obj"



//--------------------------------- Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  1.0, 0.5, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.9, 0.92, 0.29, 1.0
#define PI		 3.14159

//================================================================================
//===========================================================Variaveis e constantes

//------------------------------------------------------------ Sistema Coordenadas
GLfloat   xC=15.0, yC=15.0, zC=30.0;
GLint     wScreen=800, hScreen=600;
GLfloat   mesa=3.0;
GLfloat   bule=1.3;
GLfloat   quad=6.0;
GLfloat   mesaP[]= {4, 0, 10};
GLfloat   buleP[]= {4, 0, 10};
GLfloat   quadP[]= {4, 4, 0.1};

GLint x=0;

GLint sizeTeapot=1;
GLint xInfTela=5, xSupTela=10, yTela;


//------------------------------------------------------------ Observador
GLint    defineView=0;
GLint    defineProj=1;
GLfloat  raio   = 20;
GLfloat  angulo = 0.35*PI;
//GLfloat  obsP[] = {raio*cos(angulo), 5.5, raio*sin(angulo)};
GLfloat  obsP[] = {0,5,0};
GLfloat objP[] = {5,0,0};
GLfloat  incy   = 0.5;
GLfloat  inca   = 0.03;
GLfloat  angBule = 0;
GLfloat  incBule = 10;

//------------------------------------------------------------ Texturas
GLint    repete=2;
GLfloat  rr=1;
GLint    maxR  =20;
GLint    numFrames =5;              //numero de imagens a colocar em loop na tela
GLint    msec=100;					//.. definicao do timer (actualizacao)

//================================================================================
//=========================================================================== INIT

//------------------------------------------------------------ Texturas
GLuint  texture[5];
GLuint horseTexture[5];
GLuint  tex;
RgbImage imag;
GLUquadric* qobj;

// ----------------- Disk
GLdouble heightDisk = 1.0;
GLdouble radiusDisk = 1.0;
GLfloat  diskP[] = {5,heightDisk,0};
GLfloat speedDisk = 0.0;
// ----------------- Speed of Disk
GLfloat incInitialSpeed=1;
GLfloat initialSpeed=0;
GLfloat flagIncrement=1;
GLfloat constantSpeed = 0.3;

// ----------------- Stick
GLfloat heightStick = 3;
GLfloat lengthStick = 3;

GLfloat  stickP[] = {0,heightStick,lengthStick};
GLfloat  stickP2[] = {0,heightStick,0};
GLfloat  positionStick[] = {diskP[0]+radiusDisk, 0,-lengthStick/2};
//const float PI = 3.14159/180;

// ----------------- Texto
char texto[30];

// ----------------- START
int start=1;
int moveBall=0;
int moveStick=0;
int leftStick=0;
int rightStick=0;

// ---------------- Modelo da stone
Model stoneModel;

void drawStick() {
	
	glTranslatef(positionStick[0], positionStick[1], positionStick[2]); // para se manter sempre em frente do disco
//glRotatef(180,0,1,0);
	glTranslatef(0,0,0);
	glBegin(GL_LINES);									// stick da esquerda
		glColor3f(0,0,1);
		glVertex3i( 0,0,0);
		glVertex3i(0, stickP[1], stickP[2]);	
	glEnd();

	glBegin(GL_LINES);									// stick da direita
		glColor3f(1,0,0);
		glVertex3i( 0,0,lengthStick);
		glVertex3i(0, stickP2[1], stickP2[2]);
	glEnd();
}

/*
void criaDefineTexturas()
{
	//----------------------------------------- Chao y=0
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("snow.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

}
*/

void loadModels() {
	stoneModel.init(CURLING_STONE_OBJ, CURLING_STONE_TEXTURE);
}

void init(void)
{
	glClearColor(WHITE);
	glShadeModel(GL_SMOOTH);
	//criaDefineTexturas( );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	//only called once
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
	loadModels();
	remove("file.txt");
}


void resizeWindow(GLsizei w, GLsizei h)
{
 	wScreen=w;
	hScreen=h;
	//glViewport( 0, 0, wScreen,hScreen );	
	//glutReshapeWindow(wScreen,hScreen);
	glutPostRedisplay();
}


void cleanup() {
	gluDeleteQuadric(qobj);
}

//================================================================================
//======================================================================== DISPLAY
void displayText(char *string, GLfloat x, GLfloat y, GLfloat z) 
{  
	glRasterPos3f(x,y,z); 
	while(*string)
	  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++); 
}

void drawCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		        x + (radius * cos(i *  twicePi / triangleAmount)), 
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

void drawStone() {
	stoneModel.drawModel(0,0,0, 0.1, 0.1, 0.1, -90, 1, 1, 1);
}

void drawScene(){

	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Chao y=0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( 0,  0, -xC ); 	// assumo que vou ler mais do que 1 imagem. grelha de 10 figuras por 10 figuras.
			glTexCoord2f(10.0f,0.0f); glVertex3i( xC*2, 0, -xC ); 
			glTexCoord2f(10.0f,10.0f); glVertex3i( xC*2, 0, xC); 
			glTexCoord2f(0.0f,10.0f); glVertex3i( 0,  0,  xC); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixos
	glColor4f(1,0,0,0);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i(10, 0, 0); 
	glEnd();
	glBegin(GL_LINES);
		glVertex3i(0,  0, 0); 
		glVertex3i(0, 10, 0); 
	glEnd();
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i( 0, 0,10); 
	glEnd();

	//~~~~~~~~~~~~~~~~~~~~~~~Cylinder (disk)
/*
	glColor4f(0,0,0,0);
	glPushMatrix();
		glTranslatef(diskP[0], diskP[1], diskP[2]);
		glRotatef(90,1,0,0);
		gluCylinder(qobj, radiusDisk, radiusDisk, heightDisk, 40, 20);
		drawCircle(0,0, radiusDisk);
	glPopMatrix();
*/

	//~~~~~~~~~~~~~~~~~~~~~~~Display Coordinates of Observer
	glColor3f(0,0,1);
	sprintf(texto, "Pos: (%.0f,%.0f,%.0f)", obsP[0], obsP[1], obsP[2]);
	displayText(texto,0,3,0);

	//~~~~~~~~~~~~~~~~~~~~~~~Display Coordinates of Observer
	glColor3f(0,0,1);
	sprintf(texto, "Disco: (%.0f,%.0f,%.0f)", diskP[0], diskP[1], diskP[2]);
	displayText(texto,0,0,0);

	//~~~~~~~~~~~~~~~~~~~~~~~Display Initial Speed
	glColor3f(0,0,1);
	sprintf(texto, "Current speed: %.0f)", initialSpeed);
	displayText(texto,20,2,0);


	drawStick();

	drawStone();
	glutPostRedisplay();
}


void display(void){
  	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Apagar ]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Janela Visualizacao ]
	glViewport (0,0,wScreen, hScreen);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Projeccao]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (defineProj) {
		case 1: gluPerspective(88.0, wScreen/hScreen, 0.1, zC); break;
		default: glOrtho (-xC,xC,-yC,yC,-zC,zC);
			break;
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Modelo+View(camera/observador) ]
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(obsP[0], obsP[1], obsP[2], 0,0,0, 0, 1, 0);
	gluLookAt(obsP[0], obsP[1], obsP[2], objP[0], objP[1], objP[2], 0, 1, 0);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Objectos ]
	drawScene();
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


// J: mexer para a esquerda. K: mexer como se fosse o da direita
void moveStickAutomatically() {
	/*if (leftStick) {
		positionStick[2] += 0.1;
		if(positionStick[2] == )
	}*/
}


void moveBallAutomatically() {
	diskP[0] += constantSpeed*initialSpeed;
	obsP[0] += constantSpeed*initialSpeed;
	objP[0] += constantSpeed*initialSpeed;
	if (diskP[0] > 20)
		moveBall=!moveBall;

}

void incrementInitialSpeed() {
	initialSpeed += incInitialSpeed;
}

void decrementInitialSpeed() {
	initialSpeed -= incInitialSpeed;
}

void defineInitialSpeed() {
	if (flagIncrement==0) {
		decrementInitialSpeed();
	}
	else if (flagIncrement==1) {
		incrementInitialSpeed();
	}

	// se atingir os limites superior ou inferior, inverte o sentido
	if (initialSpeed==0 || initialSpeed == 5)
		flagIncrement=!flagIncrement;
}

/* função evocada de x em x segundos */
void Timer(int value) 
{
    //TODO: mudar imagem na tela
	if (start) {
		defineInitialSpeed();
	}

	if (moveBall) {
		moveBallAutomatically();
	}

	if (moveStick) {
		moveStickAutomatically();
	}

	glutPostRedisplay();
	glutTimerFunc(msec,Timer, 1);

}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){
	
	switch (key) {

	case 's':
	case 'S':
		start=0; // já não estamos em modo start
		moveBall=1;
		glutPostRedisplay();
		break;

	case 'j':
	case 'J':
		if (!moveStick) {
			moveStick=1;
			glutPostRedisplay();
			break;
		}
		break;
/*
	//--------------------------- Textura no papel de parede
	case 't':
	case 'T':
		glutPostRedisplay();
		break;
	//--------------------------- Projeccao
	case 'q':
	case 'Q':
		defineProj=(defineProj+1)%2;
		glutPostRedisplay();
		break;*/
	//--------------------------- Escape
	case 27:
		exit(0);
		break;	
  }
}


void teclasNotAscii(int key, int x, int y){
    if(key == GLUT_KEY_UP)
		obsP[1]=obsP[1]+incy; 
	if(key == GLUT_KEY_DOWN) 
		obsP[1]=obsP[1]-incy; 
	if(key == GLUT_KEY_LEFT)
		angulo=angulo+inca; 
	if(key == GLUT_KEY_RIGHT) 
		angulo=angulo-inca; 

	if (obsP[1]> yC)
		obsP[1]= yC;
    if (obsP[1]<-yC)
		obsP[1]=-yC;
    obsP[0] = raio*cos(angulo);
	obsP[2] = raio*sin(angulo);


	glutPostRedisplay();	
}

//======================================================= MAIN
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (wScreen, hScreen); 
	glutInitWindowPosition (100, 100); 
	glutCreateWindow ("{pjmm,jpa}@dei.uc.pt-CG ::::::::::::::: (left,right,up,down, 'q', 't)' ");
  
	init();
	
	glutSpecialFunc(teclasNotAscii); 
	glutDisplayFunc(display); 
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}

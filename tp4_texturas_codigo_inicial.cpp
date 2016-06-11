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

#include "RgbImage.h"


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
GLfloat  obsP[] = {raio*cos(angulo), 5.5, raio*sin(angulo)};
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


void init(void)
{
	glClearColor(BLACK);
	glShadeModel(GL_SMOOTH);
	criaDefineTexturas( );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}


void resizeWindow(GLsizei w, GLsizei h)
{
 	wScreen=w;
	hScreen=h;
	//glViewport( 0, 0, wScreen,hScreen );	
	//glutReshapeWindow(wScreen,hScreen);
	glutPostRedisplay();
}


void drawScene(){
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Chao y=0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( 0,  0, 0 ); 	// assumo que vou ler mais do que 1 imagem. grelha de 10 figuras por 10 figuras.
			glTexCoord2f(10.0f,0.0f); glVertex3i( xC, 0, 0 ); 
			glTexCoord2f(10.0f,10.0f); glVertex3i( xC, 0, xC); 
			glTexCoord2f(0.0f,10.0f); glVertex3i( 0,  0,  xC); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixos
	glColor4f(BLACK);
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
	gluLookAt(obsP[0], obsP[1], obsP[2], 0,0,0, 0, 1, 0);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Objectos ]
	drawScene();
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


void Timer(int value) 
{
    //TODO: mudar imagem na tela
	angBule=angBule+incBule;
	glutPostRedisplay();
	glutTimerFunc(msec,Timer, 1);
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){
	
	switch (key) {

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
		break;
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

RgbImage::RgbImage( int numRows, int numCols )
{
	NumRows = numRows;
	NumCols = numCols;
	ImagePtr = new unsigned char[NumRows*GetNumBytesPerRow()];
	if ( !ImagePtr ) {
		fprintf(stderr, "Unable to allocate memory for %ld x %ld bitmap.\n", 
				NumRows, NumCols);
		Reset();
		ErrorCode = MemoryError;
	}
	// Zero out the image
	unsigned char* c = ImagePtr;
	int rowLen = GetNumBytesPerRow();
	for ( int i=0; i<NumRows; i++ ) {
		for ( int j=0; j<rowLen; j++ ) {
			*(c++) = 0;
		}
	}
}

/* ********************************************************************
 *  LoadBmpFile
 *  Read into memory an RGB image from an uncompressed BMP file.
 *  Return true for success, false for failure.  Error code is available
 *     with a separate call.
 *  Author: Sam Buss December 2001.
 **********************************************************************/

bool RgbImage::LoadBmpFile( const char* filename ) 
{  
	Reset();
	FILE* infile = fopen( filename, "rb" );		// Open for reading binary data
	if ( !infile ) {
		fprintf(stderr, "Unable to open file: %s\n", filename);
		ErrorCode = OpenError;
		return false;
	}

	bool fileFormatOK = false;
	int bChar = fgetc( infile );
	int mChar = fgetc( infile );
	if ( bChar=='B' && mChar=='M' ) {			// If starts with "BM" for "BitMap"
		skipChars( infile, 4+2+2+4+4 );			// Skip 4 fields we don't care about
		NumCols = readLong( infile );
		NumRows = readLong( infile );
		skipChars( infile, 2 );					// Skip one field
		int bitsPerPixel = readShort( infile );
		skipChars( infile, 4+4+4+4+4+4 );		// Skip 6 more fields

		if ( NumCols>0 && NumCols<=100000 && NumRows>0 && NumRows<=100000  
			&& bitsPerPixel==24 && !feof(infile) ) {
			fileFormatOK = true;
		}
	}
	if ( !fileFormatOK ) {
		Reset();
		ErrorCode = FileFormatError;
		fprintf(stderr, "Not a valid 24-bit bitmap file: %s.\n", filename);
		fclose ( infile );
		return false;
	}

	// Allocate memory
	ImagePtr = new unsigned char[NumRows*GetNumBytesPerRow()];
	if ( !ImagePtr ) {
		fprintf(stderr, "Unable to allocate memory for %ld x %ld bitmap: %s.\n", 
				NumRows, NumCols, filename);
		Reset();
		ErrorCode = MemoryError;
		fclose ( infile );
		return false;
	}

	unsigned char* cPtr = ImagePtr;
	for ( int i=0; i<NumRows; i++ ) {
		int j;
		for ( j=0; j<NumCols; j++ ) {
			*(cPtr+2) = fgetc( infile );	// Blue color value
			*(cPtr+1) = fgetc( infile );	// Green color value
			*cPtr = fgetc( infile );		// Red color value
			cPtr += 3;
		}
		int k=3*NumCols;					// Num bytes already read
		for ( ; k<GetNumBytesPerRow(); k++ ) {
			fgetc( infile );				// Read and ignore padding;
			*(cPtr++) = 0;
		}
	}
	if ( feof( infile ) ) {
		fprintf( stderr, "Premature end of file: %s.\n", filename );
		Reset();
		ErrorCode = ReadError;
		fclose ( infile );
		return false;
	}
	fclose( infile );	// Close the file
	return true;
}

short RgbImage::readShort( FILE* infile )
{
	// read a 16 bit integer
	unsigned char lowByte, hiByte;
	lowByte = fgetc(infile);			// Read the low order byte (little endian form)
	hiByte = fgetc(infile);			// Read the high order byte

	// Pack together
	short ret = hiByte;
	ret <<= 8;
	ret |= lowByte;
	return ret;
}

long RgbImage::readLong( FILE* infile )
{  
	// Read in 32 bit integer
	unsigned char byte0, byte1, byte2, byte3;
	byte0 = fgetc(infile);			// Read bytes, low order to high order
	byte1 = fgetc(infile);
	byte2 = fgetc(infile);
	byte3 = fgetc(infile);

	// Pack together
	long ret = byte3;
	ret <<= 8;
	ret |= byte2;
	ret <<= 8;
	ret |= byte1;
	ret <<= 8;
	ret |= byte0;
	return ret;
}

void RgbImage::skipChars( FILE* infile, int numChars )
{
	for ( int i=0; i<numChars; i++ ) {
		fgetc( infile );
	}
}

/* ********************************************************************
 *  WriteBmpFile
 *  Write an RGB image to an uncompressed BMP file.
 *  Return true for success, false for failure.  Error code is available
 *     with a separate call.
 *  Author: Sam Buss, January 2003.
 **********************************************************************/

bool RgbImage::WriteBmpFile( const char* filename )
{
	FILE* outfile = fopen( filename, "wb" );		// Open for reading binary data
	if ( !outfile ) {
		fprintf(stderr, "Unable to open file: %s\n", filename);
		ErrorCode = OpenError;
		return false;
	}

	fputc('B',outfile);
	fputc('M',outfile);
	int rowLen = GetNumBytesPerRow();
	writeLong( 40+14+NumRows*rowLen, outfile );	// Length of file
	writeShort( 0, outfile );					// Reserved for future use
	writeShort( 0, outfile );
	writeLong( 40+14, outfile );				// Offset to pixel data
	writeLong( 40, outfile );					// header length
	writeLong( NumCols, outfile );				// width in pixels
	writeLong( NumRows, outfile );				// height in pixels (pos for bottom up)
	writeShort( 1, outfile );		// number of planes
	writeShort( 24, outfile );		// bits per pixel
	writeLong( 0, outfile );		// no compression
	writeLong( 0, outfile );		// not used if no compression
	writeLong( 0, outfile );		// Pixels per meter
	writeLong( 0, outfile );		// Pixels per meter
	writeLong( 0, outfile );		// unused for 24 bits/pixel
	writeLong( 0, outfile );		// unused for 24 bits/pixel

	// Now write out the pixel data:
	unsigned char* cPtr = ImagePtr;
	for ( int i=0; i<NumRows; i++ ) {
		// Write out i-th row's data
		int j;
		for ( j=0; j<NumCols; j++ ) {
			fputc( *(cPtr+2), outfile);		// Blue color value
			fputc( *(cPtr+1), outfile);		// Blue color value
			fputc( *(cPtr+0), outfile);		// Blue color value
			cPtr+=3;
		}
		// Pad row to word boundary
		int k=3*NumCols;					// Num bytes already read
		for ( ; k<GetNumBytesPerRow(); k++ ) {
			fputc( 0, outfile );				// Read and ignore padding;
			cPtr++;
		}
	}

	fclose( outfile );	// Close the file
	return true;
}

void RgbImage::writeLong( long data, FILE* outfile )
{  
	// Read in 32 bit integer
	unsigned char byte0, byte1, byte2, byte3;
	byte0 = (unsigned char)(data&0x000000ff);		// Write bytes, low order to high order
	byte1 = (unsigned char)((data>>8)&0x000000ff);
	byte2 = (unsigned char)((data>>16)&0x000000ff);
	byte3 = (unsigned char)((data>>24)&0x000000ff);

	fputc( byte0, outfile );
	fputc( byte1, outfile );
	fputc( byte2, outfile );
	fputc( byte3, outfile );
}

void RgbImage::writeShort( short data, FILE* outfile )
{  
	// Read in 32 bit integer
	unsigned char byte0, byte1;
	byte0 = data&0x000000ff;		// Write bytes, low order to high order
	byte1 = (data>>8)&0x000000ff;

	fputc( byte0, outfile );
	fputc( byte1, outfile );
}


/*********************************************************************
 * SetRgbPixel routines allow changing the contents of the RgbImage. *
 *********************************************************************/

void RgbImage::SetRgbPixelf( long row, long col, double red, double green, double blue )
{
	SetRgbPixelc( row, col, doubleToUnsignedChar(red), 
							doubleToUnsignedChar(green),
							doubleToUnsignedChar(blue) );
}

void RgbImage::SetRgbPixelc( long row, long col,
				   unsigned char red, unsigned char green, unsigned char blue )
{
	assert ( row<NumRows && col<NumCols );
	unsigned char* thePixel = GetRgbPixel( row, col );
	*(thePixel++) = red;
	*(thePixel++) = green;
	*(thePixel) = blue;
}


unsigned char RgbImage::doubleToUnsignedChar( double x )
{
	if ( x>=1.0 ) {
		return (unsigned char)255;
	}
	else if ( x<=0.0 ) {
		return (unsigned char)0;
	}
	else {
		return (unsigned char)(x*255.0);		// Rounds down
	}
}
// Bitmap file format  (24 bit/pixel form)		BITMAPFILEHEADER
// Header (14 bytes)
//	 2 bytes: "BM"
//   4 bytes: long int, file size
//   4 bytes: reserved (actually 2 bytes twice)
//   4 bytes: long int, offset to raster data
// Info header (40 bytes)						BITMAPINFOHEADER
//   4 bytes: long int, size of info header (=40)
//	 4 bytes: long int, bitmap width in pixels
//   4 bytes: long int, bitmap height in pixels
//   2 bytes: short int, number of planes (=1)
//   2 bytes: short int, bits per pixel
//   4 bytes: long int, type of compression (not applicable to 24 bits/pixel)
//   4 bytes: long int, image size (not used unless compression is used)
//   4 bytes: long int, x pixels per meter
//   4 bytes: long int, y pixels per meter
//   4 bytes: colors used (not applicable to 24 bit color)
//   4 bytes: colors important (not applicable to 24 bit color)
// "long int" really means "unsigned long int"
// Pixel data: 3 bytes per pixel: RGB values (in reverse order).
//	Rows padded to multiples of four.


#ifndef RGBIMAGE_DONT_USE_OPENGL

bool RgbImage::LoadFromOpenglBuffer()					// Load the bitmap from the current OpenGL buffer
{
	int viewportData[4];
	glGetIntegerv( GL_VIEWPORT, viewportData );
	int& vWidth = viewportData[2];
	int& vHeight = viewportData[3];
	
	if ( ImagePtr==0 ) { // If no memory allocated
		NumRows = vHeight;
		NumCols = vWidth;
		ImagePtr = new unsigned char[NumRows*GetNumBytesPerRow()];
		if ( !ImagePtr ) {
			fprintf(stderr, "Unable to allocate memory for %ld x %ld buffer.\n", 
					NumRows, NumCols);
			Reset();
			ErrorCode = MemoryError;
			return false;
		}
	}
	assert ( vWidth>=NumCols && vHeight>=NumRows );
	int oldGlRowLen;
	if ( vWidth>=NumCols ) {
		glGetIntegerv( GL_UNPACK_ROW_LENGTH, &oldGlRowLen );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, NumCols );
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// Get the frame buffer data.
	glReadPixels( 0, 0, NumCols, NumRows, GL_RGB, GL_UNSIGNED_BYTE, ImagePtr);

	// Restore the row length in glPixelStorei  (really ought to restore alignment too).
	if ( vWidth>=NumCols ) {
		glPixelStorei( GL_UNPACK_ROW_LENGTH, oldGlRowLen );
	}	
	return true;
}

#endif   // RGBIMAGE_DONT_USE_OPENGL



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

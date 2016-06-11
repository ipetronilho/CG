//Criacao de texturas com Perlin noise (criacao de nuvens)
//Adaptado a partir do código disponivel em http://www.gamedev.net/page/resources/_/technical/game-programming/simple-clouds-part-1-r2085

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float mapa32[32*32];
float mapa256[256*256];
float deslocamento=0;

float noise(int x, int y, int aleatorio)
{
    
    int n = x + y * 57 + aleatorio * 131;
    
    n = (n<<13) ^ n;
    
    return (1.0f - ( (n * (n * n * 15731 + 789221) +
                      
                      1376312589)&0x7fffffff)* 0.000000000931322574615478515625f);
    
    
    
    
}


void setNoise(float  *mapa)

{
    time_t t;
    float temp[34][34];
    int aleatorio=rand() % 5000;
    
    
    srand((unsigned) time(&t));
    
    
    for (int y=1; y<33; y++)
        
        for (int x=1; x<33; x++)
            
        {
            
            temp[x][y] = 128.0f + noise(x,  y,  aleatorio)*128.0f;
            
        }
    
    
    for (int x=1; x<33; x++)
        
    {
        
        temp[0][x] = temp[32][x];
        
        temp[33][x] = temp[1][x];
        
        temp[x][0] = temp[x][32];
        
        temp[x][33] = temp[x][1];
        
    }
    
    temp[0][0] = temp[32][32];
    
    temp[33][33] = temp[1][1];
    
    temp[0][33] = temp[32][1];
    
    temp[33][0] = temp[1][32];
    
    for (int y=1; y<33; y++)
        
        for (int x=1; x<33; x++)
            
        {
            
            float centro = temp[x][y]/4.0f;
            
            float  lados = (temp[x+1][y] + temp[x-1][y] + temp[x][y+1] + temp[x][y-1])/8.0f;
            
            float cantos = (temp[x+1][y+1] + temp[x+1][y-1] + temp[x-1][y+1] + temp[x-1][y-1])/16.0f;
            
            
            
            mapa[((x-1)*32) + (y-1)] = centro + lados + cantos;
            
        }
    
}


float interpola(float x, float y, float  *mapa)

{
    
    int Xint = (int)x;
    
    int Yint = (int)y;
    
    
    
    float Xfrac = x - Xint;
    
    float Yfrac = y - Yint;
    
    int X0 = Xint % 32;
    
    int Y0 = Yint % 32;
    
    int X1 = (Xint + 1) % 32;
    
    int Y1 = (Yint + 1) % 32;
    
    float baixo = mapa[X0*32 + Y0] + Xfrac * (mapa[X1*32 + Y0] - mapa[X0*32 + Y0]);
    
    float topo = mapa[X0*32 + Y1] + Xfrac * (mapa[X1*32 +  Y1] - mapa[X0*32 + Y1]);
    
    
    
    return (baixo + Yfrac * (topo - baixo));
    
}

void sobrepoeOitavas(float  *mapa32, float  *mapa256)

{
    
    for (int x=0; x<256*256; x++)
        
    {
        
        mapa256[x] = 0;
        
    }
    
    for (int oitava=0; oitava<4; oitava++)
        
        for (int x=0; x<256; x++)
            
            for (int y=0; y<256; y++)
                
            {
                
                float escala = 1 / pow(2, 3-oitava);
                
                float noise = interpola(x*escala, y*escala , mapa32);
                mapa256[(y*256) + x] += noise / pow(2, oitava);
                
            }
    
}




void expFilter(float  *mapa)

{
    
    float cover = 20.0f;
    
    float sharpness = 0.95f;
    
    
    
    for (int x=0; x<256*256; x++)
        
    {
        
        float c = mapa[x] - (255.0f-cover);
        
        if (c<0)     c = 0;
        
        mapa[x] = 255.0f - ((float)(pow(sharpness, c))*255.0f);
        
    }
    
}





void loop()

{
    
    sobrepoeOitavas(mapa32, mapa256);
    
    expFilter(mapa256);
    
}



void init()

{
    
    glClearColor(0,0.75,1,1);
    setNoise(mapa32);
    loop();
    
}


void desenha()

{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    
    
    
    
    char textura[256][256][3];       //Array temporario para guardar valores RGB da textura
    
    
    
    for(int i=0; i<256; i++)
        
        for(int j=0; j<256; j++)
            
        {
            
            float cor = mapa256[i*256+j];
            if (cor > 200){
                textura[i][j][0]=cor;
                
                textura[i][j][1]=cor;
                
                textura[i][j][2]=cor;
            }
            else{
                textura[i][j][0] = 0;
                textura[i][j][1] = 191;
                textura[i][j][2] = 255;
                
            }
            
        }
    
    
    
    unsigned int id;                 //identificador da textura
    
    glGenTextures(1, &id);
    
    
    glBindTexture(GL_TEXTURE_2D, id);
    
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    
    
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, textura);
    
    
    
    glMatrixMode(GL_TEXTURE);        //As nuvens movem-se da esquerda para a direita
    glLoadIdentity();
    
    
    deslocamento+=0.0001f;
    
    glTranslatef(deslocamento,0,0);
    
    
    
    glEnable(GL_TEXTURE_2D);
    
    glBegin(GL_QUADS);
    
    glTexCoord2d(1,1); glVertex3f(1.0f, 1.0f, 0.);
    
    glTexCoord2d(0,1); glVertex3f(-1.0f, 1.0f, 0.);
    
    glTexCoord2d(0,0); glVertex3f(-1.0f, -1.0f, 0.);
    
    glTexCoord2d(1,0); glVertex3f(1.0f, -1.0f, 0.);
    
    glEnd(); 
    
    
    
    glutSwapBuffers();
    
}



void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20,timer, 1);
}

int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize (800, 600); 
    glutInitWindowPosition (100, 100); 
    glutCreateWindow ("{pjmm,jpa}@dei.uc.pt-CG ::::::::::::::: Clouds (Perlin Noise) ");
    
    init();
    
    glutDisplayFunc(desenha);
    
    glutTimerFunc(20, timer, 1);
    glutMainLoop();
    
    return 0;
}



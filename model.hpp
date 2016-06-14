#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include "RgbImage.h"

#define debug 1
#define tam_line_aux 100
using namespace std;

#define CAMO_SNOW_TEXTURE "curlingball.bmp"
#define CAMO_SNOW_OBJ "curlingmodel.obj"

class Model{

    public:

        /* Arrays globais */
        float *v;
        float *vt;
        float *vn;
        int *f;
        int f_size;

        const char* filename;
        const char* texturename;

        GLuint  texture[2];
        RgbImage imag;

        Model(){}

        ~Model(){}

        void Init(const char* objFile, const char* textureFile){

            f_size = 0;
            filename = objFile;
            texturename = textureFile;
            loadingFileObj(); // Read data
            createTexture(); // Passar os dados do .obj para openGL
        }

        /* Fazer a leitura do ficheiro .obj para os arrays correspondentes */
        void loadingFileObj(){

            if(debug){
                printf("Caminho: %s\n", filename);
            }

            /* Open file */
            //ifstream myFile(fileName, ios::in);
            ifstream myFile;
            myFile.open(filename);

            if(debug){
                cout << "First round MY_FILE: " << myFile << endl;
            }

            /* Caso nao tenha encontrado o nome do ficheiro ou nao consiga abrir, imprime uma mensagem de erro */
            if (myFile.fail()){
                cerr << "Cannot open " << filename << endl;
                exit(1);
            }

            string line;
            int v_size = 0, vt_size = 0, vn_size = 0, x;

            if(debug){
                cout << "V_Size: " << v_size << " VT_Size: " << vt_size << " VN_Size: " << vn_size << " F_Size: " << f_size << endl;
            }

            /* Read sizes */
            while(myFile >> line){
                if(line.compare("v")==0){
                    v_size++;
                }
                else if(line.compare("vt")==0){
                    vt_size++;
                }
                else if(line.compare("vn")==0){
                    vn_size++;
                }
                else if(line.compare("f")==0){
                    f_size++;
                }
            }

            if(debug){
                cout << "V_Size: " << v_size << " VT_Size: " << vt_size << " VN_Size: " << vn_size << " F_Size: " << f_size << endl;
            }

            /* Declaration arrays */
            /* FIXME: delete [] v; */
            v = new float [v_size*3];
            vt = new float [vt_size*2];
            vn = new float [vn_size*3];
            f = new int [f_size*9];

            /* Read information */
            int aux_v = 0, aux_vt = 0, aux_vn = 0, aux_f = 0;
            char line_aux[tam_line_aux];

            /* Voltar com o ponteiro ao inicio do ficheiro */
            myFile.clear();
            myFile.seekg(0, ios::beg);

            if(debug){
                cout << "Second round MY_FILE: " << myFile << endl;
            }

            while (myFile >> line){
                /* Caso seja as coordenadas de um vertice 3D */
                if (line.compare("v")==0){
                    /* v N1 N2 N3 */
                    if(debug){
                        cout << "v[aux_v][0]: " << v[aux_v*3 + 0] << " v[aux_v][1]: " << v[aux_v*3 + 1] << " v[aux_v][2]: " << v[aux_v*3 + 2] << endl;
                    }
                    myFile >> v[aux_v*3 + 0] >> v[aux_v*3 + 1] >> v[aux_v*3 + 2];
                    if(debug){
                        cout << "v[aux_v][0]: " << v[aux_v*3 + 0] << " v[aux_v][1]: " << v[aux_v*3 + 1] << " v[aux_v][2]: " << v[aux_v*3 + 2] << endl;
                    }
                    aux_v++;
                }
                /* Caso seja as coordenadas de uma textura 2D */
                else if(line.compare("vt")==0){
                    /* vt N1 N2 */
                     myFile >> vt[aux_vt*2 + 0] >> vt[aux_vt*2 + 1];
                     if(debug){
                         cout << "vt[aux_vt][0]: " << vt[aux_vt*2 + 0] << " vt[aux_vt][1]: " << vt[aux_vt*2 + 1] << endl;
                     }
                     aux_vt++;
                }
                /* Caso seja as coordenadas da normal */
                else if(line.compare("vn")==0){
                    /* vn N1 N2 N3 */
                     myFile >> vn[aux_vn*3 + 0] >> vn[aux_vn*3 + 1] >> vn[aux_vn*3 + 2];
                     if(debug){
                         cout << "vn[aux_vn][0]: " << vn[aux_vn*3 + 0] << " vn[aux_vn][1]: " << vn[aux_vn*3 + 1] << " vn[aux_vn][2]: " << vn[aux_vn*3 + 2] << endl;
                     }
                     aux_vn++;
                }
                /* Caso seja os pontos de uma face */
                else if(line.compare("f")==0){

                    /* f N1/N2/N3 N4/N5/N6 N7/N8/N9 */
                    x=0;
                    while(x<9){
                        myFile >> line_aux;
                        /* Para me separar os numeros pela / e guardar */
                        sscanf(line_aux,"%d/%d/%d",&f[aux_f*9 + x], &f[aux_f*9 + (x+1)], &f[aux_f*9 + (x+2)]);
                        if(debug){
                            cout << "f[aux_f][x]: " << f[aux_f*9 + x] << " f[aux_f][x+1]: " << f[aux_f*9 + (x+1)] << " f[aux_f][x+2]: " << f[aux_f*9 + (x+2)] << endl;
                        }
                        x = x+3;
                    }
                    aux_f++;
                }
            }

            /* Close file */
            myFile.close();
        }


        void createTexture(){
            glGenTextures(1, &texture[0]);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            imag.LoadBmpFile(texturename);
            glTexImage2D(GL_TEXTURE_2D, 0, 3,
                imag.GetNumCols(),
                imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                imag.ImageData());
        }

        /* Funcao para converter os dados do ficheiro .obj em openGl */
        void drawModel(float x, float y, float z, float xs, float ys, float zs, float rotation, float r, float g, float b){

            glPushMatrix();

                glColor3f(r, g, b); // white so it doesn't affect the texture
                glTranslatef(x, y, z);
                glScalef(xs, ys, zs);
                glRotatef(rotation, 0, 1, 0);


                int aux_num;
                /* f V/VT/VN V/VT/VN V/VT/VN */
                /* f 0/1/2 3/4/5 6/7/8 */

                for(int i=0; i<f_size; i++){
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D,texture[0]);
                    glBegin(GL_TRIANGLES); /* Start drawing triangle */
                        /* Vertex 1 */
                        aux_num = (f[(i*9) + 1]) - 1;
                        glTexCoord2f(vt[aux_num*2 + 0], vt[aux_num*2 + 1]);
                        aux_num = (f[i*9 + 2]) - 1;
                        glNormal3f(vn[aux_num*3 + 0], vn[aux_num*3 + 1], vn[aux_num*3 + 2]);
                        aux_num = (f[i*9 + 0]) - 1;
                        glVertex3f(/*x+*/ v[aux_num*3 + 0], /*y+*/ v[aux_num*3 + 1], /*z+*/ v[aux_num*3 + 2]);

                        /* Vertex 2 */
                        aux_num = (f[i*9 + 4]) - 1;
                        glTexCoord2f(vt[aux_num*2 + 0], vt[aux_num*2 + 1]);
                        aux_num = (f[i*9 + 5]) - 1;
                        glNormal3f(vn[aux_num*3 + 0], vn[aux_num*3 + 1], vn[aux_num*3 + 2]);
                        aux_num = (f[i*9 + 3]) - 1;
                        glVertex3f(/*x+*/ v[aux_num*3 + 0], /*y+*/ v[aux_num*3 + 1], /*z+*/ v[aux_num*3 + 2]);

                        /* Vertex 3 */
                        aux_num = (f[i*9 + 7]) - 1;
                        glTexCoord2f(vt[aux_num*2 + 0], vt[aux_num*2 + 1]);
                        aux_num = (f[i*9 + 8]) - 1;
                        glNormal3f(vn[aux_num*3 + 0], vn[aux_num*3 + 1], vn[aux_num*3 + 2]);
                        aux_num = (f[i*9 + 6]) - 1;
                        glVertex3f(/*x+*/ v[aux_num*3 + 0], /*y+*/ v[aux_num*3 + 1], /*z+*/ v[aux_num*3 + 2]);
                    glEnd(); /* Stop drawing triangle */
                    glDisable(GL_TEXTURE_2D);
                }

                glPopMatrix();
        }



};

#endif

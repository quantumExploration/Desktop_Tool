#pragma warning( disable : 4996 )

#include "dual_depth_peeling.h"
#include <GL/glext.h>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "svConfig.h"
#include "dataUpdater.h"

using namespace __svl_lib;

int image_height =1000;
int image_width = 1000;

renderUpdater *myRUpdate;
dataUpdater *myDUpdate;
visibilityProperty vproperty;
Paser *myPaser;
dataApi *myData;
renderApi *myRender;

void init(int argc, char **argv){
 myPaser = new Paser();
 myData = new dataApi();
 myDUpdate = new dataUpdater(myData, myPaser);
 myDUpdate->updateData(strdup(argv[1]));
 myDUpdate->updateContour();
 myDUpdate->updateCluster(vproperty);
 myDUpdate->updateVisibility(vproperty);
}

void display(){
	glutSwapBuffers();
        glFlush();
}

void reshape(int w, int h){

   glClearColor(0., 0., 0., 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   image_width = w;
   image_height = h;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   float aspect =  float(w)/float(h);
   float GLfloat_fov=60;
    gluPerspective(GLfloat_fov,
                        aspect,
                        0.1,//   view_info.hither,
                        1000);//  view_info.yon);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   //gluLookAt(eye[0],eye[1],eye[2], 0,0,0,0,1,0);
}

void mouse(int button, int state, int x, int y){
        long s=0x00000000;/*
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;


    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
//    s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
        s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;
*/
        glutPostRedisplay();
}

void motion(int x, int y){
        glutPostRedisplay();
}

void movement(int x, int y){
        glutPostRedisplay();
}

void key(unsigned char key, int x, int y){
        switch (key) {
        case '\033':
        case 'q':
        case 'Q':
                exit(0);
                break;
        }
        glutPostRedisplay();

}

//--------------------------------------------------------------------------
int main(int argc, char *argv[])
{ 
/* 
  eye[0]=0;
  eye[1]=0;
  eye[2]=100;
*/
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(image_width, image_height);
	glutInit(&argc, argv);
	glutCreateWindow("Dual Depth Peeling");

	if (glewInit() != GLEW_OK)
	{
		printf("glewInit failed. Exiting...\n");
		exit(1);
	}

    if (!glewIsSupported( "GL_VERSION_2_0 "
                          "GL_ARB_texture_rectangle "
						  "GL_ARB_texture_float "
						  "GL_NV_float_buffer "
                          "GL_NV_depth_buffer_float "))
    {
        printf("Unable to load the necessary extensions\n");
		printf("This sample requires:\n");
		printf("OpenGL version 2.0\n");
		printf("GL_ARB_texture_rectangle\n");
		printf("GL_ARB_texture_float\n");
		printf("GL_NV_float_buffer\n");
		printf("GL_NV_depth_buffer_float\n");
		printf("Exiting...\n");
        exit(1);
    }

  init(argc, argv);

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(key);
  glutPassiveMotionFunc(movement);
  glutMainLoop();
  return 0;
}

#include <stdlib.h>
#include <iostream>
#include <GL/glut.h> // GLUT stuff - includes OpenGL headers
#include <iostream>
#include <vector>
#include <time.h>

#include "bezier_surface.h"
using namespace std;

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

GLint	start_win_size_width	= 300;						// Starting Window Width (in Pixels)
GLint	start_win_size_height	= 300;						// Starting Window Height (in Pixels)
GLint	curr_win_size_width		= start_win_size_width;		// Current Window Width
GLint	curr_win_size_height	= start_win_size_height;	// Current Window Height

GLint	segments_per_curve		= 75;						// Number of line segments / points to be used while drawing a curve

vector<Bezier_Surface>	bezier_surface_set;					// Vector of Bezier Surfaces currently drawn in the application


// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


void init()
{
	// Initiate View
	gluLookAt(0.5 , 0.0 , 3 , 0 , 0 , 0 , 0 , 1 , 0);

	glClearColor(0.0f , 0.0f , 0.0f , 0.0f);
	// glClearDepth(1.0f);

	GLfloat temp_points[16][3] = {
		{-1.0 , 0.0 , 0.0} , {-1.0 , 1.0 , 0.0} , {1.0 , 1.0 , 0.0} , {1.0 , 0.0 , 0.0} ,
		{-0.5 , 0.0 , 0.3} , {-0.5 , -1.0 , 0.3} , {0.5 , -1.0 , 0.3} , {0.5 , 0.0 , 0.3} ,
		{-1.0 , 0.0 , 0.6} , {-1.0 , 0.5 , 0.6} , {1.0 , 0.5 , 0.6} , {1.0 , 0.0 , 0.6} ,
		{-0.5 , 1.0 , 1.0} , {-0.5 , 0.0 , 1.0} , {0.5 , 0.0 , 1.0} , {0.5 , 1.0 , 1.0} ,
	};

	GLfloat temp_color_array[4] = {0.5,0.4,0.2,1.0};

	bezier_surface_set.push_back(Bezier_Surface(3 , 3 , temp_points , temp_color_array , segments_per_curve));
}


void reshape(int w , int h)
{
	glViewport(0,0, (GLsizei)w , (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45 , (GLsizei)w / (GLsizei)h , 1 , 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.5 , 0.0 , 3.0 , 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 0.0);
}


void enable_lights()
{
	GLfloat light_pos[4] = {0.0 , 0.0 , 2.0 , 1.0};
	GLfloat light_amb[4] = {1.0 , 1.0 , 1.0 , 1.0};
	GLfloat light_dif[4] = {1.0 , 1.0 , 1.0 , 1.0};
	GLfloat light_spe[4] = {1.0 , 1.0 , 0.0 , 1.0};
	GLfloat light_dir[4] = {0.0 , 0.0 , 0.0 , 1.0};

	glLightfv(GL_LIGHT0 , GL_POSITION , light_pos);
	glLightfv(GL_LIGHT0 , GL_AMBIENT , light_amb);
	glLightfv(GL_LIGHT0 , GL_DIFFUSE , light_dif);
	glLightfv(GL_LIGHT0 , GL_SPECULAR , light_spe);
	glLightf(GL_LIGHT0 , GL_CONSTANT_ATTENUATION , 1.0);
	glLightf(GL_LIGHT0 , GL_LINEAR_ATTENUATION , 0.2);
	glLightf(GL_LIGHT0 , GL_QUADRATIC_ATTENUATION , 0.125);

	// glLightfv(GL_LIGHT0 , GL_SPOT_DIRECTION , light_dir);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	/*glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE);*/
}


void display_surfaces()
{
	for(int i=0; i<bezier_surface_set.size(); i++)
	{
		glMaterialfv(GL_FRONT_AND_BACK , GL_DIFFUSE , bezier_surface_set[i].color);
		glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR , bezier_surface_set[i].color_spe);
		glMaterialfv(GL_FRONT_AND_BACK , GL_EMISSION , bezier_surface_set[i].color_emi);
		glMaterialf(GL_FRONT_AND_BACK , GL_SHININESS , 100);
		glColor4f(bezier_surface_set[i].color[0] , bezier_surface_set[i].color[1] , bezier_surface_set[i].color[2] , bezier_surface_set[i].color[3]);
		
		glBegin(GL_QUADS);
		for(int m=0; m<segments_per_curve; m++)
		{
			for(int n=0; n<segments_per_curve; n++)
			{
				glVertex3f(bezier_surface_set[i].draw_points[m][n][0] , bezier_surface_set[i].draw_points[m][n][1] , bezier_surface_set[i].draw_points[m][n][2]);
				glVertex3f(bezier_surface_set[i].draw_points[m+1][n][0] , bezier_surface_set[i].draw_points[m+1][n][1] , bezier_surface_set[i].draw_points[m+1][n][2]);
				glVertex3f(bezier_surface_set[i].draw_points[m+1][n+1][0] , bezier_surface_set[i].draw_points[m+1][n+1][1] , bezier_surface_set[i].draw_points[m+1][n+1][2]);
				glVertex3f(bezier_surface_set[i].draw_points[m][n+1][0] , bezier_surface_set[i].draw_points[m][n+1][1] , bezier_surface_set[i].draw_points[m][n+1][2]);

			}
		}
		glEnd();

	}

	/*glColor3d(1.0 , 0.0 , 0.0);
	glBegin(GL_QUADS);
		glVertex3f(0.0 , 0.0 , 0.0);
		glVertex3f(0.0 , -5 , 0.0);
		glVertex3f(0.0 , -5 , -0.5);
		glVertex3f(0.0 , 0.0 , -0.5);
	glEnd();*/

	// glutSolidSphere(1.0,90,90);

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	display_surfaces();

	glutSwapBuffers();
	glFlush();
}


void keyboard(unsigned char key, int x, int y)
{
	GLfloat light_pos[4] = {0.0 , 0.0 , 2.0 , 1.0};

	switch(key)
	{
	case 'w':
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		display_surfaces();
		glRotatef(5.0 , 1.0 , 0.0 , 0.0);
		glPushMatrix();
		glLoadIdentity();
		glLightfv(GL_LIGHT0 , GL_POSITION , light_pos);
		glPopMatrix();
		glutPostRedisplay();
		break;
	case 's':
		glRotatef(-5.0 , 1.0 , 0.0 , 0.0);
		glutPostRedisplay();
		break;
	case 'a':
		glRotatef(-5.0 , 0.0 , 1.0 , 0.0);
		glutPostRedisplay();
		break;
	case 'd':
		glRotatef(5.0 , 0.0 , 1.0 , 0.0);
		glutPostRedisplay();
		break;
	case 'm':
		glRotatef(5.0 , 0.0 , 0.0 , 1.0);
		glutPostRedisplay();
		break;
	case 'n':
		glRotatef(-5.0 , 0.0 , 0.0 , 1.0);
		glutPostRedisplay();
		break;
		
	}
}


int main(int argc, char **argv)
{
	// Initialise the Random number Generator with a seed derived from the Machine Clock
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);

	glutInit(&argc, argv);

	// Initialise the Window
	glutInitWindowSize(start_win_size_width , start_win_size_height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Drawing Bezier Curves");

	// Call the Initial Function
	enable_lights();
	init();
	

	// Set the CallBacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	// glutMouseFunc(mouseClick);
	// glutMotionFunc(mouseDrag);

	// Enable various parameters FOR SMOOTHNESS
	// glEnable(GL_ALPHA_TEST); 
	// glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);
	// glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_LINE_SMOOTH);
	// glHint(GL_LINE_SMOOTH_HINT , GL_NICEST);

	glutMainLoop();

	return 0;
}

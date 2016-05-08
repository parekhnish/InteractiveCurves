#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <GL/glut.h> // GLUT stuff - includes OpenGL headers
#include <iostream>
#include <vector>
#include <time.h>

#include "bspline_curve.h"

using namespace std;

GLint start_win_size_width = 300;
GLint start_win_size_height = 300;
GLint segments_per_curve = 100;

GLint mode_select_curve = 0;
GLint active_curve = -1;

GLfloat color_hist[3];
GLfloat active_color[3] = {1.0 , 1.0 , 1.0};

vector<BSpline_Curve> bspline_curve_set;
vector<GLfloat> temp_point;

void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // We set up coordinate system so that aspect ratios are always correct,
   //  and the region from -1..1 in x & y always just fits in the viewport.
   if (w > h)
   {
      gluOrtho2D(-double(w)/h, double(w)/h, -1., 1.);
   }
   else
   {
      gluOrtho2D( -1., 1., -double(h)/w, double(h)/w);
   }
   glMatrixMode(GL_MODELVIEW);  // Always go back to modelview mode
   glLoadIdentity();
}

void init()
{
	GLint i,j,k;
	// Clear Background Color
	glClearColor(0.0 , 0.0 , 0.0 , 0.0);

	// Initialise a few bspline curves
	GLfloat init_control_points_1[4][2] = {
	   {0.0	,0.0},
	   {-0.9 ,0.1},
	   {0.2	, 0.8},
	   {0.7	, -0.4}	};

	GLfloat temp_color_array[3] = {0.9,0.1,0.1};
    GLfloat init_knot_vector[9] = {0.0,0.0,0.1,0.25,0.5,0.7,0.8,1.0,1.0};
	bspline_curve_set.push_back(BSpline_Curve(4, init_control_points_1, temp_color_array, init_knot_vector));

	GLfloat init_control_points_2[6][2] = {
	   {1.0	,-1.0},
	   {-0.2	,-0.1},
	   {0.4	,	-0.8},
	   {0.7	,0.4},
	   {0.9,-0.5},
	   {-0.7,0.4}	};

	temp_color_array[0] = 0.1;
	temp_color_array[1] = 0.5;
	temp_color_array[2] = 0.7;

	bspline_curve_set.push_back(BSpline_Curve(4 , init_control_points_2 , temp_color_array, init_knot_vector));

	vector<GLfloat> temp_point;

	// glNewList(bezier_curve_drawList, GL_COMPILE);

	for(i=0; i<bspline_curve_set.size(); i++)
	{
		glColor3d(bspline_curve_set[i].color[0],bspline_curve_set[i].color[1],bspline_curve_set[i].color[2]);
		glLineWidth(1.5);
		glBegin(GL_LINE_STRIP);
		for(j=0; j<segments_per_curve; j++)
		{
			temp_point = bspline_curve_set[i].returnPoint((GLfloat)j/(GLfloat)segments_per_curve);
			glVertex2f(temp_point[0] , temp_point[1]);
		}
		glEnd();
	}

}

void add_curve()
{
	GLfloat temp_control_points[4][2] = {
		{((GLfloat)rand()/RAND_MAX)*2 - 1	,((GLfloat)rand()/RAND_MAX)*2 - 1},
		{((GLfloat)rand()/RAND_MAX)*2 - 1	,((GLfloat)rand()/RAND_MAX)*2 - 1},
		{((GLfloat)rand()/RAND_MAX)*2 - 1	,((GLfloat)rand()/RAND_MAX)*2 - 1},
		{((GLfloat)rand()/RAND_MAX)*2 - 1	,((GLfloat)rand()/RAND_MAX)*2 - 1}	};

	GLfloat temp_color[3];

	temp_color[0] = (GLfloat)rand()/RAND_MAX;
	temp_color[1] = (GLfloat)rand()/RAND_MAX;
	temp_color[2] = (GLfloat)rand()/RAND_MAX;

    GLfloat temp_knot[9];

    temp_knot[0] = temp_knot[1] = 0.0;
    temp_knot[7] = temp_knot[8] = 1.0;

    for(int i=2;i<7; i++)
    {
        temp_knot[i] = ((GLfloat)rand()/RAND_MAX);
    }

	sort(temp_knot, temp_knot + 8);

	//cout<<color_r<<" "<<color_g<<" "<<color_b<<"\n";

	bspline_curve_set.push_back(BSpline_Curve(4 , temp_control_points , temp_color, temp_knot));

	glutPostRedisplay();
}

void toggle_mode_select_curve()
{
	if(mode_select_curve == 0)
	{
		mode_select_curve = 1;
		active_curve = 0;
		bspline_curve_set[0].active = 1;

		// memcpy(color_hist , bezier_curve_set[active_curve].color , sizeof(color_hist));
		// memcpy(bezier_curve_set[active_curve].color , active_color , sizeof(active_color));

		glutPostRedisplay();
	}

	else
	{
		mode_select_curve = 0;
		bspline_curve_set[active_curve].active = 0;
		// memcpy(bezier_curve_set[active_curve].color , color_hist , sizeof(color_hist));

		active_curve = -1;
		glutPostRedisplay();
	}
}

void affect_selection(GLint dir)
{
	if(mode_select_curve == 1)
	{
		bspline_curve_set[active_curve].active = 0;
		// memcpy(bezier_curve_set[active_curve].color , color_hist , sizeof(color_hist));

		active_curve += dir;

		if(active_curve < 0)
		{
			active_curve = bspline_curve_set.size() - 1;
		}
		else if(active_curve == bspline_curve_set.size())
		{
			active_curve = 0;
		}

		bspline_curve_set[active_curve].active = 1;
		// memcpy(color_hist , bezier_curve_set[active_curve].color , sizeof(color_hist));
		// memcpy(bezier_curve_set[active_curve].color , active_color , sizeof(active_color));
	}

	glutPostRedisplay();
}

void delete_curve()
{
	if(mode_select_curve==1 && bspline_curve_set.size() > 1)
	{
		bspline_curve_set.erase(bspline_curve_set.begin() + active_curve);

		if(active_curve = bspline_curve_set.size())
		{
			active_curve = 0;
		}

		bspline_curve_set[active_curve].active = 1;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:  // Escape Key -- good way to quit
      exit(0);  // Quit
      break;
   case 'n':
	   add_curve();
	   break;
   case 'p':
	   toggle_mode_select_curve();
	   break;
   case 'a':
	   affect_selection(-1);
	   break;
   case 'd':
	   affect_selection(+1);
	   break;
   case 'h':
	   delete_curve();
	   break;
   }
}

void display_curves()
{
	for(int i=0; i<bspline_curve_set.size(); i++)
	{
		glColor3d(bspline_curve_set[i].color[0],bspline_curve_set[i].color[1],bspline_curve_set[i].color[2]);
		if(bspline_curve_set[i].active==1)
		{
			glLineWidth(4.0);
		}
		else
		{
			glLineWidth(2.0);
		}
		glBegin(GL_LINE_STRIP);
		for(int j=0; j<=segments_per_curve; j++)
		{
			temp_point = bspline_curve_set[i].returnPoint((GLfloat)j/(GLfloat)segments_per_curve);
			glVertex2f(temp_point[0] , temp_point[1]);
		}
		glEnd();
	}
}

void display_control_points()
{
	if(mode_select_curve == 1)
	{
		glColor3d(0.1 , 1.0 , 0.1);
		glPointSize(5.0);
		glBegin(GL_POINTS);
		for(int j=0; j<bspline_curve_set[active_curve].degree; j++)
		{
			glVertex2f(bspline_curve_set[active_curve].control_points[j][0] , bspline_curve_set[active_curve].control_points[j][1]);
		}
		glEnd();
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//cout<<"HI\n";

	// glCallList(bezier_curve_drawList);

	display_curves();

	display_control_points();

	glutSwapBuffers();

	glFlush();
}

int main(int argc, char **argv)
{
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);

	glutInit(&argc, argv);
	glutInitWindowSize(start_win_size_width , start_win_size_height);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(300, 300);
	glutCreateWindow("Drawing Bspline Curves");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	// glEnable(GL_ALPHA_TEST);
	// glAlphaFunc(GL_NOTEQUAL, 0);
	// glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT , GL_NICEST);

	glutMainLoop();

	return 0;
}

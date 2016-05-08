// ----------------------------------------------------------------------------
// -----------------HEADER FILE FOR THE BEZIER CURVE CLASS---------------------
// ----------------------------------------------------------------------------

#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <math.h>
#include <fstream>

// Limit on 'N' --- (NOTE: Max number of control points = 'N'+1)
#define MAX_DEGREE_BEZIER 7
#define MAX_SEGMENT_POINTS 100
using namespace std;

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

class Bezier_Surface
{
public:

				Bezier_Surface(GLint , GLint, GLfloat[][3] , GLfloat[3] , GLint);
	void		make_draw_points();
	GLfloat		bezier_basis(GLint , GLint , GLfloat);
	GLint		calc_C(GLint, GLint);
	void		make_C();


	// ----------------------------------------------------------------------------
	// -----------------------------------VARIABLES--------------------------------
	// ----------------------------------------------------------------------------

	/*GLfloat control_points_top[MAX_DEGREE_BEZIER + 1][3];
	GLfloat control_points_bottom[MAX_DEGREE_BEZIER + 1][3];
	GLfloat control_points_left[MAX_DEGREE_BEZIER + 1][3];
	GLfloat control_points_right[MAX_DEGREE_BEZIER + 1][3];*/

	GLfloat control_points[MAX_DEGREE_BEZIER + 1][MAX_DEGREE_BEZIER + 1][3];

	GLint degree_x , degree_y;
	GLint segment_max;
	GLfloat color[4];
	GLint active;

	GLfloat draw_points[MAX_SEGMENT_POINTS][MAX_SEGMENT_POINTS][3];
	GLint C_val[MAX_DEGREE_BEZIER + 1][MAX_DEGREE_BEZIER + 1];

	GLfloat color_emi[4];
	GLfloat color_spe[4];

};
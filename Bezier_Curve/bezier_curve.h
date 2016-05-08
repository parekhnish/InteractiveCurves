// ----------------------------------------------------------------------------
// -----------------HEADER FILE FOR THE BEZIER CURVE CLASS---------------------
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <GL/glut.h>
#include <vector>

// Limit on 'N' --- (NOTE: Max number of control points = 'N'+1)
#define MAX_DEGREE_BEZIER 7
using namespace std;

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

class Bezier_Curve
{
public:
	// ----------------------------------------------------------------------------
	// -----------------------------------METHODS----------------------------------
	// ----------------------------------------------------------------------------
						Bezier_Curve(GLint , GLfloat[][2] , GLfloat[3]);	// Constructor
	vector<GLfloat>		returnPoint(GLfloat t);								// Return Point coordinates at parametric position 't'
	void				increase_degree();									// Increase degree of curve
	Bezier_Curve		subdivide_curve(GLfloat t);							// Subdivide curve at parametric point 't'
	void				join_curve(Bezier_Curve start_curve);				// Join this curve with curve specfied in argument
	void				reverse_curve();									// Reverse order of Control Points
	GLint				nearest_cp_index(GLfloat x , GLfloat y);			// Return index of Control Point nearest to coordinates passed in argument

	// ----------------------------------------------------------------------------
	// -----------------------------------VARIABLES--------------------------------
	// ----------------------------------------------------------------------------

	GLfloat				control_points[MAX_DEGREE_BEZIER+1][2];				// Control Points, stored as array of [N+1][2]
	GLint				degree;												// Degree = Number of Control Points
	GLfloat				color[3];											// RGB color array of curve
	GLint				active;												// Boolean variable, to state whether curve is currently selected or not

};
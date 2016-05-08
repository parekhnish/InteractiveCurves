#include <stdlib.h>
#include <Gl/glut.h>
#include <vector>
#define MAX_BSPLINE_DEGREE 10
using namespace std;

class BSpline_Curve
{
public:
	BSpline_Curve(GLint , GLfloat[][2], GLfloat[3], GLfloat[9]);	// Constructor for the Bspline curve
	vector<GLfloat> returnPoint(GLfloat t);
	void reset_knots();
	GLfloat bspline(int,int,float);

	GLint degree;		                            // Degree of the Bspline curve
	GLint active;		                                // Variable determining presence of the Bspline curve on the screen
	GLfloat color[3];	                                // RGB values for the rendered Bspline curve
	GLfloat knots[9];	                        // Knot Vectors for Bspline curve
	GLfloat control_points[MAX_BSPLINE_DEGREE+1][2];    // Control points
};

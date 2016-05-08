// ----------------------------------------------------------------------------
// ----------------------CLASS DECLARATION: BEZIER_CURVE-----------------------
// ----------------------------------------------------------------------------

#include "bezier_curve.h"
using namespace std;

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Constructor for the class
		INPUT:	Degree (= Number of Control Points)
				Control points (in an array of max. size [N][2])
				Color (in an array of size [3]) */
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

Bezier_Curve::Bezier_Curve(GLint deg , GLfloat pt[][2] , GLfloat input_color[3])
{
	// Initialise the degree
	degree = deg;

	// Initialise the control points
	for(int i=0; i<degree; i++)
	{
		memcpy(&control_points[i] , &pt[i] , sizeof(pt[0]));
	}

	// Initialise the color
	memcpy(color , input_color , sizeof(color));

	// Set active = 0
	active = 0;
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function that returns the coordinates of a point (in XY) as specified by the parameter 't'
		INPUT:	t (Parameter, ranges from [0,1])
		OUTPUT:	Vector of Coordinates ({x,y})	

	The algorithm used is the most basic one, which uses two loops to get the value of C(t),
	by iterating through the intermediate control points P(i)(j).
		<< View notes for more details >>	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

vector<GLfloat> Bezier_Curve::returnPoint(GLfloat t)
{
	// Make a temporary arry to hold the iterative control points that'll be calculated in the algorithm
	GLfloat temp_control_points[MAX_DEGREE_BEZIER+1][2];

	// Initialise the tarray to the control points present (I.e. Q[i] = P[i])
	for(int i=0; i<degree; i++)
	{
		temp_control_points[i][0] = control_points[i][0];
		temp_control_points[i][1] = control_points[i][1];
	}

	// For <1:degree> ---
	for(int k=1; k<degree; k++)
	{
		// --- For <0:degree-k>, calculate P(i)(j)
		for(int i=0; i<(degree-k); i++)
		{
			temp_control_points[i][0] = (1-t)*temp_control_points[i][0] + (t)*temp_control_points[i+1][0];
			temp_control_points[i][1] = (1-t)*temp_control_points[i][1] + (t)*temp_control_points[i+1][1];
		}
	}

	// Make a vector of coordinates of the final control point obtained
	vector<GLfloat> return_point;
	return_point.push_back(temp_control_points[0][0]);
	return_point.push_back(temp_control_points[0][1]);

	// Return this vector
	return(return_point); 

}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to increase degree of the curve	
	
	This uses the standard algorithm, stated in the notes	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Bezier_Curve::increase_degree()
{
	// CHECK IF THE MAX LIMIT HAS BEEN REACHED!
	if(degree < (MAX_DEGREE_BEZIER + 1))
	{
		// Initialise an array to hold the new control points
		GLfloat temp_control_points[MAX_DEGREE_BEZIER+1][2];

		// New_Control_Point[i] = (i/n+1)*P[i-1] + (1 - i/n+1)*P[i]
		for(int i=0; i<=degree; i++)
		{
			temp_control_points[i][0] = ((GLfloat)i/(GLfloat)degree) * control_points[i-1][0] + (1.0 - ((GLfloat)i/(GLfloat)degree)) * control_points[i][0];
			temp_control_points[i][1] = ((GLfloat)i/(GLfloat)degree) * control_points[i-1][1] + (1.0 - ((GLfloat)i/(GLfloat)degree)) * control_points[i][1];
		}

		// Increment the degree variable
		degree += 1;

		// Copy the new control points into the main control_point array
		for(int i=0; i<degree; i++)
		{
			memcpy(&control_points[i] , &temp_control_points[i] , sizeof(control_points[0]));
		}
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to subdivide the curve at the given parametric point 't'
		INPUT: t (Parametric point)	

	The algorithm used is the standard one, given in the notes	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

Bezier_Curve Bezier_Curve::subdivide_curve(GLfloat t)
{
	// Create an array to hold all the temporary intermediate control points P(i)(j)
	GLfloat rec_control_points[MAX_DEGREE_BEZIER+1][MAX_DEGREE_BEZIER+1][2];

	// Start by initialisng them to the current control points
	for(int i=0; i<degree; i++)
	{
		rec_control_points[0][i][0] = control_points[i][0];
		rec_control_points[0][i][1] = control_points[i][1];
	}

	// Obtain ALL the intermediate control points P(i)(j)
	for(int i=1; i<degree; i++)
	{
		for(int j=0; j<(degree-i); j++)
		{
			rec_control_points[i][j][0] = (1-t)*rec_control_points[i-1][j][0] + (t)*rec_control_points[i-1][j+1][0];
			rec_control_points[i][j][1] = (1-t)*rec_control_points[i-1][j][1] + (t)*rec_control_points[i-1][j+1][1];
		}
	}

	// Initialise an array for the control points of the NEW curve formed after subdivision
	GLfloat new_control_points[MAX_DEGREE_BEZIER+1][2];

	// Old curve will have control points as: P(0)(0),		P(1)(0),	P(2)(0) ...
	// New curve will have control points as: P(N-1)(0),	P(N-2)(1),	P(N-3)(2) ...
	for(int i=0; i<degree; i++)
	{
		memcpy(&control_points[i] , &rec_control_points[i][0] , sizeof(rec_control_points[0][0]));

		memcpy(&new_control_points[i] , &rec_control_points[degree-i-1][i] , sizeof(rec_control_points[0][0]));
	}

	// Initialise and assign a random color to the new curve
	GLfloat temp_color[3];

	temp_color[0] = (GLfloat)rand()/RAND_MAX;
	temp_color[1] = (GLfloat)rand()/RAND_MAX;
	temp_color[2] = (GLfloat)rand()/RAND_MAX;

	// Create a new curve using all the things calculated above
	Bezier_Curve new_curve = Bezier_Curve(degree , new_control_points , temp_color);

	// Return this curve
	return(new_curve);
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to join the curve with the curve specified in the arguments
		INPUT: start_curve (Bezier curve, to which the curve has to be joined)	

	The current curve will be joined to the curve in the argument, using C1
	continuity.	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Bezier_Curve::join_curve(Bezier_Curve start_curve)
{
	// (For the following notation, M = start_degree = degree of the 'left'/'starting' curve)
	GLint start_degree = start_curve.degree;

	// Establishing C0 continuity ( I.e. P[0] = Q[M-1] )
	control_points[0][0] = start_curve.control_points[start_degree-1][0];
	control_points[0][1] = start_curve.control_points[start_degree-1][1];

	// Establishing C1 continuity ( I.e. P[1] = ((M)*(Q[M-1] - Q[M-2]) + (N)*P[0])/N )
	control_points[1][0] = (((GLfloat)(start_degree-1.0))*(start_curve.control_points[start_degree-1][0] - start_curve.control_points[start_degree-2][0]) + ((GLfloat)degree-1.0)*(control_points[0][0])) / ((GLfloat)degree-1.0);
	control_points[1][1] = (((GLfloat)(start_degree-1.0))*(start_curve.control_points[start_degree-1][1] - start_curve.control_points[start_degree-2][1]) + ((GLfloat)degree-1.0)*(control_points[0][1])) / ((GLfloat)degree-1.0);
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to reverse the curve, by reversing the order of control points	

	This is to be used when joinin curves at different endpoints	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Bezier_Curve::reverse_curve()
{
	// --- Simple reversal of control points --- //
	GLfloat temp;
	for(int i=0; i<degree/2; i++)
	{
		temp = control_points[i][0];
		control_points[i][0] = control_points[degree-1-i][0];
		control_points[degree-1-i][0] = temp;

		temp = control_points[i][1];
		control_points[i][1] = control_points[degree-1-i][1];
		control_points[degree-1-i][1] = temp;
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function that returns the index of the control point nearest to the coordinates passed as arguments
		INPUT:	x,y (Input coordinates)
		OUTPUT:	index (of the control point)	

	This is to be used when selecting a control point for changing it through dragging methods	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

GLint Bezier_Curve::nearest_cp_index(GLfloat x , GLfloat y)
{
	// Initialise min_dist = (p - P[0])^2 , and index = 0
	GLfloat min_dist = ((x - control_points[0][0])*(x - control_points[0][0]) + (y - control_points[0][1])*(y - control_points[0][1]));
	GLint index = 0;

	// Loop over all control points, find the one with the minimum distance to the given point
	for(int i=1; i<degree; i++)
	{
		if(((x - control_points[i][0])*(x - control_points[i][0]) + (y - control_points[i][1])*(y - control_points[i][1])) < min_dist)
		{
			min_dist = ((x - control_points[i][0])*(x - control_points[i][0]) + (y - control_points[i][1])*(y - control_points[i][1]));
			index = i;
		}
	}

	// Return the control point's index
	return(index);
}
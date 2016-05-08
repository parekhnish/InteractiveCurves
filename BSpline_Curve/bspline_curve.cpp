#include "bspline_curve.h"

using namespace std;

BSpline_Curve::BSpline_Curve(GLint deg , GLfloat pt[][2] , GLfloat input_color[3], GLfloat knot[9])
{
	// Initialise the degree
	degree = deg;
	// Initialise the control points
	for(int i=0; i<degree; i++)
	{
		memcpy(&control_points[i] , &pt[i] , sizeof(pt[0]));
	}
	memcpy(color , input_color , sizeof(color));
	active = 0;

	for(int i=0; i<9; i++)
    {
        knots[i] = knot[i];
    }
}

GLfloat BSpline_Curve::bspline(int index,int order,float t)
{
    float coef1,coef2;
    if(order == 1)
    {
        if((t>=knots[index]) && (t<knots[index+1]))
            return 1.0;
        else
            return 0.0;
    }
    else
    {
        if(knots[index+order-1] == knots[index])    // If the denominator is zero for first coefficient
        {
            if(t == knots[index])
                coef1 = 1.0;
            else
                coef1 = 0.0;
        }
        else
        {
            coef1 = (t-knots[index])/(knots[index+order-1]-knots[index]);
        }

        if(knots[index+order] == knots[index+1])    // If the denominator is zero for second coefficient
        {
            if(t == knots[index+order])
                coef2 = 1.0;
            else
                coef2 = 0.0;
        }

        else
        {
            coef2 = (knots[index+order]-t)/(knots[index+order]-knots[index+1]);
        }
    }

    return ((coef1*bspline(index,order-1,t))+(coef2*bspline(index+1,order-1,t)));
}

vector<GLfloat> BSpline_Curve::returnPoint(GLfloat t)
{
    GLfloat temp_x=0,temp_y=0;

    for(int i=0;i<degree; i++)
    {
        GLfloat spline_value = bspline(i,degree,t);
        temp_x += (control_points[i][0]*spline_value);
        temp_y += (control_points[i][1]*spline_value);
    }

    vector<GLfloat> return_point;
    return_point.push_back(temp_x);
    return_point.push_back(temp_y);
    return return_point;
}


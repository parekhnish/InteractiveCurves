// ----------------------------------------------------------------------------
// ----------------------CLASS DECLARATION: BEZIER_SURFACE-----------------------
// ----------------------------------------------------------------------------

#include "bezier_surface.h"
using namespace std;

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

Bezier_Surface::Bezier_Surface(GLint degX, GLint degY, GLfloat cp[][3], GLfloat input_color[4] , GLint seg_max)
{
	make_C();

	// Initialise the degree
	degree_x = degX;
	degree_y = degY;

	// Initialise the control points
	for(int i=0; i<=degree_x; i++)
	{
		for(int j=0; j<=degree_y; j++)
		{
			memcpy(&control_points[i][j] , &cp[i*(degree_y+1) + j] , sizeof(cp[i]));
		}
	}

	// Initialise the color
	memcpy(color , input_color , sizeof(color));

	// Set active = 0
	active = 0;

	// Set segment_max
	segment_max = seg_max;

	// Set Specular and Emission Colors
	color_spe[0] = color_spe[1] = color_spe[2] = color_spe[3] = 1.0;

	color_emi[0] = color_emi[1] = color_emi[2] = 0.0;
	color_emi[3] = 1.0;

	make_draw_points();
}


void Bezier_Surface::make_draw_points()
{
	GLfloat bx, by;
	ofstream op_file;
	op_file.open("DEBUG.txt");
	for(GLint i=0; i<=segment_max; i++)
	{
		for(GLint j=0; j<=segment_max; j++)
		{
			draw_points[i][j][0] = 0;
			draw_points[i][j][1] = 0;
			draw_points[i][j][2] = 0;

			for(int m=0; m<=degree_x; m++)
			{
				for(int n=0; n<=degree_y; n++)
				{
					bx = bezier_basis(degree_x , m , (GLfloat)i/segment_max);
					by = bezier_basis(degree_y , n , (GLfloat)j/segment_max);

					draw_points[i][j][0] += bx*by*control_points[m][n][0];
					draw_points[i][j][1] += bx*by*control_points[m][n][1];
					draw_points[i][j][2] += bx*by*control_points[m][n][2];
				}
			}

			op_file<<draw_points[i][j][0]<<" "<<draw_points[i][j][1]<<" "<<draw_points[i][j][2]<<"\n";
		}
	}

	op_file.close();

}


GLfloat Bezier_Surface::bezier_basis(GLint n, GLint i, GLfloat u)
{
	return (C_val[n][i] * pow(u,i) * pow(1.0-u,n-i));
}


GLint Bezier_Surface::calc_C(GLint n , GLint r)
{
	if(n==r)
	{
		return 1;
	}

	if(r==1)
	{
		return n;
	}

	if(r==0)
	{
		return 1;
	}

	return (calc_C(n-1,r) + calc_C(n-1,r-1));
}


void Bezier_Surface::make_C()
{
	for(int n=0; n<=MAX_DEGREE_BEZIER; n++)
	{
		for(int r=0; r<=n; r++)
		{
			C_val[n][r] = calc_C(n,r);
		}
	}
}
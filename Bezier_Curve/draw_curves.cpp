#include <stdlib.h>
#include <iostream>
#include <GL/glut.h> // GLUT stuff - includes OpenGL headers
#include <iostream>
#include <vector>
#include <time.h>
#include <AntTweakBar.h>

#include "bezier_curve.h"	// Include the Bezier_Curve Class
using namespace std;

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

GLint	start_win_size_width	= 300;						// Starting Window Width (in Pixels)
GLint	start_win_size_height	= 300;						// Starting Window Height (in Pixels)
GLint	curr_win_size_width		= start_win_size_width;		// Current Window Width
GLint	curr_win_size_height	= start_win_size_height;	// Current Window Height

GLint	segments_per_curve		= 100;						// Number of line segments / points to be used while drawing a curve

GLfloat	left_boundary			= -1.0;						// Left Clipping Boundary (in World Coordinates)
GLfloat	right_boundary			= 1.0;						// Right Clipping Boundary (in World Coordinates)
GLfloat	top_boundary			= 1.0;						// Top Clipping Boundary (in World Coordinates)
GLfloat	bottom_boundary			= -1.0;						// Bottom Clipping Boundary (in World Coordinates)
GLfloat	min_boundary_range		= 2.0;						// Minimum difference between {top-bottom , left-right} (In World Coordinates)

GLint	mode_select_curve		= 0;						// Boolean, to indicate whether 'Curve Selection' mode is on or off
GLint	active_curve			= -1;						// Holds the index of the currently selected/'active' curve
GLint	active_cp				= -1;						// Holds the index of the currently selected/'active' Control Point of the 'active' curve

GLint	mode_join_curve			= 0;						// Boolean, to indicate whether 'Curve Joining' mode is on or off
GLint	join_curve_start		= -1, join_curve_end =-1;	// Hold indexes of the 'starting' and 'ending' curves which are to be joined

vector<Bezier_Curve>	bezier_curve_set;					// Vector of Bezier Curves currently drawn in the application
vector<GLfloat>			temp_point;


// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	CallBack function when the Window is resized
	
	Algorithm is too intricate to explain ( :P ). What it basically does is
	checks whether the window has grown or shrunk, and tried to accommodate the
	biggest part of the previous window in this window. 

	<< As I said, not very descriptive ( :P ) >>. But it works	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void reshape(int w, int h)
{
   glViewport(0, 0, w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   if((w < curr_win_size_width) || (h < curr_win_size_height))
   {
	   if(((double)w/h) < ((double)curr_win_size_width/curr_win_size_height))
	   {
		   GLfloat sr_width = (double)curr_win_size_width*h/curr_win_size_height;
		   left_boundary = left_boundary + ((((double)sr_width/w)-1.0)/2.0)*min_boundary_range;
		   right_boundary = right_boundary - ((((double)sr_width/w)-1.0)/2.0)*min_boundary_range;
	   }
	   else
	   {
		   GLfloat sr_height = (double)curr_win_size_height*w/curr_win_size_width;
		   bottom_boundary = bottom_boundary + ((((double)sr_height/h)-1.0)/2.0)*min_boundary_range;
		   top_boundary = top_boundary - ((((double)sr_height/h)-1.0)/2.0)*min_boundary_range;
	   }
   }
   else
   {
	   if(((double)w/h) > ((double)curr_win_size_width/curr_win_size_height))
	   {
		   GLfloat sr_width = (double)curr_win_size_width*h/curr_win_size_height;
		   left_boundary = left_boundary - ((((double)w/sr_width)-1.0)/2.0)*min_boundary_range;
		   right_boundary = right_boundary + ((((double)w/sr_width)-1.0)/2.0)*min_boundary_range;
	   }
	   else
	   {
		   GLfloat sr_height = (double)curr_win_size_height*w/curr_win_size_width;
		   bottom_boundary = bottom_boundary - ((((double)h/sr_height)-1.0)/2.0)*min_boundary_range;
		   top_boundary = top_boundary + ((((double)h/sr_height)-1.0)/2.0)*min_boundary_range;
	   }
   }

   gluOrtho2D(left_boundary , right_boundary , bottom_boundary , top_boundary);

   curr_win_size_height = h;
   curr_win_size_width = w;

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	The Global Initialisation Function

	This is where a few starting curves are initialised, along with a few other
	things	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void init()
{
	GLint i,j,k;

	// Clear Background Color
	glClearColor(0.0 , 0.0 , 0.0 , 0.0);

	// Initialise a Bezier Curve
	GLfloat init_control_points_1[4][2] = {
	   {0.0	,0.0},
	   {-0.9	,0.1},
	   {0.2	,	0.8},
	   {0.7	,-0.4}	};

	GLfloat temp_color_array[3] = {0.9,0.1,0.1};
	bezier_curve_set.push_back(Bezier_Curve(4 , init_control_points_1 , temp_color_array));

	// Initialise another Bezier curve
	GLfloat init_control_points_2[4][2] = {
	   {1.0	,-1.0},
	   {-0.2	,-0.1},
	   {0.4	,	-0.8},
	   {0.7	,0.4}	};

	temp_color_array[0] = 0.1;
	temp_color_array[1] = 0.5;
	temp_color_array[2] = 0.7;

	bezier_curve_set.push_back(Bezier_Curve(4 , init_control_points_2 , temp_color_array));
	vector<GLfloat> temp_point;

}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to add a new curve to the application
	
	This function adds a random Bezier curve (of degree = 4) to the
	application, with random control points and a random color	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void add_curve()
{
	// Initialise 4 random control points
	GLfloat temp_control_points[4][2] = {
		{((GLfloat)rand()/RAND_MAX)*2 - 1	,((GLfloat)rand()/RAND_MAX)*2 - 1},
		{((GLfloat)rand()/RAND_MAX)*2 - 1	,((GLfloat)rand()/RAND_MAX)*2 - 1},
		{((GLfloat)rand()/RAND_MAX)*2 - 1	,((GLfloat)rand()/RAND_MAX)*2 - 1},
		{((GLfloat)rand()/RAND_MAX)*2 - 1	,((GLfloat)rand()/RAND_MAX)*2 - 1}	};

	// Initialise a random color
	GLfloat temp_color[3];
	temp_color[0] = (GLfloat)rand()/RAND_MAX;
	temp_color[1] = (GLfloat)rand()/RAND_MAX;
	temp_color[2] = (GLfloat)rand()/RAND_MAX;

	// Add this bezier curve to the set
	bezier_curve_set.push_back(Bezier_Curve(4 , temp_control_points , temp_color));

	glutPostRedisplay();
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Toggle function, to switch the 'Curve Selection' mode on/off	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void toggle_mode_select_curve()
{
	// If it is OFF, switch it ON!
	if(mode_select_curve == 0)
	{
		mode_select_curve = 1;	// Set the mode to ON

		// Set the first curve as the active curve
		active_curve = 0;		
		bezier_curve_set[0].active = 1;

		glutPostRedisplay();
	}

	// If it is ON, switch it OFF!
	else
	{
		mode_select_curve = 0;	// Set the mode to OFF

		// De-activate the 'till-now' active curve
		bezier_curve_set[active_curve].active = 0;
		active_curve = -1;

		glutPostRedisplay();
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Toggle function, to switch the 'Curve Joining' mode ON/STAGE-1/STAGE-2	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void toggle_mode_join_curve()
{
	// If it is currently OFF ...
	if(mode_join_curve == 0)
	{
		mode_join_curve = 1;	// ... Set it to STAGE-1 (I.e. selecting the 'start' curve)

		// Set the first curve as the 'start' curve
		join_curve_start = 0;
		bezier_curve_set[join_curve_start].active = 1;

		glutPostRedisplay();
	}

	// If it is in STAGE-1 or STAGE-2 ...
	else
	{
		mode_join_curve = 0;	// ... Switch it OFF

		// De-activate the 'start' curve
		bezier_curve_set[join_curve_start].active = 0;

		// (If 'end' curve has been selected) -- De-activate the 'end' curve
		if(join_curve_end != -1)
		{
			bezier_curve_set[join_curve_end].active = 0;
		}

		// Reset the 'start' and 'end' curve indices
		join_curve_start = -1;
		join_curve_end = -1;

		glutPostRedisplay();
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to change the selected curve
		INPUT: Direction (+1 = Forward, -1 = Backward)	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void affect_selection(GLint dir)
{
	// If in the 'Curve Selection' Mode ...
	if(mode_select_curve == 1)
	{
		// De-activate the currently 'active' curve
		bezier_curve_set[active_curve].active = 0;

		// Increment index (with loop-back), and activate this new curve
		active_curve = (active_curve + dir)%bezier_curve_set.size();
		bezier_curve_set[active_curve].active = 1;

		glutPostRedisplay();
	}

	// If in the 'Curve Joining' - STAGE-1 Mode ...
	if(mode_join_curve == 1)
	{
		// De-activate the current 'start' curve
		bezier_curve_set[join_curve_start].active = 0;

		// Increment 'start' index (with loopback), and activate this new curve
		join_curve_start = (join_curve_start + dir)%bezier_curve_set.size();
		bezier_curve_set[join_curve_start].active = 1;

		glutPostRedisplay();
	}

	// If in the 'Curve Joining' - STAGE-2 Mode ...
	if(mode_join_curve == 2)
	{
		// De-activate the current 'end' curve
		bezier_curve_set[join_curve_end].active = 0;

		// Increment the 'end' index (with loopback and a check that it isn't the 'start' curve), and activate this new curve
		join_curve_end = (join_curve_end + dir)%bezier_curve_set.size();
		if(join_curve_start == join_curve_end)
		{
			join_curve_end = (join_curve_end + dir)%bezier_curve_set.size();
		}
		bezier_curve_set[join_curve_end].active = 1;

		glutPostRedisplay();
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to delete the currently 'active' curve */
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void delete_curve()
{
	// If in the 'Curve Selection' Mode ...
	if(mode_select_curve==1 && bezier_curve_set.size() > 1)
	{
		// Remove the 'active' curve from the Set of Curves
		bezier_curve_set.erase(bezier_curve_set.begin() + active_curve);

		// Make the next (with loopback) curve 'active'
		if(active_curve = bezier_curve_set.size())
		{
			active_curve = 0;
		}
		bezier_curve_set[active_curve].active = 1;
	}

	glutPostRedisplay();
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to increase the degree of the 'active' curve	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void increase_curve_degree()
{
	// If in the 'Curve Selection' mode ...
	if(mode_select_curve == 1)
	{
		bezier_curve_set[active_curve].increase_degree();	// Call the 'active' curve's IncreaseDegree() function
		glutPostRedisplay();
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to subdivide the 'active' curve	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void subdivide_curve()
{
	// If in the 'Curve Selection' Mode ...
	if(mode_select_curve == 1)
	{
		// Insert the new curve just after the original curve, in the Set of Curves.
		// TODO --- HARDCODED TO SUBDIVIDE AT t=0.5. SHOULD BE MADE INTERACTIVE
		bezier_curve_set.insert(bezier_curve_set.begin()+active_curve+1 , bezier_curve_set[active_curve].subdivide_curve(0.5));
		glutPostRedisplay();
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to choose the 'start' / 'end' curve while joining two curves	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void select_joining_curve()
{
	// If in the 'Curve Joining' - STAGE-1 Mode...
	if(mode_join_curve == 1)
	{
		// Initialise the 'end' curve to index=0, (or to index=1 if index=0 is already the 'start' curve)
		join_curve_end = 0;
		if(join_curve_end == join_curve_start)
		{
			join_curve_end++;
		}

		// Activate the 'end' curve
		bezier_curve_set[join_curve_end].active = 1;

		// Switch the mode to STAGE-2
		mode_join_curve = 2;

		glutPostRedisplay();
		return;
	}

	// If in the 'Curve Joining' - STAGE-2 Mode ...
	if(mode_join_curve == 2)
	{
		// Call the JoinCurve() method of the 'end' curve, passing the 'start' curve as an argument
		bezier_curve_set[join_curve_end].join_curve(bezier_curve_set[join_curve_start]);
		glutPostRedisplay();

		// Switch OFF the 'Curve Joining' Mode
		mode_join_curve = 0;

		// De-activate the 'start' and 'end' curves
		bezier_curve_set[join_curve_start].active = 0;
		bezier_curve_set[join_curve_end].active = 0;

		// Reset the 'start' and 'end' indices
		join_curve_start = -1;
		join_curve_end = -1;

		return;
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to reverse the 'active' curve	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void reverse_curve()
{
	// If in the 'Curve Selection' Mode...
	if(mode_select_curve == 1)
	{
		// Call the active curve's ReverseCurve() function
		bezier_curve_set[active_curve].reverse_curve();
		glutPostRedisplay();
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to translate the viewing area
		INPUT:	type (1= Horizontal, 2= Vertical)
				dir  (+1= Right/Up, -1=Left/Down)	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void shift_view(GLint type , GLint dir)
{
	// Horizontal Translation -- Change the left and right clipping planes
	if(type == 1)
	{
		left_boundary += (GLfloat)dir*min_boundary_range*0.2;
		right_boundary += (GLfloat)dir*min_boundary_range*0.2;
	}

	// Vertical Translation -- Change the top and bottom clipping planes
	else if(type == 2)
	{
		top_boundary += (GLfloat)dir*min_boundary_range*0.2;
		bottom_boundary += (GLfloat)dir*min_boundary_range*0.2;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left_boundary , right_boundary , bottom_boundary , top_boundary);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to zoom in/out
	INPUT:	type(1= Zoom IN , 2= Zoom OUT)	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void zoom_view(GLint type)
{
	// Zoom IN -- Bring the clipping planes closer
	if(type == 1)
	{
		left_boundary = left_boundary + 0.125*((double)right_boundary - left_boundary);
		right_boundary = right_boundary - 0.125*((double)right_boundary - left_boundary);
		bottom_boundary = bottom_boundary + 0.125*((double)top_boundary - bottom_boundary);
		top_boundary = top_boundary - 0.125*((double)top_boundary - bottom_boundary);
	}

	// Zoom OUT -- Spread out the clipping planes
	else if(type == 2)
	{
		left_boundary = left_boundary - 0.125*((double)right_boundary - left_boundary);
		right_boundary = right_boundary + 0.125*((double)right_boundary - left_boundary);
		bottom_boundary = bottom_boundary - 0.125*((double)top_boundary - bottom_boundary);
		top_boundary = top_boundary + 0.125*((double)top_boundary - bottom_boundary);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left_boundary , right_boundary , bottom_boundary , top_boundary);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	CallBack function for Keyboard Input	
	INPUT:	key (The char which was pressed)
			x,y (coordinates of the mouse pointer when the key was pressed)	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	/* KEY MAPPINGS:

		n:	Add a Curve
		p:	Toggle 'Curve Selection' Mode
		q:	Toggle 'Curve Joining' Mode
		a:	Change selection (Backward)
		d:	Change selection (Forward)
		h:	Delete curve
		k:	Increase degree of curve
		v:	Subdivide curve
		.:	Select Curve (in 'Curve Joining' mode)
		r:	Reverse Curve

		4,5,6,8:	Translate view (use the NumPad for intuitively knowing which key is for which direction )

		1,0:		Zoom IN/OUT , respectively	*/

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
   case 'q':
	   toggle_mode_join_curve();
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
   case 'k':
	   increase_curve_degree();
	   break;
   case 'v':
	   subdivide_curve();
	   break;
   case '.':
	   select_joining_curve();
	   break;
   case 'r':
	   reverse_curve();
	   break;
   case '4':
	   shift_view(1,-1);
	   break;
   case '6':
	   shift_view(1,1);
	   break;
   case '5':
	   shift_view(2,-1);
	   break;
   case '8':
	   shift_view(2,1);
	   break;
   case '1':
	   zoom_view(1);
	   break;
   case '0':
	   zoom_view(2);
	   break;
   }
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	CallBack function for Mouse UP/DOWN events
	INPUT:	button (which caused the event - Left, Middle or Right)
			state  (UP/DOWN)
			x,y	   (Coordinates of the mouse pointer.
					NOTE--- here, origin is TOP-LEFT, not TOP-RIGHT!)	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void mouseClick(GLint button , GLint state , GLint x , GLint y)
{
	// If in the 'Curve Selection' Mode ..
	if(mode_select_curve == 1)
	{
		// If the LEFT BUTTON caused the event ..
		if(button == GLUT_LEFT_BUTTON)
		{
			// If it was a Down-Click ...
			if(state == GLUT_DOWN)
			{
				// Get the World Coordinates corresponding to the mouse pointer coordinates
				GLfloat x_coord = left_boundary + ((GLfloat)x/curr_win_size_width)*(right_boundary - left_boundary);
				GLfloat y_coord = top_boundary - ((GLfloat)y/curr_win_size_height)*(top_boundary - bottom_boundary);

				// Set the active control point as the one nearest to the mouse pointer
				active_cp = bezier_curve_set[active_curve].nearest_cp_index(x_coord , y_coord);

				// Change the active control point coordinates, to match with the mouse pointer coordinates
				bezier_curve_set[active_curve].control_points[active_cp][0] = x_coord;
				bezier_curve_set[active_curve].control_points[active_cp][1] = y_coord;

				glutPostRedisplay();
			}

			// If it was an Up-Click ...
			else if(state == GLUT_UP)
			{
				active_cp = -1;	// ... De-activate the active control point
			}
		}
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	CallBack function for when the mouse pointer is dragged
	INPUT:	x,y	   (Coordinates of the mouse pointer.
					NOTE--- here, origin is TOP-LEFT, not TOP-RIGHT!)	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void mouseDrag(GLint x, GLint y)
{
	// If in the 'Curve Selection' Mode ...
	if(mode_select_curve == 1)
	{
		// Get the World Coordinates corresponding to the mouse pointer coordinates
		GLfloat x_coord = left_boundary + ((GLfloat)x/curr_win_size_width)*(right_boundary - left_boundary);
		GLfloat y_coord = top_boundary - ((GLfloat)y/curr_win_size_height)*(top_boundary - bottom_boundary);

		// Change the active control point's coordinates to the one pointed to by the mouse
		bezier_curve_set[active_curve].control_points[active_cp][0] = x_coord;
		bezier_curve_set[active_curve].control_points[active_cp][1] = y_coord;

		glutPostRedisplay();
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to display the curves	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void display_curves()
{
	// For every curve in the Set of Curves ...
	for(int i=0; i<bezier_curve_set.size(); i++)
	{
		// Load the curve's color
		glColor3d(bezier_curve_set[i].color[0],bezier_curve_set[i].color[1],bezier_curve_set[i].color[2]);

		// If the curve is 'active', draw it thicker than the others
		if(bezier_curve_set[i].active==1)
		{
			glLineWidth(4.0);
		}
		else
		{
			glLineWidth(2.0);
		}

		// Begin Drawing the Line Strip
		glBegin(GL_LINE_STRIP);

		// For t = [0,1], in steps of 1/segments_per_curve ...
		for(int j=0; j<=segments_per_curve; j++)
		{
			// Get the point C(t), and use it to draw the line
			temp_point = bezier_curve_set[i].returnPoint((GLfloat)j/(GLfloat)segments_per_curve);
			glVertex2f(temp_point[0] , temp_point[1]);
		}
		glEnd();	// End the Line Strip
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	Function to display the 'active' curve's Control Points	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void display_control_points()
{
	// If in the 'Curve Selection' Mode...
	if(mode_select_curve == 1)
	{
		// Set a color (HARDCODED = GREEN) and size for the control points
		glColor3d(0.1 , 1.0 , 0.1);
		glPointSize(5.0);

		// Begin Drawing Points
		glBegin(GL_POINTS);

		// Draw all the control ponts of the active curve
		for(int j=0; j<bezier_curve_set[active_curve].degree; j++)
		{
			glVertex2f(bezier_curve_set[active_curve].control_points[j][0] , bezier_curve_set[active_curve].control_points[j][1]);
		}
		glEnd();	// End drawing points
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	CallBack function for the OpenGL Display method	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void display()
{
	// Clear the Background
	glClear(GL_COLOR_BUFFER_BIT);

	display_curves();			// Display curves

	display_control_points();	// Display the control points

	glutSwapBuffers();
	glFlush();
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/*	MAIN FUNCTION	*/
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	// Initialise the Random number Generator with a seed derived from the Machine Clock
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);

	glutInit(&argc, argv);

	// Initialise the Window
	glutInitWindowSize(start_win_size_width , start_win_size_height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Drawing Bezier Curves");

	// Call the Initial Function
	init();

	// Set the CallBacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag);

	// Enable various parameters FOR LINE SMOOTHNESS
	glEnable(GL_ALPHA_TEST); 
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT , GL_NICEST);

	glutMainLoop();

	return 0;
}
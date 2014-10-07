
#include <stdio.h>	// for using "printf"
#include "stdafx.h"
#include<random>
#include"imageloader.h"
#include <GL/glut.h> 
#include <stdlib.h>
#include <stdarg.h>

//mountain struct
struct mountain
{
	int terPeakx;
	int terPeakz;
	int terPeaky;
	int peakRand;
	float peakSteap;
};
//light settings
GLfloat lig_specular[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat lig_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lig_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
//mountains array
mountain mountains[5];
//iterations
int iterate = 3;
//spotlight
bool spotlight = false;
//camera positions
float camx = 100;
float camy = 0.9;
float camz = 110;
float yrot = 0;
float xrot = 0;
//movement
float camspeed = 0.05;
float prevx = 0;
float prevy = 0;
float yrotrad;
float planespin = 0;
float doors = -0.5;
//key press bools true = down
bool w = false;
bool s = false;
bool a = false;
bool d = false;
bool q = false;
//interactions
bool norm = false;
bool qold = false;
bool door = false;
bool door2 = true;
bool spot = false;
//terrain size
const int terRow = 200;
const int terCol = 200;
//terrain random
int random_amount = 300;
//white light level, between 1 and 0
float brightness = 1;
//primitive type, max = 9
int primative = 4;
//framecount variables
int frame_count = 0;
int current_time = 0;
int previous_time = 0;
float fps = 0;
//font
GLvoid* g_pFont	= GLUT_BITMAP_HELVETICA_18;
//set the light position
GLfloat light_position[] = { 50, 10, 50, 1.0 };
//skybox texels and indicies
GLfloat texel_right[4][2] = {{1,1},{0,1},{0,0},{1,0}};
GLfloat texel_left[4][2] = {{0,1},{1,1},{1,0},{0,0}};
GLfloat texel_top[4][2] = {{1,1},{1,0},{0,0},{0,1}};
GLfloat texel_back[4][2] = {{0,0},{0,1},{1,1},{1,0}};
GLfloat texel_front[4][2] = {{1,0},{1,1},{0,1},{0,0}};
int indicies_sky[4] = {0,1,2,3};
//vertex arrays, wing verticies                 top                      bottom                             sides
GLfloat wing_vertices[18][3] = {{0,0,0},{2,0,3},{2,0,-3},{0,-0.1,0},{2,-0.1,3},{2,-0.1,-3},{0,0,0},{2,0,3},{0,-0.1,0},{2,0,3},{0,-0.1,0},{2,-0.1,3}
,{0,0,0},{2,0,-3},{0,-0.1,0},{2,0,-3},{0,-0.1,0},{2,-0.1,-3} };

GLfloat wing_normals[18][3] = {{0,1,0},{0,1,0},{0,1,0},{0,-1,0},{0,-1,0},{0,-1,0},{0.3,0,-0.2},{0.3,0,-0.2},{0.3,0,-0.2},{0.3,0,-0.2},{0.3,0,-0.2},{0.3,0,-0.2},{-0.3,0,-0.2},{-0.3,0,-0.2},{-0.3,0,-0.2},{-0.3,0,-0.2},{-0.3,0,-0.2},{-0.3,0,-0.2}};
//hangar verticies front
GLfloat hangar_verticies[48][3] = {{0,0,0},{0.1,0,0},{0.1,1,0},{0,0,0},{0.1,1,0},{0,1,0},{1,0,0},{0.9,0,0},{0.9,1,0},{1,0,0},{0.9,1,0},{1,1,0},
	//side
{0,0,0},{0,0,2},{0,1,2},{0,0,0},{0,1,0},{0,1,2},{1,0,0},{1,0,2},{1,1,2},{1,0,0},{1,1,0},{1,1,2},
//back
{0,0,2},{1,0,2},{1,1,2},{0,0,2},{0,1,2},{1,1,2},
//top triangle
{0,1,0},{1,1,0},{0.5,1.2,0}, {0,1,2},{1,1,2},{0.5,1.2,2},
//roof
{0,1,0},{0,1,2},{0.5,1.2,2},{0,1,0},{0.5,1.2,0},{0.5,1.2,2},

{1,1,0},{1,1,2},{0.5,1.2,2},{1,1,0},{0.5,1.2,0},{0.5,1.2,2}};

GLfloat hangar_normals[48][3]  = {{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},
{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0},
{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},
{0,0,-1},{0,0,-1},{0,0,-1},{0,0,1},{0,0,1},{0,0,1},
{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0},
{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0}};
//button
GLfloat button_verticies[36][3] = {{0,0,0},{1,0,0},{1,2,0},{0,0,0},{0,2,0},{1,2,0},
{0,0,0},{0,0,1},{0,2,1},{0,0,0},{0,2,0},{0,2,1},
{1,0,0},{1,0,1},{1,2,1},{1,0,0},{1,2,0},{1,2,1},
{0,0,1},{1,0,1},{1,2.4,1},{0,0,1},{0,2.4,1},{1,2.4,1},
{0,2,0},{0,2.4,1},{1,2.4,1},{0,2,0},{1,2,0},{1,2.4,1},
{0,2,0},{0,2.4,1},{0,2,1},{1,2,0},{1,2.4,1},{1,2,1}};

GLfloat button_normals[36][3] = {{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},
{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},
{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0},
{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},
{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0},
{-1,0,0},{-1,0,0},{-1,0,0},{1,0,0},{1,0,0},{1,0,0}};

//create the arrays for the verticies, normals, indicies and texels
GLfloat cube_vertices[terRow*terCol][3];
GLfloat cube_normal[terRow*terCol][3];
int indices[(terRow-1)*(terCol-1)*6];
GLfloat texels[(terRow-1)*(terCol-1)*6][2];
//create an image to load the texture for the grass in to
Image tex("",64,64);
//skybox
Image sky_back_tex("",512,512);
Image sky_front_tex("",512,512);
Image sky_left_tex("",512,512);
Image sky_right_tex("",512,512);
Image sky_top_tex("",512,512);
//runway
Image run_tex("",512,512);
//generate textures
GLuint *textures = new GLuint[8];
//handle menu
void HandleSelectedMenuItem (int id)
{
	switch(id)
	{
	case 1: 
		door2 = true;
		break;
	case 2: 
		//toggle the red light
		if(lig_diffuse[0] == 1.0)
		{
			lig_diffuse[0] = 0.0;
			lig_ambient[0] = 0.0;
		}
		else
		{
			lig_diffuse[0] = 1.0;
			lig_ambient[0] = 0.2;
		}
		break;
	case 3: 
		//toggle green
		if(lig_diffuse[1] == 1.0)
		{
			lig_diffuse[1] = 0;
			lig_ambient[1] = 0;
		}
		else
		{
			lig_diffuse[1] = 1;
			lig_ambient[1] = 0.2;
		}
		break;
	case 4: 
		//toggle blue
		if(lig_diffuse[2] == 1.0)
		{
			lig_diffuse[2] = 0;
			lig_ambient[2] = 0;
		}
		else
		{
			lig_diffuse[2] = 1;
			lig_ambient[2] = 0.2;
		}
		break;
	case 5:
		//toggle between light0 and light1
		if(spot)
		{
			spot = false;
			glEnable(GL_LIGHT1);
			glDisable(GL_LIGHT0);
		}
		else
		{
			spot = true;
			glEnable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
		}
		break;
	}
	// Almost any menu selection requires a redraw
	glutPostRedisplay();
}
//menu
void Menu()
{
	//set the menu handler
	int menu = glutCreateMenu(HandleSelectedMenuItem);
	//set the menu items
	glutAddMenuEntry("Hangar door",1);
	glutAddMenuEntry("Red",2);
	glutAddMenuEntry("Green",3);
	glutAddMenuEntry("Blue",4);
	glutAddMenuEntry("Light type",5);
	//bind the menu to the right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//draw text
void ShowText2D(const char *MyText, int x, int y, float Red, float Green, float Blue, void *font)
{

	// Switch to 2D Ortho. projection
	glMatrixMode(GL_PROJECTION);
	//save the current martix
	glPushMatrix();	
	//load the identity matrix
	glLoadIdentity();	
	//switch to ortho
	gluOrtho2D(0.0,400.0,0.0,300.0);	

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();				
	glLoadIdentity();			
	glDisable(GL_LIGHTING);
	// Draw
	glColor3f(Red,Green,Blue);
	glRasterPos2i(x,y);
	while(*MyText)
	{
		glutBitmapCharacter(font, *MyText);
		++MyText;   // increment pointer to next char in the string.
	}

	// End of Drawing
	glEnable(GL_LIGHTING);
	glPopMatrix();				// restore ModelView

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();				// Restore Projection Matrix

	glMatrixMode(GL_MODELVIEW);	

}
//helicopter
void heli()
{
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	//left skid
	GLUquadricObj *quadratic = gluNewQuadric();
	glPushMatrix();
	glTranslatef(-1,0,0);
	gluCylinder(quadratic,0.1,0.1,2.5,10,10);
	glPushMatrix();
	glTranslatef(0,0.25,-0.35);
	glRotatef(30,1,0,0);
	gluCylinder(quadratic,0.1,0.1,0.5,10,10);
	glPopMatrix();
	glTranslatef(0,0,0.7);
	glPushMatrix();
	glRotatef(-90,1,0,0);
	gluCylinder(quadratic,0.06,0.06,0.5,10,10);
	glPopMatrix();
	glTranslatef(0,0,1.2);
	glPushMatrix();
	glRotatef(-90,1,0,0);
	gluCylinder(quadratic,0.06,0.06,0.5,10,10);
	glPopMatrix();
	glPopMatrix();
	//right skid
	glPushMatrix();
	glTranslatef(1,0,0);
	gluCylinder(quadratic,0.1,0.1,2.5,10,10);
	glPushMatrix();
	glTranslatef(0,0.25,-0.35);
	glRotatef(30,1,0,0);
	gluCylinder(quadratic,0.1,0.1,0.5,10,10);
	glPopMatrix();
	glTranslatef(0,0,0.7);
	glPushMatrix();
	glRotatef(-90,1,0,0);
	gluCylinder(quadratic,0.06,0.06,0.5,10,10);
	glPopMatrix();
	glTranslatef(0,0,1.2);
	glPushMatrix();
	glRotatef(-90,1,0,0);
	gluCylinder(quadratic,0.06,0.06,0.5,10,10);
	glPopMatrix();
	glPopMatrix();
	//front rail
	glPushMatrix();
	glTranslatef(-1.05,0.4,0.7);
	glRotatef(90,0,1,0);
	gluCylinder(quadratic,0.06,0.06,2.09,10,10);
	glPopMatrix();
	//back rail
	glPushMatrix();
	glTranslatef(-1.05,0.4,1.9);
	glRotatef(90,0,1,0);
	gluCylinder(quadratic,0.06,0.06,2.09,10,10);
	glPopMatrix();
	//body
	glPushMatrix();
	glColor3f(0,0,1);
	glTranslatef(0,1.1,1.4);
	glScalef(1.5,1.5,1.5);
	glutSolidCube(1);
	glPopMatrix();
	//cockpit
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(0,1.5,0.5);
	glutSolidSphere(1.2,20,20);
	glPopMatrix();
	//tail
	glPushMatrix();
	glTranslatef(0,1.6,4);
	glScalef(1,1,10);
	glutSolidCube(0.4);
	glPopMatrix();
	//tail prop
	glPushMatrix();
	glColor3f(0.9,0.9,0.9);
	glTranslatef(0,1.6,6);
	glutSolidCube(0.5);

	glTranslatef(-0.2,0,0);
	glRotatef(planespin*20,1,0,0);
	glScalef(0.1,4,0.1);
	glutSolidCube(0.5);
	glPopMatrix();
	//propeller
	glPushMatrix();
	glTranslatef(0,3,1.6);
	glRotatef(planespin*20,0,1,0);
	glScalef(5,0.01,0.3);
	glutSolidCube(1);
	glPopMatrix();
	//propshaft
	glPushMatrix();
	glTranslatef(0,3,1.6);
	glRotatef(90,1,0,0);
	gluCylinder(quadratic,0.06,0.6,2.09,10,10);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
}
//plane
void plane()
{
	//this allows me to use glcolor to set the color of an object so i dont need to use materials

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//wing right
	glPushMatrix();
	glTranslatef(-2,0,0);
	glScalef(2,1,1);
	//load the verticies
	glNormalPointer(GL_FLOAT,0,wing_normals);
	glVertexPointer(3,GL_FLOAT,0,wing_vertices);
	glMaterialf(GL_FRONT, GL_AMBIENT, (1,0,0,1));
	glMaterialf(GL_FRONT, GL_DIFFUSE, (1,0,0,1));
	glColor3f(1,0,0);
	glDrawArrays(GL_TRIANGLES,0,18);
	glPopMatrix();
	//body
	glPushMatrix();
	glScalef(3,0.5,0.5);
	glutSolidSphere(1,50,50);
	glPopMatrix();
	//cockpit
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(-1,0.2,0);
	glScalef(1,0.5,0.5);
	glutSolidSphere(1,50,50);
	glPopMatrix();
	//engine
	glPushMatrix();
	glTranslatef(3,0,0);
	glRotatef(-90,0,1,0);
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);


	glutSolidCone(0.3,1,50,50);
	glPopMatrix();
	//jet blast
	glPushMatrix();
	glTranslatef(3.5,0,0);
	glRotatef(-90,0,1,0);
	glEnable(GL_BLEND);    
	//glDisable(GL_DEPTH_TEST);
	glColor4f(0.1,0,1,0.7);
	glutSolidCone(0.3,1,20,20);
	glTranslatef(0,0,-0.2);
	glColor4f(1,0,0.1,0.7);
	glutSolidCone(0.3,1,20,20);
	glDisable(GL_BLEND);    
	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
	//glEnable(GL_DEPTH_TEST);
	glPopMatrix();

	glColor3f(1,1,1);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

}
//button
void button()
{
	//this allows me to use glcolor to set the color of an object so i dont need to use materials
	//glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	//load the verticies
	glNormalPointer(GL_FLOAT,0,button_normals);
	glVertexPointer(3,GL_FLOAT,0,button_verticies);

	glDrawArrays(GL_TRIANGLES,0,36);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
}
//hangar
void hangar()
{
	//this allows me to use glcolor to set the color of an object so i dont need to use materials
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	//door
	glPushMatrix();
	glColor3f(1,0,0);
	glTranslatef(0.3,0.5,0);
	glScalef(0.4,1,0.1);

	glPushMatrix();
	glTranslatef(-0.5+doors,0,0);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5-doors,0,0);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();
	//load the verticies
	glNormalPointer(GL_FLOAT,0,hangar_normals);
	glVertexPointer(3,GL_FLOAT,0,hangar_verticies);

	glDrawArrays(GL_TRIANGLES,0,48);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
}
//axis
void axis()
{
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(100,0,0);
	glVertex3f(0,0,0);
	glVertex3f(0,100,0);
	glVertex3f(0,0,0);
	glVertex3f(0,0,100);
	glEnd();
}
//create the terrain
//calculate verticies
void calc_vert()
{
	for(int i = 0;i<terRow;i++){
		for(int j = 0;j<terCol;j++)
		{
			cube_vertices[i*terCol + j][0] = i;
			float ytot = 0, y = 0;
			for(int k = 0; k<5;k++)
			{
				//get the absolute value of the difference from the vertex to the center in the mountain struct, add some randomness to the height and the position
				y = (-(abs(mountains[k].terPeakz-(i+(rand()%random_amount)/200)) + abs(mountains[k].terPeakx-(j+(rand()%random_amount)/200))))+mountains[k].terPeaky+(rand()%random_amount)/5000;

				if(y<0)
				{
					y = 0;
				}
				ytot += y;
			}
			cube_vertices[i*terCol + j][1] = ytot;
			cube_vertices[i*terCol + j][2] = j;
		}
	}
}
//calculate normals
void calc_normals()
{
	//cycle through each vertex and calculate normals
	for(int i = 0;i<terRow;i++)
	{
		for(int j = 0;j<terCol;j++)
		{
			//use different points to calculate normals depending on the position in the grid
			if(j!=terCol && i!=terRow)
			{
				int u[3] = {cube_vertices[i*terCol+j+1][0]-cube_vertices[i*terCol+j][0],cube_vertices[i*terCol+j+1][1]-cube_vertices[i*terCol+j][1],cube_vertices[i*terCol+j+1][2]-cube_vertices[i*terCol+j][2]};
				int v[3] = {cube_vertices[(i+1)*terCol+j][0]-cube_vertices[i*terCol+j][0],cube_vertices[(i+1)*terCol+j][1]-cube_vertices[i*terCol+j][1],cube_vertices[(i+1)*terCol+j][2]-cube_vertices[i*terCol+j][2]};
				cube_normal[i*terCol + j][0] = (u[1]*v[2])-(u[2]*v[1]);
				abs(cube_normal[i*terCol + j][1] = (u[2]*v[0])-(u[0]*v[2]));
				cube_normal[i*terCol + j][2] = (u[0]*v[1])-(u[1]*v[0]);
			}
			else
			{
				if(j== terCol)
				{
					int u[3] = {cube_vertices[i*terCol+j-1][0]-cube_vertices[i*terCol+j][0],cube_vertices[i*terCol+j-1][1]-cube_vertices[i*terCol+j][1],cube_vertices[i*terCol+j-1][2]-cube_vertices[i*terCol+j][2]};
					int v[3] = {cube_vertices[(i+1)*terCol+j][0]-cube_vertices[i*terCol+j][0],cube_vertices[(i+1)*terCol+j][1]-cube_vertices[i*terCol+j][1],cube_vertices[(i+1)*terCol+j][2]-cube_vertices[i*terCol+j][2]};
					cube_normal[i*terCol + j][0] = (u[1]*v[2])-(u[2]*v[1]);
					cube_normal[i*terCol + j][1] = (u[2]*v[0])-(u[0]*v[2]);
					cube_normal[i*terCol + j][2] = (u[0]*v[1])-(u[1]*v[0]);
				}else if(i==terRow)
				{
					int u[3] = {cube_vertices[i*terCol+j+1][0]-cube_vertices[i*terCol+j][0],cube_vertices[i*terCol+j+1][1]-cube_vertices[i*terCol+j][1],cube_vertices[i*terCol+j+1][2]-cube_vertices[i*terCol+j][2]};
					int v[3] = {cube_vertices[(i-1)*terCol+j][0]-cube_vertices[i*terCol+j][0],cube_vertices[(i-1)*terCol+j][1]-cube_vertices[i*terCol+j][1],cube_vertices[(i-1)*terCol+j][2]-cube_vertices[i*terCol+j][2]};
					cube_normal[i*terCol + j][0] = (u[1]*v[2])-(u[2]*v[1]);
					cube_normal[i*terCol + j][1] = (u[2]*v[0])-(u[0]*v[2]);
					cube_normal[i*terCol + j][2] = (u[0]*v[1])-(u[1]*v[0]);
				}else if(i==terRow&&j==terCol)
				{
					int u[3] = {cube_vertices[i*terCol+j-1][0]-cube_vertices[i*terCol+j][0],cube_vertices[i*terCol+j-1][1]-cube_vertices[i*terCol+j][1],cube_vertices[i*terCol+j-1][2]-cube_vertices[i*terCol+j][2]};
					int v[3] = {cube_vertices[(i-1)*terCol+j][0]-cube_vertices[i*terCol+j][0],cube_vertices[(i-1)*terCol+j][1]-cube_vertices[i*terCol+j][1],cube_vertices[(i-1)*terCol+j][2]-cube_vertices[i*terCol+j][2]};
					cube_normal[i*terCol + j][0] = (u[1]*v[2])-(u[2]*v[1]);
					cube_normal[i*terCol + j][1] = (u[2]*v[0])-(u[0]*v[2]);
					cube_normal[i*terCol + j][2] = (u[0]*v[1])-(u[1]*v[0]);
				}
			}
		}
	}
}
//calculate the indicies
void calc_indicies()
{
	int count = 0;
	for(int i = 1;i<terRow;i++)
	{
		for(int j = 1;j<terCol;j++)
		{
			//       3
			//       |
			//       |
			//  2----1
			//
			//  6----4
			//  |
			//  |
			//  5
			//
			indices[count] = i*terCol +j;
			count++;
			indices[count] = (i-1)*terCol +j;
			count++;
			indices[count] = i*terCol +(j-1);
			count++;
			indices[count] = (i)*terCol +(j-1);
			count++;
			indices[count] = (i-1)*terCol +j;
			count++;
			indices[count] = (i-1)*terCol +(j-1);
			count++;

		}
	}
}
//calculate the texels
void calc_texels()
{
	int count = 0;
	for(int i = 1;i<terRow;i++)
	{
		for(int j = 1;j<terCol;j++)
		{
			texels[count][0] = 1;
			texels[count][1] = 1;
			count++;

			texels[count][0] = 0;
			texels[count][1] = 0;
			count++;

			texels[count][0] = 0;
			texels[count][1] = 1;
			count++;
		}
	}
}
//calculate fps
void calcfps()
{
	frame_count++;
	current_time = glutGet(GLUT_ELAPSED_TIME);
	int interval = current_time-previous_time;
	//when 100 milliseconds pass, calculate the fps
	if(interval > 100)
	{
		fps = frame_count / (interval/100.0f);
		previous_time = current_time;
		frame_count = 0;
	}
}
//terrain
void draw_skybox()
{
	//disable all the lighting and depth and then draw the sides of the skybox
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	//right
	GLfloat sky_right[4][3] = {{camx+1, camy+1, camz+1},{camx+1,camy+1,camz-1},{camx+1,camy-1,camz-1},{camx+1,camy-1,camz+1}};
	glBindTexture(GL_TEXTURE_2D, *(textures+5));
	glVertexPointer(3,GL_FLOAT,0,sky_right);
	glTexCoordPointer(2,GL_FLOAT,0,texel_right);
	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,indicies_sky);
	//left
	glBindTexture(GL_TEXTURE_2D, *(textures+1));
	GLfloat sky_left[4][3] = {{camx-1, camy+1, camz+1},{camx-1,camy+1,camz-1},{camx-1,camy-1,camz-1},{camx-1,camy-1,camz+1}};
	glVertexPointer(3,GL_FLOAT,0,sky_left);
	glTexCoordPointer(2,GL_FLOAT,0,texel_left);
	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,indicies_sky);
	//top
	glBindTexture(GL_TEXTURE_2D, *(textures+2));
	GLfloat sky_top[4][3] = {{camx+1, camy+1, camz+1},{camx+1,camy+1,camz-1},{camx-1,camy+1,camz-1},{camx-1,camy+1,camz+1}};
	glVertexPointer(3,GL_FLOAT,0,sky_top);
	glTexCoordPointer(2,GL_FLOAT,0,texel_top);
	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,indicies_sky);

	//bottom
	glVertex3f(camx+1,camy-1,camz+1);
	glVertex3f(camx+1,camy-1,camz-1);
	glVertex3f(camx-1,camy-1,camz-1);
	glVertex3f(camx-1,camy-1,camz+1);
	glEnd();
	//back
	glBindTexture(GL_TEXTURE_2D, *(textures+3));
	GLfloat sky_back[4][3] = {{camx+1, camy-1, camz+1},{camx+1,camy+1,camz+1},{camx-1,camy+1,camz+1},{camx-1,camy-1,camz+1}};
	glVertexPointer(3,GL_FLOAT,0,sky_back);
	glTexCoordPointer(2,GL_FLOAT,0,texel_back);
	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,indicies_sky);
	//front
	glBindTexture(GL_TEXTURE_2D, *(textures+4));
	GLfloat sky_front[4][3] = {{camx+1, camy-1, camz-1},{camx+1,camy+1,camz-1},{camx-1,camy+1,camz-1},{camx-1,camy-1,camz-1}};
	glVertexPointer(3,GL_FLOAT,0,sky_front);
	glTexCoordPointer(2,GL_FLOAT,0,texel_front);
	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,indicies_sky);

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
//draw normals
void drawNormals()
{
	//draw lines that show the direction of all the normals
	for(int i = 0; i<terRow*terCol;i++)
	{
		glBegin(GL_LINES);
		glVertex3f(cube_vertices[i][0],cube_vertices[i][1],cube_vertices[i][2]);
		glVertex3f(cube_vertices[i][0] + cube_normal[i][0],cube_vertices[i][1] + cube_normal[i][1],cube_vertices[i][2] + cube_normal[i][2]);
		glEnd();
	}
}
//recursive planes
void draw_formation(int iterations, float startx, float startz, float length)
{
	//draw a plane
	glPushMatrix();
	glTranslatef(startx,0,startz);
	plane();
	//if the there are more iterations then draw 2 more behind it
	glPopMatrix();
	if(iterations != 0)
	{
		draw_formation(iterations-1, startx+length,startz-length, length/2);
		draw_formation(iterations-1, startx+length,startz+length, length/2);
	}
}
//set the texture properties
void tex_stuff()
{
	//set the texture perameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
}
//initialise opengl
void init(void)
{
	//load the textures
	tex =  *loadBMP("tex_grass3.bmp");
	sky_back_tex =  *loadBMP("plain_sky_back.bmp");
	sky_front_tex =  *loadBMP("plain_sky_front.bmp");
	sky_left_tex =  *loadBMP("plain_sky_left.bmp");
	sky_right_tex =  *loadBMP("plain_sky_right.bmp");
	sky_top_tex =  *loadBMP("plain_sky_top.bmp");
	run_tex = *loadBMP("runway.bmp");
	//bind the textures
	glGenTextures(8, textures);
	tex_stuff();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE, sky_right_tex.pixels);
	glBindTexture(GL_TEXTURE_2D, *(textures+1));
	tex_stuff();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE, sky_left_tex.pixels);
	glBindTexture(GL_TEXTURE_2D, *(textures+2));
	tex_stuff();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE, sky_top_tex.pixels);
	glBindTexture(GL_TEXTURE_2D, *(textures+3));
	tex_stuff();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE, sky_back_tex.pixels);
	glBindTexture(GL_TEXTURE_2D, *(textures+4));
	tex_stuff();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE, sky_front_tex.pixels);
	glBindTexture(GL_TEXTURE_2D, *(textures+5));
	tex_stuff();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE, sky_right_tex.pixels);
	glBindTexture(GL_TEXTURE_2D, *(textures+6));
	tex_stuff();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,64,64,0,GL_RGB,GL_UNSIGNED_BYTE, tex.pixels);
	glBindTexture(GL_TEXTURE_2D, *(textures+7));
	tex_stuff();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE, run_tex.pixels);
	glBindTexture(GL_TEXTURE_2D, *(textures+7));
	//initialize the mountains

#pragma region mountains
	mountains[0].terPeakx = 10;
	mountains[0].terPeakz = 20;
	mountains[0].terPeaky = 30;
	mountains[0].peakRand = 50;
	mountains[0].peakSteap = 0;

	mountains[1].terPeakx = 50;
	mountains[1].terPeakz = 20;
	mountains[1].terPeaky = 20;
	mountains[1].peakRand = 50;
	mountains[1].peakSteap = 0;

	mountains[2].terPeakx = 90;
	mountains[2].terPeakz = 20;
	mountains[2].terPeaky = 30;
	mountains[2].peakRand = 50;
	mountains[2].peakSteap = 0;

	mountains[3].terPeakx = 140;
	mountains[3].terPeakz = 20;
	mountains[3].terPeaky = 40;
	mountains[3].peakRand = 50;
	mountains[3].peakSteap = 0;

	mountains[4].terPeakx = 200;
	mountains[4].terPeakz = 20;
	mountains[4].terPeaky = 40;
	mountains[4].peakRand = 50;
	mountains[4].peakSteap = 0;
#pragma endregion

	//printf("\n----- init() Started \t");
	glClearColor(0,0,0,1);	// Background color (RGB)- Black
	glColor3f(1.0,1.0,1.0);	// Drawing color (RGB) - White
	glLineWidth(3);			// Width of the drawing line
	glShadeModel (GL_SMOOTH); //shade model smooth
	//projection mode
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(60, 1, .1, 800); 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);	
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	calc_vert();

	calc_normals();

	calc_indicies();

	calc_texels();

	glEnable(GL_LIGHT1);

}
//display the fps
void drawFPS()
{
	//  Print the FPS to the window
	printf ("FPS: %4.2f \n", fps*10);
}

//callback functions, display for drawing
void display()
{
	//set the lights and the materials
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lig_diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lig_ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lig_specular);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	//light1 spotlight
	GLfloat spot_pos[] = {camx,camy,camz,1};
	GLfloat spot_direction[] = {0,0,-1};

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, lig_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lig_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lig_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, spot_pos);	

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular2[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat mat_shininess[] = { 20.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);	// Set Specular Component of Material
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialf(GL_FRONT, GL_EMISSION, (0,0,0,0));

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//set the matrix mode
	glMatrixMode(GL_MODELVIEW); 

	glLoadIdentity(); 

	//camera positioning
	glRotatef(xrot,1.0,0.0,0.0);  //rotate in the x 
	glRotatef(yrot,0.0,1.0,0.0);  //rotate in the y 
	glTranslated(-camx,-camy,-camz); //translate the screen

	//Billboarding Tutorial [online] Available from: http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat

	//array to hold the model
	float modelview[16];
	int i,j;
	// save the current modelview matrix
	glPushMatrix();
	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	// undo all rotations and scaling
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spot_direction);
	// restore the modelview matrix
	glPopMatrix();
	//skybox
	draw_skybox();
	//draw terrain
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,64,64,0,GL_RGB,GL_UNSIGNED_BYTE, tex.pixels);
	//pointers for the terrain
	glNormalPointer(GL_FLOAT,0,cube_normal);
	glVertexPointer(3,GL_FLOAT,0,cube_vertices);
	//glTexCoordPointer(2,GL_FLOAT,0,texels);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
	//load grass texture
	glBindTexture(GL_TEXTURE_2D, *(textures+6));
	glDrawElements(primative,(terRow-1)*(terCol-1)*6,GL_UNSIGNED_INT,indices);
	//runway
	glBindTexture(GL_TEXTURE_2D, *(textures+7));
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(70,0.01,0);
	glTexCoord2f(10,0);
	glVertex3f(70,0.01,200);
	glTexCoord2f(10,1);
	glVertex3f(80,0.01,200);
	glTexCoord2f(0,1);
	glVertex3f(80,0.01,0);
	glEnd();
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//draw normals
	if(norm)
	{
		drawNormals();
	}
	//button
	glPushMatrix();
	glTranslatef(60,0,65);
	glPushMatrix();
	glTranslatef(0.1,0.7,-0.1);
	glutSolidCube(0.1);
	glPopMatrix();
	glScalef(0.2,0.3,0.2);
	glRotatef(90,0,1,0);
	button();
	glPopMatrix();
	//hangar
	glPushMatrix();
	glTranslatef(81,0,70);
	glRotatef(90,0,1,0);
	glScalef(4,4,4);
	hangar();
	glTranslatef(2,0,0);
	hangar();
	glTranslatef(2,0,0);
	hangar();
	glPopMatrix();
	//draw the axis
	//axis();
	//helicopter
	glPushMatrix();
	glTranslatef(50,0,50);
	heli();
	glPopMatrix();
	//planes! :D
	glPushMatrix();
	planespin-= 1/ (fps/5);
	glTranslatef(50,60,50);
	glRotatef(planespin,0,1,0);
	glTranslatef(-60,0,0);
	glRotatef(-90,0,1,0);
	glRotatef(-90,1,0,0);
	draw_formation(iterate,0,0,20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(60,1,70);
	plane();
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	//drawing text
	//create the char arrays for the hud
	char* fps_text = new char[9];
	*(fps_text) = 'F';
	*(fps_text+1) = 'P';
	*(fps_text+2) = 'S';
	*(fps_text+3) = ':';
	*(fps_text+4) = ' ';
	*(fps_text+5) = '0'+((int)fps/10);
	*(fps_text+6) = '0'+((int)fps)%10;
	*(fps_text+7) = '0'+(int)(fps*10)%10;
	ShowText2D(fps_text,10,260,0,1,1,g_pFont);
	delete []fps_text;
	fps_text = new char[13];
	*(fps_text) = 'I';
	*(fps_text+1) = 't';
	*(fps_text+2) = 'e';
	*(fps_text+3) = 'r';
	*(fps_text+4) = 'a';
	*(fps_text+5) = 't';
	*(fps_text+6) = 'i';
	*(fps_text+7) = 'n';
	*(fps_text+8) = 's';
	*(fps_text+9) = ':';
	*(fps_text+10) = ' ';
	*(fps_text+11) = '0' + iterate/10;
	*(fps_text+12) = '0' + iterate%10;
	ShowText2D(fps_text,10,245,0,1,1,g_pFont);
	glColor3f(1,1,1);
	delete []fps_text;
	fps_text = new char[13];
	*(fps_text) = 'R';
	*(fps_text+1) = 'a';
	*(fps_text+2) = 'n';
	*(fps_text+3) = 'd';
	*(fps_text+4) = 'o';
	*(fps_text+5) = 'm';
	*(fps_text+6) = ':';
	*(fps_text+7) = ' ';
	*(fps_text+8) = '0' + random_amount/100;
	*(fps_text+9) = '0' + (random_amount/10) %10;
	*(fps_text+10) = '0' ;
	ShowText2D(fps_text,10,230,0,1,1,g_pFont);
	glColor3f(1,1,1);
	delete []fps_text;
	drawFPS();
	glClearDepth(1);
	glFlush();	// Finish the drawing
}
//do while idle
void idle()
{
	//calculate the fps
	calcfps();
	//if the w key is down
	if(w)
	{
		//the rotation = the yrotation in radians
		yrotrad = ((yrot-90) / 180 * 3.141592654f);
		//cos of the rotation gives the amount of speed in the x axis
		camx += float(cos(yrotrad)) * camspeed / (fps/10);
		//sin of the rotation gives the amount of speed in the z axis
		camz += float(sin(yrotrad)) * camspeed / (fps/10);
	}
	if(s)
	{
		yrotrad = ((yrot+90) / 180 * 3.141592654f);
		camx += float(cos(yrotrad)) * camspeed / (fps/10);
		camz += float(sin(yrotrad)) * camspeed / (fps/10);
	}
	if(a)
	{
		yrotrad = (yrot / 180 * 3.141592654f);
		camx -= float(cos(yrotrad)) * camspeed / (fps/10);
		camz -= float(sin(yrotrad)) * camspeed / (fps/10);
	}
	if(d)
	{
		yrotrad = (yrot / 180 * 3.141592654f);
		camx += float(cos(yrotrad)) * camspeed / (fps/10);
		camz += float(sin(yrotrad)) * camspeed / (fps/10);
	}
	//open or close the hangar doors
	if(door2)
	{
		if(door)
		{
			doors+= 0.01;
			if(doors > 0.5)
			{
				door = false;
				door2 = false;
			}
		}
		if(!door)
		{
			doors-= 0.01;
			if(doors < -0.5)
			{
				door = true;
				door2 = false;
			}
		}
	}
	glutPostRedisplay();
}
//detect for when a key is up
void mykeyup(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'w':
		w = false;
		break;	
	case 's':
		s = false;
		break;
	case 'a':
		a = false;
		break;
	case 'd':
		d = false;
		break;

	}
}
//detect when a key is down
void mykeydown(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'w':
		w = true;
		break;	
	case 's':
		s = true;
		break;
	case 'a':
		a = true;
		break;
	case 'd':
		d = true;
		break;
	case 'q':
		primative ++;
		if(primative>9)
		{
			primative = 0;
		}
		break;
	case 'e':
		if(norm)
		{
			norm = false;
		}
		else
		{
			norm = true;
		}

		break;
	case 'x':
		iterate ++;
		break;
	case 'z':
		if(iterate >0)
		{
			iterate--;
		}
		break;

	case 'v':
		random_amount+=10;
		calc_vert();
		calc_normals();
		break;
	case 'c':
		random_amount-=10;
		calc_vert();
		calc_normals();
		break;
	case 27:
		delete []textures;
		exit(0);
		break;
	}
}
//detect the special keys
void specialKey(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		yrotrad = ((yrot-90) / 180 * 3.141592654f);
		camx += float(cos(yrotrad)) * camspeed;
		camz += float(sin(yrotrad)) * camspeed;
		break;	
	case GLUT_KEY_DOWN:
		yrotrad = ((yrot+90) / 180 * 3.141592654f);
		camx += float(cos(yrotrad)) * camspeed;
		camz += float(sin(yrotrad)) * camspeed;
		break;
	case GLUT_KEY_LEFT:

		yrotrad = (yrot / 180 * 3.141592654f);
		camx -= float(cos(yrotrad)) * camspeed;
		camz -= float(sin(yrotrad)) * camspeed;
		break;
	case GLUT_KEY_RIGHT:

		yrotrad = (yrot / 180 * 3.141592654f);
		camx += float(cos(yrotrad)) * camspeed;
		camz += float(sin(yrotrad)) * camspeed;
		break;
	}

	glutPostRedisplay();
}
//detect mouse clicks
void mymouse(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;
} 
//detect mouse movement
void mousePassive(int x, int y) {
	//calculate the diffeence between the previous x & y and the new ones
	int diffx=(x-prevx)/2;
	int diffy=(y-prevy)/2; 
	//check if the difference id too large
	if(diffx>10||diffx<-50)
	{
		diffx = 0;
	}
	if(diffy>10|| diffy<-50)
	{
		diffy = 0;
	}
	//update the previous x and y
	prevx=x;
	prevy=y; 
	//convert to float and then add the difference to the rotation
	xrot += (float) diffy;
	yrot += (float) diffx;
}
//redraw the screen
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);	// Viewport mapping
	// Projection transformation
	glMatrixMode(GL_PROJECTION); 	// Set matrix mode	
	glLoadIdentity(); 			// Clear
	gluPerspective(60, (float)w/h, .1, 800); 	// define and adjust projection
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 0.1, 100.0); 
	// ** Change the near gradually (add 0.5 each time) and observe the cube.
	glMatrixMode(GL_MODELVIEW);		// reset the matrix mode
}
//main
int main(int argc, char** argv) 
{
	glutInit(&argc, argv);	// GLUT Initialization 
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE); // Initializing the Display mode
	glutInitWindowSize(600,600);	// Define the window size
	glutCreateWindow("Cthulhu zeus");	// Create the window, with caption.
	init();	// All OpenGL initialization
	Menu();
	//Callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardUpFunc(mykeyup);
	glutKeyboardFunc(mykeydown);
	glutSpecialFunc(specialKey);
	glutMouseFunc(mymouse);
	glutPassiveMotionFunc(mousePassive);
	glutMainLoop();	// Loop waiting for event 
}

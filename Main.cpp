#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <GL/glut.h>
#include "input_module.h"
#include "color.h"
#include "camera.h"
#include "ray_tracer.h"
#include "bitmap_image.hpp"
using namespace std;

#define BLACK 0, 0, 0

bool canDrawGrid;
bool first = true;

double walkUnit = 5;
double rotateUnit = 5;

Camera camera;

int recDepth;
int pixels;
RayTracer rayTracer;
vector<Shape *> shapes;
vector<string> shapeTypes;
vector<Point> lights;
vector<vector<Color> > pixelMatrix;

void draw_checkerboard(Checkerboard *checkerboard)
{
	int &c = checkerboard->c, &dimension = checkerboard->dimension;

	for(int i = -c/2; i<=c/2; ++i)
	{
		bool colorIdx = !(i&1);
		for(int j = -c/2; j<=c/2; ++j)
		{
			glColor3f(checkerboard->color[colorIdx].red, checkerboard->color[colorIdx].green, checkerboard->color[colorIdx].blue);
			colorIdx = !colorIdx;

			glBegin(GL_POLYGON);
			glVertex3f(i*dimension, 0, j*dimension);
			glVertex3f(i*dimension+dimension, 0, j*dimension);
			glVertex3f(i*dimension+dimension, 0, j*dimension+dimension);
			glVertex3f(i*dimension, 0, j*dimension+dimension);
			glEnd();
		}
	}
}

void draw_sphere(Sphere *sphere)
{
	glColor3f(sphere->color.red, sphere->color.green, sphere->color.blue);

	glPushMatrix();
	{
		glTranslatef(sphere->center.x, sphere->center.y, sphere->center.z);
		glutSolidSphere(sphere->radius, 32, 32);
	}
	glPopMatrix();
}

void draw_cylinder(Cylinder *cylinder)
{
	glColor3f(cylinder->color.red, cylinder->color.green, cylinder->color.blue);

	glPushMatrix();
	{
		glTranslatef(cylinder->xCenter, cylinder->yMin, cylinder->zCenter);
		glScalef(1, 0, 1);
		glutSolidSphere(cylinder->radius, 32, 32);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(cylinder->xCenter, cylinder->yMax, cylinder->zCenter);
		glScalef(1, 0, 1);
		glutSolidSphere(cylinder->radius, 32, 32);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(cylinder->xCenter, cylinder->yMin, cylinder->zCenter);
		glRotatef(-90, 1, 0, 0);
		GLUquadric *q = gluNewQuadric();
		gluCylinder(q, cylinder->radius, cylinder->radius, cylinder->yMax-cylinder->yMin, 32, 32);
		gluDeleteQuadric(q);
	}
	glPopMatrix();
}

void draw_triangle(Triangle *triangle)
{
	glColor3f(triangle->color.red, triangle->color.green, triangle->color.blue);

	glBegin(GL_POLYGON);
	glVertex3f(triangle->a.x, triangle->a.y, triangle->a.z);
	glVertex3f(triangle->b.x, triangle->b.y, triangle->b.z);
	glVertex3f(triangle->c.x, triangle->c.y, triangle->c.z);
	glEnd();
}

void draw_light_source(Point &light)
{
	glColor3f(1, 1, 1);

	glPushMatrix();
	{
		glTranslatef(light.x, light.y, light.z);
		glutSolidCube(1);
	}
	glPopMatrix();
}

void draw_world()
{
	for(int i = 0; i<shapes.size(); ++i)
		if(shapeTypes[i]=="CHECKERBOARD")
			draw_checkerboard((Checkerboard *)shapes[i]);
		else if(shapeTypes[i]=="SPHERE")
			draw_sphere((Sphere *)shapes[i]);
		else if(shapeTypes[i]=="CYLINDER")
			draw_cylinder((Cylinder *)shapes[i]);
		else if(shapeTypes[i]=="TRIANGLE")
			draw_triangle((Triangle *)shapes[i]);

	for(int i = 0; i<lights.size(); ++i)
		draw_light_source(lights[i]);
}

void generate_plain_image()
{
    cout << "generating output_plain.bmp\n";

    rayTracer.render_initial_image(pixels, pixelMatrix, camera);

    bitmap_image image(pixels, pixels);

    for(int i = 0; i<pixels; ++i)
        for(int j = 0; j<pixels; ++j)
            image.set_pixel(i, j, 255*pixelMatrix[i][j].red, 255*pixelMatrix[i][j].green,
                            255*pixelMatrix[i][j].blue);

    image.save_image("output_plain.bmp");

    cout << "Done!\n";
}

void generate_detailed_image()
{
    cout << "generating output_detailed.bmp\n";

    rayTracer.trace_rays(pixels, recDepth, pixelMatrix, camera);

    bitmap_image image(pixels, pixels);

    for(int i = 0; i<pixels; ++i)
        for(int j = 0; j<pixels; ++j)
            image.set_pixel(i, j, 255*pixelMatrix[i][j].red, 255*pixelMatrix[i][j].green,
                            255*pixelMatrix[i][j].blue);

    image.save_image("output_detailed.bmp");

    cout << "Done!\n";
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(BLACK, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera is looking?
	//3. Which direction is the camera's UP direction?

	gluLookAt(camera.position.p.x, camera.position.p.y, camera.position.p.z,
				camera.lookAt.p.x, camera.lookAt.p.y, camera.lookAt.p.z,
				camera.up.p.x, camera.up.p.y, camera.up.p.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	draw_world();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	//codes for any changes in Camera

	//codes for any changes in Models

	//MISSING SOMETHING? -- YES: add the following
	glutPostRedisplay();	//this will call the display AGAIN
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			camera.yaw(-rotateUnit);
			break;

		case '2':
			camera.yaw(rotateUnit);
			break;

		case '3':
			camera.pitch(-rotateUnit);
			break;

		case '4':
			camera.pitch(rotateUnit);
			break;

		case '5':
			camera.roll(-rotateUnit);
			break;

		case '6':
			camera.roll(rotateUnit);
			break;

		case '8':	//toggle grids
			canDrawGrid = !canDrawGrid;
			break;

		case 27:	//ESCAPE KEY -- simply exit
			exit(0);
			break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:
			camera.walk(walkUnit);
			break;
		case GLUT_KEY_UP:
			camera.walk(-walkUnit);
			break;

		case GLUT_KEY_RIGHT:
			camera.strafe(walkUnit);
			break;
		case GLUT_KEY_LEFT:
			camera.strafe(-walkUnit);
			break;

		case GLUT_KEY_PAGE_UP:
			camera.fly(walkUnit);
			break;
		case GLUT_KEY_PAGE_DOWN:
			camera.fly(-walkUnit);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				generate_plain_image();
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				generate_detailed_image();
			}
			break;
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void init(){
	//codes for initialization
	canDrawGrid = true;

	//clear the screen
	glClearColor(BLACK, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(70,	1,	0.1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

	input(recDepth, pixels, shapes, shapeTypes, lights);

	pixelMatrix.resize(pixels);
	for(int i = 0; i<pixels; ++i)
        pixelMatrix[i].resize(pixels);

    rayTracer = RayTracer(shapes, lights);
}

int main(int argc, char **argv){
	freopen("Spec.txt", "r", stdin);

	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	//ADD keyboard listeners:
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	//ADD mouse listeners:
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

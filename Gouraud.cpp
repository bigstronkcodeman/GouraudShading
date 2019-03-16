#define _USE_MATH_DEFINES

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <vector>
#include <cmath>
#include <chrono>
#include <iostream>
#include <random>
#include "RectangleMesh.h"
#define SIZE 100

using namespace std;

enum MODE { TRANSLATE, ROTATE };

struct Light
{
	Vertex3D pos;
	float rgb[3];

	Light(Vertex3D position)
	{
		pos = position;
		rgb[0] = 0;
		rgb[1] = 1;
		rgb[2] = 0;
	}

	Light(Vertex3D position, float r, float g, float b)
	{
		pos = position;
		rgb[0] = r;
		rgb[1] = g;
		rgb[2] = b;
	}

	void translate(int mx, int my, int mz)
	{
		pos.translate(mx, my, mz);
	}
};

RectangleMesh mesh(SIZE, SIZE, 1000, 1000);
double vertsRGB[SIZE][SIZE][3];

Light light1(Vertex3D(0, 500, 0), 1, 0, 1);
Light light2(Vertex3D(500, 500, 0), 0, 1, 1);

bool displayNormals = false;
int controlFocus = 0;
MODE mode = ROTATE;

double Mr = 1.0;
double Mg = 1.0;
double Mb = 1.0;

double dotProduct(Vertex3D v1, Vertex3D v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

void calcVertexColors()
{
	for (int i = 0; i < mesh.getRowCount(); i++)
	{
		for (int j = 0; j < mesh.getColCount(); j++)
		{
			Vertex3D currentVert = mesh.getVertex(i, j);
			double vertMag = sqrt((currentVert.x * currentVert.x) + (currentVert.y * currentVert.y) + (currentVert.z * currentVert.z));
			Vertex3D betweenVec(light1.pos.x - currentVert.x, light1.pos.y - currentVert.y, light1.pos.z - currentVert.z);
			double betweenMag = sqrt((betweenVec.x * betweenVec.x) + (betweenVec.y * betweenVec.y) + (betweenVec.z * betweenVec.z));
			double lightAmt = dotProduct(betweenVec, mesh.getNormal(i, j)) * (1.0 / (0 + 0 * betweenMag + 0.25 * (betweenMag * betweenMag)));
			lightAmt *= 50;
			vertsRGB[i][j][0] = lightAmt * Mr * light1.rgb[0];
			vertsRGB[i][j][1] = lightAmt * Mg * light1.rgb[1];
			vertsRGB[i][j][2] = lightAmt * Mb * light1.rgb[2];
		}
	}


	for (int i = 0; i < mesh.getRowCount(); i++)
	{
		for (int j = 0; j < mesh.getColCount(); j++)
		{
			Vertex3D currentVert = mesh.getVertex(i, j);
			double vertMag = sqrt((currentVert.x * currentVert.x) + (currentVert.y * currentVert.y) + (currentVert.z * currentVert.z));
			Vertex3D betweenVec(light2.pos.x - currentVert.x, light2.pos.y - currentVert.y, light2.pos.z - currentVert.z);
			double betweenMag = sqrt((betweenVec.x * betweenVec.x) + (betweenVec.y * betweenVec.y) + (betweenVec.z * betweenVec.z));
			double lightAmt = dotProduct(betweenVec, mesh.getNormal(i, j)) * (1.0 / (0 + 0 * betweenMag + 0.25 * (betweenMag * betweenMag)));
			lightAmt *= 50;
			vertsRGB[i][j][0] = vertsRGB[i][j][0] + lightAmt * light2.rgb[0];
			vertsRGB[i][j][1] = vertsRGB[i][j][1] + lightAmt * light2.rgb[1];
			vertsRGB[i][j][2] = vertsRGB[i][j][2] + lightAmt * light2.rgb[2];
		}
	}
}

void initColors(double r, double g, double b)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			vertsRGB[i][j][0] = r;
			vertsRGB[i][j][1] = g;
			vertsRGB[i][j][2] = b;
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor3f(light1.rgb[0], light1.rgb[1], light1.rgb[2]);
	glBegin(GL_POLYGON);
	glVertex3f(light1.pos.x, light1.pos.y, light1.pos.z);
	glVertex3f(light1.pos.x + 25, light1.pos.y, light1.pos.z);
	glVertex3f(light1.pos.x + 25, light1.pos.y - 25, light1.pos.z);
	glVertex3f(light1.pos.x, light1.pos.y - 25, light1.pos.z);
	glEnd();
	glColor3f(light2.rgb[0], light2.rgb[1], light2.rgb[2]);
	glBegin(GL_POLYGON);
	glVertex3f(light2.pos.x, light2.pos.y, light2.pos.z);
	glVertex3f(light2.pos.x + 25, light2.pos.y, light2.pos.z);
	glVertex3f(light2.pos.x + 25, light2.pos.y - 25, light2.pos.z);
	glVertex3f(light2.pos.x, light2.pos.y - 25, light2.pos.z);
	glEnd();

	for (int i = 0; i < mesh.getRowCount() - 1; i++)
	{
		for (int j = 0; j < mesh.getColCount() - 1; j++)
		{
			Vertex3D v1 = mesh.getVertex(i, j);
			Vertex3D v2 = mesh.getVertex(i, j + 1);
			Vertex3D v3 = mesh.getVertex(i + 1, j + 1);
			Vertex3D v4 = mesh.getVertex(i + 1, j);
			//glPolygonMode(GL_FRONT, GL_LINE);
			glBegin(GL_POLYGON);
			glColor3f(vertsRGB[i][j][0], vertsRGB[i][j][1], vertsRGB[i][j][2]);
			glVertex3f(v1.x, v1.y, v1.z);
			glColor3f(vertsRGB[i][j + 1][0], vertsRGB[i][j + 1][1], vertsRGB[i][j + 1][2]);
			glVertex3f(v2.x, v2.y, v2.z);
			glColor3f(vertsRGB[i + 1][j + 1][0], vertsRGB[i + 1][j + 1][1], vertsRGB[i + 1][j + 1][2]);
			glVertex3f(v3.x, v3.y, v3.z);
			glEnd();
			glBegin(GL_POLYGON);
			glColor3f(vertsRGB[i][j][0], vertsRGB[i][j][1], vertsRGB[i][j][2]);
			glVertex3f(v1.x, v1.y, v1.z);
			glColor3f(vertsRGB[i + 1][j][0], vertsRGB[i + 1][j][1], vertsRGB[i + 1][j][2]);
			glVertex3f(v4.x, v4.y, v4.z);
			glColor3f(vertsRGB[i + 1][j + 1][0], vertsRGB[i + 1][j + 1][1], vertsRGB[i + 1][j + 1][2]);
			glVertex3f(v3.x, v3.y, v3.z);
			glEnd();
		}
	}

	if (displayNormals)
	{
		float length = 50;
		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_LINES);
		for (int i = 0; i < mesh.getRowCount(); i++)
		{
			for (int j = 0; j < mesh.getColCount(); j++)
			{
				Vertex3D v = mesh.getVertex(i, j);
				Vertex3D n = mesh.getNormal(i, j);
				glVertex3f(v.x, v.y, v.z);
				glVertex3f(v.x + length * n.x, v.y + length * n.y, v.z + length * n.z);
			}
		}
		glEnd();
	}
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '0':
		controlFocus = 0;
		break;
	case '1':
		controlFocus = 1;
		break;
	case '2':
		controlFocus = 2;
		break;
	case 'X':
		switch (controlFocus)
		{
		case 0:
			switch (mode)
			{
			case ROTATE:
				mesh.rotateAboutX(2);
				break;
			case TRANSLATE:
				mesh.translate(5, 0, 0);
				break;
			}
			break;
		case 1:
			light1.translate(5, 0, 0);
			break;
		case 2:
			light2.translate(5, 0, 0);
			break;
		}
		break;
	case 'x':
		switch (controlFocus)
		{
		case 0:
			switch (mode)
			{
			case ROTATE:
				mesh.rotateAboutX(-2);
				break;
			case TRANSLATE:
				mesh.translate(-5, 0, 0);
				break;
			}
			break;
		case 1:
			light1.translate(-5, 0, 0);
			break;
		case 2:
			light2.translate(-5, 0, 0);
			break;
		}
		break;
	case 'Y':
		switch (controlFocus)
		{
		case 0:
			switch (mode)
			{
			case ROTATE:
				mesh.rotateAboutY(2);
				break;
			case TRANSLATE:
				mesh.translate(0, 5, 0);
				break;
			}
			break;
		case 1:
			light1.translate(0, 5, 0);
			break;
		case 2:
			light2.translate(0, 5, 0);
			break;
		}
		break;
	case 'y':
		switch (controlFocus)
		{
		case 0:
			switch (mode)
			{
			case ROTATE:
				mesh.rotateAboutY(-2);
				break;
			case TRANSLATE:
				mesh.translate(0, -5, 0);
				break;
			}
			break;
		case 1:
			light1.translate(0, -5, 0);
			break;
		case 2:
			light2.translate(0, -5, 0);
			break;
		}
		break;
	case 'Z':
		switch (controlFocus)
		{
		case 0:
			switch (mode)
			{
			case ROTATE:
				mesh.rotateAboutZ(2);
				break;
			case TRANSLATE:
				mesh.translate(0, 0, 5);
				break;
			}
			break;
		case 1:
			light1.translate(0, 0, 5);
			break;
		case 2:
			light2.translate(0, 0, 5);
			break;
		}
		break;
	case 'z':
		switch (controlFocus)
		{
		case 0:
			switch (mode)
			{
			case ROTATE:
				mesh.rotateAboutZ(-2);
				break;
			case TRANSLATE:
				mesh.translate(0, 0, -5);
				break;
			}
			break;
		case 1:
			light1.translate(0, 0, -5);
			break;
		case 2:
			light2.translate(0, 0, -5);
			break;
		}
		break;
	case 'q':
	{
		RectangleMesh newMesh = RectangleMesh(SIZE, SIZE, 1000, 1000);
		mesh = newMesh;
		mesh.noisify();
		Vertex3D translateBy = mesh.getVertex(mesh.getRowCount() / 2, mesh.getColCount() / 2);
		mesh.translate(-translateBy.x, -translateBy.y, -translateBy.z);
		mesh.initNormals();
		break;
	}
	case 'p':
		displayNormals = !displayNormals;
		break;
	case 'r':
		switch (controlFocus)
		{
		case 1:
			light1.rgb[0] -= 0.1;
			if (light1.rgb[0] < 0) light1.rgb[0] = 0;
			break;
		case 2:
			light2.rgb[0] -= 0.1;
			if (light2.rgb[0] < 0) light2.rgb[0] = 0;
			break;
		}
		break;
	case 'R':
		switch (controlFocus)
		{
		case 1:
			light1.rgb[0] += 0.1;
			if (light1.rgb[0] > 1) light1.rgb[0] = 1;
			break;
		case 2:
			light2.rgb[0] += 0.1;
			if (light2.rgb[0] > 1) light2.rgb[0] = 1;
			break;
		}
		break;
	case 'g':
		switch (controlFocus)
		{
		case 1:
			light1.rgb[1] -= 0.1;
			if (light1.rgb[1] < 0) light1.rgb[1] = 0;
			break;
		case 2:
			light2.rgb[1] -= 0.1;
			if (light2.rgb[1] < 0) light2.rgb[1] = 0;
			break;
		}
		break;
	case 'G':
		switch (controlFocus)
		{
		case 1:
			light1.rgb[1] += 0.1;
			if (light1.rgb[1] > 1) light1.rgb[1] = 1;
			break;
		case 2:
			light2.rgb[1] += 0.1;
			if (light2.rgb[1] > 1) light2.rgb[1] = 1;
			break;
		}
		break;
	case 'b':
		switch (controlFocus)
		{
		case 1:
			light1.rgb[2] -= 0.1;
			if (light1.rgb[2] < 0) light1.rgb[2] = 0;
			break;
		case 2:
			light2.rgb[2] -= 0.1;
			if (light2.rgb[2] < 0) light2.rgb[2] = 0;
			break;
		}
		break;
	case 'B':
		switch (controlFocus)
		{
		case 1:
			light1.rgb[2] += 0.1;
			if (light1.rgb[2] > 1) light1.rgb[2] = 1;
			break;
		case 2:
			light2.rgb[2] += 0.1;
			if (light2.rgb[2] > 1) light2.rgb[2] = 1;
			break;
		}
		break;
	case 'T':
		mode = TRANSLATE;
		break;
	case 'U':
		mode = ROTATE;
		break;
	}
	calcVertexColors();
	glutPostRedisplay();
}

void init() 
{
	srand(time(NULL));

	calcVertexColors();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1000.0, 1000.0, -1000.0, 1000.0, -1000.0, 1000.0);
	glEnable(GL_DEPTH_TEST);

	cout << "Controls:\n"
		<< "U => Rotate Mode\n"
		<< "T => Translate Mode\n"
		<< "0 => give mesh the focus\n"
		<< "1 => give light 1 the focus\n"
		<< "2 => give light 2 the focus\n"
		<< "p => display mesh vertex normals\n"
		<< "x, y, z => translate or rotate in negative direction\n"
		<< "X, Y, Z => translate or rotate in positive direction\n"
		<< "r, g, b => decrease r, g, or b of focused light respectively\n"
		<< "R, G, B => increase r, g, or b of focused light respectively\n";

	mesh.noisify();
	Vertex3D translateBy = mesh.getVertex(mesh.getRowCount() / 2, mesh.getColCount() / 2);
	mesh.translate(-translateBy.x, -translateBy.y, -translateBy.z);
	mesh.initNormals();
	initColors(0.5, 0.46, 0.75);
	calcVertexColors();
}

int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 200);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutCreateWindow("Shading");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();
}

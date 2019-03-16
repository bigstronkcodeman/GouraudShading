#pragma once
#define _USE_MATH_DEFINES
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <random>

#define DEFAULT_ROWS 10
#define DEFAULT_COLS 10
#define DEFAULT_WIDTH 10
#define DEFAULT_HEIGHT 10

using namespace std;

//simple struct to represent a 3d vertex
struct Vertex3D
{
	double x;
	double y;
	double z;

	//default ctor, initialize point to origin
	Vertex3D() {
		x = 0;
		y = 0;
		z = 0;
	}

	//3 int ctor, initialize point to specified location
	Vertex3D(double inX, double inY, double inZ) {
		x = inX;
		y = inY;
		z = inZ;
	}

	//copy ctor
	Vertex3D(const Vertex3D& copyMe) {
		x = copyMe.x;
		y = copyMe.y;
		z = copyMe.z;
	}

	//assignment operator redefinition
	void operator=(const Vertex3D& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	Vertex3D operator -(const Vertex3D& other)
	{
		return Vertex3D(x - other.x, y - other.y, z - other.z);
	}

	Vertex3D operator +(const Vertex3D& other)
	{
		return Vertex3D(x + other.x, y + other.y, z + other.z);
	}

	bool operator ==(const Vertex3D& other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator !=(const Vertex3D& other)
	{
		return x != other.x || y != other.y || z != other.z;
	}

	//scale point by any real number
	void scale(double scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	//set point to any x, y, z position
	void translate(double xm, double ym, double zm)
	{
		x += xm;
		y += ym;
		z += zm;
	}

	void print()
	{
		cout << "x: " << x << "\ny: " << y << "\nz: " << z << endl;
	}
};

class RectangleMesh
{
private:
	int rowCount;
	int colCount;
	int width;
	int height;
	Vertex3D** verts;
	Vertex3D** normals;
	int numVerts;

public:
	RectangleMesh();
	RectangleMesh(int numRows, int numCols, int width, int height);
	~RectangleMesh();

	void generateMesh();
	Vertex3D getVertex(int i, int j);
	void setVertex(int i, int j, Vertex3D coord);
	Vertex3D getNormal(int i, int j);
	int getRowCount();
	int getColCount();
	void noisify();
	void translate(double mx, double my, double mz);
	void rotateAboutX(double theta);
	void rotateAboutY(double theta);
	void rotateAboutZ(double theta);
	void initNormals();
	void operator=(const RectangleMesh& otherMesh);
};


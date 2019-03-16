#include "RectangleMesh.h"



RectangleMesh::RectangleMesh()
{
	rowCount = DEFAULT_ROWS;
	colCount = DEFAULT_COLS;
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;

	verts = new Vertex3D*[rowCount];
	normals = new Vertex3D*[rowCount];
	for (int i = 0; i < rowCount; i++)
	{
		verts[i] = new Vertex3D[colCount];
		normals[i] = new Vertex3D[colCount];
	}

	numVerts = rowCount * colCount;
	generateMesh();
}

RectangleMesh::RectangleMesh(int numRows, int numCols, int w, int h)
{
	rowCount = numRows + 0;
	colCount = numCols + 0;
	width = w;
	height = h;

	verts = new Vertex3D*[rowCount];
	normals = new Vertex3D*[rowCount];
	for (int i = 0; i < rowCount; i++)
	{
		verts[i] = new Vertex3D[colCount];
		normals[i] = new Vertex3D[colCount];
	}

	numVerts = rowCount * colCount;
	generateMesh();
}


RectangleMesh::~RectangleMesh()
{
	for (int i = 0; i < rowCount; i++)
	{
		delete[] verts[i], normals[i];
	}
	delete[] verts, normals;
}


void RectangleMesh::generateMesh()
{
	double xInterval = (double)width / colCount;
	double zInterval = (double)height / rowCount;

	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			verts[i][j] = Vertex3D(j * xInterval, 0, i * zInterval);
		}
	}
}

int RectangleMesh::getRowCount()
{
	return rowCount;
}

int RectangleMesh::getColCount()
{
	return colCount;
}

Vertex3D RectangleMesh::getVertex(int i, int j)
{
	if (i < rowCount && j < colCount)
	{
		return verts[i][j];
	}
}

void RectangleMesh::setVertex(int i, int j, Vertex3D coord)
{
	if (i < rowCount && j < colCount && i >= 0 && j >= 0)
	{
		verts[i][j] = coord;
	}
}

Vertex3D RectangleMesh::getNormal(int i, int j)
{
	if (i < rowCount && j < colCount && i >= 0 && j >= 0)
	{
		return normals[i][j];
	}
}

void RectangleMesh::noisify()
{
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			verts[i][j].translate(0, rand() % 55, 0);
		}
	}

	double* newYs = new double[rowCount * colCount];
	int index = 0;
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			int sum = verts[i][j].y;
			if (i - 1 >= 0)
			{
				sum += verts[i - 1][j].y;
				if (j - 1 >= 0)
				{
					sum += verts[i][j - 1].y;
					sum += verts[i - 1][j - 1].y;
				}
				if (j + 1 < colCount)
				{
					sum += verts[i][j + 1].y;
					sum += verts[i - 1][j + 1].y;
				}
			}
			if (i + 1 < rowCount)
			{
				sum += verts[i + 1][j].y;
				if (j - 1 >= 0)
				{
					sum += verts[i + 1][j - 1].y;
				}
				if (j + 1 < colCount)
				{
					sum += verts[i + 1][j + 1].y;
				}
			}
			sum /= 9;
			newYs[index] = sum;
			index++;
		}
	}

	index = 0;
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			verts[i][j].y = newYs[index];
			index++;
		}
	}
	delete[] newYs;
}

void RectangleMesh::translate(double mx, double my, double mz)
{
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			verts[i][j].translate(mx, my, mz);
		}
	}
}

void RectangleMesh::rotateAboutX(double theta)
{
	theta = theta * (M_PI / 180);
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			double savey = verts[i][j].y;
			verts[i][j].y = verts[i][j].y * cos(theta) - verts[i][j].z * sin(theta);
			verts[i][j].z = verts[i][j].z * cos(theta) + savey * sin(theta);

			double saveyn = normals[i][j].y;
			normals[i][j].y = normals[i][j].y * cos(theta) - normals[i][j].z * sin(theta);
			normals[i][j].z = normals[i][j].z * cos(theta) + saveyn * sin(theta);
		}
	}
}

void RectangleMesh::rotateAboutY(double theta)
{
	theta = theta * (M_PI / 180);
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			double savex = verts[i][j].x;
			verts[i][j].x = verts[i][j].x * cos(theta) - verts[i][j].z * sin(theta);
			verts[i][j].z = verts[i][j].z * cos(theta) + savex * sin(theta);

			double savexn = normals[i][j].x;
			normals[i][j].x = normals[i][j].x * cos(theta) - normals[i][j].z * sin(theta);
			normals[i][j].z = normals[i][j].z * cos(theta) + savexn * sin(theta);
		}
	}
}

void RectangleMesh::rotateAboutZ(double theta)
{
	theta = theta * (M_PI / 180);
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < rowCount; j++)
		{
			double savex = verts[i][j].x;
			verts[i][j].x = (verts[i][j].x * cos(theta)) - (verts[i][j].y * sin(theta));
			verts[i][j].y = (verts[i][j].y * cos(theta)) + (savex * sin(theta));

			double savenx = normals[i][j].x;
			normals[i][j].x = (normals[i][j].x * cos(theta)) - (normals[i][j].y * sin(theta));
			normals[i][j].y = (normals[i][j].y * cos(theta)) + (savenx * sin(theta));
		}
	}
}

void RectangleMesh::initNormals()
{
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			Vertex3D tangent1, tangent2;
			tangent1.x = (i < rowCount - 1) ? verts[i + 1][j].x - verts[i][j].x : verts[i][j].x - verts[i - 1][j].x;
			tangent1.y = (i < rowCount - 1) ? verts[i + 1][j].y - verts[i][j].y : verts[i][j].y - verts[i - 1][j].y;
			tangent1.z = (i < rowCount - 1) ? verts[i + 1][j].z - verts[i][j].z : verts[i][j].z - verts[i - 1][j].z;
			tangent2.x = (j < colCount - 1) ? verts[i][j + 1].x - verts[i][j].x : verts[i][j].x - verts[i][j - 1].x;
			tangent2.y = (j < colCount - 1) ? verts[i][j + 1].y - verts[i][j].y : verts[i][j].y - verts[i][j - 1].y;
			tangent2.z = (j < colCount - 1) ? verts[i][j + 1].z - verts[i][j].z : verts[i][j].z - verts[i][j - 1].z;

			double t1Length = sqrt(tangent1.x * tangent1.x + tangent1.y * tangent1.y + tangent1.z * tangent1.z);
			double t2Length = sqrt(tangent2.x * tangent2.x + tangent2.y * tangent2.y + tangent2.z * tangent2.z);
			if ((t1Length * t2Length) > 0)
			{
				Vertex3D normal;
				normal.x = (tangent1.y * tangent2.z - tangent1.z * tangent2.y) / (t1Length * t2Length);
				normal.y = (tangent1.z * tangent2.x - tangent1.x * tangent2.z) / (t1Length * t2Length);
				normal.z = (tangent1.x * tangent2.y - tangent1.y * tangent2.x) / (t1Length * t2Length);
				normals[i][j] = normal;
			}
		}
	}
}

void RectangleMesh::operator=(const RectangleMesh& otherMesh)
{
	rowCount = otherMesh.rowCount;
	colCount = otherMesh.colCount;
	width = otherMesh.width;
	height = otherMesh.height;
	numVerts = otherMesh.numVerts;
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			verts[i][j] = otherMesh.verts[i][j];
			normals[i][j] = otherMesh.normals[i][j];
		}
	}
}
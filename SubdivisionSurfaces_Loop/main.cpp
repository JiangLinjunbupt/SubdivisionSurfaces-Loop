#include <string>
#include <iostream>
#include <algorithm>

#include "FileOperation.h"
#include "Display.h"
#include "SubdivisionSurfaces.h"

using namespace std;

void preprocessObjModel(vector<PointType>& vertices, const vector<PatchType>& patches) {
	if (vertices.empty())
		return;
	float minX, maxX, minY, maxY;
	minX = minY = 99999999;
	maxX = maxY = -99999999;
	for (int i = 0; i < vertices.size(); i++)
	{
		const PointType& v = vertices[i];
		minX = min(v.position.x, minX);
		maxX = max(v.position.x, maxX);
		minY = min(v.position.y, minY);
		maxY = max(v.position.y, maxY);
	}

	float lx = -10;
	float rx = 10;
	float ty = 10;
	float by = -10;

	float scaleX = 1;
	float scaleY = 1;
	if (minX != maxX)
		scaleX = (float)(rx - lx) / (maxX - minX);
	if (minY != maxY)
		scaleY = (float)(ty - by) / (maxY - minY);
	float scale = min(scaleX, scaleY);
	float deltaX = lx - minX*scale;
	float deltaY = by - minY*scale;

	for (int i = 0; i < vertices.size(); i++)
	{
		PointType& v = vertices[i];
		v.position *= scale;
		v.position += cv::Point3f(deltaX, deltaY, 0);
		int k=0;
		k++;
	}
}

void initModel() {
	vector<PointType> vertices;
	vector<PatchType> patches;
	string PATH = "";
	do
	{
		cout << "Input obj file path:" << endl;
		cin >> PATH;
	} while (!FO::ReadFile().objFile(PATH, vertices, patches));
	preprocessObjModel(vertices, patches);
	SS::initModel(vertices, patches);
}

int main(int argc, char* argv[]) {
	initModel();
	DS::init(argc, argv);
	DS::start();
	return 0;
}
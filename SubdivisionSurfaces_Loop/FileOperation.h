#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Common.h"
using namespace std;
//using namespace cv;

namespace FO {
	class ReadFile
	{
	public:
		static bool objFile(string path, vector<PointType>& vertices, vector<PatchType>& patches) {
			ifstream file(path);
			if (!file.is_open()) {
				cout << "Error: Can't open file: " << path << endl;
				return false;
			}
			vertices.clear();
			patches.clear();
			string line;
			while (getline(file, line))
			{
				stringstream ss(line);
				char type;
				ss >> type;
				switch (type)
				{
				case 'v':
					float x, y, z, r, g, b;
					ss >> x >> y >> z;
					if (ss.eof())
					{
						r = 1.0; g = 1.0; b = 1.0;
					}
					else
					{
						ss >> r >> g >> b;
					}
					vertices.push_back(PointType(x, y, z, r, g, b));
					break;
				case 'f':
					int v1, v2, v3;
					ss >> v1 >> v2 >> v3;
					patches.push_back(PatchType(v1 - 1, v2 - 1, v3 - 1));
					break;
				case 'g':
					break;
				case '#':
					break;
				default:
					cout << "Error: Unsupported obj element type: " << type << endl;
					break;
				}
			}
			file.close();
			return true;
		}
	};
}

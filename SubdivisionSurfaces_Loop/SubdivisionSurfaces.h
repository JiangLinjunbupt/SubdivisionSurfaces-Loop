#pragma once
#include <vector>
#include <opencv2\opencv.hpp>
#include "Common.h"
#include "GeometryDataStructure.h"
using namespace std;
//using namespace cv;

class SubSurfaces {
public:
	void initModel() {
		modeling(ori_vertices, ori_patches);	
	}
	void initModel(const vector<PointType>& vertices, const vector<PatchType>& patches) {
		ori_vertices.assign(vertices.begin(), vertices.end());
		ori_patches.assign(patches.begin(), patches.end());
		initModel();
	}
	void subdivide() {
		vector<PointType> vertices;
		const int vp_num = vPool.size();
		const int ep_num = fPool.size()*TrianglePatchSize / 2;
		vertices.resize(vp_num + ep_num);
		int actualSize = 0;
		ePool.generateNewEdgePoints(vPool, vertices, actualSize);
#ifdef DEBUG
		if (actualSize != ep_num)
		{
			cout << "Error: actual number of edge points is not consistent with precomputeted one" << endl;
			return;
		}
#endif // DEBUG
		vector<list<int>> vertexNeighbors;
		ePool.getVertexNeighbors(vertexNeighbors);
		vPool.generateNewVertexPoints(vertexNeighbors, vertices, actualSize);
#ifdef DEBUG
		if (actualSize != vertices.size())
		{
			cout << "Error: actual number of new points is not consistent with precomputeted one" << endl;
			return;
		}
#endif // DEBUG
		vertices.resize(actualSize);
		vector<PatchType> patches;
		fPool.generateNewPatches(vPool, ePool, patches);
		modeling(vertices, patches);
	}
	void generateDisplayModel(vector<PointType>& vertices, vector<PatchType>& patches) {
		vPool.getVertices(vertices);
		fPool.getPatches(patches);
		for (vector<PatchType>::iterator it = patches.begin(); it != patches.end(); it++)
		{
			cv::Vec3f va = vertices[it->v_idx[1]].position - vertices[it->v_idx[0]].position;
			cv::Vec3f vb = vertices[it->v_idx[2]].position - vertices[it->v_idx[1]].position;
			it->normal = cv::normalize(va.cross(vb));
#ifdef DEBUG
			if (it->normal.dot(it->normal) < 0.001)
			{
				cout << "Error: normal of one patch is zero" << endl;
				return;
			}
#endif // DEBUG
		}
	}
private:
	vector<PointType> ori_vertices;
	vector<PatchType> ori_patches;
	VertexPool vPool;
	EdgePool ePool;
	FacePool fPool;
	void modeling(const vector<PointType>& vertices, const vector<PatchType>& patches) {
		vPool.init(vertices.size());
		for (int i = 0; i < vertices.size(); i++)
			vPool.insert(vertices[i]);
		ePool.init(vertices.size());
		fPool.init(patches.size());
		for (vector<PatchType>::const_iterator it = patches.begin(); it != patches.end(); it++)
		{
			fPool.insert(*it);
			ePool.insert(it->v_idx[0], it->v_idx[1], it->v_idx[2]);
			ePool.insert(it->v_idx[1], it->v_idx[2], it->v_idx[0]);
			ePool.insert(it->v_idx[2], it->v_idx[0], it->v_idx[1]);
		}
		if (!ePool.checkIntegrity())
		{
			cout << "Error: the model is not a closed model, please change another one!" << endl;
			return;
		}
	}
};


static SubSurfaces ssModel;
static int ssLevel;

namespace SS
{
	enum SubLevel
	{
		INCREASE,
		DECREASE
	};
	vector<PointType> disVertices;
	vector<PatchType> disPatches;

	void initModel(const vector<PointType>& vertices, const vector<PatchType>& patches) {
		ssModel.initModel(vertices, patches);
		ssModel.generateDisplayModel(disVertices, disPatches);
		ssLevel = 0;
		cout << "Initializatin:" << endl;
		cout << "Number of vertices: " << disVertices.size() << endl;
		cout << "Number of patches: " << disPatches.size() << endl;
		cout << endl;
	}
	void subdivide(SubLevel level) {
		int64 time_start = cv::getTickCount();
		switch (level)
		{
		case INCREASE:
			ssLevel++;
			ssModel.subdivide();
			break;
		case DECREASE:
			if (ssLevel == 0)
				break;
			ssLevel--;
			ssModel.initModel();
			for (int i = 0; i < ssLevel; i++)
				ssModel.subdivide();
			break;
		default:
			break;
		}
		int64 time_end = cv::getTickCount();
		ssModel.generateDisplayModel(disVertices, disPatches);	
		cout << "Subdivision Level: " << ssLevel << endl;
		cout<<"Subdivision Time: "<< (time_end - time_start) / cv::getTickFrequency() << " s" << endl;
		cout << "Number of vertices: " << disVertices.size() << endl;
		cout << "Number of patches: " << disPatches.size() << endl;
		cout << endl;
	}
}


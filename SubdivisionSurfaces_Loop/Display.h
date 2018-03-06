#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
//#include <stdlib.h>
#include <algorithm>
#include <opencv2\opencv.hpp>
#include "SubdivisionSurfaces.h"

using namespace std;
//using namespace cv;

// 屏幕宽高宏定义
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

namespace DS {
	enum DisplayMode
	{
		MESH,
		MESH_FRAME,
		WIRE_FRAME,
	};

	float red = 0.4, blue = 0.8, green = 0.3;
	
	struct {
		float X = 0;
		float Y = 0;
		float Z = 0;
		float delta = 0;
	} angle;

	float ratio;
	int deltaMove = 0;

	struct {
		cv::Point3f eye;
		cv::Point3f center;
		cv::Point3f up;
	}cameraPos;

	DisplayMode displayMode;
	int specialKey = 0;

	void disableLight() {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}

	void enableLight() {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	void light() {
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		// 定义太阳光源，它是一种白色的光源  
		GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat sun_light_ambient[] = { 0.25f, 0.25f, 0.15f, 1.0f };
		GLfloat sun_light_diffuse[] = { 0.7f, 0.7f, 0.55f, 1.0f };
		GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position); //指定第0号光源的位置   
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient); //GL_AMBIENT表示各种光线照射到该材质上，  
															 //经过很多次反射后最终遗留在环境中的光线强度（颜色）  
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse); //漫反射后~~  
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);//镜面反射后~~~  

		glEnable(GL_LIGHT0); //使用第0号光照   
	}
	
	void initScene() {
		glEnable(GL_DEPTH_TEST);
		cameraPos.eye = cv::Point3f(0.0f, 1.0f, 35.0f);
		cameraPos.center = cv::Point3f(0, 0.2f, 0);
		cameraPos.up = cv::Point3f(0, 1.0f, 0);
		displayMode = DS::MESH;
		light();
	}

/*
	void orientMe(float ang) {
		//lx = sin(ang);
		//lz = -cos(ang);
		//glLoadIdentity();
		//gluLookAt(x, y, z,
		//	x + lx, y + ly, z + lz,
		//	0.0f, 1.0f, 0.0f);
	}
*/

	void moveMeFlat(bool isHorizontal, int step) {
		cv::Point3f lookAt = cameraPos.center - cameraPos.eye;
		cv::Point3f direction;
		if (isHorizontal)
		{
			direction = lookAt.cross(cameraPos.up);
		}
		else
		{
			direction = lookAt;
		}
		cameraPos.eye += direction*step*0.08;
		cameraPos.center += direction*step*0.08;

		glLoadIdentity();
		gluLookAt(cameraPos.eye.x, cameraPos.eye.y, cameraPos.eye.z,
			cameraPos.center.x, cameraPos.center.y, cameraPos.center.z,
			cameraPos.up.x, cameraPos.up.y, cameraPos.up.z);
	}

/*
	//顶点的回调函数  
	void CALLBACK vertexCallback(GLvoid* vertex)
	{
		DSVertex* dsv;
		dsv = (DSVertex*)vertex;
		glColor3f(red, blue, green);
		glNormal3f(dsv->norm[0],dsv->norm[1], dsv->norm[2]);
		glVertex3d(dsv->pos[0], dsv->pos[1], dsv->pos[2]);
	}

	void CALLBACK beginCallback(GLenum type)
	{
		glBegin(type);
	}

	void CALLBACK endCallback()
	{
		glEnd();
	}

	void CALLBACK errorCallback(GLenum errorCode)
	{
		const GLubyte * estring;
		//打印错误类型  
		estring = gluErrorString(errorCode);
		fprintf(stderr, "The input data is wrong, maybe loops of one face intersect.  Tessellation Error: %s/n", estring);
		int k;
		cin >> k;
		exit(0);
	}

	void triangulatePolygonWithHole(const DSFace& dsf) {
		GLUtesselator * tessobj;
		tessobj = gluNewTess();
		//注册回调函数  
		gluTessCallback(tessobj, GLU_TESS_VERTEX, (void(__stdcall *)(void))vertexCallback);
		gluTessCallback(tessobj, GLU_TESS_BEGIN, (void(__stdcall *)(void))beginCallback);
		gluTessCallback(tessobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
		gluTessCallback(tessobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
		gluTessBeginPolygon(tessobj, NULL);
		for (int i = 0; i < dsf.size(); i++)
		{
			const DSLoop& dsl = dsf[i];
			gluTessBeginContour(tessobj);//设置多边形的边线
										 //if (i==0)
			for (int j = 0; j < dsl.size(); j++)
			{
				gluTessVertex(tessobj, (GLdouble*)dsl[j].pos, (void*)&dsl[j]);
			}
			gluTessEndContour(tessobj);
		}
		gluTessEndPolygon(tessobj);
		gluDeleteTess(tessobj);
	}
*/

	void renderWireFrame() {
		glColor3f(0.7, 0.45, 0.45);
		for (vector<PatchType>::const_iterator it = SS::disPatches.begin(); it != SS::disPatches.end(); it++)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < TrianglePatchSize; i++)
			{
				const PointType& v = SS::disVertices[it->v_idx[i]];
				glVertex3f(v.position.x, v.position.y, v.position.z);
			}
			glEnd();
		}
	}

	void renderMesh() {
		glColor3f(red, green, blue);
		glBegin(GL_TRIANGLES);
		for (vector<PatchType>::const_iterator it = SS::disPatches.begin(); it != SS::disPatches.end(); it++)
		{
			const cv::Point3f& norm = it->normal;
			glNormal3f(norm.x, norm.y, norm.z);
			for (int i = 0; i < TrianglePatchSize; i++)
			{
				const PointType& v = SS::disVertices[it->v_idx[i]];
				glVertex3f(v.position.x, v.position.y, v.position.z);
			}
		}
		glEnd();
	}

	void renderScene() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glRotatef(angle.Y, 0.0, 1.0, 0.0);
		glRotatef(angle.Z, 0.0, 0.0, 1.0);
		glRotatef(angle.X, 1.0, 0.0, 0.0);
		switch (displayMode)
		{
		case WIRE_FRAME:
			disableLight();
			renderWireFrame();
			break;
		case MESH:
			enableLight();
			renderMesh();
			break;
		case MESH_FRAME:
			enableLight();
			renderMesh();
			renderWireFrame();
			break;
		default:
			break;
		}
		glPopMatrix();
		glutSwapBuffers();
	}

	void changeSize(int w, int h) {
		// 防止被0除.
		if (h == 0)
			h = 1;

		ratio = 1.0f * w / h;
		// Reset the coordinate system before modifying
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//设置视口为整个窗口大小
		glViewport(0, 0, w, h);

		//设置可视空间
		gluPerspective(45, ratio, 1, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraPos.eye.x, cameraPos.eye.y, cameraPos.eye.z,
			cameraPos.center.x, cameraPos.center.y, cameraPos.center.z,
			cameraPos.up.x, cameraPos.up.y, cameraPos.up.z);
	}

	void pressSpecialKey(int key, int x, int y) {
		specialKey = key;
		switch (key) {
		case GLUT_KEY_LEFT:
			moveMeFlat(true, -1); break;
			//angle -= 0.01f;
			//orientMe(angle); break;
		case GLUT_KEY_RIGHT:
			moveMeFlat(true, 1); break;
			//angle += 0.01f;
			//orientMe(angle); break;
		case GLUT_KEY_UP:
			moveMeFlat(false, 1); break;
		case GLUT_KEY_DOWN:
			moveMeFlat(false,-1); break;
		}
	}

	void pressNormalKey(unsigned char key, int x, int y)
	{
		if (key == 'q')
			exit(0);
		if (key == 's')
		{
			switch (displayMode)
			{
			case MESH:
				displayMode = MESH_FRAME;
				break;
			case MESH_FRAME:
				displayMode = WIRE_FRAME;
				break;
			case WIRE_FRAME:
				displayMode = MESH;
				break;
			default:
				break;
			}
		}
		if (key == 'a')
		{
			SS::subdivide(SS::INCREASE);
		}
		if (key == 'd')
		{
			SS::subdivide(SS::DECREASE);
		}
	}

	void releaseSpecialKey(int key, int x, int y) {
		specialKey = 0;
		switch (key) {
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT:
			angle.delta = 0.0f; break;
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
			deltaMove = 0; break;
		}
	}

	void processMouse(int button, int state, int x, int y) {
		specialKey = glutGetModifiers();
		// 当鼠标键和alt键都被按下
		if ((state == GLUT_DOWN) &&
			(specialKey == GLUT_ACTIVE_ALT)) {

			// set the color to pure red for the left button
			if (button == GLUT_LEFT_BUTTON) {
				red = 1.0; green = 0.0; blue = 0.0;
			}
			// set the color to pure green for the middle button
			else if (button == GLUT_MIDDLE_BUTTON) {
				red = 0.0; green = 1.0; blue = 0.0;
			}
			// set the color to pure blue for the right button
			else {
				red = 0.0; green = 0.0; blue = 1.0;
			}
		}
	}

	void processMouseActiveMotion(int x, int y) {
		// the ALT key was used in the previous function
		if (specialKey != GLUT_ACTIVE_ALT) {
			// setting red to be relative to the mouse 
			// position inside the window
			if (x < 0)
				red = 0.0;
			else if (x > WINDOW_WIDTH)
				red = 1.0;
			else
				red = ((float)x) / WINDOW_HEIGHT;
			// setting green to be relative to the mouse 
			// position inside the window
			if (y < 0)
				green = 0.0;
			else if (y > WINDOW_WIDTH)
				green = 1.0;
			else
				green = ((float)y) / WINDOW_HEIGHT;
			// removing the blue component.
			blue = 0.0;
		}
	}

	void processMouseEntry(int state) {
		if (state == GLUT_LEFT)
			angle.delta = 0.0;
		else
			angle.delta = 1.0;
	}

	void processMousePassiveMotion(int x, int y) {
		static float last_pos_x = 0;
		int specialKey = glutGetModifiers();
		// User must press the SHIFT key to change the 
		// rotation in the X axis
		if (specialKey != GLUT_ACTIVE_SHIFT && specialKey != GLUT_ACTIVE_CTRL) {
			// setting the angle to be relative to the mouse 
			// position inside the window
			if (x < 0)
				angle.X = 0.0;
			else if (x > WINDOW_WIDTH)
				angle.X = 360.0;
			else
				angle.X += 360.0 * ((float)(x-last_pos_x)) / WINDOW_HEIGHT;
		}
		if (specialKey == GLUT_ACTIVE_SHIFT)
		{
			if (x < 0)
				angle.Y = 0.0;
			else if (x > WINDOW_WIDTH)
				angle.Y = 360.0;
			else
				angle.Y += 360.0 * ((float)(x-last_pos_x)) / WINDOW_HEIGHT;
		}
		if (specialKey == GLUT_ACTIVE_CTRL)
		{

		}
		last_pos_x = x;
	}

	void InitializeGlutCallbacks()
	{
		glutDisplayFunc(renderScene);
		glutIdleFunc(renderScene);
		glutReshapeFunc(changeSize);

		//glutIgnoreKeyRepeat(1);
		glutSpecialFunc(pressSpecialKey);
		glutSpecialUpFunc(releaseSpecialKey);
		glutKeyboardFunc(pressNormalKey);

		//adding here the mouse processing callbacks
		//glutMouseFunc(processMouse);
		//glutMotionFunc(processMouseActiveMotion);
		glutPassiveMotionFunc(processMousePassiveMotion);
		//glutEntryFunc(processMouseEntry);

	}

	void init(int argc, char* argv[]) {
		// 初始化GLUT
		glutInit(&argc, argv);

		// 显示模式：双缓冲、RGBA
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

		// 窗口设置
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);    // 窗口尺寸
		glutInitWindowPosition(100, 100);  // 窗口位置
		glutCreateWindow("SubdivisionSurfaces");   // 窗口标题

		//初始化场景
		initScene();

		//回调
		InitializeGlutCallbacks();

		// 检查GLEW是否就绪，必须要在GLUT初始化之后！
		GLenum res = glewInit();
		if (res != GLEW_OK) {
			fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
			exit(1);
		}

		// 缓存清空后的颜色值
		glClearColor(0.0, 0.0f, 0.0f, 0.0f); //RGBA
	}

	void start() {
		// 通知开始GLUT的内部循环
		glutMainLoop();	
	}

}
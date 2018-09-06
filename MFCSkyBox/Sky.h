
#include <GL\glaux.h>
#include <GL\glut.h>
#include "math.h"
#define PI 3.1415926
#pragma once


class CSky
{
	bool Status;//是否加载球纹理
	bool isLoadSun;//是否加载sun纹理
	GLuint textureName;
	GLuint textureSun;
	//GLUquadric *qobj;
public:
	CSky();
	~CSky();

	AUX_RGBImageRec* loadbmp(char *filename);
	bool LoadGLTexture(char *filename);
	int CreatSky(float r, int jing, int wei);
	int CreatSun();
	

};


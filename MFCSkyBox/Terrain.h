#pragma once
#include "gl/glew.h"
#include "gl/glut.h"
#include "gl/glaux.h"
#include "glm/glm.hpp"
#include <glm/vec3.hpp> // glm::vec3
using namespace glm;
class CTerrain
{
	int stepsize;//每两个相邻顶点之间的恒（纵）坐标之差,假设灰度图是正方形
	
	bool isReadRawfile;
	//设为1024的原因：地图宽为1023，说明每一行有1024个点
	//记住，灰度图（高度图）宽为a，那么地图宽为a-1，高同理
	BYTE pheight[1024][1024];

	GLfloat xtexa1, xtexa2, ytexa1, ytexa2;

	vec3 normal[1024][1024];//本例中顶点数为1024*1024，最好用变量代替，使类具有通用通用性

public:
	CTerrain();
	~CTerrain();
	bool ReadRawFile(char* filename);
	BYTE GetHeight(int x, int z);
	AUX_RGBImageRec* LoadBmp(char *filename);
	bool LoadTexture();
	int CreatTerrain();
	void GetNormal(/*float x, float z*/);
	int CreatGrid();

public:
	int mapsize;
	GLuint texture;//绑定纹理用
	bool isLoadTexture;
	bool isCaculateNomal;
};


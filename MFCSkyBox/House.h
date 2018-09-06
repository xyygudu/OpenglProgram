
#include <GL\glaux.h>
#include <GL\glut.h>

#pragma once
class CHouse
{
	struct Head
	{
		char partName[80];//文件名称  
		int  faceNum;//面的数目  
	};
	//法线  
	struct Normal
	{
		float i;
		float j;
		float k;
	};
	//点，三个float类型的，大小为12字节  
	struct Point
	{
		float x;
		float y;
		float z;
	};
	//三角面，由一个法线，三个点，和一个两字节的保留项，一共50字节  
	struct Face
	{
		Normal normal;
		Point  p1;
		Point  p2;
		Point  p3;
		char  info[2];//保留数据，可以不用管  
	};
	Face *faces;
	//与读取stl模型相关变量
	bool isreadstl;
	int faceNum;
	bool isLoadTexture;
	GLuint texture;//绑定纹理用
public:
	CHouse();
	~CHouse();

	bool Readstl(char *filename);
	AUX_RGBImageRec* LoadBmp(char *filename);
	bool LoadGLTexture(char *filename);
	int CreatHouse();
};


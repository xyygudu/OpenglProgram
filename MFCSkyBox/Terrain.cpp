#include "stdafx.h"
#include <windows.h>
#include "Terrain.h"



CTerrain::CTerrain()
{
	isLoadTexture = false;
	isReadRawfile = false;
	isCaculateNomal = false;
}


CTerrain::~CTerrain()
{
}

bool CTerrain::ReadRawFile(char* filename)
{
	FILE *fp;
	if (fopen_s(&fp, "Data/Terrain.raw", "rb") != 0)
	{
		MessageBox(NULL, "高度图文件打开失败!\nCan't open the height map.", "Error", MB_OK);
		return false;//文件打开失败
	}
	fread(pheight, 1, 1024*1024, fp);//灰度图是1024*1024大小
	if (ferror(fp))
		return false;//读取出错
	fclose(fp);
	//free(fp);
	isReadRawfile = true;
}

BYTE CTerrain::GetHeight(int x, int z)//x表示地图横坐标不是灰度图横坐标
{
	if (!isReadRawfile)//地图起始点在（-512，-512）不是原点
		ReadRawFile("data/Terrain.raw");//以后想要获取高度就不用单独调用readrawfile了
	int X = (x-(-512)) / stepsize;
	int Z =(z-(-512)) / stepsize;
	return pheight[X][Z];//1024为高度图的宽
}

AUX_RGBImageRec* CTerrain::LoadBmp(char *filename)
{
	FILE *fp;
	if (!filename)//未给出文件名
		return 0;
	fopen_s(&fp, filename, "r");
	if (fp)//文件打开成功
	{
		fclose(fp);
		return auxDIBImageLoad(filename);
		free(fp);
	}
	return 0;
}

bool CTerrain::LoadTexture()
{
	AUX_RGBImageRec *pimage;
	char *bmpname = { "data/Terrain.bmp" };
	pimage=LoadBmp(bmpname);
	if (pimage->data&&pimage)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, pimage->sizeX, pimage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pimage->data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pimage->sizeX, pimage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pimage->data);
		isLoadTexture = true;
		free(pimage->data);											// Free The Texture Image Memory
		free(pimage);
	}
	return isLoadTexture;
}

int CTerrain::CreatTerrain()
{
	if (!isLoadTexture)
		LoadTexture();//判断是否已经加载纹理，一定要判断，不然每次旋转都执行loadtexture会很卡
	stepsize =1;//1024是灰度图的长和宽，所以尽量用变量代替，以便使类更具有通用性，只是现在还不知道如何获取灰度图长宽
	//stepsize在36行求高度用到
	
	if (isCaculateNomal == false)
		GetNormal();

	int List = glGenLists(1);//创建一个显示列表，为了提高执行效率
	glNewList(List, GL_COMPILE);//开始装入，装入后不立即执行
	glBindTexture(GL_TEXTURE_2D, texture);//此行必须装进显示列表，如果没装进去，在RenderScence中只调用了glcalllist，重绘时
										  //就没有执行此行，即未绑定纹理，绑定的可能就是其他（比如天空的）纹理了
	for (int i = -512; i < 511; i++)//1024是灰度图的宽
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = -512; j < 511; j ++)
		{
			float a=128;//图片重复次数为1024/a；必须是float型
			//I，J必须为整形，因为数组中不能是浮点型
			int I = i + 512, J = j + 512;//地图起始点在（-512，-512）不是原点,纹理坐标要从0开始所以要加512
			xtexa1 = I / a;
			xtexa2 = (I + 1) / a;
			ytexa1 = J / a;
			ytexa2 = (J + 1) / a;
			//注意到底是用大写的还是小写的变量，大写的从0开始，用于法向量数组中
			glNormal3f(normal[I + 1][J].x, normal[I + 1][J].y, normal[I + 1][J].z);
			glTexCoord2f(xtexa2, ytexa1);   glVertex3f(i + 1, GetHeight(i + 1, j), j);

			glNormal3f(normal[I][J].x, normal[I][J].y, normal[I][J].z);
			glTexCoord2f(xtexa1, ytexa1);   glVertex3f(i, GetHeight(i, j), j);
		}
		glEnd();
	}
	glEndList();
	return List;
}


void CTerrain::GetNormal(/*float x, float z*/)//x,z为地图横纵坐标
{
	vec3  va, vb, vc, vd, ve, vf,//中心o到abc...f六个点的向量va，vb...vf
		norm1, norm2, norm3, norm4, norm5;//与中心点相关的5个三角形发向量分别为norm1到6

	int x, z;
	for (x = -511; x < 511; x += stepsize)
	{
		for (z = -511; z < 511; z += stepsize)
		{
			va.x = 0; va.y = GetHeight(x, z + stepsize) - GetHeight(x, z); va.z = stepsize;
			vb.x = -stepsize; vb.y = GetHeight(x - stepsize, z) - GetHeight(x, z); vb.z = 0;
			vc.x = -stepsize; vc.y = GetHeight(x - stepsize, z - stepsize) - GetHeight(x, z); vc.z = -stepsize;
			vd.x = 0; vd.y = GetHeight(x, z - stepsize) - GetHeight(x, z); vd.z = -stepsize;
			ve.x = stepsize; ve.y = GetHeight(x + stepsize, z) - GetHeight(x, z); ve.z = 0;
			vf.x = stepsize; vf.y = GetHeight(x + stepsize, z + stepsize) - GetHeight(x, z); vf.z = stepsize;
			//除边缘外，每个顶点和5个三角形相关，下面计算这5个三角形的法向量
			norm1 = glm::cross(va, vb);    //直接利用叉乘函数即可计算
			norm2 = glm::cross(vb, vc);
			norm3 = glm::cross(vc, vd);
			norm4 = glm::cross(vd, ve);
			norm5 = glm::cross(ve, vf);
			int X = x + 512, Z = z + 512;//地图起始点在（-512，-512）不是原点,数组要从0开始所以要加512
			//X，Z的最小值为1，最大为1023，因为这个范围对应的顶点周围才有六个三角形，即
			//在这里没有专门为边缘顶点设置法向量，边源顶点相关联的三角形没有5个，本程序
			//其他地方也没有为边缘顶点设置法向量，边缘顶点对应的发向量如normal(0,0)没有
			//初始化不知道为啥运行也没出错，可能是系统有默认的初始化
			normal[X][Z].x = -(norm1.x + norm2.x + norm3.x + norm4.x + norm5.x);//不知道为什么要加负号，但是不加是反面才有光照
			normal[X][Z].y = -(norm1.y + norm2.y + norm3.y + norm4.y + norm5.y);
			normal[X][Z].z = -(norm1.z + norm2.z + norm3.z + norm4.z + norm5.z);

			glm::normalize(normal[X][Z]);
		}
	}
	isCaculateNomal = true;
}

int CTerrain::CreatGrid()
{
	int List = glGenLists(1);//创建一个显示列表，为了提高执行效率
	glNewList(List, GL_COMPILE);//开始装入，装入后不立即执行
	for (int i = -512; i < 511; i++)//1024是灰度图的宽
	{
		
		for (int j = -512; j < 511; j++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(i, GetHeight(i, j), j);
			glVertex3f(i + 1, GetHeight(i + 1, j), j);
			glVertex3f(i+1, GetHeight(i+1, j+1), j+1);
			glVertex3f(i, GetHeight(i, j+1), j+1);
			glVertex3f(i + 1, GetHeight(i + 1, j), j);
			glEnd();
		}
		
	}
	glEndList();
	return List;
}
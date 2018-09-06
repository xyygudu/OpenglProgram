#include "stdafx.h"
#include "Sky.h"


CSky::CSky()
{
	Status = false;
	isLoadSun = false;
}


CSky::~CSky()
{
}

bool CSky::LoadGLTexture(char *filename)						// Load Bitmaps And Convert To Textures
{

	AUX_RGBImageRec *pImage;										// Create Storage Space For The Texture

	pImage = loadbmp(filename);									// Loads The Bitmap Specified By filename

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (pImage != NULL && pImage->data != NULL)					// If Texture Image Exists
	{
		glGenTextures(1, &textureName);									// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, textureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);//不能用这个贴球
		free(pImage->data);											// Free The Texture Image Memory
		free(pImage);												// Free The Image Structure
	}
	Status = true;
	isLoadSun = true;
	return Status;
}

AUX_RGBImageRec* CSky::loadbmp(char *Filename)
{
	FILE *File = NULL;												// File Handle

	if (!Filename)													// Make Sure A Filename Was Given
	{
		return NULL;												// If Not Return NULL
	}

	fopen_s(&File, Filename, "r");										// Check To See If The File Exists

	if (File)														// Does The File Exist?
	{
		fclose(File);												// Close The Handle
		return auxDIBImageLoad(Filename);							// Load The Bitmap And Return A Pointer
	}

	return NULL;
}


int CSky::CreatSky(float r, int jing, int wei)//参数初始化在View.cpp的InitializeOpenGL函数中
{
	if (!Status)
		LoadGLTexture("Data/sky.bmp");
	int List = glGenLists(1);//创建一个显示列表，为了提高执行效率
	glNewList(List, GL_COMPILE);//开始装入，装入后不立即执行
	//qobj = gluNewQuadric();
	//gluQuadricDrawStyle(qobj, GLU_FILL);//设置二次却面绘制风格
	//gluQuadricNormals(qobj, GLU_SMOOTH);//设置法线,对每个顶点都计算法线向量，是默认方式
	//gluQuadricTexture(qobj, GL_TRUE);//设置是否自动计算纹理。默认是GLU_FALSE
	glBindTexture(GL_TEXTURE_2D, textureName);

	//glTranslatef(0, -50, 0);
	//glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	//gluSphere(qobj, 900, 40, 40);

	float angle = 360 / jing;
	float height = r / wei;
	for (float h = 0; h < r; h += height)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (float jiao = 0; jiao < 360; jiao += angle)
		{
			float Rxy1 = sqrt(pow(r, 2) - pow(h + height, 2));//俯视图看，先画小圆
			float Rxy = sqrt(pow(r, 2) - pow(h, 2));
			float x1 = Rxy1*cos(jiao); float z1 = Rxy1*sin(jiao); float y1 = h + height;
			float x = Rxy*cos(jiao); float z = Rxy*sin(jiao); float y = h;
			glTexCoord2f(0.5+x1 / (2 * r), 0.5+z1 / (2 * r)); glVertex3f(x1, y1, z1);//0.5+x1 / (2 * r)找到点对应的纹理坐标
			glTexCoord2f(0.5+x / (2 * r), 0.5+z / (2 * r)); glVertex3f(x, y, z);
		}
		glEnd();
	}

	glEndList();
	return List;
}

int CSky::CreatSun()
{
	//if (!isLoadSun)
		LoadGLTexture("data/NEBULA.bmp");
	//glDisable(GL_TEXTURE_2D);
		glColor4f(1, 0.5f, 0, 0.1);
	glBindTexture(GL_TEXTURE_2D, textureName);
	
	glBegin(GL_TRIANGLE_STRIP);	//画太阳					// Build Quad From A Triangle Strip
	glTexCoord2f(1, 1); glVertex4f(2, 2,0,0.5); // Top Right
	glTexCoord2f(0, 1); glVertex4f(-2, 2, 0,0.5); // Top Left
	glTexCoord2f(1, 0); glVertex4f(2, -2, 0,0.5); // Bottom Right
	glTexCoord2f(0, 0); glVertex4f(-2, -2, 0,0.5); // Bottom Left
	glEnd();
	return 0;
}
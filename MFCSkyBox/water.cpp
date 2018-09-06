#include "stdafx.h"
#include "water.h"

CWater::CWater()
{
	isLoadWater = false;
}

CWater::~CWater()
{

}

bool CWater::LoadGLTexture(char *filename)						// Load Bitmaps And Convert To Textures
{

	AUX_RGBImageRec *pImage;										// Create Storage Space For The Texture

	pImage = loadbmp(filename);									// Loads The Bitmap Specified By filename

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (pImage != NULL && pImage->data != NULL)					// If Texture Image Exists
	{
		glGenTextures(1, &textureWater);									// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, textureWater);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);//不能用这个贴球
		free(pImage->data);											// Free The Texture Image Memory
		free(pImage);												// Free The Image Structure
	}

	isLoadWater = true;
	return isLoadWater;
}

AUX_RGBImageRec* CWater::loadbmp(char *Filename)
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

int CWater::CreatWater(int a)
{
	if (!isLoadWater)
		LoadGLTexture("Data/water.bmp");
	//int List = glGenLists(1);//创建一个显示列表，为了提高执行效率
	//glNewList(List, GL_COMPILE);//开始装入，装入后不立即执行

	glBindTexture(GL_TEXTURE_2D, textureWater);
	int /*a = 0,*/step=16;
	for (float i = 0; i < 128; i += step)
	{
		
		glBegin(GL_TRIANGLE_STRIP);
		for (float j = 0; j <= 128; j += step)
		{
			
			float wawe = a*PI / 4;
			float c = (i + step) / 128, d = j / 128;
			glTexCoord2f((i + step) / 32, j / 32);
			if ((((int)i+step)/step)%2==0)
				glVertex3f(i + step, 0.5*sin(wawe), j);
			else
				glVertex3f(i + step, 0.5*cos(wawe), j);
			glTexCoord2f(i / 32, j / 32);
			if (((int)i / step) % 2 == 0)
				glVertex3f(i, 0.5*sin(wawe), j);
			else
				glVertex3f(i, 0.5*cos(wawe), j);

			a++;
			if (a>8)a = 0;
		}
		glEnd();
	}
	//glEndList();
	//return List;
	return 0;

}
#include <GL\glaux.h>
#include <GL\glut.h>
#include "math.h"
#define PI 3.1415926


class CWater
{
private:
	bool isLoadWater;
	GLuint textureWater;
public:
	CWater ();
	~CWater ();

	AUX_RGBImageRec* loadbmp(char *filename);
	bool LoadGLTexture(char *filename);
	int CreatWater(int a);
};
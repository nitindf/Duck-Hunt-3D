#include "Model.h"

using namespace objloader;

class GenericModel
{
public:
	GenericModel()
	{
		coordinataX = 0.0;
		coordinataY = -2.5;
		coordinataZ = -4;
		scaleY = 0.05;
		scaleXZ = 0.05;
		rotateY = 0;
	}

	GenericModel(float x, float y, float z, float s1, float s2, float r)
	{
		coordinataX = x;
		coordinataY = y;
		coordinataZ = z;
		scaleY = s1;
		scaleXZ = s2;
		rotateY = r;
	}

	float getCoordinataX() { return coordinataX; }
	float getCoordinataY() { return coordinataY; }
	float getCoordinataZ() { return coordinataZ; }
	float getScaleY() { return scaleY; }
	float getScaleXZ() { return scaleXZ; }
	float getRotateY() { return rotateY; }
	void setCoordinataX(float x) { coordinataX = x; }
	void setCoordinataY(float y) { coordinataY = y; }
	void setCoordinataZ(float z) { coordinataZ = z; }
	void setScaleY(float y) { scaleY = y; }
	void setScaleXZ(float xz) { scaleXZ = xz; }
	void setRotateY(float r) { rotateY = r; }

	void loadModel(); //EACH SUBCLASS REDIFINES THEM
	void drawObject();

protected:
	float coordinataX, coordinataY, coordinataZ, scaleY, scaleXZ, rotateY;

};
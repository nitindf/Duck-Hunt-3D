#include "GenericModel.h"

using namespace objloader;

//TARGET
class Target : private GenericModel
{
public:
	Target(float x, float y, float z, float s1, float s2, float r);
	void loadModel();
	void drawObject(bool shooting, int difficulty);

private:
	int targetId;
	float rangeLeft, rangeRight, rangeUp, rangeDown;
	bool going_left;
	bool going_right;
	bool going_left_up;
	bool going_left_down;
	bool going_right_up;
	bool going_right_down;
	Model mesh_target;
};

// TABLE
class Table : public GenericModel
{
public:
	Table(float x, float y, float z, float s1, float s2, float s3, float r);
	void loadModel();
	void drawObject();
	void checkCollisionWithTable(float& z);
	
	Vector<3> minimumVertex;
	Vector<3> maximumVertex;

private:
	int tableId;
	float scaleX;
	float scaleZ;
	Model mesh_table;
};

//AMMO BOX
static Model mesh_ammobox;
class Ammobox : public GenericModel
{
public:
	Ammobox(float x, float y, float z, float s1, float s2, float r);
	void loadModel();
	void drawObject();
};

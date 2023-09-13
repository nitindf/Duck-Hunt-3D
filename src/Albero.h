#include "GenericModel.h"

using namespace objloader;

static Model mesh_tree;

class Albero: public GenericModel
{
public:
	Albero(float x, float y, float z, float s1, float s2, float r);
	void loadModel();
	void drawObject();

};

using namespace objloader;

static Model mesh_target;

class Target : public GenericModel
{
public:
	Target(float x, float y, float z, float s1, float s2, float r);
	void loadModel();
	void drawObject();

};
#include "ModelsClasses.h"

static int globalTableId = 100;

using namespace std;
Table::Table(float x, float y, float z, float s1, float s2, float s3, float r)
{
	coordinataX = x;
	coordinataY = y;
	coordinataZ = z;
	scaleY = s1;
	scaleZ = s2;
	scaleX = s3;

	setRotateY(r);

	//rotateY = r;

	//give an unique id to every initialized target
	this->tableId = globalTableId++;
}

void Table::drawObject()
{
	glPushMatrix();
	glTranslatef(getCoordinataX(), getCoordinataY(), getCoordinataZ());
	glScalef(scaleX, getScaleY(), scaleZ);
	glRotatef(getRotateY(), 0, 1, 0);
	mesh_table.render(false, false);
	glPopMatrix();
}

void Table::loadModel() {
	mesh_table.load(this->tableId, "Models/", "table.obj", "table.mtl");

	//get min e max vertices

	minimumVertex = mesh_table.minimumVertex();
	maximumVertex = mesh_table.maximumVertex();
}

// -Z -coordinataZ - (coordinataZ*scaleZ) 
// Z -coordinataZ + (coordinataZ*scaleZ) 

void Table::checkCollisionWithTable(float& camZ) {
	//we just need to check the collision with the front side of the table
	//because the model is as large as the room and it's the only side 
	//where the player can collide
	
	float collision_front = coordinataZ + ((maximumVertex.z * scaleZ)/2);
	float collision_back = coordinataZ + ((minimumVertex.z * scaleZ) / 2);
	float collision_left = coordinataX + ((maximumVertex.x * scaleZ) / 2);
	float collision_right = coordinataX + ((minimumVertex.x * scaleZ) / 2);
	

	if (camZ <= collision_front + 1)
		camZ = collision_front + 1;
}
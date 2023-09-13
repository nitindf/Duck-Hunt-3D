#include "ModelsClasses.h"

static int globalTargetId = 0;

Target::Target(float x, float y, float z, float s1, float s2, float r)
{
	coordinataX = x;
	coordinataY = y;
	coordinataZ = z;
	scaleY = s1;
	scaleXZ = s2;
	rotateY = r;
	
	//give an unique id to every initialized target
	this->targetId = globalTargetId++;

	//define boolean variables to set the movement
	int range = 48; // X_MAP_LIMIT + 8
	rangeLeft = -range; //coordinataX - range;
	rangeRight = range; //coordinataX + range;
	rangeUp = 10;
	rangeDown = -2;

	//the starting movement direction is choosen randomly
	int random_dir = 1 + std::rand() % (100);
	if (random_dir >80) {
		going_left = true;
		going_right = false;
		going_left_up = false;
		going_right_down = false;
	}
	else if (random_dir > 60){
		going_left = false;
		going_right = true;
		going_left_up = false;
		going_right_down = false;
	}
	else if (random_dir > 30) {
		going_left = false;
		going_right = false;
		going_left_up = true;
		going_right_down = false;

	}
	else {
		going_left = false;
		going_right = false;
		going_left_up = false;
		going_right_down = true;
	}
}

void Target::drawObject(bool shooting, int difficulty)
{

	// MOVE TARGETS

	
	if (going_left) {
		if (getCoordinataX() > rangeLeft) {
			this->coordinataX -= (0.05 + (0.04 * difficulty));
		}
		else {
			going_left = false;
			going_right = true;
		}
	}
	else if (going_right) {
		if (getCoordinataX() < rangeRight) {
			this->coordinataX += (0.05 + (0.04 * difficulty));
		}
		else {
			going_left = true;
			going_right = false;
		}
	}
	else if (going_left_up) {
		if (getCoordinataX() > rangeLeft && getCoordinataY() < rangeUp) {
			this->coordinataX -= (0.05 + (0.04 * difficulty));
			this->coordinataY += (0.05 + (0.04 * difficulty));
		}
		else {
			going_left_up = false;
			going_right_down = true;
		}
	}
	else if (going_right_down) {
		if (getCoordinataX() < rangeRight && getCoordinataY() > rangeDown) {
			this->coordinataX += (0.05 + (0.04 * difficulty));
			this->coordinataY -= (0.05 + (0.04 * difficulty));
		}
		else {
			going_left_up = true;
			going_right_down = false;
		}
	}
	
	//END MOVIING


	glPushMatrix();
		glTranslatef(getCoordinataX(), getCoordinataY(), getCoordinataZ());
		glScalef(getScaleXZ(), getScaleY(), getScaleXZ());
		glRotatef(getRotateY(), 0, 1, 0);
		mesh_target.render(shooting, false);
	glPopMatrix();

}

void Target::loadModel() {

	mesh_target.load(this->targetId, "Models/", "duck.obj", "duck.mtl"); // ammobox 
}

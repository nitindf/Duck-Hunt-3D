#include <GL/glew.h>
#include <GL/freeglut.h>=
#include "DrawingRoutines.h"
#include "Utils.h"
using namespace std;

void drawButton(void* font, char* string, bool picking, float num, GameAssets assets, map<std::string, int> textureIds) {

	if (picking) {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glPushMatrix();
			glBegin(GL_POLYGON);
			glColor3f(0.1 + num, 0.1 * num, 0.1 * 3 * num); 
				glVertex3f(-1.4, (-1 * num) - 1, -5.0);
				glVertex3f(1.4, (-1 * num) - 1, -5.0);
				glVertex3f(1.4, (-1 * num) - 0.3, -5.0);
				glVertex3f(-1.4, (-1 * num) - 0.3, -5.0);
			glEnd();
		glPopMatrix();

		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		//First, show the text
		glDisable(GL_BLEND);// Disabling blending to avoid text transparency
		glColor4f(1, 1, 1,1);
		glRasterPos3f(-0.7, (-1 * num) - 0.7, -5.0);
		char* c;
		for (c = string; *c != '\0'; c++) {
			glutBitmapCharacter(font, *c);
		}
		glEnable(GL_BLEND);

		//Then, load the button texture and apply it to the rectangle
		glColor3f(1.0, 1.0, 1.0);

		glPushMatrix();
			//glBindTexture(GL_TEXTURE_2D, assets.getButton());
		if (num == 0) {
			glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["button_start.png"]));
		}
		else if (num == 1) {
			glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["button_difficulty.png"]));
		}
		else {
			glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["button_exit.png"]));
		}
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0); glVertex3f(-1.4, (-1 * num) - 1, -5.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(1.4, (-1 * num) - 1, -5.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(1.4, (-1 * num) - 0.3, -5.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(-1.4, (-1 * num) - 0.3, -5.0);
			glEnd();
		glPopMatrix();
	}
}

void drawArrow(string side, bool picking, GameAssets assets, map<std::string, int> textureIds) {

	if (picking) {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		if (side == "right") {// Draw the arrows on the right side
			//Left arrow
			glPushMatrix();
				glBegin(GL_POLYGON);
					glColor3f(0.1,0.98,0.112); 
					glVertex3f(2, -2, -5.0);
					glVertex3f(2.5, -2, -5.0);
					glVertex3f(2.5, -1.5, -5.0);
					glVertex3f(2, -1.5, -5.0);
				glEnd();
			glPopMatrix();

			//Right arrow
			glPushMatrix();

				glBegin(GL_POLYGON);
					glColor3f(0.12, 0.88, 0.51);
					glVertex3f(5, -2, -5.0);
					glVertex3f(5.5, -2, -5.0);
					glVertex3f(5.5, -1.5, -5.0);
					glVertex3f(5, -1.5, -5.0);
				glEnd();
			glPopMatrix();
		}
		
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
	}

	else { 
		if (side == "right") {

			//Left arrow
			glPushMatrix();
				//glBindTexture(GL_TEXTURE_2D, assets.getArrow());
				glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["arrow.png"]));
				glBegin(GL_POLYGON);
					glTexCoord2f(0.0, 0.0); glVertex3f(2, -2, -5.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(2.5, -2, -5.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(2.5, -1.5, -5.0);
					glTexCoord2f(0.0, 1.0); glVertex3f(2, -1.5, -5.0);
				glEnd();
			glPopMatrix();

			//Right arrow
			glPushMatrix();
				glTranslatef(5.25, -1.75, 0); 
				glRotatef(180, 0, 0, 1); 
				glTranslatef(-5.25, 1.75, 0);

				glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["arrow.png"]));
				glBegin(GL_POLYGON);
					glTexCoord2f(0.0, 0.0); glVertex3f(5, -2, -5.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(5.5, -2, -5.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(5.5, -1.5, -5.0);
					glTexCoord2f(0.0, 1.0); glVertex3f(5, -1.5, -5.0);
				glEnd();
			glPopMatrix();
		}
		else if (side == "left") {
			//Left arrow
			glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["arrow.png"]));
				glBegin(GL_POLYGON);
					glTexCoord2f(0.0, 0.0); glVertex3f(-5.5, -2, -5.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(-5, -2, -5.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(-5, -1.5, -5.0);
					glTexCoord2f(0.0, 1.0); glVertex3f(-5.5, -1.5, -5.0);
				glEnd();
			glPopMatrix();

			//Right arrow
			glPushMatrix();
				glTranslatef(-2.25, -1.75, 0); 
				glRotatef(180, 0, 0, 1); 
				glTranslatef(2.25, 1.75, 0); 

				glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["arrow.png"]));
				glBegin(GL_POLYGON);
					glTexCoord2f(0.0, 0.0); glVertex3f(-2.5, -2, -5.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(-2, -2, -5.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(-2, -1.5, -5.0);
					glTexCoord2f(0.0, 1.0); glVertex3f(-2.5, -1.5, -5.0);
				glEnd();
			glPopMatrix();
		}
	}
}

void drawCrosshair() {

	glDisable(GL_TEXTURE_2D);

	glPointSize((GLfloat)2);

	glLineWidth((GLfloat)1.5);
	glBegin(GL_LINES);
		glColor3f(1,1,1);
		glVertex3f(0, 0.01, -1);
		glVertex3f(0, 0.03, -1);
		glVertex3f(0, -0.01, -1);
		glVertex3f(0, -0.03, -1);
		glVertex3f(-0.01, 0, -1);
		glVertex3f(-0.03, -0, -1);
		glVertex3f(0.03, -0, -1);
		glVertex3f(0.01, 0, -1);
	glEnd();

	glLineWidth((GLfloat)4);
	glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0, 0.01, -1);
		glVertex3f(0, 0.03, -1);
		glVertex3f(0, -0.01, -1);
		glVertex3f(0, -0.03, -1);
		glVertex3f(-0.01, 0, -1);
		glVertex3f(-0.03, -0, -1);
		glVertex3f(0.03, -0, -1);
		glVertex3f(0.01, 0, -1);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void drawGameInterface(int score, int level, int difficulty, int elapsedTime, int ammo,
	int selectedWeapon, vector<string> weapons,
	GameAssets assets, map<std::string, int> textureIds) {

	glPushMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	drawText("TIME " + to_string(elapsedTime), -0.05, 0.665, -1.0);

	drawText("SCORE " + to_string(score), -0.45, 0.665, -1.0);

	drawText("LEVEL " + to_string(level), -0.25, 0.665, -1.0);

	drawText("AMMO " + to_string(ammo) + "/" + to_string(5 * (8 - difficulty)),
		0.15, 0.665, -1.0);

	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["game_interface.png"]));
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.55, 0.63, -1);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.55, 0.63, -1);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.55, 0.72, -1);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.55, 0.72, -1);
	glEnd();
	glPopMatrix();

	if (level) {
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds[weapons[selectedWeapon]]));
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.25, -0.75, -1);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.23, -0.75, -1);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.23, -0.05, -1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.25, -0.05, -1);
		glEnd();
		glPopMatrix();
	}

}

void drawEnvironment(GLfloat x_map_limit_right, GLfloat x_map_limit_left,
	GLfloat z_map_limit_back, GLfloat z_map_limit_front,
	int selectedWeather, vector<string> weathers,
	GameAssets assets, map<std::string, int> textureIds) {

	// RIGHT WALL
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["bg.jpg"]));
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);  glVertex3f(x_map_limit_right, -3.5, z_map_limit_front-5);
			glTexCoord2f(1.0, 0.0);  glVertex3f(x_map_limit_right, -3.5, z_map_limit_back);
			glTexCoord2f(1.0, 1.0);  glVertex3f(x_map_limit_right, 12, z_map_limit_back);
			glTexCoord2f(0.0, 1.0);  glVertex3f(x_map_limit_right, 12, z_map_limit_front-5);
		glEnd();
	glPopMatrix();

	// LEFT WALL
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["bg.jpg"]));
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);  glVertex3f(x_map_limit_left, -3.5, z_map_limit_back+5);
			glTexCoord2f(1.0, 0.0);  glVertex3f(x_map_limit_left, -3.5, z_map_limit_front);
			glTexCoord2f(1.0, 1.0);  glVertex3f(x_map_limit_left, 12, z_map_limit_front);
			glTexCoord2f(0.0, 1.0);  glVertex3f(x_map_limit_left, 12, z_map_limit_back+5);
		glEnd();
	glPopMatrix();

	// BACK WALL
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["bg.jpg"]));
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);  glVertex3f(x_map_limit_right+5, -3.5, z_map_limit_back);
			glTexCoord2f(1.0, 0.0);  glVertex3f(x_map_limit_left, -3.5, z_map_limit_back);
			glTexCoord2f(1.0, 1.0);  glVertex3f(x_map_limit_left, 12, z_map_limit_back);
			glTexCoord2f(0.0, 1.0);  glVertex3f(x_map_limit_right+5, 12, z_map_limit_back);
		glEnd();
	glPopMatrix();

	// FRONT WALL
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["bg.jpg"]));
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);  glVertex3f(x_map_limit_left-5, -3.5, z_map_limit_front);
			glTexCoord2f(1.0, 0.0);  glVertex3f(x_map_limit_right, -3.5, z_map_limit_front);
			glTexCoord2f(1.0, 1.0);  glVertex3f(x_map_limit_right, 12, z_map_limit_front);
			glTexCoord2f(0.0, 1.0);  glVertex3f(x_map_limit_left-5, 12, z_map_limit_front);
		glEnd();

	glPopMatrix();

	// GRASS
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["grass.png"]));
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);  glVertex3f(x_map_limit_left, -2.5, z_map_limit_back);
			glTexCoord2f(10.0, 0.0);  glVertex3f(x_map_limit_right, -2.5, z_map_limit_back);
			glTexCoord2f(10.0, 10.0);  glVertex3f(x_map_limit_right, -2.5, z_map_limit_front);
			glTexCoord2f(0.0, 10.0);  glVertex3f(x_map_limit_left, -2.5, z_map_limit_front);
		glEnd();
	glPopMatrix();
		
	//SKY
	glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["sky.png"]));
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);  glVertex3f(-200.0, 20, -200.0); //25
		glTexCoord2f(1.0, 0.0);  glVertex3f(200.0, 20, -200.0);
		glTexCoord2f(1.0, 1.0);  glVertex3f(200.0, 20, 200.0);
		glTexCoord2f(0.0, 1.0);  glVertex3f(-200.0, 20, 200.0);
	glEnd();

}

void drawScoreboard(map<string, int> &scoreList, int playerScore, string playerName, GameAssets assets, map<std::string, int> textureIds) {
	//To order the map by value and to by key, we need to convert the map into a inversed multimap
	//Declare a multimap that orders key by descending order
	multimap<int, string, greater<int>> MM;

	// Insert every (key-value) pairs from
	// map M to multimap MM as (value-key) pairs
	for (auto& it : scoreList) {
		MM.insert({ it.second, it.first });
	}
	
	glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
			glColor4f(0.0, 0.0, 0.0, 0.7);
			glVertex3f(-1.2, -2.5, -5.001);
			glVertex3f(1.2, -2.5, -5.001);
			glVertex3f(1.2, 0.5, -5.001);
			glVertex3f(-1.2, 0.5, -5.001);
		glEnd();
		glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	
	//Loading and drawing logo
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, assets.getTexture(textureIds["game_over.png"]));
		glBegin(GL_POLYGON);
			glColor3f(1,1,1);
			glTexCoord2f(0.0, 0.0); glVertex3f(-3, 1, -5.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(3, 1, -5.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(3, 3, -5.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(-3, 3, -5.0);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glDisable(GL_BLEND);// Disabling blending to avoid text transparency
		glDisable(GL_LIGHTING);

		int map_length = MM.size();

		//WRITE THE TOP 5 SCORES
		int i = 0;
		for (auto it = MM.begin(); it != MM.end(), i< MM.size(); it++, i++) {
			string entry = to_string(i+1) + ". " + it->second + " " + to_string(it->first) + "\n";
			
			//display the text on screen
			if (entry.length()) {
				drawText(entry, -0.15, -0.15 - (0.05 * i), -1.0);
			}

			if (i == 4) {
				break;
			}
		}
		
		drawText("YOUR SCORE IS " + to_string(playerScore), -0.18, 0.13, -1.0);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
	glPopMatrix();

	

}
#include <iostream>
#include <vector>
#include <time.h>
#include <map>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <irrKlang.h>

#define COD_START 4278190105
#define COD_DIFFICULTY 4283177471
#define COD_EXIT 4288230399
#define COD_LEFT_GUN 4280154649
#define COD_RIGHT_GUN 4286767135

#define X_MAP_LIMIT_RIGHT 40
#define X_MAP_LIMIT_LEFT -40
#define Z_MAP_LIMIT_BACK 40
#define Z_MAP_LIMIT_FRONT -40

#define PI 3.14159265
#define FPS 75
#define TO_RADIANS 3.14/180.0

#include "GameAssets.h"
#include "DrawingRoutines.h"
#include "ModelsClasses.h"
#include "Utils.h"
#include <algorithm>
using namespace std;
using namespace irrklang;

static GameAssets textureAssets;
static map<string, int> menuTextureIds;
static map<string, int> inGameTextureIds;

static vector<string> weaponPreviews{"zapper_preview.png", "pistol_preview.png" };
static vector<string> weapons{"zapper.png", "pistol.png" };

static vector<string> weathers{ "sky.png"};

static vector<Table> tables{ Table(0, -2.8, -5, 0.18, 0.04, 4.2, -40.5),
							 Table(0, -3.2, -5, 0.18, 0.04, 4.2, -40.5),
							 Table(0, -3.6, -5, 0.18, 0.04, 4.2, -40.5),
							 Table(0, -4, -5, 0.18, 0.04, 4.2, -40.5),
							 Table(0, -4.4, -5, 0.18, 0.04, 4.2, -40.5) }; 

static vector<Target> targets{ Target(0, 0, -10, 1.8, 1.5, 90),
							   Target(-48, 0, -10, 1.8, 1.5, 90),
							   Target(25, 0, -12, 1.8, 1.5, 90),
							   Target(-5, 0, -8, 1.8, 1.5, 90),
							   Target(5, 0, -8, 1.8, 1.5, 90) };

static vector<int> targetsLife{1,1,1,1,1};
static vector<long long> targetsColors{4293964390,4293977472,4293984153,4293984178,4293984204};

static vector<Target>::iterator targetsIt;
static vector<Table>::iterator tablesIt;

GLfloat lightPosition[] = { 0.0, 2.0, 20.0, 1.0 };

//SOUND VARIABLES
ISoundEngine* engine = createIrrKlangDevice();
ISound* inGameSound;
static vector<const char*> gunSounds{ "Audio/zapper_gunshot.mp3", "Audio/beretta_sound.mp3"};

static int window_height = 800, window_width = 1300;
//state variables to indicate the current scene that the player is in
static bool menu = true;
static bool inGame = false; 
static bool scoreboard = false;

static bool picking = false; //variable for color picking in menu
static bool shooting = false;//variable for color picking in game

static float targetSpeed = 0.2;
static int difficulty = 0; // SELECTED DIFFICULTY -> 0: easy, 1: normal, 2: hard
static int selectedWeapon = 0; // SELECTED WEAPON -> 0: zapper, 1: pistol
static int selectedWeather = 0;// SELECTED WEATHER -> 0: normal

//TIME VARIABLES
static clock_t gameTime = 60 * CLOCKS_PER_SEC; //60 seconds.
static clock_t startTime;

//IN GAME VARIABLES
static int score = 0;
static int level = 1;
static int ammo;
static map<string, int> scoreList;
static string playerName="";
static bool hasEnteredName = false;

bool zoom = false;
bool map_view = false;

//CAMERA VARIABLES
static float pitch = 0.0, yaw = 0.0;
static float camX = 0.0, camZ = 0.0;

static GLint mouseClickX = 0, mouseClickY = 0; //mouse coordinates at clicking time

struct Motion
{
	bool Forward, Backward, Left, Right;
};
Motion motion = { false,false,false,false };

GLuint GetRGB(int x, int y) { 
	//Given the x and y coordinates of the mouse,
	//this function return the color code about the pixel in that position
	GLint viewport[4];//contains viewport informations
	GLfloat winX, winY;
	GLuint color;

	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = (GLfloat)x;
	winY = (GLfloat)viewport[3] - (GLfloat)y; // height - y  

	//providing mouse coordinates and area to consider around the click zone
	glReadPixels(GLint(winX), GLint(winY), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
	//getting in the "color" variable, the color information
	return color;
}

//GLUT INTERACTION FUNCTIONS
void click(GLint button, GLint state, GLint x, GLint y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (menu) {
			mouseClickX = x;//save x coord
			mouseClickY = y;//save y coord
			picking = true; // toggle picking mode
		}
		else if (inGame) {
			
			if (ammo > 0) {
				//if the player hasn't ran out of ammo, toggle shooting mode
				ammo--;
				mouseClickX = x;//save x coord
				mouseClickY = y;//save y coord
				engine->play2D(gunSounds.at(selectedWeapon), false);
				shooting = true;
			}
		}
	}
}
void keyboard(unsigned char key, int x, int y)
{

	if (inGame || hasEnteredName) {
		switch (key)
		{
		case 'W':
		case 'w':
			motion.Forward = true;
			break;
		case 'A':
		case 'a':
			motion.Left = true;
			break;
		case 'S':
		case 's':
			motion.Backward = true;
			break;
		case 'D':
		case 'd':
			motion.Right = true;
			break;
		case 27: //Escape Key
			glutDestroyWindow(glutGetWindow());
			engine->drop();
			exit(0);
			break;
		case '1':
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
			menu = true;
			inGame = false;
			scoreboard = false;
			//if so, then move on the next level
			level = 1;
			//adjust player position
			camX = 0; camZ = 0;
			//adjust table position
			for (tablesIt = tables.begin(); tablesIt != tables.end(); ++tablesIt) {
				(*tablesIt).setCoordinataZ(-5.0);
			}
		
			//restore targets lives
			for (int i = 0; i < targetsColors.size(); ++i)
				targetsLife.at(i) = 1;

			engine->play2D("Audio/title_screen.mp3", true);
			break;
		}
	}

	if (scoreboard) {
		switch (key)
		{
		case '1':
			menu = true;
			inGame = false;
			scoreboard = false;

			//if so, then move on the next level
			level = 1;
			//adjust player position
			camX = 0; camZ = 0;
			//adjust table position
			for (tablesIt = tables.begin(); tablesIt != tables.end(); ++tablesIt) {
				(*tablesIt).setCoordinataZ(-5.0);
			}

			//restore targets lives
			for (int i = 0; i < targetsColors.size(); ++i)
				targetsLife.at(i) = 1;

			engine->play2D("Audio/title_screen.mp3", true);
			break;
		case 27: //Escape Key
			glutDestroyWindow(glutGetWindow());
			exit(0);
			break;
		case 10: //Enter Key
			if (!hasEnteredName) {
				hasEnteredName = true;
				// write the updated scorelist in the text file
				writeScorelist(scoreList, playerName, score);
				//re-read the updated score list
				readScorelist(scoreList);
			}
			break;
		}
	}

	//If we are in the scoreboard page and press letter keys
	if (scoreboard) {
		if (playerName.size() < 12 && ((key >= 65 && key <= 90) || (key >= 97 && key <= 122))) {
			playerName.push_back(key);
		}
		else if (key == 127 && playerName.size() != 0) {
			playerName.pop_back();
		}
	}
	
}
void keyboard_up(unsigned char key, int x, int y)
{
	if (inGame) {
		switch (key)
		{
		case 'W':
		case 'w':
			motion.Forward = false;
			break;
		case 'A':
		case 'a':
			motion.Left = false;
			break;
		case 'S':
		case 's':
			motion.Backward = false;
			break;
		case 'D':
		case 'd':
			motion.Right = false;
			break;
		case 'Z':
		case 'z':
			zoom = !zoom;
			break;
		case 'T':
		case 't':
			map_view = !map_view;
			break;
		}
	}

}


void passive_motion(int x, int y)
{
	if (inGame) {
		/* two variables to store X and Y coordinates, as observed from the center
		  of the window
		*/
		int dev_x, dev_y;
		dev_x = (window_width / 2) - x;
		dev_y = (window_height / 2) - y;

		/* apply the changes to pitch and yaw*/
		yaw += (float)dev_x / 50.0;
		pitch += (float)dev_y / 50.0;
	}
}

void setup() {
	
	//Loading menu textures
	menuTextureIds = textureAssets.loadTexture(vector<string>{
													"game_over.png",
													"button_start.png", "button_difficulty.png", "button_exit.png", "arrow.png",
													"title.png",
													"zapper_preview.png", "pistol_preview.png"}, true, GL_LINEAR);
	//Loading game textures
	inGameTextureIds = textureAssets.loadTexture(vector<string>{"zapper.png", "pistol.png",
													"game_interface.png",
													"grass.png", "bg.jpg", "sky.png"}, true, GL_NEAREST);

	//Loading 3D models
	for (targetsIt = targets.begin(); targetsIt != targets.end(); ++targetsIt) {
		(*targetsIt).loadModel();
	}
	for (tablesIt = tables.begin(); tablesIt != tables.end(); ++tablesIt) {
		(*tablesIt).loadModel();
	}

	glClearColor(0, 0, 0, 0);

	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

}
void resize(int w, int h) {
	/*
	INPUT: new window dimensions

	Create a new viewport and resizes everything on that window measures
	*/

	window_width = w;
	window_height = h;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(72, ((GLfloat)w / h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}

void gameEnded() {
	inGame = false;
	scoreboard = true; 
	engine->stopAllSounds();
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	engine->play2D("Audio/gameover_sound.wav", false);
}
void checkLevel(clock_t elapsedTime) {

	//CHECKING ELAPSED TIME
	if (elapsedTime > gameTime) {
		//if a minute has elapsed, you lost.
		gameEnded();
	}
	//If time hasn't elapsed yet, keep with in game elements check
	else {
		//checking if all the targets has been eliminated
		bool areTargetsDead = std::all_of(targetsLife.begin(), targetsLife.end(), [](int i) { return i == 0; });

		if (areTargetsDead == true && level < 3) {//MAX 3 LEVELS

			//if so, then move on the next level
			level += 1;

			/*adjuste player position
			Adding translation based on game level
			it's needed to increase the aiming difficulty
			So at level 1 the distance is the default
			Every additional level increases the distance by 10 units*/
			camX = 0;
			camZ = 10.0 * (level - 1);

			//adjust barrier position to increase the shooting distance
			for (tablesIt = tables.begin(); tablesIt != tables.end(); ++tablesIt) {
				(*tablesIt).setCoordinataZ((10.0 * (level - 1)) - 3);
			}

			//restore target life and ammo
			for (int i = 0; i < targetsColors.size(); ++i)
				targetsLife.at(i) = 1;

			ammo = 5 * (8 - difficulty);
			// --- //

			//restart timer
			startTime = clock();
		}
		else if (areTargetsDead && level == 3) {
			//if the player has finished all the 3 levels
			//display the scoreboard
			gameEnded();
		}
		else if (areTargetsDead == false && ammo == 0) {
			//if the player has emptied all the ammo
			//it's a game over
			gameEnded();
		}
	}
}

void timer(int)
{
	glutPostRedisplay();
	if(inGame)
		glutWarpPointer(window_width / 2, window_height / 2);
	glutTimerFunc(1000 / FPS, timer, 0);
}
void managePicking() {
	if (picking)//if we are in picking mode
	{

		GLuint color = GetRGB(mouseClickX, mouseClickY); 
		//cout << "Option picked " << "0x" << color << ";" << endl;

		switch (color) 
		{
		case COD_START:
			menu = false;
			inGame = true;
			ammo = 5 * (8 - difficulty);
			engine->play2D("Audio/pressed_start_sound.wav", false);

			//setting the movement procedures to glut
			glutTimerFunc(0, timer, 0);
			glutKeyboardFunc(keyboard);
			glutKeyboardUpFunc(keyboard_up);
			glutSetCursor(GLUT_CURSOR_NONE);

			// stop menu theme and start game soundtrack with a lower volume
			engine->stopAllSounds();
			inGameSound = engine->play2D("Audio/inGame_soundtrack.mp3", true, false, true);
			inGameSound->setVolume(0.5);

			startTime = clock();
			break;
		case COD_DIFFICULTY:
			engine->play2D("Audio/pressed_button_sound.mp3", false);
			difficulty = difficulty > 1 ? 0 : difficulty+1;
			cout << "Difficulty: " << difficulty <<"\n";
			break;
		case COD_EXIT:
			exit(0);
			cout << "EXIT\n";
			break;
		case COD_LEFT_GUN:
			engine->play2D("Audio/pressed_button_sound.mp3", false);
			if (selectedWeapon == 0)
				selectedWeapon = 1;
			else
				selectedWeapon = 0;
			break;
		case COD_RIGHT_GUN:
			engine->play2D("Audio/pressed_button_sound.mp3", false);
			if (selectedWeapon == 0)
				selectedWeapon = 1;
			else
				selectedWeapon = 0;
			break;
		default:
			break;
		}


		picking = false;
	}
	else if (shooting) {
		GLuint color = GetRGB(window_width / 2, window_height / 2);
		// we get the color in the center of the screen (the center of crosshair, where the gun is aiming)
		// because the click can be done in time slots where the mouse is not input at 0,0 by the timer function.
		//cout << "Color picked " << "0x" << color << ";" << endl;
		for (int i = 0; i < targetsColors.size(); ++i) {
			if (targetsColors.at(i) == color) { 
				//if the i-th target was shot, reduce its life count
				targetsLife.at(i)--;

				//increase the score
				score += (10 + 5*(difficulty));

				engine->play2D("Audio/target_damage_sound.wav", false);
				break;
			}
		}
		shooting = false;
	}
}

void wallCollision(float& x, float& z) {

	float padding = 1.5;

	if (x - padding <= X_MAP_LIMIT_LEFT) {
		//if the cam collapses with the left wall
		x = X_MAP_LIMIT_LEFT + padding;
	}
	else if (x + padding >= X_MAP_LIMIT_RIGHT) {
		//if the cam collapses with the right wall
		x = X_MAP_LIMIT_RIGHT - padding;
	}

	if (z - padding <= Z_MAP_LIMIT_FRONT) {
		//if the cam collapses with the front wall
		z = Z_MAP_LIMIT_FRONT + padding;
	}
	else if (z + padding >= Z_MAP_LIMIT_BACK) {
		//if the cam collapses with the front wall
		z = Z_MAP_LIMIT_BACK - padding;
	}

}

void cameraMovement()
{
	if (motion.Forward)
	{
		camX += cos((yaw + 90) * TO_RADIANS) / 5.0;
		camZ -= sin((yaw + 90) * TO_RADIANS) / 5.0;
	}
	if (motion.Backward)
	{
		camX += cos((yaw + 90 + 180) * TO_RADIANS) / 5.0;
		camZ -= sin((yaw + 90 + 180) * TO_RADIANS) / 5.0;
	}
	if (motion.Left)
	{
		camX += cos((yaw + 90 + 90) * TO_RADIANS) / 5.0;
		camZ -= sin((yaw + 90 + 90) * TO_RADIANS) / 5.0;
	}
	if (motion.Right)
	{
		camX += cos((yaw + 90 - 90) * TO_RADIANS) / 5.0;
		camZ -= sin((yaw + 90 - 90) * TO_RADIANS) / 5.0;
	}

	if (zoom) {
		glTranslatef(0, 0, 15);
	}

	if (map_view) {
		level = 0;
		glLoadIdentity();
		glRotatef(90, 0.0, 0.0, 1.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glTranslatef(0, -25, -5);
	}
	
	if (pitch >= 75)
		pitch = 75;
	if (pitch <= -55)
		pitch = -55;

	glRotatef(-pitch, 1.0, 0.0, 0.0); // Along X axis
	glRotatef(-yaw, 0.0, 1.0, 0.0);    //Along Y axis

	//cout << "camZ: " + to_string(camZ) + "\n";
	//cout << "camX: " + to_string(camX) + "\n";
	
	wallCollision(camX, camZ);
	
	glTranslatef(-camX, 0.0, -camZ);


}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glClearColor(0, 0, 0, 0);

	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);

	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	if (menu) {

		//Loading menu background
		glPushMatrix();
			glBegin(GL_POLYGON);
				glColor4f(0.0, 0.0, 0.0, 0.7);
				glVertex3f(2.55, -2.8, -5.001);
				glVertex3f(4.95, -2.8, -5.001);
				glVertex3f(4.95, -0.75, -5.001);
				glVertex3f(2.55, -0.75, -5.001);
			glEnd();
		glPopMatrix();

		glPushMatrix();
		glBegin(GL_POLYGON);
			glColor4f(0.0, 0.0, 0.0, 0.7);
			glVertex3f(-4.95, -2.8, -5.001);
			glVertex3f(-2.55, -2.8, -5.001);
			glVertex3f(-2.55, -0.75, -5.001);
			glVertex3f(-4.95, -0.75, -5.001);
		glEnd();
		glPopMatrix();

		//Start button
		drawButton(GLUT_BITMAP_HELVETICA_18, (char*)"START", picking, 0, textureAssets, menuTextureIds);

		//Difficulty button
		vector<const char*> difficulties = {"DIFFICULTY: EASY", "DIFFICULTY: NORMAL", "DIFFICULTY: HARD"};
		drawButton(GLUT_BITMAP_HELVETICA_18, (char*)difficulties[difficulty], picking, 1, textureAssets, menuTextureIds);

		//Exit button
		drawButton(GLUT_BITMAP_HELVETICA_18, (char*)"EXIT", picking, 2, textureAssets, menuTextureIds);

		//Loading and drawing gun selection arrows on the right side of the menu
		drawArrow("right", picking, textureAssets, menuTextureIds);

		//Loading and drawing logo
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, textureAssets.getTexture(menuTextureIds["title.png"]));
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0); glVertex3f(-3, 1, -5.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(3, 1, -5.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(3, 3, -5.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(-3, 3, -5.0);
			glEnd();
		glPopMatrix();

		//Load gun preview
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, textureAssets.getTexture(menuTextureIds[weaponPreviews[selectedWeapon]]));
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0); glVertex3f(2.75, -2.55, -5.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(4.75, -2.55, -5.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(4.75, -1, -5.0);
				glTexCoord2f(0.0, 1.0); glVertex3f(2.75, -1, -5.0);
			glEnd();
		glPopMatrix();

	}
	else if(inGame) {
		//Check if the game has ended or if the time has elapsed
		clock_t elapsedTime = clock() - startTime;
		checkLevel(elapsedTime);
			
		glClearColor(0.52f, 0.80f, 0.92f, 0.8f);
		
		//PUSH ALL SCENE OBJECTS INTO MATRIX 
		glPushMatrix();
		//ONCE ALL THE SCENE ELEMENTS HAVE BEEN DRAWN, WE CAN MOVE THE CAMERA
		cameraMovement();
			drawEnvironment(X_MAP_LIMIT_RIGHT, X_MAP_LIMIT_LEFT,
				Z_MAP_LIMIT_BACK, Z_MAP_LIMIT_FRONT, 
				selectedWeather, weathers,
				textureAssets, inGameTextureIds);

			//render the table and check the collision with it before to move the camera in the current frame
			for (tablesIt = tables.begin(); tablesIt != tables.end(); ++tablesIt) {
				(*tablesIt).checkCollisionWithTable(camZ);
			}
			
			//table.drawObject();
			int i = 0;
			for (tablesIt = tables.begin(); tablesIt != tables.end(); ++tablesIt) {
				(*tablesIt).drawObject();
			}

			for (i=0, targetsIt = targets.begin(); targetsIt != targets.end(); ++targetsIt, i++) {
				if (targetsLife.at(i) != 0) { // DRAW IF THE TARGET IS STILL ALIVE
					(*targetsIt).drawObject(shooting, difficulty);
				}
			}

		glPopMatrix();
		//END SCENE OBJECTS

		//LOADING THE SELECTED GUN RIGHT AFTER THE CAMERA MOVEMENT TO AVOID THE GUN MOVING WITHIN THE SCENE
		glPushMatrix();
			drawCrosshair();
		glPopMatrix();

		drawGameInterface(score, level, difficulty, 
						(int)elapsedTime/CLOCKS_PER_SEC, 
						ammo, selectedWeapon, weapons,
						textureAssets, inGameTextureIds);

	}
	else if (scoreboard) {

		if (!hasEnteredName) {
			//Request player nickname if the player does not have one already
			drawText("ENTER NAME: " + playerName + "\n", -0.2, 0.05, -1.0);
		}
		drawText("LEADERBOARD : TOP 5", -0.2, -0.06, -1.0);
		drawText("PRESS '1' TO RETURN TO MAIN MENU | PRESS 'Esc' TO EXIT THE GAME", -0.6, -0.6, -1.0);
		drawScoreboard(scoreList, score, playerName, textureAssets, menuTextureIds);
	}

	if (shooting || picking) {
		managePicking(); //call the routine to check what is being clicked
	}
	else {
		glutSwapBuffers();
	}
}
 
int main(int argc, char** argv)
{	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height); 
	glutInitWindowPosition(340, 100);
	glutCreateWindow("Duck Hunt 3D");
	//fill the score list 
	readScorelist(scoreList);

	setup(); 
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutIdleFunc(drawScene);
	glutMouseFunc(click);
	
	glutPassiveMotionFunc(passive_motion);
	engine->play2D("Audio/title_screen.mp3", true);
	glutMainLoop();

	return 0;
}
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <GL/freeglut.h>
#include "GameAssets.h"

using namespace std;

void drawButton(void* font, char* string, bool picking, float num, GameAssets assets, map<std::string, int> textureIds);
void drawArrow(string side, bool picking, GameAssets assets, map<std::string, int> textureIds);
void drawCrosshair();
void drawGameInterface(int score, int level, int difficulty, int elapsedTime, int ammo,
	int selectedWeapon, vector<string> weapons,
	GameAssets assets, map<std::string, int> textureIds);
void drawEnvironment(GLfloat x_map_limit_right, GLfloat x_map_limit_left,
	GLfloat z_map_limit_back, GLfloat z_map_limit_front,
	int selectedWeather, vector<string> weathers,
	GameAssets assets, map<std::string, int> textureIds);
void drawScoreboard(map<string, int>& scoreList, int playerScore, string playerName, GameAssets assets, map<std::string, int> textureIds);
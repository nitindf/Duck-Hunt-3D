#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

void drawText(string text, GLfloat x, GLfloat y, GLfloat z);

void readScorelist(map<string, int>& scoreList);

void writeScorelist(map<string, int>& scoreList, string playerName, int playerScore);
#pragma once
#include <SOIL.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

class GameAssets
{
public:
	GameAssets();

	unsigned int getTexture(int);
	map<string, int> loadTexture(vector<string> texturePaths, bool transparency, GLint typeOfInterpolation);

private:
	int globalIndex;
	vector<unsigned int> textures;
};


#include "GameAssets.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;

GameAssets::GameAssets() {
	globalIndex = 0;
}

unsigned int GameAssets::getTexture(int i) { return textures[i]; }

map<string, int> GameAssets::loadTexture(vector<string> texturePaths, bool transparency, GLint typeOfInterpolation) {
	/*
	This method loads a certain number of textures specified by their path.
	It stores the textures in a vector of unsigned int that can be accessed by a get method.
	which takes in input the index of the needed texture.
	This method returns a map of indices with the relatex texture paths.
	These indices can be stored and used to retrieve the corresponding textures.

	INPUT:
		- texturePaths: vector where each element is a string corresponding to the texture path
		- typeOfInterpolation: specified type of interpolation wanted for glTexParameteri (GL_NEAREST, GL_LINEAR)

	OUTPUT:
		- textureIndices: map of string and indices to access the loaded textures
	*/
	map<string, int> textureIndices;
	unsigned int SOIL_flags;

	if (transparency)
		SOIL_flags = SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_MULTIPLY_ALPHA;
	else
		SOIL_flags = SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT;

	for (int i = 0; i < texturePaths.size(); i++) {
		string path = "Textures/" + texturePaths[i];
		char* filename = &path[0];
		unsigned int texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture
		(
			filename,
			SOIL_LOAD_RGBA,
			SOIL_CREATE_NEW_ID,
			SOIL_flags
		);

		/* check for an error during the load process */
		if (0 == texture)
		{
			printf("SOIL loading %s error: '%s'\n", texturePaths[i], SOIL_last_result());
		}
		else {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, typeOfInterpolation);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, typeOfInterpolation);

			this->textures.push_back(texture);
			//insert the texture path into the map, the value is the index of the position of the vector where the texture is stored
			textureIndices[texturePaths[i]] = globalIndex++;
		}
	}

	return textureIndices;

}

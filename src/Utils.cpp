#include "Utils.h"
using namespace std;

void readScorelist(map<string, int> &scoreList) {
	//map: that's the structure where the scorelist will be stored
	//once has been read from the text file
	scoreList.clear();
	//file to read
	fstream scoreFile;
	string line;

	//opening the file both for reading
	scoreFile.open("scores.txt", ios::in);

	if (scoreFile.is_open())
	{
		//read the top 10 scores stored in the file
		while (getline(scoreFile, line))
		{
			stringstream name_and_score(line);
			string name, score;
			getline(name_and_score, name, ' ');
			getline(name_and_score, score, ' ');
			scoreList[name] = stoi(score);
		}

	}
	scoreFile.close();
}

void writeScorelist(map<string, int> &scoreList, string playerName, int playerScore) {

	//insert the player name and score into the list
	//if the name already exists, it will be overwritten
	if(playerScore > scoreList[playerName])
		scoreList[playerName] = playerScore;

	//To order the map by value and to by key, we need to 
	//convert the map into a inversed multimap
	//Declare a multimap that orders key by descending order
	multimap<int, string, greater<int>> MM;

	// Insert every (key-value) pairs from
	// map M to multimap MM as (value-key) pairs
	for (auto& it : scoreList) {
		MM.insert({ it.second, it.first });
	}

	//file to read
	fstream scoreFile;

	//opening the file both for reading and overwriting
	scoreFile.open("scores.txt", ios::out);

	//write the first 10 elements to the file
	if (scoreFile.is_open())
	{
		int i = 0;
		for (auto it = MM.begin(); it != MM.end() && i < 10; ++it, i++) {
			string entry = it->second + " " + to_string(it->first);
			//create the string which has in first place the name and then the score
			scoreFile << entry << endl;
			//write it in to the file
		}
	}

	scoreFile.close();
}

void drawText(string text, GLfloat x, GLfloat y, GLfloat z) {
	const char* show_string = text.c_str();

	glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		//display show the text
		glColor4f(1, 1, 1, 1);
		glRasterPos3f(x, y, z);
		const char* c;
		for (c = show_string; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
		glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}
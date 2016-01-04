#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <memory>
#include <fstream>

using namespace std;
class Highscore
{
private:
	//spara highscore listan
	unique_ptr<std::ofstream> fileSaveStream;
	//ladda highscore listan
	unique_ptr<std::ifstream> loadFileStream;
public:
	//själva highscore listan
	shared_ptr<vector<pair<string,int>>> highscoreVec;
	Highscore(void);
	~Highscore(void);
	//returns a string vector. of the top 5 scores.
	//sorted
	vector<string> GetStringVec();
	//add highscore to the list
	//takes player name and score.
	void AddHighscore(string name,int score);
	//loads the highscore from previouse games, from a file.
	void LoadHighscore();//from file
	//saves the current highscore list to a file.
	void SaveHighscore();//to file
};


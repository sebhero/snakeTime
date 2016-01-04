#include "stdafx.h"
#include "Highscore.h"
#include <functional>
#include <sstream>

Highscore::Highscore(void)
{
	OutputDebugString(L"CREATE HIGHSCORE\n");
	loadFileStream = unique_ptr<std::ifstream>(new ifstream());
	fileSaveStream= unique_ptr<std::ofstream>(new ofstream()); 

	highscoreVec=unique_ptr<vector<pair<string,int>>>(new vector<pair<string,int>>());
	LoadHighscore();
}


Highscore::~Highscore(void)
{
	OutputDebugString(L"Highscore is destroyed\n");
	highscoreVec->clear();
	loadFileStream.release();
	fileSaveStream.release();
	highscoreVec.reset();
}

//loads the highscore from previouse games, from a file.
void Highscore::LoadHighscore()
{
	//load from file
	if(!loadFileStream)
	{
		OutputDebugString(L"Error problem with loading the highscore file\n");
		return;
	}

	loadFileStream->open("highscore.txt",std::ofstream::out);
	std::string line;
	int score;
	string name;
	while (std::getline(*loadFileStream, line))
	{
		std::istringstream iss(line);
		if (!(iss >> name >> score)) { break; } // error
		highscoreVec->push_back(make_pair(name,score));
	}
	OutputDebugString(L"closing HIGHSCORE\n");
	loadFileStream->close();
	//sort it
	std::sort(highscoreVec->begin(), highscoreVec->end(), [](pair<string,int> firstValue, pair<string,int> lastValue) {
	//check the int score
	return lastValue.second < firstValue.second;
	});
}
//saves the current highscore list to a file.
void Highscore::SaveHighscore()
{
	fileSaveStream->open("highscore.txt",std::ifstream::in);
	OutputDebugString(L"CALLING SAVE\n");
	if(!fileSaveStream)
	{
		OutputDebugString(L"Cannot open the output file!\n");
		return;
	}
	for(auto score: *highscoreVec)
	{
		*fileSaveStream << string(score.first+" "+to_string(score.second))<<endl;
		OutputDebugStringA(string(score.first+" "+to_string(score.second)).c_str());
	}
	OutputDebugString(L"CLOSING SAVE\n");
	fileSaveStream->close();
}

vector<string> Highscore::GetStringVec()
{
	std::sort(highscoreVec->begin(), highscoreVec->end(), [](pair<string,int> firstValue, pair<string,int> lastValue) {
	//check the int score
	return lastValue.second < firstValue.second;
	});
	vector<string> tempStringVec;
	for(auto var: *highscoreVec)
	{
		tempStringVec.push_back(string(var.first+" : "+to_string(var.second)).c_str());
	}
	return tempStringVec;
}

//Add a highscore to the list	 if its high enough
void Highscore::AddHighscore(string name,int score)
{
	//lagg till
	highscoreVec->push_back(make_pair(name,score));
	//ifall highscore ar langre an 5
	//tabort 6++ vardet
	if(highscoreVec->size()>5)
	{
		//testar pa lambda
		std::sort(highscoreVec->begin(), highscoreVec->end(), [](pair<string,int> firstValue, pair<string,int> lastValue) {
			//check the int score
			return lastValue.second < firstValue.second;
		});
		//ta bort 6 vardet
		highscoreVec->pop_back();
	}

}
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <random>
#include <functional>
#include <time.h>
#include <chrono>
#include <thread>
#include "EnumContainer.h"
#include "Snake.h"
#include "Highscore.h"

//TODO is for testing
//#include <iostream>

//snaketiem


//manage the game
class SnakeEngine
{
private:
	//Ranomdizing
	std::uniform_int_distribution<int> distribution;
	std::mt19937 engine; // Mersenne twister MT19937
	
	//the field where the snake moves around on.
	char gameField[20][21];


	//generates random number(between 1-18)
	int GetRandomPosition();

	//vector holding all the paths to the maps
	vector<string> gameMapPaths;



	//void Loads the map, parses the txt file string
	//returns true if managed to load the map otherwise false
	bool LoadMap(int currentLevel);
	
	//recursiv plasering tills hittar en position som är fri
	void PlaceFruit(int newY, int newX);
	//places a fruit on a random place.
	void PlaceFruit();

public:

	//Highscore controls the highscore.
	unique_ptr<Highscore> gameHighscore;

	SnakeEngine(void);
	~SnakeEngine(void);


	std::unique_ptr<Snake> currentSnake;

	//loads the map and sets the starting values.
	//return true if was able to load the map.
	bool InitGame(int currentLevel);

	//updates the game. check player direction and moves the player
	//in that direction, check collition with walls,fruits
	//updates score.
	GameState updateGame();
	//håller koll på spelläget, continue eller förlorar m.m.
	GameState currentGameResult;
	//Change direction of the snake
	void ChangeDirection(Direction newDirection);

	
	//vad finns på denna positionen
	char getGamePieceAtPosition(int y,int x);

	//Collition detection check if player hits a wall or a fruit or itself
	GameState CollitionDetection(pair<int,int> newPosition);
	//end results win or lose
	void GameEnd(GameState result);
	//call for placeing a new fruit at a random position


	//For testing, just draw map in console.
	void ShowMap();
	//tests the game, in console auto movement with collition detection
	//for testing purposes
	void TestGame(bool isRunning);
	//returns the diffrence in x,y from last position
//exempel om spelar går till höger så ökar bara x++ då skicka det tillbaks ett pair<y,x>{0,1}
	pair<int,int> getDirectionDiffrence(Direction newDirection);

};


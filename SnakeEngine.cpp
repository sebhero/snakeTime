#include "stdafx.h"
#include "SnakeEngine.h"
#include <iostream>
#include <io.h>



using namespace std;
//Controller för snakespelet.
//laddar in alla kartor,initalizerar highscore,laddar spelare
//skapar random också
SnakeEngine::SnakeEngine(void)
{
	currentGameResult = GameState::CONTINUE;
	gameMapPaths.push_back("level1.txt");
	gameMapPaths.push_back("level2.txt");
	
	//sätter random mellan 1-18 ( Y,X Y 0 och 19 är bara wall yttersta raderna)
	distribution= std::uniform_int_distribution<int>(1,18);
	//för att få ett slumpat tal, med default seed fick jag inte slumpningen att fungera, blev alltid samma position.
	engine.seed(time(nullptr));

	//skapar spelare snaken
	currentSnake = unique_ptr<Snake>(new Snake());
	//skapar highscore modelen
	gameHighscore = unique_ptr<Highscore>(new Highscore());
}

SnakeEngine::~SnakeEngine(void)
{
	OutputDebugString(L"Snake engine is destoryed");
}

//set starting values for each game
//loads the current map.
//sets start pos.
bool SnakeEngine::InitGame(int currentLevel)
{
	//load the map and sets the start position.
	if(LoadMap(currentLevel))
	{
		//place fruit on random place.
		PlaceFruit(GetRandomPosition(),GetRandomPosition());
		return true;
	}
	return false;
}

//generates a random number.
int SnakeEngine::GetRandomPosition()
{
	return distribution(engine);
}




//moves the player and check end game conditions
//updates player variables.
GameState SnakeEngine::updateGame()
{
	//flytta till en egen funktion som heter get pair Change
	auto newPosition=getDirectionDiffrence(currentSnake->snakeDirection);

	newPosition.first+=currentSnake->GetY();
	newPosition.second+=currentSnake->GetX();

	//make snake longer
	currentSnake->snakeTail.push_back(newPosition);

	//checks if new position hits anything, fruit or wall.
	currentGameResult=	CollitionDetection(newPosition);

	//if hits wall or snaketail
	//update score and return to view the game result lose
	if(currentGameResult == GameState::LOSE)
	{
		gameHighscore->AddHighscore("player",currentSnake->playerPoints);
		return currentGameResult;
	}
	//snake just moved and didnt eat a fruit, clear snakes position on map
	//and erase the last position in the snaketail
	if(currentGameResult == GameState::CONTINUE)
	{
		//clear on map the end of the snake
		gameField[currentSnake->snakeTail.front().first][currentSnake->snakeTail.front().second]='0';
		//remove the begining of vec since is the oldest value.
		currentSnake->snakeTail.erase(currentSnake->snakeTail.begin());
	}

	//update map, get new snake cords
	gameField[currentSnake->snakeTail.back().first][currentSnake->snakeTail.back().second]='s';
	
	return currentGameResult;
}

//load current map
bool SnakeEngine::LoadMap(int currentLevel)
{
	//path to the file.
	std::string filePath = gameMapPaths[currentLevel];
	//x,y cordinats
	int indexX = 0, indexY = 0;
	
	std::ifstream loadFile;
	loadFile.open(filePath,std::ifstream::in);
	if(loadFile.is_open())
	{
		//continue if there is more text.
		while(loadFile.good())
		{
			//get current char
			char character =(char) loadFile.get();
			//if char is a new line of empty go to next row (y).
				if(character==' ' || character=='\n')
				{
					//new row increase row (y) count and reset column count (x).
					indexY++;
					indexX=0;
				}
				else
				{

					//if current char is the player/start pos.
					if(character == 's')
					{
						//add start pos for snake
						currentSnake->AddCords(indexY,indexX);
					}
					//to handle out of bounds
					if(indexY<20)
					{
						gameField[indexY][indexX] = character;
					}
					indexX++;
				}
				
		}
		loadFile.close();//close the file when done.
		return true;
	}
	else
	{
		//error loading the file.
		return false;
	}
}

//place out a new fruit at random position on the map
void SnakeEngine::PlaceFruit()
{
	PlaceFruit(GetRandomPosition(),GetRandomPosition());
}

//place a fruit on the map with starting x and y position
void SnakeEngine::PlaceFruit(int newY, int newX)
{
	switch(gameField[newY][newX])
	{
	case '0':
		//ground, replace with fruit.
		gameField[newY][newX]='f';
		break;
	case '1':
	case 'f':
	case 's':
		//if generated position contains a:
		//snake part
		//a other fruit
		//wall
		//find a better place
		PlaceFruit(GetRandomPosition(),GetRandomPosition());		
		break;
	}
	
}

//returns game piece at provided positon
char SnakeEngine::getGamePieceAtPosition(int y,int x)
{
	//TODO fix pointer const safe stuff.
	return gameField[y][x];
}

//change the direction of the player snake, input from user.
void SnakeEngine::ChangeDirection(Direction newDirection)
{
	currentSnake->snakeDirection=newDirection;
}

//returns the diffrence in x,y from last position
//exempel om spelar går till höger så ökar bara x++ då skicka det tillbaks ett pair<y,x>{0,1}
pair<int,int> SnakeEngine::getDirectionDiffrence(Direction newDirection)
{
	pair<int,int> yxDirection(0,0);
	switch (currentSnake->snakeDirection)
	{
	case Direction::NORTH:
		//newY--;//maybe not use
		yxDirection.first--;
		break;
	case Direction::SOUTH:
		//newY++;//maybe not use
		yxDirection.first++;
		break;
	case Direction::EAST:
		//newX++;
		yxDirection.second++;
		break;
	case Direction::WEST:
		//newX--;
		yxDirection.second--;
		break;
	default:
		break;
	}

	return yxDirection;
}

//kolidations detektion. Gick spelaren på något gör minimnalt med saker.
//return resultat GameState
GameState SnakeEngine::CollitionDetection(pair<int,int> newPosition)
{
	//flytta till en egen funktion som heter collition
	switch (getGamePieceAtPosition(newPosition.first,newPosition.second))
	{
		case '0':
			return GameState::CONTINUE;
			break;
		case 'f':
			//get points for eating fruit
			currentSnake->playerPoints+=10;
			//Add a new fruit
			PlaceFruit();
			return GameState::ATE_FRUIT;
			break;
		case 's':
		case '1':
		default:
			return GameState::LOSE;
			break;
	}
}

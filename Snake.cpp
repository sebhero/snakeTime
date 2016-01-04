#include "stdafx.h"
#include "Snake.h"
#include <string>


Snake::Snake()
{
	snakeDirection=Direction::NORTH;
	snakeTail = vector<pair<int,int>>();
	playerPoints= 0;
	drawnSize=1;
	//string temp = "NEW Default snake: snaketail size: "+to_string(snakeTail->size())+"\n";
	//OutputDebugStringA(temp.c_str());
}

Snake::Snake(int startY, int startX)
{
	snakeDirection=Direction::NORTH;
	snakeTail = vector<pair<int,int>>();
	snakeTail.push_back(pair<int,int>(startY,startX));
	playerPoints=0;
	drawnSize=1;
}

Snake::~Snake(void)
{
	OutputDebugString(L"Snake is destoryed");
	snakeTail.clear();
	
}

//returns the latest x value. of the snakes tail
const int Snake::GetX()
{
	return (const int)snakeTail.back().second;
	
}

//returns the latest Y value. of the snakes tail
const int Snake::GetY()
{
	return (const int)snakeTail.back().first;
}


//Add a new cord to player snake
void Snake::AddCords(int newY,int newX)
{
	snakeTail.push_back(pair<int,int>(newY,newX));		
}

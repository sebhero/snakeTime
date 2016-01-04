#pragma once
#include <vector>
#include "EnumContainer.h"
#include <memory>


using namespace std;
class Snake
{
private:
	//how much of the snake is drawn.
	int drawnSize;
public:
	std::vector<pair<int,int>> snakeTail;//all positions the snake occupies
	Direction snakeDirection;//normal north needs to be get and set.
	int playerPoints;
	Snake(void);
	Snake(int startY, int startX);
	//Snake(void);
	~Snake(void);
	//ger dig senaste X värdet
	const int GetX();
	//ger dig senaste Y värdet
	const int GetY();

	//lägger till en ny del till svansen
	void AddCords(int newY,int newX);
};


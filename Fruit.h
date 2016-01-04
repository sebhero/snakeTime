#pragma once

#include "GamePiece.h"

enum class FruitType {RED, BLUE, GREEN};

///fruit model
class Fruit:GamePiece
{
private:
	FruitType theType;

public:
	//Fruit(void);
	Fruit(int ptrNewX,int ptrNewY);
	~Fruit(void);
};


#pragma once
#include <memory>
//the diffrent types of gamepieces of the field
enum class GamePiece{ WALL=1, GROUND=0, SNAKE='s', FRUIT='f'};

//enum for where is the snake headed
enum class Direction{ NORTH, SOUTH, EAST, WEST};

//enum for game loop. Win, lose or continue GameStateGameState
enum class GameState{ ATE_FRUIT, LOSE, CONTINUE};
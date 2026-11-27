#pragma once
#include "D3DUtil.h"
//common data structures
struct short2
{
	short x;
	short y;
};



//MAP CONSTANTS
constexpr short tileSize = 1;

constexpr short gridSize = 22;


//TILE CONSTANTS
enum GridItem { EMPTY, WALL, APPLE, SNAKE_HEAD, SNAKE_BODY };

//direction
enum DirectionValue { NO_DIR, UP_DIR, LEFT_DIR, RIGHT_DIR, DOWN_DIR };


////useful funcs
static DirectX::SimpleMath::Vector3 MapPosToVec3(short2 xy) { return DirectX::SimpleMath::Vector3(xy.x - gridSize / 2.f + tileSize / 2.f, 0, xy.y - gridSize / 2.f + tileSize / 2.f); }



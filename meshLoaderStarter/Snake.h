#pragma once
#include "Map_t.h"

//https://www.geeksforgeeks.org/cpp/cpp-linked-list/
//https://www.geeksforgeeks.org/cpp/operator-overloading-cpp/

class GameObject_t; //forward declaration to allow for pointers
class Camera_t;

class Snake_t
{
public:
	Snake_t();
	~Snake_t();
	short2 GetPosition();
	short2 GetPreviousPosition();
	void MoveSnake(const short2& pos, Map_t& m, Camera_t& cam);
	bool ValidSnakeMovement(short2&);
	bool& HasLife();
	DirectionValue& GetDirection();
	const int GetScore() const;

	Snake_t& operator++();
	Snake_t& operator--();

	static GameObject_t* BodyParts[gridSize * gridSize]; 
private:
	void IncrementScore();
	Segment_t* GetTail();
	Segment_t* mpHead = nullptr;

	bool snakeAlive = true;
	DirectionValue direction = NO_DIR;
	int score = 0;
};




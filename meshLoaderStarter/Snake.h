#pragma once
#include "Map_t.h"
#include "Model.h"

//https://www.geeksforgeeks.org/cpp/cpp-linked-list/
//https://www.geeksforgeeks.org/cpp/operator-overloading-cpp/

class Snake_t
{
public:
	Snake_t();
	~Snake_t();
	short2 GetPosition();
	short2 GetPreviousPosition();
	void MoveSnake(const short2& pos, Map_t& m);
	bool ValidSnakeMovement(short2&);
	bool& HasLife();
	DirectionValue& GetDirection();

	Snake_t& operator++();
	Snake_t& operator--();
private:
	Segment_t* GetTail();
	Segment_t* mpHead = nullptr;

	bool snakeAlive = true;
	DirectionValue direction = NO_DIR;
};




#pragma once
#include "Contants.h"
class Tile_t
{
public:
	//Tile_t();
	//~Tile_t();
	const GridItem& GetID() const;
	void SetID(GridItem i);
	const char GetDisplayValue() const;
	
private:
	GridItem id = EMPTY;
	char displayValue = ' ';
	
};

class GameObject_t;				//forward declaration to allow for pointers

class Segment_t : public Tile_t
{
public:
	Segment_t() = delete;
	Segment_t(Segment_t&) = delete;
	Segment_t(Segment_t&&) = delete;

	Segment_t(Segment_t* prevSeg, const int bodyID);
	void SetPos(const short2& newPos);
	const short2 GetPos() const;
	const short2 GetPrevPos() const;
	const int GetBodyID() const;

	Segment_t* pPrevSegment;
	Segment_t* pNextSegment = nullptr;
private:
	short2 pos = { 0,0 };
	short2 prevPos = pos;
	const int BodyID;

};







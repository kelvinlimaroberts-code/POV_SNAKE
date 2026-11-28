#include "Snake.h"
#include "Model.h"
#include "Camera_t.h"

GameObject_t* Snake_t::BodyParts[gridSize * gridSize];

Snake_t::Snake_t()
{
	mpHead = new Segment_t(nullptr, 0);
	mpHead->SetID(SNAKE_HEAD);
    mpHead->SetPos({ gridSize / 2, gridSize / 2 });
}

Snake_t::~Snake_t()
{
    while (GetTail()->pPrevSegment != nullptr) //check if the segment is the head
    {
        --*this;   //delete everything in linked list
    }
	delete mpHead;      //delete head
	mpHead = nullptr;
}

short2 Snake_t::GetPosition()
{
    return mpHead->GetPos();
}

short2 Snake_t::GetPreviousPosition()
{
    return mpHead->GetPrevPos();
}

Snake_t& Snake_t::operator++() 
{
    IncrementScore();
	Segment_t* tail = GetTail();
	tail->pNextSegment = new Segment_t(tail, score);
    return *this;

}

Snake_t& Snake_t::operator--() 
{
	Segment_t* penultimatePiece = GetTail()->pPrevSegment;
	delete penultimatePiece->pNextSegment;
	penultimatePiece->pNextSegment = nullptr;
    return *this;
}

void Snake_t::MoveSnake(const short2& pos, Map_t& m, Camera_t& camera)
{
    //work from head down the linked list
    mpHead->SetPos(pos);                                //object update head pos
    m.IdentifyAsItem(SNAKE_HEAD, pos);                  //map register head
    BodyParts[0]->SetPosition(MapPosToVec3(pos));       //world register head
    if (mpHead->pNextSegment != nullptr)                //if its not just the head
    {
        m.UnidentifyItem(mpHead->GetPrevPos());         //map deregister previous head position
        m.IdentifyAsItem(SNAKE_BODY, mpHead->GetPrevPos());//map register previous position as snake body
    }
    Segment_t* segmentIndex = mpHead;                   //index for navigating linked list
    int bodyIndex = 0;                                  //index for navigating body game objects
    while (segmentIndex->pNextSegment != nullptr)       //for the rest of the segments
    {
        //update the position of the next one with the previous position of the current selected
        segmentIndex->pNextSegment->SetPos(segmentIndex->GetPrevPos());

        //update world position of the next one, with the previous position of the one selected
        BodyParts[bodyIndex + 1]->SetPosition(MapPosToVec3(segmentIndex->GetPrevPos()));

        segmentIndex = segmentIndex->pNextSegment;      //update the index to be pointing at the next element
        bodyIndex++;                                    //update body index to look at next 
    }
    m.UnidentifyItem(segmentIndex->GetPrevPos());       //map deregister snake tail tip

    //MOVE THE CAMERA
    //have its target be the direction we want the snake to face
    short2 camTrgt = pos;
    switch (direction)
    {
    case NORTH_DIR:
        camTrgt.x--;
        break;
    case WEST_DIR:
        camTrgt.y--;
        break;
    case EAST_DIR:
        camTrgt.y++;
        break;
    case SOUTH_DIR:
        camTrgt.x++;
        break;
    default:
        camTrgt = { 4,4 };
        break;
    }
    camera.Relocate(MapPosToVec3(pos), MapPosToVec3(camTrgt));
}



bool Snake_t::ValidSnakeMovement(short2& desiredNewPos)
{
    if (direction != NO_DIR)
    {
        switch (direction)
        {
        case NORTH_DIR:
            desiredNewPos.x--;
            break;
        case WEST_DIR:
            desiredNewPos.y--;
            break;
        case EAST_DIR:
            desiredNewPos.y++;
            break;
        case SOUTH_DIR:
            desiredNewPos.x++;
            break;
        default:
            break;
        }

        return true;
    }
    else
    {
        return false;
    }
}





bool& Snake_t::HasLife()
{
    return snakeAlive;
}

DirectionValue& Snake_t::GetDirection()
{
    return direction;
}

const int Snake_t::GetScore() const
{
    return score;
}

void Snake_t::IncrementScore()
{
    score++;
    BodyParts[score]->SetActive();
}

Segment_t* Snake_t::GetTail()
{
    Segment_t* pIndex = mpHead;                 //use this to read/write to list
    while (pIndex->pNextSegment != nullptr)     //does a segment have no more after it?
    {
        pIndex = pIndex->pNextSegment;			//go to next one, if there is one
    }
    return pIndex;								//return pointer to final segment object
}


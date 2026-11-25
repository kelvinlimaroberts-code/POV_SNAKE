#include "Map_t.h"

Map_t::Map_t()
{
    //IDENTIFY walls
    for (short y = 0; y < gridSize; y++) //fill first and last rows as walls (y)
    {
        IdentifyAsItem(WALL, { 0, y });
        IdentifyAsItem(WALL, { gridSize - 1, y });
    }
    for (short x = 1; x < gridSize; x++) //fill first and last columns as walls (x)
    {
        IdentifyAsItem(WALL, { x, 0 });
        IdentifyAsItem(WALL, { x, gridSize - 1 });
    }
}

GridItem Map_t::GetIdentityOfItem(short2 pos)
{
    return mapGrid[pos.x][pos.y].GetID();
}

void Map_t::IdentifyAsItem(GridItem item, short2 pos)
{
    if (mapGrid[pos.x][pos.y].GetID() == GridItem::EMPTY)
    {
        mapGrid[pos.x][pos.y].SetID(item);
    }
}

void Map_t::UnidentifyItem(short2 pos)
{
    if (mapGrid[pos.x][pos.y].GetID() != GridItem::WALL)
    {
        mapGrid[pos.x][pos.y].SetID(EMPTY);
    }
}


bool& Map_t::AppleEaten()
{
    return appleEaten;
}

char Map_t::GetDisplayValueAt(short2 pos)
{
    return mapGrid[pos.x][pos.y].GetDisplayValue();
}

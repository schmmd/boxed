#include "board.h"

#include <stdlib.h>
#include <memory.h>

char CBoard::Get (int x, int y)  
{ 
  if (x >= _width || x < 0 || y >= _height || y < 0) 
    return ERROR; 
  else 
    return _board[x + _width * y];
}
  
void CBoard::Set (int x, int y, char val) 
{ 
  _board[x + _width * y] = val;
}

void CBoard::PopulateBoxes(float flDensity)
{
  int iBoxes = (int)(_width * _height * flDensity);

  for (int i = 0; i < iBoxes; i++)
  {
    while (true)
    {
      int x, y;
      x = rand() % _width;
      y = rand() % _height;

      if (Get(x, y) == EMPTY)
      {
        Set(x, y, BOX);
        break;
      }
    }
  }
}

void CBoard::RandomSpace(int & x, int & y)
{
  while (true)
  {
    x = (int)(rand() % _width);
    y = (int)(rand() % _height);

    if (Get(x, y) == EMPTY)
      break;
  }
}

void CBoard::Size(int width, int height)
{
  _width  = width;
  _height = height;
  _board = new char[width * height];
  memset(_board, EMPTY, sizeof(char) * width * height);
}

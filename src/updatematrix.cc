#include "updatematrix.h"

#include <memory.h>

CUpdateMatrix::~CUpdateMatrix()
{
  if (_matrix)
    delete _matrix;
    
  _matrix = NULL;
}

void CUpdateMatrix::Create(int width, int height)
{
  if (_matrix)
    delete _matrix;
  
  _matrix = new bool[width + height * width];
  _width = width;
  _height = height;
  
  Clear();
}

void CUpdateMatrix::Set(int x, int y)
{
  _matrix[x + y * _width] = true;
}

bool CUpdateMatrix::Get(int x, int y)
{
  return _matrix[x + y * _width];
}

void CUpdateMatrix::Clear()
{
  memset(_matrix, 0, sizeof(bool) * _width * _height);
}

void CUpdateMatrix::Fill()
{
  memset(_matrix, true, sizeof(bool) * _width * _height);
}

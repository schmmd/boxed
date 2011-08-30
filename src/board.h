#ifndef _H_BOARD
#define _H_BOARD

#define ERROR 0
#define EMPTY 1
#define BOX   2

class CBoard
{
public:
  CBoard() : _board(0), _width(0), _height(0) { }

  ~CBoard()
  {
    delete _board;
  }

  char Get (int x, int y);
  void Set (int x, int y, char val);

  int  Width () { return _width;  }
  int  Height() { return _height; }

  void RandomSpace(int & x, int & y);

  void Size(int width, int height);

  void PopulateBoxes(float flDensity);
  void Clear()
  {
    for (int i = 0; i < _width; i++)
      for (int n = 0; n < _height; n++)
        Set(i, n, EMPTY);
  }

private:
  char * _board;
  int _width;
  int _height;
};

#endif

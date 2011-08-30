#ifndef _H_WORLD
#define _H_WORLD

#include "linkedlist.h"
#include "board.h"
#include "beast.h"

#include "updatematrix.h"

#include <gtk/gtk.h>
#include <memory.h>

enum state
{
  INTRO,GAME,ARCADE
};

struct score
{
  guint32 dwSeconds;
  int iLevel;
};

class CWorld
{
public:
  CWorld();
  ~CWorld();
  
  void SaveSettings();
  void LoadSettings();

  // New (both game and arcade)
  void New();

  void NewGame();
  void NewArcade();

  void CreateLevel();
  void CreatePlayer();

  void AttachUpdateMatrix(CUpdateMatrix* matrix)
  {
    _updatematrix = matrix;
  }

  bool KeyDown(guint keyval);
  void RunBeastAI();

  void Size(int width, int height);

  bool IsBeast(int x, int y, CBeast * exclude = NULL);
  bool BeastsContained();

  int GetWinFaceIndex() { return _iWinFaceIndex; }
  int GetFaceIndex()    { return _iFaceIndex; }

  bool IsPaused() { return _bPaused; }
  void Pause   () { _bPaused = true; }
  void Unpause () { _bPaused = false; }

  void Win ();
  bool Won () { return _bWon; }
  void Lose();
  bool Lost () { return _bLost; }

  guint32 GetSeconds() { return _dwSeconds; }
  void  AddSecond () { _dwSeconds++; }

  int GetBeastNumber() { return _iBeasts; }
  int GetBeastDelay () { return _iBeastDelay; }
  int GetBoxDensity () { return _iBoxDensity; }
  int GetLevel      () { return _iLevel; }

  state GetState() { return _state; }

  bool Highscore();

  bool OutOfBounds(CEntity & entity)
  {
    if (entity.GetX() < _game.x || entity.GetY() < _game.y || entity.GetX() > _game.x + _game.width-1 || entity.GetY() > _game.y + _game.height-1)
      return true;

    return false;
  }

  bool IllegalPosition(CEntity & entity)
  {
    if (OutOfBounds(entity) || _board.Get(entity.GetX(), entity.GetY()) == BOX)
      return true;

    return false;
  }

  CPlayer _player;
  CBoard  _board;
  CLinkedList<CBeast> _beasts;

  int _iBeasts, _iBeastDelay, _iBoxDensity;
  int _iFaceIndex, _iWinFaceIndex;

  score _highscore;

private:

  int _iLevel;

  guint32 _dwSeconds;
  guint32 _dwTotalSeconds;

  state _state;

  GdkRectangle _game;
  bool _bPaused;
  bool _bWon;
  bool _bLost;

  CUpdateMatrix* _updatematrix;
};

extern CWorld *_world;

#define FACE_NORMAL  2
#define FACE_COOL  3
#define FACE_WINK   4

#define WIN_SMILE    5
#define WIN_LAUGH    6
#define WIN_CHUCKLE  7
#define WIN_BIGSMILE 8
#define WIN_RELIEVED 9
#define WIN_COOL     10
#define WIN_SMOKER   11

#endif

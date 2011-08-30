#include "constant.h"
#include "world.h"

#include <time.h>
#include <stdlib.h>
#include <fstream>

#include <gdk/gdkkeysyms.h>

char szSettingsFile[32];

CWorld::CWorld () : _iBeasts(4), _iBeastDelay(45), _iBoxDensity(25), 
  _iFaceIndex(0), _iWinFaceIndex(0), _dwSeconds(0), _state(INTRO),
  _bPaused(false), _bWon(false), _bLost(false)
{ 
  memset(&_game, 0, sizeof(GdkRectangle)); 
  srand(time(NULL));
  
  // Record settings file path (~/.boxed)
  sprintf(szSettingsFile, "%s/.boxed", getenv("HOME"));

  LoadSettings();
}

CWorld::~CWorld ()
{
  SaveSettings();
}

void CWorld::LoadSettings()
{
  std::ifstream fin(szSettingsFile, std::ios::in | std::ios::binary);

  if (!fin)
  {
    _highscore.iLevel = 0;
    _highscore.dwSeconds = 0;
    
    g_print("File '%s' does not exist. It will be created on exit.\n", szSettingsFile);
  }
  else
  {
    fin.read((char*)&_highscore, sizeof(score));
    fin.close();
  }
}

void CWorld::SaveSettings()
{
  std::ofstream fout(szSettingsFile, std::ios::out | std::ios::binary);
  
  if (fout)
  {
    fout.write((char*)&_highscore, sizeof(score));
    fout.close();
    
    g_print("Settings saved to file '%s'.\n", szSettingsFile);
  }
  else
  {
    g_print("File '%s' could not be saved.\n", szSettingsFile);
  }
}

void CWorld::New()
{
  int x, y;

  _bWon  = false;
  _bLost = false;

  _dwSeconds = 0;

  _board.Clear();
  _board.PopulateBoxes((float)_iBoxDensity / (float)100.0);

  // Create player
  _board.RandomSpace(x, y);
  _player.SetX(x);
  _player.SetY(y);

  // Create beasts
  _beasts.DeleteItems();
  for (int i = 0; i < _iBeasts; i++)
  {
    do
    {
      _board.RandomSpace(x, y);
    } while (IsBeast(x, y) || (_player.GetX() == x && _player.GetY() == y));

    _beasts.AddItem();
    CBeast * beast = _beasts.GetItem(0);

    beast->SetX(x);
    beast->SetY(y);
    beast->AttachBoard (&_board);
    beast->AttachPlayer(&_player);
    beast->SetType     (BEAST_REGULAR);
  }

  if (BeastsContained())
    Win();
  
  _updatematrix->Fill();
}

void CWorld::NewGame()
{
  _state = GAME;
  _iLevel = 1;
  _dwTotalSeconds = 0;
  
  CreatePlayer();
  CreateLevel();
  New();
}

void CWorld::NewArcade()
{
  _state = ARCADE;
  _dwTotalSeconds = 0;

  // Create player
  if (rand() % CADE_COOL_RARITY == 0)
    _iFaceIndex = FACE_COOL;
  else
    _iFaceIndex = FACE_NORMAL;

  New();
}

void CWorld::CreatePlayer()
{
  if (rand() % GAME_COOL_RARITY == 0)
    _iFaceIndex = FACE_COOL;
  else
    _iFaceIndex = FACE_NORMAL;
}

void CWorld::CreateLevel()
{
  _iBeasts      = 3 + (_iLevel - 1);
  _iBeastDelay  = 60 - 2 * (_iLevel - 1);
  _iBoxDensity  = 35 - (int)(0.5 * (_iLevel - 1));
}

void CWorld::Size(int width, int height)
{
  _game.width  = width;
  _game.height = height;

  _board.Size(width, height);
}

bool CWorld::KeyDown(guint keyval)
{
  if (!Won() && !Lost())
  {
    int xOld = _player.GetX(), yOld = _player.GetY();
    
    // Move the player in the key's direction
    switch(keyval)
    {
      case GDK_Left:
        _player.Left();
        break;
      
      case GDK_Right:
        _player.Right();
        break;
      
      case GDK_Up:
        _player.Up();
        break;
      
      case GDK_Down:
        _player.Down();
        break;
      
      default:
        // Havn't moved. Return false.
        return false;
    }
    
    int xNew = _player.GetX(), yNew = _player.GetY();

    // Set up update matrix
    _updatematrix->Set(xOld, yOld);  
    _updatematrix->Set(xNew, yNew);  
    
    bool bIllegalPosition = false;

    // Push boxes if
    if (_board.Get(xNew, yNew) == BOX)
    {
      // Determine change of direction
      int cx = _player.GetX() - xOld;
      int cy = _player.GetY() - yOld;

      // Used to find the end of the block chain
      int  x = xNew;
      int  y = yNew;

      // Find next empty space
      while(_board.Get(x, y) == BOX)
      {
        // Move along block chain
        x += cx;
        y += cy;

        // If out of bounds
        if (x < 0 || y < 0 || x >= _board.Width() || y >= _board.Height())
        {
          // Player cannot move
          bIllegalPosition = true;
          break;
        }
      }

      if (!bIllegalPosition && !IsBeast(x, y))
      {
        // Swap empty space with pushed tile
        _board.Set(x        , y        , BOX);
        _board.Set(xNew              , yNew          , EMPTY);
        
        // Set the update matrix
        // The pushed box square must be redrawn
        _updatematrix->Set(x , y);
      }
      else
        // Illegal because a beast exists at the end
        // of the box chain
        bIllegalPosition = true;
    }
    
    // Don't move if out of bounds or against
    // a stiff box chain
    if (OutOfBounds(_player) || bIllegalPosition)
    {
      _player.SetX(xOld);
      _player.SetY(yOld);
    }

    // Test for victory or loss
    if (BeastsContained())
      Win();
    else if (IsBeast(_player.GetX(), _player.GetY()))
      Lose();

    return true;
  }

  return false;
}

bool CWorld::IsBeast(int x, int y, CBeast * exclude)
{
  int i = 0;
  CBeast * beast;
  
  // Loop through beasts
  while ((beast = _beasts.GetItem(i++)) != NULL)
  {
    // Ignore if excluded
    if (beast == exclude)
      continue;

    // Test for match
    if (beast->GetX() == x && beast->GetY() == y)
      return true;
  }

  return false;
}

bool CWorld::BeastsContained()
{
  int i = 0;
  CBeast * beast;
  
  // Loop through beasts
  while ((beast = _beasts.GetItem(i++)) != NULL)
  {
    int x = beast->GetX(), y = beast->GetY();

    // Check surrounding squares
    for (int cx = -1; cx <= 1; cx++)
    {
      for (int cy = -1; cy <= 1; cy++)
      {
        char tile = _board.Get(x + cx, y + cy);
        if (!(cx == 0 && cy == 0) && (tile != BOX && tile != ERROR && !IsBeast(x + cx, y + cy)))
          return false;
      }
    }
  }

  return true;
}

void CWorld::RunBeastAI()
{
  int i = 0;
  CBeast * beast;
  
  // Loop through beasts
  while ((beast = _beasts.GetItem(i++)) != NULL)
  {
    int xOld = beast->GetX(), yOld = beast->GetY();

    beast->RunAI();
    
    int xNew = beast->GetX(), yNew = beast->GetY();

    // Prevent illegal positions
    if (IllegalPosition(*beast) || IsBeast(xNew, yNew, beast))
    {
      beast->SetX(xOld);
      beast->SetY(yOld);
    }
    else
    {
      // Mark updated tile on the update matrix
      _updatematrix->Set(xNew, yNew);
      _updatematrix->Set(xOld, yOld);
    }
  }

  if (IsBeast(_player.GetX(), _player.GetY()))
    Lose();
}

bool CWorld::Highscore()
{
  bool bHighscore = false;

  // Highscore if reached higher level
  if (_iLevel > _highscore.iLevel)
    bHighscore = true;
  // Highscore if reached highest level but faster
  else if (_iLevel == _highscore.iLevel && _dwTotalSeconds < _highscore.dwSeconds)
    bHighscore = true;

  // Save highscore
  if (bHighscore)
  {
    _highscore.iLevel    = _iLevel;
    _highscore.dwSeconds = _dwTotalSeconds;
    return true;
  }

  return false;
}

void CWorld::Win()
{
  _dwTotalSeconds += _dwSeconds;

  if (_iFaceIndex == FACE_NORMAL)
  {
    // Determine smile based on level completion time
    if (_dwSeconds < 60)
      _iWinFaceIndex = WIN_LAUGH;
    else if (_dwSeconds < 120)
      _iWinFaceIndex = WIN_CHUCKLE;
    else if (_dwSeconds < 240)
      _iWinFaceIndex = WIN_BIGSMILE;
    else if (_dwSeconds > 480)
      _iWinFaceIndex = WIN_RELIEVED;
    else
      _iWinFaceIndex = WIN_SMILE;
  }
  else if (_iFaceIndex == FACE_COOL)
  {
    // Different selection if "cool face"
    if (_dwSeconds < guint32(60 + _iLevel * 20))
      _iWinFaceIndex = WIN_SMOKER;
    else
      _iWinFaceIndex = WIN_COOL;
  }

  _bWon = true;
  _iLevel++;
}

void CWorld::Lose()
{ 
  // Add level seconds onto total seconds
  _dwTotalSeconds += _dwSeconds;

  _bLost = true; 
}

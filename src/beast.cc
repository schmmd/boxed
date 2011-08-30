#include "beast.h"

#include <stdlib.h>

void CBeast::RunAI()
{
  int xPlayer = _player->GetX(), yPlayer = _player->GetY();
  int cx=0, cy=0;

  int i;

  switch(_type)
  {
  case BEAST_REGULAR:

    // Move diagonally
    if (GetX() < xPlayer)
      cx = 1;
    else if (GetX() > xPlayer)
      cx = -1;
    if (GetY() < yPlayer)
      cy = 1;
    else if (GetY() > yPlayer)
      cy = -1;

    // If to move position is a box
    if (_board->Get(GetX() + cx, GetY() + cy) == BOX)
    {
      // Choose random direction
      cx = rand()%3 - 1; // Random from -1 to 1
      cy = rand()%3 - 1; // Random from -1 to 1
    }
  break;
  case BEAST_RANDOM:

    for (i = 0; i < 5; i++)
    {
      // Choose random direction
      cx = rand()%3 - 1; // Random from -1 to 1
      cy = rand()%3 - 1; // Random from -1 to 1

      if (_board->Get(GetX() + cx, GetY() + cy) != BOX)
        break;
    }
  break;
  case BEAST_TRACER:

    // Move diagonally
    if (GetX() < xPlayer)
      cx = 1;
    else if (GetX() > xPlayer)
      cx = -1;
    if (GetY() < yPlayer)
      cy = 1;
    else if (GetY() > yPlayer)
      cy = -1;

    if (iTrace == 0)
    {
      // If to move position is a box
      if (_board->Get(GetX() + cx, GetY() + cy) == BOX)
      {
        // Begin tracing
        yPre = GetY();
        xPre = GetX();

        iTrace = 20;

        if (cx != 0)
        {
          if (yPlayer > GetY())
            cyTrace = 1;
          else if (yPlayer < GetY())
            cyTrace = -1;
          else
          {
            if (GetY() < _board->Height() / 2)
              cyTrace = 1;
            else
              cyTrace = -1;
          }

          cxTrace = 0;
        }
        else // (cy != 0)
        {
          if (xPlayer > GetX())
            cxTrace = 1;
          else if (xPlayer < GetX())
            cxTrace = -1;
          else
          {
            if (GetX() < _board->Width() / 2)
              cxTrace = 1;
            else
              cxTrace = -1;
          }

          cyTrace = 0;
        }
      }
    }
    if (iTrace > 0)
    {
      if (cyTrace != 0)
      {
        cy = cyTrace;

        while (_board->Get(GetX() + cx, GetY() + cy) == BOX)
        {
          if (GetX() < xPlayer)
            cx -= 1;
          else if (GetX() > xPlayer)
            cx += 1;
          else
          {
            iTrace = 0;
            break;
          }

          if (cx > 1)
          {
            cx = 1;
            break;
          }
          if (cx < -1)
          {
            cx = -1;
            break;
          }
        }

        if (xPlayer - (GetX() + cx) < xPlayer - xPre)
          iTrace = 0;
      }
      if (cxTrace != 0)
      {
        cx = cxTrace;

        while (_board->Get(GetX() + cx, GetY() + cy) == BOX)
        {
          if (GetY() < yPlayer)
            cy -= 1;
          else if (GetY() > yPlayer)
            cy += 1;
          else
          {
            iTrace = 0;
            break;
          }

          if (cy > 1)
          {
            cy = 1;
            break;
          }
          if (cy < -1)
          {
            cy = -1;
            break;
          }
        }
        if (yPlayer - (GetY() + cy) < yPlayer - yPre)
          iTrace = 0;
      }
    }
  break;
  }

  SetX(GetX() + cx);
  SetY(GetY() + cy);
}

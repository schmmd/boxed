#include "view.h"
#include "images.h"
#include "world.h" 

#include <stdio.h>

CView _view;

void CView::Size(int width, int height)
{
  _updatematrix.Create(width / TILEW, height / TILEH);
  
  _client.x      = 0;
  _client.y      = 0;
  _client.width  = width;
  _client.height = height;

  _display[0].x   = _client.x;
  _display[0].y   = _client.y;
  _display[0].width  = _client.width;
  _display[0].height = 20;

  _display[1].x = _client.x;
  _display[1].y = _display[0].y + _display[0].height;
  _display[1].width = _client.width;
  _display[1].height = _client.height - 20 - _display[0].height;
  
  _display[2].x = _client.x;
  _display[2].y = _display[1].y + _display[1].height;
  _display[2].width = _client.width;
  _display[2].height = _client.height - _display[1].height;
}

void CView::Update(GtkWidget* widget, GdkPixmap* pixmap)
{
  // Whitewash

  // Clear displays
  gdk_draw_rectangle (pixmap,
        widget->style->white_gc,
        TRUE,
        _display[0].x, _display[0].y,
        _display[0].width,
        _display[0].height);
  gdk_draw_rectangle (pixmap,
        widget->style->white_gc,
        TRUE,
        _display[2].x, _display[2].y,
        _display[2].width,
        _display[2].height);

  if (_world->GetState() == INTRO)
  {
    // Draw title screen
    DrawOutline(pixmap);
    DrawTitleText(widget, pixmap);
  }
  else
  {
    // Draw game screen
    DrawOutline(pixmap);
    DrawBoard(widget, pixmap);
    DrawPlayer(pixmap);
    DrawBeasts(pixmap);
  }
  
  // Clear the update matrix
  _updatematrix.Clear();
  
  DrawTexts(widget, pixmap);
}

void CView::DrawBoard(GtkWidget* widget, GdkPixmap* pixmap)
{
  for (int x = 0; x < _world->_board.Width(); x++)
  {
    for (int y = 0; y < _world->_board.Height(); y++)
    {
      if (_updatematrix.Get(x, y))
      {
        int xpix = (x + 1) * TILEW + _display[1].x;
        int ypix = (y + 1) * TILEH + _display[1].y;
        
        if (_world->_board.Get(x, y) == BOX)
        {
          _images.Draw(pixmap, IMG_BOX, xpix, ypix);
        }
        else
        {
          gdk_draw_rectangle (pixmap,
                widget->style->white_gc,
                TRUE,
                xpix, ypix,
                TILEW,
                TILEH);
        }
      }
    }
  }
}

void CView::DrawBeasts(GdkPixmap* pixmap)
{
  int i = 0;
  CBeast * beast;
  while((beast = _world->_beasts.GetItem(i++)) != NULL)
  {
    int xpix = (beast->GetX() + 1) * 20 + _display[1].x;
    int ypix = (beast->GetY() + 1) * 20 + _display[1].y;

    _images.Draw(pixmap, IMG_BEAST, xpix, ypix);
  }
}

void CView::DrawPlayer(GdkPixmap* pixmap)
{
  CPlayer *player = &_world->_player;

  int xpix = (player->GetX() + 1) * TILEW + _display[1].x;
  int ypix = (player->GetY() + 1) * TILEH + _display[1].y;
  
  int image;
  if (_world->Won())
    image = _world->GetWinFaceIndex();
  else
    image = _world->GetFaceIndex();
  _images.Draw(pixmap, image, xpix, ypix);
}

void CView::DrawTitleText(GtkWidget* widget, GdkPixmap* pixmap)
{
  PangoLayout *layout;
  PangoFontDescription *fontdesc;
  GdkGC* gc;

  int len;

  gc = widget->style->text_gc[GTK_STATE_ACTIVE];
  layout = gtk_widget_create_pango_layout (widget, "test");
  fontdesc = pango_font_description_from_string ("Luxi Mono 12");
  pango_layout_set_font_description(layout, fontdesc); // Null or specific font with pango_font_description?

  int x = 0, y = 0;
  int cx = 0, cy = 0;

  char szBuffer[128];

  len = sprintf(szBuffer, "Boxed");
  pango_layout_set_text (layout, szBuffer, len);
  pango_layout_get_pixel_size (layout, &cx, &cy);
  y = (_display[1].height - 2 * cy) / 2;
  x = (_display[1].width - cx) / 2;
  gdk_draw_layout(pixmap, gc, x, y, layout);

  len = sprintf(szBuffer, "©2004 Michael Schmitz");
  pango_layout_set_text (layout, szBuffer, len);
  pango_layout_get_pixel_size (layout, &cx, &cy);
  y += cy;
  x  = (_display[1].width - cx) / 2;
  gdk_draw_layout(pixmap, gc, x, y, layout);

  // Clean memory
  fontdesc = pango_font_description_from_string ("Luxi Mono 12");
  g_object_unref (layout);
}

void CView::DrawOutline(GdkPixmap* pixmap)
{
  gint x, y;
  gint x1 = _display[1].x, y1 = _display[1].y;
  gint width = _display[1].width, height = _display[1].height;

  y = y1;
  for (x = x1; x < width; x += TILEW)
  {
    _images.Draw(pixmap, IMG_BOX, x, y);
  }

  y = y1 + height - TILEH;
  for (x = x1; x < width; x += TILEW)
  {
    _images.Draw(pixmap, IMG_BOX, x, y);
  }

  x = x1;
  for (y = y1; y < height; y += TILEH)
  {
    _images.Draw(pixmap, IMG_BOX, x, y);
  }

  x = width - TILEW;
  for (y = y1; y < height; y += TILEH)
  {
    _images.Draw(pixmap, IMG_BOX, x, y);
  }
}

void CView::DrawTexts (GtkWidget* widget, GdkPixmap* pixmap)
{
  // Setup constants and destinations
  int x = 0, y = 0;

  char szTitle[128] = "";
  char szLower[128] = "";
  
  bool bCenter = true;

  if (_world->GetState() == INTRO)
  {
  }
  else if (_world->GetState() == GAME)
  {
    bool bHighscore = false;
    if (_world->Won())
    {
      if (_world->Highscore())
        bHighscore = true;

      if (bHighscore)
        sprintf(szLower, "New Highscore!  Press ENTER to continue.");//, _world->GetLevel(), _world->GetSeconds());      }
      else
        sprintf(szLower, "Level %i reached in %u seconds.  Press ENTER to continue.", _world->GetLevel(), _world->GetSeconds());
    }
    else if (_world->Lost())
    {
        sprintf(szLower, "Died on level %i in %u seconds.  Press ENTER to retry level.", _world->GetLevel(), _world->GetSeconds());
    }
    else
    {
      bCenter = false;

      sprintf(szLower, " Beasts: %i", _world->GetBeastNumber());
      sprintf(szLower, "%s     Beast Delay: %i", szLower, _world->GetBeastDelay());
      sprintf(szLower, "%s     Box Density: %i/100", szLower, _world->GetBoxDensity());
      sprintf(szLower, "%s     Level: %i", szLower, _world->GetLevel());
      sprintf(szLower, "%s     Time: %us", szLower, _world->GetSeconds());
    }

    sprintf(szTitle, "Highscore:  Level %i in %u seconds", _world->_highscore.iLevel, _world->_highscore.dwSeconds);
  }
  else if (_world->GetState() == ARCADE)
  {
    bCenter = false;
    
    sprintf(szLower, " Beasts: %i", _world->GetBeastNumber());
    sprintf(szLower, "%s       Beast Delay: %i", szLower, _world->GetBeastDelay());
    sprintf(szLower, "%s       Box Density: %i/100", szLower, _world->GetBoxDensity());
    sprintf(szLower, "%s       Time: %us", szLower, _world->GetSeconds());
  }

  PangoLayout *layout;
  PangoFontDescription *fontdesc;
  GdkGC* gc;
  
  int cx,cy,len;

  gc = widget->style->text_gc[GTK_STATE_ACTIVE];
  layout = gtk_widget_create_pango_layout (widget, "");
  fontdesc = pango_font_description_from_string ("Sans 11");
  pango_layout_set_font_description(layout, fontdesc); 

  // Draw title
  len = strlen(szTitle);
  pango_layout_set_text (layout, szTitle, len);
  pango_layout_get_pixel_size (layout, &cx, &cy);
  y = _display[0].y;
  x = (_display[0].width - cx) / 2;
  gdk_draw_layout(pixmap, gc, x, y, layout);
  
  // Draw bottom
  len = strlen(szLower);
  pango_layout_set_text (layout, szLower, len);
  pango_layout_get_pixel_size (layout, &cx, &cy);
  y = _display[2].y;
  
  if (bCenter)
    x = (_display[2].width - cx) / 2;
  else
    x = 0;
  
  gdk_draw_layout(pixmap, gc, x, y, layout);
}

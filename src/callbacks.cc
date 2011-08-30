#include "callbacks.h"
#include "interface.h"

#include "constant.h"
#include "view.h"
#include "world.h"

#include <gdk/gdkkeysyms.h>

gint beast_timer()
{
  if (!_world->IsPaused() && !_world->Lost() && !_world->Won())
    _world->RunBeastAI();
  else
    return FALSE;

  _view.Update(canvas, pixmap);
  gtk_widget_draw(canvas, NULL);
  
  return TRUE;
};

gint time_timer()
{
  if (!_world->IsPaused() && !_world->Lost() && !_world->Won())
    _world->AddSecond();
  else
    return FALSE;

  _view.Update(canvas, pixmap);
  gtk_widget_draw(canvas, NULL);
  
  return TRUE;
};

gboolean on_key_press( GtkWidget * widget,
                       GdkEventKey * event,
                       gpointer user_data )
{
  // keyval is guint

  if (event->keyval == GDK_Escape)
  {
    gtk_main_quit();
  }

  if (_world->KeyDown(event->keyval))
  {
    if (_world->IsPaused())
    {
      // Update variables and unpause
      gtk_timeout_add((int)(_world->_iBeastDelay/60.0*1000), (GtkFunction)beast_timer,0);
      gtk_timeout_add(1000, (GtkFunction)time_timer,0);
      _world->AddSecond();
      _world->RunBeastAI();
      _world->Unpause();
    }

/*
    // Reset winktimer
    _winktimer.Set(WINK_INTERVAL);

    UpdateScreen();
*/
    
    _view.Update(canvas, pixmap);
    gtk_widget_draw(canvas, NULL);

    return true;
  }
  else if ((_world->Won() || _world->Lost()) && _world->GetState() == GAME)
  {
    if (event->keyval == GDK_Return)
    {
      if (_world->Won())
        _world->CreateLevel();

      if (_world->Lost())
        _world->CreatePlayer();
      
      _world->New();
      _world->Pause();

      // _displaytimer.Set(1000);
      
      // _timer.Set((int)(_world->_iBeastDelay/60.0 * 1000.0));
      // gtk_timeout_add((int)(_world->_iBeastDelay/60*1000), (GtkFunction)beast_timer,0);

      _view._updatematrix.Fill();
      _view.Update(canvas, pixmap);
      gtk_widget_draw(canvas, NULL);
    }
  } 
  else if (event->keyval == GDK_p)
  {
    _world->Pause();
  }
  
  return false;
}

void on_help_about ( GtkMenuItem *menuitem,
          gpointer user_data )
{
  create_about ();
}

void on_file_new ( GtkMenuItem *menuitem,
                   gpointer user_data )
{
  _world->NewGame();
  
  _world->Pause();

  _view.Update(canvas, pixmap);
  gtk_widget_draw(canvas, NULL);
}

void on_file_suicide ( GtkMenuItem *menuitem,
                       gpointer user_data )
{
  _world->Lose();
  
  _view.Update(canvas, pixmap);
  gtk_widget_draw(canvas, NULL);
}

/* Create a new backing pixmap of the appropriate size */
gboolean configure_event( GtkWidget         *widget,
                          GdkEventConfigure *event )
{
  if (pixmap)
    g_object_unref (pixmap);

  pixmap = gdk_pixmap_new (widget->window,
         widget->allocation.width,
         widget->allocation.height,
         -1);
  gdk_draw_rectangle (pixmap,
          widget->style->white_gc,
          TRUE,
          0, 0,
          widget->allocation.width,
          widget->allocation.height);

    _view.Size(widget->allocation.width, widget->allocation.height);
  _world->Size((widget->allocation.width)/TILEW - 2, (widget->allocation.height - DISPLAY_H)/TILEH - 2); // -2 for borders
  _world->AttachUpdateMatrix(&_view._updatematrix);
  
  _view.Update(widget, pixmap);

  return TRUE;
}

/* Redraw the screen from the backing pixmap */
gboolean expose_event( GtkWidget      *widget,
                       GdkEventExpose *event )
{
  gdk_draw_drawable (widget->window,
         widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
         pixmap,
         event->area.x, event->area.y,
         event->area.x, event->area.y,
         event->area.width, event->area.height);

  return FALSE;
}

void on_quit_activate ( GtkMenuItem *menuitem,
                        gpointer user_data )
{
  gtk_main_quit();

  // TODO: Classify parent
  delete _world;
}

void destroy( GtkWidget *widget,
              gpointer   data )
{
    gtk_main_quit ();
  
  // TODO: Classify parent
  delete _world;
}

#ifndef _H_VIEW
#define _H_VIEW

#include <gtk/gtk.h>
#include "updatematrix.h"

class CView
{
public:
  void Update(GtkWidget* widget, GdkPixmap* pixmap);  
  void Size(int width, int height);
  
  CUpdateMatrix _updatematrix;

private:
  void DrawOutline(GdkPixmap* pixmap);
  void DrawBoard(GtkWidget* widget, GdkPixmap* pixmap);
  void DrawBeasts(GdkPixmap* pixmap);
  void DrawPlayer(GdkPixmap* pixmap);

  void DrawTitleText(GtkWidget* widget, GdkPixmap* pixmap);
  void DrawTexts (GtkWidget* widget, GdkPixmap* pixmap);

  GdkRectangle _display[3];
  GdkRectangle _client;
};

extern CView _view;

#endif

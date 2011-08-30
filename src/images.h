#ifndef _H_IMAGES
#define _H_IMAGES

#include "constant.h"

#include <gdk/gdk.h>

#define IMG_BOX     0
#define IMG_BEAST   1
#define IMG_PLAYER  2

#define IMG_FACE_NORMAL  2
#define IMG_FACE_COOL  3
#define IMG_FACE_WINK   4

#define IMG_WIN_SMILE    5
#define IMG_WIN_LAUGH    6
#define IMG_WIN_CHUCKLE  7
#define IMG_WIN_BIGSMILE 8
#define IMG_WIN_RELIEVED 9
#define IMG_WIN_COOL     10
#define IMG_WIN_SMOKER   11

extern class CImages
{
public:
  CImages() { Load(); }

  void Load();
  void Draw(GdkPixmap* pixmap, int index, int x, int y);
  
  GdkPixbuf* _pixicon;

private:
  GdkPixbuf* _pixbuf[12];
} _images;

#endif

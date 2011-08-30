#include "images.h"

#include <gtk/gtk.h>
#include "imagedata.h"

CImages _images;

void CImages::Load()
{
  // I don't know why...
  g_type_init(); 

  _pixbuf[IMG_BOX] = gdk_pixbuf_new_from_inline(-1,imgbox,FALSE,NULL);
  _pixbuf[IMG_BEAST]  = gdk_pixbuf_new_from_inline(-1,imgbeast,FALSE,NULL);
  
  _pixbuf[IMG_FACE_NORMAL] = gdk_pixbuf_new_from_inline(-1,imgplayer,FALSE,NULL);
  _pixbuf[IMG_FACE_COOL]   = gdk_pixbuf_new_from_inline(-1,imgcool,FALSE,NULL);
  _pixbuf[IMG_FACE_WINK]   = gdk_pixbuf_new_from_inline(-1,imgwink,FALSE,NULL);
  
  _pixbuf[IMG_WIN_SMILE]    = gdk_pixbuf_new_from_inline(-1,imgsmile,FALSE,NULL);
  _pixbuf[IMG_WIN_LAUGH]    = gdk_pixbuf_new_from_inline(-1,imglaugh,FALSE,NULL);
  _pixbuf[IMG_WIN_CHUCKLE]  = gdk_pixbuf_new_from_inline(-1,imgchuckle,FALSE,NULL);
  _pixbuf[IMG_WIN_BIGSMILE] = gdk_pixbuf_new_from_inline(-1,imgbigsmile,FALSE,NULL);
  _pixbuf[IMG_WIN_RELIEVED] = gdk_pixbuf_new_from_inline(-1,imgrelieved,FALSE,NULL);
  _pixbuf[IMG_WIN_COOL]     = gdk_pixbuf_new_from_inline(-1,imgwincool,FALSE,NULL);
  _pixbuf[IMG_WIN_SMOKER]   = gdk_pixbuf_new_from_inline(-1,imgwinsmoke,FALSE,NULL);
  
  _pixicon = gdk_pixbuf_new_from_inline(-1,imgicon,FALSE,NULL);
}

void CImages::Draw(GdkPixmap* pixmap, int index, int x, int y)
{
  gdk_pixbuf_render_to_drawable(_pixbuf[index],
                                pixmap,
                                NULL,
                                0, 0,
                                x, y,
                                TILEW, TILEH,
                                GDK_RGB_DITHER_NORMAL,
                                0, 0);
}

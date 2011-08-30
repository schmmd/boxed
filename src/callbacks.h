#include <gtk/gtk.h>

gboolean configure_event( GtkWidget         *widget,
                          GdkEventConfigure *event );

gboolean expose_event( GtkWidget      *widget,
                       GdkEventExpose *event );

void on_file_new ( GtkMenuItem *menuitem,
                   gpointer user_data );
           
void on_file_suicide ( GtkMenuItem *menuitem,
                       gpointer user_data );
           
void on_help_about ( GtkMenuItem *menuitem,
                   gpointer user_data );

void on_quit_activate (GtkMenuItem *menuitem,
                       gpointer user_data);

void destroy( GtkWidget *widget,
              gpointer   data );

gboolean on_key_press( GtkWidget * widget,
                       GdkEventKey * event,
                       gpointer user_data );

gint beast_timer();
gint time_timer();

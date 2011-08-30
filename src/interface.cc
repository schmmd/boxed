#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gnome.h>

#include "images.h"

#include "callbacks.h"
#include "interface.h"

#include "constant.h"

GtkWidget* create_pixmap  (GtkWidget       *widget, const gchar     *filename);

GdkPixmap *pixmap = NULL;
GtkWidget *canvas;

GtkWidget*
create_window ()
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *menubar;
  GtkWidget *menu_file;
  GtkWidget *menu1;
  GtkWidget *file_new, *file_suicide;
  GtkWidget *separatormenu_file;
  GtkWidget *file_quit;
  GtkWidget *menu_help;
  GtkWidget *menu4;
  GtkWidget *menu_help_about;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Boxed");

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  menubar = gtk_menu_bar_new ();
  gtk_widget_show (menubar);
  gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);

  menu_file = gtk_menu_item_new_with_mnemonic ("_File");
  gtk_widget_show (menu_file);
  gtk_container_add (GTK_CONTAINER (menubar), menu_file);

  menu1 = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_file), menu1);

  file_new = gtk_image_menu_item_new_from_stock ("gtk-new", accel_group);
  gtk_widget_show (file_new);
  gtk_container_add (GTK_CONTAINER (menu1), file_new);
  
  file_suicide = gtk_menu_item_new_with_mnemonic ("_Suicide");
  gtk_widget_show (file_suicide);
  gtk_container_add (GTK_CONTAINER (menu1), file_suicide);

  separatormenu_file = gtk_separator_menu_item_new ();
  gtk_widget_show (separatormenu_file);
  gtk_container_add (GTK_CONTAINER (menu1), separatormenu_file);
  gtk_widget_set_sensitive (separatormenu_file, FALSE);

  file_quit = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (file_quit);
  gtk_container_add (GTK_CONTAINER (menu1), file_quit);

  menu_help = gtk_menu_item_new_with_mnemonic ("_Help");
  gtk_widget_show (menu_help);
  gtk_container_add (GTK_CONTAINER (menubar), menu_help);

  menu4 = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_help), menu4);

  menu_help_about = gtk_menu_item_new_with_mnemonic ("_About");
  gtk_widget_show (menu_help_about);
  gtk_container_add (GTK_CONTAINER (menu4), menu_help_about);

  canvas = gtk_drawing_area_new ();
  gtk_widget_set_size_request (canvas, CANVAS_W, CANVAS_H);
  gtk_widget_show (canvas);
  gtk_box_pack_start (GTK_BOX (vbox), canvas, TRUE, TRUE, 0);

  /* Signals used to handle backing pixmap */

  g_signal_connect (G_OBJECT (canvas), "expose_event",
        G_CALLBACK (expose_event), NULL);
  g_signal_connect (G_OBJECT (canvas),"configure_event",
        G_CALLBACK (configure_event), NULL);

  /* Other signals */

  g_signal_connect ( G_OBJECT (window), "destroy",
                 G_CALLBACK (destroy), NULL);

  g_signal_connect ((gpointer) file_quit, "activate",
                    G_CALLBACK (on_quit_activate),
                    NULL);

  g_signal_connect ((gpointer) file_new, "activate",
                    G_CALLBACK (on_file_new),
                    NULL);
          
  g_signal_connect ((gpointer) file_suicide, "activate",
                    G_CALLBACK (on_file_suicide),
                    NULL);
          
  g_signal_connect ((gpointer) menu_help_about, "activate",
                    G_CALLBACK (on_help_about),
                    NULL);

  g_signal_connect ( G_OBJECT (window), "key_press_event",
                     G_CALLBACK (on_key_press), NULL);

  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  gtk_window_set_icon(GTK_WINDOW(window), _images._pixicon);   
  
   // Make unresizable
   gtk_window_set_policy(GTK_WINDOW(window),
                                         FALSE, /* allow shrink
*/
                                          FALSE, /* allow grow */
                                          FALSE /* whatever BOOL value you want for the auto shrink param */
        );

  return window;
}

void create_about ()
{
  gchar* szName = "Boxed";
  gchar* szVersion = "1.00";
  gchar* szCopyright = "Copyright (c) 2005 Michael Schmitz";
  gchar* szComments = "Developed at http://www.schmtiztech.com/.";

  const gchar *authors[] = 
  {
    "Michael Schmitz <michael@schmitztech.com>",
    NULL
  };
  const gchar *documenters[] = 
  {
    NULL
  };
  const gchar *translator_credits =
  {
    NULL
  };
   
  GtkWidget* about = gnome_about_new(szName, szVersion, szCopyright, szComments, authors, documenters, translator_credits, _images._pixicon);
  // gtk_window_set_transient_for (GTK_WINDOW (about), parent);
  gtk_window_set_icon(GTK_WINDOW(about), _images._pixicon);
  gtk_widget_show(about);
  
  g_signal_connect (G_OBJECT (about), "destroy", G_CALLBACK (gtk_widget_destroyed), &about);
}

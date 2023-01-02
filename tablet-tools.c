#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int alt = 0;
int shift = 0;

void toggle_shift (GtkWidget *button,
             gpointer   data)
{
  GtkWidget *label = gtk_button_get_child(GTK_BUTTON(button));
  const char *labelText = gtk_button_get_label(GTK_BUTTON(button));
  printf("labelText = %c\n",labelText);
  printf("labelText = %d,Stored as integer\n", labelText);
  // need to somehow check label or something to see which action to take to generalize this function

  PangoAttribute *textColor;
  if (shift == 0) {
    g_print ("Shift Keydown\n");
    system ("xdotool keydown Shift_L");
    shift = 1;
    textColor = pango_attr_foreground_new(65535,0,0);
  } else {
    g_print ("Shift Keyup\n");
    system ("xdotool keyup Shift_L");
    shift = 0;
    textColor = pango_attr_foreground_new(0,0,0);
  }
  PangoAttribute *const sizeAttr = pango_attr_size_new(60*PANGO_SCALE);
  PangoAttrList *const attrs = pango_attr_list_new();
  pango_attr_list_insert(attrs, textColor);
  pango_attr_list_insert(attrs, sizeAttr);
  gtk_label_set_attributes((GtkLabel *)label, attrs);
  pango_attr_list_unref(attrs);
}

void toggle_alt (GtkWidget *button,
             gpointer   data)
{
  GtkWidget *label = gtk_button_get_child(GTK_BUTTON(button));
  const char *labelText = gtk_button_get_label(GTK_BUTTON(button));
  printf("labelText = %c\n",labelText);
  printf("labelText = %d,Stored as integer\n", labelText);

  PangoAttribute *textColor;
  if (alt == 0) {
    g_print ("Alt Keydown\n");
    system ("xdotool keydown alt");
    alt = 1;
    textColor = pango_attr_foreground_new(65535,0,0);
  } else {
    g_print ("Alt Keyup\n");
    system ("xdotool keyup alt");
    alt = 0;
    textColor = pango_attr_foreground_new(0,0,0);
  }
  PangoAttribute *const sizeAttr = pango_attr_size_new(60*PANGO_SCALE);
  PangoAttrList *const attrs = pango_attr_list_new();
  pango_attr_list_insert(attrs, textColor);
  pango_attr_list_insert(attrs, sizeAttr);
  gtk_label_set_attributes((GtkLabel *)label, attrs);
  pango_attr_list_unref(attrs);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *altButton;
  GtkWidget *shiftButton;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "TabletTools");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 860);

  grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid),10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 6);

  altButton = gtk_button_new_with_label ("Alt");
  shiftButton = gtk_button_new_with_label ("Shift");

  gtk_grid_attach(GTK_GRID(grid), altButton, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), shiftButton, 0, 2, 1, 1);

  GtkWidget *altLabel = gtk_button_get_child(GTK_BUTTON(altButton));
  GtkWidget *shiftLabel = gtk_button_get_child(GTK_BUTTON(shiftButton));
  PangoAttribute *textColor = pango_attr_foreground_new(0, 0, 0);
  PangoAttribute *const sizeAttr = pango_attr_size_new(60*PANGO_SCALE);
  PangoAttrList *const attrs = pango_attr_list_new();
  pango_attr_list_insert(attrs, sizeAttr);
  pango_attr_list_insert(attrs, textColor);
  gtk_label_set_attributes((GtkLabel *)altLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)shiftLabel, attrs);
  pango_attr_list_unref(attrs);

  g_signal_connect (altButton, "clicked", G_CALLBACK (toggle_alt), NULL);
  g_signal_connect (shiftButton, "clicked", G_CALLBACK (toggle_shift), NULL);
  gtk_window_set_child (GTK_WINDOW (window), grid);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

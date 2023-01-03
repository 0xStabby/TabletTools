#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int alt = 0;
int shift = 0;
int rightClick = 0;
guint threadID = 0;

void switch_clicks_back (gpointer button)
{
  g_print ("Right Click off\n");
  system ("xmodmap -e 'pointer = 1 2 3'");
  GtkWidget *label = gtk_button_get_child(GTK_BUTTON(button));
  PangoAttribute *textColor = pango_attr_foreground_new(0,0,0);
  PangoAttribute *const sizeAttr = pango_attr_size_new(60*PANGO_SCALE);
  PangoAttrList *const attrs = pango_attr_list_new();
  pango_attr_list_insert(attrs, textColor);
  pango_attr_list_insert(attrs, sizeAttr);
  gtk_label_set_attributes((GtkLabel *)label, attrs);
  pango_attr_list_unref(attrs);
  rightClick = 0;
  g_source_remove(threadID);
  threadID = 0;
}

void click_button (GtkWidget *button, gpointer data)
{
  GtkWidget *label = gtk_button_get_child(GTK_BUTTON(button));
  const char *labelStr = gtk_button_get_label(GTK_BUTTON(button));
  //const char *labelStr = label;
  printf("label = %s\n", labelStr);
  //printf("label = %s\n", labelTest);
  PangoAttribute *textColor;
  
  int colorButton = 0;
  if (strcmp(labelStr, "Click") == 0) { // right click
    if (rightClick == 0) {
      colorButton = 1;
      g_print ("Right Click on\n");
      system ("xmodmap -e 'pointer = 3 2 1'");
      rightClick = 1;
      textColor = pango_attr_foreground_new(65535,0,0);
      threadID = g_timeout_add_seconds(1, switch_clicks_back, button);
    }
  } else if (strcmp(labelStr, "Shift") == 0) { // shift
    colorButton = 1;
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
  } else if (strcmp(labelStr, "Alt") == 0) { // alt
    colorButton = 1;
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
  } else if (strcmp(labelStr, "Undo") == 0) { // ctrl+z
      g_print ("Undo\n");
      system ("xdotool search --class Blender windowactivate --sync %1 key ctrl+z");
  }

  if (colorButton == 1) {
    PangoAttribute *const sizeAttr = pango_attr_size_new(60*PANGO_SCALE);
    PangoAttrList *const attrs = pango_attr_list_new();
    pango_attr_list_insert(attrs, textColor);
    pango_attr_list_insert(attrs, sizeAttr);
    gtk_label_set_attributes((GtkLabel *)label, attrs);
    pango_attr_list_unref(attrs);
  }
  colorButton = 0;
}

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *altButton;
  GtkWidget *shiftButton;
  GtkWidget *rightClickButton;
  GtkWidget *undoButton;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "TabletTools");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 969); // 969 for full hight

  grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid),10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 6);

  altButton = gtk_button_new_with_label ("Alt");
  shiftButton = gtk_button_new_with_label ("Shift");
  rightClickButton = gtk_button_new_with_label ("Click");
  undoButton = gtk_button_new_with_label ("Undo");

  gtk_grid_attach(GTK_GRID(grid), altButton, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), shiftButton, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rightClickButton, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), undoButton, 0, 6, 1, 1);

  GtkWidget *altLabel = gtk_button_get_child(GTK_BUTTON(altButton));
  GtkWidget *shiftLabel = gtk_button_get_child(GTK_BUTTON(shiftButton));
  GtkWidget *rightClickLabel = gtk_button_get_child(GTK_BUTTON(rightClickButton));
  GtkWidget *undoLabel = gtk_button_get_child(GTK_BUTTON(undoButton));

  PangoAttribute *textColor = pango_attr_foreground_new(0, 0, 0);
  PangoAttribute *const sizeAttr = pango_attr_size_new(60*PANGO_SCALE);
  PangoAttrList *const attrs = pango_attr_list_new();

  pango_attr_list_insert(attrs, sizeAttr);
  pango_attr_list_insert(attrs, textColor);

  gtk_label_set_attributes((GtkLabel *)altLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)shiftLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)rightClickLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)undoLabel, attrs);
  pango_attr_list_unref(attrs);

  g_signal_connect (altButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (shiftButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (rightClickButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (undoButton, "clicked", G_CALLBACK (click_button), NULL);
  gtk_window_set_child (GTK_WINDOW (window), grid);

  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

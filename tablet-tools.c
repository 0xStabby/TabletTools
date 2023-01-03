#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int alt = 0;
int shift = 0;
int rightClick = 0;
guint threadID = 0;
int *windowName;

void bind_window (gpointer button) {
  system("chmod +x bind-window.sh");

  FILE *cmd=popen("./bind-window.sh", "r");
  char result[24]={0x0};
  while (fgets(result, sizeof(result), cmd) !=NULL) {
    printf("%s\n", result);
  }
  pclose(cmd);
}

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
  } else if (strcmp(labelStr, "Copy") == 0) { // ctrl+c  
      g_print ("Copy\n");
      char command[100];
      strcmp(command, "");
      strcmp(command, "");
      system ("xdotool search --class Blender windowactivate --sync %1 key ctrl+c");
  } else if (strcmp(labelStr, "Paste") == 0) { // ctrl+v
      g_print ("Paste\n");
      system ("xdotool search --class Blender windowactivate --sync %1 key ctrl+v");
  } else if (strcmp(labelStr, "Undo") == 0) { // ctrl+z
      g_print ("Undo\n");
      system ("xdotool search --class Blender windowactivate --sync %1 key ctrl+z");
  } else if (strcmp(labelStr, "Redo") == 0) { // shift+ctrl+z
      g_print ("Redo\n");
      system ("xdotool search --class Blender windowactivate --sync %1 key Shift_L+ctrl+z");
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
  GtkWidget *copyButton;
  GtkWidget *pasteButton;
  GtkWidget *undoButton;
  GtkWidget *redoButton;
  GtkWidget *bindButton;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "TabletTools");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 969);

  grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid),10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 6);

  altButton = gtk_button_new_with_label ("Alt");
  shiftButton = gtk_button_new_with_label ("Shift");
  rightClickButton = gtk_button_new_with_label ("Click");
  copyButton = gtk_button_new_with_label ("Copy");
  pasteButton = gtk_button_new_with_label ("Paste");
  undoButton = gtk_button_new_with_label ("Undo");
  redoButton = gtk_button_new_with_label ("Redo");
  bindButton = gtk_button_new_with_label("Bind");

  gtk_grid_attach(GTK_GRID(grid), altButton, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), shiftButton, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rightClickButton, 0, 3, 1, 1);
  //gtk_grid_attach(GTK_GRID(grid), copyButton, 0, 4, 1, 1);
  //gtk_grid_attach(GTK_GRID(grid), pasteButton, 0, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), undoButton, 0, 6, 1, 1);
  //gtk_grid_attach(GTK_GRID(grid), redoButton, 0, 7, 1, 1);
  //gtk_grid_attach(GTK_GRID(grid), bindButton, 0, 8, 1, 1);

  GtkWidget *altLabel = gtk_button_get_child(GTK_BUTTON(altButton));
  GtkWidget *shiftLabel = gtk_button_get_child(GTK_BUTTON(shiftButton));
  GtkWidget *rightClickLabel = gtk_button_get_child(GTK_BUTTON(rightClickButton));
  GtkWidget *copyLabel = gtk_button_get_child(GTK_BUTTON(copyButton));
  GtkWidget *pasteLabel = gtk_button_get_child(GTK_BUTTON(pasteButton));
  GtkWidget *undoLabel = gtk_button_get_child(GTK_BUTTON(undoButton));
  GtkWidget *redoLabel = gtk_button_get_child(GTK_BUTTON(redoButton));
  GtkWidget *bindLabel = gtk_button_get_child(GTK_BUTTON(bindButton));

  PangoAttribute *textColor = pango_attr_foreground_new(0, 0, 0);
  PangoAttribute *const sizeAttr = pango_attr_size_new(60*PANGO_SCALE);
  PangoAttrList *const attrs = pango_attr_list_new();

  pango_attr_list_insert(attrs, sizeAttr);
  pango_attr_list_insert(attrs, textColor);

  gtk_label_set_attributes((GtkLabel *)altLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)shiftLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)rightClickLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)copyLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)pasteLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)undoLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)redoLabel, attrs);
  gtk_label_set_attributes((GtkLabel *)bindLabel, attrs);
  pango_attr_list_unref(attrs);

  g_signal_connect (altButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (shiftButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (rightClickButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (copyButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (pasteButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (undoButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (pasteButton, "clicked", G_CALLBACK (click_button), NULL);
  g_signal_connect (bindButton, "clicked", G_CALLBACK (bind_window), NULL);
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

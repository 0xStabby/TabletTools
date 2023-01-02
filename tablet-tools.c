#include <gtk/gtk.h>

int alt = 0;

void toggle_alt (GtkWidget *button,
             gpointer   data)
{
  GtkWidget *label = gtk_button_get_child(GTK_BUTTON(button));
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
  PangoAttribute *const sizeAttr = pango_attr_size_new(72*PANGO_SCALE);
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
  GtkWidget *button;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "TabletTools");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 860);

  button = gtk_button_new_with_label ("Alt");

  GtkWidget *label = gtk_button_get_child(GTK_BUTTON(button));
  PangoAttribute *textColor = pango_attr_foreground_new(0, 0, 0);
  PangoAttribute *const sizeAttr = pango_attr_size_new(72*PANGO_SCALE);
  PangoAttrList *const attrs = pango_attr_list_new();
  pango_attr_list_insert(attrs, sizeAttr);
  pango_attr_list_insert(attrs, textColor);
  gtk_label_set_attributes((GtkLabel *)label, attrs);
  pango_attr_list_unref(attrs);

  g_signal_connect (button, "clicked", G_CALLBACK (toggle_alt), NULL);
  gtk_window_set_child (GTK_WINDOW (window), button);

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

#include<gtk/gtk.h>

int main(int argc, char *argv[]) {
  gtk_init();
  gboolean done = FALSE;

  GtkWidget *window;
  window = gtk_window_new();
  gtk_widget_show(window);

  while (!done) {
    g_main_context_iteration(NULL, TRUE);
  }
  return 0;
}

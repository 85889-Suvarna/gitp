#include <gtk/gtk.h>
#include <stdlib.h>

void quit_app(GtkWidget *widget, gpointer data) {
  gboolean *done = data;
  *done = TRUE;

  g_main_context_wakeup(NULL);
}

int main(int argc, char *argv[])
{
  gboolean done = FALSE;
  gtk_init();

  /* Create a new window */
  GtkWidget *window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(window), "Text View with Scrollbar Example");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  g_signal_connect(window, "destroy", G_CALLBACK(quit_app), &done);

  /* Create a new GtkFixed container */
  GtkWidget *fixed = gtk_fixed_new();

  /* Create a GtkScrolledWindow */
  GtkWidget *scrolled_window = gtk_scrolled_window_new();
  gtk_widget_set_size_request(scrolled_window, 350, 250);

  /* Create a GtkTextView */
  GtkWidget *text_view = gtk_text_view_new();
  
  /* Load the contents of a file into the GtkTextView */
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  
  /* Assuming you have a file named "example.txt" in the current directory */
  gchar *contents;
  g_file_get_contents("file.txt", &contents, NULL, NULL);
  gtk_text_buffer_set_text(buffer, contents, -1);
  g_free(contents);

  /* Add the text view to the scrolled window */
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_view);

  /* Add the scrolled window to the fixed container at coordinates (25, 25) */
  gtk_fixed_put(GTK_FIXED(fixed), scrolled_window, 25, 25);

  /* Add the fixed container to the window */
  gtk_window_set_child(GTK_WINDOW(window), fixed);

  /* Show the window */
  gtk_widget_show(window);

  while(!done) {
    g_main_context_iteration(NULL, TRUE);
  }
  return EXIT_SUCCESS;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
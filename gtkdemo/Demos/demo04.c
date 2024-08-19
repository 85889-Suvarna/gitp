#include <gtk/gtk.h>

void quit_app(GtkWidget *widget, gpointer data) {
  gboolean *done = data;
  *done = TRUE;

  g_main_context_wakeup(NULL);
}

/* Callback function for ListBox row selection */
static void listbox_row_selected(GtkListBox *box, GtkListBoxRow *row, gpointer user_data)
{
  const gchar *text;
  GtkLabel *label;

  /* Get the label from the selected row */
  label = GTK_LABEL(gtk_list_box_row_get_child(row));
  
  /* Get the text from the label */
  text = gtk_label_get_text(label);

  g_print("Selected: %s\n", text);
}

int main(int argc, char *argv[])
{
  gboolean done = FALSE;
  gtk_init();

  /* Create a new window */
  GtkWidget *window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(window), "ListBox Example");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
  g_signal_connect(window, "destroy", G_CALLBACK(quit_app), &done);

  /* Create a new GtkFixed container */
  GtkWidget *fixed = gtk_fixed_new();

  /* Create a new GtkListBox */
  GtkWidget *list_box = gtk_list_box_new();
  
  /* Add items to the list box */
  for (int i = 0; i < 5; i++)
  {
    gchar *label_text = g_strdup_printf("Item %d", i + 1);
    GtkWidget *label = gtk_label_new(label_text);
    GtkWidget *row = gtk_list_box_row_new();
    
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
    gtk_list_box_append(GTK_LIST_BOX(list_box), row);
    
    g_free(label_text);
  }

  /* Connect the "row-selected" signal of the list box to the callback function */
  g_signal_connect(list_box, "row-selected", G_CALLBACK(listbox_row_selected), NULL);

  /* Add the list box to the fixed container at coordinates (50, 50) */
  gtk_fixed_put(GTK_FIXED(fixed), list_box, 50, 50);

  /* Add the fixed container to the window */
  gtk_window_set_child(GTK_WINDOW(window), fixed);

  /* Show the window */
  gtk_widget_show(window);

  while(!done) {
    g_main_context_iteration(NULL, TRUE);
  }

  return EXIT_SUCCESS;
}

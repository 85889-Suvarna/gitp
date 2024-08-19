#include <gtk/gtk.h>

// Callback function to handle file selection
static void on_open_response(GtkNativeDialog *native, int response, gpointer user_data) {
    if (response == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
        GListModel *files = gtk_file_chooser_get_files(chooser);

        guint n_items = g_list_model_get_n_items(files);
        for (guint i = 0; i < n_items; i++) {
            GFile *file = G_FILE(g_list_model_get_item(files, i));
            char *file_path = g_file_get_path(file);
            g_print("File selected: %s\n", file_path);
            g_free(file_path);
            g_object_unref(file);
        }
        g_object_unref(files);
    }
    g_object_unref(native);
}

// Callback function to handle menu item activation
static void on_open_menu_item_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);

    GtkFileChooserNative *native = gtk_file_chooser_native_new("Open File",
                                                               GTK_WINDOW(window),
                                                               GTK_FILE_CHOOSER_ACTION_OPEN,
                                                               "_Open",
                                                               "_Cancel");

    g_signal_connect(native, "response", G_CALLBACK(on_open_response), NULL);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(native));
}

// Callback function to handle list box row selection
static void on_list_box_row_selected(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
    if (row != NULL) {
        const gchar *text;
        GtkLabel *label;

        /* Get the label from the selected row */
        label = GTK_LABEL(gtk_list_box_row_get_child(row));
        
        /* Get the text from the label */
        text = gtk_label_get_text(label);

        g_print("Selected: %s\n", text);
    }
}

// Function to set up the UI when the application is activated
static void on_activate(GtkApplication *app) {
    // Create a new window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "IDE");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create a header bar
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    // Create a menu button and add it to the header bar
    GtkWidget *menu_button = gtk_menu_button_new();
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), menu_button);

    // Create a menu model
    GMenu *menu = g_menu_new();

    // Create a file menu and add items to it
    GMenu *file_menu = g_menu_new();
    g_menu_append(file_menu, "Open", "app.open");
    g_menu_append_section(menu, "File", G_MENU_MODEL(file_menu));

    // Set the menu model to the menu button
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_button), G_MENU_MODEL(menu));

    // Create actions for the menu items
    GSimpleAction *open_action = g_simple_action_new("open", NULL);
    g_signal_connect(open_action, "activate", G_CALLBACK(on_open_menu_item_activated), window);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(open_action));

    // Create a fixed layout container
    GtkWidget *fixed = gtk_fixed_new();

    // Create a scrolled window for the text view
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window, 590, 550);

    // Create a text view and add it to the scrolled window
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_widget_set_hexpand(text_view, TRUE);
    gtk_widget_set_vexpand(text_view, TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_view);

    // Create a list box and add some items to it
    GtkWidget *list_box = gtk_list_box_new();
    gtk_widget_set_size_request(list_box, 200, -1);
    for (int i = 1; i <= 10; i++) {
        char item_text[16];
        snprintf(item_text, sizeof(item_text), "Item %d", i);
        GtkWidget *label = gtk_label_new(item_text);
        GtkWidget *row = gtk_list_box_row_new();
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
        gtk_list_box_append(GTK_LIST_BOX(list_box), row);
    }

    // Connect the signal for list box row selection
    g_signal_connect(list_box, "row-selected", G_CALLBACK(on_list_box_row_selected), NULL);

    // Add the fixed layout to the window
    gtk_window_set_child(GTK_WINDOW(window), fixed);

    // Add the list box and scrolled window to the fixed layout
    gtk_fixed_put(GTK_FIXED(fixed), list_box, 0, 0);
    gtk_fixed_put(GTK_FIXED(fixed), scrolled_window, 205, 0);

    // Show all widgets in the window
    gtk_widget_show(window);
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init();

    // Create a new application
    GtkApplication *app = gtk_application_new("com.example.GtkApp", G_APPLICATION_FLAGS_NONE);

    // Connect the activate signal of the application to the callback function
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Run the application
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Clean up
    g_object_unref(app);

    return status;
}

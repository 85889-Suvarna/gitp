#include <gtk/gtk.h>

// Callback function to handle menu item activation
static void on_menu_item_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    const gchar *name = g_action_get_name(G_ACTION(action));
    g_print("%s activated\n", name);
}

// Function to set up the UI when the application is activated
static void on_activate(GtkApplication *app) {
    // Create a new window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK4 Menubar with Fixed Layout");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

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
    g_menu_append(file_menu, "New", "app.new");
    g_menu_append(file_menu, "Open", "app.open");
    g_menu_append(file_menu, "Save", "app.save");
    g_menu_append_section(menu, "File", G_MENU_MODEL(file_menu));

    // Set the menu model to the menu button
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_button), G_MENU_MODEL(menu));

    // Create actions for the menu items
    GSimpleAction *new_action = g_simple_action_new("new", NULL);
    g_signal_connect(new_action, "activate", G_CALLBACK(on_menu_item_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(new_action));

    GSimpleAction *open_action = g_simple_action_new("open", NULL);
    g_signal_connect(open_action, "activate", G_CALLBACK(on_menu_item_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(open_action));

    GSimpleAction *save_action = g_simple_action_new("save", NULL);
    g_signal_connect(save_action, "activate", G_CALLBACK(on_menu_item_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(save_action));

    // Create a fixed layout container
    GtkWidget *fixed = gtk_fixed_new();

    // Add the fixed layout to the window
    gtk_window_set_child(GTK_WINDOW(window), fixed);

    // Create some widgets to add to the fixed layout
    GtkWidget *label = gtk_label_new("This is a fixed layout.");
    GtkWidget *button = gtk_button_new_with_label("Click Me");

    // Add widgets to the fixed layout at specific positions
    gtk_fixed_put(GTK_FIXED(fixed), label, 50, 50);
    gtk_fixed_put(GTK_FIXED(fixed), button, 300, 250);

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

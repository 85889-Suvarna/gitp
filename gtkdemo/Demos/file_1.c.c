#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

static void on_New_Project(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}

static void on_Open_File(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *fixed;
    GtkWidget *New_Project;
    GtkWidget *Open_File;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "C IDE");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);

    fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(window), fixed);

    // Button 1: New Project
    New_Project = gtk_button_new_with_label("New Project");
    gtk_fixed_put(GTK_FIXED(fixed), New_Project, 10, 10);
    gtk_widget_set_size_request(New_Project, 150, 30);
    g_signal_connect(New_Project, "clicked", G_CALLBACK(on_New_Project), "Inside New Project");

    // Button 2: Open File
    Open_File = gtk_button_new_with_label("Open File");
    gtk_fixed_put(GTK_FIXED(fixed), Open_File, 10, 50);
    gtk_widget_set_size_request(Open_File, 150, 30);
    g_signal_connect(Open_File, "clicked", G_CALLBACK(on_Open_File), "Inside Open File");

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

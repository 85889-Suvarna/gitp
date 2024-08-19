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
static void on_New_File(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}
static void on_Delete_File(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}
static void on_Build_Project(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}
static void on_Run_Project(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}
static void on_Clean_Project(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}
static void on_Debug_Project(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}
static void on_Memory_leakage(GtkWidget *widget, gpointer data) {
    g_print("CLICKED : %s\n", (char *)data);
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *fixed;
    GtkWidget *New_Project;
    GtkWidget *Open_File;
    GtkWidget *New_File;
    GtkWidget *Delete_File;
    GtkWidget *Build_Project;
    GtkWidget *Run_Project;
    GtkWidget *Clean_Project;
    GtkWidget *Debug_Project;
    GtkWidget *Memory_leakage;




    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "C IDE");
    gtk_window_set_default_size(GTK_WINDOW(window), 1300, 800);

    fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(window), fixed);

    // Button 1: New Project
    New_Project = gtk_button_new_with_label("New Project");
    gtk_fixed_put(GTK_FIXED(fixed), New_Project, 10, 10);
    gtk_widget_set_size_request(New_Project, 150, 30);
    g_signal_connect(New_Project, "clicked", G_CALLBACK(on_New_Project), "Inside New Project");

    // Button 2: Open File
    Open_File = gtk_button_new_with_label("Open File");
    gtk_fixed_put(GTK_FIXED(fixed), Open_File,170, 10);
    gtk_widget_set_size_request(Open_File, 150, 30);
    g_signal_connect(Open_File, "clicked", G_CALLBACK(on_Open_File), "Inside Open File");

    // button 3 : New file
    New_File = gtk_button_new_with_label("New File");
    gtk_fixed_put(GTK_FIXED(fixed), New_File, 330, 10);
    gtk_widget_set_size_request(New_File, 150, 30);
    g_signal_connect(New_File, "clicked", G_CALLBACK(on_New_File), "Inside new File");

    //button 4 :delete file
    Delete_File = gtk_button_new_with_label("Delete File");
    gtk_fixed_put(GTK_FIXED(fixed), Delete_File, 490, 10);
    gtk_widget_set_size_request(Delete_File, 150, 30);
    g_signal_connect(Delete_File, "clicked", G_CALLBACK(on_Delete_File), "Inside Delete File");

    //button 5 : build project
    Build_Project = gtk_button_new_with_label("Build Project");
    gtk_fixed_put(GTK_FIXED(fixed), Build_Project, 650, 10);
    gtk_widget_set_size_request(Build_Project, 150, 30);
    g_signal_connect(Build_Project, "clicked", G_CALLBACK(on_Build_Project), "Inside Build Project");

    //button 6 : run project
    Run_Project = gtk_button_new_with_label("Run Project");
    gtk_fixed_put(GTK_FIXED(fixed), Run_Project, 810, 10);
    gtk_widget_set_size_request(Run_Project, 150, 30);
    g_signal_connect(Run_Project, "clicked", G_CALLBACK(on_Run_Project), "Inside Run Project");

    //button 7 : clean project
    Clean_Project = gtk_button_new_with_label("Clean Project");
    gtk_fixed_put(GTK_FIXED(fixed), Clean_Project, 970, 10);
    gtk_widget_set_size_request(Clean_Project, 150, 30);
    g_signal_connect(Clean_Project, "clicked", G_CALLBACK(on_Clean_Project), "Inside Clean Project");

    //button 8 : debug project
    Debug_Project = gtk_button_new_with_label("Debug Project");
    gtk_fixed_put(GTK_FIXED(fixed), Debug_Project, 1130, 10);
    gtk_widget_set_size_request(Debug_Project, 150, 30);
    g_signal_connect(Debug_Project, "clicked", G_CALLBACK(on_Debug_Project), "Inside Debug Project");
    //button 9 : check memory leakage
    Memory_leakage = gtk_button_new_with_label("Memory leakage");
    gtk_fixed_put(GTK_FIXED(fixed), Memory_leakage, 1290, 10);
    gtk_widget_set_size_request(Memory_leakage, 150, 30);
    g_signal_connect(Memory_leakage, "clicked", G_CALLBACK(on_Memory_leakage), "Inside Memory leakage");

    



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

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

const char *dir_path;
const char *project_name;
char project_path[1024];
char outfile[40];
GtkWidget *entry1, *entry2, *fileName_entry;
char g_dirpath[40];
char g_projfile[20];
char currentfile[40];
GtkWidget *entry1;
GtkWidget *entry2;
GtkWidget *NewProject_window;
GtkWidget *OpenProject_window;
GtkWidget *NewFile_window;
GtkWidget *OpenFile_window;
GtkWindow *temp_window;

//========================================================================================
#define MAX_FILES 10
#define MAX_FILENAME_LEN 255

typedef struct {
    int num_files;
    char filenames[MAX_FILES][MAX_FILENAME_LEN];
} Project;

Project project;

//===============================================================================================
void run_executable(const char *executable, char *const argv[]) {
    char command[300];
    snprintf(command, sizeof(command), "./%s", executable);
    if (system(command) == -1) {
        perror("Failed to run executable");
        exit(1);
    }
}

void link_files(Project *project, const char *output) {
    char command[1024] = "gcc";
    for (int i = 0; i < project->num_files; i++) {
        if (strstr(project->filenames[i], ".c")) {
            char obj_file[MAX_FILENAME_LEN];
            strcpy(obj_file, project->filenames[i]);
            strcpy(strrchr(obj_file, '.'), ".o");
            strcat(command, " ");
            strcat(command, obj_file);
        }
    }
    strcat(command, " -o ");
    strcat(command, output);
    if (system(command) == -1) {
        perror("Failed to link files");
        exit(1);
    }
}

void compile_file(const char *filename) {
    char command[300];
    snprintf(command, sizeof(command), "gcc -c %s", filename);
    if (system(command) == -1) {
        perror("Failed to compile file");
        exit(1);
    }
}

void open_editor(const char *filename) {
    char command[300];
    snprintf(command, sizeof(command), "vim %s", filename);
    if (system(command) == -1) {
        perror("Failed to open editor");
        exit(1);
    }
}

gboolean check_if_project_exists(const char *project_path) {
    // Implement your logic to check if the project exists

    //snprintf(project_path, sizeof(project_path), "%s", project_name);

    if (g_file_test(project_path, G_FILE_TEST_IS_DIR)) {
        return TRUE;
    } else {
        return FALSE;
    }
}


void add_file(Project *project, const char *filename)
{
    //char Full_filename[50];
    if (project->num_files >= MAX_FILES) {
        printf("Maximum number of files reached\n");
        return;
    }
    strcpy(project->filenames[project->num_files], filename);

}

void load_project(const char *project_name, Project *project) {
    FILE *file = fopen(project_name, "rb");
    if (file == NULL) {
        perror("Failed to open project file");
        exit(1);
    }
    if (fread(project, sizeof(Project), 1, file) != 1) {
        perror("Failed to read project file");
        fclose(file);
        exit(1);
    }
    fclose(file);
}

void save_project(const char *project_name, Project *project) {
    FILE *file = fopen(project_name, "wb");
    if (file == NULL) {
        perror("Failed to open project file");
        exit(1);
    }
    if (fwrite(project, sizeof(Project), 1, file) != 1) {
        perror("Failed to write project file");
        fclose(file);
        exit(1);
    }
    fclose(file);
}

void create_project(const char *project_name) {
    //Project project = {0};
    save_project(project_name, &project);
}

void create_directory(const char *dirpath) {
    if (mkdir(dirpath) != 0 && errno != EEXIST) {
        perror("Failed to create directory");
        exit(1);
    }
}
//===============================================================================================
static void on_Open_File_Button(GtkWidget *button, gpointer data) {
    g_print("Inside open file button...\n");
   
    temp_window = GTK_WINDOW(data);
   
    char filename[40];
    strcpy(filename, gtk_editable_get_text(GTK_EDITABLE(fileName_entry)));
	printf("filename : %s \n",filename);

    if (access(filename, F_OK) == -1) {
    printf("File not found in project\n"); 
    }
    else{
        open_editor(filename);
    }

    gtk_window_destroy(temp_window);
}



static void on_Open_Project_Button(GtkWidget *button, gpointer data) {
   
    g_print("Inside open project button...\n");

    project_name = gtk_editable_get_text(GTK_EDITABLE(entry1));
    dir_path = gtk_editable_get_text(GTK_EDITABLE(entry2));

    snprintf(project_path, sizeof(project_path), "%s \%s", dir_path, project_name);
    g_print("Project is available at %s: %s\n", dir_path, project_name);

    // snprintf(project_path, sizeof(project_path), "%s", project_name);
    g_print("%s\n",project_path);
    
    // Implement your logic to check if the project exists
    gboolean project_exists = check_if_project_exists(project_path);

    if (project_exists) {
        g_print("Open project successful\n");
        load_project(project_path, &project);
    } else {
        g_print("Can't open project\n");
    }
    
}


static void on_Cancle_Button(GtkWidget *widget, gpointer data)
{
    temp_window = GTK_WINDOW(data);
    gtk_window_destroy(temp_window);
}

static void on_Create_File(GtkWidget *widget, gpointer data)
{
    g_print("CLICKED : Inside Create File button\n");
    temp_window = GTK_WINDOW(data);

    char filename[40];
	char cmd[60];
	int fd;

    //strcpy(filename,gtk_entry_get_text(GTK_ENTRY(fileName_entry)));
    strcpy(filename, gtk_editable_get_text(GTK_EDITABLE(fileName_entry)));

	printf("filename : %s \n",filename);
    
    add_file(&project,filename);
    project.num_files++;
    save_project(project_path, &project);
    open_editor(filename);
}


static void on_Create_Project(GtkWidget *widget, gpointer data)
{
    temp_window =  GTK_WINDOW(data);

    //g_print("CLICKED : %s\n",data);
    dir_path = gtk_editable_get_text(GTK_EDITABLE(entry1));
    project_name = gtk_editable_get_text(GTK_EDITABLE(entry2));

    // Combine directory path and project name to form the full path
    
    snprintf(project_path, sizeof(project_path), "%s \%s", dir_path, project_name);
    g_print("Project is created at %s: %s\n", dir_path, project_name);
   
    create_directory(dir_path);
    if (chdir(dir_path) != 0) {
        perror("Failed to change directory");
        exit(1);
    }
    snprintf(project_path, sizeof(project_path), "%s.proj", project_name);
    g_print("%s.proj",project_name);
    struct stat st;
    //Project project;

    if (stat(project_path, &st) != 0) {
        create_project(project_path);
    }
    load_project(project_path, &project);
    
    gtk_window_destroy(temp_window);
}

//==============================================================================================

static void on_New_Project(GtkWidget *widget, gpointer data) {
    g_print("%s\n", (char *)data);

    g_print("CLICKED : %s\n", (char *)data);
    GtkWidget *label1, *label2, *button, *button1;
    GtkWidget *fixed;

    NewProject_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(NewProject_window), "Create New Project");
    gtk_window_set_default_size(GTK_WINDOW(NewProject_window), 400, 200);

    // Create a fixed layout container
    fixed = gtk_fixed_new();
    //gtk_widget_set_margin_top(fixed, 20);
    gtk_window_set_child(GTK_WINDOW(NewProject_window), fixed);

    // Create the "Enter Project Directory" label and entry
    label1 = gtk_label_new("Enter Project Directory:");
    gtk_fixed_put(GTK_FIXED(fixed), label1, 10, 10);

    entry1 = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entry1, 180, 10);
    gtk_widget_set_size_request(entry1, 200, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry1), "Enter project path");


    // Create the "Project Name" label and entry
    label2 = gtk_label_new("Project Name:");
    gtk_fixed_put(GTK_FIXED(fixed), label2, 10, 50);

    entry2 = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entry2, 180, 50);
    gtk_widget_set_size_request(entry2, 200, -1);  // Set the width to 200 pixels
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry2), "Enter project name");

    // Create the "Create Project" button
    button = gtk_button_new_with_label("Create Project");
    gtk_fixed_put(GTK_FIXED(fixed), button, 50, 100);
    g_signal_connect(button, "clicked", G_CALLBACK(on_Create_Project), NewProject_window);

    // Create the "Cancel" button
    button1 = gtk_button_new_with_label("Cancel");
    gtk_fixed_put(GTK_FIXED(fixed), button1, 200, 100);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_Cancle_Button), NewProject_window);

    // Show the window
    gtk_window_present(GTK_WINDOW(NewProject_window));
}

static void on_Open_Project(GtkWidget *widget, gpointer data) {
    //g_print("%s\n", (char *)data);
    GtkWidget *label1, *label2, *entry, *button, *button1;
    GtkWidget *fixed;

    OpenProject_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(OpenProject_window), "Open Project");
    gtk_window_set_default_size(GTK_WINDOW(OpenProject_window), 400, 200);

    // Create a fixed layout container
    fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(OpenProject_window), fixed);

    // Create the "Enter Project Directory" label and entry
    label1 = gtk_label_new("Enter Project Directory:");
    gtk_fixed_put(GTK_FIXED(fixed), label1, 10, 10);

    entry1 = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entry1, 180, 10);
    gtk_widget_set_size_request(entry1, 200, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry1), "Enter project name");


    // Create the "Project Name" label and entry
    label2 = gtk_label_new("Enter Project path:");
    gtk_fixed_put(GTK_FIXED(fixed), label2, 10, 50);

    entry2 = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entry2, 180, 50);
    gtk_widget_set_size_request(entry2, 200, -1);  // Set the width to 200 pixels
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry2), "Enter Project Path:");

    // Create the "Create Project" button
    button = gtk_button_new_with_label("Open Project");
    gtk_fixed_put(GTK_FIXED(fixed), button, 150, 100);

    g_signal_connect(button, "clicked", G_CALLBACK(on_Open_Project_Button), NULL);

    // Show the window
    gtk_window_present(GTK_WINDOW(OpenProject_window));


}

static void on_Build_Project(GtkWidget *widget, gpointer data) {
    g_print("%s\n", (char *)data);
    printf("Building a project...\n");
    for (int i = 0; i < project.num_files; i++) 
    {
        if (strstr(project.filenames[i], ".c")) 
        {
            printf("This file will compiles %s ...\n",project.filenames);
            compile_file(project.filenames[i]);
        }
    }
    link_files(&project, project_name);

}

static void on_Run_Project(GtkWidget *widget, gpointer data) {
    g_print("%s\n", (char *)data);

    char cmd_args[1024];
    fgets(cmd_args, sizeof(cmd_args), stdin);  // consume newline
    fgets(cmd_args, sizeof(cmd_args), stdin);
    char *args[MAX_FILES + 1];
    args[0] = project_name;
    int i = 1;
    char *arg = strtok(cmd_args, " \n");
    while (arg && i < MAX_FILES) {
    args[i++] = arg;
    arg = strtok(NULL, " \n");
    }
    args[i] = NULL;
    run_executable(project_name, args);
}

static void on_Open_File(GtkWidget *widget, gpointer data) {
    g_print("%s\n", (char *)data);
    GtkWidget *label1, *button, *button1;
    GtkWidget *fixed;

    OpenFile_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(OpenFile_window), "Open File");
    gtk_window_set_default_size(GTK_WINDOW(OpenFile_window), 400, 200);

    // Create a fixed layout container
    fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(OpenFile_window), fixed);

    // Create the "Enter New File Name" label and entry
    label1 = gtk_label_new("Enter File Name to open :");
    gtk_fixed_put(GTK_FIXED(fixed), label1, 10, 10);
    
    fileName_entry = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), fileName_entry, 180, 10);
    gtk_widget_set_size_request(fileName_entry, 200, -1);  // Set the width to 200 pixels
    gtk_entry_set_placeholder_text(GTK_ENTRY(fileName_entry), "Enter file name");

    // Create the "Open File" button
    button = gtk_button_new_with_label("Open File");
    gtk_fixed_put(GTK_FIXED(fixed), button, 50, 50);
    g_signal_connect(button, "clicked", G_CALLBACK(on_Open_File_Button), OpenFile_window);

    // Create the "Cancel" button
    button1 = gtk_button_new_with_label("Cancel");
    gtk_fixed_put(GTK_FIXED(fixed), button1, 180, 50);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_Cancle_Button), OpenFile_window);

    // Show the window
    gtk_window_present(GTK_WINDOW(OpenFile_window));
}

static void on_New_File(GtkWidget *widget, gpointer data) {
    g_print("%s\n", (char *)data);
    
    GtkWidget *label1, *button, *button1;
    GtkWidget *fixed;

    NewFile_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(NewFile_window), "Create New File");
    gtk_window_set_default_size(GTK_WINDOW(NewFile_window), 400, 200);

    // Create a fixed layout container
    fixed = gtk_fixed_new();
   // gtk_widget_set_margin_top(fixed, 20);
    gtk_window_set_child(GTK_WINDOW(NewFile_window), fixed);

    // Create the "Enter New File Name" label and entry
    label1 = gtk_label_new("Enter New File Name:");
    gtk_fixed_put(GTK_FIXED(fixed), label1, 10, 10);
    
    fileName_entry = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), fileName_entry, 180, 10);
    gtk_widget_set_size_request(fileName_entry, 200, -1);  // Set the width to 200 pixels
    gtk_entry_set_placeholder_text(GTK_ENTRY(fileName_entry), "Enter file name");


    // Create the "Create File" button
    button = gtk_button_new_with_label("Create File");
    gtk_fixed_put(GTK_FIXED(fixed), button, 50, 50);
    g_signal_connect(button, "clicked", G_CALLBACK(on_Create_File), NewFile_window);

    // Create the "Cancel" button
    button1 = gtk_button_new_with_label("Cancel");
    gtk_fixed_put(GTK_FIXED(fixed), button1, 180, 50);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_Cancle_Button), NewFile_window);

    // Show the window
    gtk_window_present(GTK_WINDOW(NewFile_window));
}

static void on_Delete_File(GtkWidget *widget, gpointer data) {
    g_print("%s\n", (char *)data);                                                                                                                                                                                
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *fixed;
    GtkWidget *New_Project, *Open_Project, *Build_Project, *Run_Project, *Open_File, *New_File, *Delete_File;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "C IDE");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);

    fixed = gtk_fixed_new();
    gtk_widget_set_margin_top(fixed, 10);
    gtk_widget_set_margin_bottom(fixed, 10);
    gtk_widget_set_margin_start(fixed, 10);
    gtk_widget_set_margin_end(fixed, 10);
    gtk_window_set_child(GTK_WINDOW(window), fixed);

    //Button 1
    New_Project = gtk_button_new_with_label("New Project");
    gtk_widget_set_size_request(New_Project, 150, 30);
    g_signal_connect(New_Project, "clicked", G_CALLBACK(on_New_Project), "Inside New Project");
    gtk_fixed_put(GTK_FIXED(fixed), New_Project, 10, 10);

    //Button 2
    Open_Project = gtk_button_new_with_label("Open Project");
    gtk_widget_set_size_request(Open_Project, 150, 30);
    g_signal_connect(Open_Project, "clicked", G_CALLBACK(on_Open_Project), "Inside Open Project");
    gtk_fixed_put(GTK_FIXED(fixed), Open_Project, 170, 10);

    //Button 3
    New_File = gtk_button_new_with_label("New File");
    gtk_widget_set_size_request(New_File, 150, 30);
    g_signal_connect(New_File, "clicked", G_CALLBACK(on_New_File), "New File button");
    gtk_fixed_put(GTK_FIXED(fixed), New_File, 330, 10);

    //Button 4
    Open_File = gtk_button_new_with_label("Open File");
    gtk_widget_set_size_request(Open_File, 150, 30);
    g_signal_connect(Open_File, "clicked", G_CALLBACK(on_Open_File), "Open button");
    gtk_fixed_put(GTK_FIXED(fixed), Open_File, 490, 10);

    //Button 5
    Build_Project = gtk_button_new_with_label("Build Project");
    gtk_widget_set_size_request(Build_Project, 150, 30);
    g_signal_connect(Build_Project, "clicked", G_CALLBACK(on_Build_Project), "Inside Build File");
    gtk_fixed_put(GTK_FIXED(fixed), Build_Project, 650, 10);


    //Button 6
    Run_Project = gtk_button_new_with_label("Run Project");
    gtk_widget_set_size_request(Run_Project, 150, 30);
    g_signal_connect(Run_Project, "clicked", G_CALLBACK(on_Run_Project), "Run button");
    gtk_fixed_put(GTK_FIXED(fixed), Run_Project, 810, 10);

    //Button 7
    Delete_File = gtk_button_new_with_label("Delete File");
    gtk_widget_set_size_request(Delete_File, 150, 30);
    g_signal_connect(Delete_File, "clicked", G_CALLBACK(on_Delete_File), "Delete button");
    gtk_fixed_put(GTK_FIXED(fixed), Delete_File, 970, 10);

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

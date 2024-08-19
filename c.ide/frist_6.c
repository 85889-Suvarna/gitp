#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#define PROJECT_EXT ".proj"
#define MAX_FILES 10
#define FILENAME_LEN 40
#define COMMAND_LEN 1024

typedef struct {
    int cnt;
    char files[MAX_FILES][FILENAME_LEN];
} proj_t;

char initial_path[100];
char project_name[100];
char project_file[100];

void create_project_dir(const char *dir_path);
void create_project_file(const char *proj_name, proj_t *project);
void load_project(const char *proj_name, proj_t *project);
void save_project(const char *proj_name, proj_t *project);
void add_file_to_project(proj_t *project, const char *filename);
void compile_and_run(proj_t *project, const char *exec_name);
void build_project(proj_t *project);
void clean_project(const char *proj_name);
void display_menu();
void handle_menu(proj_t *project, const char *proj_name);
void proj_init(proj_t *project);
void new_file(proj_t *project, const char *proj_name);
void open_file(proj_t *project);
void delete_file(proj_t *project, const char *proj_name);
void run_project_with_args(const char *exec_name);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <project_dirpath> <project_name>\n", argv[0]);
        exit(1);
    }

    strcpy(initial_path, argv[1]);
    strcpy(project_name, argv[2]);

    create_project_dir(initial_path);
    if (chdir(initial_path) != 0) {
        perror("Failed to change directory");
        exit(1);
    }

    snprintf(project_file, sizeof(project_file), "%s.proj", project_name);
    printf("Final Project file is : %s\n", project_file);

    proj_t project;
    proj_init(&project);

    FILE *fd = fopen(project_file, "rb");
    if (fd == NULL) {
        if (errno == ENOENT) {
            create_project_file(project_file, &project);
        } else {
            perror("Failed to open project file");
            exit(1);
        }
    } else {
        fclose(fd);
    }

    load_project(project_file, &project);
    handle_menu(&project, project_file);

    return 0;
}

void create_project_dir(const char *dir_path) {
    struct stat st = {0};
    if (stat(dir_path, &st) == -1) {
        if (mkdir(dir_path) != 0 && errno != EEXIST) {
            perror("Failed to create directory");
            exit(1);
        }
    }
}

void create_project_file(const char *proj_name, proj_t *project) {
    FILE *fd = fopen(proj_name, "wb");
    if (fd == NULL) {
        perror("Failed to create project file");
        exit(1);
    }
    fwrite(project, sizeof(proj_t), 1, fd);
    fclose(fd);
}

void load_project(const char *proj_name, proj_t *project) {
    FILE *fd = fopen(proj_name, "rb");
    if (fd == NULL) {
        perror("Failed to load project file");
        exit(1);
    }
    fread(project, sizeof(proj_t), 1, fd);
    fclose(fd);
}

void save_project(const char *proj_name, proj_t *project) {
    FILE *fd = fopen(proj_name, "wb");
    if (fd == NULL) {
        perror("Failed to save project file");
        exit(1);
    }
    fwrite(project, sizeof(proj_t), 1, fd);
    fclose(fd);
}

void add_file_to_project(proj_t *project, const char *filename) {
    if (project->cnt >= MAX_FILES) {
        printf("Max number of files reached\n");
        return;
    }
    strcpy(project->files[project->cnt++], filename);
}

void compile_and_run(proj_t *project, const char *exec_name) {
    char command[COMMAND_LEN] = "gcc -o ";
    strcat(command, exec_name);
    for (int i = 0; i < project->cnt; i++) {
        strcat(command, " ");
        strcat(command, project->files[i]);
    }
    int status = system(command);

    if (status != 0) {
        printf("Compilation failed\n");
        return;
    }

    char args[COMMAND_LEN];
    printf("Enter command line arguments: ");
    getchar();  // Consume the newline character left by the previous input
    fgets(args, COMMAND_LEN, stdin);
    args[strcspn(args, "\n")] = 0; // Remove the newline character

    snprintf(command, sizeof(command), "./%s %s", exec_name, args);
    system(command);
}

void build_project(proj_t *project) {
    char command[COMMAND_LEN];
    int status;
    int has_error = 0;

    // Compile each .c file into .o files
    for (int i = 0; i < project->cnt; i++) {
        if (strstr(project->files[i], ".c") != NULL) {
            snprintf(command, sizeof(command), "gcc -c %s", project->files[i]);
            status = system(command);
            if (status != 0) {
                printf("Compilation failed for %s\n", project->files[i]);
                has_error = 1;
                break;
            }
        }
    }

    if (has_error) {
        printf("Build failed. Errors occurred during compilation.\n");
        return;
    }

    // Link all .o files into the final executable
    snprintf(command, sizeof(command), "gcc -o %s", project_name);
    for (int i = 0; i < project->cnt; i++) {
        if (strstr(project->files[i], ".c") != NULL) {
            char obj_file[FILENAME_LEN];
            snprintf(obj_file, sizeof(obj_file), "%s.o", strtok(project->files[i], "."));
            strcat(command, " ");
            strcat(command, obj_file);
        }
    }

    status = system(command);
    if (status != 0) {
        printf("Linking failed\n");
    } else {
        printf("Build successful. Executable is %s\n", project_name);
    }
}

void clean_project(const char *proj_name) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(".")) == NULL) {
        perror("Failed to open directory");
        return;
    }

    // Delete all object files (.o)
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".o") != NULL) {
            if (unlink(entry->d_name) != 0) {
                perror("Failed to delete object file");
            } else {
                printf("Deleted object file: %s\n", entry->d_name);
            }
        }
    }

    // Delete the executable file
    if (unlink(proj_name) != 0) {
        perror("Failed to delete executable file");
    } else {
        printf("Deleted executable file: %s\n", proj_name);
    }

    closedir(dir);
}

void new_file(proj_t *project, const char *proj_name) {
    if (project->cnt >= MAX_FILES) {
        printf("Max number of files reached\n");
        return;
    }

    char filename[FILENAME_LEN];
    printf("Enter new filename (.c or .h): ");
    scanf("%s", filename);

    add_file_to_project(project, filename);
    save_project(proj_name, project);

    char command[256];
    snprintf(command, sizeof(command), "vim %s", filename);
    system(command);
}

void open_file(proj_t *project) {
    char filename[FILENAME_LEN];
    printf("Enter filename to open (.c or .h): ");
    scanf("%s", filename);

    int found = 0;
    for (int i = 0; i < project->cnt; i++) {
        if (strcmp(project->files[i], filename) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("File not found in the project\n");
        return;
    }

    char command[256];
    snprintf(command, sizeof(command), "vim %s", filename);
    system(command);
}

void delete_file(proj_t *project, const char *proj_name) {
    char filename[FILENAME_LEN];
    printf("Enter filename to delete (.c or .h): ");
    scanf("%s", filename);

    int found = 0;
    int index = -1;
    for (int i = 0; i < project->cnt; i++) {
        if (strcmp(project->files[i], filename) == 0) {
            found = 1;
            index = i;
            break;
        }
    }

    if (!found) {
        printf("File not found in the project\n");
        return;
    }

    if (unlink(filename) != 0) {
        perror("Failed to delete file");
        return;
    }

    for (int i = index; i < project->cnt - 1; i++) {
        strcpy(project->files[i], project->files[i + 1]);
    }
    project->cnt--;

    save_project(proj_name, project);
    printf("File deleted successfully\n");
}

void display_menu() {
    printf("\nMenu:\n");
    //printf("0 Add file to project\n");
    //printf(". Compile and run project\n");
    printf("1. New file\n");
    printf("2. Open file\n");
    printf("3. Build Project\n");
    printf("4. Delete file\n");
    printf("5. Run Project\n");
    printf("6. Clean Project\n");  
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

void handle_menu(proj_t *project, const char *proj_name) {
    int choice;
    char filename[256];

    while (1) {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
           /* case 1:
                printf("Enter filename to add: ");
                scanf("%s", filename);
                add_file_to_project(project, filename);
                save_project(proj_name, project);
                break;*/
            /*case 2:
                compile_and_run(project, project_name);
                break;*/
            case 1:
                new_file(project, proj_name);
                break;
            case 2:
                open_file(project);
                break;
            case 3:
                build_project(project);
                break;
            case 4:
                delete_file(project, proj_name);
                break;
            case 5:
                run_project_with_args(project_name);
            case 6:
                clean_project(proj_name);
                break;
            case 7:
                save_project(proj_name, project);
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
}

void proj_init(proj_t *project) {
    project->cnt = 0;
}

void run_project_with_args(const char *exec_name) {
    struct stat buffer;
    if (stat(exec_name, &buffer) != 0) {
        perror("Executable not found. Please build the project first.");                        
        return;
    }

    char args[COMMAND_LEN];
    printf("Enter command line arguments: ");
    getchar();  // Consume the newline character left by the previous input
    fgets(args, COMMAND_LEN, stdin);
    args[strcspn(args, "\n")] = 0; // Remove the newline character

    char command[COMMAND_LEN];
    snprintf(command, sizeof(command), "./%s %s", exec_name, args);
    system(command);
}

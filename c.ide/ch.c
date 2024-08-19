#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>



// Defining a structure for storing project details
typedef struct project_ide {
    int cnt; // to store the number of files created
    char file[10][40]; // storing a max of 10 files, file name not more than 40 characters
} proj_t;

/*======Function Declarations=====*/
void create_file(proj_t *p, const char *project_file);
void open_file();
void delete_file();
void build_project();
void run_project();
void clean_project();
void debug_project();
void valgrind();

int check_exe();
/*================================*/

#define SIZE sizeof(proj_t)
void proj_init(proj_t *p) {
    p->cnt = 0;
    for (int i = 0; i < 10; i++) {
        memset(p->file[i], 0, 40);
    }
}

proj_t *p;
int count;

char initial_path[100]; // Path where project folder needs to be created
char project_name[30]; // To store project Name
char project_file[100]; // Path to the .proj file
char temp_path[100]; // To store temporary operations and path changes

char* arr[13]; // to store the names of the .c files
char* arr_o[13]; // to store the names of the .o files

void new_file(proj_t *project, const char *project_file);

int main(int argc, char *argv[]) {
    // Argument validation
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <project_dirpath> <project_name>\n", argv[0]);
        exit(1);
    }
    int choice; // menu driven

    // Copy the path and Project given from user into a string and make a directory
    strcpy(initial_path, argv[1]);
    strcpy(project_name, argv[2]);

    // Make the directory at given path if it does not exist
    if (mkdir(initial_path) != 0 && errno != EEXIST) {
        perror("Failed to create directory");
        exit(1);
    } else {
        printf("Directory created\n");
    }

    if (chdir(initial_path) != 0) {
        perror("Failed to change directory");
        exit(1);
    }

    snprintf(project_file, sizeof(project_file), "%s.proj", project_name);
    printf("Final Project file is: %s\n", project_file);

    p = (proj_t *)malloc(SIZE);
    proj_init(p);

    // Check if project file exists
    FILE *fd = fopen(project_file, "rb");
    // if (fd != NULL) {
    //     fread(p, SIZE, 1, fd);
    //     fclose(fd);
    // } else {
        // Create the file using fopen()
        fd = fopen(project_file, "wb");
        fwrite(p, SIZE, 1, fd);
        fclose(fd);
    }

    while (1) {
        printf("\nMenu:\n");
        printf("1. New File\n");
        printf("2. Open File\n");
        printf("3. Delete File\n");
        printf("4. Build Project\n");
        printf("5. Clean Project\n");
        printf("6. Run Project\n");
        printf("7. Debug Project\n");
        printf("8. Check Memory Leakage\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: new_file(p, project_file); break;
            case 2: open_file(); break;
            case 3: delete_file(); break;
            case 4: build_project(); break;
            case 5: clean_project(); break;
            case 6: run_project(); break;
            case 7: debug_project(); break;
            case 8: valgrind(); break;
            case 9: 
                // Save the project before exiting
                fd = fopen(project_file, "wb");
                fwrite(p, SIZE, 1, fd);
                fclose(fd);
                free(p);
                exit(0);
            default: printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void new_file(proj_t *project, const char *project_file) {
    if (project->cnt >= 10) {
        printf("Maximum number of files reached.\n");
        return;
    }

    char filename[40];
    printf("Enter new file name (.h or .c): ");
    scanf("%s", filename);

    // Check if the file extension is valid (.h or .c)
    char *extension = strrchr(filename, '.');
    if (!extension || (strcmp(extension, ".c") != 0 && strcmp(extension, ".h") != 0)) {
        printf("Invalid file extension. Only .h or .c files are allowed.\n");
        return;
    }

    // Add the new file to the project information
    strcpy(project->file[project->cnt], filename);
    project->cnt++;

    // Update the project file
    FILE *fd = fopen(project_file, "wb");
    fwrite(project, SIZE, 1, fd);
    fclose(fd);

    // Create the new file
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("fopen");
        exit(1);
    }
    fclose(file);

    // Launch vim editor to edit the new file
    pid_t pid = fork();
    if (pid == 0) {
        execlp("vim", "vim", filename, (char *)NULL);
        perror("execlp");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
        exit(1);
    }
}
// functions for other operations
void open_file() {
    printf("Open File operation\n");
}

void delete_file() {
    printf("Delete File operation\n");
}

void build_project() {
    printf("Build Project operation\n");
}

void run_project() {
    printf("Run Project operation\n");
}

void clean_project() {
    printf("Clean Project operation\n");
}

void debug_project() {
    printf("Debug Project operation\n");
}

void valgrind() {
    printf("Valgrind operation\n");
}
 
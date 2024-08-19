#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define PROJECT_EXT ".proj"

typedef struct {
    int cnt;
    char files[10][40];
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
void display_menu();
void handle_menu(proj_t *project, const char *proj_name);
void proj_init(proj_t *project);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <project_dirpath> <project_name>\n", argv[0]);
        exit(1);
    }

    strcpy(initial_path, argv[1]);
    strcpy(project_name, argv[2]);

    // // Ensure the project name has a .proj extension
    // if (strstr(project_name, PROJECT_EXT) == NULL) {
    //     fprintf(stderr, "Project file must have .proj extension\n");
    //     exit(1);
    // }

    create_project_dir(initial_path);
    if (chdir(initial_path) != 0) {
        perror("Failed to change directory");
        exit(1);
    }
    snprintf(project_file,sizeof(project_file),"%s.proj",project_name);
    printf("Final Project file is : %s\n",project_file);
    
    //snprintf(project_file, sizeof(project_file), "%s", project_name);
    //printf("Final Project file is: %s\n", project_file);

    proj_t project;
    //proj_init(&project);

    FILE *fd = fopen(project_file, "wb");
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
    if (project->cnt >= 10) {
        printf("Max number of files reached\n");
        return;
    }
    strcpy(project->files[project->cnt++], filename);
}

void compile_and_run(proj_t *project, const char *exec_name) {
    char command[1024] = "gcc -o ";
    strcat(command, exec_name);
    for (int i = 0; i < project->cnt; i++) {
        strcat(command, " ");
        strcat(command, project->files[i]);
    }
    system(command);
    system(exec_name);
}

void display_menu() {
    printf("\nMenu:\n");
    printf("1. Add file to project\n");
    printf("2. Compile and run project\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void handle_menu(proj_t *project, const char *proj_name) {
    int choice;
    char filename[256];

    while (1) {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename to add: ");
                scanf("%s", filename);
                add_file_to_project(project, filename);
                save_project(proj_name, project);
                break;
            case 2:
                compile_and_run(project, "./a.out");
                break;
            case 3:
                save_project(proj_name, project);
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
}



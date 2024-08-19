#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_FILES 10
#define PROJ_EXT ".proj"

typedef struct {
    int file_count;
    char filenames[MAX_FILES][255];
} Project;

void load_project(const char *proj_file, Project *proj) {
    FILE *file = fopen(proj_file, "rb");
    if (file == NULL) {
        perror("Failed to open project file");
        exit(EXIT_FAILURE);
    }

    fread(proj, sizeof(Project), 1, file);
    fclose(file);
}

void save_project(const char *proj_file, Project *proj) {
    FILE *file = fopen(proj_file, "wb");
    if (file == NULL) {
        perror("Failed to open project file");
        exit(EXIT_FAILURE);
    }

    fwrite(proj, sizeof(Project), 1, file);
    fclose(file);
}

void compile_file(const char *filename, int *success) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("gcc", "gcc", "-c", filename, NULL);
        perror("Failed to exec gcc");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            *success = 1;
        } else {
            *success = 0;
        }
    } else {
        perror("Failed to fork");
        *success = 0;
    }
}

void link_files(Project *proj, const char *output) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[MAX_FILES + 4];
        args[0] = "gcc";
        for (int i = 0; i < proj->file_count; i++) {
            args[i + 1] = (char *)malloc(260);
            snprintf(args[i + 1], 260, "%s.o", proj->filenames[i]);
        }
        args[proj->file_count + 1] = "-o";
        args[proj->file_count + 2] = (char *)output;
        args[proj->file_count + 3] = NULL;
        execvp("gcc", args);
        perror("Failed to exec gcc");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Failed to fork");
    }
}

void compile_project(Project *proj, const char *output) {
    int success = 1;
    for (int i = 0; i < proj->file_count; i++) {
        compile
        compile_file(proj->filenames[i], &success);
        if (!success) {
            printf("Compilation failed for %s\n", proj->filenames[i]);
            return;
        }
    }
    link_files(proj, output);
}

void clean_project(Project *proj, const char *output) {
    for (int i = 0; i < proj->file_count; i++) {
        char obj_file[260];
        snprintf(obj_file, sizeof(obj_file), "%s.o", proj->filenames[i]);
        unlink(obj_file);
    }
    unlink(output);
}

void run_project(const char *output) {
    if (access(output, F_OK) == -1) {
        printf("Executable not found. Build the project first.\n");
        return;
    }

    printf("Enter command line arguments: ");
    char args[256];
    scanf(" %[^\n]", args);

    pid_t pid = fork();
    if (pid == 0) {
        execl(output, output, args, (char *)NULL);
        perror("Failed to exec");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Failed to fork");
    }
}

void debug_project(const char *output) {
    if (access(output, F_OK) == -1) {
        printf("Executable not found. Build the project first.\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execlp("gdb", "gdb", output, NULL);
        perror("Failed to exec gdb");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Failed to fork");
    }
}

void check_memory_leak(const char *output) {
    if (access(output, F_OK) == -1) {
        printf("Executable not found. Build the project first.\n");
        return;
    }

    printf("Enter command line arguments: ");
    char args[256];
    scanf(" %[^\n]", args);

    pid_t pid = fork();
    if (pid == 0) {
        execlp("valgrind", "valgrind", "--leak-check=full", output, args, (char *)NULL);
        perror("Failed to exec valgrind");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Failed to fork");
    }
}

void edit_file_in_project(Project *proj) {
    printf("Enter file name to edit: ");
    char filename[256];
    scanf("%s", filename);

    int found = 0;
    for (int i = 0; i < proj->file_count; i++) {
        if (strcmp(proj->filenames[i], filename) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("File not part of the project.\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execlp("vim", "vim", filename, NULL);
        perror("Failed to exec vim");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Failed to fork");
    }
}

void delete_file_from_project(Project *proj) {
    printf("Enter file name to delete: ");
    char filename[256];
    scanf("%s", filename);

    int found = 0;
    for (int i = 0; i < proj->file_count; i++) {
        if (strcmp(proj->filenames[i], filename) == 0) {
            unlink(filename);
            for (int j = i; j < proj->file_count - 1; j++) {
                strcpy(proj->filenames[j], proj->filenames[j + 1]);
            }
            proj->file_count--;
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("File not part of the project.\n");
    }
}

void display_menu() {
    printf("1. Add File to Project\n");
    printf("2. Remove File from Project\n");
    printf("3. List Files in Project\n");
    printf("4. Compile Project\n");
    printf("5. Execute Project\n");
    printf("6. Edit File in Project\n");
    printf("7. Delete File from Project\n");
    printf("8. Build Project\n");
    printf("9. Clean Project\n");
    printf("10. Run Project\n");
    printf("11. Debug Project\n");
    printf("12. Check Memory Leakage\n");
    printf("13. Exit\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <project_dirpath> <project_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *project_dir = argv[1];
    const char *project_name = argv[2];
    char proj_file[256];
    char output[256];

    snprintf(proj_file, sizeof(proj_file), "%s/%s%s", project_dir, project_name, PROJ_EXT);
    snprintf(output, sizeof(output), "%s/%s", project_dir, project_name);

    if (mkdir(project_dir) == -1 && errno != EEXIST) {
        perror("Failed to create project directory");
        exit(EXIT_FAILURE);
    }

    if (chdir(project_dir) == -1) {
        perror("Failed to change to project directory");
        exit(EXIT_FAILURE);
    }

    Project proj = {0};
    int proj_file_exists = access(proj_file, F_OK) != -1;

    if (proj_file_exists) {
        load_project(proj_file, &proj);
    } else {
        save_project(proj_file, &proj);
    }

    int choice;
    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_file_to_project(&proj);
                break;
            case 2:
                remove_file_from_project(&proj);
                break;
            case 3:
                list_files_in_project(&proj);
                break;
            case 4:
                compile_project(&proj, output);
                break;
            case 5:
                execute_project();
                break;
            case 6:
                edit_file_in_project(&proj);
                break;
            case 7:
                delete_file_from_project(&proj);
                break;
            case 8:
                compile_project(&proj, output);
                break;
            case 9:
                clean_project(&proj, output);
                break;
            case 10:
                run_project(output);
                break;
            case 11:
                debug_project(output);
                break;
            case 12:
                check_memory_leak(output);
                break;
            case 13:
                save_project(proj_file, &proj);
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

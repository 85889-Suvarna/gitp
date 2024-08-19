#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

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

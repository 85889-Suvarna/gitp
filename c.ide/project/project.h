#ifndef PROJECT_H
#define PROJECT_H

#define PROJECT_EXT ".proj"
#define MAX_FILES 10
#define FILENAME_LEN 40
#define COMMAND_LEN 1024

typedef struct {
    int cnt;
    char files[MAX_FILES][FILENAME_LEN];
} proj_t;

extern char initial_path[100];
extern char project_name[100];
extern char project_file[100];

void create_project_dir(const char *dir_path);
void create_project_file(const char *proj_name, proj_t *project);
void load_project(const char *proj_name, proj_t *project);
void save_project(const char *proj_name, proj_t *project);
void add_file_to_project(proj_t *project, const char *filename);
void compile_and_run(proj_t *project, const char *exec_name);
void build_project(const char *filename);
void clean_project(const char *proj_name);
void display_menu();
void handle_menu(proj_t *project, const char *proj_name);
void proj_init(proj_t *project);
void new_file(proj_t *project, const char *proj_name);
void open_file(proj_t *project);
void delete_file(proj_t *project, const char *proj_name);
void run_project_with_args(const char *exec_name);
void link_files(proj_t *project, const char *output);
void debug_project(const char *exec_name);
void check_memory_leaks(const char *exec_name);

#endif // PROJECT_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

//delaring a structure for storing project details
typedef struct project_ide
    {
        int cnt;//to store the no of file created
        char file[10][40];//storing a max of 10 character files, file name not more than 40 charactor


    }proj_t;

    
/*======Function Declarations=====*/
void create_file();
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
    void proj_init(proj_t*);
    proj_t *p;

    int count;

    
char initial_path[100];	//Path where project folder needs to be created
char project_name[30];	//To store project Name
//char mkdir_path[100];	//Path of the project folder (initial path + Project Name)
char project_file[100];	//Path to the .proj file
char temp_path[100];	//To store temporary operations and path changes

char* arr[13];		//to store the names of the .c files
char* arr_o[13];	//to store the names of the .o files	

int main(int argc, char *argv[])
{
    //argument validation
    if(argc!=3)
    {
                fprintf(stderr, "Usage: %s <project_dirpath> <project_name>\n", argv[0]);
                exit(1);

    }
    int choice;//menu deriven
//copy the path and Project given from user into a string and make a directory
	strcpy(initial_path, argv[1]);	strcpy(project_name, argv[2]);
	
	//make the directory at given path if it does not exist
	if(mkdir(initial_path)!=0 && errno != EEXIST)
		    perror("faild to create directory");
    else{
        printf("Directory created\n");
    }
    
    if (chdir(initial_path)!=0)
    {
        printf("Directory changed to %s\n",initial_path);
    }
    
    snprintf(project_file,sizeof(project_file),"%s.proj",project_name);
    printf("Final Project file is : %s\n",project_file);
    
	//printf("After concatinating\n");
        //creating the file using fopen()
    FILE *fd=fopen(project_file,"wb");

    fwrite(p,sizeof(proj_t),1,fd);

    fclose(fd);





    return 0;

}
//Function definitions

void proj_init(proj_t* p)
	{
		p->cnt=0;
	}

		



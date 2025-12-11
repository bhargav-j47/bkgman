#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "config.h"
#include "helper.h"
#include "remove.h"

int cmp(const void* a,const void* b){
    return strcmp(*(const char **)b, *(const char **)a);
}

void uninstall_files(char** files,int cnt){

    qsort(files,cnt,sizeof(char*),cmp);
    
    for(int i=0;i<cnt;i++){

        const char *fpath = files[i];
        struct stat file_stat;

        if(lstat(fpath, &file_stat) == -1){
            continue;
        }
        if(S_ISDIR(file_stat.st_mode)){
            if(rmdir(fpath)){
                continue;
            }
        }else{
            if(unlink(fpath)){
                print_error("error removing package");
                exit(1);
            }
        }
    }
}

//*** for pointer to array of array of char i.e pointer to array of string
int list_files(char*** files,char* files_path){
   
    FILE* fp=fopen(files_path,"r");
    if(!fp){
        print_error("corrupted package can't remove");
        exit(1);
    }

    int cnt=0;
    char buff[MAX_PATH*5];

    while(fgets(buff,sizeof(buff),fp)){
        if(strlen(buff)>1) cnt++;
    }

    char **list = malloc(cnt * sizeof(char*));
    //*files=malloc(cnt * sizeof(char*));
    
    rewind(fp);

    int i=0;
    while(fgets(buff,sizeof(buff),fp) && i<cnt){
        buff[strcspn(buff, "\n")] = 0;
        if (strlen(buff) == 0) continue;
        list[i] = strdup(buff);
        i++;
    }

    *files=list;
    fclose(fp);
    return cnt;
}

void remove_package(const char* pkgname){
    
    //check if package is installed and it is in db
    char db_path[MAX_PATH];
    snprintf(db_path,sizeof(db_path),"%s/%s",LOCAL_DB,pkgname);
    char files_path[MAX_PATH];
    snprintf(files_path,sizeof(files_path),"%s/files",db_path);

    if(!file_exists(files_path)){
        print_error("package is not installed or is corrupted");
        exit(1);
    }

    print_info("processing package changes");

    //run pre hooks

    //create files list
    char** files=NULL;
    int cnt=list_files(&files,files_path);

    printf("  --> removing pkg %s\n",pkgname);
    
    //remove installed files
    if(cnt>1){
        uninstall_files(files,cnt);

        for(int i=0;i<cnt;i++){
            free(files[i]);
        }
        free(files);
    }

    //run post hooks

    //remove entry from db
    char cmd[MAX_PATH*10];
    snprintf(cmd,sizeof(cmd),"rm -rf %s",db_path);
    run_command(cmd);
}

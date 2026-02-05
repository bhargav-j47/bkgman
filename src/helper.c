#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "helper.h"
#include "config.h"

void print_error(const char *msg) {
    fprintf(stderr, "%s%sError:%s %s\n", COL_BOLD, COL_RED, COL_RESET, msg);
}

void print_info(const char *msg) {
    printf("%s%s::%s %s\n", COL_BOLD, COL_BLUE, COL_RESET, msg);
}

void print_success(const char *msg) {
    printf("%s%s==>%s %s\n", COL_BOLD, COL_GREEN, COL_RESET, msg);
}

//i want to move fork+exec someday
int run_command(const char *cmd){
    char fcmd[MAX_PATH*4];
    snprintf(fcmd,sizeof(fcmd),"%s 1>/dev/null 2>/dev/null",cmd);
    int status = system(cmd);
    return (status == 0);
}

int file_exists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

char* read_meta_key(const char *filepath, const char *key) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) return NULL;

    char line[MAX_LINE];
    char *result = NULL;
    size_t key_len = strlen(key);

    while (fgets(line, sizeof(line), fp)) {

        line[strcspn(line, "\n")] = 0; //removes newline

        if (strncmp(line, key, key_len) == 0) {
            char *p = strchr(line, '=');
            if (p) {
                p++;
                while (*p == ' ') p++;
                result = strdup(p);
                break;
            }
        }
    }
    fclose(fp);
    return result;
}

int ensure_dir(const char* path) {
    char cmd[MAX_PATH + 20];
    //char* real_path= realpath(path,NULL);
    snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\" 1>/dev/null 2>/dev/null", path);
    return run_command(cmd);
}

int run_hook(const char *path, const char *hook){
    
    printf("running %s\n",hook);
    char cmd[MAX_PATH + 100];
    snprintf(cmd, sizeof(cmd),"bash -c 'source %s && if declare -F %s > /dev/null; then %s; fi'",path,hook,hook);
    if(!run_command(cmd)){
        printf("hook %s failed skipping\n",hook);
        return 1;
    }
    return 0;
}

//not working fix this
int parse_dep_array(const char* str, char** deps){
    
    int p=0;
    int dep_cnt=0;

    while (str[p]!='\0'&& str[p]!='(') p++;

    if (str[p]=='(') p++;
    else return dep_cnt;

    while (str[p]!='\0'&& str[p]!=')') {

        while (str[p]!=')' && isspace(str[p])) p++;

        if(str[p]==')')break;

        char dep[100];
        int x=0;

        if(str[p]=='\''){
            p++;
            while (str[p]!='\0' && str[p]!='\'' && str[p]!=')') {
                dep[x++]=str[p++];
            }

            if (str[p]==')') p++;
        }
        dep[x]='\0';
        deps[dep_cnt]=strdup(dep);
        dep_cnt++;
    }
    return dep_cnt;
}

//implement this
void free_dep_array(char** deps, int cnt){
    for (int i=0; i<cnt; i++) {
        free(deps[i]);
    }
    return;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
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

// Execute a shell command safely i want to move fork+exec someday
int run_command(const char *cmd){
    char fcmd[MAX_PATH*4];
    snprintf(fcmd,sizeof(fcmd),"%s 1>/dev/null 2>/dev/null",cmd);
    int status = system(cmd);
    return (status == 0);
}

// Check if a file exists
int file_exists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

// Read a specific key from a .JPKGINFO file (format is key = value)
char* read_meta_key(const char *filepath, const char *key) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) return NULL;

    char line[MAX_LINE];
    char *result = NULL;
    size_t key_len = strlen(key);

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        // Check if line starts with key
        if (strncmp(line, key, key_len) == 0) {
            char *p = strchr(line, '=');
            if (p) {
                p++; // Skip '='
                while (*p == ' ') p++; // Skip spaces
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


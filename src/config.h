#ifndef CONFIG_H
#define CONFIG_H

#define INSTALL_ROOT "./bkgman_root" //for production replace with "/" 
#define DB_PATH      "./bkgman_db"  //for production replace with "/var/lib/bkgman"
#define LOCAL_DB     DB_PATH "/local"
#define SYNC_DB      DB_PATH "/sync"   
#define TEMP_DIR    "/tmp/bkgman_tmp"

#define MAX_PATH 1024
#define MAX_LINE 256

/* ANSI colours */
#define COL_RESET "\033[0m"
#define COL_BOLD  "\033[1m"
#define COL_RED   "\033[31m"
#define COL_GREEN "\033[32m"
#define COL_BLUE  "\033[34m"

#endif

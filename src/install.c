#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "config.h"
#include "helper.h"
#include "install.h"


void install_archive(const char* archive_path){
    
    if(!file_exists(archive_path)){
        print_error("pkg not found");
        return;
    }
    
    print_info("preparing install");
    char cmd[MAX_LINE*5];
    
    //creating temp
    snprintf(cmd,sizeof(cmd),"rm -rf %s && mkdir -pv %s",TEMP_DIR,TEMP_DIR);
    if(!run_command(cmd)){
        print_error("failed to extract the package");
        return;
    }

    //extract archive to temp
    printf("  --> extracting %s \n",archive_path);
    snprintf(cmd,sizeof(cmd),"tar -xf \"%s\" -C %s",archive_path,TEMP_DIR);
    if(!run_command(cmd)){
        print_error("failed to extract the package");
        return;
    }
    
    //check metadata
    char pkginfo[MAX_PATH];
    snprintf(pkginfo,sizeof(pkginfo),"%s/.JPKGINFO",TEMP_DIR);

    if(!file_exists(pkginfo)){
        print_error("jpkginfo does not exist for pkg");
        return;
    }
    
    char* pkgname=read_meta_key(pkginfo,"pkgname");
    char* pkgver=read_meta_key(pkginfo,"pkgver");
    
    if(!pkgname || !pkgver){
        print_error("invalid package ");
        return;
    }
    
    printf("  --> pkg found %s:%s",pkgname,pkgver);

    //check dependencies
    //resolve dependencies
    
    //add db entry
    char db_entry[MAX_PATH];
    snprintf(db_entry,sizeof(db_entry),"%s/%s",DB_PATH,pkgname);
    ensure_dir(db_entry);

    snprintf(cmd,sizeof(cmd),"cp %s %s/desc",pkginfo,db_entry);
    if(!run_command(cmd)){
        print_error("internal error occured");
        return;
    }

    char files_path[MAX_PATH];
    snprintf(files_path,sizeof(files_path),"%s/files",db_entry);
    //adds all files to files except metadata
    snprintf(cmd,sizeof(cmd),"touch %s",files_path);
    run_command(cmd);
    snprintf(cmd, sizeof(cmd), "cd %s && find . -type f ! -name '.JPKGINFO' > \"%s\"", TEMP_DIR, files_path);
    if(!run_command(cmd)){
        print_error("internal error occured");
        return;
    }

    printf("  --> installing files to root");

    snprintf(cmd,sizeof(cmd),"cp -rn %s/* %s/",TEMP_DIR,INSTALL_ROOT);
    if(!run_command(cmd)){
        print_error("installation failed");
        return;
    }
    snprintf(cmd,sizeof(cmd),"rm -rf %s/.JPKGINFO",INSTALL_ROOT);
    run_command(cmd);

    //cleanup
    printf("  --> cleaning up");
    snprintf(cmd,sizeof(cmd),"rm -rf %s",TEMP_DIR);
    run_command(cmd);
    
    char success_msg[MAX_LINE];
    snprintf(success_msg, sizeof(success_msg), "Installed %s-%s", pkgname, pkgver);
    print_success(success_msg);

    free(pkgname);
    free(pkgver);
    return;
}



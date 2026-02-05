#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "helper.h"
#include "install.h"
#include <string.h>


void clean_temp(){
    char cmd[MAX_PATH*5];
    printf("  --> cleaning up\n");
    snprintf(cmd,sizeof(cmd),"rm -rf %s",TEMP_DIR);
    run_command(cmd);
}

void install_archive(const char* archive_path){
    
    if(!file_exists(archive_path)){
        print_error("pkg not found");
        exit(1);
    }
    
    print_info("preparing install");
    char cmd[MAX_LINE*10];
    
    //creating temp
    snprintf(cmd,sizeof(cmd),"rm -rf %s && mkdir -pv %s 1>/dev/null 2>/dev/null",TEMP_DIR,TEMP_DIR);
    if(!run_command(cmd)){
        print_error("failed to extract the package");
        exit(1);
    }

    //extract archive to temp
    printf("  --> extracting %s \n",archive_path);
    snprintf(cmd,sizeof(cmd),"tar -xf \"%s\" -C %s 1>/dev/null 2>/dev/null",archive_path,TEMP_DIR);
    if(!run_command(cmd)){
        print_error("failed to extract the package");
        clean_temp(); 
        exit(1);
    }
    
    //check metadata
    char pkginfo[MAX_PATH];
    snprintf(pkginfo,sizeof(pkginfo),"%s/.JPKGINFO",TEMP_DIR); 
    char hookpath[MAX_PATH];
    snprintf(hookpath, sizeof(hookpath),"%s/.HOOKS",TEMP_DIR);

    if(!file_exists(pkginfo)){
        print_error("jpkginfo does not exist for pkg");
        clean_temp(); 
        exit(1);
    }
    
    char* pkgname=read_meta_key(pkginfo,"pkgname");
    char* pkgver=read_meta_key(pkginfo,"pkgver");
    
    if(!pkgname || !pkgver){
        print_error("invalid package ");
        clean_temp(); 
        exit(1);
    }
    
    printf("  --> pkg found %s:%s \n",pkgname,pkgver);

    //check dependencies       ;doing this
        
    char* depends_str=read_meta_key(pkginfo, "depends");
    char** deps=(char**)malloc(sizeof(char*)*100);
    int deps_cnt=parse_dep_array(depends_str,deps);
    
    char** missing_deps=(char**)malloc(sizeof(char*)*100);
    int missing_deps_cnt=0;

    char* dep_path=(char*)malloc(sizeof(char)*MAX_PATH);
    for (int i=0; i<deps_cnt; i++){
        snprintf(dep_path,sizeof(char)*MAX_PATH,"%s/%s",LOCAL_DB,deps[i]);
        if(!file_exists(dep_path)){
            missing_deps[missing_deps_cnt++]=strdup(deps[i]);
        }
    } 
    free(dep_path);

    if(missing_deps_cnt!=0){
        print_info("following dependencies are missing && needed to install pkg");
        for (int i=0; i<missing_deps_cnt; i++) {
            printf("%s ",missing_deps[i]);
        }
        printf("\n");
    }

    free_dep_array(deps, deps_cnt);
    free(deps);

    //resolve dependencies     ;to be done later 
    

    free_dep_array(missing_deps, missing_deps_cnt);
    free(missing_deps);
    //run pre hooks
    run_hook(hookpath,"pre_install");
    
    //add db entry
    char db_entry[MAX_PATH];
    snprintf(db_entry,sizeof(db_entry),"%s/%s",LOCAL_DB,pkgname);
    if(!ensure_dir(db_entry)){
        print_error("internal error occured");
        clean_temp(); 
        exit(1);
    }

    snprintf(cmd,sizeof(cmd),"cp %s %s/desc 1>/dev/null 2>/dev/null",pkginfo,db_entry);
    if(!run_command(cmd)){
        print_error("internal error occured");
        clean_temp(); 
        exit(1);
    }
    snprintf(cmd,sizeof(cmd),"cp %s %s/hooks 1>/dev/null 2>/dev/null",hookpath,db_entry);
    if(!run_command(cmd)){
        print_error("internal error occured");
        clean_temp(); 
        exit(1);
    }


    //adds all files to files except metadata
    char files_path[MAX_PATH];
    snprintf(files_path,sizeof(files_path),"%s/files",db_entry);
    snprintf(cmd, sizeof(cmd),"find \"%s/\" ! -name '.HOOKS' ! -name '.JPKGINFO' -printf \"%s/%%P\\n\" > \"%s\" ",TEMP_DIR,INSTALL_ROOT,files_path);
    if(!run_command(cmd)){
        print_error("internal error occured");
        clean_temp(); 
        exit(1);
    }
    
    //installing files to location
    printf("  --> installing files to root\n");
    snprintf(cmd,sizeof(cmd),"cp -r %s/. %s/",TEMP_DIR,INSTALL_ROOT);
    if(!run_command(cmd)){
        print_error("installation failed");
        clean_temp(); 
        exit(1);
    }
    snprintf(cmd,sizeof(cmd),"rm -rf %s/.JPKGINFO",INSTALL_ROOT);
    run_command(cmd);
    snprintf(cmd,sizeof(cmd),"rm -rf %s/.HOOKS",INSTALL_ROOT);
    run_command(cmd);

    //run post hooks
    run_hook(hookpath,"post_install");

    //cleanup
    clean_temp(); 
    char success_msg[MAX_LINE];
    snprintf(success_msg, sizeof(success_msg), "Installed %s-%s", pkgname, pkgver);
    print_success(success_msg);

    free(pkgname);
    free(pkgver);
    return;
}



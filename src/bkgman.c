/*
 *  bkgman : bhargav's package manager
 *  A custom package manager for lfs written in c inspired from Arch-pacman
 *
 *  USAGE:
 *  bkgman -I <pkg tar> (install)
 *  bkgman -Q <pkgname> (query)
 *  bkgman -R <pkgname> (remove)
 *  bkgman -h           (print usage)
 *  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "helper.h"
#include "install.h"
#include "remove.h"
#include "query.h"

void print_usage(){
    
    printf("usage: bkgman <operation> [...]\n");
    printf("operations:\n");
    printf("  -I, --install <file>  Install package from file\n");
    printf("  -Q, --query <name>    Query package database\n");
    printf("  -R, --remove <name>   Remove package\n");
    printf("  -h, --help            Show this help message\n");

    return;
}

int main(int argc,char* argv[]){
    
    if(geteuid() != 0){
        fprintf(stderr, "Error: This command requires root privileges.\n");
        fprintf(stderr, "Please try: sudo your_program_name...\n");
        return 1;
    }

    if(argc<2){
        print_info("bkgman : bhargav's package manager");
    }
 
    char* op=argv[1];
    
    if(strcmp(op,"-I")==0 || strcmp(op,"--install")==0){
        if(argc<3){
            print_error("missing package file");
            return 1;
        }
        install_archive(argv[2]);
    }
    else if(strcmp(op,"-Q")==0 || strcmp(op,"--query")==0){
        if(argc<3){
            print_error("missing package file");
            return 1;
        }
        query_package(argv[2]);
    }
    else if(strcmp(op,"-R")==0 || strcmp(op,"--remove")==0){
        if(argc<3){
            print_error("missing package name");
            return 1;
        }
        remove_package(argv[2]);
    }
    else if(strcmp(op,"-h")==0 || strcmp(op,"--help")==0){
        print_usage();
    }
    else{
        print_error("invalid operation use -h for help");
        return 1;
    }

    return 0;
}

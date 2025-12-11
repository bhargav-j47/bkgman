#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "helper.h"
#include "query.h"

void query_package(const char* pkgname){
    //checks from db later check from sync as well
    char pkg_db[MAX_PATH];
    snprintf(pkg_db,sizeof(pkg_db),"%s/%s/desc",LOCAL_DB,pkgname);

    if(!file_exists(pkg_db)){
        print_error("pkg is not installed");
        exit(0);
    }
    /*
    FILE* fp=fopen(pkg_db,"r");
    if(!fp){
        print_error("pkg is corrupted");
        exit(0);
    }

    char line[MAX_LINE];

    while(fgets(line,sizeof(line),fp)){
        
        printf("  -> %s",line);

    } */

    char* keys[]={"pkgname","pkgver","pkgdesc","depends","builddate"};
    char* result;
    for(int i=0;i<5;i++){
        result=read_meta_key(pkg_db,keys[i]); //inefficient but okk for now
        printf("  %s : %s\n",keys[i],result);
    }
    free(result);
}

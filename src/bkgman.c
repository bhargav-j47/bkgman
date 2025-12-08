#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "config.h"
#include "install.h"
#include "remove.h"


int main(int argc,char* argv[]){

    if (argc < 3) {
        printf("Usage: %s <I/R> <package_name>\n", argv[0]);
        printf("   I : Install\n");
        printf("   R : Remove\n");
        return 1; // Return non-zero to indicate error
    }

    // 2. Get the mode character
    char mode = argv[1][0];

    // 3. STRICT LOGIC
    if (mode == 'I') {
        printf("Mode: Install\n");
        install_archive(argv[2]);
    } 
    else if (mode == 'R') {
        printf("Mode: Remove\n");
        remove_package(argv[2]);
    } 
    else {
        // 4. Handle invalid inputs gracefully
        printf("Error: Unknown mode '%c'. Please use 'I' or 'R'.\n", mode);
        return 1;
    }

    return 0;
}

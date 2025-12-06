#include "helper.h"
#include "config.h"
#include "install.h"

int main(int argc,char* argv[]){
    const char* msg="hi it is a msg";
    print_info(msg);
    print_success(argv[1]);
    install_archive(argv[1]);
    return 0;
}

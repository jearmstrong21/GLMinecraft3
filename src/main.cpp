#include <iostream>
#include <csignal>

#include "both.h"
#include <execinfo.h>

void sig_handler(int sig) {
    //https://stackoverflow.com/questions/9207599/printing-stack-trace-from-a-signal-handler
    int j, nptrs;
    void* buffer[100];
    char** strings;

    nptrs = backtrace(buffer, 100);
    printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);
    exit(sig);
}

int main(int argc, char** argv) {
    for (int i = 0; i < 32; i++) std::signal(i, sig_handler);
    if (argv[2][0] == 'c') {
        std::cout << "Starting client." << std::endl;
        networking::client();
    } else if(argv[2][0]=='s'){
        std::cout << "Starting server" << std::endl;
        networking::server();
    }
    std::cin.sync();
    std::cin.ignore(1);

    return 0;
}

#include <iostream>
#include <csignal>

#include "both.h"
#include <execinfo.h>
#include <boost/program_options.hpp>

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
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
    ("help","produce help message")
    ("client",boost::program_options::value<std::string>(),"connect client to host")
    ("server",boost::program_options::value<int>(),"run server on port");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc),vm);
    boost::program_options::notify(vm);
    if(vm.count("help")){
        std::cout<<desc<<"\n";
        return 1;
    }

    if(vm.count("client")&&vm.count("server")){
        std::cout<<"Both server and client are not currently supported.";
        std::cout<<desc<<"\n";
        return 1;
    }

    if(vm.count("client")){
        std::cout<<"Starting client on host "<<vm["client"].as<std::string>()<<"\n";
        networking::client(vm["client"].as<std::string>());
        return 0;
    }

    if(vm.count("server")){
        std::cout<<"Starting server on client "<<vm["server"].as<int>()<<"\n";
        networking::server(vm["server"].as<int>());
        return 0;
    }

    std::cout<<"Select an option.\n";
    std::cout<<desc<<"\n";

    //whats this stuff doing here
//    std::cin.sync();
//    std::cin.ignore(1);

    return 0;
}

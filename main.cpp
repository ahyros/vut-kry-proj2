#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>

#include <string.h>


#include "argParser.h"


int main(int argc, char ** argv) {
    auto programInput = parseArgs(argc, argv);
    std::cout << "ALL OK" << std::endl;
    std::cout << "Selected option: " << programInput.primaryOption << std::endl;
    std::cout << "Bytestream len: " << strlen(programInput.byteStream) << std::endl;
    std::cout << "Bytestream: " << programInput.byteStream << std::endl;
    free(programInput.byteStream);
    return 0;

}



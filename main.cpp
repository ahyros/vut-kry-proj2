#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>

#include <string.h>


#include "argParser.h"
#include "sha256.h"



int main(int argc, char ** argv) {
    auto programInput = parsePrimary(argc, argv);
    std::cout << "ALL OK" << std::endl;
    std::cout << "Selected option: " << programInput.primaryOption << std::endl;
    std::cout << "Bytestream len: " << programInput.streamSize << std::endl;
    std::cout << "Bytestream: " << programInput.byteStream << std::endl;
    if(programInput.primaryOption == "-c") {
        Message* msg = new Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        sha256(msg);
    }


    return 0;
}





void calculateSHA256() {

}

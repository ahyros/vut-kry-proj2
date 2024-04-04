/**
 * @file main.cpp
 * @author Andrej Hýroš
 * @date 03/04/2024
 * @brief This is the main source file of this project.
 */


#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>

#include <string.h>


#include "argParser.h"
#include "sha256.h"
#include "utils.h"


int main(int argc, char ** argv) {
    auto programInput = parsePrimary(argc, argv);
    std::cout << "ALL OK" << std::endl;
    std::cout << "Selected option: " << programInput.primaryOption << std::endl;
    std::cout << "Bytestream len: " << programInput.streamSize << std::endl;
    std::cout << "Bytestream: " << programInput.byteStream << std::endl;

    if(programInput.primaryOption == "-c") {
        Message msg = Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 H[HASH_SIZE];
        sha256(&msg, HInitial, H); // calculate hash

        // print out hash
        printHash(H);

        // free buffer
        free(msg.byteStream);
        return 1;
    }


    if(programInput.primaryOption == "-s") {
        Message msg = Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 MAC[HASH_SIZE];
        sha256(&msg, HInitial, MAC); // calculate MAC

        // print out result
        printHash(MAC);

        free(msg.byteStream);
        return 1;
    }


    if(programInput.primaryOption == "-v") {
        Message msg = Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 MAC[HASH_SIZE]; // calculated MAC
        uint32 enteredMAC[HASH_SIZE]; // MAC received from command line
        sha256(&msg, HInitial, MAC);
        stringHashToInt(programInput.secondaryOptions["-m"], enteredMAC);
        free(msg.byteStream);

        bool ok = true;
        for(int i = 0; i < HASH_SIZE; i++) {
            ok = ok && (MAC[i] == enteredMAC[i]);
        }
        if(ok) { DEBUG("OK"); return 0;} // TODO remove OK debug
        else {DEBUG("NOT OK"); return 1;}
    }


    if(programInput.primaryOption == "-e") {

    }
}

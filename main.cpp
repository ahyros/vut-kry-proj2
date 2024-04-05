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


    // Calculating SHA256 hash for entered message
    if(programInput.primaryOption == "-c") {
        Message msg = Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        DEBUG("TUT1");
        uint32 H[HASH_SIZE];
        sha256(&msg, HInitial, H); // calculate hash
        DEBUG("TUT10");
        // print out hash
        printHash(H);

        // free buffer
        free(msg.byteStream);
        return 1;
    }


    // Calculating MAC for entered message and key
    if(programInput.primaryOption == "-s") {
        Message msg = Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 MAC[HASH_SIZE];
        sha256(&msg, HInitial, MAC); // calculate MAC

        // print out result
        printHash(MAC);

        free(msg.byteStream);
        return 1;
    }


    // Validating MAC for entered MAC, message and key
    if(programInput.primaryOption == "-v") {
        Message msg = Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 MAC[HASH_SIZE]; // calculated MAC
        uint32 msgMAC[HASH_SIZE]; // MAC received from command line
        sha256(&msg, HInitial, MAC);
        stringHashToInt(programInput.secondaryOptions["-m"], msgMAC);
        free(msg.byteStream);

        bool ok = true;
        for(int i = 0; i < HASH_SIZE; i++) {
            ok = ok && (MAC[i] == msgMAC[i]);
        }
        if(ok) { DEBUG("OK"); return 0;} // TODO remove OK debug
        else {DEBUG("NOT OK"); return 1;}
    }


    // Performing lenght extension attack
    if(programInput.primaryOption == "-e") {
        Message msg = Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 msgMAC[HASH_SIZE];
        uint32 newMAC[HASH_SIZE];
        size_t extensionSize = programInput.secondaryOptions["-a"].size();
        stringHashToInt(programInput.secondaryOptions["-m"], msgMAC);
        sha256(&msg, msgMAC, newMAC, msg.sizeBits + extensionSize*8); // todo sprava musi byt z secondary argumentu, updatnut aj velkost spravy

        printHash(newMAC);
        // TODO tento print
        printFakeMessage(std::string(reinterpret_cast<char*>(programInput.byteStream)), programInput.secondaryOptions["-a"]);
    }
}


// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
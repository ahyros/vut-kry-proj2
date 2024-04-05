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

    // Calculating SHA256 hash for entered message
    if(programInput.primaryOption == "-c") {
        Message msg = Message(programInput.byteStream, programInput.streamSize);
        uint32 H[HASH_SIZE];
        sha256(&msg, H); // calculate hash
        // print out hash
        printHash(H);

        // free buffer
        free(msg.byteStream);
        return 1;
    }


    // Calculating MAC for entered message and key
    if(programInput.primaryOption == "-s") {
        Message msg = Message(programInput.byteStream, programInput.streamSize);
        uint32 MAC[HASH_SIZE];
        sha256(&msg, MAC); // calculate MAC

        // print out result
        printHash(MAC);

        free(msg.byteStream);
        return 1;
    }


    // Validating MAC for entered MAC, message and key
    if(programInput.primaryOption == "-v") {
        Message msg = Message(programInput.byteStream, programInput.streamSize);
        uint32 MAC[HASH_SIZE]; // calculated MAC
        uint32 msgMAC[HASH_SIZE]; // MAC received from command line
        sha256(&msg, MAC);
        stringHashToInt(programInput.secondaryOptions["-m"], msgMAC);
        free(msg.byteStream);

        bool ok = true;
        for(int i = 0; i < HASH_SIZE; i++) {
            ok = ok && (MAC[i] == msgMAC[i]);
        }
        return !ok;
    }


    // Performing lenght extension attack
    if(programInput.primaryOption == "-e") {
        uint32 msgMAC[HASH_SIZE];
        uint32 newMAC[HASH_SIZE];
        size_t extensionSize = programInput.secondaryOptions["-a"].size();
        size_t keyLen = std::stol(programInput.secondaryOptions["-n"]);
        stringHashToInt(programInput.secondaryOptions["-m"], msgMAC);
        AttackInfo attack = AttackInfo(extensionSize, msgMAC);

        auto byteStream = (byte*)malloc( extensionSize * sizeof(byte));
        if(byteStream == nullptr) throwError("[ERROR] Memory allocation failed...", ERR_MEM_ALLOCATION_FAILED);
        memcpy(byteStream, (byte*)programInput.secondaryOptions["-a"].c_str(), extensionSize); // copy it to heap

        Message msg = Message(byteStream, extensionSize);

        sha256(&msg, newMAC, &attack);

        printHash(newMAC);
        printFakeMessage(std::string(reinterpret_cast<char*>(programInput.byteStream)),
                         programInput.secondaryOptions["-a"],
                         keyLen);
    }
}


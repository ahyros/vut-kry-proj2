#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>

#include <string.h>


#include "argParser.h"
#include "sha256.h"
#include "utils.h"

// TODO uvolnujem byte buffer po zratani sha256???

int main(int argc, char ** argv) {
    auto programInput = parsePrimary(argc, argv);
    std::cout << "ALL OK" << std::endl;
    std::cout << "Selected option: " << programInput.primaryOption << std::endl;
    std::cout << "Bytestream len: " << programInput.streamSize << std::endl;
    std::cout << "Bytestream: " << programInput.byteStream << std::endl;

    if(programInput.primaryOption == "-c") {
        Message* msg = new Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 H[8];
        sha256(msg, H);
        free(msg); // TODO i probably dont even need this to be on the heap
    }

    if(programInput.primaryOption == "-s") {
        Message* msg = new Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 MAC[8];
        sha256(msg, MAC);
        free(msg);
    }
    if(programInput.primaryOption == "-v") {
        Message* msg = new Message(programInput.byteStream, programInput.streamSize, programInput.streamSize * 8);
        uint32 MAC[8];
        uint32 enteredMAC[8];
        sha256(msg, MAC);
        stringHashToInt(programInput.secondaryOptions["-m"], enteredMAC);

        for(int i = 0; i < 8; i++) {
            std::cout << std::hex << MAC[i] << " " << std::hex << enteredMAC[i] << std::endl;
        }
        bool ok = true;
        for(int i = 0; i < 8; i++) {
            ok = ok && (MAC[i] == enteredMAC[i]);
        }
        if(ok) DEBUG("MAC IS OK!");
        else DEBUG("MAC IS NOT OK!");
        DEBUG("\n");
        free(msg);
    }


        return 0;
}





void calculateSHA256() {

}

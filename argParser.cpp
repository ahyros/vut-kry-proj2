//
// Created by Andrej Hyros on 01/04/2024.
//
#include "argParser.h"


#define DEBUG(X) std::cout << X << std::endl


//ProgramInput parseArgs(int argc, char ** argv) {
//    if(argc == 1) displayUsage(); // Invalid number of args... print usage
//    if(!std::strcmp(argv[1], "-h")) displayUsage(); // For -h option show usage
//    else return parsePrimary(argc, argv);
//}

void displayUsage() {
    std::cout << "Usage: proj2 [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -c                                      Calculates checksum for message from STDIN" << std::endl;
    std::cout << "  -s [-k KEY]                             Calculates MAC for message from STDIN" << std::endl;
    std::cout << "  -v [-k KEY] [-m CHS]                    Checks if... TODO" << std::endl;
    std::cout << "  -e [-k KEY] [-m CHS] [-n NUM] [-a MSG]  Does whatever... TODO" << std::endl;
    exit(0); // program end with this message
}

/*
 * Following 4 functions checks, if arguments for individual
 * primary options (-c, -s, -v, -e) are legal. First argument
 * count is checked, then if correct secondary options were
 * entered in correct order (pairs `option argument`, where
 * option must start with symbol '-' and argument must not
 * start with symbol '-'). If illegal count/combination is
 * found, program ends with error.
 */

ProgramInput parsePrimary(int argc, char ** argv) {
    std::string primaryOption(argv[1]);

    if(argc == 1) displayUsage(); // Invalid number of args... show usage
    if(primaryOption == "-h") displayUsage(); // For -h option show usage

    // Throw error if no valid primary option is found
    if(!optConfigs.contains(primaryOption)) throwError("[ERROR] Invalid option. Use -h to see usage", ERR_INVALID_PRIMARY_OPTION);

    // get required secondary options (for found primary option) from options configurations map
    std::vector<std::string> secondaryOptions = optConfigs[primaryOption];

    // Multiply count of secondary options by two because theyre arguments also must be considered
    // Add 2 to account for program name and primary option
    if(argc != 2 + (secondaryOptions.size() * 2)) { // check if argument count is correct
        throwError("[ERROR] Illegal argument count with switch " + primaryOption + "...", ERR_INVALID_ARG_CNT);
    }

    // Parse secondary options along with theyre arguments
    // First two arguments are not considered secondary, therefore we skip them by doing +=2
    std::unordered_map<std::string, std::string> optArgs = parseSecondary(argv += 2);
    for(auto snd : secondaryOptions) { // iterate over secondary options and check if they are present in program's options
        if(!optArgs.contains(snd)) throwError("[ERROR] Missing option " + snd + "...", ERR_MISSING_OPTION);
    }

    // If everything is valid, load input and return it along with parsed program options
    return ProgramInput(argv[1], optArgs, readInput());
}


std::unordered_map<std::string, std::string> parseSecondary(char ** rest) {
    std::unordered_map<std::string, std::string> optArgs;
    while(*rest != nullptr) { // iterate over array
        // check that option and arguments are in correct  order (first option, then its argument)
        if((*rest)[0] != '-') throwError("[ERROR] Illegal argument combination", ERR_INCORECT_OPT_ARG_ORDER);
        if((*(rest+1))[0] == '-') throwError("[ERROR] Illegal argument combination", ERR_INCORECT_OPT_ARG_ORDER);
        optArgs.insert({*rest, *(rest+1)}); // insert option and its argument into map
        rest += 2;
        /*
         * It is worth noting, that if array's length is not even, segmentation fault will occur.
         * Therefore this must be checked before calling this function.
         */
    }
    return optArgs;
}


/**
 * @brief Read one line (= until `\n` character) from STDIN and stores it on the heap
 * @return Pointer to the inputted bytestream on the heap
 */
char* readInput() {
    std::string userInput;
    std::getline(std::cin, userInput); // read user input until `\n` is encountered
    auto byteStream = (char*)malloc(userInput.size() * sizeof(char));
    strcpy(byteStream, userInput.c_str()); // copy it to heap
    return byteStream;
}




void throwError(const std::string& errorMsg, int errorCode) {
    std::cerr << errorMsg << std::endl;
    exit(errorCode);
}
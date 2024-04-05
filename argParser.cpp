/**
 * @file argParser.cpp
 * @author Andrej Hýroš
 * @date 03/04/2024
 * @brief This file contains logic for parsing and validating command line arguments.
 */
#include "argParser.h"


void displayUsage() {
    std::cout << "Usage: proj2 [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -c                                      Calculates checksum for message from STDIN" << std::endl;
    std::cout << "  -s [-k KEY]                             Calculates MAC for message from STDIN" << std::endl;
    std::cout << "  -v [-k KEY] [-m CHS]                    Checks if entered MAC matches to entered message and key" << std::endl;
    std::cout << "  -e [-k KEY] [-m CHS] [-n NUM] [-a MSG]  Does whatever... TODO" << std::endl;
    exit(0); // program ends with this message
}


/*
 * Following function checks, if arguments for individual
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

    // For optimization purposses, some logic that would ideally be in other parts of the code is put here
    if(primaryOption == "-s" || primaryOption == "-v" || primaryOption == "-e") { // TODO tato podmineka zrefaktorovat
        // For -s, -v ane -e option, I want to avoid first loading user input and then prepending key,
        // because then I would have to allocate additional memory for new message, which is not ideal.
        auto input = readInputForS(optArgs["-k"]);
        return ProgramInput(primaryOption, optArgs, input.first, input.second);
    }

    // If everything is valid, load input and return it along with parsed program options
    auto input = readInput();
    return ProgramInput(primaryOption, optArgs, input.first, input.second);
}


std::unordered_map<std::string, std::string> parseSecondary(char ** rest) {
    std::unordered_map<std::string, std::string> optArgs;
    while(*rest != nullptr) { // iterate over array
        // check that option and arguments are in correct  order (first option, then its argument)
        if((*rest)[0] != '-') throwError("[ERROR] Illegal argument combination", ERR_INCORECT_OPT_ARG_ORDER);
        if((*(rest+1))[0] == '-') throwError("[ERROR] Illegal argument combination", ERR_INCORECT_OPT_ARG_ORDER);
        optArgs.insert({*rest, *(rest+1)}); // insert option and its argument into map
        rest += 2;
        /* It is worth noting, that if array's length is not even, segmentation fault will occur.
         * Therefore this must be checked before calling this function. */
    }
    return optArgs;
}


std::pair<byte*, size_t> readInput() {
    std::string userInput;
    std::getline(std::cin, userInput); // read user input until `\n` is encountered
    int size = userInput.size();

    auto byteStream = (byte*)malloc(size * sizeof(byte));
    if(byteStream == nullptr) throwError("[ERROR] Memory allocation failed...", ERR_MEM_ALLOCATION_FAILED);

    memcpy(byteStream, (byte*)userInput.c_str(), size); // copy it to heap

    return {byteStream, size};
}


std::pair<byte*, size_t> readInputForS(std::string key) {
    std::string userInput;
    std::getline(std::cin, userInput); // read user input until `\n` is encountered

    auto byteStream = (byte*)malloc((userInput.size() + key.size()) * sizeof(byte));

    if(byteStream == nullptr) throwError("[ERROR] Memory allocation failed...", ERR_MEM_ALLOCATION_FAILED);
    memcpy(byteStream, (byte*)key.c_str(), key.size());
    memcpy(byteStream + key.size(), (byte*)userInput.c_str(), userInput.size()); // copy it to heap

    return {byteStream, userInput.size() + key.size()};
}
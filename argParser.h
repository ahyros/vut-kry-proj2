/**
 * @file argParser.h
 * @author Andrej Hýroš
 * @date 03/04/2024
 */

#ifndef PROJ2_ARGPARSER_H
#define PROJ2_ARGPARSER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <utility>


#include "error.h"
#include "utils.h"

/**
 * @brief Struct for convenient manipulation with program's input data.
 * @details Contains primary option as a std::string member, and secondary options with
 * theyre arugments in dictionary (unordered map). Also contains pointer to byte array
 * with input from STDIN and its size.
 */
struct ProgramInput {
    std::string primaryOption;
    std::unordered_map<std::string, std::string> secondaryOptions;
    byte* byteStream;
    size_t streamSize;

    ProgramInput(std::string primaryOption,
            std::unordered_map<std::string, std::string> secondaryOptions,
            byte* byteStream,
            size_t streamSize) {
        this->primaryOption = primaryOption;
        this->secondaryOptions = secondaryOptions;
        this->byteStream = byteStream;
        this->streamSize = streamSize;
    }
};


/**
 * @brief Defines which primary options requre which secondary options.
 */
static std::unordered_map<std::string, std::vector<std::string>> optConfigs = {
        {"-c", {}},
        {"-s", {"-k"}},
        {"-v", {"-k", "-m"}},
        {"-e", {"-m", "-n", "-a"}}
};



/**
 * @brief Prints usage to STDOUT
 */
void displayUsage();

/**
 * @brief This function checks, if program's arguments are valid
 * @details If no arguments were entered, or -h was entered as primary argument, usage is shown and program ends.
 * If other valid primary option was entered (-c, -s, -v, -e), then corresponding secondary options are checked.
 * Any invalid combination, count, or secondary option leads to error and program ends. If everything is correct,
 * input from STDIN is read and returned.
 * @param argc Count of command line argument
 * @param argv Command line arguments
 * @return Data structure containing primary and secondary option(s), and loaded input in bytestream
 */
ProgramInput parsePrimary(int argc, char ** argv);

/**
 * @brief Reveives array of command line arguments (excluding program name and primary option), validates them,
 * and returnes them in more convenient data structure.
 * @details Function checks, if secondary options weere entered in correct order (pairs `option argument`, where
 * option must start with symbol '-' and argument must not start with symbol '-'). If illegal count/combination is
 * found, program ends with error.
 * @param rest Command line arguments excluding program name and primary option.
 * @return Secondary options with theyre respective arguments in asociative data structure.
 */
std::unordered_map<std::string, std::string> parseSecondary(char ** rest);

/**
 * @brief Read input from STDIN until symbol `\n` is encountered and then returns it as a byte array.
 * @return Pointer to byte array, where input is stored.
 */
std::pair<byte*, size_t> readInput();

/**
 * @brief Read input from STDIN until symbol `\n` is encountered, prepends key to it and then returns it as a byte array.
 * @param key Key for calculating MAC.
 * @return Pointer to byte array, where input with prepended key is stored.
 */
std::pair<byte*, size_t> readInputForS(std::string key);

#endif //PROJ2_ARGPARSER_H

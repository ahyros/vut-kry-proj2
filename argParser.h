//
// Created by Andrej Hyros on 01/04/2024.
//

#ifndef PROJ2_ARGPARSER_H
#define PROJ2_ARGPARSER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <utility>


#include "error.h"
#include "utils.h"


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

static std::unordered_map<std::string, std::vector<std::string>> optConfigs = {
        {"-c", {}},
        {"-s", {"-k"}},
        {"-v", {"-k", "-m"}},
        {"-e", {"-m", "-n", "-a"}}
};




void displayUsage();
ProgramInput parsePrimary(int argc, char ** argv);
std::unordered_map<std::string, std::string> parseSecondary(char ** rest);
std::pair<byte*, size_t> readInput();
std::pair<byte*, size_t> readInputForS(std::string key);

#endif //PROJ2_ARGPARSER_H

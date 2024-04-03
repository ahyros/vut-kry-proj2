//
// Created by Andrej Hyros on 01/04/2024.
//

#ifndef PROJ2_ARGPARSER_H
#define PROJ2_ARGPARSER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>


#include "error.h"

typedef unsigned char byte;


struct ProgramInput {
    std::string primaryOption;
    std::unordered_map<std::string, std::string> secondaryOptions;
    char* byteStream;

    ProgramInput(std::string primaryOption,
            std::unordered_map<std::string, std::string> secondaryOptions,
            char* byteStream) {
        this->primaryOption = primaryOption;
        this->secondaryOptions = secondaryOptions;
        this->byteStream = byteStream;
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
//ProgramInput parseArgs(int argc, char ** argv);
std::unordered_map<std::string, std::string> parseSecondary(char ** rest);
char* readInput();


#endif //PROJ2_ARGPARSER_H

//
// Created by Andrej Hyros on 01/04/2024.
//

#ifndef PROJ2_ARGPARSER_H
#define PROJ2_ARGPARSER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

// error 1 = nespravny pocet argument
// error 2 = prepinace a ich argumenty nie su v spravnom poradi
// error 3 = chybajuci option
// error 4 = ziadny spravny základny (prvy) option

#define ERR_INVALID_ARG_CNT 1
#define ERR_INCORECT_OPT_ARG_ORDER 2
#define ERR_MISSING_OPTION 3
#define ERR_INVALID_PRIMARY_OPTION 4


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

void throwError(const std::string& errorMsg, int errorCode);
std::unordered_map<std::string, std::string> parseSecondary(char ** rest);

char* readInput();


#endif //PROJ2_ARGPARSER_H

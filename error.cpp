//
// Created by Andrej Hyros on 03/04/2024.
//

#include "error.h"

void throwError(const std::string& errorMsg, int errorCode) {
    std::cerr << errorMsg << std::endl;
    exit(errorCode);
}
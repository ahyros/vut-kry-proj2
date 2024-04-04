/**
 * @file error.cpp
 * @author Andrej Hýroš
 * @date 03/04/2024
 * @brief This file implements error messages.
 */

#include "error.h"

void throwError(const std::string& errorMsg, int errorCode) {
    std::cerr << errorMsg << std::endl;
    exit(errorCode);
}
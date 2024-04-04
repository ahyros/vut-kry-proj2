/**
 * @file utils.h
 * @author Andrej Hýroš
 * @date 03/04/2024
 */

#ifndef PROJ2_UTILS_H
#define PROJ2_UTILS_H

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <bitset>
#include <vector>
#include <regex>

#include "error.h"

#define DEBUG(X) std::cout << X << std::endl
#define HEX_PATTTERN "[0-9A-Fa-f]+"

typedef uint32_t uint32;
typedef unsigned char byte;


uint32 closestMultiple(uint32 x, uint32 multiple);
void setBitAt(byte *buffer, size_t bitIndex);
void clearBitAt(byte *buffer, size_t bitIndex);
void setNBitsAt(byte *buffer, size_t bitIndex, size_t N);
void clearNBitsAt(byte *buffer, size_t bitIndex, size_t N);
void printByteStreamBinary(byte *buffer, size_t size);
uint32 copyRespectEndianness(byte* src);

/**
 * @brief Converts textual representation of MAC into numerical one.
 * @details MAC string must always be 64 characters long and in hexadecimal form.
 * @note This function will only work with MAC/HASH that is 32 bytes long!
 * @param strHex MAC as text
 * @param out Output buffer. Must be of size 8.
 */
void stringHashToInt(std::string strHex, uint32* out);

#endif //PROJ2_UTILS_H

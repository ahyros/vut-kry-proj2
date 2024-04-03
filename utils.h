//
// Created by Andrej Hyros on 03/04/2024.
//

#ifndef PROJ2_UTILS_H
#define PROJ2_UTILS_H

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <bitset>
#include <vector>

#include "error.h"

#define DEBUG(X) std::cout << X << std::endl

typedef uint32_t uint32;
typedef unsigned char byte;


uint32 closestMultiple(uint32 x, uint32 multiple);
void setBitAt(byte *buffer, size_t bitIndex);
void clearBitAt(byte *buffer, size_t bitIndex);
void setNBitsAt(byte *buffer, size_t bitIndex, size_t N);
void clearNBitsAt(byte *buffer, size_t bitIndex, size_t N);
void printByteStreamBinary(byte *buffer, size_t size);
uint32 copyRespectEndianness(byte* src);

#endif //PROJ2_UTILS_H

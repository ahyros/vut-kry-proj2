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
#include <iomanip>
#include <sstream>

#include "error.h"

#define DEBUG(X) std::cout << X << std::endl
#define HEX_PATTTERN "[0-9A-Fa-f]+" // TODO toto lepšie ne?: ^[A-Fa-f0-9]{64}$
#define WORD_SIZE 32
#define BLOCK_SIZE 512
#define BLOCK_SIZE_BYTES 64
#define HASH_SIZE 8

typedef uint32_t uint32;
typedef unsigned char byte;


/**
 * @brief Finds closes larger multiple of certain number.
 * @param x Closest multiple of number `multiple` to this number will be returned.
 * @param multiple Multiple of this number will be returned.
 * @return Closest multiple of number `multiple` to number `x`
 */
uint32 closestMultiple(uint32 x, uint32 multiple);

/**
 * @brief Sets individual bit to `1` in byte buffer.
 * @param buffer Byte buffer
 * @param bitIndex Specifies bit to be set. There are `size(buffer)*8` possible indices for any buffer.
 */
void setBitAt(byte *buffer, size_t bitIndex);

/**
 * @brief Sets individual bit to `0` in byte buffer.
 * @param buffer Byte buffer
 * @param bitIndex Specifies bit to be set. There are `size(buffer)*8` possible indices for any buffer.
 */
void clearBitAt(byte *buffer, size_t bitIndex);

/**
 * @brief Sets block of N bits to `1` in byte buffer.
 * @param buffer Byte buffer
 * @param bitIndex Specifies the first bit to be set.
 * @param N Specifies, how many bits will be set.
 */
void setNBitsAt(byte *buffer, size_t bitIndex, size_t N);

/**
 * @brief Sets block of N bits to `0` in byte buffer.
 * @param buffer Byte buffer
 * @param bitIndex Specifies the first bit to be set.
 * @param N Specifies, how many bits will be set.
 */
void clearNBitsAt(byte *buffer, size_t bitIndex, size_t N);

/**
 * @brief Prints byte stream to STDOUT in binary form. Used only for debugging.
 * @param buffer Byte buffer.
 * @param size Size of the buffer.
 */
void printByteStreamBinary(byte *buffer, size_t size);

/**
 * Prints out HASH as a continous string.
 * @param H Array of 8 32bit unsigned integeres which contains the hash.
 */
void printHash(uint32* H);

/**
 * Retrieves single unsigned 32 bit integer from byte buffer with respect to endianness.
 * @param src Pointer to byte buffer where 32bit integer is stored.
 * @return Unsigned 32 bit integer.
 */
uint32 copyRespectEndianness(byte* src);

/**
 * @brief Converts textual representation of MAC into numerical one.
 * @details MAC string must always be 64 characters long and in hexadecimal form.
 * @note This function will only work with MAC/HASH that is 32 bytes long!
 * @param strHex MAC as text
 * @param out Output buffer. Must be of size 8.
 */
void stringHashToInt(std::string strHex, uint32* out);
void printBytes(uint64_t value);
void printFakeMessage(std::string msg, std::string extension);
#endif //PROJ2_UTILS_H


// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

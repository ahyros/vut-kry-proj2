/**
 * @file utils.h
 * @author Andrej Hýroš
 * @date 03/04/2024
 * @brief This file implements various universal utility/helper functions that do not fit in any other specific file.
 */

#include "utils.h"

// TODO add comments/documentation


/**
 * @brief Finds closes larger multiple of certain number.
 * @param x Closest multiple of number `multiple` to this number will be returned.
 * @param multiple Multiple of this number will be returned.
 * @return Closest multiple of number `multiple` to number `x`
 */
uint32 closestMultiple(uint32 x, uint32 multiple)
{
    if (multiple == 0) return x;

    int remainder = x % multiple;
    if (remainder == 0) return x;

    return x + multiple - remainder;
}

/**
 * @brief Sets individual bit to `1` in byte buffer.
 * @param buffer Byte buffer
 * @param bitIndex Specifies bit to be set. There are `size(buffer)*8` possible indices for any buffer.
 */
void setBitAt(byte *buffer, size_t bitIndex) {
    auto byteIndex = bitIndex / 8;
    buffer[byteIndex] |= (0b10000000 >> (bitIndex % 8));
}

/**
 * @brief Sets individual bit to `0` in byte buffer.
 * @param buffer Byte buffer
 * @param bitIndex Specifies bit to be set. There are `size(buffer)*8` possible indices for any buffer.
 */
void clearBitAt(byte *buffer, size_t bitIndex) {
    auto byteIndex = bitIndex / 8;
    buffer[byteIndex] &= ~(0b10000000 >> (bitIndex % 8));
}

/**
 * @brief Sets block of N bits to `1` in byte buffer.
 * @param buffer Byte buffer
 * @param bitIndex Specifies the first bit to be set.
 * @param N Specifies, how many bits will be set.
 */
void setNBitsAt(byte *buffer, size_t bitIndex, size_t N) {
    auto byteIndex = bitIndex / 8;
    // index of the first bit of closest whole byte
    auto closestByteBitIndex = closestMultiple(bitIndex, 8);
    // index of closes byte
    auto closesByteIndex = closestByteBitIndex / 8;
    // number of bits before whole bytes
    auto headBitsSize = closestByteBitIndex - bitIndex;
    // number of bits after whole bytes
    auto tailBitsSize = (N - headBitsSize) % 8;
    // number of whole bytes to be set
    auto wholeBytesCount = (N - headBitsSize - tailBitsSize) / 8;
    // setting whole bytes
    for(int i = closesByteIndex; i < closesByteIndex + wholeBytesCount; i++) {
        buffer[i] = UCHAR_MAX;
    }
    // setting head
    for(int i = bitIndex; i < bitIndex + headBitsSize; i++) setBitAt(buffer, i);
    // setting tail
    for(int i = bitIndex + N; i >  bitIndex + N - tailBitsSize; i--) setBitAt(buffer, i);
}

/**
 * @brief Sets block of N bits to `0` in byte buffer.
 * @param buffer Byte buffer
 * @param bitIndex Specifies the first bit to be set.
 * @param N Specifies, how many bits will be set.
 */
void clearNBitsAt(byte *buffer, size_t bitIndex, size_t N) {
    auto byteIndex = bitIndex / 8;
    // index of the first bit of closest whole byte
    auto closestByteBitIndex = closestMultiple(bitIndex, 8);
    // index of closes byte
    auto closesByteIndex = closestByteBitIndex / 8;
    // number of bits before whole bytes
    auto headBitsSize = closestByteBitIndex - bitIndex;
    // number of bits after whole bytes
    auto tailBitsSize = (N - headBitsSize) % 8;
    // number of whole bytes to be set
    auto wholeBytesCount = (N - headBitsSize - tailBitsSize) / 8;
    // setting whole bytes
    for(int i = closesByteIndex; i < closesByteIndex + wholeBytesCount; i++) {
        buffer[i] = 0;
    }
    // setting head
    for(int i = bitIndex; i < bitIndex + headBitsSize; i++) clearBitAt(buffer, i);
    // setting tail
    for(int i = bitIndex + N; i >  bitIndex + N - tailBitsSize; i--) clearBitAt(buffer, i);
}

/**
 * @brief Prints byte stream to STDOUT in binary form. Used only for debugging.
 * @param buffer Byte buffer.
 * @param size Size of the buffer.
 */
void printByteStreamBinary(byte *buffer, size_t size) {
    std::bitset<8> bits;
    int j = 1;
    // iterate over buffer and print individual bytes in binary representation
    for(int i = 0; i < size; i++) {
        bits = buffer[i];
        if(j % 8) std::cout << bits << " ";
        else std::cout << bits << std::endl;
        j++;
    }
    std::cout << "\n\n";
}

/**
 * Retrieves single unsigned 32 bit integer from byte buffer with respect to endianness.
 * @param src Pointer to byte buffer where 32bit integer is stored.
 * @return Unsigned 32 bit integer.
 */
uint32 copyRespectEndianness(byte* src) {
    uint32 res = 0;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    res |= (src[0] << 24);
    res |= (src[1] << 16);
    res |= (src[2] << 8);
    res |= (src[3]);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    res |= (src[0]);
    res |= (src[1] << 8);
    res |= (src[2] << 16);
    res |= (src[3] << 24);
#else
    throwError("[ERROR] Failed to get device's endiannes...", ERR_ENDIANNESS_NOT_DEFINED);
#endif
    return res;
}


void stringHashToInt(std::string strHex, uint32* out) {
    if(strHex.size() != 64) {
        throwError("[ERROR] Entered MAC/HASH must consist of 64 hexadecimal characters...", ERR_INVALID_HASH_ENTERED);
    }
    if (!std::regex_match(strHex, std::regex(HEX_PATTTERN))) {
        throwError("[ERROR] Entered MAC/HASH must consist of 64 hexadecimal characters...", ERR_INVALID_HASH_ENTERED);
    }
    for(int i = 0; i < 8; i++) {
        //std::cout << "" << << std::endl;
        auto hexInt32 = strHex.substr(8*i, 8);
        //DEBUG(hexInt32.size());
        uint32 number = std::stol(hexInt32, nullptr, 16);
        out[i] = number;
//        std::cout << "Iteration: " << i << std::endl;
//        std::cout << "Substring: " << hexInt32 << std::endl;
//        std::cout << "Converted: " << number << std::endl;
        //std::cout << "" << << std::endl;
        //std::cout << number << " ";
    }
}
//
// 4
// d428a3a0
// fb29153e
// 464c56af
// 9751b802
// 75fa9c64
// 1b9b1d82
// 384f0139
//


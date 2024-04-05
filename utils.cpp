/**
 * @file utils.h
 * @author Andrej Hýroš
 * @date 03/04/2024
 * @brief This file implements various universal utility/helper functions that do not fit in any other specific file.
 */

#include "utils.h"


uint32 closestMultiple(uint32 x, uint32 multiple)
{
    if (multiple == 0) return x;

    int remainder = x % multiple;
    if (remainder == 0) return x;

    return x + multiple - remainder;
}


void setBitAt(byte *buffer, size_t bitIndex) {
    auto byteIndex = bitIndex / 8;
    buffer[byteIndex] |= (0b10000000 >> (bitIndex % 8));
}


void clearBitAt(byte *buffer, size_t bitIndex) {
    auto byteIndex = bitIndex / 8;
    buffer[byteIndex] &= ~(0b10000000 >> (bitIndex % 8));
}


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
    // There will be one iteration for each 32bit hash value
    for(int i = 0; i < 8; i++) {
        // extract indexed hash value, convert it to integer and store it to the output buffer
        auto hexInt32 = strHex.substr(8*i, 8);
        uint32 number = std::stol(hexInt32, nullptr, 16);
        out[i] = number;
    }
}

void printHash(uint32 *H) {
    for(int i = 0; i < HASH_SIZE; i++) {
        std::cout << std::hex << H[i];
    }
    std::cout << "\n";
}

void printFakeMessage(std::string msg, std::string extension, size_t keyLen) {
    std::string fakeMsg = msg + "\\x80"; // append fist `1` bit to the message
    size_t msgSizeBits = (msg.size() + keyLen) * 8;
    size_t msgSizeBitsEndian = __builtin_bswap64(msgSizeBits); // calculate message size in big endian
    size_t paddedBitCount = closestMultiple(msgSizeBits, BLOCK_SIZE);
    uint32 k = 448 - (msgSizeBits + 1);

    for(int i = 0; i < k/8; i++) fakeMsg += "\\x00"; // append empty bytes
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&msgSizeBitsEndian);
    std::stringstream ss;
    // append last 8 bytes (size of message)
    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[0]);
    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[1]);
    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[2]);
    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[3]);
    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[4]);
    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[5]);
    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[6]);
    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[7]);
    fakeMsg += ss.str();
    fakeMsg += extension; // finally append the new content which extends the original message
    std::cout << fakeMsg << std::endl; // print whole message on STDOUT
}
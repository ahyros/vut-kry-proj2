/**
 * @file sha256.cpp
 * @author Andrej Hýroš
 * @date 03/04/2024
 * @brief This file implements SHA256 hashing algorithm.
 */

#include "sha256.h"

void sha256(Message* msg, const uint32* H0, uint32* result, uint64_t len) {
    // preprocessing
    DEBUG("TUT1");
    //paddMessage(msg, len);
    paddMessage2(msg);
    auto blocks = parseMessage(msg);

    http://doorduino/unlock?nonce=42
    // hash calculation
    size_t N = msg->sizeBits / BLOCK_SIZE;
    uint32 a,b,c,d,e,f,g,h;
    uint32 T1, T2;
    uint32 W[64], H[HASH_SIZE];
    memset(W, 0, 64*sizeof(uint32));

    memcpy(H, H0, HASH_SIZE * sizeof(uint32));

    // hash calculation. Implemented according to specification FIPS PUB 180-4 (https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
    for(int i = 1; i <= N; i++) {
        // prepare message schedule
        for(int t=0; t < 64; t++) {
            if(0 <= t && t <= 15)
                W[t] = copyRespectEndianness(blocks.at(i-1) + t * sizeof(uint32 ));
            else
                W[t] = sigma1(W[t-2]) + W[t-7] + sigma0(W[t-15]) + W[t-16];
        }

        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];
        f = H[5];
        g = H[6];
        h = H[7];

        for(int t = 0; t <= 63; t++) {
            T1 = h + sum1(e) + Ch(e, f, g) + K[t] + W[t];
            T2 = sum0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h + H[7];
        printHash(H);
    }
    // store values to result pointer
    for(int i = 0; i < 8; i++) {
        result[i] = H[i];
    }
}


void paddMessage2(Message* msg) {
    uint64_t paddedSizeBits = closestMultiple(msg->sizeBits + 65, BLOCK_SIZE);
    uint64_t paddedSizeBytes = paddedSizeBits / 8;

    byte* padded = (byte*)malloc(paddedSizeBits/8);
    if(!padded) throwError("[ERROR] Memory allocation failed...", ERR_MEM_ALLOCATION_FAILED);
    uint64_t msgSize64BigEndian = __builtin_bswap64(msg->sizeBits);

    // set extended (padded) part of new message to zeros
    memset(padded + msg->sizeBytes, 0x00, paddedSizeBytes - msg->sizeBytes);

    // copy origanal message
    memcpy(padded, msg->byteStream, msg->sizeBytes);

    // set very first bit after original message to one
    setBitAt(padded, msg->sizeBits);

    // set last 64 bits of padded message to contain length of original message in bits (mind the endianness!)
    memcpy(padded + paddedSizeBytes - 8, &msgSize64BigEndian, 8);

    // Update data structure containing the message
    free(msg->byteStream);
    msg->byteStream = padded;
    msg->sizeBits = paddedSizeBits;
    msg->sizeBytes = paddedSizeBytes;
}


void paddMessage(Message* msg, uint64_t attackSize) {
    size_t paddedBitCount = closestMultiple(msg->sizeBits, BLOCK_SIZE);
    size_t totalByteCount = paddedBitCount / 8;

    byte* padded = (byte*)malloc(totalByteCount); // prepare larger buffer for padded message
    if(padded == nullptr) throwError("[ERROR] Memory allocation failed...", ERR_MEM_ALLOCATION_FAILED);

    // calculate value `k`
    uint32 k = 448 - (msg->sizeBits + 1);
    uint64_t msgSize64BigEndian = 0;

    // convert number expressing original message size to 64bit big endian. // todo doc for attack
    if(!attackSize) msgSize64BigEndian = __builtin_bswap64(msg->sizeBits);
    else msgSize64BigEndian = __builtin_bswap64(attackSize);
    // *** Actuall padding starts here ***
    // copy orignal message into prepared buffer for padded message
    memcpy(padded, msg->byteStream, msg->sizeBytes);

    // Append `1` bit to the orignal message
    setBitAt(padded, msg->sizeBits);

    // Append `k` zero bits
    clearNBitsAt(padded, msg->sizeBits+1, k); // todo mozno by bolo lepsie cely buffer az do konca vynulovat.
    // todo v pripade extension attacku sa mi tu mozno moze nieco dosrat, aj ked dlzka by mala pri nom mat vzdy viac bitov

    // Finally append 64-bit block that is equal to the bit count of original message
    memcpy(padded + totalByteCount - 8, &msgSize64BigEndian, 8);
//    printByteStreamBinary(padded, totalByteCount);

    // Update data structure containing the message
    free(msg->byteStream);
    msg->byteStream = padded;
    msg->sizeBits = paddedBitCount;
    msg->sizeBytes = totalByteCount;
}

std::vector<byte*> parseMessage(Message* msg) {
    DEBUG("PARSING");
    auto blocksCount = msg->sizeBits / BLOCK_SIZE;
    std::vector<byte*> outputVec;
    for(int i = 0; i < blocksCount; i++) outputVec.push_back(msg->byteStream + (BLOCK_SIZE_BYTES * i));

    for(auto a : outputVec) {
        DEBUG((uint64_t)a);
    }

    return outputVec;
}



/*
 * Bellow are helper functions used in hash calculation.
 * Implemented according to specification FIPS PUB 180-4
 * (https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
 */

uint32 Ch(uint32 x, uint32 y, uint32 z) {
    return (x & y) ^ (~x & z);
}

uint32 Maj(uint32 x, uint32 y, uint32 z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32 rotr(uint32 x, uint32 n) {
    return (x >> n) | (x << (WORD_SIZE - n));
}

uint32 rotl(uint32 x, uint32 n) {
    return (x << n) | (x >> (WORD_SIZE - n));
}

uint32 sum0(uint32 x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

uint32 sum1(uint32 x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

uint32 sigma0(uint32 x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

uint32 sigma1(uint32 x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}










//
// Created by Andrej Hyros on 03/04/2024.
//

#include "sha256.h"


void sha256(Message *msg) {
    paddMessage(msg);
    auto blocks = parseMessage(msg);
    printByteStreamBinary(msg->byteStream, msg->sizeBytes);
    calculateHash(msg, blocks);
}

void calculateHash(Message* msg, std::vector<byte*> blocks) {
    size_t N = msg->sizeBits / BLOCK_SIZE;
    uint32 a,b,c,d,e,f,g,h;
    uint32 T1 = 0, T2 = 0, tmp = 0;
    uint32 W[64];
    memset(W, 0, 64*sizeof(uint32));
    DEBUG(blocks.size());
    uint32 H[8] = {
            0x6a09e667,
            0xbb67ae85,
            0x3c6ef372,
            0xa54ff53a,
            0x510e527f,
            0x9b05688c,
            0x1f83d9ab,
            0x5be0cd19,
    };

    uint32* test;
    memset(test, 0, 4);
    //memcpy(reinterpret_cast<void*>(test), blocks.at(0) + 0 * sizeof(uint32), sizeof(uint32));
    *test = copyRespectEndianness(blocks.at(0));
    DEBUG("TUT:");
    DEBUG(std::bitset<32>(*test));
    DEBUG((uint64_t)(blocks.at(0)));
    DEBUG((uint64_t)(msg->byteStream));


    for(int i = 1; i <= N; i++) {
        // prepare message schedule
        for(int t=0; t < 64; t++) {
            if(0 <= t && t <= 15)
                W[t] = copyRespectEndianness(blocks.at(i-1) + t * sizeof(uint32 ));
            else
                W[t] = sigma1(W[t-2]) + W[t-7] + sigma0(W[t-15]) + W[t-16];

            std::cout << "W[" << t << "]= " << std::bitset<32>(W[t]) << std::endl;
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

//            std::cout << "T1[" << t << "]= " << std::hex << T1 << std::endl;
//            std::cout << "T2[" << t << "]= " << std::hex << T2 << std::endl << std::endl;

            std::cout << "T1[" << t << "]= " << std::bitset<32>(T1) << std::endl;
            std::cout << "T2[" << t << "]= " << std::bitset<32>(T2) << std::endl << std::endl;
        }

        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h + H[7];

    }
    std::bitset<32> bits;
    for(int i = 0; i < 8; i++) {
        bits = H[i];
        //std::cout << bits << " ";
        std::cout << std::hex << H[i];
    }
    std::cout << "\n\n";
}





void paddMessage(Message* msg) {
    size_t paddedBitCount = closestMultiple(msg->sizeBits, BLOCK_SIZE);
    size_t totalByteCount = paddedBitCount / 8;

    byte* padded = (byte*)malloc(totalByteCount); // prepare larger buffer for padded message
    if(padded == nullptr) throwError("[ERROR] Memory allocation failed...", ERR_MEM_ALLOCATION_FAILED);

    // calculate value `k` nad convert number expressing original message size to 64bit big endian
    uint32 k = 448 - (msg->sizeBits + 1);
    uint64_t msgSize64BigEndian = __builtin_bswap64 (msg->sizeBits);

    // *** Actuall padding starts here ***
    // copy orignal message into prepared buffer for padded message
    memcpy(padded, msg->byteStream, msg->sizeBytes);

    // Append `1` bit to the orignal message
    setBitAt(padded, msg->sizeBits);

    // Append `k` zero bits
    clearNBitsAt(padded, msg->sizeBits+1, k);

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
    auto blocksCount = msg->sizeBits / BLOCK_SIZE;
    std::vector<byte*> outputVec;
    for(int i = 0; i < blocksCount; i++) outputVec.push_back(msg->byteStream + (BLOCK_SIZE * i));
    return outputVec;
}




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

uint32 add(uint32 x, uint32 y) {
    return (x + y) % UINT32_MAX;
}










#include "base64.h"

static const uint8_t b64dict[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static const uint8_t b64revdict[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62,
    0,  0,  0,  63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,  0,  0,
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 0,  0,  0,  0,  0,  0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0,  0,  0,  0
};

std::string Base64::encode(const std::string &input)
{
    return encode((uint8_t *) input.c_str(), input.size());
}

std::string Base64::encode(const uint8_t *bytes, size_t length)
{
    char chunk[4];
    size_t pos = 0;
    std::string out;
    while (pos < length) {
        int chunk_size = 0;

        chunk = { 0, 0, 0, 0 };
        for (chunk_size = 0; chunk_size < 3 && pos < length; chunk_size++, pos++) {
            chunk[chunk_size] = bytes[pos];
        }

        encode_chunk(chunk);
        out.append(chunk, 4);
    }
    return out;
}

std::string Base64::decode(const std::string &input)
{
    char chunk[4];
    std::string out;
    size_t pos = 0;
    while (pos < input.size()) {
        for (size_t i = 0; i < 4 && pos < input.size(); i++, pos++) {
            // lets just skip incorrect chars
            if (input[pos] >= sizeof(b64revdict) || !b64revdict[input[pos]]) {
                i--;
                continue;
            }

            chunk[i] = input[pos];
        }
        decode_chunk(chunk);
        out.append(chunk, 3);
    }
    return out;
}

void Base64::encode_chunk(char *chunk)
{
    chunk[3] = chunk[2] ? chunk[2] & 0b00111111 : 64;
    chunk[2] = chunk[1] ? ((chunk[2] & 0b11000000) >> 6) | ((chunk[1] & 0b00001111) << 2) : 64;
    chunk[1] = ((chunk[1] & 0b11110000) >> 4) | ((chunk[0] & 0b00000011) << 4);
    chunk[0] = ((chunk[0] & 0b11111100) >> 2);
    for (int i = 0; i < 4; i++) {
        chunk[i] = b64dict[chunk[i]];
    }
}

void Base64::decode_chunk(char *chunk)
{
    for (int i = 0; i < 4; i++) {
        chunk[i] = b64revdict[chunk[i]];
    }
    chunk[0] = ((chunk[0] << 2) & 0b11111100) | ((chunk[1] >> 4) & 0b00000011);
    chunk[1] = ((chunk[1] << 4) & 0b11110000) | ((chunk[2] >> 2) & 0b00001111);
    chunk[2] = ((chunk[2] << 6) & 0b11000000) | ((chunk[3] >> 0) & 0b00111111);
    chunk[3] = 0;
}

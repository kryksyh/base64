#ifndef BASE64_H
#define BASE64_H

#include <string>

class Base64 {
public:
    static std::string encode(const std::string &input);
    static std::string encode(const uint8_t *bytes, size_t length);
    static std::string decode(const std::string &input);

private:
    static void encode_chunk(char *chunk);
    static void decode_chunk(char *chunk);
};

#endif // BASE64_H

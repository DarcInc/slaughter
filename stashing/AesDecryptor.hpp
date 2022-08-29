//
// Created by phoehne on 8/28/22.
//

#include <iostream>
#include <memory>

#include <openssl/evp.h>

#ifndef SLAUGHTER_AESDECRYPTOR_HPP
#define SLAUGHTER_AESDECRYPTOR_HPP

namespace Stashing {

    class AesDecryptor {
        bool extractSalt(std::istream& cipher, unsigned char* salt);
        bool extractSalt(unsigned char* cipherText, unsigned char* salt);
        int computeKey(unsigned char* salt, const std::string& passPhrase, unsigned char* outKey, unsigned char* outIv);
        EVP_CIPHER_CTX* openContex(unsigned char* key, unsigned char* iv, const int& keyLength);
    public:
        void descrypt(std::istream &cipher, std::ostream& plain, const std::string& passphrase);
        std::unique_ptr<unsigned char[]> decrypt(unsigned char* cipherText, int cipherTextSize, const std::string& passphrase);
    };
}

#endif //SLAUGHTER_AESDECRYPTOR_HPP

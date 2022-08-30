//
// Created by phoehne on 8/28/22.
//

#include <iostream>
#include <memory>

#include <openssl/evp.h>

#ifndef SLAUGHTER_AESENCRYPTOR_HPP
#define SLAUGHTER_AESENCRYPTOR_HPP

namespace Stashing {
    class AesEncryptor {
        int initializeKey(unsigned char* outSalt, unsigned char* outKey, unsigned char* outIv, const std::string& passPhrase);
        EVP_CIPHER_CTX* openContext(unsigned char* key, int keySize, unsigned char* iv);
    public:
        void encrypt(std::istream& clearText, std::ostream& cipherText, const std::string& passPhrase);
        std::unique_ptr<unsigned char[]> encrypt(void* clearText, int clearTextSize, const std::string& passphrase);

    };
}

#endif //SLAUGHTER_AESENCRYPTOR_HPP

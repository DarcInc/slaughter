//
// Created by phoehne on 8/28/22.
//

#include <iostream>
#include <memory>

#include <openssl/evp.h>

#ifndef SLAUGHTER_AESENCRYPTOR_HPP
#define SLAUGHTER_AESENCRYPTOR_HPP

namespace Stashing {
    class Block;

    class AesEncryptor {
        int initializeKey(unsigned char* outSalt, unsigned char* outKey, unsigned char* outIv, const std::string& passPhrase);
        EVP_CIPHER_CTX* openContext(unsigned char* key, int keySize, unsigned char* iv);

        /**
         * Always round the encrypted data size up to the next full block, given the salt
         * that will need to be written into the data.
         *
         * @param clearSize The size of the clear text.
         * @return The size compensating for block alignment and overhead.
         */
        size_t expectedEncryptedSize(size_t clearSize);

        const static std::string SALT_STRING;
        const static size_t AES_CBC_BLOCK_SIZE_BYTES;
    public:
        void encrypt(std::istream& clearText, std::ostream& cipherText, const std::string& passPhrase);
        std::unique_ptr<Block> encrypt(Block* block, const std::string& passphrase);
        std::unique_ptr<Block> encrypt(void* rawMemory, size_t memorySize, const std::string& passphrase);

    };
}

#endif //SLAUGHTER_AESENCRYPTOR_HPP

//
// Created by phoehne on 8/28/22.
//

#include "AesEncryptor.hpp"
#include "Block.hpp"

#include <cstring>
#include <exception>

#include <openssl/evp.h>
#include <openssl/rand.h>

namespace Stashing {
    const std::string AesEncryptor::SALT_STRING = "Salted__";
    const size_t AesEncryptor::AES_CBC_BLOCK_SIZE_BYTES = EVP_CIPHER_get_block_size(EVP_aes_256_cbc());

    int AesEncryptor::initializeKey(unsigned char* outSalt, unsigned char* outKey, unsigned char* outIv, const std::string& passPhrase) {
        RAND_bytes(outSalt, PKCS5_SALT_LEN);

        int keySize = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), outSalt,
                                     (unsigned char*)passPhrase.c_str(), passPhrase.length(), 1,
                                     outKey, outIv);
        if (keySize == 0) {
            throw std::runtime_error("Failed to convert salt and passphrase to key and iv");
        }
        return keySize;
    }

    EVP_CIPHER_CTX* AesEncryptor::openContext(unsigned char* key, int keySize, unsigned char* iv) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (ctx == nullptr) {
            throw std::runtime_error("Failed to create a new context for encryption");
        }

        try {
            if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
                throw std::runtime_error("Failed to initialize a new context for encryption");
            }

            if (1 != EVP_CIPHER_CTX_set_key_length(ctx, keySize)) {
                throw std::runtime_error("Failed to set key length on the newly initialized context");
            }
        } catch(...) {
            EVP_CIPHER_CTX_free(ctx);
            throw;
        }
        return ctx;
    }

    size_t AesEncryptor::expectedEncryptedSize(size_t clearSize) {
        size_t result = clearSize + SALT_STRING.size() + PKCS5_SALT_LEN;
        if (result % AES_CBC_BLOCK_SIZE_BYTES != 0) {
            result = (result / AES_CBC_BLOCK_SIZE_BYTES + 1) * AES_CBC_BLOCK_SIZE_BYTES;
        }
        return result;
    }

    void AesEncryptor::encrypt(std::istream& clearText, std::ostream& cipherText, const std::string& passPhrase) {
        unsigned char key[EVP_MAX_KEY_LENGTH];
        unsigned char iv[EVP_MAX_IV_LENGTH];
        unsigned char salt[PKCS5_SALT_LEN];
        EVP_CIPHER_CTX *ctx = nullptr;

        int keySize = initializeKey(salt, key, iv, passPhrase);
        ctx = openContext(key, keySize, iv);

        cipherText.write("Salted__", 8);
        cipherText.write((char*)salt, PKCS5_SALT_LEN);

        unsigned char clearBuffer[32 * 1024];
        unsigned char cipherBuffer[32 * 1024];
        int cipherBytes;

        do {
            clearText.read((char *) clearBuffer, sizeof(clearBuffer));
            auto bytesRead = clearText.gcount();
            EVP_EncryptUpdate(ctx, cipherBuffer, &cipherBytes, clearBuffer, bytesRead);
            cipherText.write((char *) cipherBuffer, cipherBytes);
        } while(clearText.good());

        EVP_EncryptFinal_ex(ctx, cipherBuffer, &cipherBytes);
        cipherText.write((char*)cipherBuffer, cipherBytes);
    }

    std::unique_ptr<Block> AesEncryptor::encrypt(Block* block, const std::string& passphrase) {
        return encrypt(block->m_rawMemory, block->m_used, passphrase);
    }

    std::unique_ptr<Block> AesEncryptor::encrypt(void *rawMemory, size_t memorySize, const std::string &passphrase) {
        unsigned char key[EVP_MAX_KEY_LENGTH];
        unsigned char iv[EVP_MAX_IV_LENGTH];
        EVP_CIPHER_CTX *ctx = nullptr;
        const size_t expectedSize = expectedEncryptedSize(memorySize);

        std::unique_ptr<Block> result = std::make_unique<Block>(expectedSize);

        try {
            result->write((void*)SALT_STRING.c_str(), SALT_STRING.size());

            unsigned char salt[PKCS5_SALT_LEN];
            int keySize = initializeKey(salt, key, iv, passphrase);

            result->write(salt, PKCS5_SALT_LEN);

            ctx = openContext(key, keySize, iv);

            int cipherSize;
            if (1 != EVP_EncryptUpdate(ctx, (unsigned char*)result->m_rawMemory + result->m_used, &cipherSize,
                                       (unsigned char *) rawMemory, memorySize)) {
                throw std::runtime_error("Failed to write data to encryption context");
            }
            result->m_used += cipherSize;

            if (1 != EVP_EncryptFinal_ex(ctx, (unsigned char*)result->m_rawMemory + result->m_used, &cipherSize)) {
                throw std::runtime_error("Unable to finalize encryption");
            }
            result->m_used += cipherSize;

            EVP_CIPHER_CTX_free(ctx);
        } catch(...) {
            if (ctx != nullptr) EVP_CIPHER_CTX_free(ctx);
            throw;
        }

        return result;
    }

}
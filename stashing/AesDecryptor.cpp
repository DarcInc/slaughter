//
// Created by phoehne on 8/28/22.
//

#include "AesDecryptor.hpp"

#include <cstring>
#include <stdexcept>
#include <openssl/err.h>
#include <openssl/evp.h>

namespace Stashing {
    bool AesDecryptor::extractSalt(std::istream &cipher, unsigned char *salt) {
        bool result = false;
        auto restartLocation = cipher.tellg();
        char taste[16];

        cipher.read(taste, sizeof(taste));
        if (::memcmp(taste, "Salted__", 8) == 0) {
            ::memcpy((void*)salt, (void*)(taste + 8), 8);
            result = true;
        } else {
            cipher.seekg(restartLocation);
        }

        return result;
    }

    bool AesDecryptor::extractSalt(unsigned char* cipherText, unsigned char* salt) {
        bool result = false;
        if (::memcmp(cipherText, "Salted__", 8) == 0) {
            ::memcpy((void*) salt, cipherText + 8, 8);
            result = true;
        }
        return result;
    }


    int AesDecryptor::computeKey(unsigned char* salt, const std::string& passPhrase, unsigned char* outKey, unsigned char* outIv) {
        ::bzero(outKey, EVP_MAX_KEY_LENGTH);
        ::bzero(outIv, EVP_MAX_IV_LENGTH);

        int keySize = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt,
                       (unsigned char*)passPhrase.c_str(), passPhrase.length(),
                       1, outKey, outIv);

        if (keySize == 0) {
            throw std::runtime_error("Unable to derive key and initialization vector from passphrase");
        }

        return keySize;
    }

    EVP_CIPHER_CTX* AesDecryptor::openContext(unsigned char* key, unsigned char* iv, const int& keyLength) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (ctx == nullptr) {
            throw std::runtime_error("Unable to open a cipher context");
        }

        try {
            if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
                throw std::runtime_error("Failed to initialize the cipher context");
            }

           if (1 != EVP_CIPHER_CTX_set_key_length(ctx, keyLength)) {
               throw std::runtime_error("Failed to set cipher key length");
           }
        } catch(...) {
            EVP_CIPHER_CTX_free(ctx);
            throw;
        }

        return ctx;
    }

    void AesDecryptor::decrypt(std::istream &cipher, std::ostream &plain, const std::string& passphrase) {
        unsigned char salt[8];
        if (!extractSalt(cipher, salt)) {
            throw std::invalid_argument("Expected encrypted binary stream data to contain salt");
        }

        unsigned char key[EVP_MAX_KEY_LENGTH];
        unsigned char iv[EVP_MAX_IV_LENGTH];

        int keyLength = computeKey(salt, passphrase, key, iv);
        EVP_CIPHER_CTX *ctx = openContext(key, iv, keyLength);

        char buffer[1024 * 32];
        char clearbuffer[1024 * 32];
        int outLength;

        do {
            cipher.read(buffer, sizeof(buffer));
            auto bytesRead = cipher.gcount();
            EVP_DecryptUpdate(ctx, (unsigned char *) clearbuffer, &outLength, (unsigned char *) buffer, bytesRead);

            plain.write(clearbuffer, outLength);
        } while(cipher.good());

        EVP_DecryptFinal_ex(ctx, (unsigned char*)clearbuffer, &outLength);
        plain.write(clearbuffer, outLength);

    }

    std::unique_ptr<unsigned char[]>
    AesDecryptor::decrypt(unsigned char* cipherText, int cipherTextSize, const std::string &passphrase) {
        if (cipherTextSize < 16) {
            throw std::invalid_argument("Cipher text is too small to decrypt with passphrase");
        }

        unsigned char salt[8];
        if (!extractSalt(cipherText, salt)) {
            throw std::invalid_argument("Expected encrypted message to contain salt");
        }

        unsigned char key[EVP_MAX_KEY_LENGTH];
        unsigned char iv[EVP_MAX_IV_LENGTH];
        EVP_CIPHER_CTX *ctx = nullptr;

        int cipherOffset = 8 + PKCS5_SALT_LEN;

        std::unique_ptr<unsigned char[]> result = std::make_unique<unsigned char[]>(cipherTextSize - cipherOffset);

        try {
            int keyLength = computeKey(salt, passphrase, key, iv);
            ctx = openContext(key, iv, keyLength);
            int outLength;

            if (1 != EVP_DecryptUpdate(ctx, result.get(), &outLength, cipherText + cipherOffset, cipherTextSize - cipherOffset)) {
                char buffer[256];
                auto err = ERR_get_error();
                ERR_error_string_n(err, buffer, sizeof(buffer));

                throw std::runtime_error(buffer);
            }

            if (1 != EVP_DecryptFinal_ex(ctx, result.get() + outLength, &outLength)) {
                char buffer[256];
                auto err = ERR_get_error();
                ERR_error_string_n(err, buffer, sizeof(buffer));
                throw std::runtime_error(buffer);
            }

            EVP_CIPHER_CTX_free(ctx);
        } catch(...) {
            if (ctx != nullptr) EVP_CIPHER_CTX_free(ctx);
            throw;
        }

        return result;
    }
}
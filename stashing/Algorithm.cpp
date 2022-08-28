//
// Created by Paul Hoehne on 8/28/22.
//

#include "Algorithm.hpp"

#include <exception>

#include "Hash.hpp"

namespace Stashing {
    Algorithm::~Algorithm() {}

    EVP_MD_CTX* Algorithm::openContext() const {
        EVP_MD_CTX *ctx = nullptr;
        try {
            ctx = ::EVP_MD_CTX_new();
            if (ctx == nullptr) {
                throw std::runtime_error("Unable to allocate a " + name() + " hashing context");
            }

            if (1 != ::EVP_DigestInit_ex(ctx, digestAlgorithm(), nullptr)) {
                throw std::runtime_error("Failed to initialize " + name() + " digest algorithm");
            }
        } catch(...) {
            if (ctx != nullptr) EVP_MD_CTX_free(ctx);
            throw;
        }

        return ctx;
    }

    void Algorithm::processFile(const std::filesystem::path& filename, EVP_MD_CTX* ctx) {
        char readBuffer[32 * 1024];

        if (!std::filesystem::exists(filename)) {
            throw std::invalid_argument("The path " + std::string(filename) + " does not exist");
        }

        std::ifstream infile(filename, std::ios::in | std::ios::binary);
        if (!infile.good()) {
            throw std::invalid_argument("The path " + std::string(filename) + " could not be opened for reading");
        }

        processStream(infile, ctx);

        infile.close();
    }

    void Algorithm::processStream(std::istream& stream, EVP_MD_CTX* ctx) {
        char readBuffer[32 * 1024];

        if (!stream.good()) {
            throw std::invalid_argument("The stream could not be read");
        }

        do {
            stream.read(readBuffer, sizeof(readBuffer));
            auto bytesRead = stream.gcount();
            m_bytesHashed += bytesRead;
            if (1 != EVP_DigestUpdate(ctx, readBuffer, bytesRead)) {
                throw std::runtime_error("Failed to write data to " + name() + " digest");
            }
        } while (stream.good());
    }

    void Algorithm::processStream(std::istream& stream, const std::streamsize& maxBytes, EVP_MD_CTX* ctx) {
        char readBuffer[32 * 1024];

        if (!stream.good()) {
            throw std::invalid_argument("The stream could not be read");
        }

        auto bytesLeft = maxBytes;
        do {
            auto bytesToRead = (std::streamsize)(bytesLeft > sizeof(readBuffer) ? sizeof(readBuffer) : bytesLeft);
            stream.read(readBuffer, bytesToRead);

            auto bytesRead = stream.gcount();
            bytesLeft -= bytesRead;
            m_bytesHashed += bytesRead;

            if (1 != EVP_DigestUpdate(ctx, readBuffer, bytesRead)) {
                throw std::runtime_error("Failed to write data to " + name() + " digest");
            }
        } while (stream.good() && bytesLeft > 0);
    }

    unsigned char* Algorithm::extractDigest(EVP_MD_CTX* ctx, unsigned int& size) const {
        unsigned char* digestData = nullptr;
        try {
            digestData = (unsigned char *) OPENSSL_malloc(hashSize());
            if (digestData == nullptr) {
                throw std::runtime_error("Unable to allocate buffer space for raw hash");
            }

            if (1 != EVP_DigestFinal_ex(ctx, digestData, &size)) {
                throw std::runtime_error("Failed to extract " + name() + " hash from digest");
            }
        } catch(...) {
            if (digestData != nullptr) OPENSSL_free(digestData);
            throw;
        }

        return digestData;
    }

    std::unique_ptr<Hash> Algorithm::hash(const std::filesystem::path& filename) {
        ::EVP_MD_CTX* ctx = nullptr;
        unsigned char* digestData = nullptr;
        std::unique_ptr<Hash> result = nullptr;
        m_bytesHashed = 0;

        try {
            ctx = openContext();
            processFile(filename, ctx);

            unsigned int digestSize = hashSize();
            digestData = extractDigest(ctx, digestSize);

            result = std::make_unique<Hash>(digestData, digestSize);

            OPENSSL_free(digestData);
            EVP_MD_CTX_free(ctx);
        } catch(...) {
            if (digestData != nullptr) OPENSSL_free(digestData);
            if (ctx != nullptr) EVP_MD_CTX_free(ctx);
            throw;
        }

        return result;
    }

    std::unique_ptr<Hash> Algorithm::hash(std::istream& stream) {
        ::EVP_MD_CTX* ctx = nullptr;
        unsigned char* digestData = nullptr;
        std::unique_ptr<Hash> result = nullptr;
        m_bytesHashed = 0;

        try {
            ctx = openContext();
            processStream(stream, ctx);

            unsigned int digestSize = hashSize();
            digestData = extractDigest(ctx, digestSize);

            result = std::make_unique<Hash>(digestData, digestSize);

            OPENSSL_free(digestData);
            EVP_MD_CTX_free(ctx);
        } catch(...) {
            if (digestData != nullptr) OPENSSL_free(digestData);
            if (ctx != nullptr) EVP_MD_CTX_free(ctx);
            throw;
        }

        return result;
    }

    std::unique_ptr<Hash> Algorithm::hash(std::istream& stream, const std::streamsize& maxBytes) {
        ::EVP_MD_CTX* ctx = nullptr;
        unsigned char* digestData = nullptr;
        std::unique_ptr<Hash> result = nullptr;
        m_bytesHashed = 0;

        try {
            ctx = openContext();
            processStream(stream, maxBytes, ctx);

            unsigned int digestSize = hashSize();
            digestData = extractDigest(ctx, digestSize);

            result = std::make_unique<Hash>(digestData, digestSize);

            OPENSSL_free(digestData);
            EVP_MD_CTX_free(ctx);
        } catch(...) {
            if (digestData != nullptr) OPENSSL_free(digestData);
            if (ctx != nullptr) EVP_MD_CTX_free(ctx);
            throw;
        }

        return result;
    }



    size_t Algorithm::bytesHashed() const {
        return m_bytesHashed;
    }
}
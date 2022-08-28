//
// Created by Paul Hoehne on 8/28/22.
//

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include <openssl/evp.h>

#ifndef SLAUGHTER_ALGORITHM_HPP
#define SLAUGHTER_ALGORITHM_HPP

namespace Stashing {
    class Hash;

    class Algorithm {
        size_t m_bytesHashed;
    protected:
        virtual const EVP_MD* digestAlgorithm() const = 0;
        virtual EVP_MD_CTX* openContext() const;
        virtual void processFile(const std::filesystem::path& filename, EVP_MD_CTX* ctx);
        virtual void processStream(std::istream& stream, EVP_MD_CTX* ctx);
        virtual void processStream(std::istream& stream, const std::streamsize& maxBytes, EVP_MD_CTX* ctx);
        virtual unsigned char* extractDigest(EVP_MD_CTX* ctx, unsigned int& size) const;
    public:
        virtual ~Algorithm();
        virtual std::string name() const = 0;
        virtual size_t hashSize() const = 0;
        virtual std::unique_ptr<Hash> hash(const std::filesystem::path& filename);
        virtual std::unique_ptr<Hash> hash(std::istream& stream);
        virtual std::unique_ptr<Hash> hash(std::istream& stream, const std::streamsize& maxBytes);
        virtual size_t bytesHashed() const;
    };
}


#endif //SLAUGHTER_ALGORITHM_HPP

//
// Created by Paul Hoehne on 8/30/22.
//

#include <cstdlib>
#include <memory>

#ifndef SLAUGHTER_BLOCK_HPP
#define SLAUGHTER_BLOCK_HPP

namespace Stashing {
    class Block {
        void* m_rawMemory;
        size_t m_capacity;
        size_t m_used;
        size_t m_offset;

        static const size_t K64;
        static const size_t K8;
    public:
        Block();
        explicit Block(const size_t& cap);
        Block(const Block& other) = delete;
        Block(Block && other);
        Block(void* buffer, size_t size);
        virtual ~Block();
        Block& operator=(Block& other) = delete;
        Block& operator=(Block&& other);

        size_t write(void* inBuffer, const size_t& inSize);
        size_t read(void* outBuffer, const size_t& bufferSize);
        size_t pos() const;
        size_t seek(size_t pos);
        void rewind();
        size_t capcity();
        size_t used();

        std::unique_ptr<unsigned char[]> raw() const;

        friend class AesEncryptor;
        friend class AesDecryptor;
    };
}

#endif //SLAUGHTER_BLOCK_HPP

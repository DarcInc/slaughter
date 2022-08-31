//
// Created by Paul Hoehne on 8/30/22.
//

#include "Block.hpp"

#include <algorithm>
#include <exception>
#include <stdexcept>
#include <cstring>

namespace Stashing {

    const size_t Block::K64 = 64 * 1024;
    const size_t Block::K8 = 8 * 1024;

    Block::Block() : m_capacity(256), m_used(0), m_rawMemory(::malloc(256)), m_offset(0) {
        if (m_rawMemory == nullptr) {
            throw std::runtime_error("failed to allocate memory");
        }
    }

    Block::Block(const size_t &cap) : m_capacity(cap), m_used(0), m_rawMemory(::malloc(cap)), m_offset(0) {
        if (m_rawMemory == nullptr) {
            throw std::runtime_error("failed to allocate memory");
        }
    }

    Block::Block(Block &&other) : m_rawMemory(other.m_rawMemory), m_used(other.m_used), m_capacity(other.m_capacity), m_offset(other.m_offset) {
        other.m_rawMemory = nullptr;
        other.m_capacity = 0;
        other.m_used = 0;
        other.m_offset = 0;
    }

    Block::Block(void* buffer, size_t size) : m_rawMemory(nullptr), m_capacity(0), m_used(0), m_offset(0) {
        m_rawMemory = malloc(size);
        if (m_rawMemory == nullptr) {
            throw std::runtime_error("failed to allocate memory");
        }
        ::memcpy(m_rawMemory, buffer, size);
        m_capacity = size;
        m_used = size;
    }

    Block::~Block() {
        ::free(m_rawMemory);
        m_rawMemory = nullptr;
        m_used = 0;
        m_capacity = 0;
        m_offset = 0;
    }

    Block &Block::operator=(Block &&other) {
        std::swap<void*>(m_rawMemory, other.m_rawMemory);
        std::swap<size_t>(m_capacity, other.m_capacity);
        std::swap<size_t>(m_used, other.m_used);
        std::swap<size_t>(m_offset, other.m_offset);

        return *this;
    }

    size_t Block::write(void *inbuffer, const size_t& inSize) {
        size_t newSize = 0;
        if (m_offset + inSize > m_capacity) {
            if ((m_offset + inSize) > K64) {
                newSize = ((m_offset + inSize) / K64 + 1) * K64;
            } else {
                newSize = ((m_offset + inSize) / K8 + 1) * K8;
            }

            void* newMemory = ::realloc(m_rawMemory, newSize);
            if (newMemory == nullptr) {
                throw std::runtime_error("Failed to allocate new memory");
            }

            m_rawMemory = newMemory;
            m_capacity = newSize;
        }

        ::memcpy((unsigned char*)m_rawMemory + m_offset, inbuffer, inSize);
        if (m_offset + inSize > m_used) {
            m_used = m_offset + inSize;
        }

        m_offset = m_offset + inSize;

        return inSize;
    }

    size_t Block::read(void *outBuffer, const size_t& bufferSize) {
        size_t availableData = m_used - m_offset;
        if (availableData > 0) {
            if (availableData >= bufferSize) {
                ::memcpy(outBuffer, (unsigned char*)m_rawMemory + m_offset, bufferSize);
                m_offset += bufferSize;
            } else {
                ::memcpy(outBuffer, (unsigned char*)m_rawMemory + m_offset, availableData);
                m_offset = m_used;
            }
        }

        return availableData > bufferSize ? bufferSize : availableData;
    }

    size_t Block::pos() const {
        return m_offset;
    }

    size_t Block::seek(size_t pos) {
        if (m_used >= pos) {
            m_offset = pos;
        } else {
            m_offset = m_used;
        }
        return m_offset;
    }

    void Block::rewind() {
        m_offset = 0;
    }

    size_t Block::capcity() {
        return m_capacity;
    }

    size_t Block::used() {
        return m_used;
    }

    std::unique_ptr<unsigned char[]> Block::raw() const {
        auto result = std::make_unique<unsigned char[]>(m_used);
        ::memcpy(result.get(), m_rawMemory, m_used);
        return result;
    }
}
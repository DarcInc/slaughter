//
// Created by Paul Hoehne on 8/28/22.
//

#include "Hash.hpp"

namespace Stashing {

    static char LOOKUP_TABLE[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    Hash::Hash(const void *raw, const size_t &size) {
        m_raw = std::vector<uint8_t>(size);
        for (auto i = 0; i < size; i++) {
            m_raw[i] = *(((uint8_t *) raw) + i);
        }
    }

    std::vector<uint8_t> Hash::raw() const {
        return m_raw;
    }

    Hash::operator std::string() const {
        char allBytes[m_raw.size() * 2 + 1];
        for (auto i = 0; i < m_raw.size(); i++) {
            allBytes[i * 2] = LOOKUP_TABLE[m_raw.at(i) >> 4];
            allBytes[i * 2 + 1] = LOOKUP_TABLE[m_raw.at(i) & 0x0f];
        }
        allBytes[m_raw.size() * 2] = '\0';
        return {allBytes};
    }

    std::string Hash::string() const {
        return operator std::string();
    }

}
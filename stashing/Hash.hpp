//
// Created by Paul Hoehne on 8/28/22.
//

#include <cstdlib>
#include <string>
#include <vector>

#ifndef SLAUGHTER_HASH_HPP
#define SLAUGHTER_HASH_HPP

namespace Stashing {
    class Hash {
        std::vector<uint8_t> m_raw;
    public:
        Hash(const void* raw, const size_t& size);
        std::vector<uint8_t> raw() const;
        explicit operator std::string() const;
        std::string string() const;
    };
}

#endif //SLAUGHTER_HASH_HPP

//
// Created by Paul Hoehne on 8/27/22.
//

#include <filesystem>
#include <string>

#include <openssl/evp.h>

#ifndef SLAUGHTER_STASH_HPP
#define SLAUGHTER_STASH_HPP

namespace Stashing {

    class Stash {
        std::filesystem::path m_filepath;
        Stash();
    public:
        static Stash hashFile(const std::filesystem::path& path);
    };

}

#endif //SLAUGHTER_STASH_HPP

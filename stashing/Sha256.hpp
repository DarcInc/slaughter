//
// Created by Paul Hoehne on 8/28/22.
//

#include "Algorithm.hpp"

#ifndef SLAUGHTER_SHA256_HPP
#define SLAUGHTER_SHA256_HPP

namespace Stashing {

    class Sha256 : public Algorithm {
    protected:
        const EVP_MD* digestAlgorithm() const override;
    public:
        std::string name() const override;
        size_t hashSize() const override;
    };

}

#endif //SLAUGHTER_SHA256_HPP

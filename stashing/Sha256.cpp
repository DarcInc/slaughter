//
// Created by Paul Hoehne on 8/28/22.
//

#include "Sha256.hpp"

#include <stdexcept>
#include <openssl/evp.h>

#include "Hash.hpp"

namespace Stashing {

    const EVP_MD* Sha256::digestAlgorithm() const {
        return EVP_sha256();
    }

    std::string Sha256::name() const {
        return {"Sha256" };
    }

    size_t Sha256::hashSize() const {
        return EVP_MD_get_size(EVP_sha256());
    }
}
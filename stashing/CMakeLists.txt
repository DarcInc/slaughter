add_library(stashing
        stashing.cpp stashing.hpp
        stash.cpp stash.hpp Algorithm.cpp Algorithm.hpp Sha256.cpp Sha256.hpp Hash.cpp Hash.hpp AesEncryptor.cpp AesEncryptor.hpp AesDecryptor.cpp AesDecryptor.hpp)

target_include_directories(stashing
        PRIVATE ${OPENSSL_INCLUDE_DIR})
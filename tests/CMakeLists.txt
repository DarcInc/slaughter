enable_testing()

add_executable(tests main.cpp
        Sha256Test.cpp Sha256Test.hpp
        AesDecryptorTest.cpp AesDecryptorTest.hpp
        AesEncryptorTest.cpp AesEncryptorTest.hpp
        BlockTest.cpp BlockTest.hpp)

target_link_directories(tests
        PUBLIC ${CMAKE_SOURCE_DIR}/stashing
        PUBLIC /usr/local/lib)
target_include_directories(tests
        PUBLIC ${CMAKE_SOURCE_DIR}/stashing
        PUBLIC ${Boost_INCLUDE_DIRS}
        PUBLIC /usr/local/include)
target_link_libraries(tests stashing OpenSSL::SSL cppunit)
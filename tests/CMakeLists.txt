enable_testing()

add_executable(tests main.cpp Sha256Test.cpp Sha256Test.hpp AesDecryptorTest.cpp AesDecryptorTest.hpp AesEncryptorTest.cpp AesEncryptorTest.hpp)

target_link_directories(tests
        PUBLIC ${CMAKE_SOURCE_DIR}/stashing)
target_include_directories(tests
        PUBLIC ${CMAKE_SOURCE_DIR}/stashing
        PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(tests stashing OpenSSL::SSL cppunit)
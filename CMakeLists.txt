cmake_minimum_required(VERSION 3.23)
project(slaughter)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED json)

add_subdirectory(stashing)

if (APPLE)
    set(CMAKE_PREFIX_PATH "/usr/local/opt/openssl")
endif ()

find_package(OpenSSL REQUIRED)

add_executable(slaughter main.cpp)
target_link_directories(slaughter PUBLIC ./stashing)
target_include_directories(slaughter PUBLIC ./stashing)
target_link_libraries(slaughter stashing OpenSSL::SSL)

enable_testing()
add_subdirectory(tests)



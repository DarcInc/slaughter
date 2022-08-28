#include <iostream>

#include <iomanip>
#include <iostream>
#include <sstream>

#include <stashing.hpp>
#include <Sha256.hpp>
#include <Hash.hpp>

int main() {
    auto stashing_version = Stashing::version();

    std::cout << "Stashing version: ";
    std::cout << stashing_version[0] << "." << stashing_version[1] << "." << stashing_version[2];
    std::cout << std::endl;

    Stashing::Sha256 hasher;
    std::cout << "Information for: " << hasher.name() << std::endl;
    std::cout << "Size: " << hasher.hashSize() << " bytes" << std::endl;

    std::filesystem::path home(::getenv("HOME"));
    auto hash = hasher.hash(home.append("java_error_in_pycharm.hprof"));
    std::cout << "java_error_in_pycharm.hprof file hash: " << hash->string() << std::endl;
    std::cout << hasher.bytesHashed() << " bytes were hashed" << std::endl;

    std::istringstream iss("This is the time for us to fly\n");
    hash = hasher.hash(iss);
    std::cout << hash->string() << std::endl;

    iss = std::istringstream("This is the time for us to fly\n");
    hash = hasher.hash(iss, 5);
    std::cout << hash->string() << std::endl;

    return 0;
}

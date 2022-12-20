#ifndef PSWMANAGER_SPOOKY_ROAD_ENCRYPTION_H
#define PSWMANAGER_SPOOKY_ROAD_ENCRYPTION_H

#include <iostream>

namespace enc {
    std::string encrypt(const std::string& plain_text, const int& key);
    std::string decrypt(const std::string& cipher_text, const int& key);
}

#endif //PSWMANAGER_SPOOKY_ROAD_ENCRYPTION_H

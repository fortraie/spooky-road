//
// Created by Jakub Kowalewski on 20/12/2022.
//

#ifndef PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H
#define PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H

#include <string>
#include "../classes/IOHandlingTag.h"

namespace ioh {

    void write_file(const std::string& file_path, const IOHandlingTag& ioHandlingTag, const std::string& plain_text);
    std::vector<std::string> read_file(const std::string& file_path, const IOHandlingTag& ioHandlingTag);
    std::string create_file(const std::string &password);

}

#endif //PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H

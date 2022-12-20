#ifndef PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H
#define PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H

#include <string>
#include <vector>
#include "../classes/IOHandlingTag.h"

namespace ioh {

    void write_file(const std::string& file_path, const std::string &password, const IOHandlingTag& ioHandlingTag, const std::string& plain_text);
    void write_file(const std::string& file_path, const IOHandlingTag& ioHandlingTag, const std::string& plain_text);

    std::vector<std::string> read_file(const std::string& file_path, const std::string &password, const IOHandlingTag& ioHandlingTag);
    std::vector<std::string> read_file(const std::string& file_path, const IOHandlingTag& ioHandlingTag);

    std::string create_file(const std::string &password);

    std::vector<std::string> get_available_files();

}

#endif //PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H

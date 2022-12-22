#ifndef PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H
#define PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H

#include <string>
#include <vector>

#include "../classes/IOHandlingTag.h"
#include "../classes/Session.h"
#include "../classes/Entry.h"


namespace ioh {


    void write_file(const std::string& file_path, const IOHandlingTag& ioHandlingTag, const std::string& plain_text);
    void write_file(const std::string& file_path, const std::string &password, const IOHandlingTag& ioHandlingTag, const std::string& plain_text);

    std::vector<std::string> read_file(const std::string& file_path, const IOHandlingTag& ioHandlingTag);
    std::vector<std::string> read_file(const std::string& file_path, const std::string &password, const IOHandlingTag& ioHandlingTag);

    std::vector<Entry> read_entries(Session &session);
    std::vector<Category> read_categories(Session &session);


    std::string create_file(const std::string &password);

    std::vector<std::string> get_available_files();


    std::string generate_password(const Session& session);
    void configure_generate_password(const Session& session, const int& length, const bool& use_uppercase, const bool& use_lowercase, const bool& use_special_characters);


    void write_timestamp(Session& session);
    std::string read_timestamp(Session& session);


    Entry parse_entry(Session& session, const std::string& entry_string);

    bool unsafe_password(Session& session, const std::string& password);


}


#endif //PSWMANAGER_SPOOKY_ROAD_IOHANDLING_H

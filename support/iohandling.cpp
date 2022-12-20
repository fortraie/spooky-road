//
// Created by Jakub Kowalewski on 20/12/2022.
//

#include "iohandling.h"
#include "../classes/IOHandlingTag.h"
#include "encryption.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

namespace ioh {

    void write_file(const std::string& file_path, const IOHandlingTag& ioHandlingTag, const std::string& plain_text) {
        std::ofstream file(file_path, std::ios::app);
        file << (int) ioHandlingTag << " " << plain_text << std::endl;
    }

    std::string create_file(const std::string &password) {
        std::srand(std::time(nullptr));
        std::string file_name = "db_" + std::to_string(std::rand()) + ".txt";
        std::ofstream file(file_name);

        write_file(file_name, IOHandlingTag::PASSWORD, enc::encrypt(password, password.length()));

        return file_name;
    }



}
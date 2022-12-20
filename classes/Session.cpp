#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include "Session.h"
#include "../support/encryption.h"
#include "../support/iohandling.h"
#include "../support/communication.h"

Session::Session(const std::string &file_path, const std::string &password) : file_path(file_path), password(password) {
    std::vector<std::string> passwords = ioh::read_file(file_path, password, IOHandlingTag::PASSWORD);
    if (passwords.size() > 1 || password.empty()) {
        std::cout << "File is corrupted. Please delete it and create a new one." << std::endl;
    } else {
        if (passwords.at(0) == password) {
            com::returningUser::password_notification(*this, true);
            com::returningUser::timestamp_notification(*this);
        } else {
            com::returningUser::password_notification(*this, false);
        }
    }
}
Session::Session(const std::string &password) : file_path(ioh::create_file(password)), password(password) {};

const std::string &Session::getFilePath() const {
    return file_path;
}

const std::string &Session::getPassword() const {
    return password;
}

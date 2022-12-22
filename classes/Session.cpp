#include <string>
#include <iostream>
#include <random>
#include "Session.h"
#include "../support/iohandling.h"
#include "../support/communication.h"

/**
 * Tworzy nową sesję użytkownika.
 * @param username Nazwa użytkownika.
 * @param password Hasło użytkownika.
 * @param filePath Ścieżka do pliku z hasłami.
 */
Session::Session(const std::string &file_path, const std::string &password) : file_path(file_path), password(password) {
    std::vector<std::string> passwords = ioh::read_file(file_path, password, IOHandlingTag::PASSWORD);
    if (passwords.size() > 1 || password.empty()) {
        std::cout << "File is corrupted. Please delete it and create a new one." << std::endl;
    } else {
        if (passwords.at(0) == password) {
            com::returningUser::password_notification(*this, true);
        } else {
            com::returningUser::password_notification(*this, false);
        }
    }
}

/**
 * Tworzy nową sesję użytkownika, tworzy nowy plik z hasłami.
 */
Session::Session(const std::string &password) : file_path(ioh::create_file(password)), password(password) {};


/**
 * @return Ścieżka do pliku z hasłami.
 */
const std::string &Session::getFilePath() const {
    return file_path;
}


/**
 * @return Hasło użytkownika.
 */
const std::string &Session::getPassword() const {
    return password;
}

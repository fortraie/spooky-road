//
// Created by Jakub Kowalewski on 20/12/2022.
//

#include "communication.h"
#include <iostream>
#include <vector>
#include "../classes/Session.h"
#include "iohandling.h"
#include <ctime>

namespace com {
    void initialize() {
        std::cout << "Hi! (✿◠‿◠)" << std::endl;
        std::cout << "Please log in or register:" << std::endl;
        std::cout << "[0] Log in" << std::endl;
        std::cout << "[1] Register" << std::endl;
        std::cout << "[2] Exit" << std::endl;

        int action;
        std::cin >> action;

        switch (action) {
            case 0:
                returning_user();
                break;
            case 1:
                new_user();
                break;
            case 2:
                exit(0);
        }
    }


    void returning_user() {
        std::cout << "Nice to see you again!" << std::endl;
        std::cout << "Begin by choosing your database from the list below." << std::endl;

        std::vector<std::string> available_files = ioh::get_available_files();
        for (int i = 0; i <= available_files.size(); i++) {
            if (i == available_files.size()) {
                std::cout << "[" << i << "] " << "Other" << std::endl;
                break;
            }
            std::cout << "[" << i << "] " << available_files.at(i) << std::endl;
        }

        int action;
        std::cin >> action;

        std::string file_path;
        if (action == available_files.size()) {
            std::cout << "Please enter a file path to the database you're willing to access:" << std::endl;
            std::cin >> file_path;
        }

        std::cout << "Please provide the password to the database " << ((action != available_files.size()) ? available_files.at(action) : file_path) << ":" << std::endl;
        std::string password;
        std::cin >> password;

        if (action == available_files.size()) {
            Session session(file_path, password);
        } else {
            Session session(available_files.at(action), password);
        }
    }


    void new_user() {

    }

    void returningUser::password_notification(Session session, bool status) {
        if (status == true) {
            std::cout << "Password is correct!" << std::endl;
            returningUser::initialize(session);
        } else {
            std::cout << "Password is incorrect!" << std::endl;
            std::vector<std::string> timestamps = ioh::read_file(session.getFilePath(), 0, IOHandlingTag::TIMESTAMP);
            for (std::string timestamp : timestamps) {
                ioh::write_file(session.getFilePath(),IOHandlingTag::DEL_TIMESTAMP, timestamp);
            }
            ioh::write_file(session.getFilePath(), IOHandlingTag::TIMESTAMP, std::to_string(time(nullptr)));
        }
    }

    void returningUser::timestamp_notification(Session session) {
       std::vector<std::string> timestamps = ioh::read_file(session.getFilePath(), IOHandlingTag::TIMESTAMP);
       if (timestamps.size() > 1) {
           std::cout << "File is corrupted. Please delete it and create a new one." << std::endl;
       } else if (timestamps.size() == 1) {
           // Translate time(nullptr) to a human-readable format
           time_t time_value = std::stol(timestamps.at(0));
           std::cout << "Last time you tried to access this database was " << ctime(&time_value);
           ioh::write_file(session.getFilePath(), IOHandlingTag::DEL_TIMESTAMP, timestamps.at(0));
       } else {
           std::cout << "It's your first time here!" << std::endl;
       }
       ioh::write_file(session.getFilePath(), IOHandlingTag::TIMESTAMP, std::to_string(time(nullptr)));
    }

    void returningUser::initialize(Session session) {

    }
}
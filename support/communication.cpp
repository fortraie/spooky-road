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
                break;
            default:
                std::cout << "Invalid input. Please try again." << std::endl;
                initialize();
                break;
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
        std::cout << "Please provide a password to your new database:" << std::endl;
        std::cout << "You might consider this randomly generated password: " << ioh::generate_password() << std::endl;
        std::string password;
        std::cin >> password;

        Session session(password);
        std::cout << "Your database has been created successfully!" << std::endl;
        std::cout << "You can now access it: " << session.getFilePath() << std::endl;

        com::returning_user();
    }

    void returningUser::password_notification(Session& session, bool status) {
        returningUser::timestamp_notification(session);
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

    void returningUser::timestamp_notification(Session& session) {
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

    void returningUser::display_passwords(Session& session) {
        std::vector<std::string> passwords = ioh::read_file(session.getFilePath(), session.getPassword(),IOHandlingTag::ENTRY);
        std::cout << "Passwords:" << std::endl;
        for (std::string password : passwords) {
            std::cout << password << std::endl;
        }
    }

    void returningUser::search_passwords(Session& session) {

    }


    void returningUser::display_categories(Session& session) {
        std::vector<std::string> categories = ioh::read_file(session.getFilePath(), session.getPassword(),IOHandlingTag::CATEGORY);
        std::cout << "Categories:" << std::endl;
        for (int i = 0; i < categories.size(); i++) {
            std::cout << "[" << i << "] " << categories.at(i) << std::endl;
        }
    }

    void returningUser::add_password(Session& session) {
        std::cout << "Please provide a name for your new password:" << std::endl;
        std::string name;
        std::cin >> name;
        std::cout << "Please provide a password:" << std::endl;
        std::cout << "You might consider this randomly generated password: " << ioh::generate_password() << std::endl;
        std::string password;
        std::cin >> password;
        std::cout << "Great! Please choose a category for your password:" << std::endl;
        std::vector<std::string> categories = ioh::read_file(session.getFilePath(), session.getPassword(),IOHandlingTag::CATEGORY);
        std::cout << "Categories:" << std::endl;
        for (int i = 0; i < categories.size(); i++) {
            std::cout << "[" << i << "] " << categories.at(i) << std::endl;
        }
        int category;
        std::cin >> category;
        ioh::write_file(session.getFilePath(), session.getPassword(), IOHandlingTag::ENTRY, name + "," + password + "," + categories.at(category));
        std::cout << "Password for " << name <<  " has been added successfully!" << std::endl;
    }

    void returningUser::search_categories(Session& session) {

    }

    void returningUser::add_category(Session& session) {

    }

    void returningUser::initialize(Session& session) {
        std::cout << "What would you like to do?" << std::endl;
        std::cout << "---------------" << std::endl;
        std::cout << "[0] Display passwords" << std::endl;
        std::cout << "[1] Search for a password" << std::endl;
        std::cout << "[2] Add new password" << std::endl;
        std::cout << "---------------" << std::endl;
        std::cout << "[3] Display categories" << std::endl;
        std::cout << "[4] Search for a category" << std::endl;
        std::cout << "[5] Add new category" << std::endl;
        std::cout << "---------------" << std::endl;
        std::cout << "[6] Exit" << std::endl;

        int action;
        std::cin >> action;

        switch (action) {
            case 0:
                returningUser::display_passwords(session);
                break;
            case 1:
                returningUser::search_passwords(session);
                break;
            case 2:
                returningUser::add_password(session);
                break;
            case 3:
                returningUser::display_categories(session);
                break;
            case 4:
                returningUser::search_categories(session);
                break;
            case 5:
                returningUser::add_category(session);
                break;
            case 6:
                break;
            default:
                std::cout << "Invalid selection. Please try again." << std::endl;
                returningUser::initialize(session);
                break;
        }
    }


}
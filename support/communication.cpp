#include <iostream>
#include <vector>

#include "communication.h"
#include "iohandling.h"

#define kStandard  "\x1B[0m"
#define kRed  "\x1B[31m"
#define kBlue  "\x1B[34m"
#define kCyan  "\x1B[36m"


// Deklaracje funkcji
void com::initialize();
void com::returning_user();
void com::new_user();
void com::returningUser::password_notification(Session &session, bool status);
void com::returningUser::initialize(Session &session);


/**
 * Loguje lub rejestruje użytkownika.
 */
void com::initialize() {


    const int kLogIn {0};
    const int kRegister {1};
    const int kExit {2};

    std::printf("%sHi! (✿◠‿◠)\n", kBlue);
    std::printf("%sPlease begin by choosing an action from the list:\n", kStandard);
    std::printf("%s[%i] Log in\n", kStandard, kLogIn);
    std::printf("%s[%i] Register\n", kStandard, kRegister);
    std::printf("%s[%i] Exit\n", kStandard, kExit);

    int action;
    std::cin >> action;

    switch (action) {
        case kLogIn: {returning_user(); break;}
        case kRegister: {new_user(); break;}
        case kExit: {exit(0);}
        default: {
            std::printf("%sInvalid action. Please try again.\n", kRed);
            com::initialize();
            break;
        }
    }


}


/**
 * Loguje użytkownika.
 * Przedstawia możliwość wyboru bazy danych spośród dostępnych w folderze domyślnym
 * lub kieruje do dostarczenia ścieżki własnej użytkownika.
 */
void com::returning_user() {


    std::printf("%sGood to see you again!\n", kBlue);
    std::printf("%sPlease begin by choosing a database from the list:\n", kStandard);

    std::vector<std::string> available_files = ioh::get_available_files();
    int i {0};
    while (i < available_files.size()) {
        std::printf("[%i] %s\n", i, available_files.at(i).c_str());
        i++;
    }
    std::printf("%s[%i] Custom database\n", kCyan, i);

    int action;
    std::cin >> action;


    std::string file_path;
    if (action == i) {
        std::printf("%sPlease enter a file path to the database you're willing to access:\n", kStandard);
        std::cin >> file_path;
    } else {
        file_path = available_files.at(action);
    }

    std::printf("%sPlease enter a password to the database %s:\n", kStandard, file_path.c_str());
    std::string password;
    std::cin >> password;

    Session session(file_path, password);


}

/**
 * Tworzy nową bazę danych.
 * Losuje nazwę pliku oraz prosi użytkownika o ustalenie hasła dostępowego.
 */
void com::new_user() {


    std::printf("%sThanks!\n", kBlue);
    std::printf("%sPlease enter a password path to the database you're willing to access:\n", kStandard);
    std::printf("%sYou might want to consider using this randomly generated password: %s\n", kCyan, ioh::generate_password().c_str());

    std::string password;
    std::cin >> password;


    Session session(password);
    std::printf("%sGreat! Your new database %s has been created.\n", kBlue, session.getFilePath().c_str());


    com::returning_user();


}


/**
 * Informuje użytkownika o poprawności wprowadzonego hasła.
 * Zapisuje timestamp próby logowania.
 * @param session Aktualna sesja użytkownika (niezależnie od jej poprawnej inicjalizacji)
 * @param status Poprawność hasła stwierdzana przez klasę Session
 */
void com::returningUser::password_notification(Session& session, bool status) {


    if (status) {
        std::printf("%sThanks! Your password is correct.\n", kBlue);
        const std::string& timestamp = ioh::read_timestamp(session).c_str();
        if (timestamp != "") {
            std::printf("%sLast time you tried to access your database was on: %s.", kCyan, timestamp.c_str());
        } else {
            std::printf("%sThis is the first time you tried to access your database.", kRed);
        }
        com::returningUser::initialize(session);
    } else {
        std::printf("%sSorry, the entered password is not correct.\n", kRed);
        ioh::write_timestamp(session);
        com::initialize();
    }


}

void com::returningUser::initialize(Session &session) {

}


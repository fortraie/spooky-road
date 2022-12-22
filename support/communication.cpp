#include <iostream>
#include <vector>

#include "communication.h"
#include "iohandling.h"

#define kStandard  "\x1B[0m"
#define kRed  "\x1B[31m"
#define kBlue  "\x1B[34m"
#define kCyan  "\x1B[36m"


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
            std::printf("%sLast time you tried to access your database was on: %s.\n", kCyan, timestamp.c_str());
        } else {
            std::printf("%sThis is the first time you tried to access your database.\n", kRed);
        }
        com::returningUser::initialize(session);
    } else {
        std::printf("%sSorry, the entered password is not correct.\n", kRed);
        ioh::write_timestamp(session);
        com::initialize();
    }


}


/**
 * Wyświetla wpisy z bazy danych i umożliwia użytkownikowi wybór akcji.
 * @param session Udana sesja użytkownika.
 */
void com::returningUser::entry::display(Session &session,
                                        std::vector<Entry> entries = {}) {


    if (entries.empty()) {
        entries = ioh::read_entries(session);
    }
    int i {0};
    while (i < entries.size()) {
        std::printf("%s[%i] Name: %s\n", kCyan, i, entries.at(i).getName().c_str());
        std::printf("%s\tUsername: \t%s\n", kStandard, entries.at(i).getUsername().c_str());
        std::printf("%s\tPassword: \t%s\n", kStandard, entries.at(i).getPassword().c_str());
        std::printf("%s\tURL: \t\t%s\n", kStandard, entries.at(i).getUrl().c_str());
        std::printf("%s\tCategory: \t%s\n", kStandard, entries.at(i).getCategory().getName().c_str());
        i++;
    }


    const int kAddEntry {0};
    const int kEditEntry {2};
    const int kDeleteEntry {1};
    const int kSortEntries {3};
    const int kExit {4};


    std::printf("%sPlease begin by choosing an action from the list:\n", kStandard);
    std::printf("%s[%i] Add new entry\n", kStandard, kAddEntry);
    std::printf("%s[%i] Delete entry\n", kStandard, kDeleteEntry);
    std::printf("%s[%i] Modify entry\n", kStandard, kEditEntry);
    std::printf("%s[%i] Sort entries\n", kStandard, kSortEntries);
    std::printf("%s[%i] Exit\n", kStandard, kExit);

    int action;
    std::cin >> action;

    switch (action) {
        case kAddEntry: {com::returningUser::entry::add(session); break;}
        case kDeleteEntry: {
            std::printf("%sPlease enter an index of the entry you want to delete:\n", kStandard);

            int index;
            std::cin >> index;

            entries.at(index).destroy();

            std::printf("%sEntry has been deleted.\n", kRed);

            com::returningUser::entry::display(session);

            break;
        }
        case kEditEntry: {
            std::printf("%sPlease enter an index of the entry you want to modify:\n", kStandard);

            int index;
            std::cin >> index;

            const int kEntryModName {0};
            const int kEntryModUsername {1};
            const int kEntryModPassword {2};
            const int kEntryModUrl {3};
            const int kEntryModCategory {4};

            std::printf("%sWhat would you like to modify?\n", kStandard);
            std::printf("%s[%i] Name\n", kStandard, kEntryModName);
            std::printf("%s[%i] Username\n", kStandard, kEntryModUsername);
            std::printf("%s[%i] Password\n", kStandard, kEntryModPassword);
            std::printf("%s[%i] URL\n", kStandard, kEntryModUrl);
            std::printf("%s[%i] Category\n", kStandard, kEntryModCategory);

            int action;
            std::cin >> action;

            switch (action) {
                case kEntryModName: {
                    std::printf("%sPlease enter a new name:\n", kStandard);
                    std::string name;
                    std::cin >> name;
                    entries.at(index).setName(name);
                    std::printf("%sName has been changed.\n", kBlue);
                    com::returningUser::entry::display(session);
                    break;
                }
                case kEntryModUsername: {
                    std::printf("%sPlease enter a new username:\n", kStandard);
                    std::string username;
                    std::cin >> username;
                    entries.at(index).setUsername(username);
                    std::printf("%sUsername has been changed.\n", kBlue);
                    com::returningUser::entry::display(session);
                    break;
                }
                case kEntryModPassword: {
                    std::printf("%sPlease enter a new password:\n", kStandard);
                    std::string password;
                    std::cin >> password;
                    entries.at(index).setPassword(password);
                    std::printf("%sPassword has been changed.\n", kBlue);
                    com::returningUser::entry::display(session);
                    break;
                }
                case kEntryModUrl: {
                    std::printf("%sPlease enter a new URL:\n", kStandard);
                    std::string url;
                    std::cin >> url;
                    entries.at(index).setUrl(url);
                    std::printf("%sURL has been changed.\n", kBlue);
                    com::returningUser::entry::display(session);
                    break;
                }
                case kEntryModCategory: {
                    std::printf("%sPlease enter a new category:\n", kStandard);
                    std::string category;
                    std::cin >> category;
                    entries.at(index).setCategory(Category(session, category));
                    std::printf("%sCategory has been changed.\n", kBlue);
                    com::returningUser::entry::display(session);
                    break;
                }
                default: {
                    std::printf("%sSorry, the entered action is not valid.\n", kRed);
                    com::returningUser::entry::display(session);
                    break;
                }
            }

            break;
        }
        case kExit: {com::returningUser::initialize(session); break;}
        case kSortEntries: {
            std::printf("%sPlease enter a value you want to sort by:\n", kStandard);

            const int kSortByName {0};
            const int kSortByUsername {1};
            const int kSortByBoth {2};

            std::printf("%s[%i] Name\n", kStandard, kSortByName);
            std::printf("%s[%i] Username\n", kStandard, kSortByUsername);
            std::printf("%s[%i] Both\n", kStandard, kSortByBoth);

            int action;
            std::cin >> action;

            switch (action) {
                case kSortByName: {
                    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
                        return a.getName() < b.getName();
                    });
                    com::returningUser::entry::display(session, entries);
                    break;
                }
                case kSortByUsername: {
                    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
                        return a.getUsername() < b.getUsername();
                    });
                    com::returningUser::entry::display(session, entries);
                    break;
                }
                case kSortByBoth: {
                    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
                        return a.getName() + a.getUsername() < b.getName() + b.getUsername();
                    });
                    com::returningUser::entry::display(session, entries);
                    break;
                }
                default: {
                    std::printf("%sSorry, the entered action is not valid.\n", kRed);
                    com::returningUser::entry::display(session);
                    break;
                }
            }
        }
        default: {
            std::printf("%sInvalid action. Please try again.\n", kRed);
            com::returningUser::entry::display(session);
            break;
        }
    }


}


/**
 * Wyszukuje wpisy w bazie danych.
 * @param session Udana sesja użytkownika.
 */
void com::returningUser::entry::search(Session& session) {
    std::vector<Entry> entries = ioh::read_entries(session);
    std::printf("%sPlease enter a keyword to search for:\n", kStandard);
    std::string keyword;
    std::cin >> keyword;
    int i {0};
    while (i < entries.size()) {
        if (entries.at(i).getName().find(keyword) != std::string::npos) {
            std::printf("%s[%i] Name: %s\n", kCyan, i, entries.at(i).getName().c_str());
            std::printf("%s\tUsername: \t%s\n", kStandard, entries.at(i).getUsername().c_str());
            std::printf("%s\tPassword: \t%s\n", kStandard, entries.at(i).getPassword().c_str());
            std::printf("%s\tCategory: \t%s\n", kStandard, entries.at(i).getCategory().getName().c_str());
        }
        i++;
    }
    com::returningUser::initialize(session);
}


/**
 * Umożliwia dodanie nowego wpisu do bazy danych.
 * @param session Udana sesja użytkownika.
 */
void com::returningUser::entry::add(Session& session) {
    std::printf("%sPlease enter a name of the entry:\n", kStandard);
    std::string name;
    std::cin >> name;
    std::printf("%sPlease enter a password:\n", kStandard);
    std::printf("%sYou might want to consider using a randomly generated password: %s.\n", kCyan, ioh::generate_password(session).c_str());
    std::string password;
    std::cin >> password;
    if (ioh::unsafe_password(session, password)) {
        std::printf("%sYou have entered an unsafe password (more than 1 occurence). Please try again.\n", kRed);
        com::returningUser::entry::add(session);
    }
    std::printf("%sPlease choose an index of the category:\n", kStandard);
    std::vector<Category> categories = ioh::read_categories(session);
    int i {0};
    while (i < categories.size()) {
        std::printf("%s[%i] %s\n", kStandard, i, categories.at(i).getName().c_str());
        i++;
    }
    int category_index;
    std::cin >> category_index;


    std::string answer;
    std::string username;
    std::string url;

    std::printf("%sYou might consider adding a username to your entry.\n", kCyan);
    std::printf("%sWould you like to add a username? [y/n]\n", kStandard);

    std::cin >> answer;
    if (answer == "y") {
        std::printf("%sPlease enter a username:\n", kStandard);
        std::cin >> username;
    } else {
        username = "";
    }

    std::printf("%sYou might consider adding a URL to your entry.\n", kCyan);
    std::printf("%sWould you like to add a URL? [y/n]\n", kStandard);

    std::cin >> answer;
    if (answer == "y") {
        std::printf("%sPlease enter a URL:\n", kStandard);
        std::cin >> url;
    } else {
        url = "";
    }

    Entry entry(session, name, username, password, url, categories.at(category_index));
    std::printf("%sEntry %s has been added.\n", kBlue, entry.getName().c_str());
    com::returningUser::initialize(session);
}


/**
 * Wyświetla kategorie z bazy danych i umożliwia użytkownikowi wybór akcji.
 * @param session Udana sesja użytkownika.
 */
void com::returningUser::category::display(Session &session) {


    std::vector<Category> categories = ioh::read_categories(session);

    int i {0};
    for (Category category : categories) {
        std::printf("%s[%i] %s\n", kStandard, i, category.getName().c_str());
        i++;
    }

    const int kCategoryAdd {0};
    const int kCategoryRemove {1};
    const int kCategoryModify {2};
    const int kExit {3};

    std::printf("%sPlease enter an action:\n", kStandard);
    std::printf("%s[%i] Add a new category\n", kStandard, kCategoryAdd);
    std::printf("%s[%i] Delete a category\n", kStandard, kCategoryRemove);
    std::printf("%s[%i] Modify a category\n", kStandard, kCategoryModify);
    std::printf("%s[%i] Exit\n", kStandard, kExit);

    int action;
    std::cin >> action;

    switch (action) {
        case kCategoryAdd: {com::returningUser::category::add(session); break;}
        case kCategoryRemove: {
            std::printf("%sPlease enter an index of the category to remove:\n", kStandard);
            int index;
            std::cin >> index;
            categories.at(index).destroy();
            std::printf("%sCategory has been removed.\n", kBlue);
            com::returningUser::category::display(session);
            break;
        }
        case kCategoryModify: {
            std::printf("%sPlease enter an index of the category to modify:\n", kStandard);
            int index;
            std::cin >> index;
            std::printf("%sPlease enter a new name of the category:\n", kStandard);
            std::string name;
            std::cin >> name;
            categories.at(index).setName(name);
            std::printf("%sCategory has been modified.\n", kBlue);
            com::returningUser::category::display(session);
            break;
        }
        case kExit: {com::returningUser::initialize(session); break;}
        default: {
            std::printf("%sInvalid action. Please try again.\n", kRed);
            com::returningUser::category::display(session);
            break;
        }
    }


}


/**
 * Umożliwia dodanie nowej kategorii do bazy danych.
 * @param session Udana sesja użytkownika.
 */
void com::returningUser::category::add(Session& session) {


    std::printf("%sPlease enter a name of the category:\n", kStandard);
    std::string name;
    std::cin >> name;
    Category category(session, name);
    std::printf("%sCategory %s has been added.\n", kBlue, category.getName().c_str());
    com::returningUser::initialize(session);


}


/**
 * Umożliwia wyszukiwanie kategorii na podstawie jej nazwy.
 * @param session Udana sesja użytkownika.
 */
void com::returningUser::category::search(Session &session) {
    std::vector<Category> categories = ioh::read_categories(session);
    std::printf("%sPlease enter a keyword to search for:\n", kStandard);
    std::string keyword;
    std::cin >> keyword;
    int i {0};
    while (i < categories.size()) {
        if (categories.at(i).getName().find(keyword) != std::string::npos) {
            std::printf("%s[%i] %s\n", kCyan, i, categories.at(i).getName().c_str());
        }
        i++;
    }
    com::returningUser::initialize(session);

}


/**
 * Pozwala zalogowanemu użytkownikowi przeglądać lub dokonywać modyfikacji bazy danych.
 * @param session Udana sesja użytkownika.
 */
void com::returningUser::initialize(Session &session) {


    const int kEntryDisplay {0};
    const int kEntrySearch {1};
    const int kEntryAdd {2};

    const int kCategoryDisplay {3};
    const int kCategorySearch {4};
    const int kCategoryAdd {5};

    const int kConfigurePasswordGeneration {6};

    const int kExit {7};


    std::printf("%sPlease begin by choosing an action from the list:\n", kStandard);
    std::printf("%s-----------------------\n", kStandard);
    std::printf("%s[%i] Display entries\n", kStandard, kEntryDisplay);
    std::printf("%s[%i] Search entries\n", kStandard, kEntrySearch);
    std::printf("%s[%i] Add entry\n", kStandard, kEntryAdd);
    std::printf("%s-----------------------\n", kStandard);
    std::printf("%s[%i] Display categories\n", kStandard, kCategoryDisplay);
    std::printf("%s[%i] Search categories\n", kStandard, kCategorySearch);
    std::printf("%s[%i] Add category\n", kStandard, kCategoryAdd);
    std::printf("%s-----------------------\n", kStandard);
    std::printf("%s[%i] Configure password generation\n", kStandard, kConfigurePasswordGeneration);
    std::printf("%s-----------------------\n", kStandard);
    std::printf("%s[%i] Exit\n", kStandard, kExit);


    int action;
    std::cin >> action;

    switch (action) {
        case kEntryDisplay: {com::returningUser::entry::display(session); break;}
        case kEntrySearch: {com::returningUser::entry::search(session); break;}
        case kEntryAdd: {com::returningUser::entry::add(session); break;}
        case kCategoryDisplay: {com::returningUser::category::display(session); break;}
        case kCategorySearch: {com::returningUser::category::search(session); break;}
        case kCategoryAdd: {com::returningUser::category::add(session); break;}
        case kConfigurePasswordGeneration: {com::returningUser::configurePasswordGeneration(session); break;}
        case kExit: {exit(0);}
        default: {
            std::printf("%sInvalid action. Please try again.\n", kRed);
            com::returningUser::initialize(session);
            break;
        }
    }
}


/**
 * Umożliwia użytkownikowi ustalenie sposobu generowania haseł.
 * @param session Udana sesja użytkownika.
 */
void com::returningUser::configurePasswordGeneration(Session &session) {


    std::printf("%sPlease enter a password length:\n", kStandard);
    int length;
    std::cin >> length;
    std::printf("%sPlease enter whether you want to use uppercase letters: (0/1)\n", kStandard);
    bool uppercase;
    std::cin >> uppercase;
    std::printf("%sPlease enter whether you want to use lowercase letters: (0/1)\n", kStandard);
    bool lowercase;
    std::cin >> lowercase;
    std::printf("%sPlease enter whether you want to use special characters: (0/1)\n", kStandard);
    bool specialCharacters;
    std::cin >> specialCharacters;
    ioh::configure_generate_password(session, length, uppercase, lowercase, specialCharacters);
    std::printf("%sPassword generation has been configured.\n", kBlue);
    com::returningUser::initialize(session);


}


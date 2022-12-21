#include "Entry.h"
#include "../support/iohandling.h"


Entry::Entry(const Session &session, const std::string &name, const std::string &username, const std::string &password,
             const Category &category) : session(session), name(name), username(username), password(password),
                                         category(category) {
    std::string outContent = name + "," + username + "," + password + "," + category.getName();
    std::vector<std::string> entries = ioh::read_file(session.getFilePath(), session.getPassword(), IOHandlingTag::ENTRY);
    bool already_exists = false;
    for (std::string& entry : entries) {
        if (entry == outContent) {
            already_exists = true;
        }
    }
    if (already_exists) {
        this -> name = name;
        this -> username = username;
        this -> password = password;
        this -> category = category;
    } else {
        ioh::write_file(session.getFilePath(), session.getPassword(), IOHandlingTag::ENTRY, outContent);
        this -> name = name;
        this -> username = username;
        this -> password = password;
        this -> category = category;
    }
}

const std::string &Entry::getName() const {
    return name;
}

void Entry::setName(const std::string &name) {
    std::string outContent = this -> name + "," + this -> username + "," + this -> password + "," + this -> category.getName();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = name + "," + this -> username + "," + this -> password + "," + this -> category.getName();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> name = name;
}

const std::string &Entry::getUsername() const {
    return username;
}

void Entry::setUsername(const std::string &username) {
    std::string outContent = this -> name + "," + this -> username + "," + this -> password + "," + this -> category.getName();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = this -> name + "," + username + "," + this -> password + "," + this -> category.getName();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> username = username;
}

const std::string &Entry::getPassword() const {
    return password;
}

void Entry::setPassword(const std::string &password) {
    std::string outContent = this -> name + "," + this -> username + "," + this -> password + "," + this -> category.getName();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = this -> name + "," + this -> username + "," + password + "," + this -> category.getName();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> password = password;
}

const Category &Entry::getCategory() const {
    return category;
}

void Entry::setCategory(const Category &category) {
std::string outContent = this -> name + "," + this -> username + "," + this -> password + "," + this -> category.getName();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = this -> name + "," + this -> username + "," + this -> password + "," + category.getName();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> category = category;
}

void Entry::destroy() {
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, this -> name + "," + this -> username + "," + this -> password + "," + this -> category.getName());
}

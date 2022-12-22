#include "Entry.h"
#include "../support/iohandling.h"


/**
 * Tworzy nową lub tworzy obiekt reprezentacji wpisu.
 * @param session Udana sesja użytkownika
 * @param name Nazwa wpisu
 * @param password Hasło wpisu
 * @param category Kategoria wpisu
 */
Entry::Entry(const Session &session, const std::string &name, const std::string &username, const std::string &password, const std::string &url,
             const Category &category) : session(session), name(name), username(username), password(password), url(url),
                                         category(category) {
    std::string outContent = this -> getEntryString();
    std::vector<std::string> entries = ioh::read_file(session.getFilePath(), session.getPassword(), IOHandlingTag::ENTRY);
    bool already_exists = false;
    for (std::string& entry : entries) {
        if (entry == outContent) {
            already_exists = true;
        }
    }
    if (!already_exists) {
        ioh::write_file(session.getFilePath(), session.getPassword(), IOHandlingTag::ENTRY, outContent);
    }
}

/**
 * @return Nazwa wpisu.
 */
const std::string &Entry::getName() const {
    return name;
}


/**
 * Zmienia nazwę wpisu.
 * @param name Nowa nazwa wpisu.
 */
void Entry::setName(const std::string &name) {
    std::string outContent = this -> getEntryString();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = this -> getEntryString(EntryModificationType::NAME, name);
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> name = name;
}


/**
 * @return Nazwa użytkownika.
 */
const std::string &Entry::getUsername() const {
    return username;
}


/**
 * Zmienia nazwę użytkownika.
 * @param username Nowa nazwa użytkownika.
 */
void Entry::setUsername(const std::string &username) {
    std::string outContent = this -> getEntryString();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = this -> getEntryString(EntryModificationType::USERNAME, username);
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> username = username;
}


/**
 * @return Hasło wpisu.
 */
const std::string &Entry::getPassword() const {
    return password;
}


/**
 * Zmienia hasło wpisu.
 * @param password Nowe hasło wpisu.
 */
void Entry::setPassword(const std::string &password) {
    std::string outContent = this -> getEntryString();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = this -> getEntryString(EntryModificationType::PASSWORD, password);
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> password = password;
}


/**
 * @return Adres URL.
 */
const Category &Entry::getCategory() const {
    return category;
}


/**
 * Zmienia kategorię wpisu.
 * @param category Nowa kategoria wpisu.
 */
void Entry::setCategory(const Category &category) {
    std::string outContent = this -> getEntryString();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = this -> getEntryString(EntryModificationType::CATEGORY, category.getName());
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> category = category;
}


/**
 * Usuwa wpis z bazy danych.
 */
void Entry::destroy() {
    std::string outContent = this -> getEntryString();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
}


/**
 * @return Zwraca url wpisu.
 */
const std::string &Entry::getUrl() const {
    return url;
}


/**
 * Zmienia url wpisu.
 * @param url Nowy url wpisu.
 */
void Entry::setUrl(const std::string &url) {
    std::string outContent = this -> getEntryString();
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_ENTRY, outContent);
    outContent = this -> getEntryString(EntryModificationType::URL, url);
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::ENTRY, outContent);
    this -> url = url;
}


/**
 * Umożliwia stworzenie obiektu typu Entry bez wartości nazwy użytkownika i url.
 * @relatesalso Entry
 */
Entry::Entry(const Session &session, const std::string &name, const std::string &password, const Category &category)
        : session(session), name(name), password(password), category(category) {
    Entry(session, name, "", password, "", category);
}


/**
 * @return Reprezentacja wpisu w postaci ciągu znaków.
 */
std::string Entry::getEntryString() const {
    return this -> name + "," + this -> username + "," + this -> password + "," + this -> url + "," + this -> category.getName();
}


/**
 * Obsługuje możliwość tworzenia wpisu z jedną zmodyfikowaną wartością.
 * @param entryModificationType Rodzaj wprowadzanej zmiany.
 * @param value Nowa wartość.
 * @return Reprezentacja wpisu w postaci ciągu znaków.
 * @relatesalso Entry::getEntryString()
 */
std::string Entry::getEntryString(EntryModificationType entryModificationType, std::string value) const {
    switch (entryModificationType) {
        case EntryModificationType::NAME: {return value + "," + username + "," + password + "," + url + "," + category.getName();}
        case EntryModificationType::USERNAME: {return name + "," + value + "," + password + "," + url + "," + category.getName();}
        case EntryModificationType::PASSWORD: {return name + "," + username + "," + value + "," + url + "," + category.getName();}
        case EntryModificationType::URL: {return name + "," + username + "," + password + "," + value + "," + category.getName();}
        case EntryModificationType::CATEGORY: {return name + "," + username + "," + password + "," + url + "," + value;}
    }
}

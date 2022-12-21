#include "Category.h"
#include "Session.h"
#include "../support/iohandling.h"


/**
 * Tworzy nową lub tworzy obiekt reprezentacji kategorii.
 * @param session Udana sesja użytkownika
 * @param name Nazwa kategorii
 */
Category::Category(Session& session, const std::string &name) : session(session) {
    std::vector categories = ioh::read_file(session.getFilePath(), session.getPassword(), IOHandlingTag::CATEGORY);
    bool already_exists = false;
    for (std::string& category : categories) {
        if (category == name) {
            already_exists = true;
        }
    }
    if (already_exists) {
        this -> name = name;
    } else {
        ioh::write_file(session.getFilePath(), session.getPassword(), IOHandlingTag::CATEGORY, name);
        this -> name = name;
    }
}


/**
 * @return Nazwa kategorii.
 */
const std::string &Category::getName() const {
    return name;
}


/**
 * Zmienia nazwę kategorii.
 * @param name Nowa nazwa kategorii.
 */
void Category::setName(const std::string &name) {
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_CATEGORY, this -> name);
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::CATEGORY, name);
    this -> name = name;
}


/**
 * Usuwa kategorię.
 */
void Category::destroy() {
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_CATEGORY, this -> name);
}



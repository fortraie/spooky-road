//
// Created by Jakub Kowalewski on 21/12/2022.
//

#include "Category.h"
#include "Session.h"
#include "../support/iohandling.h"

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

const std::string &Category::getName() const {
    return name;
}

void Category::setName(const std::string &name) {
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_CATEGORY, this -> name);
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::CATEGORY, name);
    this -> name = name;
}

void Category::destroy() {
    ioh::write_file(this -> session.getFilePath(), this -> session.getPassword(), IOHandlingTag::DEL_CATEGORY, this -> name);
}



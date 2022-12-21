//
// Created by Jakub Kowalewski on 21/12/2022.
//

#ifndef PSWMANAGER_SPOOKY_ROAD_CATEGORY_H
#define PSWMANAGER_SPOOKY_ROAD_CATEGORY_H

#include <string>
#include "Session.h"


class Category {
private:
    Session session;
    std::string name;
public:
    explicit Category(Session& session, const std::string &name);

    void destroy();

    const std::string &getName() const;

    void setName(const std::string &name);
};


#endif //PSWMANAGER_SPOOKY_ROAD_CATEGORY_H

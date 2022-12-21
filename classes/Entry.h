#ifndef PSWMANAGER_SPOOKY_ROAD_ENTRY_H
#define PSWMANAGER_SPOOKY_ROAD_ENTRY_H

#include <string>
#include "Session.h"
#include "Category.h"

class Entry {
private:
    Session session;
    std::string name;
    std::string username;
    std::string password;
    Category category;
public:
    Entry(const Session &session, const std::string &name, const std::string &username, const std::string &password,
          const Category &category);

    void destroy();

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getUsername() const;

    void setUsername(const std::string &username);

    const std::string &getPassword() const;

    void setPassword(const std::string &password);

    const Category &getCategory() const;

    void setCategory(const Category &category);
};


#endif //PSWMANAGER_SPOOKY_ROAD_ENTRY_H

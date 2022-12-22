#ifndef PSWMANAGER_SPOOKY_ROAD_ENTRY_H
#define PSWMANAGER_SPOOKY_ROAD_ENTRY_H

#include <string>
#include "Session.h"
#include "Category.h"


enum class EntryModificationType {


    NAME,
    USERNAME,
    PASSWORD,
    URL,
    CATEGORY


};


class Entry {


private:


    Session session;

    std::string name;
    std::string username;
    std::string password;
    std::string url;
    Category category;


public:


    Entry(const Session &session, const std::string &name, const std::string &password, const Category &category);
    Entry(const Session &session, const std::string &name, const std::string &username, const std::string &password,
          const std::string& url, const Category &category);

    void destroy();

    const std::string &getName() const;
    void setName(const std::string &name);

    const std::string &getUsername() const;
    void setUsername(const std::string &username);

    const std::string &getPassword() const;
    void setPassword(const std::string &password);

    const Category &getCategory() const;
    void setCategory(const Category &category);

    const std::string &getUrl() const;
    void setUrl(const std::string &url);

    std::string getEntryString() const;
    std::string getEntryString(EntryModificationType entryModificationType, std::string value) const;


};


#endif //PSWMANAGER_SPOOKY_ROAD_ENTRY_H

#ifndef PSWMANAGER_SPOOKY_ROAD_SESSION_H
#define PSWMANAGER_SPOOKY_ROAD_SESSION_H

#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>


class Session {

private:
    std::string file_path;
    std::string password;

public:
    Session(const std::string &file_path, const std::string &password);
    explicit Session(const std::string &password);

    const std::string &getFilePath() const;
    const std::string &getPassword() const;
};


#endif //PSWMANAGER_SPOOKY_ROAD_SESSION_H

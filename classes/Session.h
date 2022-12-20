#ifndef PSWMANAGER_SPOOKY_ROAD_SESSION_H
#define PSWMANAGER_SPOOKY_ROAD_SESSION_H


class Session {

private:
    std::string file_path;
    std::string password;

public:
    Session(const std::string &file_path, const std::string &password);
    explicit Session(const std::string &password);
};


#endif //PSWMANAGER_SPOOKY_ROAD_SESSION_H

#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include "Session.h"
#include "../support/encryption.h"
#include "../support/iohandling.h"

Session::Session(const std::string &file_path, const std::string &password) : file_path(file_path), password(password) {}
Session::Session(const std::string &password) : file_path(ioh::create_file(password)), password(password) {};
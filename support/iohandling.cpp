#include "iohandling.h"
#include "../classes/IOHandlingTag.h"
#include "encryption.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <dirent.h>


namespace ioh {

    const std::string kFilePrefix {"db_"};
    const std::string kFileSuffix {".txt"};

    const int kDefaultShift {10};


    void write_file(const std::string& file_path, const std::string &password, const IOHandlingTag& ioHandlingTag, const std::string& plain_text) {
        std::ofstream file(file_path, std::ios::app);
        file << (int) ioHandlingTag << " " << enc::encrypt(plain_text, password.empty() ? kDefaultShift : password.length()) << std::endl;
    }

    std::vector<std::string> read_file(const std::string &file_path, const std::string &password, const IOHandlingTag &ioHandlingTag) {
        std::vector<std::string> values;
        std::ifstream file(file_path);
        std::string line;

        std::unordered_map<std::string, std::pair<int, int>> value_counts;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::vector<std::string> words{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

            std::string tag = words.front();
            std::string value = enc::decrypt(words.back(), password.empty() ? kDefaultShift : password.length());
            if (tag == std::to_string((int) ioHandlingTag)) {
                ++value_counts[value].first;
            } else if (tag == std::to_string((int) ioHandlingTag + (int) IOHandlingTag::X_SHIFT)) {
                ++value_counts[value].second;
            }
        }

        // Add the values with a positive net count to the list of values
        for (const auto& [value, count] : value_counts) {
            int net_count = count.first - count.second;
            if (net_count > 0) {
                values.push_back(value);
            }
        }

        return values;
    }

    std::string create_file(const std::string &password) {
        std::srand(std::time(nullptr));
        std::string file_name = kFilePrefix + std::to_string(std::rand()) + kFileSuffix;
        std::ofstream file(file_name);

        write_file(file_name, password, IOHandlingTag::PASSWORD, password);
        write_file(file_name, password, IOHandlingTag::CATEGORY, "fasola");
        write_file(file_name, password,IOHandlingTag::CATEGORY, "marchewka");
        write_file(file_name, password, IOHandlingTag::CATEGORY, "rzodkiew");
        write_file(file_name, password, IOHandlingTag::DEL_CATEGORY, "fasola");

        std::vector<std::string> thing = read_file(file_name, password, IOHandlingTag::CATEGORY);
        for (std::string v : thing) {
            std::cout << v << std::endl;
        }

        return file_name;
    }

    std::vector<std::string> get_available_files() {
        std::vector<std::string> available_files;

        // Open the default directory
        DIR* dir = opendir(".");
        if (dir == NULL) {
            // opendir failed
            return available_files;
        }

        // Read the files in the directory
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string file_name = entry -> d_name;
            if (file_name.length() > 3 && file_name.substr(0, 3) == kFilePrefix && file_name.substr(file_name.length() - 4) == kFileSuffix) {
                // The file name starts with "db_" and ends with ".txt", add it to the list
                available_files.push_back(file_name);
            }
        }

        // Close the directory
        closedir(dir);

        return available_files;
    }

    void ioh::write_file(const std::string &file_path, const IOHandlingTag &ioHandlingTag, const std::string &plain_text) {
        write_file(file_path, "", ioHandlingTag, plain_text);
    }

    std::vector<std::string> ioh::read_file(const std::string &file_path, const IOHandlingTag &ioHandlingTag) {
        return read_file(file_path, "", ioHandlingTag);
    }


}
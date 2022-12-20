//
// Created by Jakub Kowalewski on 20/12/2022.
//

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


namespace ioh {

    void write_file(const std::string& file_path, const IOHandlingTag& ioHandlingTag, const std::string& plain_text) {
        std::ofstream file(file_path, std::ios::app);
        file << (int) ioHandlingTag << " " << plain_text << std::endl;
    }

    std::vector<std::string> read_file(const std::string &file_path, const IOHandlingTag &ioHandlingTag) {
        std::vector<std::string> values;
        std::ifstream file(file_path);
        std::string line;

        std::unordered_map<std::string, std::pair<int, int>> value_counts;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::vector<std::string> words{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

            std::string tag = words.front();
            std::string value = words.back();
            if (tag == std::to_string((int) ioHandlingTag))
            {
                ++value_counts[value].first;
            }
            else if (tag == std::to_string((int) ioHandlingTag + (int) IOHandlingTag::X_SHIFT))
            {
                ++value_counts[value].second;
            }
        }

        // Add the values with a positive net count to the list of values
        for (const auto& [value, count] : value_counts)
        {
            int net_count = count.first - count.second;
            if (net_count > 0)
            {
                values.push_back(value);
            }
        }

        return values;
    }

    std::string create_file(const std::string &password) {
        std::srand(std::time(nullptr));
        std::string file_name = "db_" + std::to_string(std::rand()) + ".txt";
        std::ofstream file(file_name);

        write_file(file_name, IOHandlingTag::PASSWORD, enc::encrypt(password, password.length()));
        write_file(file_name, IOHandlingTag::CATEGORY, enc::encrypt("fasola", 0));
        write_file(file_name, IOHandlingTag::CATEGORY, enc::encrypt("marchewka", 0));
        write_file(file_name, IOHandlingTag::CATEGORY, enc::encrypt("rzodkiew", 0));
        write_file(file_name, IOHandlingTag::DEL_CATEGORY, enc::encrypt("fasola", 0));
        write_file(file_name, IOHandlingTag::CATEGORY, enc::encrypt("fasola", 0));

        std::vector<std::string> thing = read_file(file_name, IOHandlingTag::CATEGORY);
        for (std::string v : thing) {
            std::cout << v << std::endl;
        }

        return file_name;
    }



}
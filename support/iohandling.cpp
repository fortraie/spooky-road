#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <dirent.h>
#include <random>

#include "iohandling.h"
#include "encryption.h"

namespace ioh {


    const std::string kFilePrefix {"db_"};
    const std::string kFileSuffix {".txt"};

    const int kDefaultShift {10};
    const int kPasswordGenerationLength {16};


    /**
     * Wywołuje void write_file(const std::string& file_path, const std::string &password, const IOHandlingTag& ioHandlingTag, const std::string& plain_text)
     * z domyślną wartością hasła.
     * @relatesalso void write_file(const std::string& file_path, const std::string &password, const IOHandlingTag& ioHandlingTag, const std::string& plain_text)
     */
    void ioh::write_file(const std::string &file_path, const IOHandlingTag &ioHandlingTag, const std::string &plain_text) {
        write_file(file_path, "", ioHandlingTag, plain_text);
    }


    /**
     * Zapisuje zaszyfrowany tekst do pliku.
     * @param file_path Plik, do którego wartość ma być zapisana.
     * @param password Hasło, które zostanie użyte do szyfrowania.
     * @param ioHandlingTag Rodzaj zapisywanej informacji zgodnej z typem wyliczeniowym IOHandlingTag.
     * @param plain_text Tekst, który ma zostać zapisany i zaszyfrowany.
     */
    void write_file(const std::string& file_path, const std::string &password, const IOHandlingTag& ioHandlingTag, const std::string& plain_text) {
        std::ofstream file(file_path, std::ios::app);
        file << (int) ioHandlingTag << " " << enc::encrypt(plain_text, password.empty() ? kDefaultShift : password.length()) << std::endl;
    }


    /**
     * Wywołuje std::vector<std::string> read_file(const std::string &file_path, const std::string &password, const IOHandlingTag &ioHandlingTag)
     * z domyślną wartością hasła.
     * @relatesalso std::vector<std::string> read_file(const std::string &file_path, const std::string &password, const IOHandlingTag &ioHandlingTag)
     */
    std::vector<std::string> ioh::read_file(const std::string &file_path, const IOHandlingTag &ioHandlingTag) {
        return read_file(file_path, "", ioHandlingTag);
    }


    /**
     * Odczytuje zaszyfrowaną informację z pliku
     * Uwzględnia wystąpienia "wygaszone" tagiem DEL
     * @param file_path Plik, w którym wartość jest zaszyfrowana.
     * @param password Hasło, które zostanie użyte do odszyfrowywania.
     * @param ioHandlingTag Rodzaj odczytywanej informacji zgodny z typem wyliczeniowym IOHandlingTag.
     * @return Wektor, zawierający wartości zgodne z wymaganiami.
     */
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

        for (const auto& [value, count] : value_counts) {
            int net_count = count.first - count.second;
            if (net_count > 0) {
                values.push_back(value);
            }
        }

        return values;
    }


    /**
     * Tworzy nową bazę danych.
     * @param password Hasło użytkownika.
     * @return Nazwa utworzonej bazy danych.
     */
    std::string create_file(const std::string &password) {
        std::srand(std::time(nullptr));
        std::string file_name = kFilePrefix + std::to_string(std::rand()) + kFileSuffix;
        std::ofstream file(file_name);

        write_file(file_name, password, IOHandlingTag::PASSWORD, password);
        write_file(file_name, password, IOHandlingTag::CATEGORY, "Entertainment");
        write_file(file_name, password,IOHandlingTag::CATEGORY, "Finance");

        return file_name;
    }


    /**
     * Odczytuje pliki bazy danych znajdujące się w folderze domyślnym.
     * @return Wektor, zawierający wszystkie dostępne bazy danych.
     */
    std::vector<std::string> get_available_files() {
        std::vector<std::string> available_files;

        DIR* dir = opendir(".");
        if (dir == NULL) {
            return available_files;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string file_name = entry->d_name;
            if (file_name.find(kFilePrefix) == 0 && file_name.find(kFileSuffix) == file_name.length() - kFileSuffix.length()) {
                available_files.push_back(file_name);
            }
        }

        closedir(dir);
        return available_files;
    }


    /**
     * Generuje losowy ciąg znaków o długości zgodnej z kPasswordGenerationLength
     * @return Losowy ciąg znaków o długości zgodnej z kPasswordGenerationLength
     */
    std::string generate_password() {
        static const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        std::string password;
        password.reserve(kPasswordGenerationLength);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

        for (int i = 0; i < kPasswordGenerationLength; ++i) {
            password += alphanum[dis(gen)];
        }
        return password;
    }


    /**
     * Zapisuje timestamp do pliku.
     * Wywołuje ioh::read_file z domyślną wartością szyfrowania.
     * @param session Sesja użytkownika (niezależna od poprawności dostarczonego hasła)
     */
    void write_timestamp(Session& session) {


        std::vector<std::string> timestamps = ioh::read_file(session.getFilePath(), IOHandlingTag::TIMESTAMP);

        for (std::string timestamp : timestamps) {
            ioh::write_file(session.getFilePath(),IOHandlingTag::DEL_TIMESTAMP, timestamp);
        }

        ioh::write_file(session.getFilePath(), IOHandlingTag::TIMESTAMP, std::to_string(time(nullptr)));


    }


    /**
     * Odczytuje timestamp ostatniej próby logowania do bazy danych.
     * @param session Udana sesja użytkownika.
     * @return Timestamp w postaci std::string human-readable, pusty std::string w braku timestampu (pierwsze logowanie)
     * lub nullptr w przypadku nieprawidłowości (wystąpienie więcej niż 1 ważnego timestampu).
     */
    std::string read_timestamp(Session& session) {


        std::vector<std::string> timestamps = ioh::read_file(session.getFilePath(), IOHandlingTag::TIMESTAMP);

        switch (timestamps.size()) {
            case 0: {
                ioh::write_file(session.getFilePath(), IOHandlingTag::TIMESTAMP, std::to_string(time(nullptr)));
                return "";
            }
            case 1: {
                write_timestamp(session);
                time_t time_value = std::stol(timestamps.at(0));
                return ctime(&time_value);
            }
            default: {
                return nullptr;
            }
        }

    }


}
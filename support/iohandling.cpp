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
    void write_file(const std::string &file_path, const IOHandlingTag &ioHandlingTag, const std::string &plain_text) {
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
    std::vector<std::string> read_file(const std::string &file_path, const IOHandlingTag &ioHandlingTag) {
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
    std::string generate_password(const Session& session) {


        std::vector<std::string> configs = read_file(session.getFilePath(), session.getPassword(), IOHandlingTag::GENERATIONCONFIG);
        if (configs.size() > 1) {
            throw std::runtime_error("Database error: invalid number of generation configs.");
        } else if (configs.empty()) {
            configure_generate_password(session, 16, 1, 1, 1);
            return generate_password(session);
        }

        std::string config = configs[0];
        std::stringstream ss(config);

        std::string length_str;
        std::string use_uppercase_str;
        std::string use_lowercase_str;
        std::string use_special_characters_str;

        std::getline(ss, length_str, ',');
        std::getline(ss, use_uppercase_str, ',');
        std::getline(ss, use_lowercase_str, ',');
        std::getline(ss, use_special_characters_str, ',');

        int length = std::stoi(length_str);
        bool use_uppercase = use_uppercase_str == "1";
        bool use_lowercase = use_lowercase_str == "1";
        bool use_special_characters = use_special_characters_str == "1";

        static const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        static const char special_characters[] = "!@#$%^&*()_+-=[]{}|;':\"./<>?";

        std::string password;
        password.reserve(length);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 62);

        for (int i = 0; i < length; ++i) {
            if (use_special_characters && std::rand() % 2 == 0) {
                password += special_characters[std::rand() % (sizeof(special_characters) - 1)];
            } else {
                password += alphanum[dis(gen)];
            }
        }

        if (!use_uppercase) {
            std::transform(password.begin(), password.end(), password.begin(), ::tolower);
        }
        if (!use_lowercase) {
            std::transform(password.begin(), password.end(), password.begin(), ::toupper);
        }

        return password;


    }


    /**
     * Pozwala użytkownikowi stale (dla danej bazy danych) skonfigurować personalizowanie generowania haseł.
     * @param session Udana sesja użytkownika.
     * @param length Długość generowanego hasła.
     * @param use_uppercase Fakt, czy ma być używana wielka litera.
     * @param use_lowercase Fakt, czy ma być używana mała litera.
     * @param use_special_characters Fakt, czy ma być używany znak specjalny.
     */
    void configure_generate_password(const Session& session, const int &length, const bool &use_uppercase,
                                     const bool &use_lowercase, const bool &use_special_characters) {
        std::string outContent = std::to_string(length) + "," + std::to_string(use_uppercase) + "," +
                                 std::to_string(use_lowercase) + "," + std::to_string(use_special_characters);


        std::vector<std::string> configs = read_file(session.getFilePath(), session.getPassword(), IOHandlingTag::GENERATIONCONFIG);
        for (const std::string& config : configs) {
            write_file(session.getFilePath(), session.getPassword(), IOHandlingTag::DEL_GENERATIONCONFIG, config);
        }


        ioh::write_file(session.getFilePath(), session.getPassword(), IOHandlingTag::GENERATIONCONFIG, outContent);
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
                std::string time_string = std::ctime(&time_value);
                time_string.erase(std::remove(time_string.begin(), time_string.end(), '\n'), time_string.end());
                return time_string;
            }
            default: {
                return nullptr;
            }
        }

    }


    /**
     * @relatesalso ioh::read_file
     * @return Wektor zawierający wszystkie wpisy z bazy danych.
     */
    std::vector<Entry> read_entries(Session& session) {
        std::vector<std::string> entries = ioh::read_file(session.getFilePath(), session.getPassword(), IOHandlingTag::ENTRY);
        std::vector<Entry> parsed_entries;

        for (std::string& entry : entries) {
            parsed_entries.emplace_back(parse_entry(session, entry));
        }

        return parsed_entries;
    }


    /**
     * @relatesalso ioh::read_file
     * @return Wektor zawierający wszystkie kategorie z bazy danych.
     */
    std::vector<Category> read_categories(Session& session) {
        std::vector<std::string> categories = ioh::read_file(session.getFilePath(), session.getPassword(), IOHandlingTag::CATEGORY);
        std::vector<Category> parsed_categories;

        for (std::string& category : categories) {
            parsed_categories.emplace_back(session, category);
        }

        return parsed_categories;
    }


    /**
     * Przekształca wpis z bazy danych na obiekt Entry.
     * @param session Udana sesja użytkownika.
     * @param entry_string Wpis z bazy danych.
     * @return Obiekt typu Entry.
     */
    Entry parse_entry(Session &session, const std::string &entry_string) {
        std::stringstream ss(entry_string);

        std::string entry_name;
        std::string entry_username;
        std::string entry_password;
        std::string entry_url;
        std::string entry_category;

        std::getline(ss, entry_name, ',');
        std::getline(ss, entry_username, ',');
        std::getline(ss, entry_password, ',');
        std::getline(ss, entry_url, ',');
        std::getline(ss, entry_category, ',');

        return Entry(session, entry_name, entry_username, entry_password, entry_url, Category(session, entry_category));
    }


    /**
     * Weryfikuje czy podany ciąg znaków jest poprawnym hasłem - informuje o wystąpieniu wpisu o tym samym haśle.
     * @param session Udana sesja użytkownika.
     * @param password Hasło, które ma zostać sprawdzone.
     * @return Wartość logiczna informująca o poprawności hasła.
     */
    bool unsafe_password(Session& session, const std::string &password) {
        std::vector<Entry> entries = ioh::read_entries(session);
        for (Entry entry : entries) {
            if (entry.getPassword() == password) {
                return true;
            }
        }
        return false;
    }


}
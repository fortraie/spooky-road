#include "encryption.h"

namespace enc {
    std::string encrypt(const std::string& plain_text, const int& key) {
        std::string encrypted_text;

        for (char c : plain_text) {
            char encrypted_char = c + key;
            if (!isalpha(c)) {
                encrypted_char = c;
            } else if (isupper(c)) {
                encrypted_char = (encrypted_char - 'A') % 26 + 'A';
            } else {
                encrypted_char = (encrypted_char - 'a') % 26 + 'a';
            }
            encrypted_text += encrypted_char;
        }

        return encrypted_text;
    }

    std::string decrypt(const std::string &cipher_text, const int &key) {
        std::string plain_text;
        for (char c : cipher_text) {
            char decrypted_char = c - key;
            if (!isalpha(c)) {
                decrypted_char = c;
            } else if (isupper(c)) {
                decrypted_char = (decrypted_char - 'A' + 26) % 26 + 'A';
            } else {
                decrypted_char = (decrypted_char - 'a' + 26) % 26 + 'a';
            };
            plain_text += decrypted_char;
        }
        return plain_text;
    }
}
#include <algorithm>
#include <string>

#include "encryption.h"


namespace enc {


    /**
     * Przekształca dostarczoną wartość zgodnie z założeniami szyfru cezara.
     * @param plain_text Ciąg znaków, który ma zostać przekształcony.
     * @param key Wartość przesunięcia (tzn. dla key = 3, a -> d, b -> e).
     * @return Przekształcona wartość.
     */
    std::string encrypt(const std::string& plain_text, const int& key) {


        std::string encrypted_text;

        for (char c : plain_text) {
            encrypted_text = encrypted_text + (char) (c + key);
        }

        return encrypted_text;


    }


    /**
     * Funkcja odwrotna względem std::string encrypt(const std::string& plain_text, const int& key)
     * Przekształca dostarczoną wartość przekształconą zgodnie z założeniami szyfru cezara w czytelny tekst.
     * @param encrypted_text Ciąg znaków, wcześniej przekształcony z użyciem std::string encrypt(const std::string& plain_text, const int& key).
     * @param key Wartość przesunięcia (tzn. dla key = 3, d -> a, e -> b).
     * @return Przekształcona wartość (czytelny tekst).
     */
    std::string decrypt(const std::string &encrypted_text, const int &key) {


        std::string decrypted_text;

        for (char c : encrypted_text) {
            decrypted_text = decrypted_text + (char) (c - key);
        }

        return decrypted_text;


    }


}
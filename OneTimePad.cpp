#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <filesystem>


// Reading from any file char by char into a buffer and returning it
std::vector<char> reading_from_file_to_buffer_c(std::string file_name) {

    std::ifstream file(file_name);

    if (!file) {
        std::cerr << "Error opening the file";
        exit(EXIT_FAILURE);
    }

    char ch;
    std::vector<char> buffer;

    while (file.get(ch)) {
        buffer.push_back(ch);
    }

    return buffer;

}


 // Create file and parent directories if they don't exist
int create_file(std::string file_name) {

    auto parent_dir = std::filesystem::path(file_name).parent_path();

    if (!parent_dir.empty()) {
        std::filesystem::create_directories(parent_dir);  // Create full path
    }

    // Create the file (open in write mode to ensure creation)
    std::ofstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Error: Failed to create file" << std::endl;
        return 0;  // Failure
    }

    return 1;  // Success

}


 // Writing from a buffer to a file char by char, creating the file if needed.
 // Changed file mode to truncation to avoid appending extra data,
 // and removed the extra newline written at the start.
int writing_from_buffer_to_file_char_for_char(std::string file_name, std::vector<char> to_write) {

    if (!create_file(file_name)) {
        return 0;
    }

    // Changed from std::ios::app to std::ios::trunc to overwrite the file instead of appending.
    std::ofstream file(file_name, std::ios::trunc);

    if (!file) {
        std::cerr << "Error opening the file" << std::endl;
        return 0;
    }

    // Removed extra newline insertion.
    for (size_t i = 0; i < to_write.size(); ++i) {

        if (to_write[i] == '\n') {
            file << std::endl;
        }
        else {
            file << to_write[i];
        }
    }

    file.close();
    return 1;

}


 // Modified get_random_char() to always return an uppercase letter (A-Z).
char get_random_char() {

    static std::random_device rd;  
    static std::mt19937 gen(rd());
    // Changed distribution to 0-25 to generate letters A-Z only.
    static std::uniform_int_distribution<> distrib(0, 25); 

    return 'A' + distrib(gen);

}


 // Create a random key of given size as a buffer and return it.
std::vector<char> random_key(int size) {

    std::vector<char> result_vector;
    int current_size = 0;

    while (current_size < size) {

        char random_char = get_random_char();
        result_vector.push_back(random_char);
        current_size += 1;
    }

    return result_vector;

}


 // Encrypt a file using ONE-TIMEPAD encryption.
bool one_time_pad_encryption(std::string file_name) {

    std::vector<char> chars_file = reading_from_file_to_buffer_c(file_name);
    int file_size = chars_file.size();
    std::vector<char> random_buffer_key = random_key(file_size);
    std::vector<char> write_to_encrypt;

    for (int i = 0; i < file_size; ++i) {

        bool is_upper = (chars_file[i] >= 'A' && chars_file[i] <= 'Z');
        bool is_lower = (chars_file[i] >= 'a' && chars_file[i] <= 'z');

        if (!is_upper && !is_lower) {
            write_to_encrypt.push_back(chars_file[i]);
            continue;
        }

        int input_index = is_upper ? (chars_file[i] - 'A') : (chars_file[i] - 'a');
        // Changed: always use the uppercase key index since get_random_char() returns uppercase only.
        int key_index = (random_buffer_key[i] - 'A'); 
        int shifted_index = (input_index + key_index) % 26;
        char shifted_char = is_upper ? ('A' + shifted_index) : ('a' + shifted_index);
        write_to_encrypt.push_back(shifted_char);
    }

    std::filesystem::path file_path(file_name);
    std::string t_file_name = file_path.stem().string();
    std::string file_name_key = t_file_name + "_key.txt";
    std::string file_name_encrypted = t_file_name + "_encrypted.txt";

    int status_1 = writing_from_buffer_to_file_char_for_char(file_name_key, random_buffer_key);
    if (status_1 == 0) {
        return false;
    }

    int status_2 = writing_from_buffer_to_file_char_for_char(file_name_encrypted, write_to_encrypt);
    if (status_2 == 0) {
        return false;
    }

    return true;

}


 // Decrypt a file using ONE-TIMEPAD decryption.
bool one_time_pad_decryption(std::string file_name_encrypted, std::string file_name_key) {

    std::vector<char> key_buffer = reading_from_file_to_buffer_c(file_name_key);
    std::vector<char> encrypted_buffer = reading_from_file_to_buffer_c(file_name_encrypted);
    std::vector<char> to_write;

    // Proper filename handling: remove "_key" suffix if present.
    std::filesystem::path key_path(file_name_key);
    std::string t_file_name = key_path.stem().string();
    size_t pos = t_file_name.find("_key");
    if (pos != std::string::npos) {
        t_file_name = t_file_name.substr(0, pos);
    }
    t_file_name += "_decrypted.txt";

    // Fix: Check that the key and encrypted buffers have matching sizes.
    if (key_buffer.size() != encrypted_buffer.size()) {
        std::cerr << "Error: Key and encrypted file sizes don't match!" << std::endl;
        return false;
    }

    for (size_t i = 0; i < key_buffer.size(); ++i) {

        bool is_upper = (encrypted_buffer[i] >= 'A' && encrypted_buffer[i] <= 'Z');
        bool is_lower = (encrypted_buffer[i] >= 'a' && encrypted_buffer[i] <= 'z');

        // Changed: Use encrypted_buffer instead of file_name_encrypted[i] for non-alphabetic characters.
        if (!is_upper && !is_lower) {
            to_write.push_back(encrypted_buffer[i]);
            continue;
        }

        int encrypted_index = is_upper ? (encrypted_buffer[i] - 'A') : (encrypted_buffer[i] - 'a');
        // Changed: Always use uppercase key index.
        int key_index = (key_buffer[i] - 'A');
        int decrypted_index = (encrypted_index - key_index + 26) % 26;
        char decrypted_char = is_upper ? ('A' + decrypted_index) : ('a' + decrypted_index);
        to_write.push_back(decrypted_char);
    }

    int status = writing_from_buffer_to_file_char_for_char(t_file_name, to_write);
    if (status == 0) {
        return false;
    }

    return true;

}


int main() {

    // Example usage:
    // Uncomment the encryption line if you wish to encrypt first:
    // bool encryptionStatus = one_time_pad_encryption("example.txt");
    // if (!encryptionStatus) {
    //     std::cout << "Encryption Failed" << std::endl;
    // }

    int status_2 = one_time_pad_decryption("example_encrypted.txt", "example_key.txt");
    if (status_2 == 0) {
        std::cout << "Decryption Failed" << std::endl;
    }

    return 0;

}

Write a C++ function `decodeRSAEncodedFile` that reads an input text file whose contents are RSA-encrypted numeric blocks separated by single spaces (the file itself contains ASCII digits and spaces, with no trailing space), and writes the decrypted plaintext characters to an output file whose name is the input filename with `.decoded` appended. The function must take the input filename as a `std::string`, the private exponent `e` (an `int`), and the modulus `n` (an `int`), and return a `bool` indicating success (`true` if the input file opened and was processed, `false` otherwise). Decryption is performed per numeric block using the modular exponentiation `block^e mod n`; the resulting integer is interpreted as an ASCII character code and written to the output. The output file must contain the concatenation of all decoded characters. If the input file cannot be opened, no output file should be created and the function returns `false`. Assume the input file may contain only digits, spaces, and a newline at the end (which should be ignored). The function must be reusable and must not print anything to the console.

The solution uses the standard modular exponentiation algorithm (binary exponentiation) to compute `(base^exponent) mod modulus` efficiently for each numeric block. The input is read character-by-character; digits are accumulated into a string until a space is encountered, at which point the accumulated string is converted to an integer, decrypted, and the result is written as a single character to the output file. After the last block, a final accumulated value (if any) is also processed in case the file ends without a trailing space. Important edge cases: empty input file (should produce an empty output file and return `true`), missing trailing space, and the check for file opening failure (using `fopen` and `perror` is avoided; instead, simply return `false`). The algorithm processes each digit exactly once, so for `L` total characters in the file (including spaces), time complexity is `O(L * log e)` due to modular exponentiation for each block (but since each block is processed once, overall complexity is `O(B * log e)` where `B` is the number of blocks). Space complexity is `O(1)` aside from the temporary string for the current block.

#include <string>
#include <fstream>
#include <cctype>

// Compute (base^exponent) % modulus using binary exponentiation.
long long modularPow(long long base, long long exponent, long long modulus) {
    long long result = 1;
    base %= modulus;
    while (exponent > 0) {
        if (exponent & 1) {
            result = (result * base) % modulus;
        }
        exponent >>= 1;
        base = (base * base) % modulus;
    }
    return result;
}

// Decode an RSA-encrypted numeric file, writing plaintext to "<filename>.decoded".
bool decodeRSAEncodedFile(const std::string& inputFilename, int e, int n) {
    std::ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        return false;
    }

    std::ofstream outputFile(inputFilename + ".decoded");
    if (!outputFile.is_open()) {
        return false;
    }

    std::string currentBlock;
    char ch;
    while (inputFile.get(ch)) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            currentBlock += ch;
        } else if (ch == ' ') {
            if (!currentBlock.empty()) {
                long long blockValue = std::stoll(currentBlock);
                long long decrypted = modularPow(blockValue, e, n);
                outputFile.put(static_cast<char>(decrypted));
                currentBlock.clear();
            }
        }
        // Ignore newlines and any other characters.
    }

    // Process the last block if the file did not end with a space.
    if (!currentBlock.empty()) {
        long long blockValue = std::stoll(currentBlock);
        long long decrypted = modularPow(blockValue, e, n);
        outputFile.put(static_cast<char>(decrypted));
    }

    return true;
}

#include <cassert>
#include <fstream>
#include <string>
#include <sys/stat.h>

// Helper to check file content matches expected.
bool fileContentEquals(const std::string& path, const std::string& expected) {
    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content == expected;
}

// Helper to delete a file (Unix-like only; for testing simplicity).
void deleteFile(const std::string& path) {
    remove(path.c_str());
}

int main() {
    // Create a temporary input file with encrypted blocks for "A" (ASCII 65) with e=7, n=143.
    // 65^7 mod 143 = 42, so file contains "42".
    {
        std::ofstream input("test_enc.txt");
        input << "42";
        input.close();
        bool success = decodeRSAEncodedFile("test_enc.txt", 7, 143);
        assert(success == true);
        assert(fileContentEquals("test_enc.txt.decoded", "A"));
        deleteFile("test_enc.txt");
        deleteFile("test_enc.txt.decoded");
    }

    // Test with multiple blocks and trailing space.
    {
        std::ofstream input("test_enc2.txt");
        // "Hi" -> ASCII 72 and 105. With e=7, n=143: 72^7 mod 143 = 2, 105^7 mod 143 = 81.
        input << "2 81 ";
        input.close();
        bool success = decodeRSAEncodedFile("test_enc2.txt", 7, 143);
        assert(success == true);
        assert(fileContentEquals("test_enc2.txt.decoded", "Hi"));
        deleteFile("test_enc2.txt");
        deleteFile("test_enc2.txt.decoded");
    }

    // Test non-existent file.
    {
        bool success = decodeRSAEncodedFile("no_such_file.txt", 7, 143);
        assert(success == false);
    }

    // Test empty input file.
    {
        std::ofstream input("test_empty.txt");
        input.close();
        bool success = decodeRSAEncodedFile("test_empty.txt", 7, 143);
        assert(success == true);
        assert(fileContentEquals("test_empty.txt.decoded", ""));
        deleteFile("test_empty.txt");
        deleteFile("test_empty.txt.decoded");
    }

    // Test with newline at end (no trailing space).
    {
        std::ofstream input("test_newline.txt");
        // "Z" ASCII 90 -> 90^7 mod 143 = 12
        input << "12\n";
        input.close();
        bool success = decodeRSAEncodedFile("test_newline.txt", 7, 143);
        assert(success == true);
        assert(fileContentEquals("test_newline.txt.decoded", "Z"));
        deleteFile("test_newline.txt");
        deleteFile("test_newline.txt.decoded");
    }

    return 0;
}

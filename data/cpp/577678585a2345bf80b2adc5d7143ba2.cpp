Write a C++ function `sumValidIntegersFromFile` that takes a file name (as a `std::string`) and returns the sum of all valid `long long` integers found in the file, treating each whitespace-separated token individually. If a token is a valid integer (ignoring overflow beyond `long long` range, but tokens that are too large or contain non-numeric characters are skipped), its numeric value is added to the total. If the file cannot be opened, the function should return 0. The function must not read from standard input; it must read from the given file name. Handle tokens like `"123abc"` (skip because they are not fully convertible), `"007"` (add as 7), `"-42"` (add as -42), and `"+15"` (add as 15). Empty files or files with only invalid tokens should return 0.
#include <cassert>
#include <fstream>
#include <string>

// Declare the function being tested (in a real test, include the header).
long long sumValidIntegersFromFile(const std::string& fileName);

int main() {
    // Test 1: Basic integers
    {
        std::ofstream file("test1.txt");
        file << "10 20 -5 007\n";
        file.close();
        assert(sumValidIntegersFromFile("test1.txt") == 32); // 10+20-5+7
    }

    // Test 2: Invalid tokens and overflow
    {
        std::ofstream file("test2.txt");
        file << "123abc 9999999999999999999999 +15 45\n";
        file.close();
        // "123abc" and the huge number are skipped; +15 => 15, 45 => 45
        assert(sumValidIntegersFromFile("test2.txt") == 60);
    }

    // Test 3: Empty file
    {
        std::ofstream file("test3.txt");
        file << "";
        file.close();
        assert(sumValidIntegersFromFile("test3.txt") == 0);
    }

    // Test 4: Only invalid tokens
    {
        std::ofstream file("test4.txt");
        file << "abc --5 12.3\n";
        file.close();
        // None are valid integers
        assert(sumValidIntegersFromFile("test4.txt") == 0);
    }

    // Test 5: Nonexistent file
    {
        assert(sumValidIntegersFromFile("nonexistent_file_xyz.txt") == 0);
    }

    // Test 6: Single negative and positive numbers
    {
        std::ofstream file("test6.txt");
        file << "-42 +15\n";
        file.close();
        assert(sumValidIntegersFromFile("test6.txt") == -27);
    }

    // Test 7: Tokens with leading plus and minus signs
    {
        std::ofstream file("test7.txt");
        file << "+0 -0 0\n";
        file.close();
        assert(sumValidIntegersFromFile("test7.txt") == 0);
    }

    // Clean up test files (optional, but good practice)
    remove("test1.txt");
    remove("test2.txt");
    remove("test3.txt");
    remove("test4.txt");
    remove("test6.txt");
    remove("test7.txt");

    return 0;
}
#include <fstream>
#include <sstream>
#include <string>

// Sum all valid long long integers read from the specified file.
// Each whitespace-separated token is considered; if it is a fully valid
// long long integer (no extra non-whitespace characters), its value is added.
// Returns 0 if the file cannot be opened or contains no valid integers.
long long sumValidIntegersFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        return 0;
    }

    long long total = 0;
    std::string token;

    while (file >> token) {
        std::stringstream ss(token);
        long long number;
        if (ss >> number) {
            // Check if there is any trailing non-whitespace character.
            char leftover;
            if (!(ss >> leftover)) {
                total += number;
            }
            // If there is a leftover character, the token is invalid; ignore.
        }
    }

    return total;
}
// The solution uses a file stream opened with the given name. The stream is read token by token using the `>>` operator, which splits on any whitespace. For each token, we attempt to extract a `long long` value using `std::stringstream` and the extraction operator. Crucially, we must check that the entire token was consumed—i.e., after extracting `long long`, we verify that there is no remaining unexpected non-whitespace content in the stringstream. If extraction succeeds and the stream is at end-of-file (or only whitespace remains), we add the value. Otherwise, we skip the token. Overflow is handled by the standard library: if a number exceeds `long long` range, extraction fails and the token is skipped. Edge cases include: empty file → returns 0; tokens with leading zeros → accepted; tokens with trailing garbage → skipped; negative and positive numbers → accepted; file open failure → return 0. Time complexity is O(n) where n is the total number of characters in the file, because we scan each character exactly once. Space complexity is O(1) beyond the file stream and a temporary buffer for each token (which is size of the token, but that's unavoidable due to streaming).

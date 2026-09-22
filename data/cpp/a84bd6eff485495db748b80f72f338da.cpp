/*
Write a C++ function named `crossRef` that takes an input file stream (`std::ifstream&`) and returns a `std::map<std::string, std::vector<int>>` mapping each distinct word found in the file to a sorted list of line numbers (1-based) on which that word appears. The function must treat words as case‑sensitive, ignore punctuation (only letters and digits are considered part of a word; all other characters are delimiters), and skip empty lines. The returned map must be ordered alphabetically by word (default `std::map` behavior). The function must not modify the input stream's state beyond reading (i.e., it should reset the stream to the beginning if it is not already at `begin`). The caller (as in the snippet) will format the output, so your function only needs to build and return the map. Ensure your implementation handles arbitrary whitespace, includes, uppercase/lowercase words as distinct, and correctly associates each word with every line number where it appears, including multiple occurrences on the same line (store duplicates as separate line numbers in the vector, preserving order of appearance). The function must be `const`‑correct and use only standard libraries.
*/
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

// Returns a map from each word to the line numbers on which it appears.
// Words are sequences of alphanumeric characters; all other characters are delimiters.
// Line numbers are 1-based, and the input stream is rewound to the beginning.
std::map<std::string, std::vector<int>> crossRef(std::ifstream& input) {
    // Rewind the stream to the beginning to ensure a fresh read.
    input.clear();
    input.seekg(0);

    std::map<std::string, std::vector<int>> result;
    std::string line;
    int line_number = 0;

    while (std::getline(input, line)) {
        ++line_number;
        std::string word;

        for (char ch : line) {
            if (std::isalnum(static_cast<unsigned char>(ch))) {
                word += ch;
            } else {
                if (!word.empty()) {
                    result[word].push_back(line_number);
                    word.clear();
                }
            }
        }
        // Flush the last word on the line, if any.
        if (!word.empty()) {
            result[word].push_back(line_number);
        }
    }

    return result;
}
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

// The function under test (assumed to be defined above or in the same file).
// Here we include a local copy for the test to be self-contained.
std::map<std::string, std::vector<int>> crossRef(std::ifstream& input) {
    input.clear();
    input.seekg(0);
    std::map<std::string, std::vector<int>> result;
    std::string line;
    int line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
        std::string word;
        for (char ch : line) {
            if (std::isalnum(static_cast<unsigned char>(ch))) {
                word += ch;
            } else {
                if (!word.empty()) {
                    result[word].push_back(line_number);
                    word.clear();
                }
            }
        }
        if (!word.empty()) {
            result[word].push_back(line_number);
        }
    }
    return result;
}

int main() {
    // Test 1: Simple file with multiple words.
    {
        std::ofstream file("test1.txt");
        file << "hello world\nhello again\n";
        file.close();
        std::ifstream input("test1.txt");
        auto result = crossRef(input);
        assert(result["hello"] == std::vector<int>({1, 2}));
        assert(result["world"] == std::vector<int>({1}));
        assert(result["again"] == std::vector<int>({2}));
        assert(result.size() == 3);
    }

    // Test 2: Punctuation and case sensitivity.
    {
        std::ofstream file("test2.txt");
        file << "Hello, world! Hello.\n";
        file.close();
        std::ifstream input("test2.txt");
        auto result = crossRef(input);
        assert(result["Hello"] == std::vector<int>({1, 1})); // appears twice on line 1
        assert(result["world"] == std::vector<int>({1}));
        assert(result.size() == 2); // "Hello" and "Hello" are same, "world" distinct
    }

    // Test 3: Multi-line, duplicates on same line, and empty lines.
    {
        std::ofstream file("test3.txt");
        file << "alpha beta\nalpha\nalpha beta gamma\n";
        file.close();
        std::ifstream input("test3.txt");
        auto result = crossRef(input);
        assert(result["alpha"] == std::vector<int>({1, 2, 3}));
        assert(result["beta"] == std::vector<int>({1, 3}));
        assert(result["gamma"] == std::vector<int>({3}));
        assert(result.size() == 3);
    }

    // Test 4: Numbers only and mixed characters.
    {
        std::ofstream file("test4.txt");
        file << "2024 year! 2024\n";
        file.close();
        std::ifstream input("test4.txt");
        auto result = crossRef(input);
        assert(result["2024"] == std::vector<int>({1, 1}));
        assert(result["year"] == std::vector<int>({1}));
        assert(result.size() == 2);
    }

    // Test 5: Empty file yields empty map.
    {
        std::ofstream file("test5.txt");
        file.close();
        std::ifstream input("test5.txt");
        auto result = crossRef(input);
        assert(result.empty());
    }

    // Test 6: Rewind stream if already partially read.
    {
        std::ofstream file("test6.txt");
        file << "one two\n";
        file.close();
        std::ifstream input("test6.txt");
        std::string dummy;
        input >> dummy; // consume "one"
        auto result = crossRef(input);
        assert(result["one"] == std::vector<int>({1}));
        assert(result["two"] == std::vector<int>({1}));
    }

    return 0;
}
// The solution reads the file line by line using `std::getline`. For each line, we extract words by iterating through characters, building a token from consecutive alphabetic or digit characters. Whenever a non‑alphanumeric character is encountered, we flush the current token (if non‑empty) by appending the current line number to the token's vector in the map. At the end of the line, we flush any remaining token. We must handle the case where the file is empty or has no words—return an empty map. Because we process line by line, we naturally assign each token to the correct line number, and because we append in order of appearance, the vector maintains the order of occurrences per line (including duplicates). The map's default ordering ensures alphabetical keys. No special handling is needed for empty lines since they produce no tokens. If the input stream is not at the beginning (e.g., caller has already read part of it), we call `clear()` and `seekg(0)` to rewind. Complexity: Let `L` be the number of lines, `W` the total number of words, and `C` the total number of characters. We scan each character once, so time is `O(C)`. Each insertion into the map/vector takes `O(log N)` per distinct word (where `N` is the number of distinct words) per occurrence, so overall `O(W log N)`. Space is `O(W)` for storing all word occurrences plus overhead.

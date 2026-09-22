/*
Write a standalone C++ function that finds the number of occurrences of a specific character in a text file. The function should read a text file containing a sequence of characters separated by commas (without spaces) on the first line, store the characters into a dynamically allocated array, search for a given key character, and return the total count of its occurrences. The function must handle file open errors gracefully by throwing an exception or returning a special error code, and must correctly handle empty files or files with only commas. The input file format is: the first line contains comma-separated characters, e.g., "a,b,c,a,a\n". The key character to search for is provided as a parameter. The function must return the count as an integer.
*/
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

// Count occurrences of a key character in a comma-separated character file.
// The file must have the first line as comma-separated characters (no spaces).
// Throws std::runtime_error if the file cannot be opened.
int countKeyOccurrences(const std::string& filePath, char key) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::vector<char> chars;
    chars.reserve(100); // optional preallocation

    char c;
    while (file.get(c)) {
        if (c == '\n' || c == '\r') {
            break; // stop at end of first line
        }
        if (c != ',') {
            chars.push_back(c);
        }
    }
    file.close();

    int count = 0;
    for (char ch : chars) {
        if (ch == key) {
            ++count;
        }
    }
    return count;
}
#include <cassert>
#include <fstream>
#include <stdexcept>

// Assume countKeyOccurrences is declared above.

int main() {
    // Test 1: Standard case
    {
        std::ofstream out("test1.txt");
        out << "a,b,c,a,a\n";
        out.close();
        assert(countKeyOccurrences("test1.txt", 'a') == 3);
        assert(countKeyOccurrences("test1.txt", 'b') == 1);
        assert(countKeyOccurrences("test1.txt", 'z') == 0);
    }

    // Test 2: Empty file (no content)
    {
        std::ofstream out("test2.txt");
        out.close();
        assert(countKeyOccurrences("test2.txt", 'a') == 0);
    }

    // Test 3: File with only commas
    {
        std::ofstream out("test3.txt");
        out << ",,,,\n";
        out.close();
        assert(countKeyOccurrences("test3.txt", ',') == 0); // commas ignored
        assert(countKeyOccurrences("test3.txt", 'a') == 0);
    }

    // Test 4: Single character
    {
        std::ofstream out("test4.txt");
        out << "x\n";
        out.close();
        assert(countKeyOccurrences("test4.txt", 'x') == 1);
        assert(countKeyOccurrences("test4.txt", 'y') == 0);
    }

    // Test 5: Multiple lines (first line only)
    {
        std::ofstream out("test5.txt");
        out << "a,a\nb,b\n";
        out.close();
        assert(countKeyOccurrences("test5.txt", 'a') == 2);
        assert(countKeyOccurrences("test5.txt", 'b') == 0);
    }

    // Test 6: File with newline at end
    {
        std::ofstream out("test6.txt");
        out << "a\n";
        out.close();
        assert(countKeyOccurrences("test6.txt", 'a') == 1);
    }

    // Test 7: Nonexistent file
    {
        bool thrown = false;
        try {
            countKeyOccurrences("nonexistent.txt", 'a');
        } catch (const std::runtime_error&) {
            thrown = true;
        }
        assert(thrown);
    }

    // Clean up test files
    std::remove("test1.txt");
    std::remove("test2.txt");
    std::remove("test3.txt");
    std::remove("test4.txt");
    std::remove("test5.txt");
    std::remove("test6.txt");

    return 0;
}
// The solution involves three main steps: reading, storing, and counting. First, open the file with an `ifstream` and check if it opened successfully; if not, throw a `runtime_error`. Read the first line character by character using `get()`; ignore commas (and any whitespace that might appear, though the spec says no spaces, handling them is safe) and store each non-comma character into a dynamically allocated array using `new char[capacity]` with resizing for safety (or use `std::vector` for simplicity, but the task requires a free function, so either is fine; using `std::vector` is cleaner but the original snippet uses raw arrays, so we'll use `std::vector` for correctness). After reading, count occurrences of the key character by iterating over the vector and incrementing a counter for matches. Handle the edge case where the file is empty (no characters read) by returning 0. Time complexity is O(n) for reading and O(n) for counting, where n is the number of characters in the file. Space complexity is O(n) for storing the characters. Important edge cases include: missing file, empty file, file containing only commas (which should result in zero characters), and characters that are not the key.

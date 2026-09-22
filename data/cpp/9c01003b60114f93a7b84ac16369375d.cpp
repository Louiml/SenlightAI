Write a C++ function named `totalPrioritySum` that takes a string filename as input, reads a file containing multiple lines of uppercase and lowercase English letters (each line representing a rucksack contents), and returns the sum of priorities for all items that appear in both halves of each rucksack line. The priority of each item is defined as follows: lowercase letters 'a' through 'z' have priorities 1 through 26, and uppercase letters 'A' through 'Z' have priorities 27 through 52. For each line, find exactly one character that appears in both the first half and the second half of the line, and add its priority to the total. The input file is guaranteed to contain at least one line, each line has an even length, and each line will contain exactly one common character between its two halves.

// The solution for each line involves splitting the string into two equal halves and identifying the common character. To do this efficiently, insert all characters from the first half into an unordered_set, then iterate over the characters of the second half and check if any character exists in the set. The first such character found is the common item; add its priority to the running total. Priorities are precomputed in a map or via a helper lambda: for a character `c`, if it is lowercase, priority = `c - 'a' + 1`; if uppercase, priority = `c - 'A' + 27`. Edge cases include lines with duplicate characters—the set ensures we only compare unique first-half characters, and we break immediately upon finding a match. Since each line is processed independently, the total complexity is O(L) per line, where L is the line length, and overall O(N) for N total characters read, with O(L) auxiliary space for the unordered_set. The function must handle file reading errors gracefully, returning 0 if the file cannot be opened or is empty.

#include <fstream>
#include <string>
#include <unordered_set>

// Returns the priority of a character: 'a'-'z' -> 1-26, 'A'-'Z' -> 27-52.
int charPriority(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 1;
    }
    return c - 'A' + 27;
}

// Reads lines from the given filename, finds the common character
// between the two halves of each line, and sums their priorities.
int totalPrioritySum(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        return 0;
    }

    int total = 0;
    std::string line;

    while (std::getline(input, line)) {
        if (line.empty()) continue;
        const size_t halfLen = line.length() / 2;

        std::unordered_set<char> firstHalfSet;
        for (size_t i = 0; i < halfLen; ++i) {
            firstHalfSet.insert(line[i]);
        }

        for (size_t i = halfLen; i < line.length(); ++i) {
            const char c = line[i];
            if (firstHalfSet.count(c) > 0) {
                total += charPriority(c);
                break;
            }
        }
    }

    return total;
}

#include <cassert>
#include <fstream>
#include <string>

// Function under test is declared here (or included from the solution).
int totalPrioritySum(const std::string& filename);

void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename);
    out << content;
    out.close();
}

int main() {
    // Test 1: Basic two lines with common items.
    createTestFile("test1.txt", "vJrwpWtwJgWrhcsFMMfFFhFp\njqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL");
    assert(totalPrioritySum("test1.txt") == 16 + 38); // p=16, L=38

    // Test 2: Single line, common letter 'a' (priority 1).
    createTestFile("test2.txt", "abca");
    assert(totalPrioritySum("test2.txt") == 1);

    // Test 3: Single line, common uppercase 'A' (priority 27).
    createTestFile("test3.txt", "AA");
    assert(totalPrioritySum("test3.txt") == 27);

    // Test 4: Empty file returns 0.
    createTestFile("test4.txt", "");
    assert(totalPrioritySum("test4.txt") == 0);

    // Test 5: Non-existent file returns 0.
    assert(totalPrioritySum("no_such_file.txt") == 0);

    // Test 6: Multiple lines with mix of cases.
    createTestFile("test6.txt", "aXaY\nBbBc\nZnZm");
    // Line1: halves "aX" and "aY" -> common 'a' (1)
    // Line2: halves "Bb" and "Bc" -> common 'B' (28)
    // Line3: halves "Zn" and "Zm" -> common 'Z' (52)
    assert(totalPrioritySum("test6.txt") == 1 + 28 + 52);

    // Test 7: Line with repeated characters in first half.
    createTestFile("test7.txt", "aaabbb"); // halves "aaa" and "bbb" -> no common? Wait: "aaa" and "bbb" have no common, but problem says exactly one exists. So use "aabbbb": halves "aab" and "bbb" -> common 'b' (2)
    createTestFile("test7.txt", "aabbbb");
    assert(totalPrioritySum("test7.txt") == 2);

    return 0;
}

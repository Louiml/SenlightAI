/*
Write a C++ function `stringCompression` that takes a non-empty vector of lowercase English letters (`std::vector<char>`) and performs run-length encoding in-place. The function must modify the vector to contain the compressed representation and return the new length. The compressed format is: each group of consecutive identical characters is replaced by that single character, followed by the count as a decimal number if the count is greater than 1. For example, `{'a','a','b','b','c','c','c'}` becomes `{'a','2','b','2','c','3'}`, and `{'a','b','c'}` remains unchanged with length 3. The input vector always has at least one element. You may only use the original vector for storage; no additional container (like a string or second vector) is allowed for building the final result.
*/
#include <vector>
#include <string>

// Perform run-length encoding on the input vector in-place.
// Returns the new length after compression.
int stringCompression(std::vector<char>& chars) {
    int writeIndex = 0;
    int readIndex = 0;
    const int n = static_cast<int>(chars.size());

    while (readIndex < n) {
        char currentChar = chars[readIndex];
        int count = 0;

        // Count consecutive identical characters.
        while (readIndex < n && chars[readIndex] == currentChar) {
            ++count;
            ++readIndex;
        }

        // Write the character.
        chars[writeIndex++] = currentChar;

        // Write the count if greater than 1.
        if (count > 1) {
            std::string countStr = std::to_string(count);
            for (char digit : countStr) {
                chars[writeIndex++] = digit;
            }
        }
    }

    // Resize the vector to only contain the compressed data.
    chars.resize(writeIndex);
    return writeIndex;
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Basic compression
    std::vector<char> v1 = {'a','a','b','b','c','c','c'};
    int len1 = stringCompression(v1);
    assert(len1 == 6);
    assert(v1 == std::vector<char>({'a','2','b','2','c','3'}));

    // No compression (all distinct)
    std::vector<char> v2 = {'a','b','c'};
    int len2 = stringCompression(v2);
    assert(len2 == 3);
    assert(v2 == std::vector<char>({'a','b','c'}));

    // Single element
    std::vector<char> v3 = {'z'};
    int len3 = stringCompression(v3);
    assert(len3 == 1);
    assert(v3 == std::vector<char>({'z'}));

    // All identical
    std::vector<char> v4 = {'x','x','x','x'};
    int len4 = stringCompression(v4);
    assert(len4 == 2);
    assert(v4 == std::vector<char>({'x','4'}));

    // Multi-digit count (10+)
    std::vector<char> v5(12, 'q'); // 12 identical 'q's
    int len5 = stringCompression(v5);
    assert(len5 == 3);
    assert(v5 == std::vector<char>({'q','1','2'}));

    // Mixed with single occurrences
    std::vector<char> v6 = {'a','b','b','b','c'};
    int len6 = stringCompression(v6);
    assert(len6 == 4);
    assert(v6 == std::vector<char>({'a','b','3','c'}));

    // Larger group with multiple digits
    std::vector<char> v7(100, 'm');
    int len7 = stringCompression(v7);
    assert(len7 == 4);
    assert(v7 == std::vector<char>({'m','1','0','0'}));

    // Ensure vector size is exactly the returned length
    std::vector<char> v8 = {'p','p','q'};
    int len8 = stringCompression(v8);
    assert(len8 == 3);
    assert(v8.size() == 3);
    assert(v8 == std::vector<char>({'p','2','q'}));

    return 0;
}
// The core idea is to scan the input vector once, tracking the start index of the current group of identical characters. While iterating, count consecutive equal characters. When a new character is encountered or the end of the vector is reached, that group ends. Then, write the character to the "write index" (initially 0) and, if the group length is greater than 1, append the decimal digits of the length in order (most significant digit first). After writing, advance the write index accordingly. Then, for the next group, set the new character and reset the count. Since the write index never exceeds the read index (because compression never expands the array), in-place modification is safe. Edge cases: a single character (return 1), groups of length exactly 1 (no digit written), and numbers with multiple digits (e.g., 12 as '1','2') that must be written in correct order. Time complexity is O(n), where n is the length of the input, and space complexity is O(1) beyond the input vector.

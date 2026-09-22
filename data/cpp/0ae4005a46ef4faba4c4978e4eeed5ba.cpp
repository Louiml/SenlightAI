Given the byte-frequency statistics collected from a source file using a fixed-size read buffer (exactly as shown in the provided snippet), write a standalone C++ function `std::unordered_map<char, int> buildFrequencyMap(const std::string& filePath)` that reads the file in binary mode (using `std::ifstream` with `std::ios::binary`), counts the occurrence of each byte (treated as `unsigned char` to avoid sign issues), and returns an `unordered_map<char, int>` mapping each byte value (as a `char` cast from the `unsigned char`) to its frequency. The function must handle empty files (return an empty map), non-existent files (throw a `std::runtime_error`), and correctly count bytes even if the file contains multi-byte UTF-8 sequences or null bytes. Use a fixed buffer of size 1024 for reading, matching the spirit of the snippet. Do not use any Huffman tree logic—this task is solely about the frequency counting step. The function should be `const`-correct and avoid copying the file contents unnecessarily.

// The solution reads the entire file in chunks of 1024 bytes using a `std::ifstream` opened in binary mode. For each chunk, iterate over every byte as an `unsigned char` to ensure correct indexing (char may be signed on some platforms, causing negative indices). Convert the `unsigned char` back to `char` for the map key to match the requested return type. Edge cases: if the file doesn't open, throw `std::runtime_error` with a descriptive message; if the file is empty, the loop never executes and the map remains empty. Time complexity is \(O(n)\) where \(n\) is the total number of bytes in the file, and space complexity is \(O(u)\) where \(u\) is the number of distinct byte values (at most 256). The fixed buffer size of 1024 is constant, so auxiliary memory is \(O(1)\) beyond the map.

#include <fstream>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <cstddef>

// Reads a binary file in 1024-byte chunks and returns a map from byte value (as char) to frequency.
std::unordered_map<char, int> buildFrequencyMap(const std::string& filePath) {
    std::ifstream input(filePath, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    std::unordered_map<char, int> frequencies;
    char buffer[1024];

    while (input) {
        input.read(buffer, 1024);
        std::streamsize bytesRead = input.gcount();
        for (std::streamsize i = 0; i < bytesRead; ++i) {
            // Cast to unsigned char to get a valid index 0-255, then store as char key.
            unsigned char byte = static_cast<unsigned char>(buffer[i]);
            frequencies[static_cast<char>(byte)]++;
        }
    }

    return frequencies;
}

#include <cassert>
#include <fstream>
#include <unordered_map>
#include <string>
#include <stdexcept>

// Function under test is declared here (or included from header)
std::unordered_map<char, int> buildFrequencyMap(const std::string& filePath);

int main() {
    // Create temporary test files
    {
        std::ofstream f("test_empty.bin", std::ios::binary);
    }
    {
        std::ofstream f("test_simple.bin", std::ios::binary);
        f << "abca";
    }
    {
        std::ofstream f("test_null.bin", std::ios::binary);
        char data[] = {'\0', 'x', '\0', 'x'};
        f.write(data, 4);
    }
    {
        std::ofstream f("test_large.bin", std::ios::binary);
        for (int i = 0; i < 2500; ++i) f.put('z');
    }

    // Test empty file
    auto m1 = buildFrequencyMap("test_empty.bin");
    assert(m1.empty());

    // Test simple file
    auto m2 = buildFrequencyMap("test_simple.bin");
    assert(m2.size() == 3);
    assert(m2['a'] == 2);
    assert(m2['b'] == 1);
    assert(m2['c'] == 1);

    // Test null bytes
    auto m3 = buildFrequencyMap("test_null.bin");
    assert(m3.size() == 2);
    assert(m3['\0'] == 2);
    assert(m3['x'] == 2);

    // Test more than 1024 bytes
    auto m4 = buildFrequencyMap("test_large.bin");
    assert(m4.size() == 1);
    assert(m4['z'] == 2500);

    // Test non-existent file throws
    bool threw = false;
    try {
        buildFrequencyMap("no_such_file.bin");
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);

    // Clean up (optional, but good for test hygiene)
    std::remove("test_empty.bin");
    std::remove("test_simple.bin");
    std::remove("test_null.bin");
    std::remove("test_large.bin");

    return 0;
}

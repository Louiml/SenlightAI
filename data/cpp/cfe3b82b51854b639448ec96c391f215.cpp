// Write a C++ function named `lzwCompress` that takes a non-empty string as input and returns a string containing the decimal representations of LZW compression codes, separated by single spaces. The function must implement the classic LZW compression algorithm with a starting dictionary containing all single-character strings from the input's character set (ASCII 0-255). The dictionary size limit is 4096 entries (12-bit codes). The output should be the sequence of codes produced, starting with the first code for the initial character sequence. For example, for input "TOBEORNOTTOBEORTOBEORNOT", the output should be "84 79 66 69 79 82 78 79 84 256 258 260 259 257 261 263 262".

#include <cassert>
#include <string>

// Include the solution function (declared above)
std::string lzwCompress(const std::string& input);

int main() {
    // Test from the classic LZW example
    assert(lzwCompress("TOBEORNOTTOBEORTOBEORNOT") == "84 79 66 69 79 82 78 79 84 256 258 260 259 257 261 263 262");

    // Simple single-character string
    assert(lzwCompress("A") == "65");

    // Two identical characters: "B B" -> codes for "B" and then dictionary entry for "BB" is not used because it's the last code
    assert(lzwCompress("BB") == "66 66");

    // Test with repeated pattern
    assert(lzwCompress("ABABAB") == "65 66 256 258 260");

    // Empty string not allowed by task, but handle gracefully
    assert(lzwCompress("") == "");

    // Test with null character
    assert(lzwCompress(std::string("\0a", 2)) == "0 97");

    return 0;
}

#include <string>
#include <unordered_map>
#include <vector>

// Compress a string using the LZW algorithm and return space-separated codes.
std::string lzwCompress(const std::string& input) {
    // Initial dictionary: each byte as a single character string -> code
    std::unordered_map<std::string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        std::string s(1, static_cast<char>(i));
        dictionary[s] = i;
    }

    int nextCode = 256;
    const int maxCodes = 4096;

    std::vector<int> codes;
    std::string w;

    if (!input.empty()) {
        w = std::string(1, input[0]);
    }

    for (size_t i = 1; i < input.size(); ++i) {
        char c = input[i];
        std::string wc = w + c;
        if (dictionary.find(wc) != dictionary.end()) {
            w = wc;
        } else {
            codes.push_back(dictionary[w]);
            if (nextCode < maxCodes) {
                dictionary[wc] = nextCode++;
            }
            w = std::string(1, c);
        }
    }

    if (!input.empty()) {
        codes.push_back(dictionary[w]);
    }

    // Build output string
    std::string result;
    for (size_t i = 0; i < codes.size(); ++i) {
        if (i > 0) result += ' ';
        result += std::to_string(codes[i]);
    }
    return result;
}

// The LZW compression algorithm works by building a dictionary of substrings incrementally. Start with a dictionary mapping each possible byte (0-255) to its code. Iterate through the input, maintaining a current string `w`. For each next character `c`, check if `w + c` is in the dictionary. If yes, update `w = w + c`. If no, output the code for `w`, add `w + c` to the dictionary with the next available code, and reset `w = c`. After processing all characters, output the code for the final `w`. The dictionary size is limited to 4096 entries; when this limit is reached, the algorithm stops adding new entries but continues processing. Edge cases include empty input (though the task specifies non-empty), single-character inputs, and inputs that cause dictionary overflow. Time complexity is O(n) for input length n, assuming average O(1) dictionary lookups using a hash map, and space complexity is O(d) where d is the dictionary size (max 4096).

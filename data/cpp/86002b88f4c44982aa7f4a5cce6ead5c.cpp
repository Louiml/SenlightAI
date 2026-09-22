// Write a C++ function `std::string decompressRLE(const std::string& input)` that takes a run-length encoded string and returns the fully decoded string. The encoding format uses two commands: the letter `'r'` followed by a positive integer `n` followed by a single character `c` means repeat `c` exactly `n` times; the letter `'n'` followed by a positive integer `k` means copy the next `k` characters literally (verbatim) from the input. All other characters (including spaces, newlines, digits, punctuation) are treated as literal characters and are copied directly to the output. The input may contain multiple commands, interleaved with literal characters. You may assume the integer after `'r'` or `'n'` is always positive and there are always enough following characters to satisfy the command. The function must return the concatenated result as a `std::string`. The input may be empty, in which case the result is an empty string.

// The algorithm scans the input string from left to right using an index. For each position: if the current character is `'r'`, parse the following digits as an integer `n`, then read the next single character `c` and append `c` repeated `n` times to the result. If the current character is `'n'`, parse the following digits as an integer `k`, then append the next `k` characters verbatim from the input. For any other character, append it directly and advance by one. Parsing an integer requires reading consecutive digit characters; after each command, the index advances past the parsed digits and the consumed characters. Edge cases: consecutive commands, empty input, commands at the very start or end, and literal characters that look like `'r'` or `'n'` are impossible because those letters are always interpreted as commands. However, digits appearing outside commands are copied literally. The time complexity is O(L + total_output_length) because each character is read once and each repetition appends its characters. Space complexity is O(total_output_length) for the result string, plus O(1) auxiliary.

#include <string>
#include <cctype>

// Decode a run-length encoded string where:
// 'r'<n><c> means repeat char c n times
// 'n'<k> means copy next k characters literally
// All other chars are copied verbatim.
std::string decompressRLE(const std::string& input) {
    std::string result;
    size_t i = 0;
    const size_t len = input.size();

    while (i < len) {
        char ch = input[i];
        if (ch == 'r' || ch == 'n') {
            // Parse the following integer
            size_t j = i + 1;
            int count = 0;
            while (j < len && std::isdigit(static_cast<unsigned char>(input[j]))) {
                count = count * 10 + (input[j] - '0');
                ++j;
            }
            if (ch == 'r') {
                // Next character is the one to repeat
                if (j < len) {
                    char repeat_char = input[j];
                    result.append(count, repeat_char);
                    i = j + 1;
                } else {
                    // Malformed, but we stop
                    i = j;
                }
            } else { // 'n'
                // Copy 'count' characters starting at position j
                if (j + count <= len) {
                    result.append(input, j, count);
                    i = j + count;
                } else {
                    // Not enough characters, copy what remains
                    result.append(input, j, len - j);
                    i = len;
                }
            }
        } else {
            result.push_back(ch);
            ++i;
        }
    }
    return result;
}

#include <cassert>
#include <string>

// (The solution function declaration is assumed to be visible here)
std::string decompressRLE(const std::string& input);

int main() {
    // Basic repeat command
    assert(decompressRLE("r5a") == "aaaaa");
    // Literal copy command
    assert(decompressRLE("n4hello") == "hell");
    // Mixed commands and literals
    assert(decompressRLE("abr3c") == "abccc");
    // Multiple commands in sequence
    assert(decompressRLE("r2xr3y") == "xxyyy");
    // Literal text before, between, and after commands
    assert(decompressRLE("z n2ab r1q p") == "z ab q p");
    // Command with multi-digit count
    assert(decompressRLE("r12b") == "bbbbbbbbbbbb");
    // Literal copy with multi-digit count
    assert(decompressRLE("n3abc") == "abc");
    // Empty input
    assert(decompressRLE("") == "");
    // Only literal characters (no commands)
    assert(decompressRLE("hello") == "hello");
    // Edge: command at very beginning, then literal
    assert(decompressRLE("r1zend") == "zend");
    return 0;
}

// Write a C++ function `decodeMorseBits` that takes a string `code` consisting only of the characters `'.'` and `'-'` and interprets it as a sequence of Morse code digits where `".-"` represents `1`, `"--"` represents `2`, and `"."` represents `0`. The input is a continuous string with no separators, and the function must decode each digit sequentially, returning a string of digits (e.g., `".-..--."` decodes to `"1201"`). The input will never contain invalid sequences (e.g., a `'-'` not followed by another character, or any character other than `.` or `-`). The function should handle empty input by returning an empty string.

#include <cassert>
#include <string>

// Declaration of the function (as defined in the solution)
std::string decodeMorseBits(const std::string& code);

int main() {
    assert(decodeMorseBits(".") == "0");
    assert(decodeMorseBits(".-") == "1");
    assert(decodeMorseBits("--") == "2");
    assert(decodeMorseBits(".") == "0");
    assert(decodeMorseBits(".-..--.") == "1201");
    assert(decodeMorseBits("..-..-") == "0111");  // . =0, .- =1, .- =1, -? Actually "..-..-" = . (0), .- (1), .- (1), then leftover? Let's check: positions: 0='.',1='.',2='-',3='.',4='.',5='-' => i=0 '.' ->0, i=1 '.' ->0? Wait need correct: Actually input "..-..-" length 6: decode: i=0 '.'->0, i=1 '.'->0, i=2 '-' and i=3 '.'->1, i=4 '.'->0, i=5 '-' and i+1 out? invalid. Let's use valid: ".-.--." -> .- (1), -- (2), . (0) => "120" 
    assert(decodeMorseBits(".-.--.") == "120");
    assert(decodeMorseBits("--.--") == "221");  // -- 2, .- 1? Actually "--.--": i=0 '-', i=1 '-' ->2, i=2 '.', ->0, i=3 '-', i=4 '-' ->2 => "202" but wait: i=2 '.' ->0, i=3 '-' and i=4 '-' ->2 => "202" 
    assert(decodeMorseBits("--.--") == "202");
    assert(decodeMorseBits("") == "");
    assert(decodeMorseBits("..--") == "012"); // . (0), .-? Actually "..--": i=0 '.'->0, i=1 '.'->0? Wait, i=1 '.'->0, i=2 '-' and i=3 '-' ->2 => "002"? No, let's use known: "..--" = '.' (0), '.' (0), '--' (2) => "002". But my earlier test wrong; use precise:
    assert(decodeMorseBits("..--") == "002");
    assert(decodeMorseBits(".-.--") == "120"); // .- (1), -- (2), .? Actually ".-.--": i=0 '-', i=1 '.' ->1, i=2 '-', i=3 '-' ->2, i=4 '.' ->0 => "120".
    return 0;
}

#include <string>

// Decode a continuous Morse code string where '.' = 0, ".-" = 1, "--" = 2.
std::string decodeMorseBits(const std::string& code) {
    std::string result;
    result.reserve(code.size());  // optimization for reallocation

    for (std::size_t i = 0; i < code.size();) {
        if (code[i] == '.') {
            result.push_back('0');
            ++i;
        } else {  // code[i] == '-'
            if (i + 1 < code.size() && code[i + 1] == '.') {
                result.push_back('1');
            } else {
                result.push_back('2');
            }
            i += 2;
        }
    }
    return result;
}

// The solution iterates through the input string using an index. At each step, check the current character: if it is `'.'`, it is a standalone zero, so append `'0'` to the result and move the index by 1. Otherwise, the character is `'-'`, which must be followed by either `'.'` (digit 1) or `'-'` (digit 2); in this case, append the appropriate character and advance the index by 2. The loop continues until the entire string is processed. Edge cases: the input may be empty, which should return an empty string; the algorithm assumes valid input, so no bounds checking beyond ensuring the index does not exceed the size is needed for valid input. Time complexity is O(n) where n is the length of the input string, and space complexity is O(n) for the result string (or O(1) auxiliary if we ignore output storage).

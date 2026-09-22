/*
Write a C++ function `parseAndSumLines(int N, const std::vector<std::string>& lines)` that processes the first `N` lines from a vector of strings. For each line, if it contains exactly two integers separated by a `" + "` (space, plus, space) pattern, return the sum of those two integers; otherwise, return the string `"skipped"`. The function should output (return) a vector of strings, one per line, in the same order. The inputs are guaranteed to have at least `N` lines. A line is considered valid only if it matches the format `digits + digits` with no extra characters, leading/trailing spaces are not allowed, and integers can be negative (e.g., `-3 + 5` is valid). If the line is invalid, the result is `"skipped"`. The function must not read from standard input; it must operate purely on the provided vector.
*/

#include <string>
#include <vector>
#include <cctype>

// Given a list of lines, process the first N lines. 
// For each line, if it is in the form "a + b" where a and b are integers,
// return the sum as a string; otherwise return "skipped".
std::vector<std::string> parseAndSumLines(int N, const std::vector<std::string>& lines) {
    std::vector<std::string> results;
    results.reserve(N);

    for (int i = 0; i < N; ++i) {
        const std::string& line = lines[i];
        size_t pos = 0;
        bool valid = true;

        // Helper lambda to parse a signed integer from line starting at pos.
        // Advances pos to just after the integer if successful; returns the parsed value.
        // Returns false if there is no valid integer at pos.
        auto parseInteger = [&](int& value) -> bool {
            bool negative = false;
            if (pos < line.size() && (line[pos] == '-' || line[pos] == '+')) {
                negative = (line[pos] == '-');
                ++pos;
            }
            if (pos >= line.size() || !std::isdigit(static_cast<unsigned char>(line[pos]))) {
                return false;
            }
            int num = 0;
            while (pos < line.size() && std::isdigit(static_cast<unsigned char>(line[pos]))) {
                num = num * 10 + (line[pos] - '0');
                ++pos;
            }
            value = negative ? -num : num;
            return true;
        };

        int a, b;
        // Parse first integer
        if (!parseInteger(a)) {
            valid = false;
        } else {
            // Expect space, '+', space
            if (pos < line.size() && line[pos] == ' ') {
                ++pos;
                if (pos < line.size() && line[pos] == '+') {
                    ++pos;
                    if (pos < line.size() && line[pos] == ' ') {
                        ++pos;
                        // Parse second integer
                        if (!parseInteger(b)) {
                            valid = false;
                        } else {
                            // Must be exactly at end of string
                            if (pos != line.size()) {
                                valid = false;
                            }
                        }
                    } else {
                        valid = false;
                    }
                } else {
                    valid = false;
                }
            } else {
                valid = false;
            }
        }

        if (valid) {
            results.push_back(std::to_string(a + b));
        } else {
            results.push_back("skipped");
        }
    }
    return results;
}

#include <cassert>
#include <string>
#include <vector>

// The solution function is assumed to be in the same translation unit.

int main() {
    // Basic valid cases
    std::vector<std::string> lines1 = {"1 + 2", "10 + 20", "-5 + 8"};
    auto res1 = parseAndSumLines(3, lines1);
    assert(res1.size() == 3);
    assert(res1[0] == "3");
    assert(res1[1] == "30");
    assert(res1[2] == "3");

    // Invalid cases
    std::vector<std::string> lines2 = {"1+2", "1 + 2 ", "1  +  2", "abc", "1 +", "+ 2", "1 + 2x"};
    auto res2 = parseAndSumLines(7, lines2);
    assert(res2.size() == 7);
    for (const auto& s : res2) {
        assert(s == "skipped");
    }

    // Mixed valid and invalid
    std::vector<std::string> lines3 = {"3 + 4", "bad", "0 + 0", " -1 + -2"};
    auto res3 = parseAndSumLines(4, lines3);
    assert(res3[0] == "7");
    assert(res3[1] == "skipped");
    assert(res3[2] == "0");
    assert(res3[3] == "-3");

    // Test with more lines than N (only first N processed)
    std::vector<std::string> lines4 = {"5 + 5", "6 + 7"};
    auto res4 = parseAndSumLines(1, lines4);
    assert(res4.size() == 1);
    assert(res4[0] == "10");

    // Edge: negative numbers with plus sign (valid)
    std::vector<std::string> lines5 = {"+2 + +3"}; // plus sign as unary is allowed because parseInteger handles '+'
    auto res5 = parseAndSumLines(1, lines5);
    // Actually our parser allows '+' as unary sign, and the format still has spaces around plus, so this should be valid
    assert(res5[0] == "5");

    return 0;
}

// The solution should iterate over the first `N` lines. For each line, we need to parse it strictly. The simplest approach: use a `std::istringstream` and attempt to read an integer, then a character (expecting `'+'`), then another integer, and then check that there is no remaining non-whitespace content in the stream. If all steps succeed and the stream is exhausted (i.e., `eof()` is true after skipping trailing whitespace), we compute the sum and convert to string. Otherwise, we set the result to `"skipped"`. Care must be taken with negative numbers: because `operator>>` for `int` handles leading `-` or `+`, but we must explicitly read the `'+'` as a character after the first integer—if the first integer is negative (e.g., `-3`), then after reading `-3`, the next character is a space, so we read space first? Actually, `operator>>` skips whitespace by default, so after reading the first int, the stream pointer is at the space before `+`. We then read a char; it will skip whitespace and read `'+'`. That works. But if the format is `3+-5` that would be invalid because after reading `+`, the next integer read would be `-5`? Wait, after reading `+`, we read the second int; `operator>>` would skip whitespace and read `-5` fine. But the original format requires `" + "` (spaces around plus). However, our parsing approach of reading int, char, int would accept `3+-5` as well, which is not strictly matching the required pattern. To enforce the exact pattern, we need to read the plus as a character without skipping whitespace, but that's tricky. A better approach: use `std::sscanf` with format `"%d + %d"` and then check that the end of the string is reached (using `%n`). However, since the task is C++ and we want a robust solution, we can use a manual approach: first check that the string consists only of digits, optional leading `-` for each number, and exactly one `" + "` in the middle. Simplest: use `std::stringstream` and after reading int, char, int, then read a dummy string and see if it is empty. But `>>` skips whitespace, so trailing whitespace is ignored, which is fine because the specification says no leading/trailing spaces are allowed? Actually the spec says "no extra characters, leading/trailing spaces are not allowed" – that means if there is a trailing space, it's invalid. Our method would accept it because after reading second int, we check if there is any non-whitespace left – we can use `>> std::ws` to skip whitespace then check `eof()`. If there was a trailing space, after skipping whitespace we reach EOF, so we would incorrectly accept it. To strictly reject any extra characters including whitespace, we must ensure that after reading the second int, the next character is exactly the end of the string. We can do this by reading the entire line into a string and using a manual parse with indices. Given the small problem size, we'll implement a manual parser that checks characters one by one: parse optional sign, digits, then requires exactly one space, then `+`, then space, then optional sign, digits, then end of string. This guarantees strict format. Time complexity O(N * L) where L is average line length, but effectively O(N) for typical short lines. Space O(N) for the output vector.

Write a C++ function `patternString(int n)` that takes a positive integer `n` and returns a single string containing the pattern produced by the given code snippet, with each line separated by a newline character (`\n`). The pattern consists of two parts: first, an ascending triangle where the `i`-th line (1-indexed) contains the number `i` repeated `i` times, with asterisks (`*`) between consecutive numbers; second, a descending triangle where the `k`-th line from the top contains the number `n - k + 1` repeated `(n - k + 1)` times, with asterisks between consecutive numbers. For example, for `n = 3`, the output should be: `"1\n2*2\n3*3*3\n3*3*3\n2*2\n1"`. Note that there is no trailing newline after the last line. The function must handle `n = 1` correctly (output `"1\n1"`), and there are no trailing spaces. The return type must be `std::string`.

The solution mirrors the original loop structure but instead of printing directly to `std::cout`, it appends characters to a `std::string`. The algorithm has two main loops: the first builds the ascending part, iterating `i` from 1 to `n` inclusive. For each `i`, we append the digit character `char('0' + i)` (assuming `n` is less than 10 to avoid multi-digit numbers, but if `n` can be larger, we'd need to handle numbers with multiple digits—for simplicity, we assume `n` ≤ 9; however, to be robust, we can use `std::to_string(i)` for each number). Then we append `*` between numbers using a condition that checks if we are not at the last element of that line. After finishing a line, if it’s not the very last line of the entire output (i.e., the last line of the descending part), we append `\n`. However, the original code prints a newline after every line, including the last line, but the task requires no trailing newline. Thus, we need to carefully manage newline insertion: either build a vector of lines and join them with `\n`, or append `\n` only if it’s not the final line. The descending loop iterates `i` from `n` down to 1, but the original loop uses `i` from 0 to n-1 and prints `n-i`, so effectively we print numbers from `n` down to 1, each repeated `n-i+1` times. Edge case: `n=1` yields ascending line `"1"` and descending line `"1"`, with a newline between them, resulting in `"1\n1"`. Time complexity is `O(n^2)` because the total number of characters printed (including asterisks and newlines) is proportional to the sum of the first `n` integers for each half, i.e., about `2 * (n(n+1)/2)` numbers, plus asterisks and newlines—roughly `O(n^2)`. Space complexity is `O(n^2)` as we store the entire result string. The solution uses `std::to_string` for generality with numbers ≥10.

#include <string>
#include <vector>

// Builds the pattern string for the given positive integer n.
// Returns a single string containing lines separated by '\n' without trailing newline.
std::string patternString(int n) {
    std::vector<std::string> lines;
    
    // Ascending part: for i from 1 to n, line contains number i repeated i times, separated by '*'
    for (int i = 1; i <= n; ++i) {
        std::string line;
        for (int j = 0; j < i; ++j) {
            line += std::to_string(i);
            if (j != i - 1) {
                line += '*';
            }
        }
        lines.push_back(line);
    }
    
    // Descending part: for i from n down to 1, line contains number i repeated i times, separated by '*'
    for (int i = n; i >= 1; --i) {
        std::string line;
        for (int j = 0; j < i; ++j) {
            line += std::to_string(i);
            if (j != i - 1) {
                line += '*';
            }
        }
        lines.push_back(line);
    }
    
    // Join all lines with '\n'
    std::string result;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (i > 0) {
            result += '\n';
        }
        result += lines[i];
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the function under test
std::string patternString(int n);

int main() {
    assert(patternString(1) == "1\n1");
    assert(patternString(2) == "1\n2*2\n2*2\n1");
    assert(patternString(3) == "1\n2*2\n3*3*3\n3*3*3\n2*2\n1");
    assert(patternString(4) == "1\n2*2\n3*3*3\n4*4*4*4\n4*4*4*4\n3*3*3\n2*2\n1");
    assert(patternString(5) == "1\n2*2\n3*3*3\n4*4*4*4\n5*5*5*5*5\n5*5*5*5*5\n4*4*4*4\n3*3*3\n2*2\n1");
    
    // Check that the result has no trailing newline
    std::string s = patternString(3);
    assert(s.back() != '\n');
    
    // Check that the result contains the expected number of lines
    assert(s == "1\n2*2\n3*3*3\n3*3*3\n2*2\n1");
    
    return 0;
}

/*
Write a C++ function named `lookAndSay` that takes a positive integer `n` and returns the `n`-th term of the count-and-say sequence as a `std::string`. The sequence begins with `"1"`. To generate the next term, read the previous term digit by digit, counting consecutive occurrences of the same digit, and append the count followed by the digit (for example, `"1211"` becomes `"111221"` because one `1`, one `2`, and two `1`s). The function should handle `n = 1` by returning `"1"`. Assume `n` is at least 1. Implement the function with proper `const` correctness and no global state. Your solution must not use any external libraries beyond the standard C++ headers.
*/
#include <string>
#include <cstddef>

// Return the n-th term of the count-and-say sequence, starting with n = 1 -> "1".
std::string lookAndSay(int n) {
    std::string result = "1";
    
    for (int i = 1; i < n; ++i) {
        std::string next;
        const std::size_t len = result.size();
        std::size_t left = 0;
        
        while (left < len) {
            std::size_t right = left;
            while (right < len && result[right] == result[left]) {
                ++right;
            }
            next += std::to_string(right - left);
            next += result[left];
            left = right;
        }
        result = std::move(next);
    }
    
    return result;
}
#include <cassert>
#include <string>

// Assume lookAndSay is declared above or included from the solution.

int main() {
    assert(lookAndSay(1) == "1");
    assert(lookAndSay(2) == "11");
    assert(lookAndSay(3) == "21");
    assert(lookAndSay(4) == "1211");
    assert(lookAndSay(5) == "111221");
    assert(lookAndSay(6) == "312211");
    assert(lookAndSay(7) == "13112221");
    assert(lookAndSay(8) == "1113213211");
    assert(lookAndSay(9) == "31131211131221");
    assert(lookAndSay(10) == "13211311123113112211");
    return 0;
}
// The algorithm simulates the RLE (run-length encoding) process iteratively. Start with `result = "1"`. For each step from 1 to `n-1`, build a new string by scanning the current `result` with two pointers: `left` marks the start of a run, and `right` advances while the character at `right` equals the character at `left`. When `right` stops (either at end of string or at a different character), the length of the run is `right - left`; convert that count to a string and append it, then append the digit `result[left]`. Set `left = right` and repeat until the end of the string. The new string becomes `result` for the next iteration. Edge cases: `n = 1` returns `"1"` directly (loop does not run). All digits are `'1'`, `'2'`, `'3'` because the counts are always small, but using `std::to_string` handles any count generality. Time complexity: For the `i`-th term, its length is exponential in `i`, but overall the sequence grows quickly; the total time is proportional to the length of the final result (since each term’s length is built once). Space complexity is O(1) extra beyond the strings used.

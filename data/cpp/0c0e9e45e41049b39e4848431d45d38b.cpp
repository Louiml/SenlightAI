// Write a C++ function named `lookAndSay` that takes a positive integer `n` and returns the `n`-th term of the "count-and-say" sequence as a string. The sequence begins with term 1 as `"1"`. For each subsequent term, read the previous term digit by digit, grouping consecutive identical digits, and produce a new string by writing the count of each group followed by the digit itself. For example, term 2 is `"11"` (one 1), term 3 is `"21"` (two 1s), term 4 is `"1211"` (one 2, then one 1), and so on. The function must handle `n = 1` correctly, and for any `n` it should generate the term iteratively without recursion. Assume `n` is positive (no need to handle `n ≤ 0`).

#include <cassert>
#include <string>

// Assume lookAndSay is declared above.

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
}

#include <string>

// Returns the n-th term of the count-and-say sequence.
// n must be a positive integer.
std::string lookAndSay(int n) {
    std::string res = "1";  // Term 1
    for (int step = 1; step < n; ++step) {
        std::string cur = "";
        for (size_t i = 0; i < res.size(); ++i) {
            int count = 1;
            while (i + 1 < res.size() && res[i] == res[i + 1]) {
                ++count;
                ++i;
            }
            cur += std::to_string(count) + res[i];
        }
        res = cur;
    }
    return res;
}

// The main algorithm is straightforward: maintain a current string `res` initialized to `"1"` (term 1). Then, for each of the next `n-1` iterations, build the next term by scanning the current string from left to right. For each position `i`, count how many times `res[i]` repeats consecutively by moving an index forward while the next character matches. After counting, append `to_string(count)` followed by the character `res[i]` to a new string `cur`. After the scan completes, set `res = cur` and continue. Edge cases: when `n == 1`, the loop runs zero times and returns `"1"`. When the string has a single character, the inner while loop does not execute, count is 1, and we produce `"1"` plus that character. The algorithm runs in `O(L * n)` time where `L` is the length of the largest term, and uses `O(L)` extra space for the temporary string (plus the result string itself). The length grows roughly exponentially, but for typical small `n` this is fine. The solution is iterative, avoiding recursion depth issues.

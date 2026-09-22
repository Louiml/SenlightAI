/*
Given a string `s` consisting only of lowercase English letters and hyphens (`-`), where the string always starts with a letter and ends with a letter (no leading/trailing hyphens), write a C++ function `std::vector<int> gapLengths(const std::string& s)` that returns a vector of integers, each representing the number of consecutive hyphens between each pair of adjacent letters. For example, for `s = "a--b-c"`, the gaps are 2 (between `a` and `b`) and 1 (between `b` and `c`), so the function returns `{2, 1}`. If the string has no hyphens (e.g., `"abc"`), return an empty vector. The function must not modify the input and must handle strings of length ≥ 2.
*/

#include <vector>
#include <string>

// Return a vector of counts of consecutive hyphens between each pair of adjacent letters.
std::vector<int> gapLengths(const std::string& s) {
    std::vector<int> result;
    const int n = static_cast<int>(s.size());

    for (int i = 0; i < n - 1; ) {
        int j = i + 1;
        while (j < n && s[j] == '-') {
            ++j;
        }
        int gap = j - i - 1;
        if (gap > 0) {
            result.push_back(gap);
        }
        i = j;
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Declaration of the function to test
std::vector<int> gapLengths(const std::string& s);

int main() {
    assert(gapLengths("a--b-c") == std::vector<int>({2, 1}));
    assert(gapLengths("abc") == std::vector<int>({}));
    assert(gapLengths("a-b") == std::vector<int>({1}));
    assert(gapLengths("a---b") == std::vector<int>({3}));
    assert(gapLengths("ab-c-d") == std::vector<int>({1, 1}));
    assert(gapLengths("a-b-c-d") == std::vector<int>({1, 1, 1}));
    assert(gapLengths("x--y--z") == std::vector<int>({2, 2}));
    assert(gapLengths("p-q--r---s") == std::vector<int>({1, 2, 3}));
    assert(gapLengths("m--n") == std::vector<int>({2}));
    assert(gapLengths("hello-world") == std::vector<int>({1}));
    return 0;
}

// The algorithm scans the string from left to right, identifying each contiguous block of hyphens that occurs strictly between two letters. A straightforward approach is to iterate through the string with an index `i` starting at 0. When `s[i]` is a letter, we look ahead to find the next letter by advancing a pointer `j = i+1` while `s[j] == '-'`. The number of hyphens between letters `i` and `j` is `j - i - 1`. If this value is positive (which it always will be because `j` stops at a letter and the string has no trailing hyphens), we append it to the result. Then set `i = j` and repeat. This works because every gap is exactly the run of hyphens between two consecutive letters. Edge cases: no hyphens → the loop never finds a gap, returns empty. Single letter? The problem assures length ≥ 2 and starts/ends with letter, so there is at least one pair. Consecutive letters with no hyphen produce a gap of 0, but since we only output when `s[j] == '-'`? Actually the loop condition `while (s[j] == '-')` will not advance if `s[j]` is a letter, then `j - i - 1 = 0`. Do we want to include zeros? The specification says "gaps" meaning consecutive hyphens, so zeros should not be included. The original snippet outputs `j-i-1` unconditionally, which would output 0 for adjacent letters. But to match the task description ("representing the number of consecutive hyphens"), we should skip zero-length gaps. In the solution, we will check if `gap > 0` before pushing. Time complexity is O(n) because each character is visited once by the outer and inner pointers combined. Space complexity O(1) auxiliary (excluding the output vector).

/*
Write a C++ function named `findValidSplit` that takes a non-empty string `s` consisting only of decimal digits and representing a positive integer (so `s[0]` is never `'0'`), and returns a string describing a valid split of `s` into two positive integers `a` and `b` such that both of the following conditions hold: (1) the split position is between characters `i` and `i+1` for some `0 <= i < s.length()-1`, and the left part is `s[0..i]` (which must have no leading zeros, but since `s[0]` is non-zero this is automatically satisfied) and the right part is `s[i+1..end]` (which must NOT start with `'0'` because `b` must be a positive integer), and (2) `a < b` when both are interpreted as integers. If multiple valid splits exist, return the one with the smallest possible `i` (i.e., the leftmost split that works). If no valid split exists, return the string `"-1"`. The returned string must be in the format `"a b"` where `a` and `b` are the decimal representations (without leading zeros) of the two integers. The input string will have length between 2 and 15, inclusive, so integer conversion using `stoi` is safe. You must implement the function with `const` correctness for the input parameter.
*/

#include <string>
#include <cstddef>

// Find the leftmost split of digit string s into two positive integers a < b,
// where the right part must not have a leading zero. Returns "a b" or "-1".
std::string findValidSplit(const std::string& s) {
    const std::size_t n = s.size();
    for (std::size_t i = 0; i + 1 < n; ++i) {
        // Right part must not start with '0' to be a positive integer.
        if (s[i + 1] == '0') {
            continue;
        }
        const std::string left = s.substr(0, i + 1);
        const std::string right = s.substr(i + 1);
        const int a = std::stoi(left);
        const int b = std::stoi(right);
        if (a < b) {
            return left + " " + right;
        }
    }
    return "-1";
}

#include <cassert>
#include <string>

// The solution function is declared above. Here we test it.
int main() {
    // Basic valid split: 12 and 34 -> 12 < 34.
    assert(findValidSplit("1234") == "12 34");
    // Leftmost valid split: 1 < 23? Yes, so split at i=0.
    assert(findValidSplit("123") == "1 23");
    // Right part starts with zero, skip; then 10 < 1? No, then 101 < 1? No -> -1.
    assert(findValidSplit("101") == "-1");
    // Split at i=1 because right part "34" is non-zero and 23 < 34.
    assert(findValidSplit("2334") == "23 34");
    // No split works because all right parts are too small.
    assert(findValidSplit("999") == "-1");
    // Single split at i=0: 1 < 5.
    assert(findValidSplit("15") == "1 5");
    // Right part starts with 0, skipped. Then 10 < 01? No, stoi("01")=1, 10<1 false.
    assert(findValidSplit("1001") == "-1");
    // 12 < 345? Yes, valid.
    assert(findValidSplit("12345") == "1 2345");
    // Right part "000" has leading zero, skip; then "1000" and "0" invalid? Actually "1000" and "0", but right starts with '0' skip; so -1.
    assert(findValidSplit("10000") == "-1");
    // 5 < 678? Yes.
    assert(findValidSplit("5678") == "5 678");
    return 0;
}

// The solution iterates over all possible split positions `i` from `0` to `n-2`. For each split, the right substring `s.substr(i+1)` must not start with `'0'`; if it does, we skip that split because `b` would have a leading zero, making it invalid as a positive integer (even though `stoi` would ignore leading zeros, the problem explicitly disallows them). If the right part is valid, we compute `a = stoi(s.substr(0, i+1))` and `b = stoi(s.substr(i+1))`. If `a < b`, we immediately return the formatted string `a + " " + b` because we are iterating from left to right and thus the first valid split is the one with the smallest `i`. If the loop completes without finding any valid split, we return `"-1"`. Since the input length is at most 15, the time complexity is `O(n^2)` in the worst case due to string copies and `stoi` calls, but effectively `O(n)` with small constants. Space complexity is `O(n)` for the substring copies, which is trivial for the given constraints. Edge cases include when the second part starts with zero (skip), when `a` equals `b` (not valid), and when no split works (return `"-1"`).

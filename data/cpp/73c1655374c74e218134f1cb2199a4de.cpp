Write a C++ function `minSwapsToBalance` that takes a string `s` consisting only of the characters `'['` and `']'` (with the number of open and close brackets always equal, and the string length is even). The function must return the minimum number of adjacent swaps (swapping any two adjacent characters) required to make the bracket sequence balanced (i.e., every prefix of the string has at least as many `'['` as `']'`, and the total counts are equal). For example, for `"][]["` the answer is `2` (swap positions 0 and 1 to get `"[]]["`, then swap positions 2 and 3 to get `"[][]"`), while for `"]]][[["` the answer is `3`. The input string may already be balanced, in which case the answer is `0`. The function must be efficient and handle strings up to length 100,000.

#include <cassert>

int main() {
    assert(minSwapsToBalance("[]") == 0);
    assert(minSwapsToBalance("[[]]") == 0);
    assert(minSwapsToBalance("][") == 1);
    assert(minSwapsToBalance("][][") == 1);
    assert(minSwapsToBalance("]][[") == 1);
    assert(minSwapsToBalance("]]][[[") == 3);
    assert(minSwapsToBalance("][][][") == 2);
    assert(minSwapsToBalance("[][]") == 0);
    assert(minSwapsToBalance("][]]][[[") == 2);  // length 8, equal counts
    assert(minSwapsToBalance("[[][]]") == 0);
    return 0;
}

#include <string>

// Return the minimum number of adjacent swaps needed to balance a bracket sequence.
// The string must contain only '[' and ']' with equal counts.
int minSwapsToBalance(const std::string& s) {
    int unmatchedOpen = 0;  // Number of '[' that could not be matched so far

    for (char c : s) {
        if (c == '[') {
            ++unmatchedOpen;
        } else {  // c == ']'
            if (unmatchedOpen > 0) {
                --unmatchedOpen;  // Match with a previous '['
            }
            // If unmatchedOpen == 0, this ']' is unmatched; keep as is.
        }
    }

    // Each swap fixes one unmatched '[' and one unmatched ']'.
    // unmatchedOpen equals the number of unmatched '[' (and also unmatched ']').
    return (unmatchedOpen + 1) / 2;
}

// The key insight is that in any valid bracket sequence, at any point the number of `']'` cannot exceed the number of `'['` in a prefix. We can simulate scanning the string while tracking a counter `balance` that increments on `'['` and decrements on `']'`, but we never let it go negative—if we encounter a `']'` when `balance` is zero, we treat it as an unmatched closing bracket that needs fixing. In the given solution, we track `x` as the number of unmatched opening brackets after attempting to match as many as possible. Starting with `x = 0`, for each `'['` we increment `x` (an unmatched open), and for each `']'` if `x > 0` we decrement `x` (matching it with a previous open), else we leave `x` unchanged (the close is unmatched). After the scan, `x` equals the number of unmatched `'['` that could not be matched to any subsequent `']'` (because we only match when possible). Since the total counts are equal, the number of unmatched `'['` equals the number of unmatched `']'` that appeared when `x` was zero. These unmatched pairs need to be swapped. Each swap fixes two unmatched brackets (one at the left, one at the right), so the minimum number of adjacent swaps is `ceil(x / 2)`, i.e., `(x + 1) / 2`. Edge cases: if the string is already balanced, `x = 0` and answer is `0`. If all opens come first, `x` equals total count, and answer is half. Time complexity is `O(n)` for a single pass; space is `O(1)`.

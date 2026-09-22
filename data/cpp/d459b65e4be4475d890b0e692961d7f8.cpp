// Write a C++ function that takes a string consisting only of characters `'0'` and `'1'` (a binary string) and returns the number of occurrences of the pattern `"101"` where the `0` can be repeated any number of times (i.e., the pattern `1` followed by one or more `0`s followed by `1`). Count each maximal run of zeros between two ones as a single occurrence. For example, in `"1001"` there is one occurrence, in `"10001"` still one, and in `"101001"` there are two (the first `1`, zeros, then `1`; and the second `1`, zeros, then last `1`). The input will be non-empty and contain only `'0'` and `'1'`. Return the integer count.

#include <cassert>

int main() {
    assert(countBinaryPattern("1") == 0);
    assert(countBinaryPattern("0") == 0);
    assert(countBinaryPattern("10") == 0);
    assert(countBinaryPattern("01") == 0);
    assert(countBinaryPattern("101") == 1);
    assert(countBinaryPattern("1001") == 1);
    assert(countBinaryPattern("10001") == 1);
    assert(countBinaryPattern("101001") == 2);
    assert(countBinaryPattern("10101") == 2);
    assert(countBinaryPattern("0100001110111") == 2);
    return 0;
}

#include <string>

// Counts occurrences of pattern "1 0+ 1" in a binary string.
// Each maximal run of one or more zeros between two ones counts once.
int countBinaryPattern(const std::string& str) {
    if (str.empty()) return 0;
    char last = str[0];
    int i = 1;
    int counter = 0;

    while (i < static_cast<int>(str.size())) {
        if (str[i] == '0' && last == '1') {
            // Skip all consecutive zeros
            while (i < static_cast<int>(str.size()) && str[i] == '0') {
                ++i;
            }
            // If we stopped at a '1', we found a valid pattern
            if (i < static_cast<int>(str.size()) && str[i] == '1') {
                ++counter;
            }
            // Note: we don't set last here; the next iteration will handle the current character
            // But to avoid double counting, we update last at the loop end normally.
            // However, after skipping zeros, we need to continue from index i without reprocessing.
            // We'll set last to str[i-1] if i>0, but easiest: continue loop with last updated at end.
            if (i < static_cast<int>(str.size())) {
                last = str[i];
                ++i;
                continue;
            } else {
                break;
            }
        }
        last = str[i];
        ++i;
    }
    return counter;
}

// The algorithm scans the string once. We keep track of the last seen character. When we encounter a `'0'` and the previous character was `'1'`, we enter a zero‑run: we advance the index past all consecutive zeros. Then, if the character after the zero‑run is `'1'`, we increment the counter. This correctly handles overlapping patterns such as `"10101"` (counts 2: positions 0‑2 and 2‑4) because after counting the first occurrence, we set `last` to the `'1'` at position 2 and continue. Edge cases include strings with no `'0'` (returns 0), strings like `"10"` (returns 0 because no trailing `'1'`), and strings with multiple zeros between ones (counts once per maximal run). The traversal uses a single while loop with an inner skip over zeros, making the total time `O(n)` and auxiliary space `O(1)`.

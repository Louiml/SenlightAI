Write a C++ function `totalLetterShift(const std::string& a, const std::string& b)` that takes two strings of equal length containing only lowercase English letters and returns an integer equal to the sum of the minimal number of circular letter shifts needed to transform each character of `a` into the corresponding character of `b` (i.e., for each position `i`, compute `(b[i] - a[i] + 26) % 26` and sum these values over all positions). The function must work for any non-empty equal‑length strings (you may assume the input guarantee), and it should not modify the input strings. In the same file, provide a separate `main` function that reads an integer `n`, then `n` pairs of strings (each pair on one line) and prints for each pair the total shift computed by the function. Do not use any global state.
// The problem is a direct translation of the snippet: for each position, the minimal forward shift from character `a[i]` to `b[i]` on the circular alphabet of 26 letters is `(b[i] - a[i] + 26) % 26` (the `+26` ensures a non-negative result when `b[i]` is smaller than `a[i]`). Sum these over all indices. Since the strings are guaranteed equal-length and lowercase, no bounds checking is needed. The edge case of `a[i] == b[i]` yields a shift of 0. The algorithm runs in `O(L)` time per pair (where `L` is the string length) and uses `O(1)` extra space. For `n` pairs, total time is `O(n * L)`.
#include <string>

// Returns the total minimal circular shifts to turn string a into string b.
// Both strings must have equal length and contain lowercase letters.
int totalLetterShift(const std::string& a, const std::string& b) {
    int total = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        total += (b[i] - a[i] + 26) % 26;
    }
    return total;
}
#include <cassert>

int main() {
    // Basic equal letters
    assert(totalLetterShift("abc", "abc") == 0);
    // One-position forward shifts
    assert(totalLetterShift("abc", "bcd") == 3);
    // Circular wrap: z to a is 1 shift
    assert(totalLetterShift("z", "a") == 1);
    // Mixed example
    assert(totalLetterShift("az", "za") == (1 + 25));
    // Longer string with mixed shifts
    assert(totalLetterShift("hello", "world") == 49);
    // All shifts of 13
    assert(totalLetterShift("aaaa", "nnnn") == 52);
    // Single character
    assert(totalLetterShift("m", "m") == 0);
    // Reverse direction is always forward minimal
    assert(totalLetterShift("a", "z") == 25);
    // Repeated test
    assert(totalLetterShift("abcabc", "cbaabc") == 12);
    return 0;
}

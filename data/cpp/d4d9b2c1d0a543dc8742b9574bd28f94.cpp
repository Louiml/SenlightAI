// Write a C++ function named `countEqualPairs` that takes a non-empty string `s` and returns an integer equal to the number of pairs of indices `(i, j)` with `0 <= i <= j < s.length()` such that the characters at positions `i` and `j` are identical. This count includes pairs where both indices point to the same character (i.e., `i == j`). The function must be efficient, handle all printable ASCII characters (including digits, letters, spaces, and symbols), and work correctly for any string length from 1 up to a practical limit (e.g., 10^5). The function should be declared as a free function with `const` correctness, returning a `long long` to avoid overflow when the string is large (since the maximum count is `n*(n+1)/2` which exceeds 32-bit integer range for `n` above ~65536).
// The straightforward brute‑force approach is to nest two loops over `i` and `j` and increment a counter whenever `s[i] == s[j]`. This works but runs in O(n²) time, which is too slow for large inputs (e.g., 10^5 characters). Instead, observe that the total number of valid pairs is equal to the sum over each distinct character of `c * (c + 1) / 2`, where `c` is the frequency of that character in the string. The reason: for a given character that appears `c` times, every pair of two occurrences (including the same occurrence) yields a valid pair. The number of unordered pairs with repetition from `c` items is `c*(c+1)/2`. Summing this over all characters gives the answer. We can count character frequencies using an array of size 256 (for extended ASCII) or 128 (for basic ASCII). Edge cases: empty string? The task says non-empty, but the solution must handle it gracefully if the function is called with an empty string (return 0). For a string of length 1, the answer is 1 (i==j). Time complexity is O(n) for a single pass to count frequencies, plus O(256) for summing, so O(n + 1) = O(n). Space complexity is O(1) because the frequency array has fixed size.
#include <string>
#include <vector>

// Count the number of index pairs (i, j) with i <= j where s[i] == s[j].
// Uses character frequency counting: for each character with frequency f,
// it contributes f*(f+1)/2 pairs. O(n) time, O(1) auxiliary space.
long long countEqualPairs(const std::string& s) {
    if (s.empty()) return 0;
    std::vector<long long> freq(128, 0); // covers all ASCII characters
    for (char ch : s) {
        ++freq[static_cast<unsigned char>(ch)];
    }
    long long result = 0;
    for (long long f : freq) {
        if (f > 0) {
            result += f * (f + 1) / 2;
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Function under test is declared above; include its definition before main.
long long countEqualPairs(const std::string& s);

int main() {
    // Single character: only the pair (0,0)
    assert(countEqualPairs("a") == 1);

    // Two identical characters: pairs (0,0), (1,1), (0,1) => 3
    assert(countEqualPairs("bb") == 3);

    // Two different characters: each contributes 1 pair => 2
    assert(countEqualPairs("ab") == 2);

    // Longer string with repeated characters:
    // "aaa" -> each 'a' freq=3 => 3*4/2=6
    assert(countEqualPairs("aaa") == 6);

    // Mixed case and digits:
    // "a1a1" -> 'a' freq=2 => 3, '1' freq=2 => 3, total=6
    assert(countEqualPairs("a1a1") == 6);

    // Non-ASCII? We only handle ASCII, but spaces are allowed:
    // "  " (two spaces) -> freq=2 => 3
    assert(countEqualPairs("  ") == 3);

    // Empty string (edge case, not typical but safe)
    assert(countEqualPairs("") == 0);

    // Large string test: "abc" repeated 10000 times -> each char freq=10000
    // each contributes 10000*10001/2 = 50,005,000; total = 150,015,000
    std::string big;
    for (int i = 0; i < 10000; ++i) big += "abc";
    assert(countEqualPairs(big) == 150015000LL);

    // All same character with length 10: 10*11/2 = 55
    assert(countEqualPairs("hhhhhhhhhh") == 55);

    return 0;
}

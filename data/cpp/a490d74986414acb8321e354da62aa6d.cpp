// Write a standalone C++ function named `isEvenHalfRepeated` that takes an integer `n` and a string `S` as input. The function must return `true` if the string `S` can be split into two equal halves such that the first half is exactly identical to the second half (i.e., the string has even length and the first `n/2` characters match the next `n/2` characters). Otherwise, it returns `false`. The function should not read from standard input or print to output; it should purely check the condition and return a boolean. Assume the string contains only printable ASCII characters, and `n` is the length of `S` (so the caller will pass consistent values, but the function should handle mismatches gracefully by returning `false` if `n` is negative, odd, or not equal to `S.length()`).
// The core algorithm is simple: first verify that the string length is even and matches the given `n`. If not, immediately return `false`. Then, compute the midpoint `half = n / 2`. Iterate from index `0` to `half - 1` and compare `S[i]` with `S[half + i]`. If any pair differs, the string is not a repeated half, so return `false`. If all pairs match, return `true`. Edge cases include: an empty string (length 0, which is even, but the halves are empty, so it should return `true`), odd-length strings (always `false`), and the case where `n` does not match the actual string length (should return `false` to avoid accessing out-of-bounds). Time complexity is O(n) because we scan at most half the string, and space complexity is O(1) extra space.
#include <string>

// Returns true if the string S consists of two identical halves.
// n is expected to be the length of S. The function checks consistency.
bool isEvenHalfRepeated(int n, const std::string& S) {
    // Edge cases: negative n, mismatched length, or odd length.
    if (n < 0 || static_cast<size_t>(n) != S.length() || n % 2 != 0) {
        return false;
    }
    
    // Empty string: two empty halves are identical.
    if (n == 0) {
        return true;
    }
    
    int half = n / 2;
    for (int i = 0; i < half; ++i) {
        if (S[i] != S[half + i]) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <string>

// The solution function is declared here for completeness.
bool isEvenHalfRepeated(int n, const std::string& S);

int main() {
    // Basic even-length repeated halves
    assert(isEvenHalfRepeated(4, "abab") == true);
    assert(isEvenHalfRepeated(4, "abba") == false);
    
    // Odd length always false
    assert(isEvenHalfRepeated(3, "abc") == false);
    assert(isEvenHalfRepeated(5, "abcab") == false);
    
    // Empty string: even (0) and halves match
    assert(isEvenHalfRepeated(0, "") == true);
    
    // Mismatched n vs actual length
    assert(isEvenHalfRepeated(4, "abc") == false);
    assert(isEvenHalfRepeated(3, "abcd") == false);
    
    // Negative n
    assert(isEvenHalfRepeated(-2, "ab") == false);
    
    // Longer strings with repeated halves
    assert(isEvenHalfRepeated(8, "hellohello") == true);
    assert(isEvenHalfRepeated(8, "helloworld") == false);
    
    // Single character even length? Length 1 is odd, so false
    assert(isEvenHalfRepeated(1, "a") == false);
    
    // Repeated halves with spaces and special characters
    assert(isEvenHalfRepeated(6, "a a a a") == false); // length 6, halves "a a " vs "a a "? Actually string has 6 chars: 'a',' ','a',' ','a',' ' -> halves "a a" and " a " which differ -> false
    assert(isEvenHalfRepeated(6, "a a a a") == false); // still false, redundant check
    assert(isEvenHalfRepeated(4, "a!a!") == true);
    
    // Large even length with repeated pattern
    std::string big = "";
    for (int i = 0; i < 1000; ++i) big += "xy";
    assert(isEvenHalfRepeated(2000, big) == true);
    
    return 0;
}

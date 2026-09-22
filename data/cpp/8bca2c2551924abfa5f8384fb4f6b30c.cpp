// Write a standalone C++ function named `computeHammingDistance` that takes two `std::string` parameters and an optional third parameter `threshold` (with a default value of `-1` to indicate no threshold). The function must compute the Hamming distance between the two strings (the number of positions where the characters differ). If the strings have different lengths, return `-1`. If a positive `threshold` is provided and the running distance reaches or exceeds that threshold during the scan, return `-1` immediately (early termination). Otherwise, return the total distance. The function should be declared with appropriate `const` correctness (parameters as `const std::string&`, function itself `const`-qualified if a member, but here as a free function). You must not use any loops that count mismatches after early termination for threshold mode. The function must handle empty strings (distance 0 if both empty and same length) and handle large inputs efficiently.
// The core algorithm is a single pass over both strings, comparing character by character. For the no-threshold version, we iterate from index 0 to `str1.length()-1` (since lengths are equal after checking), incrementing a counter each time `str1[i] != str2[i]`. For the threshold version, we do the same but after each increment, check if the counter equals the threshold; if so, return `-1` immediately, avoiding unnecessary further comparisons. Key edge cases: (1) different lengths – return `-1` before any comparison; (2) empty strings – if both empty, lengths equal, loop body never executes, return 0; (3) threshold `<= 0` – treat as no threshold (or handle per spec; here we interpret only positive thresholds as active); (4) threshold larger than the actual distance – return the actual distance. Time complexity is \(O(n)\) where \(n\) is the string length, but with early termination, the average time can be less. Space complexity is \(O(1)\) auxiliary.
#include <string>

// Compute Hamming distance between two equal-length strings.
// If lengths differ, return -1.
// If threshold > 0 and distance reaches threshold, return -1 early.
int computeHammingDistance(const std::string& str1, const std::string& str2, int threshold = -1) {
    if (str1.length() != str2.length()) {
        return -1;
    }
    
    int distance = 0;
    const size_t n = str1.length();
    for (size_t i = 0; i < n; ++i) {
        if (str1[i] != str2[i]) {
            ++distance;
            if (threshold > 0 && distance >= threshold) {
                return -1;
            }
        }
    }
    return distance;
}
#include <cassert>

int main() {
    // Basic cases
    assert(computeHammingDistance("abc", "abc") == 0);
    assert(computeHammingDistance("abc", "abd") == 1);
    assert(computeHammingDistance("abc", "xyz") == 3);
    
    // Different lengths
    assert(computeHammingDistance("abc", "ab") == -1);
    assert(computeHammingDistance("", "a") == -1);
    
    // Empty strings (both empty)
    assert(computeHammingDistance("", "") == 0);
    
    // Threshold early termination
    assert(computeHammingDistance("abc", "xyz", 2) == -1); // distance would be 3, but reaches threshold 2
    assert(computeHammingDistance("abc", "abd", 2) == 1); // distance 1, less than threshold
    assert(computeHammingDistance("abc", "abc", 1) == 0); // distance 0, less than threshold
    
    // Threshold with zero or negative (treated as no threshold)
    assert(computeHammingDistance("abc", "xyz", 0) == 3);
    assert(computeHammingDistance("abc", "xyz", -5) == 3);
    
    // Longer strings with mixed matches
    assert(computeHammingDistance("hello world", "hallo worle") == 2);
    assert(computeHammingDistance("aaaa", "bbbb", 3) == -1); // distance 4, early stop at 3
    
    return 0;
}

// Given a binary string `s` consisting only of characters `'0'` and `'1'`, write a C++ function `int minFlipsToSort(const std::string& s)` that returns the minimum number of character flips (changing `'0'` to `'1'` or `'1'` to `'0'`) required to make the string non-decreasing (i.e., all `'0'`s appear before all `'1'`s, so the resulting string is of the form `0...01...1`). The function must handle strings of any length, including empty or single-character strings, and must operate efficiently without modifying the original string.
// The goal is to transform the input binary string into a sorted (non-decreasing) form using the fewest flips. A sorted binary string has a single change point: some prefix consists of `'0'`s and the remaining suffix consists of `'1'`s. For each possible split position `i` (where `i` ranges from 0 to `n`), we consider making the prefix `s[0..i-1]` all `'0'`s and the suffix `s[i..n-1]` all `'1'`s. The number of flips needed for a given split `i` equals the count of `'1'`s in the prefix (those must become `'0'`) plus the count of `'0'`s in the suffix (those must become `'1'`). To compute this efficiently for all splits, precompute prefix counts of `'1'`s and suffix counts of `'0'`s (or use running counts). Then iterate over all `i` from 0 to `n` and take the minimum. Edge cases: empty string (answer 0), all `'0'`s (answer 0, since already sorted), all `'1'`s (answer 0), and string with alternating bits (must flip at least half). Complexity: O(n) time and O(n) auxiliary space (or O(1) if using a single prefix array and a running suffix count). This approach is optimal because any sorted binary string must have exactly one boundary, and we check all possible boundaries.
#include <string>
#include <algorithm>
#include <vector>

// Returns the minimum number of flips needed to make the binary string non-decreasing.
// A non-decreasing binary string has all '0's before all '1's.
int minFlipsToSort(const std::string& s) {
    int n = static_cast<int>(s.size());
    
    // prefixOnes[i] = number of '1's in s[0..i-1]
    std::vector<int> prefixOnes(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        prefixOnes[i] = prefixOnes[i - 1] + (s[i - 1] == '1' ? 1 : 0);
    }
    
    // totalZeros = total number of '0's in the whole string
    int totalZeros = n - prefixOnes[n];
    
    int best = n;  // worst case: flip all characters
    
    // Try every split point: prefix of length i becomes all '0', suffix becomes all '1'
    // For split i: flips = (# ones in prefix) + (# zeros in suffix)
    // # zeros in suffix = totalZeros - (# zeros in prefix) = totalZeros - (i - prefixOnes[i])
    for (int i = 0; i <= n; ++i) {
        int onesInPrefix = prefixOnes[i];
        int zerosInPrefix = i - onesInPrefix;
        int zerosInSuffix = totalZeros - zerosInPrefix;
        int flips = onesInPrefix + zerosInSuffix;
        best = std::min(best, flips);
    }
    
    return best;
}
#include <cassert>
#include <string>

// Declaration of the solution function (assumed defined elsewhere or above)
int minFlipsToSort(const std::string& s);

int main() {
    // Test basic cases
    assert(minFlipsToSort("") == 0);
    assert(minFlipsToSort("0") == 0);
    assert(minFlipsToSort("1") == 0);
    
    // Already sorted strings
    assert(minFlipsToSort("000") == 0);
    assert(minFlipsToSort("111") == 0);
    assert(minFlipsToSort("00111") == 0);
    
    // Single flip needed
    assert(minFlipsToSort("10") == 1);      // flip '1' to '0' or '0' to '1'
    assert(minFlipsToSort("100") == 1);     // flip first '1' to '0'
    assert(minFlipsToSort("0010") == 1);    // flip last '0' to '1'
    
    // Multiple flips
    assert(minFlipsToSort("1010") == 2);    // e.g., flip positions 1 and 3 -> 0101? Actually better: flip second to '0' and last to '0'? Let's check: 0000? That's 2 flips. Or 0101? That's 2 flips. Minimum is 2.
    assert(minFlipsToSort("1100") == 2);    // flip first two '1's to '0' or last two '0's to '1'
    assert(minFlipsToSort("01010") == 2);   // check: flip positions 2 and 4 (0-indexed) to get 00111? Actually string 0 1 0 1 0 -> flip s[1] to '0' and s[3] to '0' gives 00000 (2 flips), or flip s[2] and s[4] to '1' gives 01111 (2 flips). Minimum = 2.
    
    // Large string with all alternating bits
    std::string alt = "1010101010"; // length 10
    assert(minFlipsToSort(alt) == 5); // flip either all position of '1' or all '0' to get sorted
    
    // All flips needed (e.g., string "01" is already sorted, but "10" is 1, "101" is 1? Let's test "101": flip middle '0' to '1' gives "111" (1 flip) or flip both ends to '0' gives "000" (2 flips). So min = 1.
    assert(minFlipsToSort("101") == 1);
    assert(minFlipsToSort("010") == 1); // flip middle '1' to '0' gives "000"
    
    return 0;
}

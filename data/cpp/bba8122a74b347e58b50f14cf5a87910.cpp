Write a C++ function `minFlipsToMonotoneIncreasing(const std::string& s)` that takes a binary string `s` (containing only characters `'0'` and `'1'`) and returns the minimum number of flips (changing a `'0'` to `'1'` or vice versa) needed to make the string monotonically increasing. A monotonically increasing binary string is one where all `'0'`s appear before all `'1'`s (i.e., there is no occurrence of `'1'` followed later by a `'0'`). The input string length is between 1 and 100,000. The function must be efficient enough to handle the upper bound, and should not modify the input string—operate on a copy if needed. Return the minimal flip count as an integer.

The problem is a classic dynamic programming / greedy scan. We aim to split the string at some index `i` such that all characters from `0` to `i` (inclusive) are flipped to `'0'` (if not already) and all characters from `i+1` to the end are flipped to `'1'` (if not already). For each possible split point, the total flips = number of `'1'`s in the left part (need to flip to `'0'`) + number of `'0'`s in the right part (need to flip to `'1'`). The answer is the minimum over all split points, including splitting before the first character (all flips to `'1'`) and after the last (all flips to `'0'`).

We can compute this in one pass using a running count. First, count total zeros in the string — call this `totalZeros`. As we iterate left to right, maintain:
- `onesLeft`: number of `'1'`s seen so far (left part).
- `zerosRemaining`: number of `'0'`s not yet seen (right part). Initialize to `totalZeros`.
At each index `i` (0 to n-1), we consider a split after index `i` (i.e., left part = indices 0..i, right part = i+1..n-1). For split before first character (i = -1) we special-case as all right: flips = totalZeros. For split after last (i = n-1) we have flips = onesLeft (after processing all). For each valid split, flips = onesLeft + zerosRemaining (where zerosRemaining is count of zeros from i+1 onward). We update `onesLeft` if current char is `'1'`, and decrement `zerosRemaining` if current char is `'0'` before computing the split for the next position.

Edge cases: empty string? Problem says length at least 1. String already monotone increasing yields answer 0. String all zeros or all ones yields 0 as well. Complexity: O(n) time, O(1) auxiliary space (excluding input copy if we avoid mutating). We must not mutate the input string, so we either read directly or copy.

#include <string>
#include <algorithm>

// Returns the minimum number of bit flips to make the binary string monotonically increasing.
// A monotonically increasing binary string has all '0's before all '1's.
int minFlipsToMonotoneIncreasing(const std::string& s) {
    int n = static_cast<int>(s.size());
    int totalZeros = 0;
    for (char c : s) {
        if (c == '0') ++totalZeros;
    }

    int bestFlips = totalZeros; // Split before first char => flip all zeros to ones
    int onesLeft = 0;
    int zerosRemaining = totalZeros;

    for (int i = 0; i < n; ++i) {
        if (s[i] == '0') {
            --zerosRemaining; // This zero is now in the left part
        } else {
            ++onesLeft;       // This one stays in the left part (needs flip to 0)
        }
        // Split after index i: left part = [0..i], right part = [i+1..n-1]
        int currentFlips = onesLeft + zerosRemaining;
        bestFlips = std::min(bestFlips, currentFlips);
    }
    // After loop, zerosRemaining is 0 and onesLeft = total ones, so currentFlips = totalOnes.
    // But we already considered split after last char because the loop's last iteration
    // uses i = n-1 and zerosRemaining = 0, onesLeft = all ones, so it's covered.
    return bestFlips;
}

#include <cassert>
#include <string>

// Assume the solution function is defined above.
int main() {
    // Basic cases
    assert(minFlipsToMonotoneIncreasing("00110") == 1); // Flip last '0' to '1' -> "00111"
    assert(minFlipsToMonotoneIncreasing("010110") == 2); // Flip position 1 '1' to '0' and position 5 '0' to '1' -> "000111"
    assert(minFlipsToMonotoneIncreasing("00011000") == 3); // e.g., flip last three zeros to ones -> "00011111"
    
    // Already monotone increasing
    assert(minFlipsToMonotoneIncreasing("000111") == 0);
    assert(minFlipsToMonotoneIncreasing("00000") == 0);
    assert(minFlipsToMonotoneIncreasing("11111") == 0);
    
    // All zeros then zeros? Already covered.
    // Single character
    assert(minFlipsToMonotoneIncreasing("0") == 0);
    assert(minFlipsToMonotoneIncreasing("1") == 0);
    
    // Mixed large case
    assert(minFlipsToMonotoneIncreasing("101010") == 3); // e.g., flip first three '1's to '0'? Actually minimize: split at index 3 -> left "1010" has three ones, right "" has zero => 3, or split before first: total zeros=3 => 3.
    
    // Specific known case from original snippet
    assert(minFlipsToMonotoneIncreasing("010") == 1); // Flip middle '1' to '0' -> "000" or flip first '0' to '1' -> "110"? Better: flip last '0' to '1' -> "011" also 1 flip, but min is 1.
    
    return 0;
}

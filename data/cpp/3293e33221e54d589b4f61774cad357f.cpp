/*
Write a C++ function named `maxSubsetSize` that takes a vector of binary strings (consisting only of '0' and '1' characters), an integer `zerosLimit` representing the maximum total number of zeros allowed in the chosen subset, and an integer `onesLimit` representing the maximum total number of ones allowed. The function must return the size (number of strings) of the largest subset of the input vector such that the total number of zeros across all selected strings does not exceed `zerosLimit` and the total number of ones across all selected strings does not exceed `onesLimit`. Each string may be selected at most once. The function should handle empty inputs and cases where no string fits within the limits by returning 0. Use a classic 2D dynamic programming approach where the state represents the remaining zeros and ones capacity, iterating over each string in reverse order to avoid reusing a string multiple times.
*/
#include <vector>
#include <string>
#include <algorithm>

// Returns the maximum number of binary strings that can be selected
// such that total zeros <= zerosLimit and total ones <= onesLimit.
int maxSubsetSize(const std::vector<std::string>& strs, int zerosLimit, int onesLimit) {
    // dp[z][o] = max strings selectable with at most z zeros and o ones.
    std::vector<std::vector<int>> dp(zerosLimit + 1, std::vector<int>(onesLimit + 1, 0));

    for (const std::string& s : strs) {
        int zeroCount = static_cast<int>(std::count(s.begin(), s.end(), '0'));
        int oneCount = static_cast<int>(s.size()) - zeroCount;

        // Iterate capacities in reverse to avoid using the current string multiple times.
        for (int z = zerosLimit; z >= zeroCount; --z) {
            for (int o = onesLimit; o >= oneCount; --o) {
                dp[z][o] = std::max(dp[z][o], 1 + dp[z - zeroCount][o - oneCount]);
            }
        }
    }

    return dp[zerosLimit][onesLimit];
}
#include <cassert>
#include <vector>
#include <string>

// Assume maxSubsetSize is declared above.

int main() {
    // Example from the prompt.
    std::vector<std::string> strs1 = {"10", "0001", "111001", "1", "0"};
    assert(maxSubsetSize(strs1, 5, 3) == 4);

    // Selecting all strings uses 5 zeros and 3 ones: actually count: "10"(1z,1o), "0001"(3z,1o), "111001"(2z,4o) -> too many ones, "1"(0z,1o), "0"(1z,0o) total 7z,7o > limits, so max is 4.
    assert(maxSubsetSize(strs1, 7, 7) == 5); // all 5 strings fit exactly.

    // Empty input.
    std::vector<std::string> empty;
    assert(maxSubsetSize(empty, 10, 10) == 0);

    // No string fits within tight limits.
    std::vector<std::string> strs2 = {"11", "111", "1111"};
    assert(maxSubsetSize(strs2, 1, 1) == 0);

    // Only one string fits.
    std::vector<std::string> strs3 = {"01", "10"};
    assert(maxSubsetSize(strs3, 1, 1) == 1);

    // All strings are single zeros, limit zeros to 2.
    std::vector<std::string> strs4 = {"0", "0", "0", "0"};
    assert(maxSubsetSize(strs4, 2, 0) == 2); // can select 2 zeros with 2 zero capacity.

    // Strings with no zeros.
    std::vector<std::string> strs5 = {"1", "11", "111"};
    assert(maxSubsetSize(strs5, 10, 5) == 3); // all fit.

    // Check that reverse DP correctly avoids reuse.
    std::vector<std::string> strs6 = {"00", "00"};
    assert(maxSubsetSize(strs6, 2, 0) == 1); // each string has 2 zeros, only one fits.

    // Combination with both limits.
    std::vector<std::string> strs7 = {"01", "10", "11"};
    assert(maxSubsetSize(strs7, 2, 2) == 2); // can pick "01" and "10" (2z,2o), or two others.

    return 0;
}
// The problem is a variation of the 0/1 knapsack problem with two independent capacity constraints (zeros and ones). For each binary string, compute its zero count and one count. Then use a 2D DP table `dp[z][o]` where `dp[z][o]` is the maximum number of strings we can select using at most `z` zeros and `o` ones, considering only the strings processed so far. Initialize all entries to 0. For each string, iterate `z` from `zerosLimit` down to the string's zero count, and for each such `z`, iterate `o` from `onesLimit` down to the string's one count. Update `dp[z][o] = max(dp[z][o], 1 + dp[z - zeroCount][o - oneCount])`. The reverse iteration ensures each string is only used once (0/1 property). The final answer is `dp[zerosLimit][onesLimit]`. Edge cases: (1) If the input vector is empty, the loop does nothing and we return 0. (2) If a string itself exceeds both limits individually, the inner loops won't execute for that string, so it won't be selected, which is correct. (3) If all strings are too large individually, the answer remains 0. Time complexity: O(L * zerosLimit * onesLimit) where L is the number of strings. Space complexity: O(zerosLimit * onesLimit). This is optimal in the typical sense for this knapsack variant.

Write a C++ function that, given a string composed of characters `'.'`, `'#'`, and `'?'` representing a row of damaged springs, and a vector of non-negative integers representing the sizes of contiguous groups of damaged springs, returns the number of distinct arrangements of `'#'` and `'.'` in the unknown positions (`'?'`) that exactly match the given group sizes. The known `'#'` must remain damaged, and known `'.'` must remain operational. Groups of damaged springs must be separated by at least one operational spring, and no extra damaged springs may exist outside the groups. The function must count all valid ways to replace each `'?'` with either `'#'` or `'.'` such that the final configuration satisfies the constraints. Example: for `"???.###"` and `[1,1,3]`, the answer is `1` (only `#.#.###`). For `".??..??...?##."` and `[1,1,3]`, the answer is `4`. The input string length may be up to 100, and the number of groups up to 20.

#include <cassert>
#include <string>
#include <vector>

int countArrangements(const std::string& row, const std::vector<int>& groups);

int main() {
    // Example from the problem statement.
    assert(countArrangements("???.###", {1,1,3}) == 1);
    assert(countArrangements(".??..??...?##.", {1,1,3}) == 4);

    // Simple tests.
    assert(countArrangements("###", {3}) == 1);
    assert(countArrangements("...", {}) == 1);
    assert(countArrangements("???", {}) == 1); // all dots
    assert(countArrangements("???", {1}) == 3); // #.., .#., ..#
    assert(countArrangements("???", {2}) == 2); // ##., .##
    assert(countArrangements("???", {1,1}) == 1); // #.#
    assert(countArrangements("????", {1,1}) == 3); // #.#., #..#, .#.#
    assert(countArrangements("##?", {2}) == 1); // only ##.
    assert(countArrangements("#?#", {3}) == 1); // ###
    assert(countArrangements("?###?", {3}) == 2); // ###., .###

    // Edge cases with zeros or empty groups.
    assert(countArrangements("", {}) == 1);
    assert(countArrangements("", {1}) == 0);
    assert(countArrangements("#", {0}) == 0); // zero group size not allowed with '#'.
    assert(countArrangements(".", {0}) == 1); // zero-size group means no damaged springs.

    // Longer test case.
    assert(countArrangements("?###????????", {3,2,1}) == 10);
}

#include <string>
#include <vector>
#include <cstring>
#include <functional>

/**
 * Counts the number of distinct arrangements of '#' and '.' in the given
 * row that exactly match the required contiguous damaged groups.
 *
 * @param row The spring row pattern with '.', '#', and '?' (unknown).
 * @param groups A vector of non-negative integers specifying the sizes of
 *               contiguous damaged groups in order.
 * @return The number of valid arrangements.
 */
int countArrangements(const std::string& row, const std::vector<int>& groups) {
    // Memoization table: dp[i][g][b] = number of ways from position i with
    // g groups already completed and current block length b (0 if in gap).
    // We use -1 to indicate uncomputed.
    const int n = (int)row.size();
    const int maxGroupLen = (groups.empty()) ? 0 : groups.back();
    const int maxPossibleBlock = n;
    std::vector<std::vector<std::vector<long long>>> memo(
        n + 1,
        std::vector<std::vector<long long>>(
            groups.size() + 1,
            std::vector<long long>(maxPossibleBlock + 1, -1)
        )
    );

    // Recursive lambda with memoization.
    std::function<long long(int, int, int)> dfs = [&](int pos, int groupIdx, int blockLen) -> long long {
        // Base case: reached the end of the row.
        if (pos == n) {
            // If we are in a block, we must close it now.
            if (blockLen > 0) {
                // The block must match the next required group, and no groups left after.
                return (groupIdx < (int)groups.size() && groups[groupIdx] == blockLen && groupIdx + 1 == (int)groups.size()) ? 1 : 0;
            } else {
                // In a gap, all groups must be completed.
                return (groupIdx == (int)groups.size()) ? 1 : 0;
            }
        }

        // Memoization lookup.
        long long& cached = memo[pos][groupIdx][blockLen];
        if (cached != -1) return cached;

        long long total = 0;
        char ch = row[pos];

        // Function to try placing a '#' (damaged).
        auto tryHash = [&]() {
            // If we are currently in a gap, we must start a new group.
            // But we must ensure the next group exists.
            if (blockLen == 0) {
                if (groupIdx >= (int)groups.size()) return; // no more groups allowed
                // Starting a new group, its required size must be at least 1.
                if (groups[groupIdx] == 0) return; // group size 0 is invalid in this context
                // Place a '#', block length becomes 1.
                if (groups[groupIdx] >= 1) {
                    total += dfs(pos + 1, groupIdx, 1);
                }
            } else {
                // Continue the current block.
                if (blockLen + 1 <= groups[groupIdx]) {
                    total += dfs(pos + 1, groupIdx, blockLen + 1);
                }
                // If blockLen + 1 > groups[groupIdx], it's invalid.
            }
        };

        // Function to try placing a '.' (operational).
        auto tryDot = [&]() {
            if (blockLen > 0) {
                // We need to close the current block.
                // The block must match the next required group.
                if (groupIdx >= (int)groups.size()) return; // no group to close
                if (groups[groupIdx] != blockLen) return; // wrong size
                // Close block, move to next group, reset block length.
                total += dfs(pos + 1, groupIdx + 1, 0);
            } else {
                // In a gap, just continue.
                total += dfs(pos + 1, groupIdx, 0);
            }
        };

        if (ch == '#') {
            tryHash();
        } else if (ch == '.') {
            tryDot();
        } else { // '?'
            tryHash();
            tryDot();
        }

        cached = total;
        return total;
    };

    return (int)dfs(0, 0, 0);
}

// The problem is a classic combinatorial count that can be solved via recursion with memoization (top-down dynamic programming) or an iterative DP. The key idea is to process the string from left to right and, at each position, decide whether to place a `'#'` (start or continue a group) or `'.'` (gap). We maintain a state consisting of: the current index in the string, the current index in the group list (how many groups have been completed so far), and the length of the current contiguous block of `'#'` that we are building (0 if we are currently in a gap). At each position:
// - If the character is `'#'`, we must place a damaged spring: increment the current block length, and if this block length exceeds the next required group size, the state is invalid.
// - If the character is `'.'`, we must place an operational spring: if we are in the middle of a block (i.e., current block length > 0), we must close that block: check that the block length exactly equals the next required group size, then move to the next group and reset block length to 0. If we are already in a gap, just continue.
// - If the character is `'?'`, we can try both choices (place `'#'` or place `'.'`), but only if they lead to valid states; we sum the results.
//
// At the end of the string, we need to close any unfinished block: if we are in a block, we check that its length matches the last remaining group, and that no groups are left over. If we are in a gap, we check that all groups have been used up. Memoization on the triple `(index, group_index, block_length)` avoids recomputation. Since the string length can be up to 100 and the number of groups up to 20, and block length can be at most 100, the state space is at most 100*20*100 = 200,000 states, each processed in O(1), so the time complexity is O(n * g * max_group_length) but bounded by O(n^2 * g) in the worst case, which is fine for the given limits. Space complexity is O(n * g * max_block_length) for the memo table, which is also acceptable. Edge cases include empty string (should be handled gracefully), zero required groups (if no groups, the only valid arrangement is all dots), and strings with leading/trailing dots.

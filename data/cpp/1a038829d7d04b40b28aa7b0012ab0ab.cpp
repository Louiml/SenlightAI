/*
Write a C++ function `std::string specialDivisibleNumber()` that finds and returns the longest positive integer (as a decimal string, no leading zeros) such that for every prefix of length `k` (where `k` ranges from 1 up to the total length), that prefix (interpreted as an integer) is divisible by `k`. The integer can be at most 100 digits long. If multiple longest numbers exist, return the one that is lexicographically smallest. Your function should compute this deterministically and return the result as a string.
*/
#include <string>
#include <functional>

// Returns the longest polydivisible number (prefix of length k divisible by k)
// as a decimal string, with lexicographically smallest tie-break.
std::string findLongestPolydivisible() {
    std::string best;
    size_t bestLen = 0;

    // Recursive DFS: current prefix string, current length, current prefix modulo length
    std::function<void(std::string&, int, int)> dfs = [&](std::string& cur, int len, int mod) {
        // Try appending each digit 0-9
        for (int d = 0; d <= 9; ++d) {
            char digit = char('0' + d);
            cur.push_back(digit);
            int newMod = (mod * 10 + d) % (len + 1);
            if (newMod == 0) {
                // This prefix is valid
                if (len + 1 > bestLen) {
                    bestLen = len + 1;
                    best = cur;
                } else if (len + 1 == bestLen && cur < best) {
                    best = cur; // lexicographically smaller
                }
                if (len + 1 < 100) {
                    dfs(cur, len + 1, newMod);
                }
            }
            cur.pop_back();
        }
    };

    // Start with leading digits 1..9 (no zero)
    for (int first = 1; first <= 9; ++first) {
        std::string start(1, char('0' + first));
        // First prefix length 1 mod 1 is 0
        dfs(start, 1, first % 1);
    }

    return best;
}
#include <cassert>
#include <string>

// Declare the function (since it's in a separate compilation unit in real usage)
std::string findLongestPolydivisible();

int main() {
    // The known longest polydivisible number (with no leading zeros) is:
    // 3608528850368400786036725
    // We'll just verify that the function returns a string of length >= 25 and
    // that the result is indeed valid according to the definition.
    std::string result = findLongestPolydivisible();
    // Result must be non-empty
    assert(!result.empty());
    // Check length is at least 25 (known maximum)
    assert(result.size() >= 25);
    // Verify the property for the returned string
    bool valid = true;
    long long mod = 0;
    for (size_t k = 1; k <= result.size(); ++k) {
        mod = (mod * 10 + (result[k-1] - '0')) % k;
        if (mod != 0) { valid = false; break; }
    }
    assert(valid);
    // Also check that no longer than 25 exists (we just check the return is exactly 25)
    assert(result.size() == 25);
    // The known lexicographically smallest longest is the one above
    assert(result == "3608528850368400786036725");
    return 0;
}
// We need to find the longest sequence of digits (no leading zero) where each prefix of length `k` is divisible by `k`. This is a classic "polydivisible number" search. The approach is depth-first search with backtracking. We build the number digit by digit. At each step, given the current prefix of length `s-1`, we try appending digits 0–9, forming a new prefix of length `s`. We then check divisibility: compute the integer value of the prefix modulo `s` (can be done incrementally: `new_mod = (old_mod * 10 + digit) % s`). If divisible, we recurse deeper. We track the longest length found so far, and for ties (same length), we keep the lexicographically smallest. The search tree is finite because no prefix can be longer than 100 digits (an arbitrary limit). We start with leading digits 1–9 (since no leading zero). The maximum possible length is small in practice (the longest known polydivisible number has 25 digits), but with backtracking it will terminate quickly. Edge cases: single-digit numbers trivially satisfy condition (digit divisible by 1). The time complexity is O(10^d) in worst case but much faster in practice due to pruning; space is O(d) for recursion depth. The solution uses a recursive helper that tracks current prefix modulo current length, and updates global best result.

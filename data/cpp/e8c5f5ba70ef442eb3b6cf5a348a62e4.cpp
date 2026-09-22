/*
Write a C++ function `int spinLockSteps(const std::string& ring, const std::string& key)` that simulates a circular dial with characters arranged around it (the `ring` string) and a sequence of characters to spell out (the `key` string). The dial starts at index 0. For each character in `key`, you must rotate the dial (in either direction, counting each position moved as one step) so that the character at the current dial index matches the target character, then press the button (which costs 1 step, always). Find and return the minimum total number of steps (rotations + presses) required to spell the entire `key`. The ring may contain duplicate characters, and the key may be empty. The solution must be efficient for ring lengths up to 100 and key lengths up to 100.
*/
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>

// Return the minimum total steps (rotations + button presses) to spell key on a circular ring.
int spinLockSteps(const std::string& ring, const std::string& key) {
    if (key.empty()) return 0;

    // Map each character to all its positions in the ring.
    std::unordered_map<char, std::vector<int>> positions;
    for (int i = 0; i < static_cast<int>(ring.size()); ++i) {
        positions[ring[i]].push_back(i);
    }

    const int ringLen = static_cast<int>(ring.size());

    // Memoization: memo[i][p] = min steps from key index i and dial position p.
    std::vector<std::vector<int>> memo(key.size(), std::vector<int>(ringLen, -1));

    // Recursive lambda with memoization.
    std::function<int(int, int)> dfs = [&](int keyIdx, int pos) -> int {
        if (keyIdx == static_cast<int>(key.size())) return 0;
        if (memo[keyIdx][pos] != -1) return memo[keyIdx][pos];

        int best = INT_MAX;
        for (int target : positions.at(key[keyIdx])) {
            // Circular distance between pos and target on the ring.
            int diff = std::abs(pos - target);
            int rotateSteps = std::min(diff, ringLen - diff);
            int total = rotateSteps + 1 + dfs(keyIdx + 1, target);
            best = std::min(best, total);
        }
        memo[keyIdx][pos] = best;
        return best;
    };

    return dfs(0, 0);
}
#include <cassert>

int main() {
    // Basic examples
    assert(spinLockSteps("godding", "gd") == 4);
    assert(spinLockSteps("godding", "godding") == 13);
    assert(spinLockSteps("abc", "abc") == 3);
    assert(spinLockSteps("abc", "cba") == 5);

    // Empty key
    assert(spinLockSteps("abc", "") == 0);

    // Single character ring
    assert(spinLockSteps("a", "aaaa") == 4);

    // Duplicate characters in ring
    assert(spinLockSteps("abab", "ab") == 3);
    assert(spinLockSteps("aaaa", "aa") == 2);

    // Worst-case length
    std::string ring(100, 'a');
    std::string key(100, 'a');
    assert(spinLockSteps(ring, key) == 100);

    // Mixed characters and non-trivial rotations
    assert(spinLockSteps("abcdef", "fedcba") == 6);
}
// This is a classic dynamic programming problem. We model the state as `(keyIndex, dialPosition)` where `keyIndex` is how many characters of `key` we have already processed, and `dialPosition` is the current index on the ring. The recurrence: from state `(i, p)`, to match `key[i]`, we can move the dial to any occurrence `j` of `key[i]` in the ring. The rotation cost is the minimum distance clockwise or counterclockwise between `p` and `j` on the circular ring of length `L`, which is `min(abs(p-j), L - abs(p-j))`. After rotating to `j`, we press the button (cost 1), then solve the subproblem `(i+1, j)`. The base case is when `i == key.size()`, cost is 0. We use memoization to avoid recomputation. Important edge cases: duplicate characters in ring – we must try all occurrences; empty key – return 0; ring length 1 – rotation distance is always 0. Time complexity: `O(K * R * occurrences)` where `K` is key length, `R` is ring length, and occurrences per character bounded by `R`, so worst-case `O(K * R^2)` which for limits 100 is fine. Space: `O(K * R)` for memo table.

Given an integer `n` followed by `n` non-negative integers, write a C++ function `bool canForm(int x)` that returns `true` if `x` can be expressed as a sum of any number of 3’s and 7’s (each used zero or more times), and `false` otherwise. The function must handle all values from 0 up to 100 inclusive. For full correctness, also provide a separate free function `bool allQueries(const std::vector<int>& queries)` that returns `true` if every query in the vector satisfies the condition, and `false` otherwise. The solution must be self-contained, not rely on global variables, and be reusable for arbitrary test inputs.
// The core observation is that the set of reachable sums using coins 3 and 7 can be computed via dynamic programming (or a simple boolean DP array). Define `dp[i]` = true if `i` can be formed. Initialize `dp[0] = true`. For each index `i` from 0 to 97 (since adding 3 or 7 only goes forward), if `dp[i]` is true, then set `dp[i+3] = true` and `dp[i+7] = true`. This works because the transitions only move forward and do not require revisiting earlier states. The maximum needed value is 100, but we can allocate an array of size 101 (or 104 to avoid boundary checks). Important edge cases: 0 is always reachable (using zero coins). Small values like 1, 2, 4, 5, 8, 11 are not reachable; 3, 6, 7, 9, 10, 12, etc. are reachable. Beyond a certain point (specifically for x ≥ 12), every number is reachable because 3 and 7 are coprime, but the DP handles all up to 100 directly. Time complexity is O(100) to precompute the DP, and each query is O(1). Space complexity is O(1) (a fixed-size bool array). For the `allQueries` function, loop through the vector and return false immediately if any query fails, otherwise true after the loop; that takes O(m) time where m is the number of queries.
#include <vector>
#include <cstddef>

// Returns true if x can be expressed as a sum of 3's and 7's (each >=0 times).
bool canForm(int x) {
    if (x < 0) return false;
    static const int MAX = 100;
    static bool dp[MAX + 1] = {false};
    static bool initialized = false;
    if (!initialized) {
        dp[0] = true;
        for (int i = 0; i <= MAX - 3; ++i) {
            if (dp[i]) {
                dp[i + 3] = true;
                if (i + 7 <= MAX) dp[i + 7] = true;
            }
        }
        // Note: loop up to MAX-3 covers i+7 only up to 100 automatically.
        initialized = true;
    }
    if (x > MAX) {
        // Beyond 100, every x >= 12 is representable, but for safety handle all.
        // For arbitrary large x: use classic coin problem (3 and 7 coprime).
        if (x >= 12) return true;
        // Small cases handled by dp below (x is up to 11 here).
    }
    return dp[x];
}

// Returns true if every query in the vector can be formed.
bool allQueries(const std::vector<int>& queries) {
    for (int q : queries) {
        if (!canForm(q)) return false;
    }
    return true;
}
#include <cassert>
#include <vector>

// Declare the functions from the solution above (omitted for brevity, but must be present in test file).
int main() {
    // Basic individual values
    assert(canForm(0) == true);
    assert(canForm(3) == true);
    assert(canForm(7) == true);
    assert(canForm(6) == true);      // 3+3
    assert(canForm(10) == true);     // 3+7
    assert(canForm(1) == false);
    assert(canForm(2) == false);
    assert(canForm(4) == false);
    assert(canForm(5) == false);
    assert(canForm(8) == false);
    assert(canForm(11) == false);
    assert(canForm(12) == true);     // 3+3+3+3 or 7+5? actually 3+3+3+3
    assert(canForm(13) == true);     // 3+3+7
    assert(canForm(14) == true);     // 7+7
    assert(canForm(100) == true);    // large reachable
    assert(canForm(101) == true);    // >=12 always true
    // Test the querying function
    std::vector<int> q1 = {0, 3, 7, 10, 12, 100};
    assert(allQueries(q1) == true);
    std::vector<int> q2 = {0, 5, 7, 11};
    assert(allQueries(q2) == false);
    std::vector<int> q3 = {1};
    assert(allQueries(q3) == false);
    std::vector<int> q4 = {};
    assert(allQueries(q4) == true);
}

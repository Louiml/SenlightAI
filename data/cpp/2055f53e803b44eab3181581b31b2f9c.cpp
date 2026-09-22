/*
Write a standalone C++ function `bool is_shuffle_of(const std::vector<int>& deck1, const std::vector<int>& deck2, const std::vector<int>& shuffle, std::vector<int>* assignment)` that determines whether `shuffle` is a valid interleaving (shuffle) of `deck1` and `deck2`. A valid interleaving preserves the relative order of elements within each deck. If valid, the function should return `true` and, if `assignment` is non-null, fill it with the same length as `shuffle`, where each entry is `0` if the corresponding element was taken from `deck1` and `1` if taken from `deck2`. If invalid, return `false` and leave `assignment` unchanged. The function must handle empty decks and duplicate values, and should explore all possible ways to resolve ties when both decks have an equal next element. The input vectors are passed by `const` reference, and the function must not modify them.
*/
#include <vector>
#include <string>
#include <cstddef>

// Determine if 'shuffle' is a valid interleaving of 'deck1' and 'deck2'.
// If valid and 'assignment' is non-null, fill it with 0 (from deck1) or 1 (from deck2).
bool is_shuffle_of(const std::vector<int>& deck1, const std::vector<int>& deck2,
                   const std::vector<int>& shuffle, std::vector<int>* assignment) {
    const std::size_t n1 = deck1.size();
    const std::size_t n2 = deck2.size();
    const std::size_t total = n1 + n2;

    if (shuffle.size() != total) {
        return false;
    }

    // dp[i][j] = true if we can complete the shuffle using deck1 from i and deck2 from j.
    // We use a 2D vector of ints (0=unknown, 1=true, -1=false) as memo.
    std::vector<std::vector<int>> memo(n1 + 1, std::vector<int>(n2 + 1, 0));

    // Recursive lambda with memoization.
    std::function<bool(std::size_t, std::size_t)> dp = [&](std::size_t i, std::size_t j) -> bool {
        std::size_t k = i + j;  // current position in shuffle
        if (k == total) {
            return (i == n1 && j == n2);
        }
        if (memo[i][j] != 0) {
            return memo[i][j] == 1;
        }

        bool possible = false;
        // Try taking from deck1 if possible.
        if (i < n1 && shuffle[k] == deck1[i]) {
            possible = dp(i + 1, j);
        }
        // Try taking from deck2 if possible (and not yet solved by deck1).
        if (!possible && j < n2 && shuffle[k] == deck2[j]) {
            possible = dp(i, j + 1);
        }

        memo[i][j] = possible ? 1 : -1;
        return possible;
    };

    bool result = dp(0, 0);

    if (result && assignment != nullptr) {
        assignment->clear();
        assignment->reserve(total);

        std::size_t i = 0, j = 0;
        for (std::size_t k = 0; k < total; ++k) {
            if (i < n1 && shuffle[k] == deck1[i] && dp(i + 1, j)) {
                // Taking from deck1 leads to a solution.
                assignment->push_back(0);
                ++i;
            } else {
                // Must take from deck2 (guaranteed to be valid by dp result).
                assignment->push_back(1);
                ++j;
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Basic valid case
    {
        std::vector<int> d1 = {1, 2};
        std::vector<int> d2 = {3, 4};
        std::vector<int> sh = {1, 3, 2, 4};
        std::vector<int> assign;
        assert(is_shuffle_of(d1, d2, sh, &assign));
        assert(assign == std::vector<int>({0, 1, 0, 1}));
    }

    // Invalid: order within deck1 violated
    {
        std::vector<int> d1 = {1, 2};
        std::vector<int> d2 = {3};
        std::vector<int> sh = {1, 3, 2};
        std::vector<int> assign;
        assert(is_shuffle_of(d1, d2, sh, &assign)); // Actually should be valid? Check: 1(from1),3(from2),2(from1) => valid
        assert(assign == std::vector<int>({0, 1, 0}));
    }

    // Invalid: too many elements
    {
        std::vector<int> d1 = {1};
        std::vector<int> d2 = {2};
        std::vector<int> sh = {1, 2, 3};
        std::vector<int> assign;
        assert(!is_shuffle_of(d1, d2, sh, &assign));
    }

    // Invalid: element not in either deck
    {
        std::vector<int> d1 = {1};
        std::vector<int> d2 = {2};
        std::vector<int> sh = {1, 3};
        std::vector<int> assign;
        assert(!is_shuffle_of(d1, d2, sh, &assign));
    }

    // Empty decks
    {
        std::vector<int> d1 = {};
        std::vector<int> d2 = {};
        std::vector<int> sh = {};
        std::vector<int> assign;
        assert(is_shuffle_of(d1, d2, sh, &assign));
        assert(assign.empty());
    }

    // One deck empty
    {
        std::vector<int> d1 = {};
        std::vector<int> d2 = {5, 6};
        std::vector<int> sh = {5, 6};
        std::vector<int> assign;
        assert(is_shuffle_of(d1, d2, sh, &assign));
        assert(assign == std::vector<int>({1, 1}));
    }

    // Duplicate values where multiple assignments possible; must find a valid one
    {
        std::vector<int> d1 = {1, 1};
        std::vector<int> d2 = {1};
        std::vector<int> sh = {1, 1, 1};
        std::vector<int> assign;
        assert(is_shuffle_of(d1, d2, sh, &assign));
        // It should be true; assignment could be e.g. {0,0,1} or {0,1,0} etc.
        assert(assign.size() == 3);
        // Verify assignment is coherent
        std::vector<int> rev1, rev2;
        for (std::size_t i = 0; i < sh.size(); ++i) {
            if (assign[i] == 0) rev1.push_back(sh[i]);
            else rev2.push_back(sh[i]);
        }
        assert(rev1 == d1 && rev2 == d2);
    }

    // Larger case where naive greedy fails: need to backtrack
    {
        std::vector<int> d1 = {1, 2, 3};
        std::vector<int> d2 = {2, 1};
        std::vector<int> sh = {1, 2, 2, 1, 3};
        std::vector<int> assign;
        assert(is_shuffle_of(d1, d2, sh, &assign));
        // Verify
        std::vector<int> rev1, rev2;
        for (std::size_t i = 0; i < sh.size(); ++i) {
            if (assign[i] == 0) rev1.push_back(sh[i]);
            else rev2.push_back(sh[i]);
        }
        assert(rev1 == d1 && rev2 == d2);
    }

    return 0;
}
// The core problem is to check whether the shuffle sequence can be partitioned into two subsequences that exactly match `deck1` and `deck2` in order. This is a classic decision problem solvable via backtracking with memoization (dynamic programming on indices). Define a recursive function `dp(i, j, k)` where `i` is the current index in `deck1`, `j` in `deck2`, and `k` in `shuffle`. At each step, if `shuffle[k]` matches `deck1[i]`, we can try taking from deck1; if it matches `deck2[j]`, we can try taking from deck2. If both match and lead to different futures, we need to explore both; if neither matches, the current branch fails. Base case: when `k == shuffle.size()`, success if `i == deck1.size() && j == deck2.size()`. Because `k = i + j` always (the number of elements taken so far equals the sum of indices), we can reduce to a 2D DP over `i` and `j`. Memoization stores a boolean for each `(i,j)` indicating whether that state is reachable and leads to a solution. Time complexity is `O(|deck1| * |deck2|)` states, each doing O(1) work. Space complexity is also `O(|deck1| * |deck2|)` for memoization plus O(1) for tracking. For reconstruction of the assignment array, we can do a second pass using the memoized DP table to decide at each step which choice leads to a valid solution (prefer deck1 first, but if that fails, try deck2). Edge cases include empty decks (e.g., one deck empty, then shuffle must equal the other exactly), duplicate values causing multiple valid paths, and the case where both decks are empty and shuffle is empty (valid, assignment empty). The initial call is `dp(0,0,0)`.

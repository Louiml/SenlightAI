Write a C++ function that, given a positive integer `n` (where `2 <= n <= 58`), returns the maximum product obtainable by breaking `n` into the sum of at least two positive integers, then multiplying those integers together. For example, for `n = 2`, the only way to break it is `1 + 1`, and the product is `1 * 1 = 1`. For `n = 10`, breaking into `3 + 3 + 4` gives the maximum product `36`. The function must handle all values in the given range and return an `int`.

#include <cassert>

int main() {
    assert(maxProductAfterBreaking(2) == 1);   // 1*1
    assert(maxProductAfterBreaking(3) == 2);   // 1*2
    assert(maxProductAfterBreaking(4) == 4);   // 2*2
    assert(maxProductAfterBreaking(5) == 6);   // 2*3
    assert(maxProductAfterBreaking(6) == 9);   // 3*3
    assert(maxProductAfterBreaking(7) == 12);  // 3*4
    assert(maxProductAfterBreaking(8) == 18);  // 3*3*2
    assert(maxProductAfterBreaking(9) == 27);  // 3*3*3
    assert(maxProductAfterBreaking(10) == 36); // 3*3*4
    assert(maxProductAfterBreaking(58) == 1549681956); // known maximum
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstring>

// Returns the maximum product obtainable from breaking n into at least two positive integers.
int maxProductAfterBreaking(int n) {
    // Memoization array, since n <= 58, we can use a fixed-size array.
    int memo[59];
    std::memset(memo, -1, sizeof(memo));

    // Inner recursive lambda for clarity, but we implement as a helper function.
    // Use a local static-like approach via a lambda with capture.
    std::function<int(int)> solve = [&](int num) -> int {
        if (num == 1) {
            return 1;
        }
        if (memo[num] != -1) {
            return memo[num];
        }
        int best = 0; // initialize to 0 since all products are positive
        for (int i = 1; i <= num - 1; ++i) {
            // Either keep the rest as is, or break the rest further.
            int product = i * std::max(num - i, solve(num - i));
            best = std::max(best, product);
        }
        memo[num] = best;
        return best;
    };

    return solve(n);
}

// This is a classic dynamic programming problem. We define `t[n]` as the maximum product obtainable from breaking `n` (with at least one break, but note that for subproblems we allow no further break, meaning the subproblem can be kept whole). The recurrence is: for each `i` from `1` to `n-1`, we consider breaking off a piece `i` and then either keeping the rest `n-i` whole (product `i * (n-i)`) or further breaking the rest (product `i * solve(n-i)`). The maximum over all `i` gives the answer for `n`. Base case: `solve(1) = 1` (though for the main problem `n >= 2`, subproblems may reach `1`). We use memoization with an array `t` initialized to `-1`. Important edge case: for the original `n`, we must ensure at least one break (which the loop guarantees since `i` ranges from 1 to `n-1`), but for subproblems, allowing the whole value without breaking is correct because when we combine with `i`, the total number of parts is at least 2 overall. Time complexity is `O(n^2)` due to the nested loop over `i` for each `n`, and space is `O(n)` for the memoization array.

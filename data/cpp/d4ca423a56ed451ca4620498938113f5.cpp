Write a C++ function `binomialCoefficient(int n, int k)` that computes the binomial coefficient \( C(n, k) \) (also written as "n choose k"), which represents the number of ways to choose \( k \) items from \( n \) items without regard to order. The function must return the result as an `int`. You must implement the function using **three different approaches**: (1) a plain recursive method (no memoization), (2) a top-down dynamic programming method using memoization, and (3) a bottom-up dynamic programming method using a 2D table. The function signature must be `int binomialCoefficient(int n, int k)` and it must internally decide which method to use based on a third parameter `int method` (pass `1`, `2`, or `3` for recursive, memoized, or bottom-up respectively). The function should handle edge cases where `k` is `0` or `k == n` (returning `1`), and also validate that `0 ≤ k ≤ n`; if not, the function should return `0`. The input `n` and `k` are non‑negative integers and assume `n` is at most 30 so that the result fits within a signed 32‑bit `int` (since \( C(30,15) = 155,117,520\) which is safe, but \( C(31,15) \) would overflow). Write a self‑contained function (no `main`) that includes necessary headers and uses `const` correctness where appropriate.

#include <cassert>

int main() {
    // Basic cases
    assert(binomialCoefficient(0, 0, 1) == 1);
    assert(binomialCoefficient(5, 0, 1) == 1);
    assert(binomialCoefficient(5, 5, 1) == 1);
    assert(binomialCoefficient(5, 2, 1) == 10);

    // Invalid inputs
    assert(binomialCoefficient(5, 6, 1) == 0);
    assert(binomialCoefficient(5, -1, 1) == 0);

    // Compare all three methods for a range
    for (int n = 0; n <= 12; ++n) {
        for (int k = 0; k <= n; ++k) {
            int r1 = binomialCoefficient(n, k, 1);
            int r2 = binomialCoefficient(n, k, 2);
            int r3 = binomialCoefficient(n, k, 3);
            assert(r1 == r2 && r2 == r3);
        }
    }

    // Larger value within safe int range
    assert(binomialCoefficient(20, 10, 3) == 184756);
    assert(binomialCoefficient(30, 15, 2) == 155117520);

    return 0;
}

#include <vector>

// Compute the binomial coefficient C(n, k) using the specified method:
// method = 1: plain recursion
// method = 2: top-down memoization
// method = 3: bottom-up DP
// Returns 0 if k < 0 or k > n.
// Assumes n <= 30 so the result fits in an int.
int binomialCoefficient(int n, int k, int method) {
    if (k < 0 || k > n || n < 0) {
        return 0;
    }
    if (k == 0 || k == n) {
        return 1;
    }

    if (method == 1) {
        // Plain recursion
        return binomialCoefficient(n - 1, k - 1, 1) + binomialCoefficient(n - 1, k, 1);
    }
    else if (method == 2) {
        // Top-down memoization
        std::vector<std::vector<int>> memo(n + 1, std::vector<int>(n + 1, -1));
        // Recursive lambda to compute and cache
        // We use a helper function inside to avoid re-creating memo each call
        // Since the function signature is fixed, we define a lambda here
        // but we need a way to pass memo by reference. We'll use a std::function.
        // However, to keep it simple and self-contained, we'll implement a static helper.
        // To avoid complexity, we'll implement a nested function via a lambda that captures memo by reference.
        // But C++ doesn't allow recursive lambdas easily without std::function.
        // Therefore, we'll define a separate local struct or use a static helper inside the function.
        // For clarity, we'll implement the memoization with a loop-based helper using a lambda and std::function.
        // But std::function requires <functional>. We'll include it.
        // Since the solution must be self-contained, we'll add #include <functional>.
        // However, the problem expects a concise solution. Let's implement a separate recursive helper function
        // that takes memo by reference and computes. We'll declare it as a static function inside the block.
        // Actually, we can use a private helper function outside the main function. But the task says output code only
        // with a single function? The instruction says "Write a high-quality, self-contained C++ implementation with a descriptively named free function that matches the task specification." It doesn't forbid helper functions, but to keep it simple, we'll use a lambda with std::function.
        // Let's do that.
        #include <functional>
        std::function<int(int,int)> rec = [&](int a, int b) -> int {
            if (b == 0 || a == b) return 1;
            if (memo[a][b] != -1) return memo[a][b];
            memo[a][b] = rec(a-1, b-1) + rec(a-1, b);
            return memo[a][b];
        };
        return rec(n, k);
    }
    else if (method == 3) {
        // Bottom-up DP
        std::vector<std::vector<int>> table(n + 1, std::vector<int>(n + 1, 0));
        for (int i = 0; i <= n; ++i) {
            table[i][0] = 1;
            table[i][i] = 1;
        }
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j < i; ++j) {
                table[i][j] = table[i-1][j-1] + table[i-1][j];
            }
        }
        return table[n][k];
    }
    // If method is invalid, return -1 as an error indicator? But spec says method is 1,2,3.
    // We'll default to method 1.
    return binomialCoefficient(n, k, 1);
}

**Note:** The above solution includes `#include <functional>` inside the function, which is not allowed. The provided solution must be clean and self-contained. The correct approach is to write a separate helper function outside the main function. But the instruction says "Output code only. Do not include a main function or any other entry-point wrapper." It does not forbid helper functions. So we can define a helper function before the main function. However, the user specifically says "Provide a high-quality, self-contained C++ implementation with a descriptively named free function that matches the task specification." That implies we can have additional helper functions. To keep it clean, I'll rewrite the solution with a helper function for memoization.

Given the ambiguity, I'll provide a clean solution with three separate internal functions, all encapsulated within a single public function. For the sake of clarity and to avoid including `<functional>` inside the function, I'll use a helper function that takes the memo table by reference and uses recursion. Here is the revised solution:

#include <vector>

// Helper for top-down memoization
int binomialMemo(int n, int k, std::vector<std::vector<int>>& memo) {
    if (k == 0 || n == k) return 1;
    if (memo[n][k] != -1) return memo[n][k];
    memo[n][k] = binomialMemo(n - 1, k - 1, memo) + binomialMemo(n - 1, k, memo);
    return memo[n][k];
}

// Public function: compute C(n,k) using method (1=recursive, 2=memoized, 3=bottom-up)
int binomialCoefficient(int n, int k, int method) {
    if (k < 0 || k > n || n < 0) return 0;
    if (k == 0 || k == n) return 1;

    if (method == 1) {
        // Plain recursion
        return binomialCoefficient(n - 1, k - 1, 1) + binomialCoefficient(n - 1, k, 1);
    }
    else if (method == 2) {
        std::vector<std::vector<int>> memo(n + 1, std::vector<int>(n + 1, -1));
        return binomialMemo(n, k, memo);
    }
    else if (method == 3) {
        std::vector<std::vector<int>> table(n + 1, std::vector<int>(n + 1, 0));
        for (int i = 0; i <= n; ++i) {
            table[i][0] = 1;
            table[i][i] = 1;
        }
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j < i; ++j) {
                table[i][j] = table[i - 1][j - 1] + table[i - 1][j];
            }
        }
        return table[n][k];
    }
    // Invalid method – fallback to recursion
    return binomialCoefficient(n, k, 1);
}

// The binomial coefficient can be computed using the recurrence \( C(n,k) = C(n-1,k-1) + C(n-1,k) \) with base cases \( C(n,0) = C(n,n) = 1 \).  
// - **Method 1 (Recursive)**: Directly implement the recurrence. This leads to exponential time complexity \( O(2^n) \) and \( O(n) \) recursion stack space. It works fine for small `n` but becomes extremely slow for `n > 20`.  
// - **Method 2 (Top‑down memoization)**: Use a 2D vector `memo` of size `(n+1) × (n+1)` initialized to `-1`. On each call, if `memo[n][k]` is already computed, return it; otherwise compute recursively and store. This reduces time to \( O(n \cdot k) \) and space to \( O(n^2) \) for the table plus recursion stack.  
// - **Method 3 (Bottom‑up DP)**: Fill a 2D table `memo` of size `(n+1) × (n+1)`. Set `memo[i][0] = 1` and `memo[i][i] = 1` for all `i`. Then for `i` from 1 to `n` and `j` from 1 to `i-1`, compute `memo[i][j] = memo[i-1][j-1] + memo[i-1][j]`. The answer is `memo[n][k]`. Time is \( O(n^2) \) and space \( O(n^2) \).  
// All methods must handle invalid input (`k < 0` or `k > n`) by returning `0`. Edge case `n == 0` with `k == 0` returns `1`. The chosen implementation should be robust and avoid integer overflow by assuming `n ≤ 30`. The solution will provide a single function that dispatches to the appropriate internal helper based on the `method` parameter.

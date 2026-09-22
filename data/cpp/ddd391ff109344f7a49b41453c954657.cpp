/*
Write a C++ function `countDistributions` that takes two positive integers `n` (number of children) and `m` (number of candies) and returns a vector of strings, where each string represents one valid way to distribute all `m` candies among `n` children such that each child receives a strictly positive integer number of candies, and the candies are distributed in increasing order (i.e., each child gets more candies than the previous child). The function should return an empty vector if no such distribution exists (e.g., when `n > m`). Each string in the returned vector should contain the `n` numbers separated by single spaces, and the distributions must be generated in lexicographic order (the order produced by the recursive backtracking in the snippet). The function must be `const`-correct and should not modify its inputs.
*/
#include <string>
#include <vector>
#include <sstream>

// Returns all ways to distribute m candies among n children in strictly
// increasing positive amounts. Each string is "c1 c2 ... cn" with c1 < c2 < ... < cn.
std::vector<std::string> countDistributions(int n, int m) {
    std::vector<std::string> result;
    std::vector<int> candies(n + 1, 0); // 1-indexed for clarity
    int currentSum = 0;

    std::function<void(int)> tryAssign = [&](int child) {
        if (child == n) {
            int last = m - currentSum;
            // Must be positive and > previous (strict increase)
            if (last > candies[child - 1]) {
                candies[child] = last;
                std::ostringstream oss;
                for (int i = 1; i <= n; ++i) {
                    if (i > 1) oss << ' ';
                    oss << candies[i];
                }
                result.push_back(oss.str());
            }
            return;
        }

        // Maximum value for this child so that remaining children get at least
        // (value+1), (value+2), ... and the last gets at least value + (n-child)
        // but also ensuring total does not exceed m.
        int maxVal = m - currentSum - (n - child);
        for (int v = (child == 1 ? 1 : candies[child - 1] + 1); v <= maxVal; ++v) {
            candies[child] = v;
            currentSum += v;
            tryAssign(child + 1);
            currentSum -= v;
        }
    };

    if (n <= m) {
        tryAssign(1);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Assume the solution function is declared above.

int main() {
    // No solution when n > m
    assert(countDistributions(3, 2).empty());
    // Minimal case n=1, m=1 → one distribution
    assert(countDistributions(1, 1) == std::vector<std::string>{"1"});
    // n=1, m=5 → only one distribution with all candies to one child
    assert(countDistributions(1, 5) == std::vector<std::string>{"5"});
    // n=2, m=3 → only (1,2)
    assert(countDistributions(2, 3) == std::vector<std::string>{"1 2"});
    // n=2, m=4 → (1,3) and (2,2 but not allowed), so only (1,3) since strict increase
    // Actually (1,3) and (2,2) invalid due to equal, so result is {"1 3"}
    assert(countDistributions(2, 4) == std::vector<std::string>{"1 3"});
    // n=3, m=6 → only (1,2,3)
    assert(countDistributions(3, 6) == std::vector<std::string>{"1 2 3"});
    // n=3, m=7 → (1,2,4)
    assert(countDistributions(3, 7) == std::vector<std::string>{"1 2 4"});
    // n=3, m=9 → (1,2,6), (1,3,5), (2,3,4) in lexicographic order
    assert(countDistributions(3, 9) == (std::vector<std::string>{"1 2 6", "1 3 5", "2 3 4"}));
    // n=4, m=10 → minimal is 1+2+3+4=10 → one solution
    assert(countDistributions(4, 10) == std::vector<std::string>{"1 2 3 4"});
    return 0;
}
// The problem is a classic combinatorial enumeration: partition `m` into `n` strictly positive, strictly increasing parts. The algorithm uses recursion with backtracking. At step `k` (1-based index of the current child), the remaining candies are `m - f` where `f` is the sum of candies already assigned to children `1..k-1`. To ensure that each subsequent child gets strictly more than the previous one, and that there are enough candies left for the remaining children (each needing at least 1 more than the previous), the maximum value for the current child is `m - f - (n - k)`. The recursion sets `x[k] = v` for values `v` from 1 up to that maximum, updates the sum, and recurses. When `k == n`, the last child gets all remaining candies, but only if that remainder is positive and greater than `x[n-1]` (the strict increase is guaranteed because the previous steps enforced that the next minimum is at least `v+1`). The base case in the snippet checks `x[k] > 0`, but also implicitly ensures positivity and ordering because the remaining candies must be at least 1 more than the previous assignment due to the earlier bound. Edge cases: if `n > m`, no solution exists (each child needs at least 1, so `m` must be at least `n`). Also, if `m == n`, the only solution is all 1's, which does not satisfy strict increase unless `n == 1`; so for `n > 1`, `m` must be at least `n + n*(n-1)/2` (the minimal sum of `1..n`). The algorithm explores all valid combinations in lexicographic order because it iterates `v` from 1 upward. Time complexity is exponential in the worst case (number of solutions), but each solution is generated in O(n) time to produce the string; the total work is O(n * number_of_solutions). Space complexity is O(n) for the recursion stack and the temporary array (not counting the output vector).

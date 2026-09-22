Write a C++ function `vector<vector<int>> combinationSum3(int k, int n)` that returns all possible combinations of exactly `k` distinct numbers from 1 to 9 (inclusive) that sum to `n`. Each combination must be a list of unique integers in increasing order, and the order of combinations in the returned vector does not matter. The function should return an empty vector if no such combinations exist. Assume `k` and `n` are positive integers, with `k` between 1 and 9, and `n` between 1 and 60 (though logically the maximum sum of 9 distinct digits is 45). The solution must not use any global or static variables except the function itself, and must be implemented recursively with backtracking.

The problem is a classic combination-sum variant with a fixed set of candidates: the digits 1 through 9. We use a recursive backtracking approach. Starting from a candidate digit `start` (initially 1), we iterate through possible digits from `start` to 9. For each digit, we add it to a temporary vector, update the current sum, and recursively call the solver with the next digit (`i+1`) to ensure distinctness and increasing order. If at any point the current sum exceeds `n` or the temporary vector size exceeds `k`, we prune (return early). When the current sum equals `n` and the size equals `k`, we record a valid combination. After the recursive call, we backtrack by removing the last digit and subtracting its value from the sum. Edge cases include: when `n` is too small or too large relative to `k` (e.g., `k=9` requires sum at least 45, so `n<45` yields no solutions), and when `k` itself exceeds 9 (but we assume valid input). Time complexity: The number of combinations is bounded by C(9, k), and each combination is built in O(k) time, so worst-case O(C(9,k)*k) which is at most 126 * 9 = ~1134 operations. Space complexity: O(k) for the recursion stack and temporary vector, plus O(number of solutions * k) for the answer storage.

#include <vector>

// Returns all combinations of exactly k distinct digits from 1..9 that sum to n.
std::vector<std::vector<int>> combinationSum3(int k, int n) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    
    // Recursive helper: start is the smallest digit to consider next.
    // currentSum is the sum of digits in current.
    // currentSize is current.size() (but we can use current.size() directly).
    auto backtrack = [&](int start, int currentSum, auto&& self) -> void {
        // Prune: if sum exceeds target or we have too many digits.
        if (currentSum > n || current.size() > k) return;
        // Found a valid combination.
        if (currentSum == n && current.size() == k) {
            result.push_back(current);
            return;
        }
        // Try each digit from start to 9.
        for (int digit = start; digit <= 9; ++digit) {
            current.push_back(digit);
            self(digit + 1, currentSum + digit, self);
            current.pop_back();
        }
    };
    
    backtrack(1, 0, backtrack);
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// The function declaration is provided by the solution above.
// Here we include a copy for the test file (or rely on inclusion).
std::vector<std::vector<int>> combinationSum3(int k, int n);

int main() {
    // Example: k=3, n=7 -> [1,2,4]
    auto r1 = combinationSum3(3, 7);
    assert(r1.size() == 1);
    assert(r1[0] == std::vector<int>({1,2,4}));

    // Example: k=3, n=9 -> [1,2,6], [1,3,5], [2,3,4]
    auto r2 = combinationSum3(3, 9);
    assert(r2.size() == 3);
    // Since order is unspecified, sort and compare sets.
    for (auto& comb : r2) std::sort(comb.begin(), comb.end());
    std::sort(r2.begin(), r2.end());
    std::vector<std::vector<int>> expected2 = {{1,2,6}, {1,3,5}, {2,3,4}};
    for (auto& comb : expected2) std::sort(comb.begin(), comb.end());
    std::sort(expected2.begin(), expected2.end());
    assert(r2 == expected2);

    // Example: k=4, n=1 -> no combination (min sum with 4 digits is 1+2+3+4=10)
    auto r3 = combinationSum3(4, 1);
    assert(r3.empty());

    // Example: k=9, n=45 -> only [1..9]
    auto r4 = combinationSum3(9, 45);
    assert(r4.size() == 1);
    assert(r4[0] == std::vector<int>({1,2,3,4,5,6,7,8,9}));

    // Example: k=1, n=9 -> [9]
    auto r5 = combinationSum3(1, 9);
    assert(r5.size() == 1);
    assert(r5[0] == std::vector<int>({9}));

    // Example: k=1, n=10 -> none
    auto r6 = combinationSum3(1, 10);
    assert(r6.empty());

    // Example: k=2, n=18 -> [9,9] not allowed (distinct) so none.
    auto r7 = combinationSum3(2, 18);
    assert(r7.empty());

    // Example: k=2, n=17 -> [8,9]
    auto r8 = combinationSum3(2, 17);
    assert(r8.size() == 1);
    assert(r8[0] == std::vector<int>({8,9}));

    // Example: k=3, n=6 -> [1,2,3]
    auto r9 = combinationSum3(3, 6);
    assert(r9.size() == 1);
    assert(r9[0] == std::vector<int>({1,2,3}));

    // Example: k=4, n=10 -> [1,2,3,4]
    auto r10 = combinationSum3(4, 10);
    assert(r10.size() == 1);
    assert(r10[0] == std::vector<int>({1,2,3,4}));

    return 0;
}

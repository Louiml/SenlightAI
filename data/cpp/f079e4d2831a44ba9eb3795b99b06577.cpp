// Write a C++ function named `nchoosekAll` that takes three integer parameters: `offset` (the smallest value allowed in a combination), `k` (the number of elements per combination), and `N` (the upper bound, exclusive, for possible values, i.e., values are from `offset` to `N-1` inclusive). The function must return a `std::vector<std::vector<int>>` containing all combinations of exactly `k` distinct integers chosen from the range `[offset, N-1]`, listed in lexicographic (increasing) order. Each inner vector must be sorted in ascending order. If `k` is 0, return a single empty combination. If `k` is negative or `k > N - offset`, return an empty vector. The function must be self-contained and not rely on any global or static state.

The core algorithm is a recursive backtracking/DFS approach. At each recursion level, we fix the next element of the current combination by iterating from a starting index `start` up to `N - k` (where `k` is the remaining number of elements to choose). This bound ensures enough elements remain to fill the combination. When `k` reaches zero, the current partial combination is complete and is appended to the result. After recursion, we backtrack by removing the last added element to try the next candidate. The initial call uses `start = offset` and the full `k`. Edge cases: when `k == 0`, immediately return a vector containing one empty inner vector (no recursion needed). When `k < 0` or `k > N - offset` (i.e., impossible to select that many distinct numbers), return an empty vector. Time complexity is O(C(n, k) * k) where n = N - offset, because each combination of size k is generated once, and copying it to the result takes O(k). Space complexity is O(k) for the recursion stack and the temporary combination buffer, plus O(C(n, k) * k) for the result storage.

#include <vector>

// Generate all combinations of k distinct integers from [offset, N-1].
// Returns combinations in lexicographic order. Each combination is sorted ascending.
std::vector<std::vector<int>> nchoosekAll(int offset, int k, int N) {
    std::vector<std::vector<int>> result;
    
    // Edge cases: invalid k or impossible to choose k distinct numbers.
    if (k < 0 || k > N - offset) {
        return result;
    }
    
    // Special case: exactly one empty combination.
    if (k == 0) {
        result.push_back({});
        return result;
    }
    
    std::vector<int> current;
    
    // Recursive helper to build combinations.
    // start: next candidate index to consider.
    // remaining: how many more elements to select.
    auto backtrack = [&](auto&& self, int start, int remaining) -> void {
        if (remaining == 0) {
            result.push_back(current);
            return;
        }
        // Ensure at least 'remaining' numbers are available: start <= N - remaining.
        for (int i = start; i <= N - remaining; ++i) {
            current.push_back(i);
            self(self, i + 1, remaining - 1);
            current.pop_back();
        }
    };
    
    backtrack(backtrack, offset, k);
    return result;
}

#include <cassert>
#include <vector>

// Function under test (declared here for clarity, but in actual use would be included from header)
std::vector<std::vector<int>> nchoosekAll(int offset, int k, int N);

int main() {
    // Basic case: combinations of 2 from [1,4] exclusive => values 1,2,3
    std::vector<std::vector<int>> result1 = nchoosekAll(1, 2, 4);
    std::vector<std::vector<int>> expected1 = {{1,2}, {1,3}, {2,3}};
    assert(result1 == expected1);

    // k=0 returns one empty combination
    assert(nchoosekAll(0, 0, 5) == std::vector<std::vector<int>>({{}}));

    // k > available range returns empty
    assert(nchoosekAll(2, 3, 4).empty());

    // k negative returns empty
    assert(nchoosekAll(1, -1, 5).empty());

    // Full range with non-zero offset: combinations of 2 from [3,6]
    std::vector<std::vector<int>> result2 = nchoosekAll(3, 2, 7);
    std::vector<std::vector<int>> expected2 = {{3,4}, {3,5}, {3,6}, {4,5}, {4,6}, {5,6}};
    assert(result2 == expected2);

    // k equals full size: only one combination containing all numbers
    assert(nchoosekAll(0, 3, 3) == std::vector<std::vector<int>>({{0,1,2}}));

    // offset and N with negative numbers: range [-2, 1] inclusive
    std::vector<std::vector<int>> result3 = nchoosekAll(-2, 2, 2);
    std::vector<std::vector<int>> expected3 = {{-2,-1}, {-2,0}, {-2,1}, {-1,0}, {-1,1}, {0,1}};
    assert(result3 == expected3);

    // Large N, small k: verify count using binomial formula
    std::vector<std::vector<int>> result4 = nchoosekAll(0, 2, 10);
    assert(result4.size() == 45); // C(10,2)=45
    // Check first and last combinations
    assert(result4.front() == std::vector<int>({0,1}));
    assert(result4.back() == std::vector<int>({8,9}));

    // offset equal to N yields no elements
    assert(nchoosekAll(5, 1, 5).empty());

    // Ensure each inner vector is sorted (checked by construction) and all elements unique
    for (const auto& comb : result4) {
        for (size_t i = 1; i < comb.size(); ++i) {
            assert(comb[i] > comb[i-1]);
        }
    }

    return 0;
}

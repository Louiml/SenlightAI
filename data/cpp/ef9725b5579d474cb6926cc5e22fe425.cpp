// Write a C++ function named `generateCombinations` that takes two integers `n` and `k` (where `1 <= n <= 20` and `1 <= k <= n`) and returns a `std::vector<std::vector<int>>` containing all possible combinations of `k` numbers chosen from the range `[1, n]`. The combinations must be returned in lexicographic (ascending) order, and within each combination, the numbers must be in strictly increasing order. The function should be a free function (not a class method) and must handle the edge case where `k == 0` by returning a single empty combination (i.e., `{{}}`), and if `k > n` or `n < 1`, it should return an empty vector. The solution must use backtracking recursion, avoid duplicates, and be efficient for the given constraints.
#include <cassert>
#include <vector>

// (The solution function is assumed to be included above)

int main() {
    // Case 1: n=4, k=2 (standard example)
    std::vector<std::vector<int>> expected1 = {{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};
    assert(generateCombinations(4, 2) == expected1);

    // Case 2: n=1, k=1
    std::vector<std::vector<int>> expected2 = {{1}};
    assert(generateCombinations(1, 1) == expected2);

    // Case 3: n=5, k=3 (check size and first/last)
    auto res3 = generateCombinations(5, 3);
    assert(res3.size() == 10); // C(5,3)=10
    assert(res3.front() == std::vector<int>({1,2,3}));
    assert(res3.back() == std::vector<int>({3,4,5}));

    // Case 4: k == 0 -> single empty vector
    assert(generateCombinations(3, 0) == std::vector<std::vector<int>>{{}});

    // Case 5: k > n -> empty
    assert(generateCombinations(2, 3).empty());

    // Case 6: n < 1 -> empty
    assert(generateCombinations(0, 1).empty());

    // Case 7: n=20, k=1 (all single elements)
    auto res7 = generateCombinations(20, 1);
    assert(res7.size() == 20);
    assert(res7.front() == std::vector<int>({1}));
    assert(res7.back() == std::vector<int>({20}));

    // Case 8: n=4, k=4 (only one full combination)
    assert(generateCombinations(4, 4) == std::vector<std::vector<int>>{{1,2,3,4}});

    // Case 9: n=3, k=2 (check lexicographic order)
    std::vector<std::vector<int>> expected9 = {{1,2},{1,3},{2,3}};
    assert(generateCombinations(3, 2) == expected9);

    // Case 10: n=6, k=3 (size check only)
    assert(generateCombinations(6, 3).size() == 20); // C(6,3)=20

    return 0;
}
#include <vector>

// Generate all combinations of k numbers chosen from [1, n].
// Returns combinations in lexicographic order; each combination is sorted ascending.
// If k == 0, returns one empty combination. If k > n or n < 1, returns empty vector.
std::vector<std::vector<int>> generateCombinations(int n, int k) {
    std::vector<std::vector<int>> result;
    
    // Edge case: k == 0 -> single empty combination
    if (k == 0) {
        result.push_back({});
        return result;
    }
    
    // Edge case: invalid input
    if (n < 1 || k > n) {
        return result; // empty vector
    }
    
    std::vector<int> current;
    
    // Backtracking helper (lambda to avoid extra function in header)
    // Captures n, k, current, and result by reference.
    auto backtrack = [&](auto&& self, int start) -> void {
        // If current combination is complete, add it to result
        if (current.size() == static_cast<size_t>(k)) {
            result.push_back(current);
            return;
        }
        
        // Try numbers from start to n
        for (int value = start; value <= n; ++value) {
            current.push_back(value);
            self(self, value + 1); // recurse with next starting index
            current.pop_back();    // backtrack
        }
    };
    
    backtrack(backtrack, 1);
    return result;
}
// The problem is a classic combination generation task, solved via depth-first backtracking. The main idea is to build combinations incrementally: at each recursive step, we choose the next number starting from a given index `start` (initially 1) up to `n`, add it to a temporary `current` vector, recursively proceed with the next index (`start + 1`), and then backtrack by removing the last element to try the next number. When the size of `current` equals `k`, we push a copy into the answer. To ensure lexicographic order, we iterate `start` from low to high, and because we only move forward (never reuse numbers), each combination is naturally sorted and unique. Edge cases: if `k == 0`, we immediately return a vector containing one empty vector (since the empty set is a valid combination of size 0). If `k > n` or `n < 1`, no combination exists, so return an empty vector. The recursion depth is at most `k` (≤ 20), and the total number of combinations is C(n, k). Time complexity is O(C(n, k) * k) because each of the C(n, k) combinations requires copying a vector of size `k` into the answer. Space complexity is O(k) for the recursion stack and current vector, plus O(C(n, k) * k) for the output storage.

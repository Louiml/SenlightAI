// Write a C++ function named `allPermutations` that takes a constant reference to a vector of integers and returns a vector of vectors containing all unique permutations of the input. The function should handle empty input by returning an empty outer vector. The order of permutations in the output is not strict, but every permutation must appear exactly once, and the original elements must be preserved (no removal of duplicates from the input, but duplicates in input will naturally produce duplicate permutations, which must also be returned — you may assume for simplicity that the input contains distinct integers; however, your solution should not crash or produce undefined behavior if duplicates exist, even if it outputs duplicate permutations). The function must be self-contained, use recursion or an iterative method, and must not rely on `std::next_permutation` or any standard library permutation helper. The implementation should be efficient enough for n ≤ 8 (at most 40320 permutations).

// The solution uses a recursive backtracking approach. The main idea is to build permutations incrementally by selecting an element from the "remaining" pool, adding it to a "current" partial permutation, and then recursing with the reduced remaining pool. At the base case where the remaining pool is empty, the current partial permutation is complete and is appended to the result. After the recursion returns, the last element is removed from the current partial permutation (backtracking) to allow the next candidate to be tried. This ensures every possible ordering is explored exactly once. Edge cases: an empty input returns an empty vector (no permutations). A single-element input returns a vector containing that element alone. For duplicate input values, the algorithm will generate duplicate permutations, which is acceptable per the specification (though not required to handle). Time complexity is \(O(n \cdot n!)\) because there are \(n!\) permutations and each permutation costs \(O(n)\) to copy into the result; the recursion itself builds each permutation in \(O(n)\) steps. Space complexity is \(O(n \cdot n!)\) for storing the output, plus \(O(n)\) recursion stack depth and \(O(n)\) for the temporary vectors.

#include <vector>
#include <cstddef>

// Return all permutations of the given input vector.
// Uses recursive backtracking. Empty input yields empty output.
std::vector<std::vector<int>> allPermutations(const std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    if (nums.empty()) {
        return result;
    }

    // Recursive helper that builds permutations.
    // current holds the partial permutation built so far.
    // remaining holds the elements not yet placed.
    std::function<void(std::vector<int>&, std::vector<int>&)> backtrack = 
        [&](std::vector<int>& current, std::vector<int>& remaining) {
            if (remaining.empty()) {
                result.push_back(current);
                return;
            }
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                int val = remaining[i];
                current.push_back(val);
                // Build new remaining vector without element at index i.
                std::vector<int> nextRemaining;
                nextRemaining.reserve(remaining.size() - 1);
                for (std::size_t j = 0; j < remaining.size(); ++j) {
                    if (j != i) {
                        nextRemaining.push_back(remaining[j]);
                    }
                }
                backtrack(current, nextRemaining);
                current.pop_back(); // backtrack
            }
        };

    std::vector<int> current;
    std::vector<int> remaining = nums;
    backtrack(current, remaining);
    return result;
}
(Note: To be self-contained, include `<functional>` and `<vector>`. The above uses `std::function`; if you prefer, you can write a separate helper function. The code as written compiles when headers `<vector>`, `<functional>` are added. For the test, we'll include the necessary headers.)

#include <cassert>
#include <vector>
#include <algorithm>
#include <functional>

// Include the solution function here (omitted for brevity in this section, but assumed available)

int main() {
    // Test 1: Empty input returns empty vector.
    std::vector<int> empty;
    assert(allPermutations(empty).empty());

    // Test 2: Single element.
    std::vector<int> single = {7};
    auto resSingle = allPermutations(single);
    assert(resSingle.size() == 1);
    assert(resSingle[0] == single);

    // Test 3: Three distinct elements, 6 permutations.
    std::vector<int> nums = {1, 2, 3};
    auto res = allPermutations(nums);
    assert(res.size() == 6);
    // Check that each permutation is a valid rearrangement and unique.
    std::vector<std::vector<int>> expected = {
        {1,2,3}, {1,3,2}, {2,1,3}, {2,3,1}, {3,1,2}, {3,2,1}
    };
    // Sort both for comparison because order is not specified.
    auto sortedRes = res;
    auto sortedExpected = expected;
    std::sort(sortedRes.begin(), sortedRes.end());
    std::sort(sortedExpected.begin(), sortedExpected.end());
    assert(sortedRes == sortedExpected);

    // Test 4: Two elements.
    std::vector<int> two = {5, -2};
    auto resTwo = allPermutations(two);
    assert(resTwo.size() == 2);
    std::vector<std::vector<int>> expectedTwo = {{5,-2},{-2,5}};
    auto sortedTwo = resTwo;
    std::sort(sortedTwo.begin(), sortedTwo.end());
    std::sort(expectedTwo.begin(), expectedTwo.end());
    assert(sortedTwo == expectedTwo);

    // Test 5: Four distinct elements, 24 permutations.
    std::vector<int> four = {1, 2, 3, 4};
    auto resFour = allPermutations(four);
    assert(resFour.size() == 24);
    // Verify that all permutations are unique and each contains exactly the same multiset.
    for (const auto& perm : resFour) {
        assert(perm.size() == 4);
        std::vector<int> sortedPerm = perm;
        std::sort(sortedPerm.begin(), sortedPerm.end());
        assert(sortedPerm == four);
    }
    // Check uniqueness by sorting and using unique.
    auto sortedAll = resFour;
    std::sort(sortedAll.begin(), sortedAll.end());
    auto last = std::unique(sortedAll.begin(), sortedAll.end());
    assert(last == sortedAll.end());

    // Test 6: Duplicate input (behavior not specified but must not crash).
    std::vector<int> dup = {1, 1, 2};
    auto resDup = allPermutations(dup);
    // It may produce 6 permutations (including duplicates) or fewer, but must not crash.
    // Here we just check it returns something.
    assert(!resDup.empty());
}

Write a C++ function `vector<vector<int>> permute(const vector<int>& nums)` that takes a vector of distinct integers (no duplicates) and returns a vector containing all possible permutations of those integers. The order of permutations in the result does not matter. The function must not modify the input vector (i.e., it should accept a `const` reference). For example, given `nums = {1,2,3}`, the returned vector should contain all six permutations: `[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]` in any order. Handle edge cases such as an empty input (return an empty vector) and a single-element input (return a vector containing that single element as the only permutation).
The core algorithm is a backtracking approach that generates permutations by recursively fixing elements at each position. We create a **copy** of the input vector internally (since the parameter is `const`), then use a helper recursive function that takes the working copy, the result vector, and the current index `i`. At each recursive step, we iterate `j` from `i` to `n-1`, swapping `nums[i]` and `nums[j]` to place each remaining element at position `i`, then recursively call the helper with `i+1`. After the recursive call, we swap back to restore the original order (backtracking). When `i` reaches the size of the vector, we have a complete permutation and push the current state into the result. Important edge cases: an empty input should produce an empty result (no permutations exist); a single-element input produces exactly one permutation (the element itself). Time complexity: There are `n!` permutations, and each permutation requires copying a vector of length `n` into the result, so total time is `O(n * n!)`. Space complexity: The recursion depth is `O(n)`, and the output vector stores `n!` permutations each of size `n`, so output space is `O(n * n!)`. The auxiliary space beyond the output is `O(n)` for the recursion stack.
#include <vector>
#include <algorithm>

// Generate all permutations of a vector of distinct integers.
// Returns a vector of vectors, each inner vector being one permutation.
std::vector<std::vector<int>> permute(const std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    if (nums.empty()) {
        return result; // No permutations for empty input
    }
    // Work on a mutable copy of the input
    std::vector<int> current = nums;
    // Recursive helper using backtracking
    std::function<void(int)> backtrack = [&](int index) {
        if (index == current.size()) {
            result.push_back(current); // A complete permutation is formed
            return;
        }
        for (int j = index; j < current.size(); ++j) {
            std::swap(current[index], current[j]);
            backtrack(index + 1);
            std::swap(current[index], current[j]); // Undo swap (backtrack)
        }
    };
    backtrack(0);
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is declared above (permute) — assumed available.

int main() {
    // Test 1: Basic case with 3 elements
    std::vector<int> nums1 = {1, 2, 3};
    auto result1 = permute(nums1);
    assert(result1.size() == 6);
    // Each permutation must be a permutation of the original set
    for (const auto& perm : result1) {
        assert(perm.size() == 3);
        std::vector<int> sorted = perm;
        std::sort(sorted.begin(), sorted.end());
        assert(sorted == std::vector<int>({1, 2, 3}));
    }
    // Ensure distinct permutations (no duplicates)
    std::vector<std::vector<int>> sortedPerms = result1;
    std::sort(sortedPerms.begin(), sortedPerms.end());
    auto last = std::unique(sortedPerms.begin(), sortedPerms.end());
    assert(last == sortedPerms.end());

    // Test 2: Input is not modified (const correctness)
    std::vector<int> input2 = {4, 5};
    std::vector<int> copy2 = input2;
    auto result2 = permute(input2);
    assert(input2 == copy2); // original unchanged
    assert(result2.size() == 2);
    assert((result2 == std::vector<std::vector<int>>{{4, 5}, {5, 4}}) || 
           (result2 == std::vector<std::vector<int>>{{5, 4}, {4, 5}}));

    // Test 3: Empty input
    std::vector<int> empty;
    auto result3 = permute(empty);
    assert(result3.empty());

    // Test 4: Single element
    std::vector<int> single = {42};
    auto result4 = permute(single);
    assert(result4.size() == 1);
    assert(result4[0] == single);

    // Test 5: Four elements to verify count
    std::vector<int> nums5 = {1, 2, 3, 4};
    auto result5 = permute(nums5);
    assert(result5.size() == 24); // 4! = 24

    // Test 6: Two elements reversed order
    std::vector<int> nums6 = {0, -1};
    auto result6 = permute(nums6);
    assert(result6.size() == 2);
    // Check both permutations are present
    std::vector<std::vector<int>> expected6 = {{0, -1}, {-1, 0}};
    std::sort(result6.begin(), result6.end());
    std::sort(expected6.begin(), expected6.end());
    assert(result6 == expected6);

    return 0;
}

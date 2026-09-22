// Write a standalone C++ function that accepts a vector of integers (which may contain duplicates and appear in any order) and returns a vector containing all possible subsets (the power set) of the input, with the important constraint that duplicate subsets must appear only once. The returned subsets should be sorted in non‑decreasing order and the overall collection must be free of duplicate subsets. For example, given `{1, 2, 2}`, the output should be `{{}, {1}, {1,2}, {1,2,2}, {2}, {2,2}}` – note that `{2}` appears only once even though the input has two 2’s. The function should not modify the input vector. You must design the function signature as `std::vector<std::vector<int>> uniqueSubsets(const std::vector<int>& nums)`.
The core algorithm follows a classic backtracking (depth‑first search) pattern on a sorted input. First, sort the input vector so that duplicate values are adjacent. Then, in a recursive helper, at each state we add the current subset (starting with the empty set) to the answer. The loop iterates from the current index to the end of the vector; before recursing with the current element included, we skip any duplicate values by checking `if (i > idx && nums[i] == nums[i-1]) continue`. This ensures that we do not generate subsets that differ only by which duplicate occurrence was chosen – e.g., the subset `{2}` is generated only once from the first 2, not from the second. After including an element, we recurse with index `i+1` and then backtrack by popping the element. Key edge cases: an empty input vector (the function returns a vector containing only the empty subset), a vector with all identical elements (each subset appears exactly once, and the number of subsets is `size + 1`), and inputs already containing duplicates in arbitrary order. The time complexity is `O(2^n * n)` in the worst case because there are at most `2^n` subsets (after de‑duplication, often fewer) and copying each subset into the answer costs `O(n)`. The space complexity is `O(n)` for the recursion stack and the temporary subset vector, plus `O(2^n * n)` for storing the answer.
#include <vector>
#include <algorithm>

// Generate all unique subsets of a multiset of integers.
// The input vector is not modified. Subsets are returned in lexicographic order.
std::vector<std::vector<int>> uniqueSubsets(const std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    
    // Helper recursive lambda to build subsets
    std::function<void(int)> backtrack = [&](int start) {
        // Add the current subset (which is valid at every recursion level)
        result.push_back(current);
        
        // Try to extend the current subset with elements from index 'start' onward
        for (int i = start; i < static_cast<int>(nums.size()); ++i) {
            // Skip duplicates to avoid generating the same subset
            if (i > start && nums[i] == nums[i - 1]) {
                continue;
            }
            // Include nums[i] and recurse
            current.push_back(nums[i]);
            backtrack(i + 1);
            // Backtrack
            current.pop_back();
        }
    };
    
    // Sort the input to bring duplicates together
    std::vector<int> sortedNums = nums;
    std::sort(sortedNums.begin(), sortedNums.end());
    
    // Start recursion with the sorted vector
    backtrack(0, sortedNums, current, result);
    
    return result;
}
Actually, to keep the helper a free function or a lambda without capturing extra parameters, here is a cleaner version:

#include <vector>
#include <algorithm>
#include <functional>

// Recursive helper: builds subsets starting from index 'start'
void generateSubsets(int start, const std::vector<int>& nums,
                     std::vector<int>& current,
                     std::vector<std::vector<int>>& result) {
    result.push_back(current);
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        if (i > start && nums[i] == nums[i - 1]) {
            continue;
        }
        current.push_back(nums[i]);
        generateSubsets(i + 1, nums, current, result);
        current.pop_back();
    }
}

// Generate all unique subsets of a multiset of integers.
// The input vector is not modified. Subsets are returned in lexicographic order.
std::vector<std::vector<int>> uniqueSubsets(const std::vector<int>& nums) {
    std::vector<int> sortedNums = nums;
    std::sort(sortedNums.begin(), sortedNums.end());
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    generateSubsets(0, sortedNums, current, result);
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test case 1: empty input -> only empty subset
    std::vector<std::vector<int>> result1 = uniqueSubsets({});
    assert(result1.size() == 1 && result1[0].empty());
    
    // Test case 2: single element
    auto result2 = uniqueSubsets({1});
    assert(result2.size() == 2);
    assert(result2[0].empty());
    assert(result2[1] == std::vector<int>{1});
    
    // Test case 3: duplicates
    auto result3 = uniqueSubsets({1, 2, 2});
    assert(result3.size() == 6);
    // Expected: {}, {1}, {1,2}, {1,2,2}, {2}, {2,2}
    assert(result3[0].empty());
    assert(result3[1] == std::vector<int>{1});
    assert(result3[2] == std::vector<int>{1,2});
    assert(result3[3] == std::vector<int>{1,2,2});
    assert(result3[4] == std::vector<int>{2});
    assert(result3[5] == std::vector<int>{2,2});
    
    // Test case 4: all duplicates
    auto result4 = uniqueSubsets({5,5,5});
    assert(result4.size() == 4);
    // Expected: {}, {5}, {5,5}, {5,5,5}
    assert(result4[0].empty());
    assert(result4[1] == std::vector<int>{5});
    assert(result4[2] == std::vector<int>{5,5});
    assert(result4[3] == std::vector<int>{5,5,5});
    
    // Test case 5: no duplicates
    auto result5 = uniqueSubsets({1,2,3});
    assert(result5.size() == 8);
    // Check each subset is unique (by sorting and comparing)
    auto sortedResult = result5;
    for (auto& subset : sortedResult) std::sort(subset.begin(), subset.end());
    std::sort(sortedResult.begin(), sortedResult.end());
    for (size_t i = 1; i < sortedResult.size(); ++i) {
        assert(sortedResult[i] != sortedResult[i-1]);
    }
    
    // Test case 6: negative numbers and zero
    auto result6 = uniqueSubsets({-1,0,0,1});
    assert(result6.size() == 8);
    // Quick spot check
    bool foundEmpty = false;
    bool foundNegOne = false;
    for (const auto& s : result6) {
        if (s.empty()) foundEmpty = true;
        if (s.size() == 1 && s[0] == -1) foundNegOne = true;
    }
    assert(foundEmpty && foundNegOne);
    
    return 0;
}

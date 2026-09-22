// Write a C++ function `generateUniqueSubsets` that takes a vector of integers (which may contain duplicates) and returns a vector of vectors containing all unique subsets (i.e., the power set without duplicate subsets). The order of subsets or elements within each subset does not matter. The input vector may be empty (in which case the function should return a vector containing one empty vector) and may contain repeated values. The function must handle negative numbers and zero correctly. The returned subsets must be sorted internally (non-decreasing order of elements) to guarantee uniqueness when compared.
#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is defined above; here we test it.

int main() {
    // Test 1: Empty input -> one empty subset.
    std::vector<std::vector<int>> result1 = generateUniqueSubsets({});
    assert(result1.size() == 1 && result1[0].empty());
    
    // Test 2: Single element.
    auto result2 = generateUniqueSubsets({3});
    assert(result2.size() == 2);
    assert(std::find(result2.begin(), result2.end(), std::vector<int>{}) != result2.end());
    assert(std::find(result2.begin(), result2.end(), std::vector<int>{3}) != result2.end());
    
    // Test 3: Duplicates - {1,2,2} should yield 6 subsets.
    auto result3 = generateUniqueSubsets({1,2,2});
    assert(result3.size() == 6);
    assert(std::find(result3.begin(), result3.end(), std::vector<int>{}) != result3.end());
    assert(std::find(result3.begin(), result3.end(), std::vector<int>{1}) != result3.end());
    assert(std::find(result3.begin(), result3.end(), std::vector<int>{2}) != result3.end());
    assert(std::find(result3.begin(), result3.end(), std::vector<int>{1,2}) != result3.end());
    assert(std::find(result3.begin(), result3.end(), std::vector<int>{2,2}) != result3.end());
    assert(std::find(result3.begin(), result3.end(), std::vector<int>{1,2,2}) != result3.end());
    
    // Test 4: All identical - only n+1 subsets.
    auto result4 = generateUniqueSubsets({5,5,5});
    assert(result4.size() == 4);
    assert(std::find(result4.begin(), result4.end(), std::vector<int>{}) != result4.end());
    assert(std::find(result4.begin(), result4.end(), std::vector<int>{5}) != result4.end());
    assert(std::find(result4.begin(), result4.end(), std::vector<int>{5,5}) != result4.end());
    assert(std::find(result4.begin(), result4.end(), std::vector<int>{5,5,5}) != result4.end());
    
    // Test 5: Negative numbers and zero.
    auto result5 = generateUniqueSubsets({-1,0,-1});
    assert(result5.size() == 5);
    assert(std::find(result5.begin(), result5.end(), std::vector<int>{}) != result5.end());
    assert(std::find(result5.begin(), result5.end(), std::vector<int>{-1}) != result5.end());
    assert(std::find(result5.begin(), result5.end(), std::vector<int>{0}) != result5.end());
    assert(std::find(result5.begin(), result5.end(), std::vector<int>{-1,0}) != result5.end());
    assert(std::find(result5.begin(), result5.end(), std::vector<int>{-1,-1}) != result5.end());
    
    // Test 6: Larger set with mixed duplicates.
    auto result6 = generateUniqueSubsets({1,2,2,3});
    assert(result6.size() == 12); // 2^4 =16, but duplicates reduce to 12
    // Check a few specific subsets
    assert(std::find(result6.begin(), result6.end(), std::vector<int>{2,3}) != result6.end());
    assert(std::find(result6.begin(), result6.end(), std::vector<int>{1,2,2,3}) != result6.end());
    
    return 0;
}
#include <vector>
#include <algorithm>

// Generate all unique subsets from a vector that may contain duplicates.
// The input vector is not modified; the result contains subsets with elements sorted.
std::vector<std::vector<int>> generateUniqueSubsets(const std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    
    // Work on a sorted copy to bring duplicates together.
    std::vector<int> sortedNums = nums;
    std::sort(sortedNums.begin(), sortedNums.end());
    
    // Recursive lambda to build subsets.
    std::function<void(int)> backtrack = [&](int index) {
        // Record the current subset.
        result.push_back(current);
        
        for (int i = index; i < sortedNums.size(); ++i) {
            // Skip duplicates when not at the starting position.
            if (i != index && sortedNums[i] == sortedNums[i - 1]) continue;
            
            current.push_back(sortedNums[i]);
            backtrack(i + 1);
            current.pop_back();
        }
    };
    
    backtrack(0);
    return result;
}
// The solution uses a recursive backtracking approach with a sorted input array to efficiently skip duplicates. First, sort the input vector so that duplicate elements are adjacent. Define a recursive helper that, at each index, pushes the current subset into the answer, then iterates from the current index to the end. For each position, if the element is the same as the previous element and it is not the starting position of the loop, skip it—this prevents generating duplicate subsets. Otherwise, include the element, recurse with the next index, and then backtrack by popping. The base case is implicit when the loop ends; each call to the helper first records the current subset. Edge cases include an empty input (returns `{{}}`), all identical elements (only `n+1` subsets), and large duplicate counts. Time complexity is O(2^n * n) in the worst case for generating and copying subsets, and space complexity is O(2^n * k) where k is the average subset length, plus recursion stack depth of O(n).

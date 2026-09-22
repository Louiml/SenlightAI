// Write a C++ function `subsetSums` that takes a `const std::vector<int>&` of non-negative integers and returns a `std::vector<int>` containing all possible subset sums (including the empty subset sum of 0), sorted in ascending order. The function must handle arrays of any length from 0 to 20, with numbers up to 1000 each. The result should contain duplicates if different subsets produce the same sum. For example, given `{1, 2, 3}`, the output should be `{0, 1, 2, 3, 3, 4, 5, 6}`. The function must be efficient enough for up to 2^20 total subsets, and must be written with proper `const` correctness and no global state.
The core approach is recursive enumeration of all subsets. At each index, we have two choices: either include the current element in the running sum, or exclude it. When we reach the end of the array (index == n), we record the accumulated sum into the result vector. This generates exactly 2^n sums. After the recursion completes, we sort the result vector in ascending order to meet the requirement. 

Edge cases: 
- An empty input vector `{}` should return a single element `{0}` because the only subset is the empty one.
- Arrays with duplicate values will produce duplicate sums (e.g., `{1,1}` yields `{0,1,1,2}`).
- Large n (up to 20) leads to 1,048,576 recursive calls and the same number of stored sums. This is manageable within typical time limits using recursion. 
- The numbers are non-negative, so no special handling for negative sums is needed.

Time complexity: O(2^n) for generating sums, plus O(2^n log(2^n)) for sorting, which simplifies to O(2^n log(2^n)). In practice, for n ≤ 20, this is acceptable. Space complexity: O(2^n) for storing the result vector, plus O(n) recursion depth.
#include <vector>
#include <algorithm>

// Compute all subset sums of the input array and return them sorted ascending.
// The input must contain non-negative integers.
std::vector<int> subsetSums(const std::vector<int>& arr) {
    std::vector<int> result;
    int n = static_cast<int>(arr.size());
    
    // Helper lambda for recursive enumeration
    // Pass sum by value to avoid mutation across branches
    std::function<void(int, int)> dfs = [&](int index, int currentSum) {
        if (index == n) {
            result.push_back(currentSum);
            return;
        }
        // Include arr[index]
        dfs(index + 1, currentSum + arr[index]);
        // Exclude arr[index]
        dfs(index + 1, currentSum);
    };
    
    dfs(0, 0);
    
    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>
#include <functional>

// (Include the solution function above or in a header)

int main() {
    // Basic case
    std::vector<int> arr1 = {1, 2, 3};
    std::vector<int> result1 = subsetSums(arr1);
    std::vector<int> expected1 = {0, 1, 2, 3, 3, 4, 5, 6};
    assert(result1 == expected1);

    // Empty array
    std::vector<int> arr2 = {};
    std::vector<int> result2 = subsetSums(arr2);
    std::vector<int> expected2 = {0};
    assert(result2 == expected2);

    // Single element
    std::vector<int> arr3 = {5};
    std::vector<int> result3 = subsetSums(arr3);
    std::vector<int> expected3 = {0, 5};
    assert(result3 == expected3);

    // Duplicate values
    std::vector<int> arr4 = {1, 1};
    std::vector<int> result4 = subsetSums(arr4);
    std::vector<int> expected4 = {0, 1, 1, 2};
    assert(result4 == expected4);

    // All zeros
    std::vector<int> arr5 = {0, 0};
    std::vector<int> result5 = subsetSums(arr5);
    std::vector<int> expected5 = {0, 0, 0, 0};
    assert(result5 == expected5);

    // Larger array with mixed values
    std::vector<int> arr6 = {2, 4, 8};
    std::vector<int> result6 = subsetSums(arr6);
    std::vector<int> expected6 = {0, 2, 4, 6, 8, 10, 12, 14};
    assert(result6 == expected6);

    // Verify size for n=10 (should be 1024 sums)
    std::vector<int> arr7(10, 1);
    std::vector<int> result7 = subsetSums(arr7);
    assert(result7.size() == 1024);
    // First element is 0, last is 10
    assert(result7.front() == 0);
    assert(result7.back() == 10);
    // Count occurrences: each sum k appears C(10,k) times
    // Check sum of 5 appears exactly 252 times
    int count5 = 0;
    for (int val : result7) if (val == 5) count5++;
    assert(count5 == 252);

    return 0;
}

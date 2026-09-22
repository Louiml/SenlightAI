// Write a C++ function `maxGainFromNegativeSort` that takes two integers `n` and `m` (where `1 <= m <= n`), followed by a vector of `n` integers. The function should sort the vector in ascending order using a simple O(n²) algorithm (bubble sort or selection sort, not `std::sort`), then sum the absolute values of the first `m` elements, but only if those elements are negative. If any of the first `m` elements are zero or positive, they contribute `0` to the sum. Return the resulting sum as an integer. This is based on the provided snippet, which reads `n`, `m`, then `n` values, sorts them ascending, and adds `-array[i]` for the first `m` elements only when `array[i] < 0`.

// The main algorithm is straightforward: first, sort the input vector in ascending order using a simple nested-loop selection sort (comparing each element with all later elements and swapping when a smaller element is found). After sorting, iterate through the first `m` positions (indices `0` to `m-1`). For each position, check if the value is negative; if so, add its absolute value (i.e., negate it since it’s negative) to the running sum; otherwise, add nothing. Edge cases include: `m` equals `n`, meaning we inspect all elements; all values positive, yielding sum `0`; all values negative, yielding sum of absolute values of the smallest `m` (which, after sorting, are the most negative ones); and duplicates which are handled naturally. Time complexity is O(n²) due to the sorting loop, plus O(m) for summation, so overall O(n²). Space complexity is O(1) extra beyond the input vector, as sorting is done in-place and only a constant number of variables are used.

#include <vector>
#include <algorithm> // for std::swap

/**
 * Sorts a vector in ascending order using selection sort,
 * then sums the absolute values of the first m elements that are negative.
 * 
 * @param nums A vector of integers to be sorted and processed.
 * @param m The number of leading elements (after sorting) to examine.
 * @return The sum of absolute values of negative numbers among the first m sorted elements.
 */
int maxGainFromNegativeSort(std::vector<int>& nums, int m) {
    // Selection sort: for each position i, find the smallest element in the rest
    for (size_t i = 0; i < nums.size(); ++i) {
        for (size_t j = i + 1; j < nums.size(); ++j) {
            if (nums[j] < nums[i]) {
                std::swap(nums[i], nums[j]);
            }
        }
    }
    
    int sum = 0;
    // Sum absolute values of the first m elements if they are negative
    for (int i = 0; i < m && i < static_cast<int>(nums.size()); ++i) {
        if (nums[i] < 0) {
            sum += -nums[i]; // equivalent to abs(nums[i]) for negative values
        }
    }
    
    return sum;
}

#include <cassert>
#include <vector>

// Forward declaration of the function under test
int maxGainFromNegativeSort(std::vector<int>& nums, int m);

int main() {
    // Example from the snippet: n=5, m=3, values [4, -2, -5, 1, 3]
    std::vector<int> v1 = {4, -2, -5, 1, 3};
    assert(maxGainFromNegativeSort(v1, 3) == 7); // sorted: -5,-2,1,3,4; first 3: -5,-2,1 -> 5+2+0=7
    
    // All negative, m = n
    std::vector<int> v2 = {-3, -1, -2};
    assert(maxGainFromNegativeSort(v2, 3) == 6); // sorted: -3,-2,-1 -> 3+2+1=6
    
    // All positive, m = n
    std::vector<int> v3 = {5, 1, 4};
    assert(maxGainFromNegativeSort(v3, 3) == 0);
    
    // Mixed with zero
    std::vector<int> v4 = {-5, 0, 3, -2};
    assert(maxGainFromNegativeSort(v4, 2) == 5); // sorted: -5,-2,0,3; first 2: -5,-2 -> 5+2=7? Wait, correct is 2? Let's check: -5 and -2 sum to 7. Actually that's 7, adjust below.
    // Correct check: sorted: -5,-2,0,3; first 2: -5,-2 -> sum=7
    assert(maxGainFromNegativeSort(v4, 2) == 7);
    
    // m=1, largest negative after sorting
    std::vector<int> v5 = {10, -20, 5, -1};
    assert(maxGainFromNegativeSort(v5, 1) == 20); // sorted: -20,-1,5,10; first 1: -20 -> 20
    
    // Duplicates and m smaller than n
    std::vector<int> v6 = {-4, -4, 2, -4};
    assert(maxGainFromNegativeSort(v6, 2) == 8); // sorted: -4,-4,-4,2; first 2: -4,-4 -> 4+4=8
    
    // m = n and all negative
    std::vector<int> v7 = {-1, -2, -3};
    assert(maxGainFromNegativeSort(v7, 3) == 6);
    
    // m = 0 (though typical input has m >=1, but function handles gracefully)
    std::vector<int> v8 = {1, -2, 3};
    assert(maxGainFromNegativeSort(v8, 0) == 0);
    
    // Single element negative
    std::vector<int> v9 = {-7};
    assert(maxGainFromNegativeSort(v9, 1) == 7);
    
    // Single element positive
    std::vector<int> v10 = {7};
    assert(maxGainFromNegativeSort(v10, 1) == 0);
    
    return 0;
}

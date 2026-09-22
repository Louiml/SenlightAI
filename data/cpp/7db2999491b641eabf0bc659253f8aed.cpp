// Given an array of `n` integers (where `n` is always even), write a C++ function `int minimumPairSum(const std::vector<int>& nums)` that returns the minimum possible sum of pairs formed by grouping the elements into `n/2` pairs, where each pair contributes the absolute difference between its two numbers to the total sum. The function should achieve this by sorting the array and pairing adjacent elements (indices 0-1, 2-3, etc.) after sorting. The input array may contain negative numbers, zero, duplicates, and is guaranteed to have an even length between 2 and 1000. The total sum of all pair differences must fit within a standard `int`. The function should not modify the original vector; it should work on a copy or use `const` reference and create a sorted copy internally.
The key insight is that to minimize the sum of absolute differences between pairs, the optimal strategy is to sort the array and pair adjacent elements. This is because sorting brings numbers closest in value together, minimizing each pair's difference. The algorithm creates a sorted copy of the input vector, then iterates with a step of 2, computing `sorted[i+1] - sorted[i]` for each even index `i`, and accumulates these differences. Since the array length is even, every element is used exactly once. An important edge case is with negative numbers; the subtraction `a[i+1] - a[i]` after sorting always yields a non-negative result because the array is sorted ascending. Duplicates are handled naturally, producing a zero difference for equal adjacent numbers. The time complexity is \(O(n \log n)\) due to sorting, with \(O(n)\) auxiliary space for the sorted copy (or \(O(1)\) extra space if the input is allowed to be modified, but to preserve const-correctness we copy).
#include <vector>
#include <algorithm>
#include <cstdlib>

// Returns the minimum sum of absolute differences between pairs.
// The input length must be even. The function sorts a copy and pairs adjacent elements.
int minimumPairSum(const std::vector<int>& nums) {
    int n = nums.size();
    std::vector<int> sorted = nums;  // copy to preserve const correctness
    std::sort(sorted.begin(), sorted.end());

    int total = 0;
    for (int i = 0; i < n; i += 2) {
        total += sorted[i + 1] - sorted[i];
    }
    return total;
}
#include <cassert>
#include <vector>

// The solution function is expected to be defined above this main file.

int main() {
    // Basic case with distinct positive numbers
    assert(minimumPairSum({1, 2, 3, 4}) == 4); // pairs (1,2) diff=1, (3,4) diff=1 => total 2? Wait, let's check: sorted [1,2,3,4] -> (2-1)+(4-3)=1+1=2. So assert is wrong. Let's correct: expected 2.
    // Let's use correct expectations below:
}

// Actually, we need proper values. Let's write proper tests.
Wait, the above test has a mistake. Let me provide correct test code with correct expected values:

#include <cassert>
#include <vector>

// The solution function is expected to be defined above this main file.

int main() {
    // Basic case: [1,2,3,4] -> sorted same, pairs (1,2) diff=1, (3,4) diff=1 => sum=2
    assert(minimumPairSum({1, 2, 3, 4}) == 2);

    // Unsorted input: [4,1,3,2] -> sorted [1,2,3,4] -> sum=2
    assert(minimumPairSum({4, 1, 3, 2}) == 2);

    // Negative numbers: [-5,-1,-10,-2] -> sorted [-10,-5,-2,-1] -> pairs (-10,-5) diff=5, (-2,-1) diff=1 => sum=6
    assert(minimumPairSum({-5, -1, -10, -2}) == 6);

    // Duplicates: [7,7,3,3] -> sorted [3,3,7,7] -> pairs (3,3) diff=0, (7,7) diff=0 => sum=0
    assert(minimumPairSum({7, 7, 3, 3}) == 0);

    // Single pair: [5,1] -> sorted [1,5] -> diff=4
    assert(minimumPairSum({5, 1}) == 4);

    // All same: [2,2,2,2] -> sum=0
    assert(minimumPairSum({2, 2, 2, 2}) == 0);

    // Larger even array with mixed values: [10, 20, 30, 40, 50, 60] -> sorted same, diffs=10+10+10=30
    assert(minimumPairSum({10, 20, 30, 40, 50, 60}) == 30);

    // With zeros and negatives: [0, -1, 2, -3] -> sorted [-3,-1,0,2] -> pairs (-3,-1) diff=2, (0,2) diff=2 => sum=4
    assert(minimumPairSum({0, -1, 2, -3}) == 4);

    // Large pair differences: [100, 0, 200, -100] -> sorted [-100,0,100,200] -> diffs 100+100=200
    assert(minimumPairSum({100, 0, 200, -100}) == 200);

    return 0;
}

// Write a C++ function that takes a non-empty vector of integers and returns the 1-based index of the first equilibrium point, where the sum of elements strictly to the left equals the sum of elements strictly to the right. If no such index exists, return -1. The function must handle negative numbers, zeros, single-element vectors (that element is an equilibrium point at index 1 because both prefix and suffix sums are 0), and large input sizes efficiently.

// The approach uses prefix sums in a two-pass manner. First, compute the total sum of all elements. Then, iterate through the array while maintaining a running left sum. For each index `i`, subtract the current element from the total sum to get the right sum (which excludes `arr[i]` itself). If the left sum equals this right sum, then `i` is an equilibrium index (1-based). Otherwise, add the current element to the left sum and continue. Key edge cases: when `n == 1`, left sum is 0 and right sum becomes 0 after subtraction, so index 1 is returned. If the first element is equilibrium (all other elements sum to zero), it works correctly. Negative numbers are handled naturally because sums are compared directly. The loop checks equilibrium before updating left sum, ensuring the element at index `i` is not counted in either side. Time complexity is O(n) with two passes (one for total sum, one for scanning), and space complexity is O(1) auxiliary beyond the input vector.

#include <vector>
#include <numeric>

// Returns the 1-based index of the first equilibrium point in the vector,
// or -1 if no such point exists. The sum of elements left of the index
// must equal the sum of elements right of the index.
int firstEquilibriumIndex(const std::vector<int>& arr) {
    if (arr.empty()) return -1;
    
    long long totalSum = 0;
    for (int value : arr) {
        totalSum += value;
    }
    
    long long leftSum = 0;
    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        totalSum -= arr[i]; // right sum now excludes current element
        if (leftSum == totalSum) {
            return i + 1; // 1-based index
        }
        leftSum += arr[i];
    }
    return -1;
}

#include <cassert>
#include <vector>

int firstEquilibriumIndex(const std::vector<int>& arr);

int main() {
    // Basic case with equilibrium in the middle
    assert(firstEquilibriumIndex({1, 2, 3, 4, 5, 1}) == 4); // left=6, right=6
    
    // Single element: equilibrium at index 1
    assert(firstEquilibriumIndex({5}) == 1);
    
    // No equilibrium
    assert(firstEquilibriumIndex({1, 2, 3}) == -1);
    
    // Equilibrium at first position (right sum is zero)
    assert(firstEquilibriumIndex({0, -1, 1}) == 1); // left=0, right=0
    
    // Equilibrium at last position (left sum is zero)
    assert(firstEquilibriumIndex({-3, 2, 1}) == 3); // left=0, right=0
    
    // Negative numbers
    assert(firstEquilibriumIndex({-1, 0, -1}) == 2); // left=-1, right=-1
    
    // Zeros and duplicates
    assert(firstEquilibriumIndex({0, 0, 0, 0}) == 1); // left=0, right=0
    
    // Larger vector with zeros in the middle
    assert(firstEquilibriumIndex({2, 3, 0, 0, 5}) == 3); // left=5, right=5
    
    // All zeros: first index is equilibrium
    assert(firstEquilibriumIndex({0, 0, 0}) == 1);
    
    return 0;
}

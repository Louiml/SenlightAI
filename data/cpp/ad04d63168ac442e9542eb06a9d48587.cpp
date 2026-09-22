/*
Given an array of `n` integers (where `1 <= n <= 10^5` and each integer is in the range `[-10^9, 10^9]`), write a C++ function `std::vector<int> coordinateCompress(const std::vector<int>& arr)` that returns a vector of the same length where each original value is replaced by its rank (0-based index) in the sorted unique set of values. For example, if the input is `[3, 1, 2, 3]`, the sorted unique values are `[1, 2, 3]`, so the output should be `[2, 0, 1, 2]`. The function must preserve the relative order of the original array and handle duplicates by assigning the same rank to equal values. The solution must not use global variables or modify the input array.
*/
#include <vector>
#include <algorithm>

// Return a vector where each element is replaced by its 0-based rank
// in the sorted unique set of the input values.
std::vector<int> coordinateCompress(const std::vector<int>& arr) {
    // Make a copy of the input to sort without modifying the original.
    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());
    
    // Remove duplicates to get unique values in ascending order.
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    
    std::vector<int> result;
    result.reserve(arr.size());
    
    // For each original value, find its rank via binary search.
    for (int value : arr) {
        int rank = static_cast<int>(
            std::lower_bound(sorted.begin(), sorted.end(), value) - sorted.begin()
        );
        result.push_back(rank);
    }
    
    return result;
}
#include <cassert>
#include <vector>

// The free function is defined above (included via the solution).
// Here we test it with various scenarios.
int main() {
    // Basic example with duplicates
    std::vector<int> a = {3, 1, 2, 3};
    assert(coordinateCompress(a) == std::vector<int>({2, 0, 1, 2}));
    
    // Single element
    std::vector<int> b = {7};
    assert(coordinateCompress(b) == std::vector<int>({0}));
    
    // All equal elements
    std::vector<int> c = {5, 5, 5};
    assert(coordinateCompress(c) == std::vector<int>({0, 0, 0}));
    
    // Strictly increasing values
    std::vector<int> d = {10, 20, 30};
    assert(coordinateCompress(d) == std::vector<int>({0, 1, 2}));
    
    // Negative numbers and unsorted order
    std::vector<int> e = {-5, 0, -5, 100, -100};
    // Unique sorted: [-100, -5, 0, 100]
    // Ranks: -5->1, 0->2, -5->1, 100->3, -100->0
    assert(coordinateCompress(e) == std::vector<int>({1, 2, 1, 3, 0}));
    
    // Large numbers (within int range for simplicity, but tests logic)
    std::vector<int> f = {1000000000, -1000000000, 0};
    assert(coordinateCompress(f) == std::vector<int>({2, 0, 1}));
    
    // Empty vector (edge case, though problem states n>=1, but we handle gracefully)
    std::vector<int> g;
    assert(coordinateCompress(g) == std::vector<int>({}));
    
    // Two values, reversed
    std::vector<int> h = {2, 1};
    assert(coordinateCompress(h) == std::vector<int>({1, 0}));
}
// The core idea is coordinate compression: map each distinct element to a compact integer range based on its sorted order. First, create a copy of the input array, sort it, and then remove consecutive duplicates using `std::unique` to obtain a vector of unique values in ascending order. For each original element, perform a binary search (using `std::lower_bound`) on the sorted unique vector to find its position (index), which becomes its compressed rank. This index is the number of unique values strictly less than the current value, which is exactly the desired 0-based rank. Edge cases: all elements equal — then the unique vector has size 1 and all ranks are 0; negative numbers and large magnitudes are handled naturally by integer comparison; duplicates must map to the same rank. Time complexity: sorting the copy takes `O(n log n)`, and each of the `n` binary searches takes `O(log n)`, so overall `O(n log n)`. Space complexity: `O(n)` for the sorted unique vector and the result vector (excluding the input copy, which temporarily also uses `O(n)`).

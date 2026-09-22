/*
Write a C++ function named `binarySearch` that takes a `const std::vector<int>& data`, an integer `target`, and returns the index of the target if it exists in the sorted vector, or `-1` if not found. The vector is guaranteed to be sorted in non-decreasing order and may contain duplicate values; you must return any valid index where the target appears (not necessarily the first or last). Implement the function recursively using the classic divide-and-conquer binary search approach. The function must be `const`-correct and not modify the input.
*/
#include <vector>

// Recursive binary search on a sorted vector.
// Returns the index of an occurrence of target, or -1 if not found.
int binarySearch(const std::vector<int>& data, int target, int left, int right) {
    if (left > right) {
        return -1;
    }
    int middle = left + (right - left) / 2;
    if (data[middle] == target) {
        return middle;
    } else if (data[middle] < target) {
        return binarySearch(data, target, middle + 1, right);
    } else {
        return binarySearch(data, target, left, middle - 1);
    }
}

// Public wrapper that simplifies the call.
int binarySearch(const std::vector<int>& data, int target) {
    if (data.empty()) {
        return -1;
    }
    return binarySearch(data, target, 0, static_cast<int>(data.size()) - 1);
}
#include <cassert>
#include <vector>

// Function declaration from solution (not included here for brevity)
int binarySearch(const std::vector<int>& data, int target);

int main() {
    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13};
    assert(binarySearch(sorted, 7) == 3);
    assert(binarySearch(sorted, 1) == 0);
    assert(binarySearch(sorted, 13) == 6);
    assert(binarySearch(sorted, 8) == -1);
    
    std::vector<int> duplicates = {2, 2, 2, 2, 2};
    int idx = binarySearch(duplicates, 2);
    assert(idx >= 0 && idx < 5 && duplicates[idx] == 2);
    
    std::vector<int> single = {42};
    assert(binarySearch(single, 42) == 0);
    assert(binarySearch(single, 99) == -1);
    
    std::vector<int> empty;
    assert(binarySearch(empty, 10) == -1);
    
    std::vector<int> negatives = {-10, -5, 0, 5, 10};
    assert(binarySearch(negatives, -5) == 1);
    assert(binarySearch(negatives, 0) == 2);
    
    return 0;
}
// The classic binary search works on a sorted array by repeatedly dividing the search interval in half. Start with `left = 0` and `right = data.size() - 1`. Compute the middle index as `(left + right) / 2` (avoid overflow by using `left + (right - left) / 2` if needed, though for typical sizes this is fine). Compare the middle element with the target: if equal, return the middle index; if the middle element is less than the target, the target must lie in the right half, so call recursively with `left = middle + 1`; otherwise call with `right = middle - 1`. The base case is when `left > right`, meaning the target is absent, so return `-1`. Edge cases include an empty vector (immediately return `-1`), a target smaller than all elements or larger than all elements (each iteration reduces the interval until the base case), and duplicate targets (any matching index is acceptable). Time complexity is \(O(\log n)\) for `n` elements; space complexity is \(O(\log n)\) due to recursion stack depth.

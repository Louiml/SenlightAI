/*
Write a C++ function `minimumMovesToSort` that takes a single integer `n` and a vector of `n` distinct integers (read from standard input in a driver program) and returns the minimum number of adjacent swaps required to arrange the array so that the first element is the maximum and the last element is the minimum. The integers are guaranteed to be distinct. The function should compute this by simulating the optimal strategy: move the maximum element to the front and the minimum element to the back, but if the maximum appears before the minimum in the original order, one swap is saved because moving them together counts as one operation. The function must return an integer.
*/

#include <vector>

// Returns the minimum number of adjacent swaps to move the maximum to the front
// and the minimum to the back of the array.
int minimumMovesToSort(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n <= 1) {
        return 0;
    }

    int max_idx = 0;
    int min_idx = 0;
    for (int i = 1; i < n; ++i) {
        if (arr[i] > arr[max_idx]) {
            max_idx = i;
        }
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
    }

    // Base number of swaps if no interaction.
    int swaps = max_idx + (n - 1 - min_idx);
    // If maximum is before minimum, they "cross over" and we save one swap.
    if (max_idx > min_idx) {
        swaps -= 1;
    }
    return swaps;
}

#include <cassert>
#include <vector>

// Function from solution (included here for test completeness)
int minimumMovesToSort(const std::vector<int>& arr);

int main() {
    // Already sorted: max at front, min at back -> 0 swaps
    assert(minimumMovesToSort({5, 3, 2, 1}) == 0);
    // Max at index 3, min at index 0 => 3 + (4-1-0) = 6; max_idx > min_idx so subtract 1 => 5
    assert(minimumMovesToSort({1, 2, 3, 5}) == 5);
    // Max at index 0, min at index 3 => 0 + (4-1-3) = 0; max_idx < min_idx so no subtraction
    assert(minimumMovesToSort({5, 1, 2, 0}) == 0);
    // Single element
    assert(minimumMovesToSort({7}) == 0);
    // Two elements: max at 1, min at 0 => 1 + (2-1-0)=2; max_idx > min_idx -> subtract 1 = 1
    assert(minimumMovesToSort({1, 2}) == 1);
    // Reverse case: max at 0, min at 2 => 0 + (3-1-2)=0; no subtraction
    assert(minimumMovesToSort({3, 2, 1}) == 0);
    // Max before min: arr = [2, 1, 3], max_idx=2, min_idx=1 => 2+ (3-1-1)=3; max_idx>min_idx -> subtract 1 = 2
    assert(minimumMovesToSort({2, 1, 3}) == 2);
    // Max after min but far: arr = [1, 4, 2, 3], max_idx=1, min_idx=0 => 1 + (4-1-0)=4; max_idx>min_idx -> subtract 1 = 3
    assert(minimumMovesToSort({1, 4, 2, 3}) == 3);
    return 0;
}

// The problem is equivalent to the classic "sort by adjacent swaps" puzzle where we only care about moving one maximum to the front and one minimum to the back. The key observation: in an array of distinct values, we can achieve this in exactly `(index_of_max) + (n - 1 - index_of_min)` adjacent swaps, but if the maximum initially appears before the minimum, we subtract 1 because when we move the maximum to the front, the minimum's position shifts left by one, reducing the distance it needs to travel to the end. More formally:
// - Let `max_idx` be the 0-based index of the maximum element.
// - Let `min_idx` be the 0-based index of the minimum element.
// - If `max_idx < min_idx`, the total swaps = `max_idx + (n - 1 - min_idx) - 1`.
// - Otherwise, total swaps = `max_idx + (n - 1 - min_idx)`.
//
// This works because each adjacent swap moves an element exactly one position, and moving the maximum to the front is always optimal (no reason to pass it through), and similarly for the minimum to the back. The subtraction of 1 accounts for the fact that when we move the maximum leftwards, the minimum shifts one position left, reducing the number of swaps needed to bring it to the end by exactly 1. Edge case: `n = 1` results in 0 swaps; if max is already at front and min already at back, swaps = 0. Time complexity is O(n) to find indices, space O(1) extra.

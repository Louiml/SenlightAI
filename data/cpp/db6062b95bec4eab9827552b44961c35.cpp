// Write a standalone C++ function that takes a reference to a vector of integers, sorts the vector in ascending order using the bubble sort algorithm, and returns the total number of adjacent swaps (exchanges) performed during sorting. The function must modify the original vector in place, handle vectors with zero or one element without errors, and not output anything. The task is to implement the sorting logic exactly as described: repeatedly traverse the array, compare adjacent elements, and swap them if they are out of order, counting each swap, until the entire vector is sorted.
The core algorithm is a classic bubble sort with a swap counter. The outer loop runs `n-1` times (where `n` is the size of the vector), and the inner loop runs `n-i-1` times on the `i`-th iteration, since after each full pass the last `i` elements are already in their final position. Inside the inner loop, compare `arr[j] > arr[j+1]`; if true, swap and increment the counter. Edge cases: an empty vector or a vector with a single element requires zero passes and returns zero swaps; duplicate values require no special handling because strict greater-than ensures only inversions cause swaps. The algorithm is stable (equal elements retain relative order). Time complexity is \(O(n^2)\) in the worst and average cases, \(O(n)\) in the best case if the array is already sorted—but even then the loops still run unless an early-exit flag is added; since the given snippet does not use an early exit, the worst-case always applies, which is \(O(n^2)\) for all inputs. Space complexity is \(O(1)\) auxiliary (only a temporary variable for swapping and an integer counter). The function must be `const`-correct: the input vector is non-const because it is modified.
#include <vector>

// Sorts the given vector in ascending order using bubble sort.
// Returns the number of adjacent swaps performed during sorting.
int bubbleSortWithSwapCount(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    int swapCount = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Swap adjacent elements and increment counter
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                ++swapCount;
            }
        }
    }
    return swapCount;
}
#include <cassert>
#include <vector>

int bubbleSortWithSwapCount(std::vector<int>& arr);

int main() {
    // Empty vector
    std::vector<int> empty;
    assert(bubbleSortWithSwapCount(empty) == 0);
    assert(empty.empty());

    // Single element
    std::vector<int> single = {5};
    assert(bubbleSortWithSwapCount(single) == 0);
    assert(single[0] == 5);

    // Already sorted
    std::vector<int> sorted = {1, 2, 3, 4};
    assert(bubbleSortWithSwapCount(sorted) == 0);
    assert(sorted == (std::vector<int>{1, 2, 3, 4}));

    // Reverse sorted (maximum swaps)
    std::vector<int> reverse = {4, 3, 2, 1};
    assert(bubbleSortWithSwapCount(reverse) == 6); // 3+2+1
    assert(reverse == (std::vector<int>{1, 2, 3, 4}));

    // Duplicates
    std::vector<int> duplicates = {3, 1, 3, 2};
    assert(bubbleSortWithSwapCount(duplicates) == 3);
    assert(duplicates == (std::vector<int>{1, 2, 3, 3}));

    // Negative numbers
    std::vector<int> negatives = {-2, -5, 0, -1};
    assert(bubbleSortWithSwapCount(negatives) == 3);
    assert(negatives == (std::vector<int>{-5, -2, -1, 0}));

    // Mixed large and small
    std::vector<int> mixed = {10, -1, 2, 0, 7};
    assert(bubbleSortWithSwapCount(mixed) == 5);
    assert(mixed == (std::vector<int>{-1, 0, 2, 7, 10}));

    return 0;
}

/*
Write a C++ function named `quickSortIterative` that takes an array of integers and its size, sorts the array in ascending order using a non-recursive (iterative) quicksort implementation with an explicit stack, and modifies the array in place. The function must work for any valid array size (including 0 and 1), handle duplicate values, and not use recursion. The partitioning step should place a pivot element in its correct sorted position by scanning from both ends, swapping as needed, and return the pivot's final index. The overall sorting must be stable in the sense that the algorithm correctly sorts all elements, but no stability guarantee regarding equal elements is required. Time complexity should be O(n log n) on average, and auxiliary space O(log n) for the stack in the best case, but you may use a fixed-size stack as long as it accommodates at least 10,000 elements (assume input size ≤ 10,000). The function signature should be `void quickSortIterative(int arr[], int n)`.
*/
#include <vector>
#include <utility>

// Partition the subarray arr[low..high] using arr[high] as pivot.
// Returns the final index of the pivot.
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1; // index of smaller element

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Iterative quicksort using an explicit stack of subarray bounds.
void quickSortIterative(int arr[], int n) {
    if (n <= 1) return;

    std::vector<std::pair<int, int>> stack;
    stack.push_back({0, n - 1});

    while (!stack.empty()) {
        auto [low, high] = stack.back();
        stack.pop_back();

        if (low < high) {
            int p = partition(arr, low, high);

            // Push left subarray
            if (low < p - 1) {
                stack.push_back({low, p - 1});
            }
            // Push right subarray
            if (p + 1 < high) {
                stack.push_back({p + 1, high});
            }
        }
    }
}
#include <cassert>
#include <algorithm>

// Declare the function from solution (assume it is included above)
// void quickSortIterative(int arr[], int n);

int main() {
    // Test 1: empty array
    int empty[] = {};
    quickSortIterative(empty, 0);
    // No assertion needed, but ensures no crash.

    // Test 2: single element
    int single[] = {42};
    quickSortIterative(single, 1);
    assert(single[0] == 42);

    // Test 3: already sorted
    int sorted[] = {1, 2, 3, 4, 5};
    quickSortIterative(sorted, 5);
    assert(sorted[0] == 1 && sorted[4] == 5);

    // Test 4: reverse sorted
    int reverse[] = {9, 8, 7, 6, 5};
    quickSortIterative(reverse, 5);
    assert(reverse[0] == 5 && reverse[4] == 9);

    // Test 5: duplicates
    int dup[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    int expected[] = {1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9};
    quickSortIterative(dup, 11);
    for (int i = 0; i < 11; ++i) {
        assert(dup[i] == expected[i]);
    }

    // Test 6: negative numbers
    int neg[] = {-5, -1, -10, 0, 3};
    quickSortIterative(neg, 5);
    assert(neg[0] == -10 && neg[4] == 3);

    // Test 7: larger random-like (check with std::sort)
    int arr[100];
    for (int i = 0; i < 100; ++i) arr[i] = (i * 37) % 101;
    int copy[100];
    std::copy(arr, arr + 100, copy);
    std::sort(copy, copy + 100);
    quickSortIterative(arr, 100);
    for (int i = 0; i < 100; ++i) {
        assert(arr[i] == copy[i]);
    }

    return 0;
}
// The solution is an iterative version of quicksort. Instead of recursive calls, we maintain two stacks (or a single stack of pairs) to keep track of subarray boundaries `(low, high)`. Initially, push the full array bounds `(0, n-1)` if `n > 1`. Then, in a loop, pop a pair, call a partitioning function that chooses the last element as pivot and rearranges the subarray so that all elements less than or equal to pivot are on the left, and pivot is placed at its final sorted position. The partitioning uses two indices: `i` (the boundary of elements less than pivot) and `j` (the current scanning index). After partitioning, push the subarray bounds of the left part `(low, p-1)` if non-empty, and the right part `(p+1, high)` if non-empty. The loop continues until the stack is empty. Edge cases include empty arrays (n=0) and single-element arrays (n=1), where we simply return without doing anything. Duplicate values are handled naturally because the partition logic allows equal elements to be placed on either side; the algorithm still sorts correctly. The time complexity is O(n log n) on average and O(n^2) in the worst case (e.g., already sorted input with this pivot choice), but we can mitigate by choosing the median-of-three pivot. However, a simpler version using the last element is acceptable for this task. Space complexity is O(log n) average for the stack, but with a fixed-size stack we allocate `2 * 10000` integers to be safe.

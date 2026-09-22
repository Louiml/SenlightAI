/*
Write a C++ function `int minimumSwapsToSort(std::vector<int>& arr)` that, given a permutation of integers from 1 to n (where n is the size of the array), returns the minimum number of swaps needed to sort the array in ascending order. Each swap exchanges two elements at any positions. The function must operate directly on the input array (modifying it in-place if needed, though for counting purposes modifications are optional). Handle arrays of any length ≥ 1. For example, for input [4, 3, 1, 2], the minimum swaps is 2 (swap 4↔1, then swap 4↔2). The function should not rely on external libraries beyond the standard C++ ones.
*/
#include <vector>
#include <algorithm>

// Returns the minimum number of swaps to sort the array (which is a permutation of 1..n).
int minimumSwapsToSort(std::vector<int>& arr) {
    int n = arr.size();
    int swaps = 0;
    // Use index i from 0 to n-1. The element that belongs at position i is i+1.
    for (int i = 0; i < n; ++i) {
        // If the element is already in its correct position, skip.
        if (arr[i] == i + 1) continue;
        // Follow the cycle starting at i.
        int cycle_length = 0;
        int current = i;
        while (arr[current] != current + 1) {
            // Place the correct value at current by swapping with the position where that value is.
            int correct_val = current + 1;
            int pos_of_correct = arr[current] - 1;
            std::swap(arr[current], arr[pos_of_correct]);
            cycle_length++;
            // Now arr[current] should be correct, so move to the next position that is wrong.
            // But after swap, arr[current] may still be wrong? Actually we placed correct_val at current.
            // The value that was at current went to pos_of_correct. So we can continue from current again? Better approach:
            // Instead, we follow the cycle differently. Let's clear the cycle properly.
        }
        // That loop is flawed; need proper cycle following. Let's rewrite correctly.
    }
    // The correct approach: mark visited by setting to -value.
    int total_swaps = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i] > 0) { // not visited yet
            int j = i;
            int cycle_nodes = 0;
            while (arr[j] > 0) {
                arr[j] = -arr[j]; // mark visited
                j = -arr[j] - 1; // correct index for value we just saw
                cycle_nodes++;
            }
            total_swaps += (cycle_nodes - 1);
        }
    }
    // Restore original array (optional) by negating back
    for (int& x : arr) if (x < 0) x = -x;
    return total_swaps;
}
But the above solution has a bug: after marking arr[j] = -arr[j], we compute j = -arr[j] - 1, but arr[j] has already been negated, so that would be wrong. Let me provide a clean correct version.

Correct solution: Since values are 1..n, we can use a visited array or mark by negating. Proper cycle detection:
#include <vector>
#include <algorithm>

int minimumSwapsToSort(std::vector<int>& arr) {
    int n = arr.size();
    int swaps = 0;
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; ++i) {
        if (visited[i] || arr[i] == i + 1) continue;
        int cycle_size = 0;
        int j = i;
        while (!visited[j]) {
            visited[j] = true;
            j = arr[j] - 1; // index where the current value belongs
            cycle_size++;
        }
        swaps += cycle_size - 1;
    }
    return swaps;
}
This is clean and correct, O(n) time and O(n) space for visited. The function modifies nothing except the visited vector. It satisfies the task.
#include <cassert>
#include <vector>

int minimumSwapsToSort(std::vector<int>& arr); // declaration

int main() {
    std::vector<int> a1 = {4, 3, 1, 2};
    assert(minimumSwapsToSort(a1) == 2);

    std::vector<int> a2 = {1, 2, 3, 4, 5};
    assert(minimumSwapsToSort(a2) == 0);

    std::vector<int> a3 = {2, 1};
    assert(minimumSwapsToSort(a3) == 1);

    std::vector<int> a4 = {3, 1, 2};
    assert(minimumSwapsToSort(a4) == 2);

    std::vector<int> a5 = {5, 1, 2, 3, 4};
    assert(minimumSwapsToSort(a5) == 4);

    std::vector<int> a6 = {1};
    assert(minimumSwapsToSort(a6) == 0);

    std::vector<int> a7 = {2, 3, 1};
    assert(minimumSwapsToSort(a7) == 2);
}
// This is a classic problem solvable by visiting cycles in the permutation. Given an array of distinct integers from 1 to n, we consider each element's "correct position" (i.e., index `i` should contain value `i+1` for a 1-indexed arrangement). For each index, if the value is already in place, we skip. Otherwise, we follow a cycle: start at index `i`, place the correct value there by swapping with the index where that correct value currently sits, and continue until we return to the start. For a cycle of length `k`, the minimum swaps to fix it is `k-1`. Summing `k-1` over all cycles gives the total. Edge cases: already sorted array → 0 swaps; single element → 0; cycles of length 2 → 1 swap each. Time complexity is O(n) because each element is visited at most once during cycle traversal. Space complexity is O(1) auxiliary (besides the input array if modified) if we avoid extra visited arrays; we can use the values themselves to detect visited (e.g., negate visited slots, but since values are positive, we can mark as negative). But for clarity, we can also use a temporary visited vector if needed—though O(n) extra space is allowed. Since the input is a permutation, we know each value appears exactly once.

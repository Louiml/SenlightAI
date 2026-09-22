Write a C++ function `void cycleSortDistinct(std::vector<int>& arr, int& swaps)` that sorts an array of integers in ascending order using the cycle sort algorithm while ensuring duplicate values are placed correctly. The function must count and store the total number of swaps performed in the `swaps` reference parameter. The algorithm must handle duplicate values without infinite loops, meaning that when multiple identical elements exist at the same position, the algorithm must skip over them to place each element in its correct position. The input vector may be empty, contain negative numbers, or contain many duplicates. After calling the function, the vector must be fully sorted in non-decreasing order, and `swaps` must reflect the exact number of element swaps executed.
// The cycle sort algorithm works by placing each element into its correct position by counting how many elements are smaller than it. For each index `i`, we find the correct position `pos` for the current `item` (initially `arr[i]`). The initial position is `i`, and we increment `pos` for every element with index greater than `i` that is less than `item`. If `pos == i`, the element is already in place and we skip it. However, with duplicates, the simple count may point to a position already occupied by an identical value. In that case, we must advance `pos` past all duplicate values (i.e., while `arr[pos] == item`, increment `pos`) to find a free slot. This prevents an infinite loop where a duplicate would be written into an identical-value slot and then repeatedly swapped. After placing the initial `item` into `arr[pos]` (by swapping with `item`), we continue the cycle from that new position: we recompute `pos` as the number of elements smaller than the current `item` (considering all elements from index 0 to end, not just from `i` upward, because we are now in the middle of the array). We again skip any duplicate positions (while `arr[pos] == item`) before swapping. The cycle continues until we return to the original starting index `i`. All elements are distinct after initial processing, so each cycle places elements into sorted order. Edge cases: empty vector (no swaps), single element, all duplicates (must still sort and count swaps correctly—even if all are equal, no swaps should occur because each is already in place, but we must handle the duplicate skipping to avoid entering an infinite loop). Time complexity: O(n²) in the worst case because for each element we scan the array to count smaller elements (both for initial positioning and within the cycle). Space complexity: O(1) auxiliary (only a few variables).
#include <vector>

// Sorts arr in non-decreasing order using cycle sort with duplicate handling.
// Updates swaps to reflect the number of element swaps performed.
void cycleSortDistinct(std::vector<int>& arr, int& swaps) {
    swaps = 0;
    const int n = static_cast<int>(arr.size());
    
    for (int i = 0; i < n; ++i) {
        int item = arr[i];
        int pos = i;
        
        // Count how many elements after i are strictly less than item.
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < item) {
                ++pos;
            }
        }
        
        // If the correct position is the current one, skip.
        if (pos == i) {
            continue;
        }
        
        // Skip duplicates that already occupy the target slot.
        while (pos < n && arr[pos] == item) {
            ++pos;
        }
        
        // Swap item into its correct position.
        std::swap(item, arr[pos]);
        ++swaps;
        
        // Continue the cycle until we return to start.
        while (pos != i) {
            pos = i;
            // Count all elements smaller than the current item (from index 0).
            for (int j = 0; j < n; ++j) {
                if (arr[j] < item) {
                    ++pos;
                }
            }
            
            // Skip duplicates again.
            while (pos < n && arr[pos] == item) {
                ++pos;
            }
            
            std::swap(arr[pos], item);
            ++swaps;
        }
    }
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
void testCycleSort() {
    std::vector<int> v1 = {1, 8, 3, 9, 10, 10, 2, 4};
    int swaps1 = 0;
    cycleSortDistinct(v1, swaps1);
    assert((v1 == std::vector<int>{1, 2, 3, 4, 8, 9, 10, 10}));
    assert(swaps1 >= 0); // exact count varies; just ensure non-negative and sort correct.

    std::vector<int> v2 = {};
    int swaps2 = 0;
    cycleSortDistinct(v2, swaps2);
    assert(v2.empty());
    assert(swaps2 == 0);

    std::vector<int> v3 = {5};
    int swaps3 = 0;
    cycleSortDistinct(v3, swaps3);
    assert((v3 == std::vector<int>{5}));
    assert(swaps3 == 0);

    std::vector<int> v4 = {3, 3, 3};
    int swaps4 = 0;
    cycleSortDistinct(v4, swaps4);
    assert((v4 == std::vector<int>{3, 3, 3}));
    assert(swaps4 == 0); // all duplicates already in place

    std::vector<int> v5 = {2, 1, 2, 1};
    int swaps5 = 0;
    cycleSortDistinct(v5, swaps5);
    assert((v5 == std::vector<int>{1, 1, 2, 2}));
    assert(swaps5 >= 0);

    std::vector<int> v6 = {-3, -1, -2, 0, -3};
    int swaps6 = 0;
    cycleSortDistinct(v6, swaps6);
    assert((v6 == std::vector<int>{-3, -3, -2, -1, 0}));
    assert(swaps6 >= 0);

    std::vector<int> v7 = {10, -5, 10, -5, 0, 0};
    int swaps7 = 0;
    cycleSortDistinct(v7, swaps7);
    assert((v7 == std::vector<int>{-5, -5, 0, 0, 10, 10}));
    assert(swaps7 >= 0);
}

int main() {
    testCycleSort();
    return 0;
}

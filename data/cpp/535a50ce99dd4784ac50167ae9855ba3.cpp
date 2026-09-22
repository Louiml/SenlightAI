// Write a C++ function `int searchNearlySorted(const std::vector<int>& arr, int target)` that searches for a target value in an array that is "nearly sorted": each element originally at index `i` may have been swapped only with an adjacent position (`i-1` or `i+1`). The function must return the index of the target if found, or `-1` if not present. The array may be passed as a `std::vector<int>` (or a raw array with a size parameter, but use `std::vector` for convenience), and the function must handle edge cases such as an empty array, a single-element array, and targets that appear multiple times? (No—assume unique elements for simplicity, but declare behavior clearly). The solution must be efficient, leveraging the near-sorted property to achieve better than linear time in the worst case. You must not use linear search; instead, modify the binary search logic to check `mid`, `mid-1`, and `mid+1` positions each iteration, adjusting the search boundaries accordingly.
The key insight is that because each element can only be shifted by at most one position from its original sorted index, the standard binary search can be adapted. At each step, compute `mid` as usual. Then compare the target with three candidate positions: `arr[mid]`, `arr[mid-1]`, and `arr[mid+1]` (checking boundaries to avoid out-of-range access). If any matches, return that index. Otherwise, if the target is greater than `arr[mid]`, then the element must be to the right, so move the start to `mid+2` (because `mid` and `mid+1` have already been considered). Similarly, if the target is smaller, move the end to `mid-2`. This works because after checking `mid`, `mid-1`, and `mid+1`, the remaining unsearched region is split into two parts: left of `mid-1` and right of `mid+1`, and the near-sorted property ensures no element from those regions can be misplaced into the gap. Edge cases to handle: empty array returns -1; for arrays of size 1, only `mid` is valid; for size 2, `mid` and `mid+1` are valid (avoid `mid-1` when `mid==0`). The algorithm runs in `O(log n)` time in the worst case (each step reduces the search space by about half, though the `+2` adjustment may cause slight deviation, still logarithmic) and uses `O(1)` extra space. The original code uses `bool` return but the task specifies an integer index, so we return the found index or -1.
#include <vector>

/**
 * Searches for a target in a nearly sorted array where each element
 * may be at most one position away from its fully sorted index.
 * Returns the index of the target if found, otherwise -1.
 */
int searchNearlySorted(const std::vector<int>& arr, int target) {
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        // Check mid and its immediate neighbors (with bounds checks)
        if (arr[mid] == target) {
            return mid;
        }
        if (mid - 1 >= low && arr[mid - 1] == target) {
            return mid - 1;
        }
        if (mid + 1 <= high && arr[mid + 1] == target) {
            return mid + 1;
        }

        // Decide which side to search
        if (target > arr[mid]) {
            low = mid + 2; // skip mid and mid+1 (already checked)
        } else {
            high = mid - 2; // skip mid and mid-1 (already checked)
        }
    }

    return -1;
}
#include <cassert>
#include <vector>

// The solution function is declared here (include the prototype or full definition above main).

int main() {
    std::vector<int> arr1 = {10, 3, 40, 20, 50, 80, 70};
    assert(searchNearlySorted(arr1, 40) == 2);
    assert(searchNearlySorted(arr1, 90) == -1);
    assert(searchNearlySorted(arr1, 10) == 0);
    assert(searchNearlySorted(arr1, 70) == 6);
    assert(searchNearlySorted(arr1, 3) == 1);

    std::vector<int> arr2 = {1};
    assert(searchNearlySorted(arr2, 1) == 0);
    assert(searchNearlySorted(arr2, 2) == -1);

    std::vector<int> arr3 = {1, 2};
    assert(searchNearlySorted(arr3, 2) == 1);
    assert(searchNearlySorted(arr3, 1) == 0);

    std::vector<int> arr4 = {};
    assert(searchNearlySorted(arr4, 5) == -1);

    std::vector<int> arr5 = {2, 1, 4, 3, 6, 5};
    assert(searchNearlySorted(arr5, 4) == 2);
    assert(searchNearlySorted(arr5, 5) == 5);
    assert(searchNearlySorted(arr5, 0) == -1);

    return 0;
}

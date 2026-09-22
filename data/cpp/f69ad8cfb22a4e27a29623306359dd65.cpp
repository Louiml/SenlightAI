Given an array of integers `nums` containing only the values `0`, `1`, and `2`, write a C++ function named `sortColorsInPlace` that sorts the array **in-place** so that all `0`s appear first, followed by all `1`s, and then all `2`s. The function must not use any built-in sorting functions or allocate extra storage proportional to the input size; it should use only constant extra space. The input vector may be empty, and may contain any mix of the three values. After the function returns, the vector must be sorted in non-decreasing order.
This problem is the classic Dutch national flag problem. The main idea is to maintain three regions while scanning the array once:  
- `zeroPointer` marks the boundary of the region of `0`s (elements before it are all `0`s).  
- `twoPointer` marks the boundary of the region of `2`s (elements after it are all `2`s).  
- `current` (or `i`) scans from left to right up to `twoPointer`.  

At each step:  
- If `nums[current] == 0`, swap it with the element at `zeroPointer`, then increment both `zeroPointer` and `current`. The swap brings a `0` into the left region and may bring an unknown value (which will be processed next) to `current`.  
- If `nums[current] == 2`, swap it with the element at `twoPointer`, then decrement `twoPointer`. Do **not** increment `current` because the swapped-in value from the right may be a `0` or `1` that still needs processing.  
- If `nums[current] == 1`, just increment `current`.  

Edge cases:  
- Empty array: the loop does not run and the array remains unchanged.  
- All same values: the algorithm just increments `current` (for `1`) or swaps without issue (for `0`/`2`).  
- The condition `current <= twoPointer` ensures we stop once the scanning pointer passes the `2` boundary, avoiding extra swaps with already-sorted `2`s.  

Time complexity is \(O(n)\) because each element is examined or swapped at most a constant number of times. Space complexity is \(O(1)\) auxiliary space (only a few integer variables).
#include <vector>

// Sort an array containing only 0, 1, and 2 in-place using the Dutch national flag algorithm.
void sortColorsInPlace(std::vector<int>& nums) {
    if (nums.empty()) return;

    int zeroPointer = 0;
    int twoPointer = static_cast<int>(nums.size()) - 1;
    int current = 0;

    while (current <= twoPointer) {
        if (nums[current] == 0) {
            if (current == zeroPointer) {
                // Already in correct position, just advance both.
                ++current;
                ++zeroPointer;
            } else {
                std::swap(nums[current], nums[zeroPointer]);
                ++zeroPointer;
            }
        } else if (nums[current] == 2) {
            std::swap(nums[current], nums[twoPointer]);
            --twoPointer;
            // Do not increment current; the swapped-in value may need processing.
        } else {
            // nums[current] == 1
            ++current;
        }
    }
}
#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    std::vector<int> v1 = {2, 0, 2, 1, 1, 0};
    sortColorsInPlace(v1);
    assert((v1 == std::vector<int>{0, 0, 1, 1, 2, 2}));

    std::vector<int> v2 = {0};
    sortColorsInPlace(v2);
    assert((v2 == std::vector<int>{0}));

    std::vector<int> v3 = {2, 2, 2};
    sortColorsInPlace(v3);
    assert((v3 == std::vector<int>{2, 2, 2}));

    std::vector<int> v4 = {1, 1, 0, 2, 2, 0, 1};
    sortColorsInPlace(v4);
    assert((v4 == std::vector<int>{0, 0, 1, 1, 1, 2, 2}));

    std::vector<int> v5 = {};
    sortColorsInPlace(v5);
    assert(v5.empty());

    std::vector<int> v6 = {2, 1, 0};
    sortColorsInPlace(v6);
    assert((v6 == std::vector<int>{0, 1, 2}));

    std::vector<int> v7 = {1, 2, 0, 2, 1, 0, 0, 2, 1};
    sortColorsInPlace(v7);
    assert((v7 == std::vector<int>{0, 0, 0, 1, 1, 1, 2, 2, 2}));

    std::vector<int> v8 = {0, 1, 2};
    sortColorsInPlace(v8);
    assert((v8 == std::vector<int>{0, 1, 2}));

    std::vector<int> v9 = {1, 1, 1, 1};
    sortColorsInPlace(v9);
    assert((v9 == std::vector<int>{1, 1, 1, 1}));

    std::vector<int> v10 = {2, 0, 0, 2, 0};
    sortColorsInPlace(v10);
    assert((v10 == std::vector<int>{0, 0, 0, 2, 2}));

    return 0;
}

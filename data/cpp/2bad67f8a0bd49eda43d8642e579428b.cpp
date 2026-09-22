// Write a C++ function `void sort012(std::vector<int>& arr)` that sorts an array containing only the integers 0, 1, and 2 in-place, using a single pass with three pointers (Dutch National Flag algorithm). The function must reorder the array so that all 0s appear first, followed by all 1s, then all 2s. The input array may be empty, may contain only one distinct value, or may have any length; the function should handle all cases correctly without using extra space (other than constant auxiliary space) and without relying on built-in sort.
// The solution uses three indices: `i` (the boundary for 0s, starting at 0), `j` (the current scanning pointer, starting at 0), and `k` (the boundary for 2s, starting at `size-1`). The invariant is that all elements before `i` are 0, all elements after `k` are 2, and elements between `i` and `j` are 1 (unknown/being processed). While `j <= k`, we examine `arr[j]`: if it's 0, swap with `arr[i]` then increment both `i` and `j`; if it's 2, swap with `arr[k]` then decrement `k` (but do not increment `j` because the swapped-in value could be 0, 1, or 2 and needs re-examination); if it's 1, just increment `j`. This ensures a single pass. Edge cases: empty array (loop never runs), all same values (loop processes fine), and after swapping a 2, the new element at `j` is unknown so `j` is not advanced. Time complexity is O(n) since each index moves monotonically, and space complexity is O(1).
#include <vector>
#include <utility>  // for std::swap

// Sorts an array containing only 0s, 1s, and 2s in-place using the Dutch National Flag algorithm.
// The array is reordered so that all 0s come first, then all 1s, then all 2s.
// The input vector can be empty or contain any combination of 0, 1, and 2.
void sort012(std::vector<int>& arr) {
    int low = 0;          // boundary for 0s (exclusive)
    int mid = 0;          // current scanning pointer
    int high = static_cast<int>(arr.size()) - 1;  // boundary for 2s (exclusive)

    while (mid <= high) {
        if (arr[mid] == 0) {
            std::swap(arr[low], arr[mid]);
            ++low;
            ++mid;
        } else if (arr[mid] == 2) {
            std::swap(arr[mid], arr[high]);
            --high;
        } else {  // arr[mid] == 1
            ++mid;
        }
    }
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> a1 = {0, 1, 2, 0, 1, 2};
    sort012(a1);
    assert(a1 == std::vector<int>({0, 0, 1, 1, 2, 2}));

    std::vector<int> a2 = {2, 2, 2};
    sort012(a2);
    assert(a2 == std::vector<int>({2, 2, 2}));

    std::vector<int> a3 = {0, 0, 0};
    sort012(a3);
    assert(a3 == std::vector<int>({0, 0, 0}));

    std::vector<int> a4 = {1, 1, 1};
    sort012(a4);
    assert(a4 == std::vector<int>({1, 1, 1}));

    std::vector<int> a5 = {};
    sort012(a5);
    assert(a5.empty());

    std::vector<int> a6 = {2, 0, 1};
    sort012(a6);
    assert(a6 == std::vector<int>({0, 1, 2}));

    std::vector<int> a7 = {1, 2, 0, 2, 0, 1, 2, 0};
    sort012(a7);
    assert(a7 == std::vector<int>({0, 0, 0, 1, 1, 2, 2, 2}));

    std::vector<int> a8 = {0};
    sort012(a8);
    assert(a8 == std::vector<int>({0}));

    std::vector<int> a9 = {1};
    sort012(a9);
    assert(a9 == std::vector<int>({1}));

    std::vector<int> a10 = {2};
    sort012(a10);
    assert(a10 == std::vector<int>({2}));

    return 0;
}

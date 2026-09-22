// Write a C++ function `int removeDuplicates(std::vector<int>& nums)` that removes duplicate elements from a sorted integer vector such that each element appears at most twice. The function must modify the input vector in-place (keeping the relative order of the remaining elements) and return the new length `k`. The first `k` elements of the vector after the call must contain the valid result, and the elements beyond `k` are irrelevant (e.g., can be left as-is). The input vector is guaranteed to be sorted in non-decreasing order. You may assume the vector is non-empty (size at least 1). Do not allocate extra space for another array; you must do this by modifying the input array in-place with O(1) extra memory.
// The problem is a classic two-pointer technique. Since the vector is sorted, duplicates appear consecutively. We maintain a write index `avail` that marks the position where the next valid element should be placed. We also track the current "window" value `win` and its run length `win_cnt` to limit each value to at most two occurrences. We iterate from index 1 onward (since index 0 is always kept). For each element:
// - If it equals `win`, increment `win_cnt`. If `win_cnt` exceeds 2, skip (do not copy) because we already have two copies. Otherwise, swap the current element with the element at `avail` and increment `avail`.
// - If it differs from `win`, update `win` to the new value, reset `win_cnt` to 1, and swap the current element into `avail` and increment `avail`.
//
// Using `std::swap` ensures that the elements beyond `avail` can hold leftover values; we don’t care about them. This is in-place and runs in O(n) time with O(1) auxiliary space. Edge cases: vector size 1 (returns 1), vector with all identical elements (returns min(2, n)), and already having at most two of each (returns n). The algorithm handles these naturally because the first element is always kept as `win` initial value. Since the input is sorted, no extra sorting or hash maps are needed.
#include <vector>
#include <utility>  // for std::swap

// Removes duplicates such that each element appears at most twice.
// Returns the new length of the valid prefix in the modified vector.
int removeDuplicates(std::vector<int>& nums) {
    const int maxWinCnt = 2;
    int n = static_cast<int>(nums.size());
    if (n == 0) return 0;

    int avail = 1;               // next position to write valid element
    int win = nums[0];           // current run's value
    int winCnt = 1;              // count of current run's value seen so far

    for (int i = 1; i < n; ++i) {
        if (nums[i] == win) {
            ++winCnt;
            if (winCnt > maxWinCnt) {
                // skip: we already have two copies of this value
                continue;
            }
            // valid second occurrence: move it to avail
            std::swap(nums[avail++], nums[i]);
        } else {
            // new value: reset run tracking and move element
            win = nums[i];
            winCnt = 1;
            std::swap(nums[avail++], nums[i]);
        }
    }
    return avail;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: typical case with duplicates beyond two
    std::vector<int> v1 = {1,1,1,2,2,3};
    int k1 = removeDuplicates(v1);
    assert(k1 == 5);
    assert(v1[0] == 1 && v1[1] == 1 && v1[2] == 2 && v1[3] == 2 && v1[4] == 3);

    // Test 2: all identical elements
    std::vector<int> v2 = {5,5,5,5};
    int k2 = removeDuplicates(v2);
    assert(k2 == 2);
    assert(v2[0] == 5 && v2[1] == 5);

    // Test 3: already all unique
    std::vector<int> v3 = {1,2,3,4};
    int k3 = removeDuplicates(v3);
    assert(k3 == 4);
    assert(v3[0] == 1 && v3[1] == 2 && v3[2] == 3 && v3[3] == 4);

    // Test 4: single element
    std::vector<int> v4 = {42};
    int k4 = removeDuplicates(v4);
    assert(k4 == 1);
    assert(v4[0] == 42);

    // Test 5: two of each allowed, but no triples
    std::vector<int> v5 = {0,0,1,1,2,2,3};
    int k5 = removeDuplicates(v5);
    assert(k5 == 7);
    // The content of first 7 elements should be exactly 0,0,1,1,2,2,3 in order
    std::vector<int> expected = {0,0,1,1,2,2,3};
    for (int i = 0; i < k5; ++i) {
        assert(v5[i] == expected[i]);
    }

    // Test 6: triples mixed with singles
    std::vector<int> v6 = {1,1,1,2,3,3,3,4};
    int k6 = removeDuplicates(v6);
    assert(k6 == 6);
    assert(v6[0] == 1 && v6[1] == 1 && v6[2] == 2 && v6[3] == 3 && v6[4] == 3 && v6[5] == 4);

    // Test 7: descending? Not needed because sorted, but check that it works for sorted ascending only
    // Extra: test vector with zeros and negatives
    std::vector<int> v7 = {-5,-5,-5,-1,0,0,2};
    int k7 = removeDuplicates(v7);
    assert(k7 == 6);
    assert(v7[0] == -5 && v7[1] == -5 && v7[2] == -1 && v7[3] == 0 && v7[4] == 0 && v7[5] == 2);

    return 0;
}

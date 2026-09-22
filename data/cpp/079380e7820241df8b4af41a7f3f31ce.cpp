// Write a C++ function that takes a non-empty vector of non-negative integers and repeatedly reduces it according to the following rule until only one element remains: at each reduction step, if the current length is `n` (which is guaranteed to be a power of two), create a new vector of length `n/2` where the element at index `i` is `min(nums[2*i], nums[2*i+1])` when `i` is even, and `max(nums[2*i], nums[2*i+1])` when `i` is odd. The function should return the single integer left after all reductions. The input vector length will be a power of two between 1 and 2^16. Note that the input vector may be modified during the process, but you must not rely on the original contents after the function returns; the function should not allocate extra vectors beyond constant auxiliary memory (in-place reduction is allowed).
#include <cassert>
#include <vector>

int minMaxGame(std::vector<int>& nums); // declaration from solution

int main() {
    std::vector<int> v1 = {1, 2, 3, 4};
    assert(minMaxGame(v1) == 1);

    std::vector<int> v2 = {5, 3, 8, 1, 9, 2, 7, 6};
    // Level 1: [min(5,3)=3, max(8,1)=8, min(9,2)=2, max(7,6)=7] => [3,8,2,7]
    // Level 2: [min(3,8)=3, max(2,7)=7] => [3,7]
    // Level 3: [min(3,7)=3] => 3
    assert(minMaxGame(v2) == 3);

    std::vector<int> v3 = {7};
    assert(minMaxGame(v3) == 7);

    std::vector<int> v4 = {2, 10};
    // Level 1: [min(2,10)=2] => 2
    assert(minMaxGame(v4) == 2);

    std::vector<int> v5 = {10, 2};
    // Level 1: [min(10,2)=2] => 2
    assert(minMaxGame(v5) == 2);

    std::vector<int> v6 = {1, 1, 1, 1, 1, 1, 1, 1};
    assert(minMaxGame(v6) == 1);

    std::vector<int> v7 = {100, 1, 99, 2, 98, 3, 97, 4, 96, 5, 95, 6, 94, 7, 93, 8};
    // Can manually trace or trust logic; expected to be min of first pair because final step is index 0 (even) => min.
    // Level 1: [1,99,2,98,3,97,4,96,5,95,6,94,7,93,8,?] Wait length 16 -> newSize=8
    // i0: min(100,1)=1, i1: max(99,2)=99, i2: min(98,3)=3, i3: max(97,4)=97, i4: min(96,5)=5, i5: max(95,6)=95, i6: min(94,7)=7, i7: max(93,8)=93 => [1,99,3,97,5,95,7,93]
    // Level 2 (size 8 -> newSize=4): i0 min(1,99)=1, i1 max(3,97)=97, i2 min(5,95)=5, i3 max(7,93)=93 => [1,97,5,93]
    // Level 3 (size 4 -> newSize=2): i0 min(1,97)=1, i1 max(5,93)=93 => [1,93]
    // Level 4 (size 2 -> newSize=1): i0 min(1,93)=1 => 1
    assert(minMaxGame(v7) == 1);

    // Large power of two: all same value
    std::vector<int> v8(1024, 42);
    assert(minMaxGame(v8) == 42);

    return 0;
}
#include <vector>
#include <algorithm>

// Repeatedly reduce the vector as described: even indices use min, odd use max,
// until a single element remains. The input vector is modified in place.
int minMaxGame(std::vector<int>& nums) {
    int currentSize = static_cast<int>(nums.size());
    while (currentSize > 1) {
        int newSize = currentSize / 2;
        for (int i = 0; i < newSize; ++i) {
            int left = nums[2 * i];
            int right = nums[2 * i + 1];
            nums[i] = (i % 2 == 0) ? std::min(left, right) : std::max(left, right);
        }
        currentSize = newSize;
    }
    return nums[0];
}
// The algorithm simulates the reduction process directly. Let `m` be the current length of the array, initially equal to the input size. While `m > 1`, we set `m = m / 2` (since the new length is exactly half), and for each index `i` from 0 to `m-1`, we compute a pair from the old array positions `2*i` and `2*i+1`, and write the result back into the array at position `i`. Because we always overwrite positions that are strictly less than `2*i`, we never destroy data that we still need for the next pair within the same iteration; the old values at positions `0..m-1` are not needed after they are overwritten, and positions `m..2*m-1` contain the original values until they are consumed. The alternating rule is determined by `i % 2 == 0` for min, else max. The loop terminates when only one element remains, which is the answer. Edge cases: when the vector has size 1, the loop body never executes and the function returns `nums[0]`. Since the length is always a power of two, the halving is exact and no leftover elements exist. Time complexity is O(N) where N is the input size, because each level processes a total of N/2 + N/4 + ... + 1 = N-1 elements. Space complexity is O(1) auxiliary, as we modify the input vector in place.

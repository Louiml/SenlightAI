/*
Write a C++ function `int maxUnguardedPairs(vector<int>& nums)` that, given an array of non-negative integers, first sorts them in ascending order. Then, starting from the largest element, repeatedly try to pair each element from the smaller half of the array (starting from the largest of the smaller half down to the smallest) with the current largest unpaired element. A pair is valid only if twice the smaller element is less than or equal to the larger element. If a valid pair exists, the larger element is consumed (paired) and you move to the next largest element. The function should return the number of elements that remain unpaired after optimally performing as many valid pairings as possible. The input array may be modified (you may sort it in place). Assume the array length is even and at least 2. For example, given `[1, 2, 3, 4]`, the optimal pairing is (1,4) and (2,3) giving 0 unpaired; for `[1, 1, 2, 2]` pairing (1,2) and (1,2) gives 0; for `[3, 5, 8, 10]` you can pair (3,8) only, leaving 2 unpaired; for `[7, 9, 9, 20]` you can pair (7,20) leaving 2 unpaired.
*/
#include <vector>
#include <algorithm>

// Returns the number of elements that remain unpaired after optimally
// pairing each element from the smaller half with the largest available
// element such that 2 * smaller <= larger. The input vector is sorted in place.
int maxUnguardedPairs(std::vector<int>& nums) {
    std::sort(nums.begin(), nums.end());
    int n = static_cast<int>(nums.size());
    int large = n - 1;            // index of current largest available element
    for (int small = n / 2 - 1; small >= 0; --small) {
        if (nums[small] * 2 <= nums[large]) {
            --large;              // this large element is paired, move to next
        }
    }
    // All elements from index 'large' downward to 0 are unpaired,
    // but index 'large' is the last unpaired large element, so count is large+1.
    return large + 1;
}
#include <cassert>
#include <vector>

// declaration of the function being tested
int maxUnguardedPairs(std::vector<int>& nums);

int main() {
    std::vector<int> a1 = {1, 2, 3, 4};
    assert(maxUnguardedPairs(a1) == 0);

    std::vector<int> a2 = {1, 1, 2, 2};
    assert(maxUnguardedPairs(a2) == 0);

    std::vector<int> a3 = {3, 5, 8, 10};
    assert(maxUnguardedPairs(a3) == 2);

    std::vector<int> a4 = {7, 9, 9, 20};
    assert(maxUnguardedPairs(a4) == 2);

    std::vector<int> a5 = {1, 1, 1, 1};
    assert(maxUnguardedPairs(a5) == 0);

    std::vector<int> a6 = {1, 2, 2, 4};
    assert(maxUnguardedPairs(a6) == 0);

    std::vector<int> a7 = {4, 4, 5, 5};
    assert(maxUnguardedPairs(a7) == 2);

    std::vector<int> a8 = {0, 0, 0, 0};
    assert(maxUnguardedPairs(a8) == 0);

    std::vector<int> a9 = {100, 1, 2, 3};
    assert(maxUnguardedPairs(a9) == 2);

    std::vector<int> a10 = {10, 20, 30, 40, 50, 60};
    assert(maxUnguardedPairs(a10) == 0);
}
// The task is derived from a classic greedy algorithm: after sorting the array, the optimal way to maximize the number of valid pairs is to try to pair the largest available element with the largest possible element from the smaller half that satisfies the condition. The provided snippet uses two pointers: `p` initialized to the largest index (`n-1`), representing the current candidate large element to pair with. Then it iterates `i` from `n/2 - 1` down to `0`, covering the smaller half (since the array is sorted, the first half elements are ≤ the second half). For each `i`, if `s[i] * 2 <= s[p]`, it means a valid pairing exists between `s[i]` and `s[p]`, so we decrement `p` to use the next largest available element (since this one is now paired). If the condition fails, we skip that small element (it cannot be paired with the current large one, nor with any larger one, so it remains unpaired). After the loop, the number of unpaired elements is exactly `p + 1` because all elements from index `p` onward are either already paired (larger ones) or the current `p` index points to the largest unpaired element and all smaller indices are unpaired. The logic works because pairing a small element with the largest available large element is optimal—if a small element cannot pair with the largest, it cannot pair with any smaller large element. Edge cases: if no valid pair exists, `p` never decreases, so the result is `n` (all unpaired). Time complexity is O(n log n) due to sorting, and O(1) auxiliary space (ignoring input storage). The function modifies the input vector by sorting it.

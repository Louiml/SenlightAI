/*
Write a C++ function named `findOriginalArray` that takes a non-empty `std::vector<int>` named `changed` and returns a `std::vector<int>` representing the original array from which `changed` was derived. The `changed` array is produced by taking an original array, doubling each element, and then shuffling the resulting doubled elements together with the original elements (i.e., concatenating original and doubled arrays then shuffling). The function must return the original array in any order if a valid original array exists; otherwise, it must return an empty vector. If the size of `changed` is odd, no valid original exists. Elements can be negative and may include duplicates. The solution must handle a dangling edge case where zero appears: since `0 * 2 == 0`, pairing zeros requires careful counting (each zero in the original consumes two zeros in `changed`). The function should be efficient for up to 10^5 elements.
*/

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>

// Given a vector `changed` that is a permutation of original elements and their doubles,
// return the original array if possible, otherwise an empty vector.
std::vector<int> findOriginalArray(const std::vector<int>& changed) {
    if (changed.size() % 2 != 0) {
        return {};
    }

    std::unordered_map<int, int> freq;
    for (int value : changed) {
        ++freq[value];
    }

    // Collect unique keys and sort by absolute value to process smaller halves first.
    std::vector<int> keys;
    for (const auto& entry : freq) {
        keys.push_back(entry.first);
    }
    std::sort(keys.begin(), keys.end(),
              [](int a, int b) { return std::abs(a) < std::abs(b); });

    std::vector<int> original;
    original.reserve(changed.size() / 2);

    for (int x : keys) {
        if (x == 0) {
            // For zero, each original zero produces two zeros in `changed`.
            if (freq[0] % 2 != 0) {
                return {};
            }
            int zeros = freq[0] / 2;
            original.insert(original.end(), zeros, 0);
            freq[0] = 0;
            continue;
        }

        // For non-zero, the number of `x` in original must not exceed the count of `2*x`.
        if (freq[x] > freq[2 * x]) {
            return {};
        }
        original.insert(original.end(), freq[x], x);
        freq[2 * x] -= freq[x];
    }

    return original;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<int> res1 = findOriginalArray({1,2,3,4,6,8});
    assert(res1.size() == 3);
    std::sort(res1.begin(), res1.end());
    assert((res1 == std::vector<int>{1,3,4}));

    // All zeros: original has half as many zeros
    std::vector<int> res2 = findOriginalArray({0,0,0,0});
    assert(res2.size() == 2);
    assert((res2 == std::vector<int>{0,0}));

    // Odd length -> empty
    assert(findOriginalArray({1,2,3}).empty());

    // Impossible pairing
    assert(findOriginalArray({1,2,4}).empty());

    // Negative numbers
    std::vector<int> res3 = findOriginalArray({-2,-4,-1,-2});
    std::sort(res3.begin(), res3.end());
    assert((res3 == std::vector<int>{-2,-1}));

    // Single zero
    assert(findOriginalArray({0}).empty());

    // Single non-zero
    assert(findOriginalArray({5}).empty());

    // Duplicate values
    std::vector<int> res4 = findOriginalArray({1,2,2,4,4,8});
    std::sort(res4.begin(), res4.end());
    assert((res4 == std::vector<int>{1,2,4}));

    // Mixed with zero and non-zero
    std::vector<int> res5 = findOriginalArray({0,0,1,2,2,4});
    std::sort(res5.begin(), res5.end());
    assert((res5 == std::vector<int>{0,1,2}));

    return 0;
}

// The key insight is to pair each element `x` in `changed` with its double `2*x`. Because the original array's elements are paired with their doubles, we can recover the original by processing numbers in increasing order of absolute value. Sorting by `abs(x)` ensures that when we process a number, all smaller absolute values (which could be its half) have already been processed, so we never mistakenly pair a large number before its smaller counterpart. Use a hash map to count frequencies of each number in `changed`. Sort the unique keys by `abs(value)`. For each key `x` in sorted order, if `mp[x] > mp[2*x]`, there are more occurrences of `x` than can be paired with doubles, so it's impossible and return empty. Otherwise, append `x` to the result `mp[x]` times, and decrement `mp[2*x]` by `mp[x]` for each pairing. This works even for negative numbers and zero: for `x = 0`, `2*x = 0`, so when processing zero, `mp[0] > mp[0]` is false (equal), and we append `mp[0]` zeros while decrementing `mp[0]` by `mp[0]`, effectively leaving `mp[0]` unchanged? Wait, careful: `mp[0] > mp[0]` is false, but we then loop `i=0..mp[0]-1`, each time decrement `mp[0]` by 1. Starting with `mp[0] = k`, after the loop we appended `k` zeros and `mp[0]` becomes `k - k = 0`. But `mp[2*0]` is the same `mp[0]`, so we are decrementing the same map entry. That works because after the loop, `mp[0]` becomes 0, and we appended `k` zeros, meaning `k` original zeros consumed `2k` changed zeros? But wait: each original zero produces two zeros in `changed` (because 0*2=0). So if original has `z` zeros, changed has `2z` zeros. Processing key `0`: `mp[0]` = `2z`. Since `mp[0] > mp[0]` false, then loop `i=0..2z-1`, append `2z` zeros? That would be incorrect. The algorithm as written appends `mp[x]` times, so for `x=0`, it appends `2z` zeros, which is wrong; original should have only `z` zeros. The snippet's logic actually fails for zero? Let's examine: In the snippet, for `x` with `mp[x] > mp[2*x]` return empty; for `x=0`, `mp[0] == mp[0]`, not greater, so no return. Then the loop `for(int i=0;i<mp[x];i++,mp[2*x]--)` appends `mp[x]` copies of `x`. For `x=0`, appends `mp[0]` zeros and decrements `mp[0]` the same number. That would indeed append too many zeros. But wait, the original problem (LeetCode 2007) requires handling zero specially. The standard solution processes zero separately: because 0's double is 0, you need to pair them in groups of two. The snippet as given might be incorrect for zeros. For the task, we need a correct solution. So in analysis, we must note that zero requires special treatment: process zeros by checking that `mp[0]` is even, then append `mp[0]/2` zeros, and set `mp[0]` to 0. For all other keys, the normal pairing works. Alternatively, process keys sorted by `abs(x)`, and for `x != 0` use the given logic; for `x == 0`, require even count and append half. Time complexity O(n log n) due to sorting unique keys (or O(n) with counting sort if range small, but generally O(n log n)). Space O(n). Edge cases: odd length, all zeros (must have even count), negative numbers (sort by abs), and duplicates.

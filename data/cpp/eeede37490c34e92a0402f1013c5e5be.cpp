/*
Write a C++ function `int findBlock(const vector<long long>& pileSizes, long long target)` that, given a non-empty vector of positive pile sizes sorted in ascending cumulative order (each element is the positive size of a pile, and piles are arranged contiguously so that the cumulative sums form an increasing sequence), and a positive integer target representing the `k`-th element when all piles are concatenated in order (1-indexed), returns the 1-based index of the pile that contains the `target`-th element. For example, if pileSizes = {3, 1, 4}, then the cumulative sequence is 1,2,3 (pile 1), 4 (pile 2), 5,6,7,8 (pile 3). A target of 5 returns 3; a target of 3 returns 1. The function must use binary search on the prefix sums. Assume all inputs are valid (pile sizes > 0, target between 1 and total sum inclusive). Do not modify the input vector.
*/

#include <vector>

// Given pile sizes and a 1-indexed target position, return the 1-based pile index.
int findBlock(const std::vector<long long>& pileSizes, long long target) {
    int n = static_cast<int>(pileSizes.size());
    std::vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + pileSizes[i];
    }

    int low = 0, high = n;
    while (low < high) {
        int mid = (low + high) / 2;
        if (prefix[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low + 1; // convert 0-based index to 1-based pile number
}

#include <cassert>
#include <vector>

// The solution function is declared above (omitted here for brevity).
int main() {
    std::vector<long long> piles1 = {3, 1, 4};
    assert(findBlock(piles1, 1) == 1);
    assert(findBlock(piles1, 3) == 1);
    assert(findBlock(piles1, 4) == 2);
    assert(findBlock(piles1, 8) == 3);
    assert(findBlock(piles1, 5) == 3);

    std::vector<long long> piles2 = {10};
    assert(findBlock(piles2, 1) == 1);
    assert(findBlock(piles2, 10) == 1);

    std::vector<long long> piles3 = {2, 2, 2};
    assert(findBlock(piles3, 2) == 1);
    assert(findBlock(piles3, 3) == 2);
    assert(findBlock(piles3, 5) == 3);
    assert(findBlock(piles3, 6) == 3);

    std::vector<long long> piles4 = {1, 5, 2, 3};
    assert(findBlock(piles4, 1) == 1);
    assert(findBlock(piles4, 2) == 2);
    assert(findBlock(piles4, 6) == 2);
    assert(findBlock(piles4, 7) == 3);
    assert(findBlock(piles4, 11) == 4);
}

// The solution builds a prefix sum array `pref` of size `n+1` where `pref[0]=0` and `pref[i] = pref[i-1] + pileSizes[i-1]`. Since all pile sizes are positive, `pref` is strictly increasing, so we can binary search for the smallest index `i` (0-based) such that `pref[i] >= target`. The answer is that index `i` (because `pref[i]` covers elements `pref[i-1]+1` through `pref[i]`, and the 1-based pile index is `i`). Use two pointers or standard binary search: set `low=0`, `high=n`, while `low<high` compute `mid=(low+high)/2`, if `pref[mid] < target` then `low=mid+1` else `high=mid`. After loop, `low` equals the desired pile index (0-based), return `low+1`. Edge cases: target equals exactly a boundary (e.g., target = pileSizes[0]) should return the first pile; target equals total sum returns the last pile. Time complexity: O(n) to build prefix sums and O(log n) per query. Space complexity: O(n) for prefix sums. The function is `const`-correct by taking `const vector<long long>&`.

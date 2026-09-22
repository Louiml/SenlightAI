Write a C++ function `long long countSubarraysWithAverageAtMostK(const std::vector<long long>& arr, long long k)` that counts the number of non-empty contiguous subarrays whose average is strictly less than or equal to `k`. The average of a subarray is defined as the sum of its elements divided by its length, and you should avoid floating-point comparisons. The array may contain negative numbers and the result may be large (up to `n*(n+1)/2` for `n` up to `10^5`), so use `long long`. The function should run efficiently in `O(n log n)` time.

// The key is to transform the condition for a subarray from `(v[i] + ... + v[j]) / (j - i + 1) <= k` into `(v[i] - k) + ... + (v[j] - k) <= 0`. Define a new array `b[i] = v[i] - k`, and let `prefix[0] = 0` and `prefix[i] = b[0] + ... + b[i-1]` for `i >= 1` (so `prefix[i]` is the sum of the first `i` elements). Then the sum of `b[l .. r]` equals `prefix[r+1] - prefix[l]`. The condition becomes `prefix[r+1] - prefix[l] <= 0`, i.e., `prefix[l] >= prefix[r+1]`. For each right endpoint `r` (from 0 to n-1), we need to count how many left endpoints `l` from 0 to `r` satisfy `prefix[l] >= prefix[r+1]`. This is equivalent to counting, in a set of previously seen prefix values (including `prefix[0]`), how many are greater than or equal to the current `prefix[r+1]`. We can maintain an ordered set (e.g., GNU pbds tree) that supports order statistics. Since the condition is `>=`, we can find the number of elements strictly less than `current` and subtract from the total count of elements inserted so far. The total number of possible subarrays is `n*(n+1)/2`; subtract the count of subarrays that fail the condition (i.e., average > k) from this total. Edge cases: negative numbers, duplicate prefix values—the ordered set stores duplicates, so `order_of_key(current)` gives the count of elements strictly less than current; the number of elements `>= current` is `(size of set) - order_of_key(current)`. Complexity: `O(n log n)` time, `O(n)` space.

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

template<class K>
using ordered_set = tree<K, null_type, less<K>, rb_tree_tag, tree_order_statistics_node_update>;

// Counts non-empty contiguous subarrays whose average is <= k.
// Uses prefix sums of (arr[i] - k) and an order-statistics tree.
long long countSubarraysWithAverageAtMostK(const vector<long long>& arr, long long k) {
    const long long n = static_cast<long long>(arr.size());
    ordered_set<long long> prefixes;
    prefixes.insert(0);  // prefix sum before the first element

    long long totalSubarrays = n * (n + 1LL) / 2LL;  // all possible non-empty subarrays
    long long badSubarrays = 0;  // subarrays with average > k

    long long currentPrefix = 0;
    for (long long i = 0; i < n; ++i) {
        currentPrefix += (arr[i] - k);  // incremental prefix of (arr - k)

        // Count how many previous prefixes are < currentPrefix.
        // Those correspond to subarrays with sum > 0, i.e., average > k.
        long long strictlyLess = static_cast<long long>(prefixes.order_of_key(currentPrefix));
        badSubarrays += strictlyLess;

        prefixes.insert(currentPrefix);
    }

    return totalSubarrays - badSubarrays;
}

#include <bits/stdc++.h>
#include "solution.h"  // or paste the function above

int main() {
    // Test 1: simple positive case, e.g., [1,2,3], k=2
    // Subarrays with avg <=2: [1] (1), [2] (2), [1,2] (1.5), [3] (3>2 fails), [2,3] (2.5 fails), [1,2,3] (2 <=2 ok) -> 4
    assert(countSubarraysWithAverageAtMostK({1,2,3}, 2) == 4);

    // Test 2: all equal to k -> every subarray has avg == k -> all n*(n+1)/2
    assert(countSubarraysWithAverageAtMostK({5,5,5}, 5) == 6);

    // Test 3: negative numbers, k=0
    // [-1,-2] subarrays: [-1] (-1<=0 ok), [-2] (-2<=0 ok), [-1,-2] (-1.5<=0 ok) -> 3
    assert(countSubarraysWithAverageAtMostK({-1,-2}, 0) == 3);

    // Test 4: empty? Not applicable, but ensure n=0 returns 0 (though spec says non-empty, we can guard)
    // Actually the function is for non-empty, but test small.
    // For n=1, k=10, arr=[5] -> 1
    assert(countSubarraysWithAverageAtMostK({5}, 10) == 1);

    // Test 5: duplicates prefix handling
    // [3, -3, 3], k=0 -> b = [3,-3,3] prefix: 0,3,0,3
    // Subarrays avg<=0: compute manually
    // All subarrays: [3](3>0 fail), [-3](-3<=0 ok), [3](3>0 fail), [3,-3](0<=0 ok), [-3,3](0<=0 ok), [3,-3,3](1>0 fail) => 3
    assert(countSubarraysWithAverageAtMostK({3, -3, 3}, 0) == 3);

    // Test 6: strictly greater k
    // [1], k=0 -> avg=1 >0 -> 0
    assert(countSubarraysWithAverageAtMostK({1}, 0) == 0);

    // Test 7: large values, basic check with single element
    assert(countSubarraysWithAverageAtMostK({1000000000}, 1000000000) == 1);

    // Test 8: mixed large and small, k= -5
    // arr = [-10, 5], k=-5 -> b = [-5, 10] prefix: 0,-5,5
    // Subarrays: [-10] avg=-10<=-5 ok, [5] avg=5>-5 fail, [-10,5] avg=-2.5 > -5 fail -> 1
    assert(countSubarraysWithAverageAtMostK({-10,5}, -5) == 1);

    // Test 9: sorted increasing, k large enough to include all
    assert(countSubarraysWithAverageAtMostK({1,2,3,4}, 100) == 10);

    // Test 10: all negative, k= -1
    // [-2, -2, -2] avg of any subarray = -2 <= -1 -> all 6
    assert(countSubarraysWithAverageAtMostK({-2,-2,-2}, -1) == 6);

    return 0;
}

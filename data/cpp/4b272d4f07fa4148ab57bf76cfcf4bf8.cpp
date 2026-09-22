// You are given a sorted array `a` of `n` distinct positive integers and `q` queries each containing a positive integer `k`. For each query, find the smallest positive integer `x` such that `x >= k` and `x` is not present in the array `a`. In other words, find the smallest number that is at least `k` and is missing from the sorted list. Write a function `long long smallestMissingAtLeast(const vector<long long>& sortedArr, long long k)` that returns this value. The array is guaranteed to be sorted in non-decreasing order, and all elements are distinct. The function must handle up to `n` and `q` in the order of up to 10^5, and values of `k` and array elements up to 10^18. Note that the answer can be larger than any element in the array. The algorithm should be efficient for many queries.

#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function declaration here or above.
// For brevity, assume the function from the solution is defined above.

int main() {
    // Basic cases
    std::vector<long long> a1 = {1, 3, 5};
    assert(smallestMissingAtLeast(a1, 2) == 2);
    assert(smallestMissingAtLeast(a1, 3) == 4);
    assert(smallestMissingAtLeast(a1, 4) == 4);
    assert(smallestMissingAtLeast(a1, 6) == 6);

    // k is less than first element
    std::vector<long long> a2 = {10, 20, 30};
    assert(smallestMissingAtLeast(a2, 1) == 1);
    assert(smallestMissingAtLeast(a2, 11) == 11);
    assert(smallestMissingAtLeast(a2, 21) == 21);

    // Consecutive numbers present
    std::vector<long long> a3 = {1, 2, 3, 4, 5};
    assert(smallestMissingAtLeast(a3, 1) == 6);
    assert(smallestMissingAtLeast(a3, 3) == 6);
    assert(smallestMissingAtLeast(a3, 6) == 6);

    // Large values and k at upper end
    std::vector<long long> a4 = {1000000000000000000LL, 1000000000000000002LL};
    assert(smallestMissingAtLeast(a4, 1000000000000000000LL) == 1000000000000000001LL);
    assert(smallestMissingAtLeast(a4, 1000000000000000003LL) == 1000000000000000003LL);

    // Empty array (skip? but task implies non-empty? We handle anyway)
    std::vector<long long> a5;
    assert(smallestMissingAtLeast(a5, 5) == 5);

    // Edge: k is exactly a present element, but next one is present too
    std::vector<long long> a6 = {2, 3, 4, 7};
    assert(smallestMissingAtLeast(a6, 3) == 5);
    assert(smallestMissingAtLeast(a6, 4) == 5);
    assert(smallestMissingAtLeast(a6, 5) == 5);
    assert(smallestMissingAtLeast(a6, 6) == 6);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the smallest integer x >= k such that x is not present in the sorted distinct array a.
long long smallestMissingAtLeast(const std::vector<long long>& a, long long k) {
    // Binary search for the smallest x >= k with at least one missing number in [k, x].
    long long low = k;
    long long high = k + static_cast<long long>(a.size()) + 1;  // safe upper bound
    while (low < high) {
        long long mid = low + (high - low) / 2;
        // Number of array elements in [k, mid]
        long long presentInRange = static_cast<long long>(
            std::upper_bound(a.begin(), a.end(), mid) - 
            std::lower_bound(a.begin(), a.end(), k)
        );
        // Total integers from k to mid inclusive: mid - k + 1
        long long totalInRange = mid - k + 1;
        if (totalInRange > presentInRange) {
            // There is at least one missing number in [k, mid]
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

// The problem is essentially: given a sorted set of distinct numbers, for a query `k`, we want the smallest missing number `>= k`. A naive approach of checking every integer starting from `k` would be too slow if there are many consecutive present numbers. The key observation: if we take a candidate value `ans`, the number of array elements that are `<= ans` is given by `upper_bound(a.begin(), a.end(), ans) - a.begin()` (let's call that `cnt`). If `ans` were missing, then `cnt` should be strictly less than `ans - k + 1`? Not directly. A better formulation: The number of missing numbers up to `ans` is `ans - (number of present numbers <= ans)`. We want the first `ans >= k` such that the count of present numbers `<= ans` is exactly `ans - k`? Let's derive. If we shift focus: Let `ans = k + offset`. For `ans` to be missing, we need that among numbers from `k` to `ans-1`, there are exactly `offset` distinct numbers, but since the array is sorted and distinct, the number of elements in the whole array that are `<= ans` minus the number of elements `< k` must equal the number of present numbers in `[k, ans]`. Actually, the standard trick is: define `missing_count(t)` = number of integers in `[1, t]` that are not in the array = `t - (number of array elements <= t)`. We need smallest `x >= k` such that `x` is missing. That means `missing_count(x) > missing_count(x-1)`. The `missing_count` is non-decreasing, and each time we pass a non-present number, it increases by 1. So we need to find the first `x >= k` where `missing_count(x) - missing_count(k-1) >= 1`? Not exactly. Actually, the condition that `x` is missing is equivalent to `upper_bound(a, x) - lower_bound(a, x) == 0`. But we can binary search on `x` because the predicate "is x missing?" is not monotonic, but the predicate "the number of missing numbers in `[k, x]` is at least 1" is monotonic (if it's true for `x`, it's true for all larger `x`). However, we want the smallest `x` where the count of missing numbers in `[k, x]` is exactly 1 and that missing number is `x` itself? No, if there are multiple missing numbers, the smallest one is the first. So we can binary search on `x` for the condition: `(x - k + 1) > (upper_bound(a, x) - lower_bound(a, k))`. Here `(x - k + 1)` is total numbers in range `[k, x]`, and `(upper_bound(a, x) - lower_bound(a, k))` is how many of them are present. If the difference is positive, then there is at least one missing number in that range. The predicate "difference >= 1" is monotonic in `x`. So the smallest `x` satisfying it is the answer. The lower bound for binary search is `k`, and the upper bound can be `k + n + 1` because in the worst case, all `n` distinct numbers are consecutive from some starting point, so the first missing after `k` is at most `k + n` (since among `n+1` consecutive numbers starting from `k`, at most `n` can be present). We can set high = `k + n + 1` to be safe. Time complexity per query: O(log(n + answer_range)) ≈ O(log n) because we do binary search with `upper_bound` each step, which is O(log n), so total O(log^2 n) per query. But we can do better: The original code uses an iterative approach that simulates adding the count of elements <= current ans to ans, which converges quickly because the count grows. That approach is essentially binary-searching the fixed point: `ans = k + (number of elements <= ans)`. That fixed point is the answer. The number of iterations is at most the number of times the count can increase before reaching the fixed point, which is O(number of elements in a range) but in practice fast; worst-case O(n) per query if the array contains exactly the numbers from 1 to n and k=1? Actually, if a = [1,2,3,...,n], and k=1, then initial ans=1, tmp=1 (upper_bound gives 1), new ans=1+1=2, tmp=2, ans=3, ... it takes n iterations. So that's O(n) worst-case. We can improve to O(log n) per query using binary search on `x` with a monotonic predicate. Edge cases: `k` can be less than the first element, so `lower_bound(a, k)` returns 0. If `k` is itself missing, the answer is `k`. The array is sorted and distinct. Use 64-bit integers since values up to 1e18. Space complexity O(1) extra.

/*
Write a C++ function `long long weightedMedianBlockSum(int n, int k, const std::vector<int>& a)` that computes the sum of selected elements from a 1-indexed sequence `a` of length `n*k` according to the following rule:  
- If `n == 1`, select **all** elements (sum of the entire array).  
- If `n == 2`, select elements at odd 1-based indices (1,3,5,...).  
- For `n >= 3`, let `mid = (n+1)/2 - 1`. Select elements starting from index `k*mid + 1` and then take every `(n - mid)`-th element (i.e., indices `k*mid + 1`, `k*mid + 1 + (n - mid)`, `k*mid + 1 + 2*(n - mid)`, ...) until the end of the array. Return the sum of these selected elements as a `long long`. The input `a` is given as a 0-indexed vector, but the indices in the problem are 1-based, so adjust accordingly. Ensure the function works for any positive `n`, `k`, and `a.size() == n*k`. Validate that `mid` is non-negative when `n >= 3` (it always is for `n >= 2`). The function must not rely on global variables or `scanf`; it should process the vector directly.
*/
#include <vector>
#include <cstddef>

// Computes the sum of selected elements from a according to the described rules.
// n and k are positive integers, and a.size() must equal n*k.
long long weightedMedianBlockSum(int n, int k, const std::vector<int>& a) {
    long long ans = 0;
    const std::size_t total = static_cast<std::size_t>(n) * k;
    
    if (n == 1) {
        // Sum all elements
        for (std::size_t i = 0; i < total; ++i) {
            ans += a[i];
        }
    } else if (n == 2) {
        // Sum elements at 1-based odd indices (i=0,2,4,...)
        for (std::size_t i = 0; i < total; i += 2) {
            ans += a[i];
        }
    } else {
        // n >= 3
        const int mid = (n + 1) / 2 - 1; // mid is at least 1 for n>=3
        const std::size_t start = static_cast<std::size_t>(k) * mid; // 0-based index
        const std::size_t step = static_cast<std::size_t>(n - mid);
        for (std::size_t i = start; i < total; i += step) {
            ans += a[i];
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// Declare the function being tested (it is defined elsewhere in the solution)
long long weightedMedianBlockSum(int n, int k, const std::vector<int>& a);

int main() {
    // n=1: sum all
    assert(weightedMedianBlockSum(1, 3, {1,2,3}) == 6);
    assert(weightedMedianBlockSum(1, 1, {10}) == 10);

    // n=2: sum odd 1-based indices
    // 1-based odd indices: 1,3,5 → 0-based: 0,2,4
    assert(weightedMedianBlockSum(2, 2, {5,3,7,1}) == (5+7)); // 12
    assert(weightedMedianBlockSum(2, 1, {4,9}) == 4);

    // n=3: k=1, n*k=3, mid=(3+1)/2-1=1, start=k*mid=1, step=n-mid=2
    // indices: 1,3 (0-based) → a[1]+a[3]? Wait total=3, indices 1 and 1+2=3 (out of bounds) so only a[1]
    assert(weightedMedianBlockSum(3, 1, {1,2,3}) == 2);
    // n=3, k=2, total=6, start=2, step=2 → indices 2,4 (0-based) = a[2]+a[4]
    assert(weightedMedianBlockSum(3, 2, {1,2,3,4,5,6}) == (3+5)); // 8

    // n=4: mid=(4+1)/2-1=2-1=1? Actually (5/2)=2, so mid=1. start=k*1, step=n-mid=3
    // k=2, total=8, start=2, indices 2,5 (0-based) = a[2]+a[5]
    assert(weightedMedianBlockSum(4, 2, {10,20,30,40,50,60,70,80}) == (30+60)); // 90

    // n=5: mid=(5+1)/2-1=3-1=2, start=2*k, step=5-2=3
    // k=1, total=5, start=2, indices 2,5 (out) → a[2]
    assert(weightedMedianBlockSum(5, 1, {1,2,3,4,5}) == 3);
    // k=2, total=10, start=4, step=3 → indices 4,7 → a[4]+a[7]
    assert(weightedMedianBlockSum(5, 2, {1,2,3,4,5,6,7,8,9,10}) == (5+8)); // 13

    // Large values to check long long sum
    std::vector<int> big(1*1, 1000000);
    assert(weightedMedianBlockSum(1, 1, big) == 1000000);

    return 0;
}
// The solution directly implements the three cases. For `n == 1`, iterate over all `n*k` elements and accumulate the sum. For `n == 2`, iterate with step 2 starting from index 0 (1-based index 1). For `n >= 3`, compute `mid = (n+1)/2 - 1` (this is an integer ≥ 1 for `n >= 3`), then compute the starting 0-based index `start = k*mid` (because 1-based index `k*mid + 1` becomes 0-based `k*mid`). Then iterate from `start` up to `n*k - 1` with step `step = n - mid`. Note that `step` is at least 1, and for typical values (e.g., `n=3`, `mid=1`, step=2) it selects every other element after the starting point. The loop must ensure that we only sum indices within the vector bounds; the loop condition handles that. Edge case: when `k` is large and `start` may be close to `n*k`, the loop may run only once or not at all (but `start < n*k` always because `mid < n` and `k*mid < k*n`). Time complexity is `O(n*k)` in the worst case (when `n==1` or when step is 1), but typically it is `O(k)` for `n>=2` because the number of selected elements is at most `k` (or `k` plus a small constant). More precisely, the number of iterations is roughly `k + (n - mid - 1)/(n - mid)` which is `O(k)`. Space complexity is `O(1)` besides the input vector. Use `long long` for the sum to avoid overflow since values may be large.

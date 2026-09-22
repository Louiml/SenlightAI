/*
Write a C++ function `ll compute_sequence_sum(ll n, ll k)` that, given positive integers `n` and `k` with `1 <= k <= n`, computes the sum of every element at positions that are multiples of `k` in the 0-indexed sequence `arr` defined as follows: `arr` contains the `n` integers `1,2,...,n` arranged in a special pattern where the first half (floor(n/2)) elements are the odd-indexed values from `1,2,...,n` in increasing order, and the second half consists of the remaining values in increasing order. More precisely, define `arr[0..n-1]` such that `arr[0] = 1`, `arr[1] = 3`, `arr[2] = 5`, ... (all odd numbers up to the largest odd ≤ n) then all even numbers in increasing order. For example, with `n=7`, `arr = [1,3,5,7,2,4,6]`. The function must return the sum `arr[k-1] + arr[2k-1] + arr[3k-1] + ...` while the index remains < n. The result is guaranteed to fit in a 64-bit signed integer. The input constraints are up to `n,k ≤ 10^18`, so the solution must be O(log n) time and O(1) space. Note: the original snippet's logic involves a binary decomposition of the pattern; you must reverse-engineer a correct and efficient formula without using loops that iterate over the array.
*/
#include <cstdint>
#include <algorithm>

using ll = long long;

// Compute ceil(a/b) for positive integers
inline ll ceil_div(ll a, ll b) {
    return (a + b - 1) / b;
}

// Sum of arithmetic progression from 0 to cnt-1 with step 1
inline ll sum_first_n(ll cnt) {
    return cnt * (cnt - 1) / 2;
}

// Sum of arithmetic progression from start to end inclusive
inline ll sum_range(ll start, ll end) {
    if (start > end) return 0;
    return (start + end) * (end - start + 1) / 2;
}

// Compute sum of arr[i] for all i multiples of k (i = m*k), where arr is odd numbers first then even numbers.
ll compute_sequence_sum(ll n, ll k) {
    const ll odd_count = (n + 1) / 2;  // ceil(n/2)
    
    // Multiples in the odd region: indices < odd_count
    // largest m such that m*k < odd_count
    ll m_odd = (odd_count - 1) / k;
    ll cnt_odd = m_odd + 1;
    ll sum_odd = 0;
    if (cnt_odd > 0) {
        // sum_{m=0}^{m_odd} (2*m*k + 1) = 2*k * sum(m) + cnt_odd
        ll sum_m = sum_first_n(cnt_odd);  // sum of 0..m_odd
        sum_odd = 2 * k * sum_m + cnt_odd;
    }
    
    // Multiples in the even region: indices >= odd_count and < n
    ll m_start = ceil_div(odd_count, k); // smallest m with m*k >= odd_count
    ll m_end = (n - 1) / k;
    ll sum_even = 0;
    if (m_start <= m_end) {
        ll cnt_even = m_end - m_start + 1;
        // for m in [m_start, m_end], value = 2*(m*k - odd_count + 1)
        // sum = 2*k * sum(m) - 2*odd_count*cnt_even + 2*cnt_even
        ll sum_m = sum_range(m_start, m_end);
        sum_even = 2 * k * sum_m - 2 * odd_count * cnt_even + 2 * cnt_even;
    }
    
    return sum_odd + sum_even;
}
#include <cassert>

// Declare the function prototype to match solution
using ll = long long;
ll compute_sequence_sum(ll n, ll k);

int main() {
    // n=7, arr=[1,3,5,7,2,4,6]; k=1 => sum all = 28
    assert(compute_sequence_sum(7, 1) == 28);
    // k=2 => indices 0,2,4,6 => arr[0]=1, arr[2]=5, arr[4]=2, arr[6]=6 => sum=14
    assert(compute_sequence_sum(7, 2) == 14);
    // k=3 => indices 0,3,6 => 1+7+6=14
    assert(compute_sequence_sum(7, 3) == 14);
    // k=4 => indices 0,4 => 1+2=3
    assert(compute_sequence_sum(7, 4) == 3);
    // n=6, arr=[1,3,5,2,4,6]; k=2 => indices 0,2,4 => 1+5+4=10
    assert(compute_sequence_sum(6, 2) == 10);
    // n=1, arr=[1]; k=1 => 1
    assert(compute_sequence_sum(1, 1) == 1);
    // n=5, arr=[1,3,5,2,4]; k=3 => indices 0,3 => 1+2=3
    assert(compute_sequence_sum(5, 3) == 3);
    // Large test: n=1e18, k=1e18-1, only index 0 => arr[0]=1
    assert(compute_sequence_sum(1000000000000000000LL, 999999999999999999LL) == 1);
    // n=10, arr=[1,3,5,7,9,2,4,6,8,10]; k=5 => indices 0,5 => 1+2=3
    assert(compute_sequence_sum(10, 5) == 3);
    // n=10, k=3 => indices 0,3,6,9 => 1+7+4+10=22
    assert(compute_sequence_sum(10, 3) == 22);
    return 0;
}
// The sequence `arr` is defined by placing all odd numbers from 1 to n (in increasing order) first, then all even numbers (in increasing order). Let `odd_count = ceil(n/2) = (n+1)/2` (integer division) and `even_count = floor(n/2)`. For any index `i` (0-based), if `i < odd_count`, then `arr[i] = 2*i + 1` (odd). Otherwise, let `j = i - odd_count` (0-based among evens), then `arr[i] = 2*(j+1) = 2*j+2` (even). We need the sum over indices that are multiples of `k` (i.e., `i = m*k` for `m = 0,1,2,...` while `i < n`). Let `M = floor((n-1)/k)` be the largest integer such that `M*k < n`. The sum is `S = sum_{m=0}^{M} arr[m*k]`. Since `n` can be up to 1e18, we cannot iterate. However, notice that the pattern is periodic in the sense that the mapping from index to value is linear in two segments. We can split the multiples of `k` into those that fall in the odd region and those in the even region. Let `L = odd_count`. The multiples `m*k` that are < L form an arithmetic progression of indices: `0, k, 2k, ..., (a-1)k` where `a = ceil(L/k)` = the number of multiples strictly less than L (since L may not be a multiple of k). Actually the condition for odd region is `i < L`, so the largest m satisfying `m*k < L` is `m_odd = floor((L-1)/k)`, giving count `cnt_odd = m_odd + 1`. For each such m, the value is `2*(m*k)+1`. Sum = `2*k * sum_{m=0}^{m_odd} m + (cnt_odd)`. The sum of first `cnt_odd` integers starting from 0 is `cnt_odd*(cnt_odd-1)/2`. Similarly, for even region: indices `m*k` that are >= L and < n. Let `start = ceil(L/k) * k`? Actually let the next multiple after L-1 be `L` rounded up to multiple of k. Let `m_start = ceil(L/k)` (since if L is multiple of k, then `m_start = L/k` gives index exactly L which is first even index). Let `m_end = floor((n-1)/k)`. If `m_start <= m_end`, then for each such m, the value is `2*(m*k - L + 1)`. Sum = `2*k * sum_{m=m_start}^{m_end} m - 2*L*(count_even) + 2*(count_even)` where `count_even = m_end - m_start + 1`. So the total sum can be computed in O(1) using arithmetic series formulas. Edge cases: when `k > n`, then only m=0 works, giving arr[0]=1 if n>=1 (but the problem states k<=n). When `n` is odd or even, the formulas handle. Also, use `long long` with careful division to avoid overflow (but sums up to ~1e18 fit in 64-bit). We must implement `ceil_div(a,b)` = (a+b-1)/b for positive integers. The time complexity is O(1) arithmetic operations, space O(1). The original snippet had a binary decomposition but that is overcomplicated; we provide a direct formula.

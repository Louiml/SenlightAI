Write a C++ function named `minimumCopyTime` that takes three integer arguments: `n` (the number of copies needed, ≥ 1), `x` (time in seconds to make one copy on machine A, > 0), and `y` (time in seconds to make one copy on machine B, > 0). Both machines can work simultaneously, each producing one copy per its respective fixed time interval. The function must compute and return the minimum total time (in seconds) required to produce at least `n` copies, given that one copy has already been produced before the timing starts (so we need `n-1` additional copies). For example, if `n=5`, `x=1`, `y=1`, the machines together produce 2 copies per second, so we need 2 more seconds after the initial copy, total 3 seconds. Use a binary search over the possible time to find the earliest moment when the two machines collectively have produced at least `n-1` additional copies, then add the initial copy time `min(x,y)`. Handle large values (up to 1e18) using 64-bit integers.

#include <cassert>

int main() {
    // n=1: no extra copies needed, just the initial copy time.
    assert(minimumCopyTime(1, 5, 7) == 5);
    assert(minimumCopyTime(1, 100, 1) == 1);
    
    // n=2, x and y equal: need one extra copy, faster machine does it.
    assert(minimumCopyTime(2, 3, 5) == 3 + 3); // initial 3, extra 3 = 6
    assert(minimumCopyTime(2, 10, 10) == 10 + 10);
    
    // n=5, x=1, y=1: initial 1, then need 4 copies, each second produces 2, so 2 seconds.
    assert(minimumCopyTime(5, 1, 1) == 1 + 2);
    
    // n=6, x=2, y=3: initial 2, need 5 more, check time.
    // At T=6: 6/2 + 6/3 = 3+2 =5 => works, so total = 8.
    assert(minimumCopyTime(6, 2, 3) == 2 + 6);
    
    // n=10, x=4, y=6: initial 4, need 9. Try T=24: 24/4+24/6=6+4=10 >=9.
    // But can we do with T=18? 18/4+18/6=4+3=7 <9. T=20: 5+3=8 <9. T=22: 5+3=8 <9.
    // T=24 works, so total = 28.
    assert(minimumCopyTime(10, 4, 6) == 4 + 24);
    
    // Large values to check overflow safety.
    assert(minimumCopyTime(1000000000000LL, 1LL, 1000000000000LL) == 1LL + 999999999999LL);
    
    // Asymmetric speeds: x very small, y huge, many copies.
    // n=1000, x=1, y=1000000: initial 1, need 999, likely T=999 (machine A alone).
    assert(minimumCopyTime(1000, 1, 1000000) == 1 + 999);
    
    // n=1000, x=1000000, y=1: symmetric to above.
    assert(minimumCopyTime(1000, 1000000, 1) == 1 + 999);
    
    // Both machines needed together: n=100, x=3, y=5.
    // Initial 3, need 99. At T=186: 186/3=62, 186/5=37, sum=99 exactly. total=189.
    assert(minimumCopyTime(100, 3, 5) == 3 + 186);
}

#include <algorithm>

// Returns the minimum total time to produce at least n copies
// given that one copy is already done before timing starts.
// x and y are the per-copy times for two machines working in parallel.
long long minimumCopyTime(long long n, long long x, long long y) {
    if (n <= 1) {
        return std::min(x, y);
    }
    
    const long long copies_needed = n - 1;
    const long long initial_time = std::min(x, y);
    
    // Binary search for the smallest T such that T/x + T/y >= copies_needed
    long long low = 0;
    long long high = std::max(x, y) * copies_needed; // safe upper bound
    long long best_time = high;
    
    while (low <= high) {
        const long long mid = low + (high - low) / 2; // avoid overflow
        const long long made = mid / x + mid / y;
        
        if (made >= copies_needed) {
            best_time = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    return initial_time + best_time;
}

// The key insight is that the two machines operate independently and in parallel. After the first copy is made (which takes `min(x,y)` seconds because the faster machine finishes first), we need to make `n-1` more copies. Let `T` be the time spent after the initial copy. During that time, machine A produces `T / x` copies and machine B produces `T / y` copies (integer division, since each completes only after full intervals). We need `T` such that `T/x + T/y >= n-1`. The function `f(T) = T/x + T/y` is non-decreasing in `T`, allowing binary search. The lower bound is 0 and the upper bound is `max(x,y) * (n-1)`, because even the slower machine alone would finish by then. Binary search finds the smallest `T` satisfying the inequality, then we return `T + min(x,y)`. Edge cases: when `n=1`, we need no additional copies, answer is `min(x,y)`; the binary search handles this because `n-1=0` and the smallest `T` is 0. Time complexity is `O(log(max(x,y)*(n-1)))` ≈ `O(log(max(x,y)) + log(n))`, and space complexity is O(1). All arithmetic must use `long long` to avoid overflow.

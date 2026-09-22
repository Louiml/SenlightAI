/*
Given integers `n` and `k` (1 ≤ n ≤ 100, 1 ≤ k ≤ 10^9), write a C++ function `double probabilityAtLeastK(int n, int k)` that simulates the following process: You have a counter starting at 1. For each of `n` rounds, you choose a random integer `i` uniformly from 1 to `n`. Then you repeatedly halve a variable `cnt` (initially 1) and double `i` until `i` is at least `k`. The contribution to the result is `cnt / n` (after halving as many times as needed). The function returns the sum of these contributions over all `n` rounds (the expected value). The answer must be printed with 10 decimal places. Note that `n` and `k` are integers, but the computation uses floating-point arithmetic. The process is deterministic once `i` is chosen; you are summing over all possible `i`. The function must handle large `k` where `i` may need many halvings, and small `n` with edge cases like `k <= n`.
*/
#include <cmath>

// Computes the expected value of the process described.
// For each i from 1 to n, start cnt=1, now=i.
// While now < k, halve cnt and double now. Add cnt/n to total.
// Returns the sum.
double probabilityAtLeastK(int n, int k) {
    double total = 0.0;
    const double inv_n = 1.0 / n;
    for (int i = 1; i <= n; ++i) {
        double now = static_cast<double>(i);
        double cnt = 1.0;
        while (now < k) {
            cnt *= 0.5;
            now *= 2.0;
        }
        total += cnt * inv_n;
    }
    return total;
}
#include <cassert>
#include <cmath>

// Forward declaration of the function under test.
double probabilityAtLeastK(int n, int k);

int main() {
    // Edge case: k <= n, so no halving is ever needed. Each i is already >= k when i>=k.
    // For i < k, halving occurs. Let's verify manually for small values.
    // n=1, k=1: i=1 >=1, cnt=1, total=1.0
    assert(std::fabs(probabilityAtLeastK(1, 1) - 1.0) < 1e-12);
    // n=1, k=2: i=1 -> now=1<2 -> cnt=0.5, now=2, total=0.5
    assert(std::fabs(probabilityAtLeastK(1, 2) - 0.5) < 1e-12);
    // n=2, k=3: i=1 -> halve once: cnt=0.5, now=2<3 -> halve again: cnt=0.25, now=4 => 0.25/2=0.125
    //          i=2 -> now=2<3 -> halve once: cnt=0.5, now=4 => 0.5/2=0.25 => total=0.375
    assert(std::fabs(probabilityAtLeastK(2, 3) - 0.375) < 1e-12);
    // n=2, k=4: i=1 -> now=1<4 -> halve: cnt=0.5, now=2<4 -> halve: cnt=0.25, now=4 => 0.25/2=0.125
    //          i=2 -> now=2<4 -> halve: cnt=0.5, now=4 => 0.5/2=0.25 => total=0.375
    assert(std::fabs(probabilityAtLeastK(2, 4) - 0.375) < 1e-12);
    // n=3, k=5: i=1 -> now=1->2->4->8 (3 halvings): cnt=1/8=0.125, /3=0.041666...
    //          i=2 -> now=2->4->8 (2 halvings): cnt=1/4=0.25, /3=0.083333...
    //          i=3 -> now=3->6 (1 halving): cnt=0.5, /3=0.166666...
    //          total = 0.291666...
    assert(std::fabs(probabilityAtLeastK(3, 5) - 0.2916666666666667) < 1e-12);
    // n=10, k=1000: no overflow, double handles large k. Check that sum is positive and <=1.
    double result = probabilityAtLeastK(10, 1000);
    assert(result > 0.0 && result <= 1.0);
    // n=100, k=1e9: huge halvings, but still fine.
    result = probabilityAtLeastK(100, 1000000000);
    assert(result > 0.0 && result <= 1.0);
    // n=1, k=1000000000: i=1, halve until now >= 1e9, which is about 30 times -> cnt ~ 1e-9
    assert(std::fabs(probabilityAtLeastK(1, 1000000000) - std::pow(0.5, 30)) < 1e-12);
    return 0;
}
// The algorithm iterates through every integer `i` from 1 to `n`. For each `i`, we start `cnt = 1` and `now = i`. While `now < k`, we halve `cnt` and double `now`. This loop runs at most `ceil(log2(k / i))` times when `i < k`, otherwise it doesn't run at all. After the loop, we add `cnt / n` to the answer. The variable `cnt` is reset to 1 for each `i`. The sum over all `i` gives the total expected value because each `i` is equally likely (probability 1/n). Edge cases: when `i >= k`, no halving occurs, so contribution is `1/n` each. When `i` is very small and `k` is large, the number of iterations is logarithmic in `k`, which is at most ~30 for `k ≤ 10^9`. Time complexity is O(n log k), and space complexity is O(1). Precision: using `double` is sufficient because with at most 30 halvings, the smallest `cnt` is about 1e-9, and summing up to 100 such terms retains enough precision for 10 decimal places when printed with `setprecision(10)`.

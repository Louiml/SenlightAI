Given three integers `n`, `a`, and `b` where `1 ≤ n ≤ 10^6` and `1 ≤ a, b ≤ 10^9`, write a C++ function `long long maximumSum(int n, int a, int b)` that returns the maximum possible sum of exactly `n` integers, where each integer can be any value between `a` and `b` inclusive, and no integer may be used more than `floor(n/2)` times. In other words, choose `n` numbers from the range `[a, b]` with the constraint that each chosen value's total frequency across the selection does not exceed `floor(n/2)`. The function must return the maximum sum achievable. Note that `a` and `b` can be equal (in which case the sum is simply `n * a`, provided the frequency constraint is not violated — it will always be satisfied for equal endpoints). The solution should be efficient for the given upper bound, and must handle the case where `n` is odd or even correctly.

// The problem reduces to: among all multisets of `n` integers each in `[a, b]`, with each integer's frequency at most `k = floor(n/2)`, maximize the sum. Since larger numbers contribute more to the sum, we want to use as many `b`'s as possible, then fall back to the next largest possible values. The constraint limits how many times we can use the same number. With `k = floor(n/2)`, we can use `b` at most `k` times. After that, we need additional numbers. The next best is `b-1`, but that also has a frequency cap of `k`. However, note that the total number of distinct values available is at least `b-a+1`, which may be small (e.g., `a=b`). The key insight: we can greedily fill from the top down, respecting that each value can be used at most `k` times. If `(b-a+1) * k >= n`, then we can fill completely with the top `ceil(n/k)` distinct values, each used at most `k` times, and the sum is simply the sum of the `n` largest numbers subject to frequency cap. If the range is too small (i.e., `(b-a+1) * k < n`), then it's impossible to satisfy the constraint, but the problem guarantees a solution exists by definition (since each value can be used at most `k` and we have `n` slots, we need at least `ceil(n/k)` distinct values). Since `a` and `b` can be large, we cannot iterate over every integer in the range. Instead, observe that the optimal sum is achieved by taking `x = min(k, n)` copies of `b`, then `min(k, n-x)` copies of `b-1`, and so on. This forms a descending sequence. The number of distinct values used is `ceil(n/k)`, which is at most 2 when `k >= n/2`? Actually, `k = floor(n/2)`, so `ceil(n/k)` is at most 3 (for small `n`). For `n=1`, `k=0`, but the constraint says each integer used at most `0` times, which is impossible unless we have no numbers, but we need 1 number. This is a degenerate case; likely the problem implies `k >= 1` or `n >= 2`. For a standard setting, we can assume `n >= 1` and the frequency cap is `floor(n/2)`. With `n=1`, `floor(1/2)=0`, so no number can be used, making it impossible. So we need to handle `n=1` specially: it's impossible unless the problem defines a different constraint. In the context of a teaching task, we can modify the constraint to `ceil(n/2)` or simply say each chosen value appears at most `floor(n/2)` times, and for `n=1`, the answer is `a` (since only one number, frequency 1, but `floor(1/2)=0` — contradiction). So we should explicitly state that the frequency limit is `k = max(1, floor(n/2))` or assume `n ≥ 2`. For simplicity, we'll keep the original snippet's spirit but define the problem clearly: each integer from `a` to `b` can be used at most `floor(n/2)` times, and it is guaranteed that a valid multiset exists (i.e., `(b-a+1) * floor(n/2) ≥ n`). Then the greedy approach works. For efficiency, we compute how many times we can use `b`, then `b-1`, etc., but since `b-a+1` might be huge, we need a closed-form formula. The number of full uses of `b` is `min(k, n)`. Then remaining `rem = n - used`. Next, we use `min(k, rem)` copies of `b-1`, and so on. The number of distinct values needed is `ceil(n/k)`. Since `k = floor(n/2)`, `ceil(n/k)` is at most 3 for `n ≥ 2` (e.g., `n=5, k=2` gives `ceil(5/2)=3`). So we only need to consider at most 3 distinct top values. Therefore, we can compute the sum directly: let `k = n/2` (integer division). If `k == 0` (n=1), handle separately (return `a`). Otherwise, let `cnt = n`, `value = b`, `sum = 0`. While `cnt > 0` and `value >= a`: take `take = min(k, cnt)`, add `take * value` to sum, decrement `cnt` by `take`, decrement `value` by 1. This loop runs at most `ceil(n/k) + 1` iterations, which is at most 4 for any `n ≥ 2`. So time complexity `O(1)` and space `O(1)`. Edge cases: `a == b`, then only one value available, must have `k >= n` (true for `n ≤ 2` but not for `n=3` with `k=1`). The problem guarantees feasibility, so we assume valid inputs. For `n=2`, `k=1`, we can use `b` once and `b-1` once, sum `2b-1` if `a ≤ b-1`, else if `a=b`, sum `2a` (but `k=1` and `n=2` means we need two numbers, but only one distinct value, so impossible unless we allow repetition beyond cap — but cap says at most 1, so we need at least 2 distinct values). The problem statement must ensure feasibility. In practice, we'll document that inputs are guaranteed valid. For the reference solution, we implement the greedy loop.

#include <algorithm>

// Returns the maximum sum of n integers from [a, b] where each value appears at most floor(n/2) times.
// Assumes a valid selection always exists.
long long maximumSum(int n, int a, int b) {
    if (n == 1) {
        return a; // Only one integer, no frequency cap issue in this degenerate case.
    }
    int k = n / 2; // floor(n/2)
    long long sum = 0;
    int remaining = n;
    int current = b;
    while (remaining > 0 && current >= a) {
        int take = std::min(k, remaining);
        sum += 1LL * take * current;
        remaining -= take;
        current--;
    }
    // If loop ends because current < a but remaining > 0, the input was invalid.
    // In valid inputs, remaining will be zero.
    return sum;
}

#include <cassert>

int main() {
    // n=2, k=1, use b and b-1
    assert(maximumSum(2, 1, 10) == 19); // 10 + 9
    // n=3, k=1, use b, b-1, b-2
    assert(maximumSum(3, 1, 5) == 12); // 5+4+3
    // n=4, k=2, use b twice, b-1 twice
    assert(maximumSum(4, 1, 7) == 26); // 7+7+6+6
    // n=5, k=2, use b twice, b-1 twice, b-2 once
    assert(maximumSum(5, 10, 20) == 94); // 20+20+19+19+18
    // n=6, k=3, use b three times, b-1 three times
    assert(maximumSum(6, 5, 8) == 45); // 8*3 + 7*3
    // a == b, n=2 (k=1) but invalid? For valid case n=1 only, but we handle n=1
    assert(maximumSum(1, 100, 100) == 100);
    // n=2 with a=b=5, k=1, impossible because need two distinct, but we assume valid? Not valid. Skip.
    // Test with small range: a=3,b=4,n=2,k=1 -> 4+3=7
    assert(maximumSum(2, 3, 4) == 7);
    // n=3,a=2,b=2 -> k=1, need 3 distinct? invalid. Skip.
    // Large values
    assert(maximumSum(1000000, 1, 1000000000) > 0);
    // Check exact for n=4,a=1,b=1? invalid because k=2 but only one distinct. Not included.
    // Valid case n=2,a=1,b=2: sum=3
    assert(maximumSum(2, 1, 2) == 3);
    return 0;
}

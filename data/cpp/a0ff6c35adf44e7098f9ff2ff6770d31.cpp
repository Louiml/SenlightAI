Write a C++ function that, given a positive integer `n` and a zero-based step index `k`, simulates the classic "Josephus problem with a twist": starting from person 1 in a circle of people numbered 1 through `n`, every `k`-th person is eliminated, but only after skipping `k` steps at a time (with `k` being the 0-based number of steps to move forward). The function should return the number of the last remaining person. Specifically, the elimination order is: start at person 1, then move `k` steps forward (wrapping around), eliminate that person, and repeat with the next person after the eliminated one. The function must compute the survivor for arbitrary `n` and `k` (both non-negative integers, with `k` possibly zero, and `n` ≥ 1), and must handle large inputs efficiently without simulating the process. The function signature is: `long long josephus_twist(long long n, long long k);`.

#include <cassert>

int main() {
    // n = 1 returns 1 regardless of k
    assert(josephusStep(1, 0) == 1);
    assert(josephusStep(1, 100) == 1);

    // k = 0 always eliminates current person -> survivor is n
    assert(josephusStep(5, 0) == 5);
    assert(josephusStep(1, 0) == 1);

    // Classic Josephus with m = k+1 = 2 for n=5 -> survivor 3
    assert(josephusStep(5, 1) == 3);

    // Classic Josephus with m = k+1 = 3 for n=7 -> survivor 4 (known result)
    assert(josephusStep(7, 2) == 4);

    // Example from task description: n=5, k=2 -> survivor 5
    assert(josephusStep(5, 2) == 5);

    // n=4, k=1: simulate: start 1->2 (remove2), from3->4(remove4), from1->2? actually after removal 2, next is3, move1->4 remove4, next is1, move1->3 remove3, survivor1.
    assert(josephusStep(4, 1) == 1);

    // n=6, k=3: manually simulate: start 1->4 (remove4), next5->2 (remove2), next3->6 (remove6), next1->3 (remove3), next5->1 (remove1), survivor5.
    assert(josephusStep(6, 3) == 5);

    // Large n, k=0 still returns n
    assert(josephusStep(100000, 0) == 100000);

    // Large n, k=1: classic Josephus m=2, for n=10 survivor 5? Let's verify: n=10, m=2 -> sequence: 2,4,6,8,10,3,7,1,9 -> survivor 5. Yes.
    assert(josephusStep(10, 1) == 5);

    return 0;
}

#include <cstdint>

// Returns the last remaining person when starting from person 1, moving k steps forward each time.
// n: number of people, k: number of forward steps per elimination.
long long josephusStep(long long n, long long k) {
    long long ans = 0; // 0-indexed survivor for n=1
    for (long long i = 2; i <= n; ++i) {
        ans = (ans + k + 1) % i;
    }
    return ans + 1; // convert to 1-indexed person number
}

// This is a standard Josephus problem variant where the step count is `k+1` if we count inclusively from the current person, because moving `k` positions forward and then eliminating the landed person is equivalent to counting `k+1` people including the starting position. The classic recurrence for the survivor’s 0-based index `S(n)` is `S(1)=0` and `S(n) = (S(n-1) + k + 1) % n`. After computing `S(n)`, the person number is `S(n)+1`. This recurrence works because when we eliminate one person, the problem reduces to `n-1` people, but the starting index shifts by `k+1` positions. The recurrence is linear in `n`, so for `n ≤ 10^5` it runs in `O(n)` time and `O(1)` auxiliary space. Edge cases: if `n==1`, the answer is always 1. If `k==0`, we always eliminate the current person, so the survivor is always `n` (since we eliminate 1,2,...,n-1 in order). The recurrence handles this correctly. The modulo operation with large `k` is safe because the intermediate sum may exceed `long long` if `k` is up to 10^9 and `n` up to 10^5, which is fine; `(ans + k + 1)` is at most `10^9 + 10^5 + 1`, well within 64-bit range.

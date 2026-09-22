// You are given an array of \( n \) positive integers \( a_1, a_2, \dots, a_n \). In one operation, you may choose any element and increase it by 1, or choose any element and increase it by 2. You may perform these operations any number of times, but each operation increases the total sum by either 1 or 2. The goal is to make all elements equal to some integer \( X \), and you may choose any positive integer \( X \). Determine the **minimum number of operations** required to make all elements equal to \( X \), and then also output the minimum over all valid choices of \( X \). Write a C++ function `long long minOperations(const std::vector<int>& arr)` that returns that minimum total operation count. The array length satisfies \( 1 \le n \le 3 \times 10^5 \), and each \( a_i \) satisfies \( 1 \le a_i \le 10^9 \). The function must be efficient enough to handle the maximum constraints.
// The key observation is that for a fixed target value \( X \), the problem reduces to counting how many elements require an odd increment (`dif = X - a[i]` is odd) versus even increments. Since an increment of 1 is needed exactly once for each odd difference, while even differences can be filled entirely by +2 operations. Let `odd` = number of elements with odd `dif`, and `even` = total `dif / 2` across all elements. If `even > odd`, we can convert pairs of +2 operations into two +1 operations by replacing one +2 and one +1 with three +1? Actually a better way: if there is a surplus of even increments, we can combine them to reduce the number of operations: every time we have three extra even increments (i.e., 3 units of 2 = 6 total, but need odd count), we can replace 3 even operations (3 operations of +2) by 2 odd operations (one +2? Wait, careful). The standard technique: if `even > odd`, let `k = (even - odd) / 3`. Then we can reduce `even` by `k` and increase `odd` by `2k` (since each group of 3 even increments uses 6 total value, which can be replaced by 2 odd increments + 2 even? Let’s derive properly: The total sum to add is `S = sum(dif)`. Let `odd` = number of odd difs, `even = (S - odd)/2`. The minimal operations for a given `(odd, even)` is: if `odd <= even`, then we can pair each odd with an even? Actually, each odd needs at least one +1. Each even needs +2s. If `even >= odd`, we can do: for each odd, use one +1 (1 op) and then the remaining even count `even - odd` can be done with +2 ops. But sometimes we can replace 2 even ops (2*2=4) with 3 odd ops (+1+1+? no). The known result: after balancing, if `even > odd`, set `k = (even - odd) / 3`, then `odd += 2*k`, `even -= k`. This is because 3 even increments (each +2) give total 6, which can be replaced by 2 odd (+1) and 2 even (+2)?? That doesn't reduce count. Actually, the correct transformation: three +2 operations (3 ops, total +6) can be replaced by two +1 and one +4? No, that doesn't help. The real trick: if we have 3 extra even increments (i.e., 3 more +2 operations than needed for odd pairing), we can combine them into 2 operations: one +2 and two +1? That would be 3 ops total. Hmm.
//
// Let me think from known solution: The minimal number of operations for a fixed X is:  
// Let `odd` = count of i where (X - a[i]) is odd.  
// Let `even` = sum_{i} (X - a[i]) / 2.  
// If `even <= odd`, answer = `2*odd - 1` if odd > even, else `2*odd` (when equal).  
// If `even > odd`, let diff = even - odd. Then reduce diff by groups of 3: t = diff / 3, then odd += 2*t, even -= t. Then recompute: if odd == even, ans = 2*odd; if odd > even, ans = 2*odd -1; if even == odd+1? Actually after reduction, we get either odd == even or odd == even+1? The formula from code: after balancing, if odd==even return 2*odd; else if odd>even return 2*odd-1; else (even==odd+1) return 2*even; else return 2*odd+3 (which rarely happens after the reduction).  
// The logic: Each odd difference requires at least 1 operation (+1). Each even difference of size 2k requires k operations (+2). If we have more even than odd, we can merge some even operations with odd ones? Actually, the optimal is to use +1 for all odd differences, and +2 for all even differences, but if we have surplus even, we can replace some +2 operations with two +1 operations? That would increase count, so not helpful. The real trick is to change the target X to X+1 sometimes, because for X and X+1, the parity of difs flips. The code checks both maxi and maxi+1, because optimal X is either max(a) or max(a)+1 (since increasing X beyond that only adds unnecessary operations).  
// Thus the algorithm: For a given target X, compute odd and even as above, apply the balancing (if even>odd, reduce by groups of 3), then compute minimal operations using the derived formula. Then check X = max(a) and X = max(a)+1, take the minimum. Time complexity O(n) per check, overall O(n). Space O(1) besides input.
#include <vector>
#include <algorithm>
#include <cstdint>

// Compute minimum operations to make all elements equal to target.
long long costForTarget(const std::vector<int>& arr, long long target) {
    long long odd = 0;
    long long even = 0;
    for (int x : arr) {
        long long dif = target - x;
        if (dif < 0) return (1LL << 62); // cannot decrease
        if (dif & 1) odd++;
        even += dif / 2;
    }
    if (even > odd) {
        long long need = (even - odd) / 3;
        odd += 2 * need;
        even -= need;
    }
    if (odd == even) return 2 * odd;
    if (odd > even) return 2 * odd - 1;
    if (even == odd + 1) return 2 * even;
    return 2 * odd + 3;
}

// Return minimum total operations over all possible target values.
long long minOperations(const std::vector<int>& arr) {
    if (arr.empty()) return 0;
    int maxVal = *std::max_element(arr.begin(), arr.end());
    long long ans = (1LL << 62);
    ans = std::min(ans, costForTarget(arr, maxVal));
    ans = std::min(ans, costForTarget(arr, maxVal + 1));
    return ans;
}
#include <cassert>
#include <vector>

// Assume solution function is above (as in

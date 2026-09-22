/*
You are given \(N\) items arranged in a sequence, each with a positive integer value \(c_i\) (for \(i=1..N\)). You start with an amount of money \(B\) (a positive integer). You may perform the following operation **at most once**: choose any item \(i\) (where \(1 \le i < N\)), spend some amount of money \(x\) (where \(0 \le x \le B\)) that is a multiple of \(c_i\), and in return you receive \(x / c_i\) copies of item \(i\). Each copy of item \(i\) can later be sold for the **maximum value among items from position \(i\) to \(N\)** (i.e., \(\max(c_i, c_{i+1}, \dots, c_N)\)). You are also allowed to keep the remaining money \(B-x\). Write a C++ function that, given the number of items \(N\), the initial money \(B\), and the array of item values \(c\) (1-indexed conceptually), returns the **maximum possible final amount of money** you can have after performing at most one such operation. If you do nothing, you keep \(B\). Note that you can also choose to spend all or part of \(B\), but you must spend a multiple of \(c_i\). The array is given as a 0-indexed `std::vector<int>`.
*/
#include <vector>
#include <algorithm>

// Compute the maximum final money after at most one purchase operation.
// Parameters:
//   n - number of items
//   money - initial amount B
//   c - vector of item values (size n, 0-indexed)
// Returns the maximum possible final money as a long long.
long long maxFinalMoney(int n, long long money, const std::vector<int>& c) {
    if (n <= 1) return money;
    
    // best[i] = maximum value among c[i]..c[n-1]
    std::vector<int> best(n);
    best[n-1] = c[n-1];
    for (int i = n-2; i >= 0; --i) {
        best[i] = std::max(c[i], best[i+1]);
    }
    
    long long answer = money; // doing nothing
    
    for (int i = 0; i < n-1; ++i) {
        long long ci = c[i];
        long long buyCount = money / ci;
        long long spent = buyCount * ci;
        long long leftover = money - spent;
        long long candidate = leftover + buyCount * static_cast<long long>(best[i]);
        answer = std::max(answer, candidate);
    }
    
    return answer;
}
#include <cassert>
#include <vector>

// The solution function is already declared above (in the Solution section).
// Test cases directly call maxFinalMoney.
int main() {
    // Basic case: buy item with value 3, sell at future max 5, B=10 => spend 9, get 3*5=15 + 1 = 16
    assert(maxFinalMoney(3, 10, std::vector<int>{3, 5, 4}) == 16);
    
    // No profitable trade: all equal values => keep B
    assert(maxFinalMoney(3, 10, std::vector<int>{2, 2, 2}) == 10);
    
    // Single item: no purchase allowed => keep B
    assert(maxFinalMoney(1, 7, std::vector<int>{5}) == 7);
    
    // B too small to buy any item => keep B
    assert(maxFinalMoney(4, 3, std::vector<int>{5, 6, 7, 8}) == 3);
    
    // Best future value is at a later position, buy early cheap item
    // c = [1, 100, 2], B=5 => buy item0 (cost 1), sell at max(1,100,2)=100, spend 5, get 500
    assert(maxFinalMoney(3, 5, std::vector<int>{1, 100, 2}) == 500);
    
    // Money not multiple of cost: B=10, c=[3, 7, 6] => buy item0 cost 3*3=9, sell at max(3,7,6)=7 => 21+1=22
    assert(maxFinalMoney(3, 10, std::vector<int>{3, 7, 6}) == 22);
    
    // Large numbers to test long long
    assert(maxFinalMoney(2, 1000000000LL, std::vector<int>{1, 1000000000}) == 1000000000000000000LL);
    
    // Purchase at last possible index (i = n-2) uses its own value as best if it's max
    assert(maxFinalMoney(2, 6, std::vector<int>{4, 3}) == 6); // no profit (c0=4, best=4 since max(4,3)=4)
    
    // Case where leaving all money is better than any partial trade (cost > B)
    assert(maxFinalMoney(4, 5, std::vector<int>{10, 20, 30, 40}) == 5);
    
    return 0;
}
// The problem is a classic exchange-rate arbitrage: For each possible purchase item \(i\) (from 1 to \(N-1\)), you can buy as many copies as your budget allows, each costing \(c_i\) and selling at the maximum future value `best[i] = max(c[i]...c[N-1])`. The profit per unit is `best[i] - c[i]`. To maximize the final money, you should spend the **entire portion** of your budget that is a multiple of \(c_i\) on that item, i.e., `(B / c[i]) * c[i]` money spent, yielding `(B/c[i]) * best[i]` from selling, plus the leftover `B % c[i]` money. So candidate final money for item \(i\) is `B % c[i] + (B/c[i]) * best[i]`. You also consider doing nothing, which yields \(B\). The answer is the maximum of all candidates. Edge cases: If \(B < c_i\) then `B/c[i] == 0`, candidate equals `B` (no profit). If all `c_i` are equal, the best future value equals `c_i`, so no profit, and the answer is `B`. Also, if `N==1`, no purchase is possible, so answer is `B`. To compute `best[i]` efficiently, iterate from the last element backward, maintaining the maximum seen so far. Time complexity is \(O(N)\), space is \(O(N)\) for the `best` array (or \(O(1)\) if we precompute suffix maxima in a separate vector, but the simplest is to use a vector of size N). Space can be reduced to \(O(1)\) by scanning from right to left and maintaining the suffix max without storing it, but we need to evaluate each `i` from 0 to N-2 with the suffix max of positions `i..N-1`, so we can precompute suffix max in a vector or compute on the fly by iterating from right to left and storing each candidate. The on-the-fly approach: start from the last element, maintain `suffixMax`, then iterate `i` from N-1 down to 0; for each `i`, if `i < N-1`, we have a valid purchase candidate using `suffixMax` (which currently equals max of `c[i+1..N-1]` only if we update after moving left; careful: we need max from i to N-1, so we should first include `c[i]`). Better: precompute a suffix max array `best[i] = max(c[i], best[i+1])` for i from N-1 down to 0. Then for each i from 0 to N-2 compute candidate. That is simple and clear. Edge case when `N==0`? Not needed; assume `N >= 1`. The function should handle `N=1` by returning `B`. The answer fits in `long long` because `B/c[i]` can be up to `B` (if `c[i]=1`) and `best[i]` up to large, but we use `long long` for safety.

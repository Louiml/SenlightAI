// Given an array of positive integers, write a C++ function `int minRemainingItems(std::vector<int>& a)` that returns the minimum number of items that must be kept after performing the following operation as many times as possible: repeatedly pair the smallest available item with the largest available item such that the smaller value `x` satisfies `2 * x <= y` (where `y` is the paired larger item). Each successful pairing removes both items from the array. The goal is to remove as many items as possible; the answer is the number of items that remain after no more valid pairings can be made. The input array may be unsorted, and its length `n` satisfies `2 <= n <= 2*10^5` (though the function should work for any positive `n`). The values are positive integers up to `10^9`. The function must modify the array (e.g., sort it) if needed, but the returned value is just the remaining count.
The optimal strategy is to always pair the smallest possible item with the largest possible item because that maximizes the chance of satisfying the condition `2 * small <= large`. Sorting the array first allows us to use a two‑pointer approach: one pointer `l` starts at the smallest elements and another `r` starts at the largest. However, we do not simply scan once from both ends; instead, we need to determine how many of the smallest items can be successfully matched with the largest items. The key observation is that if we sort the array `a[0..n-1]`, then in an optimal matching, every small item comes from the first half (positions `0..m-1`) and every matching large item comes from the last `m` positions (`n-m..n-1`). We need to find the maximum `m` such that for every `i` from `0` to `m-1`, `a[i] * 2 <= a[n-m+i]`. This can be checked naively in `O(m)` per candidate, but using binary search on `m` (with a feasibility check) reduces the time complexity. The feasibility check for a given `m` verifies that the smallest `m` elements can be paired in order with the largest `m` elements under the given inequality. If feasible, we can try larger `m`; otherwise, we must reduce `m`. After finding the maximum `m`, the number of removed items is `2 * m`, so the remaining count is `n - 2 * m`. Edge cases: if no pairing is possible, `m=0` and answer is `n`; also ensure that `m` never exceeds `n/2`. The time complexity is `O(n log n)` for sorting plus `O(n log n)` for binary search (each check is `O(m) ≤ O(n)`), and space is `O(1)` auxiliary (ignoring input storage).
#include <vector>
#include <algorithm>
#include <functional>

// Given a vector of positive integers, return the minimum number of items
// that remain after repeatedly pairing the smallest unpaired item with the
// largest unpaired item whenever 2 * smallest <= largest. Each pair removes
// both items. The function modifies the input vector (sorts it).
int minRemainingItems(std::vector<int>& a) {
    if (a.empty()) return 0;
    std::sort(a.begin(), a.end());
    
    const int n = static_cast<int>(a.size());
    
    // Check if we can form m valid pairs using the m smallest items
    // with the m largest items.
    auto canFormPairs = [&](int m) -> bool {
        if (m == 0) return true;
        // Pair a[0]..a[m-1] with a[n-m]..a[n-1]
        for (int i = 0; i < m; ++i) {
            if (a[i] * 2 > a[n - m + i]) {
                return false;
            }
        }
        return true;
    };
    
    // Binary search the maximum m in [0, n/2] that satisfies the pairing condition.
    int lo = 0;
    int hi = n / 2;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (canFormPairs(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    
    // We successfully removed 2 * lo items, so answer is n - 2 * lo.
    return n - 2 * lo;
}
#include <cassert>
#include <vector>

// Forward declaration of the function being tested.
int minRemainingItems(std::vector<int>& a);

int main() {
    // Basic case: [1,2,3,4] -> pair 1 with 4 (2*1<=4) and 2 with 3? No, because 2*2<=3 is false.
    // Actually largest items are 4 and 3: pair 1 with 4, then 2 with 3 fails -> only one pair, answer = 2.
    {
        std::vector<int> a = {1,2,3,4};
        assert(minRemainingItems(a) == 2);
    }
    // All pairs possible: [1,2,3,6] -> pair 1 with 6 (2<=6) and 2 with 3 (4>3? no, 2*2=4 >3) so only one pair? Wait: sorted: [1,2,3,6], largest items are 6 and 3. Pair 1 with 6 OK, pair 2 with 3? 2*2=4 >3 fails. So answer 2.
    {
        std::vector<int> a = {1,2,3,6};
        assert(minRemainingItems(a) == 2);
    }
    // No pairs possible: [1,1,1,2] -> smallest 1, largest 2: 2*1<=2 OK, then remaining [1,1] -> 1 with 1 fails. So one pair, answer 2.
    {
        std::vector<int> a = {1,1,1,2};
        assert(minRemainingItems(a) == 2);
    }
    // Single item array (not expected but handle): answer 1.
    {
        std::vector<int> a = {5};
        assert(minRemainingItems(a) == 1);
    }
    // Large gap: [1,10,20,30] -> sorted: [1,10,20,30] pair 1 with 30 OK, pair 10 with 20? 20<=20 OK, so two pairs -> answer 0.
    {
        std::vector<int> a = {1,10,20,30};
        assert(minRemainingItems(a) == 0);
    }
    // Unsorted with duplicates: [10,1,2,10] -> sorted [1,2,10,10] pair 1 with 10 OK (2<=10), pair 2 with 10 OK (4<=10) -> two pairs -> answer 0.
    {
        std::vector<int> a = {10,1,2,10};
        assert(minRemainingItems(a) == 0);
    }
    // Larger case: [1,2,3,4,5,100] -> sorted [1,2,3,4,5,100] pair 1 with 100, 2 with 5 (4<=5), 3 with 4 (6>4?) fails, so only two pairs -> answer 2.
    {
        std::vector<int> a = {1,2,3,4,5,100};
        assert(minRemainingItems(a) == 2);
    }
    // All identical: [7,7,7,7] -> pair 7 with 7? 14<=7 false, so no pairs -> answer 4.
    {
        std::vector<int> a = {7,7,7,7};
        assert(minRemainingItems(a) == 4);
    }
    // Example from original snippet: [1,2,3,4,5,6] -> sorted same. Pair 1 with 6, 2 with 5, 3 with 4? 6<=4 false, so two pairs -> answer 2.
    {
        std::vector<int> a = {1,2,3,4,5,6};
        assert(minRemainingItems(a) == 2);
    }
    // Edge: exactly two elements that can pair: [2,5] -> 2*2=4<=5 OK, answer 0.
    {
        std::vector<int> a = {2,5};
        assert(minRemainingItems(a) == 0);
    }
    // Edge: exactly two elements that cannot pair: [3,5] -> 6>5, answer 2.
    {
        std::vector<int> a = {3,5};
        assert(minRemainingItems(a) == 2);
    }
    return 0;
}

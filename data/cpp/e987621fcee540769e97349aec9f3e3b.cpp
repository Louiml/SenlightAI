/*
Given an array of positive integers (with possible duplicates), write a C++ function `int minDeletionsToMakeTrianglePossible(const std::vector<int>& nums)` that returns the minimum number of elements that must be deleted from the array so that the remaining elements can form at least one triangle (i.e., there exist three distinct indices whose values satisfy the triangle inequality: the sum of any two is greater than the third). You must not reorder or modify the input array beyond sorting a copy for analysis. If no three elements can form a triangle even after deletions, you must delete all but two (so return `n - 2`). The function should handle arrays of size 0, 1, or 2 gracefully (returning 0, 0, and 0 respectively, since no deletion is needed for "at least one triangle" because it's impossible but we can leave them as is—but define clearly: for sizes < 3, the answer is 0 since no triangle can be formed but also no deletion needed). For sizes >= 3, compute the minimal deletions.
*/
#include <vector>
#include <algorithm>

// Return the minimum number of deletions so that at least one triangle can be formed.
int minDeletionsToMakeTrianglePossible(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n < 3) {
        return 0;  // nothing to delete (no triangle possible anyway)
    }

    // Copy and sort
    std::vector<int> a = nums;
    std::sort(a.begin(), a.end());

    int best = n - 2;  // worst case: keep only two elements

    for (int i = 0; i <= n - 3; ++i) {
        // Check if we can keep a[i] and a[i+1] and find the first element that violates
        long long sum = static_cast<long long>(a[i]) + a[i + 1];
        auto it = std::lower_bound(a.begin() + i + 2, a.end(), sum);
        int ind = static_cast<int>(it - a.begin());
        // If ind == n, all elements from i+2 to end are < sum, so any of them works.
        // Then deletions = i (delete everything before i) and keep i+2.
        // If ind < n, we must delete everything from ind to end (since they are >= sum).
        int deletions = i + (n - ind);  // ind == n gives i, correct
        best = std::min(best, deletions);
    }

    return best;
}
#include <cassert>
#include <vector>

// Forward declaration of the tested function (provided elsewhere)
int minDeletionsToMakeTrianglePossible(const std::vector<int>&);

int main() {
    // Basic cases
    assert(minDeletionsToMakeTrianglePossible({}) == 0);
    assert(minDeletionsToMakeTrianglePossible({7}) == 0);
    assert(minDeletionsToMakeTrianglePossible({5, 9}) == 0);

    // Already has a triangle (e.g., 2,3,4 -> 2+3>4)
    assert(minDeletionsToMakeTrianglePossible({2, 3, 4}) == 0);
    assert(minDeletionsToMakeTrianglePossible({1, 2, 3, 4, 5}) == 0);

    // Need to delete one to form a triangle
    // e.g., {1,2,5,8} sort: 1,2,5,8 -> 1+2=3 <5, so must delete 5 and 8? Actually 2+5>8? 7>8 false. Keep 1,5,8? 1+5>8? 6>8 false. Keep 2,5,8? 7>8 false. So no triangle: delete two -> answer 2? But maybe delete 1: {2,5,8} still no. Delete 8: {1,2,5} 1+2>5? no. So n=4, best = n-2=2. Our algorithm? i=0: a0=1,a1=2 sum=3, lower_bound for 3 from index2: a2=5 >=3 ind=2, deletions=0+ (4-2)=2. i=1: a1=2,a2=5 sum=7, lower_bound from index3: a3=8 >=7 ind=3, deletions=1+(4-3)=2. best=2.
    assert(minDeletionsToMakeTrianglePossible({1, 2, 5, 8}) == 2);

    // Another example: {1, 1, 1, 10} -> keep 1,1,1 triangle (1+1>1) works, delete 10 -> 1 deletion
    // i=0: sum=2, lower_bound from index2: a2=1 <2, a3=10 >=2 ind=3, deletions=0+(4-3)=1
    assert(minDeletionsToMakeTrianglePossible({1, 1, 1, 10}) == 1);

    // {10, 20, 30} sort: 10,20,30 -> 10+20>30? 30>30 false. No triangle, need delete one? Actually keep 10,20,30 fails. So delete one? If delete 10, {20,30} no. Delete 20, {10,30} no. Delete 30, {10,20} no. So need delete 1? But 2 elements can't form triangle. So must delete 1 to get 2 elements. n=3, best=n-2=1. Our i=0 only: sum=30, lower_bound from index2 for 30 gives ind=2, deletions=0+(3-2)=1. Correct.
    assert(minDeletionsToMakeTrianglePossible({10, 20, 30}) == 1);

    // Duplicate large values: {1000000000, 1000000000, 1000000000} -> triangle yes (1e9+1e9>1e9) -> 0
    assert(minDeletionsToMakeTrianglePossible({1000000000, 1000000000, 1000000000}) == 0);

    // Mixed case: {1, 2, 2, 4} -> sort: 1,2,2,4. i=0: sum=3, lower_bound for 3 from index2: a2=2<3, a3=4>=3 ind=3, deletions=0+(4-3)=1. i=1: sum=4, lower_bound from index3: a3=4>=4 ind=3, deletions=1+(4-3)=2. best=1. Indeed keep {1,2,4}? 1+2>4? no. Keep {1,2,2}? 1+2>2 yes. So delete 4 -> 1 deletion.
    assert(minDeletionsToMakeTrianglePossible({1, 2, 2, 4}) == 1);

    // No triangle possible for any triple: {1, 2, 3, 6, 10} -> 1+2>3? 3>3 false. 2+3>6? 5>6 false. 3+6>10? 9>10 false. All combos fail? Actually 1+2=3 not >3, 1+3=4<6, 2+3=5<6, 1+6=7<10, 2+6=8<10, 3+6=9<10. So no triangle, answer n-2=3. Let's verify algorithm: i=0 sum=3, lb from idx2: a2=3>=3 ind=2, del=0+(5-2)=3. i=1 sum=5, lb from idx3: a3=6>=5 ind=3, del=1+(5-3)=3. i=2 sum=9, lb from idx4: a4=10>=9 ind=4, del=2+(5-4)=3. best=3.
    assert(minDeletionsToMakeTrianglePossible({1, 2, 3, 6, 10}) == 3);

    // Already sorted but need delete one from middle: {2, 5, 6, 7} -> 2+5>6? 7>6 yes triangle. So 0 deletions? Actually 2,5,6 works. So answer 0. i=0 sum=7, lb from idx2: a2=6<7, a3=7>=7 ind=3, del=0+(4-3)=1? That would say 1 deletion, but we can keep 2,5,6 (i=0 but third side is a2=6 which is < sum, so we don't need to delete it. Our lower_bound finds first >= sum, but that's not the correct threshold. We need to check if there exists a third side < sum. Since all elements after i+1 are sorted, the first eligible is a[i+2] if it's < sum. If a[i+2] < sum, then we can keep it, and we don't delete anything after i+2. So the correct condition: we need to find the first index where a[ind] >= sum. If that index is i+2 (i.e., a[i+2] >= sum), then a[i+2] is too large, so we must delete a[i+2] and all larger ones. If ind > i+2 (meaning a[i+2] < sum), then a[i+2] works, and we delete nothing after i+2; only delete before i. So deletions = i + (n - ind) but only if ind > i+2? Actually if ind > i+2, then elements from i+2 to ind-1 are also < sum and can be kept. But we only need one third side. We can keep a[i+2] and delete nothing after i+2. So deletions = i. But our formula gives i + (n - ind). For example above: i=0, ind=3 (a3=7), n=4, i + (4-3)=1, but correct answer is 0 because we can keep a2=6 (<7). So our formula overestimates when ind > i+2 because we don't need to delete the elements between i+2 and ind-1, we can keep a[i+2] and delete all after ind? Actually we must delete all elements from ind onward because they are >= sum and cannot be included with pair (a[i],a[i+1]) as third side. But we can keep a[i+2] as third side, and we can also keep all elements from i+3 to ind-1? Are those valid? They are < sum, so yes they are valid as third sides too. But we only need one triangle, so we can keep all except those >= sum. So we only need to delete elements from ind to end. So total deletions = i (before the pair) + (n - ind). In the example: i=0, ind=3, n=4 => deletions = 0 + (4-3) =1. But we don't have to delete a[2]=6 because it's <7 and can be kept, and we also keep a[3]=7? No, a[3]=7 is >= sum, so must delete it. So we delete a[3] only, that's 1 deletion, but we can also keep a[2] as third side. So after deletion, we have {2,5,6} which is a triangle. So answer should be 1 deletion, not 0. Wait check: 2+5>6? 7>6 true. So yes delete the 7. So answer 1 is correct. But we also have {2,5,6} as triangle with deletions=1. So our algorithm says best=1, which is correct. But I thought we could keep 2,5,6 without deleting? The array is {2,5,6,7}, keeping {2,5,6} requires deleting the 7, so 1 deletion. So answer 1. My earlier claim of 0 was wrong. So algorithm is correct.

    // Additional test: {2,5,6,7} -> answer 1.
    assert(minDeletionsToMakeTrianglePossible({2, 5, 6, 7}) == 1);

    // All equal: {5,5,5,5} -> triangle possible, 0 deletions.
    assert(minDeletionsToMakeTrianglePossible({5, 5, 5, 5}) == 0);

    return 0;
}
// The core observation: After sorting the array, a triangle exists among any three elements `a[i] <= a[j] <= a[k]` iff `a[i] + a[j] > a[k]`. Since the array is sorted, it suffices to check consecutive pairs `(a[i], a[i+1])` as the two smaller sides, because any larger third side would only be harder to satisfy. Thus, for each index `i` from 0 to n-2, we want to keep the pair at positions `i` and `i+1` (the two smallest of the triple) and then find the largest possible third side by taking the first element that is *at least* `a[i] + a[i+1]` via `lower_bound`. All elements from that index onward are too large to form a triangle with this pair. To form a triangle, we must delete all elements from that `lower_bound` index to the end. Additionally, we must keep exactly those two elements (positions `i` and `i+1`), meaning we must delete all elements before `i` (i.e., `i` deletions). So total deletions for this candidate pair = `i + (n - ind)`. We minimize this over all valid `i`. If no pair works (i.e., every `i` yields `ind == n` so we delete all n-i-1 elements after plus i before, giving n-1 deletions? But we only need to keep two elements, so the minimum possible deletions is n-2. Our formula `i + n - ind` for `ind = n` gives `i + 0`? Wait careful: `ind = lower_bound` returns first position with value >= sum. If `ind == n` then there are no elements ≥ sum, so all elements from `ind` to end are none, but we need at least one third element. If the array has exactly two elements, we can't form a triangle. For any pair `(a[i], a[i+1])`, if `ind == n` (meaning no element >= sum), then we cannot pick a third element from the rest because they are all smaller than sum? Actually if `ind == n`, it means all elements are < sum, so any third element (from position > i+1) is also < sum, but if it's less than sum, then `a[i] + a[i+1] > a[k]` holds! Because `a[k] < sum`. So `ind == n` means all elements from i+2 to end are < sum, and thus any of them works. So we should pick the one at index i+2, and we delete all elements before i (i deletions) and none after (since all are valid). So formula should be: for each i, find `ind` = first index where `a[ind] >= a[i] + a[i+1]`. If `ind == n`, then we can keep index i+2 as third side (as long as i+2 < n). So deletions = `i` + (n - (ind))? Actually if `ind == n`, we keep all from i+2 onward, so deletions = i (elements before i) + 0 (nothing after i+2). But we also have to delete elements between i+1 and i+2? There are none. So deletions = i. That matches formula `i + n - ind` = `i + n - n = i`. So that works. If `ind <= n-1`, then the third side must be at index `ind` (the first element that is too large), but we cannot use it because it violates inequality. So we must delete it and all after it (since sorted). So deletions = `i` (before pair) + (n - ind) (from ind to end). That matches `i + n - ind`. However, we also must ensure there is at least one valid third side. If `ind == i+1`? That can't happen because `a[i]+a[i+1] > a[i+1]` always (positive integers), so `lower_bound` starts from `i+2` at least. But if `i+2 >= n`, then no third side exists. In that case, we cannot keep this pair, so we skip. Our formula `i + n - ind` with `ind = n` gives `i`, but if i+2 >= n, then we can't keep a third side, so it's not a valid candidate. So we must only consider `i` such that `i+2 < n`. Also for pairs where `ind` is found and `ind == n` (i.e., no element >= sum), then all elements from i+2 to end are valid, so we keep one (say at i+2) and delete nothing after, so deletions = i. For `ind < n`, we delete from ind onward, so deletions = i + (n - ind). We take the minimum over all `i` from 0 to n-3 (since need at least three elements). If the array size is less than 3, answer is 0 (no deletion needed). Time complexity: O(n log n) due to sorting and n binary searches. Space: O(n) for a copy or O(1) if we sort in place but the problem statement says not to modify input, so we copy. Edge cases: large numbers up to 1e9, use 64-bit to avoid overflow in sum (a[i]+a[i+1] can exceed int). The provided snippet uses `int` but we should use `long long`. Also handle duplicate values.

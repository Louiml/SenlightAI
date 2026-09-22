/*
You are given two arrays of positive integers representing colors of socks in a drawer: array `a` of length `l` and array `b` of length `r`. All values are integers between 1 and `n` (inclusive), where `n` is the total number of distinct color names possible. You may perform the following operation any number of times: choose one sock from either array and change its color to any other color in 1..n. Your goal is to make both arrays contain exactly the same multiset of colors. Write a C++ function `long long minOperations(int n, vector<int> a, vector<int> b)` that returns the minimum number of color-change operations needed. The input arrays may have any lengths, but `l + r` is at least 1, and both arrays may contain duplicates. The order of socks does not matter. You can assume `1 ≤ n ≤ 200000`. The function must be efficient even for large inputs.
*/

#include <bits/stdc++.h>
using namespace std;

// Returns the minimum number of color-change operations to make two multisets equal.
// a and b are vectors of positive integers (colors) from 1..n.
long long minColorChanges(int n, const vector<int>& a, const vector<int>& b) {
    vector<int> cntA(n + 1, 0), cntB(n + 1, 0);
    for (int x : a) cntA[x]++;
    for (int x : b) cntB[x]++;

    long long totalDiff = 0;
    for (int c = 1; c <= n; ++c) {
        totalDiff += abs(cntA[c] - cntB[c]);
    }
    // Each operation fixes two mismatched positions (one from each side when adjusted)
    return totalDiff / 2;
}

#include <bits/stdc++.h>
using namespace std;

// Include the solution function above.

int main() {
    // Test 1: simple identical
    assert(minColorChanges(3, {1,2,3}, {1,2,3}) == 0);

    // Test 2: one change needed
    assert(minColorChanges(3, {1,2,3}, {1,2,2}) == 1);

    // Test 3: different lengths - need to change extra
    assert(minColorChanges(3, {1,2,3,1}, {1,2,3}) == 1);

    // Test 4: all different
    assert(minColorChanges(2, {1,1}, {2,2}) == 2);

    // Test 5: duplicate in one side
    assert(minColorChanges(4, {1,1,2,2}, {1,2,3,4}) == 2);

    // Test 6: large n, empty side
    assert(minColorChanges(5, {1,2,3}, {}) == 3);

    // Test 7: both empty
    assert(minColorChanges(1, {}, {}) == 0);

    // Test 8: single element each
    assert(minColorChanges(5, {5}, {3}) == 1);

    // Test 9: already balanced with duplicates
    assert(minColorChanges(4, {1,1,2,2}, {2,2,1,1}) == 0);

    // Test 10: many duplicates
    assert(minColorChanges(2, {1,1,1,2}, {2,2,1,1}) == 1);

    cout << "All tests passed!" << endl;
    return 0;
}

// This is a classic problem of matching multisets with minimal edits. Since only color changes are allowed (no insertions/deletions), the two arrays must end up with the same total length. If lengths differ, we must change the excess socks in the longer array. Let `L = max(l, r)` and `S = min(l, r)`. The number of socks that must be changed just to equalize lengths is `(L - S)` because we must recolor those extra socks to match the colors in the shorter array. However, we also need to make the color counts match. The optimal approach: first, match as many identical colors as possible between the two arrays. For each color that appears in both arrays, we can pair up `min(countA[color], countB[color])` socks without any changes. After pairing, the remaining socks in the longer array (after length equalization) must be recolored. Let `extra = L - S`. Among the remaining unpaired socks in the longer array, some may already have the same color as each other (duplicates within the longer array). We can minimize changes by recoloring pairs of duplicates to a color that exists in the shorter array. Specifically, after matching common colors, let `unmatchedA` = number of socks in the longer array not yet matched, `unmatchedB` = number in the shorter array not yet matched. We must recolor `extra` socks from the longer array to fix the length difference. Among the rest of the unmatchedA, some are duplicates within longer array; we can change one of each duplicate pair to match a color in unmatchedB without extra cost beyond the pair change. The known formula: answer = (L - S)/2 + (L + S)/2 - min( number_of_pairs_that_are_duplicates_in_longer, (L - S)/2 ). Actually a simpler derivation: Let `A` and `B` be the counts after removing common pairs. Then `A - B = L - S` must be fixed by changing `(A - B)/2` socks from A to match B's unique colors, and also every change also fixes one sock on each side? Wait: Actually each operation changes one sock's color. After removing common pairs, we have `A` socks in longer array and `B` socks in shorter array, with `A >= B`. To make them equal, we must change exactly `(A - B)` socks in the longer array? No, because if we change a sock in longer array to a color that already exists in shorter array, we reduce both the excess in longer and reduce the deficit in shorter? Let's think: Let counts after matching be `cntA[c]` and `cntB[c]`. The total excess in A over B is `sum max(0, cntA - cntB) = sum over colors where A>B of (cntA - cntB)`. Similarly deficit in B is `sum max(0, cntB - cntA)`. Since total length difference is L-S, we have `excess - deficit = L - S`. Each operation changes one sock: we can change a sock of a color where A>B to a color where B>A. That reduces both excess and deficit by 1. So each such operation reduces the total mismatched count by 2? Actually after one operation, the total number of socks in each array remains the same, but the multiset becomes closer. The minimal number of operations equals `(excess + deficit)/2`? Let's derive: We need to transform multiset A to multiset B. The minimum number of single-element changes to make two multisets equal is exactly the number of elements that differ, which is `(sum |cntA - cntB|)/2`. Because each change fixes two positions (one in A, one in B) if we think of aligning them. Indeed, the formula is: `ans = (sum |cntA[c] - cntB[c]|)/2`. However, we can also think: After removing common pairs, we have counts `d[c] = cntA[c] - cntB[c]`, and sum d = L - S (could be negative if B longer, but we can always assume A is the longer). The number of operations is `(sum |d|)/2`. This equals `(L - S + 2 * sum_positive d? )` Let's just compute directly. In the original code, they use a matching approach with sorting and two pointers. For our solution, we can count frequencies of each color in both arrays. Let `cntA` and `cntB` arrays of size n+1. Then compute `pairs` = sum over c of min(cntA[c], cntB[c]). Then the total unmatched in A = `L - pairs`, in B = `R - pairs`. The number of operations required is `max(unmatchedA, unmatchedB)`? No, because each operation changes one sock, so if we have unmatchedA sockets and unmatchedB sockets, we can change a sock from A to a color that matches an unmatchedB. Each operation reduces unmatchedA by 1 and unmatchedB by 1. So the number of operations is exactly `max(unmatchedA, unmatchedB)`? Actually if unmatchedA > unmatchedB, then we have extra socks in A that have no counterpart in B. We must change those extra socks to colors that exist in B but are oversubscribed? Wait, after pairing, all remaining socks in A have colors that are either not present in B or present but already fully used. Similarly for B. To make multisets equal, we need to align each remaining A sock with a remaining B sock. If unmatchedA != unmatchedB, the difference must be fixed by changing colors of the excess. For example, A has 3 unmatched, B has 1 unmatched. Then we can change 1 A sock to match that B sock (now both have 2 unmatched). Then we have 2 A socks left with no B counterpart. We must change those 2 to colors that exist in B but are already matched? Actually we can change them to any color, but they also need to match the final multiset. The optimal is to change them to colors that are oversubscribed in B? Let's do a formal approach: The minimal number of changes is `(sum|cntA - cntB|)/2`. Because each change fixes two positions (one in A, one in B) if we think of aligning. Indeed, if we sort both arrays and compare, the minimum number of changes to make them equal is the number of positions where they differ after alignment? But alignment is not fixed. The known result: The minimum number of element modifications to turn multiset A into multiset B is `(sum |cntA[c] - cntB[c]|)/2`. Proof: For each color, the excess in A must be changed to some color's excess in B. Each operation reduces both positive and negative excess by 1. So total operations = sum of positive excess = sum of negative excess = (sum |d|)/2. So answer = sum over colors of |cntA - cntB| / 2. We can compute that directly. Edge cases: n can be large, but we only need to iterate over distinct colors present in either array. Complexity O(n + l + r) time and O(n) space. For very large n but small arrays, we can use a hash map to only store present colors. But since n ≤ 200000, we can use arrays.

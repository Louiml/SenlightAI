/*
Write a C++ function `int maximumDistinctSetSize(vector<int>& nums1, vector<int>& nums2)` that, given two integer vectors of equal even length `n` (where `n` is the same for both), returns the maximum possible number of distinct integers that can be placed in a set `S` under the following constraint: from `nums1`, you may choose at most `n/2` elements to include in `S`; from `nums2`, you may choose at most `n/2` elements to include in `S`. The total number of distinct elements in `S` is the size of the union of the chosen elements from both vectors. You can select any subset of up to `n/2` elements from each vector, and the chosen elements from each vector do not have to be distinct from each other (i.e., the same value may appear in both selections), but the set `S` contains each distinct value only once. Return the maximum possible size of `S`.
*/

#include <vector>
#include <unordered_set>

// Given two integer vectors of equal even length, return the maximum number of
// distinct integers that can be selected, picking at most size()/2 elements from each.
int maximumDistinctSetSize(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    int half = static_cast<int>(nums1.size() / 2);

    std::unordered_set<int> set1(nums1.begin(), nums1.end());
    std::unordered_set<int> set2(nums2.begin(), nums2.end());

    // Size of union set1 ∪ set2
    std::unordered_set<int> unionSet(set1);
    unionSet.insert(set2.begin(), set2.end());
    int unionSize = static_cast<int>(unionSet.size());

    int takeFrom1 = std::min(half, static_cast<int>(set1.size()));
    int takeFrom2 = std::min(half, static_cast<int>(set2.size()));

    return std::min(unionSize, takeFrom1 + takeFrom2);
}

#include <cassert>
#include <vector>

// Function declaration (normally in header)
int maximumDistinctSetSize(const std::vector<int>& nums1, const std::vector<int>& nums2);

int main() {
    // Example 1: No overlap, both have enough distinct values
    std::vector<int> a1 = {1, 2, 3, 4};
    std::vector<int> b1 = {5, 6, 7, 8};
    assert(maximumDistinctSetSize(a1, b1) == 4);

    // Example 2: All common, but enough distinct values to fill picks
    std::vector<int> a2 = {1, 2, 3, 4};
    std::vector<int> b2 = {1, 2, 3, 4};
    assert(maximumDistinctSetSize(a2, b2) == 4);

    // Example 3: Overlap limits union size
    std::vector<int> a3 = {1, 2, 3};
    std::vector<int> b3 = {1, 2, 3}; // n=3 but must be even? The problem says equal even length, so odd not allowed. Adjust:
    // Use n=4, but with limited distinct values:
    std::vector<int> a3b = {1, 1, 2, 2};
    std::vector<int> b3b = {1, 1, 3, 3};
    assert(maximumDistinctSetSize(a3b, b3b) == 3); // union {1,2,3} size 3, picks: min(2,2)+min(2,2)=4, min=3

    // Example 4: One vector has fewer distinct than half
    std::vector<int> a4 = {1, 1, 1, 1};
    std::vector<int> b4 = {1, 2, 3, 4};
    assert(maximumDistinctSetSize(a4, b4) == 2); // from a4 take at most 1 distinct, from b4 take 2, union size 4, min(4, 1+2)=3? Wait: set1.size()=1, half=2, takeFrom1=1; set2.size()=4, takeFrom2=2; sum=3; union={1,2,3,4} size 4; min=3. But can we get 3? Pick 1 from a4, pick 2 and 3 from b4 -> that's 3 distinct. Yes.

    // Example 5: Empty input (n=0)
    std::vector<int> a5;
    std::vector<int> b5;
    assert(maximumDistinctSetSize(a5, b5) == 0);

    // Example 6: Mixed duplicates and overlap
    std::vector<int> a6 = {1, 2, 2, 3};
    std::vector<int> b6 = {3, 4, 4, 5};
    // set1={1,2,3} size 3, set2={3,4,5} size 3, union size=5, half=2, takeFrom1=2, takeFrom2=2, sum=4, min(5,4)=4
    assert(maximumDistinctSetSize(a6, b6) == 4);

    return 0;
}

// The problem is equivalent to maximizing the number of distinct values we can "cover" by picking at most `n/2` values from each of two sets, where `nums1` and `nums2` may have duplicates. Let `set1` be the set of unique values in `nums1`, `set2` in `nums2`, and let `common` be the set of values present in both.
//
// We can always pick any distinct value from a vector as long as we do not exceed `n/2` picks from that vector. There are two constraints: (1) the number of distinct values we can take from `set1` is at most `min(|set1|, n/2)` because we cannot take more distinct values than exist, and also cannot take more than `n/2` picks; similarly for `set2`. (2) However, if we take the same value from both vectors, it only counts once in `S`. So the naive upper bound is `min(|set1|, n/2) + min(|set2|, n/2)`, but we must subtract the overlap where we double-count common values.
//
// The optimal strategy: we want to take as many distinct values as possible. First, note that we can always take up to `n/2` distinct values from each vector if they have at least that many distinct elements. If a vector has fewer than `n/2` distinct elements, we can take all of them. The only issue is when both vectors have enough distinct elements to fill their quota, but the sets overlap heavily. In such cases, we may be forced to reduce the total because if we take a common value from both, we waste one pick.
//
// We can approach greedily: First, take as many non-common (unique-to-one-vector) values as possible, because they don't risk double counting. Then fill the remaining quota with common values, but only count each common value once. The maximum total is:
// - Let `only1 = |set1| - commonCount`, `only2 = |set2| - commonCount`, where `commonCount` is the number of values in both sets.
// - We can take at most `min(only1, n/2)` values from only1, `min(only2, n/2)` from only2.
// - After that, we have remaining quota `r1 = max(0, n/2 - takenFrom1)` and `r2 = max(0, n/2 - takenFrom2)` for common values. We can take at most `min(commonCount, r1 + r2)` distinct common values, because each common value taken consumes one pick from one of the two vectors, and we can distribute the picks between the two vectors arbitrarily.
//
// However, there is a subtlety: if we take a common value, it doesn't matter which vector's quota it uses. Since we have two independent quotas of size `n/2`, we can allocate picks for common values to either vector. So the total distinct values we can get is:
// `min(only1, n/2) + min(only2, n/2) + min(commonCount, max(0, n/2 - min(only1, n/2)) + max(0, n/2 - min(only2, n/2)))`.
// But also, the total cannot exceed `|set1 ∪ set2|` = `only1 + only2 + commonCount`. The formula above already respects that because `commonCount` is limited by the remaining quota sum.
//
// We can simplify: since `n` is even, `n/2` is an integer. The total distinct values we can choose is bounded by `n` total picks (from both vectors combined), but the union size is at most `|set1| + |set2| - commonCount`. However, the constraint that each vector individually can contribute at most `n/2` means the maximum union is also at most `min( |set1| + |set2| - commonCount, n` ). Actually, the maximum union cannot exceed `n` because we have only `n` total picks, and each pick can contribute at most one new distinct value, but duplicates waste picks. The optimal strategy is to avoid duplicates as much as possible.
//
// A simpler method known from the original problem: Let `a = min(nums1.size()/2, set1.size())` and `b = min(nums2.size()/2, set2.size())`. Let `c = |set1 ∪ set2|`. The answer is `min(c, a + b)`? Wait, that would be wrong because if `set1` and `set2` overlap heavily, `a + b` might exceed `c` but we can still get `c` if `c <= a + b`? Let's test: Suppose `nums1` has all distinct 4 values, `nums2` has the same 4 values, `n=4`. Then `set1.size()=4`, `set2.size()=4`, `a = min(2,4)=2`, `b=2`, `c=4`, `min(4, 4)=4`? But can we get 4 distinct values? We have only 2 picks from each vector, total 4 picks, and all values are common. We can pick 2 from nums1 and 2 from nums2, but if they are the same values, we might get only 2 distinct. Actually we can pick different values: pick values 1 and 2 from nums1, and values 3 and 4 from nums2. That gives 4 distinct. So yes 4 is possible. So `a + b = 4`, `c = 4`, min is 4.
//
// What if `set1` has 4 distinct, `set2` has 4 distinct, but they share 3 common, so union size is 5, and `n=4`. Then `a=2`, `b=2`, sum = 4, min(5,4)=4. Is 4 possible? We have 2 picks from each, we can pick 2 from nums1 that are unique to nums1 if they exist? But if common=3, then only1 = 1, only2 = 1. We can pick that one unique from each, that's 2 distinct, then we have 2 picks left (one from each vector) and we can pick a common value from one of them, but then we still have one more pick left that would be a duplicate if we pick the same common value again. To maximize, we pick the unique from nums1, unique from nums2, then pick common value from nums1, and then pick another common value from nums2 (but that would be a duplicate of the first common? No, we can pick a different common value). Since commonCount=3, we can pick two different common values. So total = 2 unique + 2 common = 4. So yes.
//
// What about a case where the answer is less than `a + b`? Suppose `nums1` has 3 distinct values, all common with `nums2`, and `nums2` has 3 distinct values, all common, `n=4` (so `n/2=2`). Then `set1.size()=3`, `set2.size()=3`, `a = min(2,3)=2`, `b = min(2,3)=2`, `a+b=4`, but `c=3` (since all common). Can we get 3 distinct? We have 2 picks from each, total 4 picks but only 3 distinct values exist. We can pick values 1 and 2 from nums1, and value 3 from nums2, that's 3 distinct. We can't get 4. So answer is 3, which is `min(c, a+b)` = `min(3,4)=3`. So that formula works.
//
// But wait, is it always true that the answer is `min( |set1 ∪ set2|, min(|set1|, n/2) + min(|set2|, n/2) )`? Let's test a counterexample: `nums1` has 4 distinct values, `nums2` has 4 distinct values, and they share 2 common, so union size = 6, `n=4`, `n/2=2`. Then `a = min(4,2)=2`, `b=2`, sum = 4, min(6,4)=4. Is 4 possible? We have 2 picks from each, we can pick 2 unique from nums1 and 2 unique from nums2 (since there are 2 unique in each: total distinct 4, unique in nums1 = 2, unique in nums2 = 2), that gives 4 distinct. Yes.
//
// What if `nums1` has 5 distinct, `nums2` has 5 distinct, they share 4 common, so only1=1, only2=1, union=6, `n=6`? But task says equal even length, so `n` must be even. Let's set `n=6`, `n/2=3`. Then `a = min(5,3)=3`, `b=3`, sum=6, c=6, min=6. Can we get 6? We have 3 picks from each. We can pick the 1 unique from nums1, the 1 unique from nums2, and then from each vector we have 2 picks left, but we have 4 common values. We can pick 2 common from nums1 and 2 common from nums2, but if we pick different common values, that's 4 more distinct? But we only have 2 picks left in each vector, so we can pick 2 common from nums1 and 2 common from nums2, but those 4 are from the 4 common, so total distinct = 1+1+4=6. So yes.
//
// But what about a case where `a + b` is large but `c` is smaller? That's fine, min handles it. The question is whether `min(c, a+b)` always works. Let's think: We have two independent selection quotas. The number of distinct values we can select is at most the size of the union minus the number of duplicates we are forced to take. Since we can choose which values to take, we can always avoid duplicates as long as we have enough unique values to fill the quotas. If we have more unique values than the total picks, we can fill all picks with distinct values. If not, we must repeat some, but the total distinct is capped by the union size. The sum `a+b` represents the maximum number of picks we can make (which is `min(|set1|, n/2) + min(|set2|, n/2)`) but some of those picks might be duplicates between the two vectors. However, because we have two separate quotas, we can coordinate: we can prioritize picking values that are unique to one vector first, and then fill with common values. The maximum distinct values we can get is exactly `min( |set1 ∪ set2|, min(|set1|, n/2) + min(|set2|, n/2) )`. Why? Because we cannot exceed the union, and we cannot exceed the total number of picks. And we can always achieve that bound by this greedy: take as many unique-to-nums1 values as possible up to `n/2`, then unique-to-nums2 up to `n/2`, then fill any remaining picks with common values, but only counting each once. If we run out of common values, we stop. The number of distinct values we get is `min(only1, n/2) + min(only2, n/2) + min(commonCount, max(0, n/2 - min(only1, n/2)) + max(0, n/2 - min(only2, n/2)))`. This simplifies to `min( only1 + only2 + commonCount, n/2 + n/2 )`? Let's check: `only1 + only2 + commonCount` = union size. And the sum of the three terms is exactly `min( unionSize, n )` because we can fill up to `n` picks, but each pick that is a unique or common value gives a distinct increment, and we stop when we either exhaust all distinct values or exhaust the total picks. But careful: the total picks is `n/2 + n/2 = n` only if both vectors have at least `n/2` distinct values; if one has fewer, we have fewer picks. Indeed, the total picks we can make is exactly `min(|set1|, n/2) + min(|set2|, n/2)`, not necessarily `n`. So the formula `min(unionSize, min(|set1|, n/2) + min(|set2|, n/2))` correctly captures that. So the answer is `min( sizeOfUnion, min(set1.size(), n/2) + min(set2.size(), n/2) )`.
//
// Edge cases: Duplicate values within each vector are handled by using sets. `n` is even and same for both. The vectors may be empty? The problem probably assumes at least one element, but if `n=0`, then `n/2=0`, answer is 0. The function should handle that. Time complexity: O(n) to build sets, O(n) to compute union size. Space: O(n) for sets.

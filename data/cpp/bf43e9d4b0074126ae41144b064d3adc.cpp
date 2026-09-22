Given an array of \(n\) integers and a sequence of range increment operations \([l, r]\) (1-indexed) by a value \(d\), write a C++ function `maxHillAfterUpdates` that, after each update, returns the length of the longest contiguous subarray of the *adjacent differences* (i.e., differences between consecutive original array elements) that is a "hill". A hill is defined as a non-empty contiguous sequence where all elements are non-zero, and the sequence is strictly increasing then strictly decreasing (the peak can be at the very start or very end, but no two consecutive differences have the same sign; zeros are breaks). After applying each update to the original array (the difference at position \(l-1\) increases by \(d\), and the difference at position \(r-1\) decreases by \(d\), if those positions exist), output the length of the longest hill in the modified difference array plus 1 (why plus 1 is specified in analysis). The initial array is given, and updates are applied cumulatively. The function should handle up to \(n \leq 100,000\) and \(m \leq 100,000\) updates efficiently; every update changes at most two elements of the difference array (each to a new value that may be zero or non-zero), and you must query the global maximum after each update.
#include <cassert>
#include <vector>
#include <tuple>
#include <cstdint>

// Include the solution code here (or copy the function above)
// For brevity, assume the function is declared above.

int main() {
    // Case 1: Given example from problem statement style
    // n=4, initial array a = [1,2,4,8] -> diffs = [1,2,4] (all positive)
    // Update range [2,3] by +1: affects diff[1] +=1 (2->3) and diff[3] if r<n? r=3, n=4 so diff[2]? Actually r=3 <4 => diff[2] -=1 (4->3) => diffs become [1,3,3]
    // All positive, longest hill differences length = 3 (since all positive is a valid hill? Yes, a strictly increasing sequence of positive differences is a hill). Answer = 4.
    std::vector<int64_t> diffs1 = {1, 2, 4};
    std::vector<std::tuple<int,int,int64_t>> updates1 = {{2,3,1}, {1,4,0}};
    auto res1 = maxHillAfterUpdates(diffs1, updates1);
    assert(res1.size() == 2);
    assert(res1[0] == 4); // after first update, diffs [1,3,3] all positive -> maxHill=3 -> answer 4
    assert(res1[1] == 4); // no change -> still 4

    // Case 2: Single element array -> no differences, maxHill=0, answer always 1
    std::vector<int64_t> diffs2 = {};
    std::vector<std::tuple<int,int,int64_t>> updates2 = {{1,1,5}, {1,1,-3}};
    auto res2 = maxHillAfterUpdates(diffs2, updates2);
    assert(res2.size() == 2);
    assert(res2[0] == 1);
    assert(res2[1] == 1);

    // Case 3: Array [0,0,0] -> diffs = [0,0] (all zeros) -> maxHill=0, answer 1
    // Update range [2,2] by +3: diff[1] +=3 (0->3), diff[2]? n=3, r=2 <3 => diff[1] -=3? Actually both indices same? l=2, r=2: l>1 so diff[0] +=3; r<3 so diff[1]? r-1=1, diff[1] -=3 -> diffs become [3,-3] -> hill of length 2 -> answer 3
    std::vector<int64_t> diffs3 = {0, 0};
    std::vector<std::tuple<int,int,int64_t>> updates3 = {{2,2,3}};
    auto res3 = maxHillAfterUpdates(diffs3, updates3);
    assert(res3.size() == 1);
    assert(res3[0] == 3); // diffs [3,-3] positive then negative -> maxHill=2 -> answer 3

    // Case 4: Mixed signs
    // Array diffs = [-2, 1, 1, -3, 5] -> maxHill? Longest hill could be [1,1,-3]? That is positive, positive, negative -> valid (positive then negative) length 3 -> answer 4? Also [-2] alone length1. Actually [1,1,-3] length3 -> answer4. Update l=2,r=3 with d=2: diff[1]+=2 (1->3), diff[2]-=2 (1->-1) -> diffs [-2,3,-1,-3,5] -> hill [3,-1,-3]? length3 (positive, negative, negative? For a hill, the sequence must be non-zero and sign pattern must be either all negative, all positive, or positive then negative, but negative then negative is not allowed because two negatives in a row would break the "strictly decreasing then "? Actually the definition: a hill is a sequence that is non-zero and never has two consecutive same-sign? Wait from the monoid behavior: a single negative is a hill, a single positive is a hill. Combining negative+negative? The combine checks: `positiveSuffix` (from left) + `negativePrefix` (from right) gives a valid hill only if left ends with positives and right starts with negatives. Two negatives in a row would have `positiveSuffix=0` from left? Actually if left is all negative, positiveSuffix=0, so can't combine. So two negatives are NOT a hill together. So in [-2,3,-1,-3,5], the subsequence [3,-1,-3] has signs +,-,- which is not valid because the second - followed by - is same sign. Only [3,-1] length2 is a hill. Also [-2,3] gives negative then positive? Not allowed because pattern must be positive then negative, not negative then positive? Actually the definition: "strictly increasing then strictly decreasing" in original array means differences are positive then negative. A single negative is trivially increasing (no positive part) then decreasing? Hmm the problem from the code allows a single negative as a hill (as seen from Sum for val<0, hillPrefix=1). So a hill can be all negative, all positive, or non-empty positives then non-empty negatives. So [-2,3] is negative then positive, not allowed because that would be decreasing then increasing. So longest hill after update: [-2] length1, [3,-1] length2, [-3] length1, [5] length1 -> maxHill=2 -> answer3. Let's compute and assert.
    std::vector<int64_t> diffs4 = {-2, 1, 1, -3, 5};
    std::vector<std::tuple<int,int,int64_t>> updates4 = {{2,3,2}};
    auto res4 = maxHillAfterUpdates(diffs4, updates4);
    assert(res4.size() == 1);
    assert(res4[0] == 3); // maxHill=2 -> answer 3

    // Case 5: Stress small: n=3, diffs [1, -1], update [2,2] by +1: diff[0]+=1 (1->2), diff[1]-=1 (-1->-2) -> [2,-2] still hill length 2 -> answer 3
    std::vector<int64_t> diffs5 = {1, -1};
    std::vector<std::tuple<int,int,int64_t>> updates5 = {{2,2,1}};
    auto res5 = maxHillAfterUpdates(diffs5, updates5);
    assert(res5.size() == 1);
    assert(res5[0] == 3);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdint>

struct HillSegment {
    int size;
    int negativePrefix;   // longest prefix of all negative numbers
    int positiveSuffix;   // longest suffix of all positive numbers
    int hillPrefix;       // longest prefix that is a valid hill (non-zero, sign pattern of negatives only, positives only, or positives then negatives)
    int hillSuffix;       // longest suffix that is a valid hill (mirror: negatives only, positives only, or negatives then positives)
    int maxHill;          // longest contiguous hill inside this segment

    HillSegment() : size(0), negativePrefix(0), positiveSuffix(0), hillPrefix(0), hillSuffix(0), maxHill(0) {}

    explicit HillSegment(int64_t val) :
        size(1),
        negativePrefix(val < 0 ? 1 : 0),
        positiveSuffix(val > 0 ? 1 : 0),
        hillPrefix(val != 0 ? 1 : 0),
        hillSuffix(val != 0 ? 1 : 0),
        maxHill(val != 0 ? 1 : 0)
    {}

    bool isAllNegative() const { return negativePrefix == size; }
    bool isAllPositive() const { return positiveSuffix == size; }
    bool isHillPrefix() const { return hillPrefix == size; }
    bool isHillSuffix() const { return hillSuffix == size; }

    HillSegment operator+(const HillSegment& other) const {
        HillSegment res;
        res.size = size + other.size;
        // negativePrefix: if this whole segment is negative, extend with other's negativePrefix; else keep own
        res.negativePrefix = isAllNegative() ? size + other.negativePrefix : negativePrefix;
        // positiveSuffix: if other is all positive, extend own positiveSuffix with whole other; else keep other's
        res.positiveSuffix = other.isAllPositive() ? positiveSuffix + other.size : other.positiveSuffix;
        // hillPrefix: if this whole segment is a hill prefix
        res.hillPrefix = isHillPrefix() ? size + (isAllPositive() ? other.hillPrefix : other.negativePrefix) : hillPrefix;
        // hillSuffix: if other is a hill suffix
        res.hillSuffix = other.isHillSuffix() ? (other.isAllNegative() ? hillSuffix : positiveSuffix) + other.size : other.hillSuffix;
        // maxHill: take max of children, then try combining left positiveSuffix + right hillPrefix (if left ends with positives and right starts a hill that can begin with positives)
        res.maxHill = std::max(maxHill, other.maxHill);
        res.maxHill = std::max(res.maxHill, positiveSuffix + other.hillPrefix);
        res.maxHill = std::max(res.maxHill, hillSuffix + other.negativePrefix);
        return res;
    }
};

// Segment tree supporting point updates and full-range query of the concatenation monoid.
class HillSegmentTree {
private:
    std::vector<HillSegment> tree;
    int base; // smallest power of two >= size

    void build(const std::vector<int64_t>& diffs) {
        base = 1;
        while (base < (int)diffs.size()) base <<= 1;
        tree.assign(base * 2, HillSegment());
        for (int i = 0; i < (int)diffs.size(); ++i) {
            tree[base + i] = HillSegment(diffs[i]);
        }
        for (int i = base - 1; i > 0; --i) {
            tree[i] = tree[i * 2] + tree[i * 2 + 1];
        }
    }

public:
    // diffs: size n-1 (may be 0 if array length 1)
    explicit HillSegmentTree(const std::vector<int64_t>& diffs) {
        build(diffs);
    }

    void set(int index, int64_t value) {
        index += base;
        tree[index] = HillSegment(value);
        for (index >>= 1; index > 0; index >>= 1) {
            tree[index] = tree[index * 2] + tree[index * 2 + 1];
        }
    }

    HillSegment getWhole() const {
        return tree[1];
    }
};

// Main solution: after each range increment update, return the longest hill length in original array
// diffs: adjacent differences (size n-1), a: original array (size n) — but we only need diffs
// updates: list of (l, r, d) 1-indexed inclusive.
std::vector<int> maxHillAfterUpdates(
    const std::vector<int64_t>& initialDiffs,
    const std::vector<std::tuple<int,int,int64_t>>& updates
) {
    std::vector<int64_t> diffs = initialDiffs;
    int n = (int)diffs.size() + 1;
    HillSegmentTree tree(diffs);
    std::vector<int> answers;
    answers.reserve(updates.size());

    for (const auto& [l, r, d] : updates) {
        // l and r are 1-indexed on original array
        if (l > 1) {
            int idx = l - 2; // 0-indexed difference position
            diffs[idx] += d;
            tree.set(idx, diffs[idx]);
        }
        if (r < n) {
            int idx = r - 1; // 0-indexed difference position (since difference between a[r-1] and a[r])
            diffs[idx] -= d;
            tree.set(idx, diffs[idx]);
        }
        HillSegment whole = tree.getWhole();
        int maxHill = whole.maxHill;
        // plus 1 because a hill of k differences corresponds to k+1 original elements
        answers.push_back(maxHill + 1);
    }
    return answers;
}
// The key observation is that the problem reduces to maintaining a sequence of adjacent differences \(d_i = a_{i+1} - a_i\) (length \(n-1\)), where each update to a range \([l, r]\) in the original array modifies at most two differences: \(d_{l-1} += d\) and \(d_{r-1} -= d\) (if those indices are in valid range \(1\) to \(n-1\)). After each update, we need the maximum length of a contiguous subarray of these differences that forms a "hill": a sequence where all elements are non-zero, and the signs alternate according to a pattern that is either all negative, all positive, or strictly increasing then strictly decreasing (i.e., a sequence that is non-increasing in absolute value? Actually the definition is: the sequence must be non-zero and must be either entirely decreasing (all negative), entirely increasing (all positive), or a combination: first strictly increasing (positive differences) then strictly decreasing (negative differences), but the increase and decrease are measured by sign changes, not magnitude — wait let's reinterpret from the Sum struct). The Sum struct maintains for a segment: `negativePrefix` = length of longest prefix consisting entirely of negative values; `positiveSuffix` = length of longest suffix consisting entirely of positive values; `hillPrefix` = length of longest prefix that is a "hill" (i.e., non-zero and its sign pattern is either all negative, all positive, or positive then negative from left to right, with no two adjacent same-sign? Actually the combine logic suggests a hill prefix must be either all negative (if the left segment is negative) or all positive, or positive-then-negative. The hill suffix similarly is either all positive, all negative, or negative-then-positive? Let's understand: For a segment, `hillPrefix` is the length of the longest prefix that is a valid hill where signs are allowed to be all negative, all positive, or start with positives then negatives. `hillSuffix` is similarly the longest suffix that is a valid hill where signs are all negative, all positive, or start with negatives then positives (mirror). The `maxHill` is the maximum length of any contiguous hill inside the segment. When concatenating two segments, a new hill can be formed by taking a positive suffix of the left segment and a hill prefix of the right segment that begins with positive values (since the left ends with positives and right starts with positives, they merge? Actually code: `res.maxHill = max(maxHill, that.maxHill); amax(res.maxHill, positiveSuffix + that.hillPrefix); amax(res.maxHill, hillSuffix + that.negativePrefix);` — the first combination is: left has a suffix of all positive numbers, right has a hill prefix. If right's hill prefix starts with positives (since the `hillPrefix` property when right is not entirely negative includes positive-then-negative? Let's deduce: The segment is treated as a monoid. For a single value: if val<0, negativePrefix=1, positiveSuffix=0, hillPrefix=1 (since a single negative is a hill), hillSuffix=1. If val>0: negativePrefix=0, positiveSuffix=1, hillPrefix=1, hillSuffix=1. If val==0: all are 0. Combining: `hillPrefix` of the concatenation: if left is entirely negative (isNegative()) then you can take left.length + that.hillPrefix? Actually code: `res.hillPrefix = isHill() ? size + (isPositive() ? that.hillPrefix : that.negativePrefix) : hillPrefix;` Wait `isHill()` means `hillPrefix == size`, i.e., the entire left segment is a hill prefix (i.e., it is a valid hill that can be extended? The combine logic is: If the left segment is a hill prefix (meaning it is a valid hill that could be extended to the right), then the new hill prefix is the entire left plus either the right's hill prefix (if left's entire segment is positive, because then a hill can continue with positives or negative? Actually if left is all positive, then adding right's hill prefix — which can be all positive or positive-then-negative — maintains the increasing-then-decreasing shape. If left is not all positive but is a hill prefix (which means it must be all negative? Because if left is a hill prefix and not all positive, then it must be all negative? Let's test: A hill prefix can be all negative. If left is all negative, then right's negativePrefix can be added to keep it all negative, which is still a hill. So code: `isPositive() ? that.hillPrefix : that.negativePrefix`. Similarly for hillSuffix. This is tricky but the important part is that the monoid correctly maintains the maximum hill length. The final answer after each update is `sum.maxHill + 1`, because a subarray of differences of length \(k\) corresponds to a subarray of the original array of length \(k+1\). So the longest hill in differences of length `maxHill` gives `maxHill+1` original elements. Edge cases include empty difference array when \(n=1\), then `maxHill=0` and answer 1; comparisons with zero break hills; updates may create or destroy zeros at the boundaries. Use a segment tree with the `Sum` node supporting point updates and global query (whole tree). For each update, modify the two affected difference values (if in range) and set them in the segment tree. Complexity: each update is \(O(\log n)\), each segment tree operation is \(O(\log n)\), with constant factor per combine; total \(O((n+m)\log n)\) time, \(O(n)\) space.

Given two permutations `nums1` and `nums2` of the same length `n` (containing integers from `0` to `n-1`), write a C++ function `long long countGoodTriplets(const std::vector<int>& nums1, const std::vector<int>& nums2)` that returns the number of triples of indices `(i, j, k)` with `i < j < k` such that in both permutations, the values at those indices appear in the same relative order. In other words, the triple is “good” if the relative ordering of the three values is identical in both permutations. The permutations are guaranteed to be valid and of length at least 3, but your function should still work for any length. Return the count as a 64-bit integer.

// The problem asks for counting triples of values (not positions) that maintain the same relative order in both permutations. Since the elements are a permutation of `0..n-1`, we can map one permutation to the identity order. Specifically, for each value `v`, record its position in `nums2` (`pos2[v] = index`). Then, for each position in `nums1`, we create an array `rev` where `rev[ pos2[nums1[i]] ] = i`. This `rev` array is a permutation of `0..n-1` such that if we process values in increasing order of their position in `nums2`, the value `rev[pos]` gives its position in `nums1`. Now, the condition for a triple of values `a < b < c` (by index in the original ordering, which is just their numeric value) to be good is that their relative order in `nums1` (given by `rev`) is also increasing. That is, if we process values from `0` to `n-1`, when we reach value `x` (which corresponds to position `pos = rev[x]`), the number of previously processed values that appear before `pos` in `nums1` (i.e., have `rev` index less than `pos`) are exactly the possible left partners for triples where `x` is the middle element. Similarly, the number of future values (larger than `x`) that appear after `pos` in `nums1` are the right partners. The count of triples with `x` as the middle value is `left * right`. Iterating `x` from 0 to n-1 and updating a Fenwick tree of seen positions gives us `left = query(pos)` (number of seen values with position ≤ pos). Then `right = (n - 1 - pos) - (x - left)` because among the `x` processed values, `left` are before `pos`, so `x - left` are after `pos`; the total number of values after `pos` is `n - 1 - pos`; subtracting those already seen (`x - left`) leaves future values after `pos`. Edge cases: n < 3 yields 0; duplicates never occur since input is a permutation. Time complexity is O(n log n) per call, space O(n). Fenwick tree operations are O(log n).

#include <vector>

class FenwickTree {
private:
    std::vector<int> tree;
public:
    FenwickTree(int size) : tree(size + 1, 0) {}
    void update(int index, int delta) {
        index++;
        while (index < (int)tree.size()) {
            tree[index] += delta;
            index += index & -index;
        }
    }
    int query(int index) const {
        index++;
        int res = 0;
        while (index > 0) {
            res += tree[index];
            index -= index & -index;
        }
        return res;
    }
};

// Count triples (i<j<k) whose relative order is same in both permutations.
long long countGoodTriplets(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    const int n = (int)nums1.size();
    if (n < 3) return 0;

    std::vector<int> pos2(n);
    for (int i = 0; i < n; ++i) {
        pos2[nums2[i]] = i;
    }

    // rev[pos] = position of value in nums1, where pos = position in nums2
    std::vector<int> rev(n);
    for (int i = 0; i < n; ++i) {
        rev[pos2[nums1[i]]] = i;
    }

    FenwickTree bit(n);
    long long result = 0;
    for (int value = 0; value < n; ++value) {
        int pos = rev[value];          // position of this value in nums1
        int left = bit.query(pos);     // how many smaller values are before pos
        bit.update(pos, 1);            // mark this value as seen
        int right = (n - 1 - pos) - (value - left);
        result += (long long)left * right;
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    // Simple increasing permutations -> all triples are good
    assert(countGoodTriplets({0,1,2}, {0,1,2}) == 1);
    assert(countGoodTriplets({0,1,2,3}, {0,1,2,3}) == 4);

    // Reverse both permutations -> no triples have same relative order (since order reversed)
    assert(countGoodTriplets({3,2,1,0}, {3,2,1,0}) == 0);

    // n=3, a single swap changes relative order
    assert(countGoodTriplets({0,1,2}, {0,2,1}) == 0);
    assert(countGoodTriplets({0,2,1}, {0,1,2}) == 0);

    // Mixed example from problem context
    assert(countGoodTriplets({2,0,1,3}, {0,1,2,3}) == 1);
    assert(countGoodTriplets({1,0,2,3}, {0,1,2,3}) == 1);

    // Larger case
    assert(countGoodTriplets({0,1,2,3,4}, {4,3,2,1,0}) == 0);
    assert(countGoodTriplets({1,0,2,3,4}, {1,2,0,3,4}) == 1);

    // Edge: n<3
    assert(countGoodTriplets({0}, {0}) == 0);
    assert(countGoodTriplets({0,1}, {1,0}) == 0);

    // Known triple count for length 4 with one inversion
    assert(countGoodTriplets({0,2,1,3}, {0,1,2,3}) == 1);
    assert(countGoodTriplets({0,1,3,2}, {0,1,2,3}) == 1);

    return 0;
}

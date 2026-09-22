/*
Write a C++ function `int minimalTotalMoves(std::vector<int> a)` that takes a non-empty vector of integers (the initial quantities of items in `n` bins, where `n` is the vector's size) and returns the minimum number of moves required to make all bins contain equal quantities, where a single move transfers exactly one item from one bin to any other bin. The input may contain any negative? No — assume all quantities are non-negative integers (≥0). The total sum of all quantities may or may not be divisible by the number of bins; in the latter case, the best achievable equalization makes some bins have `floor(sum/n)` and some have `ceil(sum/n)`, with the number of `ceil` bins being `sum % n`. The function must compute and return the minimal total number of item moves, which is guaranteed to be an integer (the sum of positive deviations divided by 2, as each move fixes one overage and one underage). Handle duplicate values, large sums (up to 10^5 bins, each up to 10^4), and edge cases such as all bins already equal or a single bin.
*/

#include <vector>
#include <algorithm>
#include <cstdlib>

// Return the minimal total moves to equalize bin quantities as much as possible.
// 'a' is a vector of non-negative integers representing initial quantities in each bin.
int minimalTotalMoves(std::vector<int> a) {
    const int n = static_cast<int>(a.size());
    if (n == 0) return 0;

    long long total = 0;
    for (int value : a) total += value;

    const int base = static_cast<int>(total / n);
    const int extra = static_cast<int>(total % n);

    std::sort(a.begin(), a.end());

    std::vector<int> target(n);
    for (int i = 0; i < n; ++i) {
        // The largest 'extra' indices get base+1 in sorted order.
        if (i >= n - extra) target[i] = base + 1;
        else target[i] = base;
    }

    long long diff_sum = 0;
    for (int i = 0; i < n; ++i) {
        diff_sum += std::abs(a[i] - target[i]);
    }

    return static_cast<int>(diff_sum / 2);
}

#include <cassert>
#include <vector>

int main() {
    // Already equal.
    assert(minimalTotalMoves({5, 5, 5}) == 0);
    // Single bin.
    assert(minimalTotalMoves({7}) == 0);
    // Sum divisible: move 1 from bin0 to bin2.
    assert(minimalTotalMoves({1, 2, 3}) == 1);
    // Sum not divisible: e.g., sum=10, n=3 → targets {3,3,4}. Sorted original {1,4,5} → diff {2,1,1} sum=4 → moves=2.
    assert(minimalTotalMoves({1, 4, 5}) == 2);
    // Duplicates and zero.
    assert(minimalTotalMoves({0, 0, 6}) == 2);
    // Larger case: {10, 0, 5} sum=15, n=3 → targets {5,5,5} → sorted {0,5,10} diff {5,0,5} sum=10 → moves=5.
    assert(minimalTotalMoves({10, 0, 5}) == 5);
    // Already with extra: {4,4,5} sum=13 → targets {4,4,5} → diff 0.
    assert(minimalTotalMoves({4, 4, 5}) == 0);
    // Large n but small values.
    std::vector<int> many(100000, 1);
    assert(minimalTotalMoves(many) == 0);
    // One bin has all, rest zero.
    assert(minimalTotalMoves({8, 0, 0, 0}) == 3);
    return 0;
}

// The optimal strategy is to sort the original quantities and then construct the target distribution: if `sum % n == r`, then the `r` bins with the largest targets receive `sum/n + 1` and the remaining `n-r` receive `sum/n`. After sorting both the original array and the target array in the same order (ascending), the minimal number of moves is half the sum of absolute differences between corresponding elements. This is because each move transfers one unit from a bin that has a surplus relative to its target to a bin with a deficit; matching sorted orders minimizes the sum of absolute deviations (the rearrangement inequality). Edge cases: when `r == 0`, all targets are equal; when `n == 1`, the answer is 0; when values already match, the sum of absolute differences is 0. Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the target array.

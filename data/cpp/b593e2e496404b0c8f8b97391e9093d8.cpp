/*
Write a C++ function that takes a `std::vector<double>` of floating-point values and sorts it using a custom comparison that treats all NaN values as equal and less than any finite number, and treats negative zero and positive zero as equal. The function must not modify the input vector; instead, it should return a new sorted vector. The sorting should be stable (preserve the relative order of elements that compare equal). The input vector may contain any combination of finite values, infinities, NaNs, and negative zero. The function should handle an empty input and a single-element input correctly.
*/

#include <vector>
#include <algorithm>
#include <cmath>

// Sort a copy of the input vector with a custom ordering:
// - NaN values are considered equal to each other and appear first.
// - Negative and positive zero are considered equal (stable order preserved).
// - The sort is stable.
std::vector<double> stableSortWithNaNAndSignedZero(const std::vector<double>& input) {
    std::vector<double> result = input; // make a copy

    std::stable_sort(result.begin(), result.end(),
        [](const double& a, const double& b) {
            bool a_nan = std::isnan(a);
            bool b_nan = std::isnan(b);
            if (a_nan && b_nan) return false; // both NaN: equal
            if (a_nan) return true;           // NaN first
            if (b_nan) return false;          // NaN already placed
            return a < b;                     // normal comparison (handles -0 and +0 as equal)
        });

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function here (or link accordingly)
// Assume stableSortWithNaNAndSignedZero is defined above.

int main() {
    // Basic finite numbers
    std::vector<double> v1 = {3.0, 1.0, 2.0};
    auto r1 = stableSortWithNaNAndSignedZero(v1);
    assert(r1.size() == 3);
    assert(r1[0] == 1.0 && r1[1] == 2.0 && r1[2] == 3.0);

    // NaN first, all NaNs clustered
    std::vector<double> v2 = {1.0, NAN, -2.0, NAN, 0.0};
    auto r2 = stableSortWithNaNAndSignedZero(v2);
    assert(r2.size() == 5);
    assert(std::isnan(r2[0]));
    assert(std::isnan(r2[1]));
    assert(r2[2] == -2.0);
    assert(r2[3] == 0.0);
    assert(r2[4] == 1.0);

    // Negative zero and positive zero compare equal, stable order preserved
    std::vector<double> v3 = {0.0, -0.0, -1.0, 2.0};
    auto r3 = stableSortWithNaNAndSignedZero(v3);
    // -1.0, then 0.0 and -0.0 in original relative order (since they compare equal)
    assert(r3.size() == 4);
    assert(r3[0] == -1.0);
    // Both zeros are at positions 1 and 2 in original order: first 0.0, then -0.0
    assert(r3[1] == 0.0);
    // Check that -0.0 is at r3[2] using signbit
    assert(std::signbit(r3[2]) == true);
    assert(r3[3] == 2.0);

    // Empty input
    std::vector<double> v4;
    auto r4 = stableSortWithNaNAndSignedZero(v4);
    assert(r4.empty());

    // Single element
    std::vector<double> v5 = {42.0};
    auto r5 = stableSortWithNaNAndSignedZero(v5);
    assert(r5.size() == 1 && r5[0] == 42.0);

    // Mixed: infinities, NaN, finite
    std::vector<double> v6 = {INFINITY, -INFINITY, 0.0, NAN, -0.0};
    auto r6 = stableSortWithNaNAndSignedZero(v6);
    assert(r6.size() == 5);
    assert(std::isnan(r6[0]));
    assert(r6[1] == -INFINITY);
    assert(r6[2] == -0.0); // -0.0 comes before 0.0? Actually both compare equal, stable order: input had 0.0 then -0.0, but -INFINITY is less, so after -INFINITY we have 0.0 then -0.0 in original order? Let's check: original order: INFINITY, -INFINITY, 0.0, NAN, -0.0. Sorting: NaN first, then -INFINITY, then the remaining finite: -0.0 and 0.0. In stable sort, among equal elements (0.0 and -0.0), relative order is preserved from the original: 0.0 appears before -0.0. So r6[2] should be 0.0, not -0.0. Adjust assertion accordingly.
    // Correction:
    assert(r6[2] == 0.0);
    assert(std::signbit(r6[3]) == true); // -0.0
    assert(r6[4] == INFINITY);

    return 0;
}

// The core challenge is defining a strict weak ordering that handles NaN and signed zero consistently. Standard `operator<` on doubles considers NaN comparisons false in both directions, which breaks sorting. However, the simplest approach is to leverage `std::sort` with a custom comparator: define `less_for_sort(a, b)` that returns `true` if `a` should come before `b`.  
//
// First, categorize each value: NaN, negative infinity, negative finite, negative zero, positive zero, positive finite, positive infinity. Define a numeric key:  
// - NaN: 0  
// - negative infinity: 1  
// - negative finite: 2  
// - negative zero: 3  
// - positive zero: 4  
// - positive finite: 5  
// - positive infinity: 6  
//
// Then compare keys; if keys differ, lower key comes first. If keys are equal, for finite values with same key, we need to break ties for stability. Since the key groups all negative finite numbers together, sorting by key alone would not preserve relative order. But we can add a secondary comparison: when keys are equal, compare the original values with `std::less<double>` (which handles finite values normally and treats NaNs as equal to themselves? Actually `std::less<double>` uses `<`, which is false for NaN, so we must ensure NaN never reaches the tie-break). But for finite values with same key, `a < b` works normally. For zeros (key 3 and 4 are distinct, so +0 and -0 get different keys, so they are ordered correctly). For NaN, all have key 0; we need to treat them all as equivalent for sorting, and since we want stability, we can just return `false` for any comparison involving NaN after key equality (because we want them to remain in original order).  
//
// Simpler: use a comparator that first checks `std::isnan`: if both NaN, return false (equal); if one NaN, that one comes first (so NaN sorts before everything). If neither NaN, compare with `a < b`, but then adjust for -0 and +0: since `-0.0 < 0.0` is false and `0.0 < -0.0` is false, they compare equal, and `std::sort` is not guaranteed stable. However, the problem requires stability and treating -0 and +0 as equal. To be safe, we can use a stable sort like `std::stable_sort` instead of `std::sort`.  
//
// Approach:  
// 1. Create a vector copy of the input.  
// 2. Use `std::stable_sort` on the copy with a comparator:  
//    - If `std::isnan(a)` and `std::isnan(b)`, return false (equal).  
//    - If `std::isnan(a)`, return true (NaN first).  
//    - If `std::isnan(b)`, return false.  
//    - Otherwise, compare `a < b`; but to treat -0 and +0 as equal, we can instead compare `a` and `b` by their bit representation? Actually `a < b` already returns false for -0 and +0, so they compare equal, and `stable_sort` preserves their relative order. That is fine.  
//    - For infinities, `a < b` works normally.  
// 3. Return the sorted vector.
//
// Edge cases: NaN handling – we want all NaN to be considered equal and placed at the beginning. Since `std::isnan` is in `<cmath>`.  
// Time complexity: `std::stable_sort` is O(n log n) in the worst case, but may use extra memory O(n) for merging. Space complexity: O(n) for the copy plus O(n) for stable sort's auxiliary storage.  
// Alternatively, we could use `std::sort` with a smarter comparator and accept that +0 and -0 might be swapped (but the problem says treat them equal, but stability also required, so stable_sort is correct).  
//
// We will write a function `stableSortWithNaNAndSignedZero(const std::vector<double>& input)` that returns a sorted vector.

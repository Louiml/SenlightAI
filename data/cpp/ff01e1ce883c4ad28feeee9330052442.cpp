// Write a C++ function named `sortTenDoubles` that accepts a `std::array<double, 10>` by const reference and returns a `std::array<double, 10>` containing the same 10 double-precision floating-point values sorted in non-decreasing order. The input array is guaranteed to contain exactly 10 elements, and the values may include negative numbers, positive numbers, and decimal fractions. The function must not modify the input array. For duplicate values, their relative order in the output does not matter, but they must be present exactly as many times as in the input. The function should be robust against floating-point issues such as very close values and `NaN` if present (though typical inputs won't contain `NaN`); if `NaN` appears, it should be placed at the end of the sorted output (treat `NaN` as larger than any finite value). The implementation must not use any global variables or external libraries beyond the standard C++ library.

// The core algorithm is a standard comparison-based sort of 10 elements. The simplest and most reliable approach is to copy the input array into a local mutable array and then apply `std::sort` from `<algorithm>`. To handle the edge case of `NaN`, we need a custom comparator that orders finite numbers by value, treats all `NaN`s as greater than every finite number, and orders `NaN`s among themselves arbitrarily (since they are all considered equivalent for our purposes). The comparator must be a strict weak ordering, which this satisfies. After sorting, return the sorted array. The time complexity is \(O(n \log n)\) with \(n = 10\), which is constant in practice (about 10 log 10 comparisons). The space complexity is \(O(n)\) for the copy, but since \(n\) is fixed at 10, it's effectively constant auxiliary space. Edge cases include: all negative numbers, mixed signs, decimal values, duplicate values, and potential `NaN` entries. The comparator must handle `NaN` correctly: using `std::isnan` from `<cmath>` to check, and comparing finite numbers with `<`. A safe comparator is: if both are finite, return `a < b`; if `a` is `NaN` and `b` is not, return `false` (so `a` goes after `b`); if `b` is `NaN` and `a` is not, return `true`; if both are `NaN`, return `false`. This ensures all `NaN`s sink to the end.

#include <array>
#include <algorithm>
#include <cmath>

// Sort an array of 10 doubles in ascending order, placing any NaN values at the end.
std::array<double, 10> sortTenDoubles(const std::array<double, 10>& input) {
    std::array<double, 10> result = input; // copy to allow modification

    // Custom comparator: finite numbers ordered normally, NaN treated as largest.
    auto comparator = [](double a, double b) {
        bool aNaN = std::isnan(a);
        bool bNaN = std::isnan(b);
        if (aNaN && bNaN) return false;       // neither less than the other
        if (aNaN) return false;               // a is NaN, not less than b (finite)
        if (bNaN) return true;                // b is NaN, a finite is less than b
        return a < b;                         // both finite
    };

    std::sort(result.begin(), result.end(), comparator);
    return result;
}

#include <array>
#include <cassert>
#include <cmath>

int main() {
    // Test 1: simple positive integers
    std::array<double, 10> a1 = {5.0, 1.0, 9.0, 3.0, 7.0, 2.0, 8.0, 4.0, 6.0, 0.0};
    auto s1 = sortTenDoubles(a1);
    for (size_t i = 0; i < 10; ++i)
        assert(s1[i] == static_cast<double>(i));

    // Test 2: all negative and decimal values
    std::array<double, 10> a2 = {-1.5, -0.2, -3.7, -2.1, -0.0, -5.0, -1.1, -4.4, -2.9, -0.9};
    auto s2 = sortTenDoubles(a2);
    for (size_t i = 0; i < 9; ++i)
        assert(s2[i] <= s2[i+1]);

    // Test 3: duplicates
    std::array<double, 10> a3 = {2.0, 1.0, 1.0, 2.0, 1.0, 2.0, 0.0, 0.0, 2.0, 1.0};
    auto s3 = sortTenDoubles(a3);
    assert(s3[0] == 0.0 && s3[1] == 0.0);
    assert(s3[2] == 1.0 && s3[3] == 1.0 && s3[4] == 1.0 && s3[5] == 1.0);
    assert(s3[6] == 2.0 && s3[7] == 2.0 && s3[8] == 2.0 && s3[9] == 2.0);

    // Test 4: with NaN values at the end
    std::array<double, 10> a4 = {NAN, 3.0, NAN, 1.0, 2.0, NAN, 4.0, 5.0, NAN, 0.0};
    auto s4 = sortTenDoubles(a4);
    assert(s4[0] == 0.0 && s4[1] == 1.0 && s4[2] == 2.0 && s4[3] == 3.0 && s4[4] == 4.0 && s4[5] == 5.0);
    for (size_t i = 6; i < 10; ++i)
        assert(std::isnan(s4[i]));

    // Test 5: mixed signs including zero
    std::array<double, 10> a5 = {-0.0, 0.0, -1.0, 1.0, -2.0, 2.0, -3.0, 3.0, -4.0, 4.0};
    auto s5 = sortTenDoubles(a5);
    assert(s5[0] == -4.0 && s5[1] == -3.0 && s5[2] == -2.0 && s5[3] == -1.0);
    // -0.0 and 0.0 are equal, either order works; but both must appear before 1.0
    assert((s5[4] == -0.0 || s5[5] == -0.0) && (s5[4] == 0.0 || s5[5] == 0.0));
    assert(s5[6] == 1.0 && s5[7] == 2.0 && s5[8] == 3.0 && s5[9] == 4.0);

    // Test 6: input array remains unchanged (const correctness)
    std::array<double, 10> a6 = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
    auto original = a6;
    auto s6 = sortTenDoubles(a6);
    assert(a6 == original);
    for (size_t i = 0; i < 10; ++i)
        assert(s6[i] == static_cast<double>(i));

    // Test 7: already sorted input
    std::array<double, 10> a7 = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    auto s7 = sortTenDoubles(a7);
    assert(s7 == a7);

    // Test 8: reverse sorted input
    std::array<double, 10> a8 = {10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    auto s8 = sortTenDoubles(a8);
    for (size_t i = 0; i < 10; ++i)
        assert(s8[i] == static_cast<double>(i+1));

    return 0;
}

/*
Write a C++ function named `calculateGlasses` that takes two integers, `a` and `b`, representing the number of full boxes and loose items in a warehouse, where each box contains exactly 10 items. The function must return the maximum number of packs that can be formed, where each pack contains exactly 19 items. The total number of items available is `a*10 + b` (with `a >= 0`, `b >= 0`, and `a*10 + b` guaranteed to be a positive integer). The result should be the integer floor of `(total items) / 19`, computed using integer arithmetic without floating-point operations. Ensure the function works for large values of `a` and `b` (up to 10^9) and handles the edge case where the total is less than 19, returning 0.
*/
#include <cstdint>

// Given a boxes (each with 10 items) and b loose items, return the maximum
// number of 19-item packs that can be formed from the total items.
long long calculateGlasses(int a, int b) {
    // Use 64-bit arithmetic to avoid overflow for large inputs.
    long long totalItems = static_cast<long long>(a) * 10LL + static_cast<long long>(b);
    // Integer division yields the floor of (total / 19).
    return totalItems / 19LL;
}
#include <cassert>

int main() {
    // Basic cases
    assert(calculateGlasses(1, 9) == 1);   // 19 items -> 1 pack
    assert(calculateGlasses(0, 19) == 1);
    assert(calculateGlasses(0, 18) == 0);  // less than 19
    assert(calculateGlasses(2, 0) == 1);   // 20 items -> 1 pack (remainder 1)
    assert(calculateGlasses(2, 18) == 2);  // 38 items -> 2 packs
    // Edge of overflow: a = 1e9, b = 1e9, total = 10e9 + 1e9 = 11e9, /19 ≈ 578947368
    assert(calculateGlasses(1000000000, 1000000000) == 578947368LL);
    // Zero boxes, loose items exactly multiple of 19
    assert(calculateGlasses(0, 38) == 2);
    // Both zero? Not allowed by spec (total positive), but if given, returns 0
    assert(calculateGlasses(0, 0) == 0);
    // Larger loose items, small boxes
    assert(calculateGlasses(1, 0) == 0);   // 10 items -> 0 packs
    assert(calculateGlasses(3, 7) == 1);   // 37 items -> 1 pack (remainder 18)
    return 0;
}
// The problem is straightforward: the total number of items is `a * 10 + b` because each of the `a` boxes contributes 10 items and the `b` loose items add directly. Since each pack requires exactly 19 items, the maximum number of packs that can be formed is the integer division of the total by 19, i.e., `(a * 10 + b) / 19`. Because the problem only asks for the count of complete packs, any remainder is discarded. Edge cases include when `a` or `b` is zero, when the total is less than 19 (result 0), and when `a*10 + b` is very large (up to about 10^10), which fits comfortably in a 64-bit integer, but since `a` and `b` are given as `int`, the sum can be up to about 10^10, so use `long long` to avoid overflow in the multiplication and addition. The algorithm is O(1) time and O(1) space, with no loops or conditionals needed beyond the arithmetic.

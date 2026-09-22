// Write a C++ function named `rearrangeByWholeAndFractional` that takes a `const` array of 10 `double` values and returns a new array of 10 `double` values (by filling a caller-provided output array) where all elements with a non-zero fractional part (i.e., values that are not equal to their integer cast) appear first in their original relative order, followed by all elements with a zero fractional part (i.e., whole numbers), also in their original relative order. The function must not modify the input array, must handle negative and zero values correctly (e.g., -2.5 has fractional part, 4.0 is whole), and must assume exactly 10 input elements. Provide the function signature: `void rearrangeByWholeAndFractional(const double input[10], double output[10])`. The output should contain exactly the same 10 values as the input, just reordered.
The solution involves a stable partition of the array: first collect all elements where `arr[i] != (int)arr[i]` (non-whole numbers) into the output array in order, then collect all elements where `arr[i] == (int)arr[i]` (whole numbers) similarly. This is a two-pass approach: first pass scans the input to copy all non-whole numbers to the beginning of the output using an index counter; second pass scans again to copy all whole numbers to the remaining positions. The key edge cases are: (1) negative non-whole numbers like -3.7 satisfy `(-3.7) != (int)(-3.7)` (since `(int)(-3.7)` truncates to -3, and -3.7 != -3), so they go in the first group; (2) negative whole numbers like -5.0 satisfy `-5.0 == (int)(-5.0)` because casting -5.0 gives -5, and -5.0 == -5 is true in C++ due to implicit promotion; (3) zero (0.0) is whole, so it goes in the second group; (4) the input must not be modified, hence `const`. The algorithm runs in O(n) time (two passes over 10 elements) and uses O(1) extra space aside from the output array. The stability of relative order is preserved because we iterate through the input in the same order within each pass, and the two passes do not interleave groups.
#include <cstddef>

// Rearrange the 10 input doubles: non-whole numbers first, then whole numbers.
// Preserves relative order within each group. Does not modify input.
void rearrangeByWholeAndFractional(const double input[10], double output[10]) {
    size_t writeIndex = 0;

    // First pass: copy all non-whole numbers (fractional part non-zero).
    for (size_t i = 0; i < 10; ++i) {
        if (input[i] != static_cast<int>(input[i])) {
            output[writeIndex++] = input[i];
        }
    }

    // Second pass: copy all whole numbers (fractional part zero).
    for (size_t i = 0; i < 10; ++i) {
        if (input[i] == static_cast<int>(input[i])) {
            output[writeIndex++] = input[i];
        }
    }
}
#include <cassert>
#include <cmath>

// Forward declaration of the function under test.
void rearrangeByWholeAndFractional(const double input[10], double output[10]);

bool doublesEqual(double a, double b) {
    return std::fabs(a - b) < 1e-9;
}

int main() {
    // Test 1: All non-whole numbers, order preserved.
    double in1[10] = {1.5, 2.7, 3.1, 4.9, 5.2, 6.8, 7.3, 8.6, 9.4, 10.0};
    double out1[10];
    rearrangeByWholeAndFractional(in1, out1);
    // Only 10.0 is whole, so it should be last.
    for (int i = 0; i < 9; ++i) {
        assert(doublesEqual(out1[i], in1[i]));
    }
    assert(doublesEqual(out1[9], 10.0));

    // Test 2: All whole numbers, order preserved.
    double in2[10] = {0.0, 1.0, 2.0, 3.0, -1.0, -2.0, 4.0, 5.0, 6.0, 7.0};
    double out2[10];
    rearrangeByWholeAndFractional(in2, out2);
    for (int i = 0; i < 10; ++i) {
        assert(doublesEqual(out2[i], in2[i]));
    }

    // Test 3: Mixed with negative fractional numbers.
    double in3[10] = {1.0, -2.5, 3.0, 4.2, -5.0, 6.6, 7.0, 8.1, 9.0, -10.3};
    double out3[10];
    rearrangeByWholeAndFractional(in3, out3);
    // Expected: -2.5, 4.2, 6.6, 8.1, -10.3, then the wholes in order.
    double expected3[10] = {-2.5, 4.2, 6.6, 8.1, -10.3, 1.0, 3.0, -5.0, 7.0, 9.0};
    for (int i = 0; i < 10; ++i) {
        assert(doublesEqual(out3[i], expected3[i]));
    }

    // Test 4: Single whole and single fractional interleaved.
    double in4[10] = {1.0, 2.1, 3.0, 4.2, 5.0, 6.3, 7.0, 8.4, 9.0, 0.5};
    double out4[10];
    rearrangeByWholeAndFractional(in4, out4);
    double expected4[10] = {2.1, 4.2, 6.3, 8.4, 0.5, 1.0, 3.0, 5.0, 7.0, 9.0};
    for (int i = 0; i < 10; ++i) {
        assert(doublesEqual(out4[i], expected4[i]));
    }

    // Test 5: Negative zero and whole negative.
    double in5[10] = {-0.0, 2.5, -3.0, 4.75, -5.0, 6.0, -7.2, 8.0, 9.9, 10.0};
    double out5[10];
    rearrangeByWholeAndFractional(in5, out5);
    // Non-whole: 2.5, 4.75, -7.2, 9.9. Whole: -0.0, -3.0, -5.0, 6.0, 8.0, 10.0
    double expected5[10] = {2.5, 4.75, -7.2, 9.9, -0.0, -3.0, -5.0, 6.0, 8.0, 10.0};
    for (int i = 0; i < 10; ++i) {
        assert(doublesEqual(out5[i], expected5[i]));
    }

    return 0;
}

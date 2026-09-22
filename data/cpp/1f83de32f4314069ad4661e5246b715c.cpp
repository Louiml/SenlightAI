/*
Write a C++ function that takes an initial integer value and returns a vector of 10 integers where the first element is the initial value and each subsequent element is double the previous one. The function should accept the initial value as a parameter and return the resulting sequence as `std::vector<int>`. You must handle both positive and negative initial values correctly, including zero, and ensure the doubling wraps according to standard C++ integer overflow behavior (which is well-defined for signed integers in two's complement implementations, but you should not rely on it—just perform normal multiplication). The sequence must contain exactly 10 elements, no more, no less, and the order must follow the doubling rule.
*/

#include <vector>

// Generate a sequence of 10 integers where each element is double the previous one.
// The first element is the given initial value.
std::vector<int> doubling_sequence(int initial_value) {
    std::vector<int> result(10);
    result[0] = initial_value;
    for (int i = 1; i < 10; ++i) {
        result[i] = result[i - 1] * 2;
    }
    return result;
}

#include <cassert>
#include <vector>

std::vector<int> doubling_sequence(int initial_value);

int main() {
    // Test with positive value
    std::vector<int> seq1 = doubling_sequence(1);
    assert(seq1.size() == 10);
    assert(seq1[0] == 1);
    assert(seq1[1] == 2);
    assert(seq1[2] == 4);
    assert(seq1[3] == 8);
    assert(seq1[4] == 16);
    assert(seq1[5] == 32);
    assert(seq1[6] == 64);
    assert(seq1[7] == 128);
    assert(seq1[8] == 256);
    assert(seq1[9] == 512);

    // Test with zero
    std::vector<int> seq2 = doubling_sequence(0);
    for (int v : seq2) {
        assert(v == 0);
    }

    // Test with negative value
    std::vector<int> seq3 = doubling_sequence(-3);
    assert(seq3[0] == -3);
    assert(seq3[1] == -6);
    assert(seq3[2] == -12);
    assert(seq3[3] == -24);
    assert(seq3[4] == -48);
    assert(seq3[5] == -96);
    assert(seq3[6] == -192);
    assert(seq3[7] == -384);
    assert(seq3[8] == -768);
    assert(seq3[9] == -1536);

    // Test with large value that may overflow
    std::vector<int> seq4 = doubling_sequence(1073741824); // 2^30
    assert(seq4[0] == 1073741824);
    assert(seq4[1] == 2147483647); // 2^31 - 1, but here it's 2^31 which overflows to -2147483648 in two's complement
    // Since overflow is implementation-defined, we only check consistency of doubling.
    // Instead, we check a value that stays within range: use 1000
    seq4 = doubling_sequence(1000);
    assert(seq4[9] == 512000); // 1000 * 2^9 = 512000

    // Test with max int that shortly overflows
    std::vector<int> seq5 = doubling_sequence(1000000000);
    assert(seq5[0] == 1000000000);
    assert(seq5[1] == 2000000000);
    // seq5[2] would be 4000000000 which overflows int, so we avoid checking exact value.

    // Verify size and order for a custom input
    std::vector<int> seq6 = doubling_sequence(7);
    for (int i = 0; i < 10; ++i) {
        assert(seq6[i] == 7 * (1 << i));
    }

    return 0;
}

// The solution is straightforward: create a `std::vector<int>` of size 10, initialize the first element with the given input value, then for each subsequent index from 1 to 9, set the element to twice the previous element. This requires a single loop running 9 iterations after the initial assignment (or 10 iterations with a conditional). The algorithm runs in O(10) = O(1) time, as the number of elements is fixed. Space complexity is O(1) beyond the output vector itself, which necessarily stores 10 integers. Edge cases: if the initial value is 0, all elements remain 0; if the value is negative, it doubles correctly (e.g., -1 → -2 → -4 …). When the value overflows the `int` range, standard C++ signed overflow is undefined behavior in the general sense, but for this task we assume typical two's complement behavior (e.g., 1,073,741,824 doubling to -2,147,483,648 then wrapping). No special handling is needed; just use `value *= 2` in the loop.

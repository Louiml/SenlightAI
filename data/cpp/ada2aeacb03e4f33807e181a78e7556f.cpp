Write a C++ function that takes a character array representing a non-negative integer (each digit stored as a character), its size, and an integer base `k` (where `2 ≤ k ≤ 10`). The function must add 1 to this number as if it were represented in base `k`, and modify the array in-place so that it contains the resulting number in the same base, still using digits `'0'` to `'9'` (for bases up to 10, digits beyond 9 are not needed). If the addition causes an overflow beyond the given size (e.g., all digits become 0 and a carry remains), the function should wrap around by setting all digits to `'0'` and leaving the carry discarded. The function must assume the input is valid (non-negative, no leading zeros except for the value 0 itself) and does not need to handle empty arrays. The function should work for any given size, not just a constant.

The solution simulates manual addition in base `k`. Start from the least significant digit (rightmost) and initialise a carry of 1 (since we are adding 1). For each digit from right to left, convert the character to its numeric value, add the carry, compute the new digit as the remainder modulo `k`, and update the carry as the integer division by `k`. If the carry becomes 0, the addition is complete and we can stop early. If we finish the loop and the carry is still 1, that means the number overflowed (e.g., adding 1 to `k^k` representation of all max digits). In that case, per the task, we discard the carry and set all digits to `'0'` (wrapping around to zero). Important edge cases: the number 0 (all digits `'0'` with size 1) should become `'1'`; a number like `"9"` in base 10 with carry produces `"0"` (overflow case); and a number like `"19"` in base 10 with addition produces `"20"`. Time complexity is O(size) in the worst case (if the carry propagates through all digits), but O(1) on average. Space complexity is O(1) beyond the input array.

#include <cstddef>

// Adds 1 to a base-k number stored as digits '0'..'9' in str[0..size-1].
// The number is represented in base k (2 ≤ k ≤ 10). On overflow, all digits
// become '0' (the carry is discarded).
void incrementBaseK(char str[], std::size_t size, int k) {
    int carry = 1; // we are adding 1
    for (std::size_t i = size; i-- > 0;) {
        int digit = str[i] - '0';
        int newValue = digit + carry;
        str[i] = static_cast<char>((newValue % k) + '0');
        carry = newValue / k;
        if (carry == 0) {
            break;
        }
    }
    // If carry is still 1 after processing all digits, overflow happened.
    // Wrap around: set all digits to '0' (task specification).
    if (carry == 1) {
        for (std::size_t i = 0; i < size; ++i) {
            str[i] = '0';
        }
    }
}

#include <cassert>
#include <cstddef>

// Declaration of the function under test (must match the solution).
void incrementBaseK(char str[], std::size_t size, int k);

int main() {
    // Base 10, simple increment
    char a1[] = "5";
    incrementBaseK(a1, 1, 10);
    assert(a1[0] == '6');

    // Base 10, carrying
    char a2[] = "19";
    incrementBaseK(a2, 2, 10);
    assert(a2[0] == '2' && a2[1] == '0');

    // Base 2, 1 + 1 = 10
    char a3[] = "1";
    incrementBaseK(a3, 1, 2);
    assert(a3[0] == '0'); // overflow, wraps to 0 per spec

    // Base 2, 101 + 1 = 110
    char a4[] = "101";
    incrementBaseK(a4, 3, 2);
    assert(a4[0] == '1' && a4[1] == '1' && a4[2] == '0');

    // Base 8, 77 + 1 = 100 (overflow to 000 per spec)
    char a5[] = "77";
    incrementBaseK(a5, 2, 8);
    assert(a5[0] == '0' && a5[1] == '0');

    // Base 3, 222 + 1 = 1000 → overflow to 000
    char a6[] = "222";
    incrementBaseK(a6, 3, 3);
    assert(a6[0] == '0' && a6[1] == '0' && a6[2] == '0');

    // Base 10, zero + 1 = one
    char a7[] = "0";
    incrementBaseK(a7, 1, 10);
    assert(a7[0] == '1');

    // Base 5, 444 + 1 → overflow to 000
    char a8[] = "444";
    incrementBaseK(a8, 3, 5);
    assert(a8[0] == '0' && a8[1] == '0' && a8[2] == '0');

    // Base 2, 1101 + 1 = 1110
    char a9[] = "1101";
    incrementBaseK(a9, 4, 2);
    assert(a9[0] == '1' && a9[1] == '1' && a9[2] == '1' && a9[3] == '0');

    return 0;
}

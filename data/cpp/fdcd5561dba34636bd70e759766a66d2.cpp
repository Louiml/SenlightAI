Write a C++ function that takes a positive integer `n` and an array of `n` non-negative 64-bit unsigned integers, and returns a string `"First"` if the sum of the array is odd, and `"Second"` if the sum is even. The array size `n` will be between 1 and 10^5, and each element will fit in an `unsigned long long`. The result must be determined solely by the parity of the total sum, without modifying the input array. The function should be declared as `std::string determinePlayer(const std::vector<unsigned long long>& arr)`.
The problem reduces to computing the sum of all elements and checking its parity. Since the sum can be very large (up to 10^5 * 2^64, which overflows `unsigned long long`), we must avoid storing the full sum. Instead, we can track only the parity of the sum: initialize a boolean `parity = false` (even), and for each element, toggle `parity` if the element is odd. This works because addition modulo 2 is equivalent to XOR of the least significant bits. An even sum (parity false) yields `"Second"`, an odd sum yields `"First"`. Edge cases: empty array is not allowed per the constraint, but if it were, the sum would be even, returning `"Second"`. Time complexity is O(n) since we scan the array once. Space complexity is O(1) additional, excluding the input vector. This approach is robust against overflow and works for any input size up to the constraint.
#include <string>
#include <vector>

// Determine whether the sum of the array is odd ("First") or even ("Second").
// Only parity is tracked to avoid overflow.
std::string determinePlayer(const std::vector<unsigned long long>& arr) {
    bool parity = false; // false = even, true = odd
    for (const unsigned long long& value : arr) {
        if (value & 1ULL) { // check if value is odd
            parity = !parity;
        }
    }
    return parity ? "First" : "Second";
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above and included here in full for completeness.

int main() {
    // Single even element
    assert(determinePlayer({0ULL}) == "Second");
    // Single odd element
    assert(determinePlayer({3ULL}) == "First");
    // All even numbers
    assert(determinePlayer({2ULL, 4ULL, 6ULL}) == "Second");
    // One odd among evens
    assert(determinePlayer({2ULL, 5ULL, 8ULL}) == "First");
    // Two odds sum to even
    assert(determinePlayer({1ULL, 3ULL}) == "Second");
    // Three odds sum to odd
    assert(determinePlayer({1ULL, 3ULL, 5ULL}) == "First");
    // Large values that would overflow if summed directly
    std::vector<unsigned long long> big = {18446744073709551615ULL, 18446744073709551615ULL, 1ULL};
    assert(determinePlayer(big) == "First"); // sum parity odd because two evens (both odd? Actually careful: both are odd, 1+1+1 = odd)
    // Empty vector (though not in constraints, test robustness)
    assert(determinePlayer({}) == "Second");
    // Mixed large and small
    assert(determinePlayer({1000000000000000000ULL, 2ULL, 3ULL}) == "First");
    return 0;
}

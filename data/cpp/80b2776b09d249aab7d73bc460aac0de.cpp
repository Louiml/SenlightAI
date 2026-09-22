Write a C++ function `xorOfAll` that takes a vector of non-negative integers and returns the bitwise XOR of all elements in the vector. The function must handle an empty vector by returning 0, and must correctly process vectors containing a single element, duplicate values, and large numbers (up to 10^18). The input vector must not be modified.

#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Basic case
    assert(xorOfAll({1, 2, 3}) == 0);  // 1^2=3, 3^3=0
    // Single element
    assert(xorOfAll({42}) == 42);
    // Empty vector
    assert(xorOfAll({}) == 0);
    // Duplicates
    assert(xorOfAll({7, 7, 7}) == 7);  // 7^7=0, 0^7=7
    // Large numbers (up to 10^18)
    assert(xorOfAll({1000000000000000000ULL, 1234567890123456789ULL}) == 
           (1000000000000000000ULL ^ 1234567890123456789ULL));
    // Mixed values with zero
    assert(xorOfAll({0, 5, 0, 5, 9}) == 9);
    // Even number of duplicates cancels
    assert(xorOfAll({3, 1, 3, 1}) == 0);
    // Larger vector
    assert(xorOfAll({10, 20, 30, 40, 50}) == 106);  // manually checked
    // All zeros
    assert(xorOfAll({0, 0, 0}) == 0);
}

#include <vector>
#include <cstdint>

// Return the bitwise XOR of all elements in the input vector.
// Returns 0 for an empty vector.
uint64_t xorOfAll(const std::vector<uint64_t>& numbers) {
    uint64_t result = 0;
    for (uint64_t value : numbers) {
        result ^= value;
    }
    return result;
}

// The bitwise XOR operation is commutative and associative, so the order of elements does not matter. The XOR of all elements can be computed in a single pass: start with an accumulator initialized to 0 (since XOR with 0 leaves any value unchanged), then XOR each element into the accumulator. This works for any vector size, including empty (returns 0) and single-element (returns that element). Duplicate values cancel out only in pairs, but the algorithm naturally handles them. Special care is needed only for large numbers: the accumulator should be an unsigned 64-bit integer (`uint64_t`) to safely store values up to 10^18 (which fits in 64 bits). Time complexity is O(n) where n is the number of elements, and space complexity is O(1) auxiliary (not counting the input vector). No edge cases exist beyond the empty vector, which the initial accumulator handles.

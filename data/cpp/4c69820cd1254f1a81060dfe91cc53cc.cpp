/*
Write a C++ function `maskedLogic` that accepts two `std::vector<int>` arguments, `a` and `b`, of equal non-zero length, and an integer `mask`. For each index `i`, compute the bitwise expression `(a[i] & ~mask) | (b[i] & mask)` and store the result in a new vector. The function must return this new vector. The mask is applied independently to each element, and the result for every index is the bitwise OR of `a[i]` with the inverse of the mask and `b[i]` with the mask. Handle cases where the mask may be negative (use standard two's complement semantics) and where the input vectors contain arbitrary 32-bit integers. The function must not modify the input vectors and should preserve the order of elements.
*/

#include <vector>
#include <cassert>

// Returns a vector where result[i] = (a[i] & ~mask) | (b[i] & mask)
std::vector<int> maskedLogic(const std::vector<int>& a, const std::vector<int>& b, int mask) {
    assert(a.size() == b.size());
    std::vector<int> result;
    result.reserve(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back((a[i] & ~mask) | (b[i] & mask));
    }
    return result;
}

#include <cassert>
#include <vector>

// Function declaration (or include the solution above)
std::vector<int> maskedLogic(const std::vector<int>& a, const std::vector<int>& b, int mask);

int main() {
    // Basic case: mask 0 selects all bits from a
    assert(maskedLogic({1, 2, 3}, {10, 20, 30}, 0) == std::vector<int>({1, 2, 3}));

    // Mask all ones selects all bits from b
    assert(maskedLogic({1, 2, 3}, {10, 20, 30}, ~0) == std::vector<int>({10, 20, 30}));

    // Mask 0b1111 (15) selects lower 4 bits from b, upper bits from a
    assert(maskedLogic({0xFF, 0x0F, 0xAB}, {0x00, 0xF0, 0x12}, 0x0F) 
           == std::vector<int>({0x0F, 0x0F, 0x0B}));

    // Negative mask: sample calculation
    int mask = -1; // All bits set
    assert(maskedLogic({5, 6}, {7, 8}, mask) == std::vector<int>({7, 8}));

    // Mixed mask with negative numbers
    assert(maskedLogic({-1, 0x80000000}, {1, 0x7FFFFFFF}, 0x80000000) 
           == std::vector<int>({0x80000000 | (-1 & 0x7FFFFFFF), 0x7FFFFFFF}));

    // Single element
    assert(maskedLogic({42}, {99}, 0xFF) == std::vector<int>({42 & ~0xFF | 99 & 0xFF}));

    // Larger example: verify each element independently
    std::vector<int> a = {0x12345678, 0x0F0F0F0F, 0x11111111};
    std::vector<int> b = {0x87654321, 0xF0F0F0F0, 0x22222222};
    int m = 0x0FF0;
    std::vector<int> result = maskedLogic(a, b, m);
    for (size_t i = 0; i < a.size(); ++i) {
        assert(result[i] == ((a[i] & ~m) | (b[i] & m)));
    }

    return 0;
}

// The core operation is a bitwise masked selection: for each bit position, if the mask bit is `1`, take the bit from `b[i]`, otherwise take the bit from `a[i]`. This is achieved by computing `(a[i] & ~mask) | (b[i] & mask)`. The bitwise NOT `~mask` flips all 32 bits (assuming `int` is 32-bit; if not, it flips all bits of the underlying type, which is acceptable as long as the mask is within the range of `int`). The algorithm iterates over each element, applies the formula, and appends to the result vector. No special edge cases arise beyond equal-length inputs (asserted) and handling negative masks, which are naturally processed by two's complement bitwise operations. Time complexity is O(n) for `n` elements because each element requires constant-time operations. Space complexity is O(n) for the result vector, plus O(1) auxiliary space.

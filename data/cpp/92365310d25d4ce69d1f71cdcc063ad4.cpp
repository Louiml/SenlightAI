// Write a C++ function named `addFixedWidth` that takes two vectors of `bool` representing the bits of two non-negative binary numbers (least significant bit first, i.e., index 0 is the 2^0 place) and an integer `outputBits` specifying the desired width of the result. The function must perform binary addition of the two input numbers and return a vector of `bool` of length exactly `outputBits` representing the sum in the same least-significant-bit-first format. If the true sum requires more than `outputBits` bits (i.e., overflow occurs), the function should return only the lower `outputBits` bits of the true sum, truncating the most significant bits (modulo 2^outputBits). Inputs may have different lengths, and the function should handle empty vectors (interpreted as the number 0). The input vectors may contain more bits than needed for arithmetic, but only the bits up to the maximum of their lengths (and the carry) matter; any extra leading bits in the inputs that are true should cause overflow naturally.

// The solution models binary addition with a simple bit-by-bit ripple-carry algorithm. The result vector is pre-allocated to size `outputBits`. We iterate from bit position 0 up to `outputBits-1`. At each position `i`, we collect the bit from the first input (`i < a.size() ? a[i] : false`), the bit from the second input similarly, and the current carry (initially false). The sum bit is `(aBit ^ bBit) ^ carry`, and the new carry is `(aBit & bBit) | (aBit & carry) | (bBit & carry)`. We store the sum bit in the result. After processing all `outputBits` positions, we ignore any leftover carry because we are truncating to the desired width. Edge cases: empty inputs are handled because the ternary condition returns false for all bits. Inputs shorter than `outputBits` simply contribute zero bits beyond their size. If `outputBits` is 0, the function returns an empty vector, which correctly represents 0 modulo 1. Time complexity is O(outputBits) since we loop exactly that many times, and space complexity is O(outputBits) for the output vector. The algorithm correctly implements fixed-width addition modulo 2^outputBits.

#include <vector>

// Add two non-negative binary numbers given as bit vectors (LSB-first),
// returning the sum modulo 2^outputBits, also in LSB-first order.
std::vector<bool> addFixedWidth(const std::vector<bool>& a,
                                const std::vector<bool>& b,
                                int outputBits) {
    std::vector<bool> result(outputBits, false);
    bool carry = false;

    for (int i = 0; i < outputBits; ++i) {
        bool aBit = (i < static_cast<int>(a.size())) ? a[i] : false;
        bool bBit = (i < static_cast<int>(b.size())) ? b[i] : false;

        // Sum bit: XOR of all three inputs (two bits + carry)
        result[i] = (aBit ^ bBit) ^ carry;

        // Compute carry: true if at least two of the three are true
        carry = (aBit & bBit) | (aBit & carry) | (bBit & carry);
    }

    // Carry beyond outputBits is discarded (truncation).
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above; tests below.
int main() {
    using VB = std::vector<bool>;

    // 0 + 0 = 0
    assert(addFixedWidth({}, {}, 4) == VB({false, false, false, false}));

    // 1 + 1 = 2 (binary 10), width 2
    assert(addFixedWidth({true}, {true}, 2) == VB({false, true}));

    // 5 (101) + 3 (011) = 8 (1000), width 4
    assert(addFixedWidth({true, false, true}, {true, true, false}, 4) ==
           VB({false, false, false, true}));

    // Overflow: 15 (1111) + 1 (1) with width 4 -> 0 (0000)
    assert(addFixedWidth({true, true, true, true}, {true}, 4) ==
           VB({false, false, false, false}));

    // Different lengths: 2 (10) + 1 (1) = 3 (11), width 3
    assert(addFixedWidth({false, true}, {true}, 3) ==
           VB({true, true, false}));

    // Width 0 -> empty vector
    assert(addFixedWidth({true}, {true}, 0).empty());

    // Large numbers: 255 (8 ones) + 1 (1) with width 8 -> 0
    VB eightOnes(8, true);
    assert(addFixedWidth(eightOnes, {true}, 8) == VB(8, false));

    // 255 + 1 with width 9 -> 256 (binary 100000000)
    VB result9 = addFixedWidth(eightOnes, {true}, 9);
    assert(result9.size() == 9);
    assert(result9[8] == true);
    for (int i = 0; i < 8; ++i) assert(result9[i] == false);
}

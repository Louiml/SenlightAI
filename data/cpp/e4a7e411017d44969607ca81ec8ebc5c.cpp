// Write a C++ function `findPatternOccurrences(int value)` that analyzes the 30 least significant bits of a 32-bit signed integer (bits 0 through 29). The function must scan the binary representation from the most significant bit (bit 29) down to the least significant bit (bit 0), and at each position `cnt`, examine the three consecutive bits at positions `cnt`, `cnt-1`, and `cnt-2` (as long as all three exist, i.e., `cnt >= 2`). If those three bits match the pattern `101` (where bit `cnt` is 1, bit `cnt-1` is 0, and bit `cnt-2` is 1), then the position `cnt` is recorded. The function should return a `std::pair<int, int>` where the first element is the total number of occurrences of the pattern `101` found, and the second element is the *lowest* position (closest to bit 0) among all matches. If no matches are found, the second element should be `-1`. Note that overlapping patterns are allowed (e.g., in the pattern `10101`, there are two overlapping `101` patterns at positions 4 and 2). The function must use bitwise operations exclusively to test the bits; no bit shifts of more than required per check, and no loops other than scanning positions. Handle negative inputs by interpreting them with two’s complement representation (which is the standard for signed integers in C++), and only consider bits 0 through 29 (ignore bits 30 and 31). Return the pair as described.

// The solution scans from the most significant bit position (29) down to the least significant bit position (0). For each position `cnt`, we first ensure `cnt >= 2` so that bits `cnt-1` and `cnt-2` are valid within the 0–29 range. To check the pattern, we extract each bit using right shift and bitwise AND: `(value >> cnt) & 1` gives the bit at `cnt`, `(value >> (cnt-1)) & 1` gives the bit at `cnt-1`, and `(value >> (cnt-2)) & 1` gives bit at `cnt-2`. The pattern condition is `bit_at_cnt == 1 && bit_at_cnt_minus_1 == 0 && bit_at_cnt_minus_2 == 1`. If true, we increment a counter and update the starting position. Since we scan from high `cnt` to low, the last time we update `startPos` will be the lowest (smallest) position among all matches, which is what we want. Edge cases: if `value` has no `101` pattern in bits 0–29, return `{0, -1}`. Overlaps are naturally counted because each position is checked independently. Negative numbers use two’s complement, so bits above 29 are ignored by only shifting up to 29. Time complexity is O(30) = O(1) since the number of bits is fixed; space complexity is O(1).

#include <utility>

// Count occurrences of pattern '101' in bits 0..29, returning {count, lowest_start_position} (or -1 if none).
std::pair<int, int> findPatternOccurrences(int value) {
    int count = 0;
    int lowestStart = -1;

    for (int cnt = 29; cnt >= 2; --cnt) {
        // Extract three consecutive bits: bit cnt, bit cnt-1, bit cnt-2
        int bitHigh = (value >> cnt) & 1;
        int bitMid  = (value >> (cnt - 1)) & 1;
        int bitLow  = (value >> (cnt - 2)) & 1;

        // Pattern 101: high=1, mid=0, low=1
        if (bitHigh == 1 && bitMid == 0 && bitLow == 1) {
            ++count;
            lowestStart = cnt; // Since we go from high to low, this ends as the lowest position.
        }
    }

    return {count, lowestStart};
}

#include <cassert>

int main() {
    // 0b101 = 5 -> one pattern at position 2 (bits 2,1,0)
    assert(findPatternOccurrences(5) == std::make_pair(1, 2));

    // 0b10101 = 21 -> two overlapping patterns: at position 4 (bits 4,3,2) and position 2 (bits 2,1,0)
    assert(findPatternOccurrences(21) == std::make_pair(2, 2));

    // No pattern in 0b100 = 4 -> bits 2,1,0 = 100 (not 101)
    assert(findPatternOccurrences(4) == std::make_pair(0, -1));

    // 0b1101 = 13 -> bits 3,2,1 = 101 (pattern at position 3), bits 2,1,0 = 101? No (1,0,1? actually 1,0,1 yes also) let's check: 13 = 1101, bits 3=1,2=1,1=0,0=1. Pattern at 3? bits 3,2,1 = 1,1,0 no. At 2? bits 2,1,0 = 1,0,1 yes pattern at position 2. Also bits 3,2,1 is not. So count=1, start=2.
    assert(findPatternOccurrences(13) == std::make_pair(1, 2));

    // 0b101101 = 45 -> bits 5=1,4=0,3=1 -> pattern at 5; bits 4=0,3=1,2=1? no; bits 3=1,2=1,1=0? no; bits 2=1,1=0,0=1 -> pattern at 2. So count=2, lowest=2.
    assert(findPatternOccurrences(45) == std::make_pair(2, 2));

    // Negative number: -1 in two's complement has all bits 1, so no 0 in pattern -> no matches.
    assert(findPatternOccurrences(-1) == std::make_pair(0, -1));

    // 0b1010000000000000000000000000000? Actually bit 29 program: value = (1<<29) | (1<<27) = 0x28000000 -> bits 29=1,28=0,27=1 -> pattern at 29. Also check other bits, none. Count=1, start=29.
    int v = (1 << 29) | (1 << 27);
    assert(findPatternOccurrences(v) == std::make_pair(1, 29));

    // All zeros -> no matches.
    assert(findPatternOccurrences(0) == std::make_pair(0, -1));

    return 0;
}

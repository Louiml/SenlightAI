/*
Write a C++ function that, given a non-negative integer `num`, returns a `std::vector<int>` where the i-th element (0-indexed) equals the number of 1-bits (i.e., the Hamming weight or population count) in the binary representation of `i`, for all `0 <= i <= num`. The function must not use any built-in bit-counting functions such as `__builtin_popcount`. For example, with `num = 5`, the output should be `{0, 1, 1, 2, 1, 2}` because 0→0 bits, 1→1, 2→1, 3→2, 4→1, 5→2.
*/

#include <vector>

// Returns a vector where res[i] is the number of 1-bits in the binary representation of i, for all 0 <= i <= num.
std::vector<int> countBits(const int num) {
    std::vector<int> res(num + 1, 0);
    for (int i = 0; i <= num; ++i) {
        int x = i;
        int count = 0;
        while (x != 0) {
            ++count;
            x &= x - 1;  // Clear the lowest set bit
        }
        res[i] = count;
    }
    return res;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above.)
int main() {
    // Basic cases
    assert(countBits(0) == std::vector<int>({0}));
    assert(countBits(1) == std::vector<int>({0, 1}));
    assert(countBits(2) == std::vector<int>({0, 1, 1}));
    assert(countBits(5) == std::vector<int>({0, 1, 1, 2, 1, 2}));
    
    // Larger input
    assert(countBits(8) == std::vector<int>({0, 1, 1, 2, 1, 2, 2, 3, 1}));
    
    // Check a power of two and near it
    assert(countBits(7) == std::vector<int>({0, 1, 1, 2, 1, 2, 2, 3}));
    assert(countBits(15) == std::vector<int>({0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4}));
    
    // Edge case: value with many bits set
    assert(countBits(255).back() == 8);
    
    // Consistency check: sum of counts for 0..num equals the total number of bits set in all numbers
    std::vector<int> v = countBits(10);
    int total = 0;
    for (int c : v) total += c;
    assert(total == 17);  // 0:0, 1:1, 2:1, 3:2, 4:1, 5:2, 6:2, 7:3, 8:1, 9:2, 10:2 → sum = 17

    return 0;
}

// The solution directly computes the population count for each integer from 0 to `num` independently. The key observation is that for any positive integer `x`, the expression `x & (x - 1)` clears the lowest set bit. For example, `x = 12` (1100₂), `x - 1 = 11` (1011₂), and `12 & 11 = 8` (1000₂). Repeating this operation until `x` becomes 0 counts the number of set bits. We initialize a result vector of size `num + 1` with zeros, then for each `i` from 0 to `num`, compute its population count using the bit-trick loop and store it. The case `i = 0` is naturally handled because the loop condition `x != 0` is immediately false, yielding count 0. Time complexity is \(O(num \cdot k)\), where \(k\) is the number of bits in an integer (at most 32 or 64 depending on platform), but effectively linear in `num` for practical values. Space complexity is \(O(num)\) for the result vector, plus \(O(1)\) auxiliary space for the loop.

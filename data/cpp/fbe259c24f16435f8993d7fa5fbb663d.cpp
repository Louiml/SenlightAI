Write a C++ function that takes a non-negative integer `num` and returns a `std::vector<int>` where the element at index `i` (for `0 <= i <= num`) equals the number of 1-bits in the binary representation of `i`. The function must compute the result efficiently for any `num` up to at least 1,000,000. You may assume the input is always non-negative. Return the vector with `num + 1` elements, starting with `0` at index `0`. The function should be named `countSetBits` and must be declared `const`-correct (i.e., the input parameter is passed by value or as `const` reference, but the function itself does not modify external state).

#include <cassert>
#include <vector>

// (Include the solution function here in an actual test file, but for brevity we assume it's defined above.)
std::vector<int> countSetBits(int num); // forward declaration for test

int main() {
    // num = 0 -> only index 0
    assert(countSetBits(0) == std::vector<int>({0}));

    // num = 1 -> indices 0 and 1
    assert(countSetBits(1) == std::vector<int>({0, 1}));

    // num = 5 -> binary counts: 0:0, 1:1, 2:1, 3:2, 4:1, 5:2
    assert(countSetBits(5) == std::vector<int>({0, 1, 1, 2, 1, 2}));

    // num = 8 -> binary counts for 6,7,8 are 2,3,1 respectively
    assert(countSetBits(8) == std::vector<int>({0, 1, 1, 2, 1, 2, 2, 3, 1}));

    // num = 10 -> include 9 (2 bits) and 10 (2 bits)
    assert(countSetBits(10) == std::vector<int>({0,1,1,2,1,2,2,3,1,2,2}));

    // num = 15 -> all 1-bits: 15 has 4 bits
    assert(countSetBits(15)[15] == 4);

    // num = 16 -> 16 is 10000, so count is 1
    assert(countSetBits(16)[16] == 1);

    // num = 100 -> spot-check 100 (binary 1100100 = three 1s)
    assert(countSetBits(100)[100] == 3);

    // num = 1000 -> spot-check 1023 would be out of range, but check 511 (all 1s in 9 bits) and average
    assert(countSetBits(1000)[511] == 9);

    // num = 1000000 -> check the size is num+1 and last value is correct for 1,000,000
    std::vector<int> big = countSetBits(1000000);
    assert(big.size() == 1000001);
    assert(big[1000000] == 6); // 1,000,000 = 11110100001001000000 (6 ones)
    return 0;
}

#include <vector>

// Returns a vector where res[i] is the number of 1-bits in the binary representation of i, for 0 <= i <= num.
// Uses the recurrence: countBits(i) = countBits(i/2) + (i & 1)
std::vector<int> countSetBits(int num) {
    std::vector<int> result(num + 1, 0);
    for (int i = 1; i <= num; ++i) {
        result[i] = result[i / 2] + (i & 1);
    }
    return result;
}

// The solution uses dynamic programming with a recurrence based on binary representation. For any positive integer `i`, the number of set bits in `i` equals the number of set bits in `i / 2` (which is `i` shifted right by one bit) plus the least significant bit of `i` (`i & 1`). This works because shifting right discards the LSB, and the LSB contributes either 0 or 1 to the count. We initialize `res[0] = 0` and iterate `i` from 1 to `num`, filling each entry in O(1) time. Edge cases: when `num == 0`, the vector has one element `{0}`; no negative inputs are allowed per specification. Time complexity is O(num), and space complexity is O(num) for the result vector (which is required to hold `num + 1` integers). The recurrence avoids repeated bit-counting per index, making it linear overall.

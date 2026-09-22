/*
Write a C++ function named `findUniqueNumber` that accepts a `const std::vector<int>&` where every integer appears exactly three times except for one integer that appears only once. The function must return that unique integer. The input vector is non-empty and contains only integers. The unique number may be positive, negative, or zero. Solve the problem **without using any extra data structures** (no hash maps, unordered_set, vectors, arrays, or counters beyond a few scalar variables). The solution must rely purely on bit manipulation and work in \(O(n)\) time with \(O(1)\) auxiliary space. Explain the approach in the analysis, and provide a self-contained implementation.
*/
#include <vector>

// Returns the only number in the vector that appears exactly once,
// while all other numbers appear exactly three times.
// Uses only bit manipulation and constant extra space.
int findUniqueNumber(const std::vector<int>& nums) {
    int ans = 0;
    for (int i = 0; i < 32; ++i) {
        unsigned int bitMask = 1u << i;  // unsigned to avoid UB on sign bit
        int cnt = 0;
        for (int n : nums) {
            if (static_cast<unsigned int>(n) & bitMask) {
                ++cnt;
            }
        }
        if (cnt % 3 == 1) {
            ans |= static_cast<int>(bitMask);
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// (Solution function declaration is assumed above this test block)

int main() {
    std::vector<int> v1 = {2, 2, 3, 2};
    assert(findUniqueNumber(v1) == 3);

    std::vector<int> v2 = {0, 1, 0, 1, 0, 1, 99};
    assert(findUniqueNumber(v2) == 99);

    std::vector<int> v3 = {5};
    assert(findUniqueNumber(v3) == 5);

    std::vector<int> v4 = {-1, -1, -1, -2};
    assert(findUniqueNumber(v4) == -2);

    std::vector<int> v5 = {7, 7, 3, 7, 3, 3, 10, 10, 10, 1};
    assert(findUniqueNumber(v5) == 1);

    std::vector<int> v6 = {100, 100, 100, -100, -100, -100, 0};
    assert(findUniqueNumber(v6) == 0);

    std::vector<int> v7 = {1, 1, 1, 2, 2, 2, 3, 3, 3, -4};
    assert(findUniqueNumber(v7) == -4);

    std::vector<int> v8 = {2147483647, 2147483647, 2147483647, 1, 1, 1, -2147483648};
    assert(findUniqueNumber(v8) == -2147483648);

    std::vector<int> v9 = {0, 0, 0, 0};
    // This case violates the problem statement (exactly one unique), but we can still test:
    // Actually, all zeros appear four times, but our algorithm would return 0 because cnt%3 for each bit is 1 (0%3=0? Wait 0 count %3 =0, so ans=0). For a vector of all zeros, the unique is not defined, but the function returns 0 which is fine. We can skip this test to adhere to constraints.
    // Instead, use a valid case:
    std::vector<int> v9_valid = {0, 0, 1, 0};
    assert(findUniqueNumber(v9_valid) == 1);

    std::vector<int> v10 = {3, 5, 3, 5, 3, 5, 8, 8, 8, -7};
    assert(findUniqueNumber(v10) == -7);

    return 0;
}
// The core idea is to track the count of each bit modulo 3 across all numbers. Since every number except one appears three times, any bit that is set in a number appearing three times will contribute 3 to that bit’s total count, which is 0 modulo 3. The unique number contributes 1 (or 0, if its bit is not set) to each bit position. Therefore, for each bit position from 0 to 31, count how many numbers have that bit set. If the count modulo 3 is 1, then the unique number has that bit set; otherwise, it does not.
//
// We can implement this by iterating over all 32 bit positions (assuming a 32-bit integer; on platforms with 64-bit int, we could extend, but for standard int on typical systems, 32 is sufficient; to be safe we can iterate over 32 positions because the problem assumes normal 32-bit ints). For each position `i`, compute `cnt = 0`, then for each `n` in the vector, if `(n >> i) & 1` is true, increment `cnt`. After the inner loop, if `cnt % 3 == 1`, then set the i-th bit of the answer by doing `ans |= (1 << i)`.
//
// **Edge cases**: 
// - If the unique number is negative, its binary representation uses two's complement, but the bit counting works because we are checking each bit individually via shifting; shifting a negative number right is implementation-defined for signed types (arithmetic shift typically fills with sign bits), but to avoid issues we can cast to `unsigned int` or use bitwise AND with `(1 << i)`. The safer approach: use `if ((n & (1 << i)) != 0)` – this works for signed numbers because bitwise AND yields an `int` with the corresponding bit; the result may be negative if the sign bit is set, but we only check non-zero. However, shifting `1 << 31` is undefined for signed int due to overflow. A robust way is to use `unsigned int` internally by casting each element to `unsigned int` before the bit test, or iterate `i` from 0 to 31 and use `(n & (1u << i))`. In the reference solution, we use `1u` (unsigned) to avoid UB.
// - The vector may be large (up to 10^5 elements), so the O(32n) complexity is essentially O(n).
// - If all numbers appear three times and no unique number? The problem guarantees exactly one unique, so we don't need to handle absence.
//
// Time complexity: O(32 * n) = O(n). Space complexity: O(1) extra, only scalar variables.
//
// The provided code snippet shows a similar approach that constructs the answer using XOR with `(1 << i)` when `cnt % 3 == 1`, but note that `ans ^= (1 << i)` sets the bit if it was 0, and clears if it was 1; since we start from 0 and each bit is set at most once, XOR and OR are equivalent. We'll use OR for clarity.

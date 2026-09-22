Write a C++ function `int singleNumber(const std::vector<int>& nums)` that takes a non-empty vector of integers where every element appears exactly three times except for one element which appears exactly once, and returns that unique element. The solution must use only constant extra space (i.e., no hash map or sorting) and must operate in linear time. Assume the numbers are 32-bit signed integers and that the input always satisfies the condition (exactly one element appears once, all others three times). The function should be `const`-correct and your implementation must use a bitwise manipulation approach with two or more counters to simulate base-3 counting.
The required problem is a classic extension of the "Single Number" problem where every element appears three times except one. The key insight is to count occurrences of each bit across all numbers modulo 3. Since each number appears three times, any bit that appears in a number three times will have its count divisible by 3, leaving only the bits of the unique number. We can simulate a ternary counter using two integer variables, `ones` and `twos`, representing the current count of each bit modulo 3:
- State 0 (count = 0): both `ones` and `twos` have bit 0.
- State 1 (count = 1): `ones` has bit 1, `twos` has bit 0.
- State 2 (count = 2): `ones` has bit 0, `twos` has bit 1.
When processing a new number `num`, for each bit, if the current count is 0 and `num` has bit 1, it becomes 1; if 1 and bit 1, becomes 2; if 2 and bit 1, becomes 0 (since 3 mod 3 = 0). This can be implemented with the formulas:
`ones = (ones ^ num) & ~twos;`
`twos = (twos ^ num) & ~ones;`
The first line updates `ones` to toggle bits where `twos` is 0, and the second updates `twos` to toggle bits where `ones` (after update) is 0. This correctly simulates modulo-3 counting. After processing all numbers, `ones` holds the bits that have count modulo 3 equal to 1, which is exactly the unique number. Edge cases: if the array size is 1, the function returns that number; if the unique number is negative, bitwise operations handle the two's complement representation correctly since we operate on 32-bit signed integers (the bitwise operations work on the underlying bits). Time complexity is O(n) because we iterate through each element once and do constant work. Space complexity is O(1) as we only use two integer variables.
#include <vector>

// Returns the element that appears exactly once when all others appear three times.
int singleNumber(const std::vector<int>& nums) {
    int ones = 0;  // bits that have appeared modulo 3 count = 1
    int twos = 0;  // bits that have appeared modulo 3 count = 2
    for (int num : nums) {
        ones = (ones ^ num) & ~twos;
        twos = (twos ^ num) & ~ones;
    }
    return ones;
}
#include <cassert>
#include <vector>

// Forward declaration of the solution function.
int singleNumber(const std::vector<int>& nums);

int main() {
    // Basic case
    assert(singleNumber({2, 2, 3, 2}) == 3);
    // All positive, unique is small
    assert(singleNumber({0, 1, 0, 1, 0, 1, 99}) == 99);
    // Includes negative numbers
    assert(singleNumber({-1, -1, -1, -2, -2, -2, 5}) == 5);
    // Larger numbers and mixed signs
    assert(singleNumber({30000, 500, 30000, 500, 30000, 500, -12345}) == -12345);
    // Single element array
    assert(singleNumber({42}) == 42);
    // Unique is zero
    assert(singleNumber({7, 7, 7, 0}) == 0);
    // Random large array with duplicates
    assert(singleNumber({1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5}) == 5);
    // Edge with INT_MAX
    assert(singleNumber({2147483647, 2147483647, 2147483647, 1}) == 1);
    // Edge with INT_MIN
    assert(singleNumber({-2147483648, -2147483648, -2147483648, 77}) == 77);
    return 0;
}

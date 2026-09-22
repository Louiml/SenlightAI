Write a C++ function `findRepeatingAndMissing` that takes a non-empty vector of integers where each integer is between 1 and n (inclusive), n being the size of the vector, with exactly one number appearing twice and exactly one number missing from the range. The function should return a `pair<int,int>` where the first element is the repeating number and the second is the missing number. The input vector may be unsorted, may contain duplicates (but only one number appears twice; all others appear exactly once), and the vector size is at least 2. The function must not modify the input vector. Provide a solution using XOR-based bit manipulation to achieve O(n) time and O(1) extra space.
// The XOR approach leverages the fact that XORing a number with itself gives 0 and XORing with 0 gives the number itself. First, we compute `xorAll` by XORing all elements of the array and all numbers from 1 to n. This combines the repeating number (which appears twice in the array but once in the 1..n set, so net contribution is one copy) and the missing number (appears once in the array but once in the 1..n set, so net contribution is one copy) — actually wait, let me re-derive: The array has numbers from 1..n with one duplicate and one missing. So if we XOR all array elements and all 1..n numbers, the duplicate appears twice in the array and once in the 1..n set → XOR contributes one copy of the duplicate. The missing appears zero times in the array and once in 1..n set → XOR contributes one copy of the missing. So `xorAll` = duplicate XOR missing. Since duplicate != missing, `xorAll` is non-zero. We find the rightmost set bit in `xorAll` (using a bit mask). This bit distinguishes the duplicate and missing numbers because they differ at that bit. Then we partition all numbers (both array elements and all 1..n) into two groups: those with that bit set and those not. XORing each group separately yields one number from each group — these two numbers are the duplicate and missing, but we don't know which is which. To determine, we count occurrences of one candidate in the original array: if it appears twice, it's the repeating; otherwise it's the missing. Edge cases: n≥2, exactly one duplicate and one missing, input values guaranteed in [1,n]. Time complexity O(n) because we do a few linear passes. Space O(1).
#include <utility>
#include <vector>

// Returns {repeating, missing} for a vector containing 1..n with one duplicate and one missing.
std::pair<int, int> findRepeatingAndMissing(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    int xorAll = 0;

    // XOR all array elements and all numbers 1..n
    for (int i = 0; i < n; ++i) {
        xorAll ^= arr[i];
        xorAll ^= (i + 1);
    }

    // Find rightmost set bit (this bit differs between repeating and missing)
    int rightmostBit = xorAll & ~(xorAll - 1);

    int group0 = 0; // numbers with this bit not set
    int group1 = 0; // numbers with this bit set

    // Partition array elements
    for (int num : arr) {
        if ((num & rightmostBit) != 0) {
            group1 ^= num;
        } else {
            group0 ^= num;
        }
    }

    // Partition numbers 1..n
    for (int i = 1; i <= n; ++i) {
        if ((i & rightmostBit) != 0) {
            group1 ^= i;
        } else {
            group0 ^= i;
        }
    }

    // One of group0/group1 is repeating, the other is missing.
    // Count occurrences of group0 in the array.
    int count = 0;
    for (int num : arr) {
        if (num == group0) {
            ++count;
        }
    }

    if (count == 2) {
        return {group0, group1}; // group0 is repeating, group1 is missing
    } else {
        return {group1, group0}; // group1 is repeating, group0 is missing
    }
}
#include <cassert>
#include <vector>
#include <utility>

// Declare the solution function (assume it's included from above)
std::pair<int, int> findRepeatingAndMissing(const std::vector<int>& arr);

int main() {
    // Basic cases
    assert(findRepeatingAndMissing({3, 1, 2, 5, 4, 6, 7, 5}) == std::make_pair(5, 8));
    assert(findRepeatingAndMissing({1, 2, 2, 4}) == std::make_pair(2, 3));
    assert(findRepeatingAndMissing({2, 2}) == std::make_pair(2, 1));
    assert(findRepeatingAndMissing({1, 1}) == std::make_pair(1, 2));

    // Larger, unsorted with duplicates
    std::vector<int> a = {10, 7, 5, 3, 9, 6, 1, 2, 4, 10, 8}; // n=11, missing 11, repeat 10
    assert(findRepeatingAndMissing(a) == std::make_pair(10, 11));

    // Edge case: repeat at start, missing at end
    assert(findRepeatingAndMissing({5, 5, 1, 2, 3}) == std::make_pair(5, 4));

    // Edge case: repeat at end, missing at start
    assert(findRepeatingAndMissing({2, 3, 4, 4}) == std::make_pair(4, 1));

    // Verify the solution does not modify input
    std::vector<int> original = {1, 3, 3, 4};
    std::vector<int> copy = original;
    findRepeatingAndMissing(original);
    assert(original == copy);

    return 0;
}

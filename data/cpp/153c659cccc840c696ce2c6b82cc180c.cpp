/*
Implement a function `int rangeSum(const std::vector<int>& data, int left, int right)` that, given a vector of integers and two inclusive indices, returns the sum of the elements from index `left` to `right`, inclusive, after repeatedly applying the following operation simulated in the provided snippet: for every element whose index is divisible by 3, subtract 2 from it, regardless of whether that index falls inside `[left, right]` or not (the mutation happens globally before the query). Assume `0 <= left <= right < data.size()`. The function must correctly compute the mutated sum without modifying the original vector (i.e., it should work on a copy or compute the result mathematically). The solution must handle empty vectors gracefully (return 0), and for any non-empty input, the result must reflect the global mutation applied once.
*/
#include <vector>

// Compute the sum of elements from data[left..right] after applying a global
// modification: every element at an index divisible by 3 is decreased by 2.
// The original vector is not modified.
int rangeSum(const std::vector<int>& data, int left, int right) {
    if (data.empty() || left > right) {
        return 0;
    }

    long long sum = 0;
    for (int i = left; i <= right; ++i) {
        sum += data[i];
    }

    // Count indices in [left, right] that are multiples of 3.
    // Multiples of 3 up to `right`: right/3 (integer division).
    // Multiples of 3 up to `left-1`: (left-1)/3 (integer division, safe for left=0).
    int countMultiples3 = (right / 3) - ((left - 1) / 3);

    // Subtract 2 for each such index.
    sum -= 2LL * countMultiples3;

    return static_cast<int>(sum);
}
#include <cassert>
#include <vector>

// Declare the function under test (exact signature from solution).
int rangeSum(const std::vector<int>& data, int left, int right);

int main() {
    // Basic test: indices 0,3 are multiples of 3, subtract 2 each.
    std::vector<int> v1 = {5, 1, 2, 4, 3};
    // Original sum [0..4] = 15. Subtract 2 for index0 and index3 => 15 - 4 = 11.
    assert(rangeSum(v1, 0, 4) == 11);

    // Range not starting at 0.
    // Indices 1..3: original sum=1+2+4=7. Only index3 is multiple of 3, subtract 2 => 5.
    assert(rangeSum(v1, 1, 3) == 5);

    // Single element at index 2 (not multiple of 3).
    assert(rangeSum(v1, 2, 2) == 2);

    // Single element at index 3 (multiple of 3), original 4 - 2 = 2.
    assert(rangeSum(v1, 3, 3) == 2);

    // All negative numbers.
    std::vector<int> v2 = {-1, -4, -7};
    // Original sum = -12. indices 0 is multiple of 3, subtract 2 => -14.
    assert(rangeSum(v2, 0, 2) == -14);

    // Range with no multiples of 3 (indices 1..2).
    assert(rangeSum(v2, 1, 2) == -11); // -4 + -7 = -11

    // Empty vector.
    std::vector<int> v3;
    assert(rangeSum(v3, 0, 0) == 0);

    // Large range (all indices) – ensure count formula works.
    std::vector<int> v4 = {10, 10, 10, 10, 10, 10, 10}; // size 7
    // Original sum = 70. Multiples of 3 in [0..6]: 0,3,6 => three elements, subtract 6 => 64.
    assert(rangeSum(v4, 0, 6) == 64);

    // Range starting at a multiple of 3.
    assert(rangeSum(v4, 3, 6) == 40 - 4); // indices 3,4,5,6 sum=40, only 3 and 6 are multiples => 40-4=36

    return 0;
}
// The key insight is that the operation only affects indices that are multiples of 3 (i.e., `i % 3 == 0`), subtracting 2 from each such element. Since this mutation is global and applied exactly once before the query, the final sum over the requested range can be computed by first summing the original elements in the range, then subtracting `2 * k`, where `k` is the number of indices in `[left, right]` that are divisible by 3. The count `k` can be calculated directly using integer arithmetic: the number of multiples of 3 in `[left, right]` is `right/3 - (left-1)/3` (with integer division), or equivalently `floor(right/3) - floor((left-1)/3)` where `floor` for non-negative integers is just division. Edge cases: if the vector is empty, return 0; if the range is valid, the arithmetic handles any size, including large values (use `long long` to avoid overflow if the input might be large, but for the test we can stay with `int`). Time complexity is O(1) (or O(n) if we naively iterate, but the formula is O(1)), and space complexity is O(1) since we avoid copying. The solution must be `const` correct: the function takes a `const std::vector<int>&` and does not modify it.

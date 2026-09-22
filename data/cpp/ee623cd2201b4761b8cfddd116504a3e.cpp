// Write a standalone C++ function named `sumAndExtremes` that takes a `std::vector<int>` as input (by `const` reference) and returns a `std::tuple<long long, long long, long long>` containing the minimum element, the maximum element, and the sum of all elements, in that order. The input vector is guaranteed to be non-empty. The function must handle negative numbers, large values (up to 2×10^9 in magnitude), and duplicate values correctly. The result's sum should be computed as a `long long` to avoid overflow. Your code must include only the function definition and required headers, with no `main` function.
// The solution initializes `minimum` and `maximum` using the first element of the vector, and sets `sum` to that element. Then it iterates through the remaining elements starting from index 1, updating `minimum` and `maximum` using `std::min` and `std::max`, and accumulating the sum. Since the vector is non-empty, no special handling is needed for an empty container. Edge cases: a single element (all three values equal), duplicates (no effect on min/max), and large magnitudes (using `long long` for sum prevents overflow). Time complexity is O(n) with a single pass. Space complexity is O(1) auxiliary, aside from the input vector itself.
#include <vector>
#include <tuple>
#include <algorithm>
#include <cstdint>

// Returns (minimum, maximum, sum) of the given non-empty vector.
// Uses long long for the sum to avoid overflow.
std::tuple<long long, long long, long long> sumAndExtremes(const std::vector<int>& nums) {
    long long minimum = nums[0];
    long long maximum = nums[0];
    long long sum = nums[0];
    
    for (std::size_t i = 1; i < nums.size(); ++i) {
        long long value = nums[i];
        minimum = std::min(minimum, value);
        maximum = std::max(maximum, value);
        sum += value;
    }
    
    return std::make_tuple(minimum, maximum, sum);
}
#include <cassert>
#include <vector>
#include <tuple>

// The function under test is declared above (not repeated here).
int main() {
    // Basic case with positives
    std::vector<int> v1 = {1, 2, 3, 4};
    auto r1 = sumAndExtremes(v1);
    assert(std::get<0>(r1) == 1 && std::get<1>(r1) == 4 && std::get<2>(r1) == 10);
    
    // Negative numbers
    std::vector<int> v2 = {-5, -1, -10, -3};
    auto r2 = sumAndExtremes(v2);
    assert(std::get<0>(r2) == -10 && std::get<1>(r2) == -1 && std::get<2>(r2) == -19);
    
    // Single element
    std::vector<int> v3 = {7};
    auto r3 = sumAndExtremes(v3);
    assert(std::get<0>(r3) == 7 && std::get<1>(r3) == 7 && std::get<2>(r3) == 7);
    
    // Duplicates
    std::vector<int> v4 = {3, 3, 3, 3};
    auto r4 = sumAndExtremes(v4);
    assert(std::get<0>(r4) == 3 && std::get<1>(r4) == 3 && std::get<2>(r4) == 12);
    
    // Mixed large positives and negatives
    std::vector<int> v5 = {2000000000, -2000000000, 1000000000, -1000000000};
    auto r5 = sumAndExtremes(v5);
    assert(std::get<0>(r5) == -2000000000 && std::get<1>(r5) == 2000000000 && std::get<2>(r5) == 0);
    
    // Unsorted with random order
    std::vector<int> v6 = {42, -17, 0, 99, -100, 25};
    auto r6 = sumAndExtremes(v6);
    assert(std::get<0>(r6) == -100 && std::get<1>(r6) == 99 && std::get<2>(r6) == 49);
    
    // All negative equal values
    std::vector<int> v7 = {-1, -1, -1};
    auto r7 = sumAndExtremes(v7);
    assert(std::get<0>(r7) == -1 && std::get<1>(r7) == -1 && std::get<2>(r7) == -3);
    
    // Large single value to verify long long sum (though vector is int, sum of many can overflow int)
    std::vector<int> v8 = {1000000000, 1000000000, 1000000000, 1000000000};
    auto r8 = sumAndExtremes(v8);
    assert(std::get<0>(r8) == 1000000000 && std::get<1>(r8) == 1000000000 && std::get<2>(r8) == 4000000000LL);
    
    return 0;
}

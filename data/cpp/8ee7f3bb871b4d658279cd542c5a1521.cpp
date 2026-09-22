/*
Write a C++ function `long long absoluteSum(const std::vector<long long>& numbers)` that takes a vector of integers and returns the absolute value of their sum. The function must handle vectors of any non-negative length, including empty vectors (returning 0), and must correctly process negative numbers and large values that could overflow a 32-bit integer. The function should use `long long` arithmetic throughout and compute `std::llabs` (or `std::abs` with `long long`) on the final sum, ensuring no intermediate overflow occurs beyond the specified type. The solution must be standalone, include only necessary headers, and avoid any global state or input/output operations.
*/

#include <vector>
#include <cstdlib>   // for std::llabs (C++11) or std::abs with <cstdlib>
#include <cstddef>   // for std::size_t

// Returns the absolute value of the sum of all elements in the vector.
// For an empty vector, returns 0.
long long absoluteSum(const std::vector<long long>& numbers) {
    long long total = 0;
    for (std::size_t i = 0; i < numbers.size(); ++i) {
        total += numbers[i];
    }
    // std::llabs returns long long absolute value; safe for any long long.
    return std::llabs(total);
}

#include <cassert>
#include <vector>

// Declare the solution function (must match the provided signature).
long long absoluteSum(const std::vector<long long>& numbers);

int main() {
    // Basic positive numbers
    assert(absoluteSum({1, 2, 3, 4}) == 10);
    // Negative numbers
    assert(absoluteSum({-1, -2, -3}) == 6);
    // Mixed signs
    assert(absoluteSum({5, -3, 2, -1}) == 3);
    // Single element positive
    assert(absoluteSum({7}) == 7);
    // Single element negative
    assert(absoluteSum({-7}) == 7);
    // Empty vector
    assert(absoluteSum({}) == 0);
    // Zeros and duplicates
    assert(absoluteSum({0, 0, 0}) == 0);
    // Large values that overflow int but fit in long long
    assert(absoluteSum({2147483647LL, 2147483647LL}) == 4294967294LL);
    // Sum that is negative large
    assert(absoluteSum({-2147483648LL, -2147483648LL}) == 4294967296LL);
    return 0;
}

// The core algorithm is straightforward: iterate through the vector, accumulating each element into a `long long` variable initialized to 0. After processing all elements, apply the absolute value function `std::llabs` (or `std::abs` for `long long`) to handle negative sums safely. Edge cases: (1) an empty vector should return 0 because the sum of zero elements is 0 and `abs(0)=0`; (2) a single negative element returns its positive magnitude; (3) very large positive/negative sums must fit within `long long` range—the problem constraints implicitly avoid overflow if the caller provides valid input, but we must not use `int` for accumulation to avoid overflow on sums exceeding ±2^31−1. Time complexity is O(n) where n is the vector size, and space complexity is O(1) beyond the input vector (only a counter variable). The solution avoids any special handling for signs during accumulation; absolute value is applied once at the end.

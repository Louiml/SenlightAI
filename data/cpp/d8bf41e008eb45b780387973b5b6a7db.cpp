// Write a C++ function that takes a vector of integers and returns the absolute value of their sum as a long long. The function should handle positive, negative, and zero values, and must work correctly even when the sum would overflow a 32-bit int (assume each input element fits in a 32-bit signed int, but the sum may exceed that range). The function should be named `absoluteSum`, accept a `const std::vector<int>&` parameter, and return a `long long`.
The solution is straightforward: iterate through the vector once, accumulating the sum into a `long long` variable (to avoid overflow from adding many 32-bit ints). After processing all elements, compute the absolute value. The key edge case is when the sum equals the minimum possible `long long` value (i.e., `LLONG_MIN`), where taking the absolute value directly would overflow. However, since each input element is a 32-bit int, the sum of up to \(2^{31}-1\) elements each of magnitude at most \(2^{31}-1\) can reach at most \( (2^{31}-1)^2 \), which is about \(2^{62}\), safely below `LLONG_MAX`. Therefore, the sum can never be `LLONG_MIN`. For general safety, we can still handle the theoretical overflow by using a branch: if the sum is negative, return `-sum`; otherwise return `sum`. Time complexity is \(O(n)\) where \(n\) is the vector size, and space complexity is \(O(1)\) auxiliary.
#include <vector>
#include <cstddef>

// Return the absolute value of the sum of all integers in the given vector.
// Uses long long to avoid overflow when summing many 32-bit integers.
long long absoluteSum(const std::vector<int>& numbers) {
    long long total = 0;
    for (std::size_t i = 0; i < numbers.size(); ++i) {
        total += numbers[i];
    }
    // If total is negative, negate it; otherwise keep as is.
    // Since total is in range [-2^62, 2^62], no overflow occurs here.
    return total < 0 ? -total : total;
}
#include <cassert>
#include <vector>

// Function under test (for completeness, include the header content or link it)
long long absoluteSum(const std::vector<int>& numbers);

int main() {
    // Basic positive sum
    assert(absoluteSum({1, 2, 3}) == 6);
    // Basic negative sum
    assert(absoluteSum({-1, -2, -3}) == 6);
    // Mixed positive and negative
    assert(absoluteSum({-5, 10, -3}) == 2);
    // Single positive
    assert(absoluteSum({42}) == 42);
    // Single negative
    assert(absoluteSum({-42}) == 42);
    // Zero elements
    assert(absoluteSum({0, 0, 0}) == 0);
    // Large magnitude to test long long (sum near 2^62)
    assert(absoluteSum({2147483647, 2147483647, 2147483647}) == 6442450941LL);
    // A negative sum that is large in magnitude
    assert(absoluteSum({-1000000000, -1000000000, -1000000000, -1000000000}) == 4000000000LL);
    return 0;
}

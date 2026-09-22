// Write a C++ function `inclusiveDoublePrefixSum` that takes a `const std::vector<int>&` and returns a `std::vector<long long>` where each element at index `i` is the sum of the doubled values of all elements from index `0` through `i` (inclusive). The function must support large input sizes efficiently, using parallel execution to compute the prefix sums. The returned vector must have the same size as the input. The function must handle empty input by returning an empty vector. The input may contain arbitrary integers (positive, negative, zero), and overflow should be avoided by using a wider type (`long long`) for the accumulation. The function must not modify the input vector.

// The core algorithm is an inclusive scan (prefix sum) with an unary transform that doubles each element. C++17's `<execution>` and `<numeric>` provide `std::transform_inclusive_scan` which can be called with `std::execution::par` to parallelize the computation for large vectors. The transform step is `[](int n) { return static_cast<long long>(n) * 2; }`, and the binary operation is `std::plus<long long>()`. Edge cases: empty input should return an empty vector without invoking the algorithm (since call with empty range is safe but we can early-return for clarity). The result type must be `long long` to avoid overflow when doubling large ints and accumulating. Time complexity is O(n) work, but with parallel execution the wall-clock time may be reduced on multi-core systems; space complexity is O(n) for the output vector. The implementation should use a descriptive free function and `const` correctness for the input parameter.

#include <vector>
#include <numeric>
#include <execution>
#include <functional>

// Compute inclusive prefix sum of doubled elements.
// Returns vector of long long where result[i] = sum_{j=0..i} (2 * input[j]).
std::vector<long long> inclusiveDoublePrefixSum(const std::vector<int>& input) {
    if (input.empty()) {
        return {};
    }
    std::vector<long long> result(input.size());
    std::transform_inclusive_scan(
        std::execution::par,
        input.begin(),
        input.end(),
        result.begin(),
        std::plus<long long>(),
        [](int n) -> long long {
            return static_cast<long long>(n) * 2;
        }
    );
    return result;
}

#include <cassert>
#include <vector>

// Declaration from the solution
std::vector<long long> inclusiveDoublePrefixSum(const std::vector<int>& input);

int main() {
    // Basic case
    std::vector<int> v1 = {1, 2, 3};
    auto r1 = inclusiveDoublePrefixSum(v1);
    assert(r1.size() == 3);
    assert(r1[0] == 2);
    assert(r1[1] == 6);
    assert(r1[2] == 12);

    // Negative values
    std::vector<int> v2 = {-1, -2, 3};
    auto r2 = inclusiveDoublePrefixSum(v2);
    assert(r2.size() == 3);
    assert(r2[0] == -2);
    assert(r2[1] == -6);
    assert(r2[2] == 0);

    // Single element
    std::vector<int> v3 = {5};
    auto r3 = inclusiveDoublePrefixSum(v3);
    assert(r3.size() == 1);
    assert(r3[0] == 10);

    // Empty input
    std::vector<int> v4;
    auto r4 = inclusiveDoublePrefixSum(v4);
    assert(r4.empty());

    // All zeros
    std::vector<int> v5 = {0, 0, 0};
    auto r5 = inclusiveDoublePrefixSum(v5);
    assert(r5.size() == 3);
    for (long long x : r5) assert(x == 0);

    // Large values (overflow check)
    std::vector<int> v6 = {1000000000, 1000000000};
    auto r6 = inclusiveDoublePrefixSum(v6);
    assert(r6[0] == 2000000000LL);
    assert(r6[1] == 4000000000LL);

    // Verify with manual computation for a longer vector
    std::vector<int> v7 = {1, -1, 2, -2, 3, -3};
    auto r7 = inclusiveDoublePrefixSum(v7);
    std::vector<long long> expected = {2, 0, 4, 0, 6, 0};
    assert(r7 == expected);
}

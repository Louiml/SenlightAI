/*
Write a standalone C++ function named `sumChannels` that accepts a `std::vector<int>` representing a flattened row-major multi-channel array (where each element is a vector of `cn` consecutive channel values), along with an integer channel count `cn` (where `cn` is 1, 2, 3, or 4). The function should return a `std::vector<long long>` of length `cn`, where each entry is the sum of all values for that specific channel across the entire input. For example, if the input is `[1,2,3,4,5,6]` and `cn=2`, then channel 0 sums to `1+3+5=9` and channel 1 sums to `2+4+6=10`, returning `[9,10]`. Handle empty input gracefully by returning a vector of zeros of size `cn`. The implementation must be self-contained without any external libraries beyond standard headers, and must use appropriate `const` correctness.
*/

#include <vector>
#include <cstddef>

// Sum each channel of a flattened multi-channel array.
// `cn` is the number of channels (1..4). The input `data` must have
// length that is a multiple of `cn`. Returns a vector of size `cn`
// containing the sum for each channel.
std::vector<long long> sumChannels(const std::vector<int>& data, int cn) {
    // Initialize result with zeros for each channel.
    std::vector<long long> result(static_cast<std::size_t>(cn), 0LL);

    // If cn is invalid (<=0), return empty vector (or handle gracefully).
    if (cn <= 0) {
        return {};
    }

    // Iterate over each group of `cn` elements.
    const std::size_t n = data.size();
    for (std::size_t i = 0; i < n; i += static_cast<std::size_t>(cn)) {
        // Add each channel value in this group to the accumulator.
        for (int j = 0; j < cn; ++j) {
            result[static_cast<std::size_t>(j)] += data[i + static_cast<std::size_t>(j)];
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Declare the function (or include the previous code here).
std::vector<long long> sumChannels(const std::vector<int>& data, int cn);

int main() {
    // Test case 1: 2 channels, 3 groups.
    assert(sumChannels({1,2,3,4,5,6}, 2) == std::vector<long long>({9, 12}));

    // Test case 2: 1 channel (just total sum).
    assert(sumChannels({10,20,30}, 1) == std::vector<long long>({60}));

    // Test case 3: 3 channels, 2 groups.
    assert(sumChannels({1,2,3,4,5,6}, 3) == std::vector<long long>({5, 7, 9}));

    // Test case 4: 4 channels, single group.
    assert(sumChannels({7,8,9,10}, 4) == std::vector<long long>({7, 8, 9, 10}));

    // Test case 5: empty input returns zeros of size cn.
    assert(sumChannels({}, 3) == std::vector<long long>({0,0,0}));

    // Test case 6: negative values.
    assert(sumChannels({-1,-2,-3,-4}, 2) == std::vector<long long>({-4, -6}));

    // Test case 7: large values (no overflow in long long).
    assert(sumChannels({2000000000, 2000000000}, 1) == std::vector<long long>({4000000000LL}));

    // Test case 8: cn = 4 with multiple groups, including zeros.
    assert(sumChannels({1,0,0,0, 0,2,0,0, 0,0,3,0, 0,0,0,4}, 4) == std::vector<long long>({1,2,3,4}));

    return 0;
}

// The core algorithm is straightforward: iterate over the flattened input in groups of `cn` consecutive elements. For each group, add the `j`-th element (for `j` from `0` to `cn-1`) to the accumulator for channel `j`. Because the input is row-major and contiguous, this directly corresponds to summing each channel independently. Important edge cases include: empty input (return zeros of size `cn`), `cn` potentially not dividing the input length exactly (the description assumes valid input but a defensive implementation can still process full groups; however, for robustness, we only need to handle the case where the input length is a multiple of `cn`, as stated implicitly by the "flattened row-major multi-channel array" description). The result type is `long long` to avoid overflow when summing many `int` values. Time complexity is \(O(N)\) where \(N\) is the total number of values, and space complexity is \(O(cn)\) for the output vector, which is constant since `cn` is at most 4. No additional data structures are needed.

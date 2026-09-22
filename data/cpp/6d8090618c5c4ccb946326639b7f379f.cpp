Write a C++ function that, given a vector of 32-bit unsigned integers, returns a new vector containing only those elements that appear an odd number of times in the input, but with all duplicate occurrences of any such element collapsed to exactly one copy. In other words, for every value whose total frequency in the input is odd, the output should contain that value exactly once. The relative order of the output values should follow the order of their first occurrence in the input. The function should be named `filterOddFrequency` and should take a `const std::vector<uint32_t>&` and return a `std::vector<uint32_t>`. The input vector may contain zero or more elements, and you may assume values fit within 32-bit unsigned integers. Do not modify the input vector. The function must not use any global variables or standard library algorithms that directly provide frequency counting (such as `std::count_if` or sorting-based grouping) — you must implement the logic manually using a hash map or similar structure.
// The core idea is to track the parity of each value’s frequency as we scan the input from left to right. A hash map (`std::unordered_map<uint32_t, bool>`) can store whether the current count of each value is odd or even. Initially, no value has been seen, so we consider its parity as even (false). For each element `x` in the input, toggle its parity in the map: if it was even, it becomes odd; if it was odd, it becomes even. To preserve the first‑occurrence order, we also need to remember the order in which values first appeared. When we encounter a value for the first time (i.e., it was not previously in the map), we record it in a separate order list. After processing all elements, we iterate over that order list and include a value in the result only if its final parity is odd. Edge cases: an empty input yields an empty output; a value appearing twice becomes even and is excluded; a value appearing three times remains odd and is included once; the order is determined solely by the first occurrence, not by the last. Time complexity is O(n) on average for n input elements, because each element causes a constant‑time hash map operation and a constant‑time push to the order list. Auxiliary space is O(u) where u is the number of distinct values in the input, since we store one parity and one order entry per distinct value.
#include <vector>
#include <cstdint>
#include <unordered_map>

// Given a vector of unsigned 32-bit integers, return a new vector containing
// each value that appears an odd number of times, with each such value
// occurring exactly once and preserving the order of first appearance.
std::vector<uint32_t> filterOddFrequency(const std::vector<uint32_t>& input) {
    // Map each value to its current parity: true = odd count, false = even count.
    std::unordered_map<uint32_t, bool> parity;
    // Store distinct values in order of first occurrence.
    std::vector<uint32_t> firstOrder;

    for (uint32_t val : input) {
        auto it = parity.find(val);
        if (it == parity.end()) {
            // First time seeing this value: mark as odd, record order.
            parity.emplace(val, true);
            firstOrder.push_back(val);
        } else {
            // Toggle parity.
            it->second = !it->second;
        }
    }

    // Build result from values whose final parity is odd.
    std::vector<uint32_t> result;
    for (uint32_t val : firstOrder) {
        if (parity[val]) {
            result.push_back(val);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Declaration of the function under test.
std::vector<uint32_t> filterOddFrequency(const std::vector<uint32_t>& input);

int main() {
    // Empty input.
    assert(filterOddFrequency({}) == std::vector<uint32_t>{});

    // All values appear once → all included.
    assert(filterOddFrequency({1, 2, 3}) == (std::vector<uint32_t>{1, 2, 3}));

    // Values appearing twice are removed.
    assert(filterOddFrequency({5, 5, 9}) == (std::vector<uint32_t>{9}));

    // Value appearing three times stays (odd), duplicate collapsed.
    assert(filterOddFrequency({7, 7, 7}) == (std::vector<uint32_t>{7}));

    // Mixed frequencies, check order of first occurrence.
    assert(filterOddFrequency({1, 2, 1, 3, 2}) == (std::vector<uint32_t>{1, 3}));

    // Larger example with repeated values.
    std::vector<uint32_t> input = {10, 20, 10, 30, 20, 10, 40};
    // Frequencies: 10→3 (odd), 20→2 (even), 30→1 (odd), 40→1 (odd).
    // First occurrences order: 10, 20, 30, 40 → output {10, 30, 40}.
    assert(filterOddFrequency(input) == (std::vector<uint32_t>{10, 30, 40}));

    // All values appear exactly twice → all removed.
    assert(filterOddFrequency({8, 8, 6, 6}) == std::vector<uint32_t>{});

    // Single element appears once → included.
    assert(filterOddFrequency({42}) == (std::vector<uint32_t>{42}));

    // Zero is a valid value.
    assert(filterOddFrequency({0, 0, 0}) == (std::vector<uint32_t>{0}));
    assert(filterOddFrequency({0, 1, 0}) == (std::vector<uint32_t>{1}));

    return 0;
}

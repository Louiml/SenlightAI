Given two positive integers `n` and `m`, write a C++ function `generatePairs` that takes both integers and returns a `std::vector<std::string>` where each string is formatted as `"i i+1"` for every integer `i` from `0` up to and including `min(n, m)`. The output must preserve the exact ordering of `i` from smallest to largest, with no trailing spaces or extra lines. The function should handle cases where `n` or `m` equals 1, where the minimum is 0 (but since inputs are positive, minimum will be ≥1), and potentially large values (up to 10^9) to ensure no overflow in the loop counter or string construction. Return an empty vector if the minimum is negative (though not expected) for robustness.
#include <cassert>
#include <vector>
#include <string>

// Assume generatePairs is defined above.

int main() {
    // Test basic case where n == m
    std::vector<std::string> r1 = generatePairs(3, 3);
    assert(r1.size() == 4);
    assert(r1[0] == "0 1");
    assert(r1[1] == "1 2");
    assert(r1[2] == "2 3");
    assert(r1[3] == "3 4");

    // Test where n < m
    std::vector<std::string> r2 = generatePairs(2, 5);
    assert(r2.size() == 3);
    assert(r2[0] == "0 1");
    assert(r2[1] == "1 2");
    assert(r2[2] == "2 3");

    // Test where m < n
    std::vector<std::string> r3 = generatePairs(7, 1);
    assert(r3.size() == 2);
    assert(r3[0] == "0 1");
    assert(r3[1] == "1 2");

    // Test where both are 1
    std::vector<std::string> r4 = generatePairs(1, 1);
    assert(r4.size() == 2);
    assert(r4[0] == "0 1");
    assert(r4[1] == "1 2");

    // Test large values (but small enough to not overflow)
    std::vector<std::string> r5 = generatePairs(10, 10);
    assert(r5.size() == 11);
    assert(r5.back() == "10 11");
    assert(r5.front() == "0 1");

    // Test with zero? Inputs are positive, but function should treat min(0,5)=0
    std::vector<std::string> r6 = generatePairs(0, 5);
    assert(r6.size() == 1);
    assert(r6[0] == "0 1");

    // Test with negative n (robustness)
    std::vector<std::string> r7 = generatePairs(-2, 4);
    assert(r7.empty()); // since min(-2,4) = -2, loop doesn't run

    return 0;
}
#include <vector>
#include <string>
#include <algorithm>

// Generate formatted pairs "i i+1" for i = 0..min(n,m) inclusive.
// Returns a vector of strings in increasing order of i.
std::vector<std::string> generatePairs(int n, int m) {
    std::vector<std::string> result;
    int limit = std::min(n, m);

    for (int i = 0; i <= limit; ++i) {
        result.push_back(std::to_string(i) + " " + std::to_string(i + 1));
    }

    return result;
}
// The solution directly iterates from `i = 0` to `i = min(n, m)` inclusive, constructing a string for each step in the format `"i i+1"`. The key algorithm is a simple linear loop; we compute `limit = std::min(n, m)` and then for each `i` we use `std::to_string(i)` and `std::to_string(i+1)` to form the pair. Edge cases: if `n` or `m` is 1, the loop runs from 0 to 1 inclusive (since min=1), producing pairs (0 1) and (1 2). If both are large, the loop runs `limit+1` iterations, but since `i+1` can be up to `limit+1`, we must ensure `limit` itself doesn't exceed `INT_MAX` to avoid overflow in `i+1` (inputs are positive ints, so fine). The time complexity is O(min(n,m)) because we iterate exactly that many times plus one, and each string construction is O(log(i)) for the integers. The space complexity is O(min(n,m)) for storing all the output strings, plus temporary string overhead. No sorting or additional data structures are needed.

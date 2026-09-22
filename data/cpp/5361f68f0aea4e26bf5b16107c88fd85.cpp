/*
Write a C++ function named `countConfidentDecisions` that takes an integer `n` and a vector of triples (each triple containing three integers `p`, `v`, `t` representing the certainty of three friends on a problem), and returns the number of triples where the sum of the three integers is at least 2. Each triple represents a problem: if two or more of the friends are certain (each certainty is a 0 or 1), the team will solve the problem. The function should read from a robust input format where `n` is provided first, followed by `n` lines each containing three space-separated integers (each guaranteed to be either 0 or 1). Handle the edge case where `n` could be 0, returning 0. The function should not print anything.
*/
#include <vector>
#include <cstddef>

// Count the number of triples where the sum of the three integers is at least 2.
// Each triple is expected to contain only 0s and 1s.
int countConfidentDecisions(const std::vector<std::tuple<int, int, int>>& triples) {
    int count = 0;
    for (const auto& triple : triples) {
        const int sum = std::get<0>(triple) + std::get<1>(triple) + std::get<2>(triple);
        if (sum >= 2) {
            ++count;
        }
    }
    return count;
}
#include <cassert>
#include <tuple>
#include <vector>

// The solution function is assumed to be defined above; here we test it.
int countConfidentDecisions(const std::vector<std::tuple<int, int, int>>& triples);

int main() {
    // Empty input
    assert(countConfidentDecisions({}) == 0);
    
    // Single triple: (1,1,1) -> sum=3
    assert(countConfidentDecisions({{1,1,1}}) == 1);
    
    // Single triple: (0,0,0) -> sum=0
    assert(countConfidentDecisions({{0,0,0}}) == 0);
    
    // Multiple triples with mixed results
    std::vector<std::tuple<int, int, int>> triples1 = {{1,0,1}, {0,0,1}, {1,1,0}, {0,0,0}, {1,0,0}};
    // sums: 2,1,2,0,1 -> only 2 pass (first and third)
    assert(countConfidentDecisions(triples1) == 2);
    
    // Edge: exactly sum=2 (e.g., (1,1,0)) vs sum=1 (e.g., (1,0,0))
    assert(countConfidentDecisions({{1,1,0}, {1,0,0}, {0,1,1}}) == 2);
    
    // Larger test: all triples pass except one
    std::vector<std::tuple<int, int, int>> triples2 = {{1,1,1},{1,0,1},{0,1,1},{1,1,0},{0,0,1},{0,1,0},{1,0,0}};
    // sums: 3,2,2,2,1,1,1 -> pass count = 4
    assert(countConfidentDecisions(triples2) == 4);
    
    // Zero triples but with n=0 (empty vector) already tested
    
    // Data with only zeros
    assert(countConfidentDecisions({{0,0,0}, {0,0,0}}) == 0);
    
    // Data with all ones
    assert(countConfidentDecisions({{1,1,1}, {1,1,1}, {1,1,1}}) == 3);
    
    return 0;
}
// The solution is straightforward: iterate through each triple, compute the sum of the three integers, and increment a counter whenever the sum is at least 2. Since each integer is only 0 or 1, the sum is always between 0 and 3; the condition `sum >= 2` is equivalent to "at least two are 1". No special handling is needed for edge cases beyond the empty input case (n == 0 returns 0) and large n (the loop runs exactly n times). The algorithm runs in O(n) time because each triple is processed once with constant-time arithmetic, and uses O(1) auxiliary space (only the counter and temporary sum variable), excluding the storage of the input vector itself if passed by value; passing by const reference avoids copying and maintains O(1) additional space beyond the input.

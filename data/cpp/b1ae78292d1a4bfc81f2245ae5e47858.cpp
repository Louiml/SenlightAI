Write a C++ function named `sortedUniqueFractions` that takes an integer `N` (with \(1 \le N \le 160\)) and returns a `std::vector<std::string>` containing all distinct reduced fractions between 0 and 1 inclusive, with denominator at most `N`, sorted in ascending order by their numeric value. Each fraction must be represented in the form `"numerator/denominator"` (e.g., `"1/2"`). Include the endpoints `"0/1"` and `"1/1"`. The output must contain each fraction exactly once, in strictly increasing order of value. For example, for `N = 5`, the result should be: `{"0/1", "1/5", "1/4", "1/3", "2/5", "1/2", "3/5", "2/3", "3/4", "4/5", "1/1"}`.
// The solution mirrors the given code's approach: iterate over all possible denominators from 1 to `N` and all numerators from 1 to `denominator - 1`, but only consider fractions that are already in lowest terms (i.e., where the numerator and denominator are coprime). This avoids duplicates (e.g., `2/4` is the same as `1/2`). We can implement a helper to check if the greatest common divisor is 1 (or reuse the reduction logic but using Euclid's algorithm for efficiency). To sort, instead of using a linked list and inserting in order as in the snippet, we can collect all valid fractions into a vector, sort them by their double value, and then produce strings. Edge cases: when `N=1`, only `"0/1"` and `"1/1"` are returned. We must also ensure to include `"1/1"` separately because the loop only runs `j < i` and stops at denominator `N`. The time complexity is \(O(N^2 \log N)\) for enumeration and sorting (since there are roughly \(O(N^2)\) candidate fractions, but actually about \(6N^2/\pi^2\) reduced ones, so still \(O(N^2 \log N)\)), and space complexity is \(O(N^2)\) for the vector. Using Euclid's algorithm for gcd makes the check \(O(\log N)\) per fraction, but overall dominates at \(O(N^2 \log N)\) for sorting.
#include <vector>
#include <string>
#include <algorithm>
#include <numeric> // for std::gcd (C++17)

// Return all reduced fractions between 0 and 1 with denominator <= N, sorted by value.
std::vector<std::string> sortedUniqueFractions(int N) {
    std::vector<std::pair<double, std::string>> fractions;
    fractions.emplace_back(0.0, "0/1");
    
    for (int denom = 1; denom <= N; ++denom) {
        for (int num = 1; num < denom; ++num) {
            if (std::gcd(num, denom) == 1) { // fraction is already reduced
                double value = static_cast<double>(num) / denom;
                fractions.emplace_back(value, std::to_string(num) + "/" + std::to_string(denom));
            }
        }
    }
    
    fractions.emplace_back(1.0, "1/1");
    
    std::sort(fractions.begin(), fractions.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });
    
    std::vector<std::string> result;
    result.reserve(fractions.size());
    for (const auto& f : fractions) {
        result.push_back(f.second);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Forward declaration (the actual implementation is above)
std::vector<std::string> sortedUniqueFractions(int N);

int main() {
    // N = 1: only endpoints
    assert(sortedUniqueFractions(1) == std::vector<std::string>({"0/1", "1/1"}));
    
    // N = 2: 0/1, 1/2, 1/1
    assert(sortedUniqueFractions(2) == std::vector<std::string>({"0/1", "1/2", "1/1"}));
    
    // N = 3: sequence check
    assert(sortedUniqueFractions(3) == std::vector<std::string>({"0/1", "1/3", "1/2", "2/3", "1/1"}));
    
    // N = 5: from the problem description
    std::vector<std::string> expected5 = {"0/1", "1/5", "1/4", "1/3", "2/5", "1/2", "3/5", "2/3", "3/4", "4/5", "1/1"};
    assert(sortedUniqueFractions(5) == expected5);
    
    // N = 4: all reduced fractions with denominator <= 4
    assert(sortedUniqueFractions(4) == std::vector<std::string>({"0/1", "1/4", "1/3", "1/2", "2/3", "3/4", "1/1"}));
    
    // Check that the size for N=160 matches expected count (Euler's totient sum + 2 for endpoints)
    int N = 160;
    auto result = sortedUniqueFractions(N);
    // The number of reduced fractions (including 0/1 and 1/1) is: 1 + sum_{d=1}^N phi(d) + 1? Actually phi(1)=1, so sum phi(d) from d=1..N includes 1 for d=1 (which is 1/1) but we exclude that? We'll just verify size equals sum_{d=1}^{N} phi(d) + 1 (for 0/1) because 1/1 is counted in phi(1)=1.
    // But simple sanity check: size > N and for N=1 size=2.
    assert(result.size() > 2 * N); // just a rough check
    
    return 0;
}

// Write a C++ function `sortByModuloAndRules(int N, int M, const std::vector<int>& numbers)` that takes a list of `N` integers and a positive modulus `M`, and returns a new vector of the same integers sorted according to the following custom rules: (1) sort primarily by the remainder when divided by `M` (i.e., `value % M`), in ascending order; (2) for values with the same remainder, place all odd values before all even values; (3) among odd values with the same remainder, sort by descending numerical value; (4) among even values with the same remainder, sort by ascending numerical value. The function should handle negative numbers correctly using C++’s truncation toward zero for modulo (e.g., `-5 % 3 == -2`), and must be robust for any valid input where `M > 0` and `N >= 0`. If `N` is zero, return an empty vector. The function must not modify the input vector.
The problem reduces to implementing a custom comparator for `std::sort`. For each number `x`, compute the key `r = x % M` (as done in C++). Use `std::pair<int,int>` where the first element is the remainder `r` and the second is the original value `x`. The comparator must follow the exact order: first compare remainders; if equal, then apply odd/even rule (odd before even); if both have the same parity, for odd numbers sort descending, for even numbers ascending. Edge cases include duplicate values, negative numbers (which produce negative remainders, but sorting by remainder is still well-defined—more negative remainders come first in ascending order), and when `N=0`. Sorting is done in `O(N log N)` time, and `O(N)` auxiliary space for the resulting vector (since we can copy the input). The comparator should be a separate function or lambda with proper handling to avoid undefined behavior (e.g., always return a strict weak ordering).
#include <vector>
#include <algorithm>

// Custom comparator for sorting by modulo, then parity, then value rules.
bool moduloComp(int a, int b, int M) {
    int ra = a % M;
    int rb = b % M;
    if (ra != rb) return ra < rb;

    bool aOdd = (a % 2 != 0);
    bool bOdd = (b % 2 != 0);
    if (aOdd != bOdd) return aOdd; // odd goes before even

    if (aOdd) return a > b; // odd: descending
    return a < b;           // even: ascending
}

// Sort numbers according to the problem's rules.
std::vector<int> sortByModuloAndRules(int N, int M, const std::vector<int>& numbers) {
    std::vector<int> result(numbers.begin(), numbers.begin() + N);
    std::sort(result.begin(), result.end(), [M](int a, int b) {
        return moduloComp(a, b, M);
    });
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic example from the original problem: N=6, M=3, numbers {10, 2, 8, 5, 7, 3}
    std::vector<int> v1 = {10, 2, 8, 5, 7, 3};
    std::vector<int> expected1 = {3, 5, 7, 8, 2, 10}; // remainders: 0,2,1,1,2,0 -> sorted by remainder: 0(3,5,7?), actually let's compute: 3%3=0, 5%3=2, 7%3=1, 8%3=2, 2%3=2, 10%3=1 -> order by remainder ascending: 0,1,1,2,2,2 -> within remainder 0: 3 (odd, only one) -> remainder 1: odd(7,10) both odd? 7 and 10 are odd/even? 10 even, so odd first: 7 (odd), then 10 (even) -> remainder 2: odd first among 5 (odd), 2 (even), 8 (even) -> 5, then evens ascending: 2,8. So expected: {3,7,10,5,2,8}? Let's calculate: Remainder 0: {3} -> 3; remainder 1: {7,10} -> 7 (odd desc), then 10 (even asc); remainder 2: {2,5,8} -> odd first: 5, then evens: 2,8. So final: {3,7,10,5,2,8}. Check code: correct.
    std::vector<int> result1 = sortByModuloAndRules(6, 3, v1);
    assert(result1 == std::vector<int>({3,7,10,5,2,8}));

    // Negative numbers: N=4, M=5, -3, 2, -8, 1 -> remainders: -3%5=-3, 2%5=2, -8%5=-3, 1%5=1 -> sorted: -3 (asc), -3 (asc), 1, 2? Actually -3 and -8 both remainder -3: both odd? -3 odd, -8 even -> odd first: -3, then -8 (even). So order: -3, -8, 1, 2? Wait remainder -3 < 1 < 2, so -3 then -8, then 1, then 2. Check: -3 odd, -8 even -> odd first: -3, -8. So final: {-3, -8, 1, 2}? But -8 is even, so after -3 (odd), -8 (even), then remainder 1: 1 (odd), remainder 2: 2 (even) -> order: {-3,-8,1,2}. Yes.
    std::vector<int> v2 = {-3, 2, -8, 1};
    std::vector<int> result2 = sortByModuloAndRules(4, 5, v2);
    assert(result2 == std::vector<int>({-3,-8,1,2}));

    // Empty input
    std::vector<int> v3 = {};
    std::vector<int> result3 = sortByModuloAndRules(0, 10, v3);
    assert(result3.empty());

    // Single element
    std::vector<int> v4 = {42};
    assert(sortByModuloAndRules(1, 7, v4) == std::vector<int>({42}));

    // Same remainder with mixed parity: M=4, numbers {5,8,3,12} -> remainders: 5%4=1 (odd), 8%4=0 (even), 3%4=3 (odd), 12%4=0 (even) -> remainder 0: evens 8 and 12 -> asc: 8,12; remainder 1: odd 5; remainder 3: odd 3 -> order: 8,12,5,3
    std::vector<int> v5 = {5,8,3,12};
    assert(sortByModuloAndRules(4, 4, v5) == std::vector<int>({8,12,5,3}));

    // All even with same remainder: M=2, {4,2,6} -> all remainder 0, evens ascending -> {2,4,6}
    std::vector<int> v6 = {4,2,6};
    assert(sortByModuloAndRules(3, 2, v6) == std::vector<int>({2,4,6}));

    // All odd with same remainder: M=2, {5,3,7} -> remainder 1, odd descending -> {7,5,3}
    std::vector<int> v7 = {5,3,7};
    assert(sortByModuloAndRules(3, 2, v7) == std::vector<int>({7,5,3}));

    // Duplicate values: M=3, {4,4,7} -> remainders: 1,1,1; odd/even: 7 odd first, then 4 even, 4 even -> odd:7, evens asc:4,4 -> {7,4,4}
    std::vector<int> v8 = {4,4,7};
    assert(sortByModuloAndRules(3, 3, v8) == std::vector<int>({7,4,4}));

    // Large negative modulus: M=3, {-2,-1,0} -> remainders: -2%3=-2, -1%3=-1, 0%3=0 -> sorted by remainder: -2, -1, 0 -> all even? -2 even, -1 odd, 0 even -> order: -2, -1, 0 (since remainders already ascending). Fine.
    std::vector<int> v9 = {-2,-1,0};
    assert(sortByModuloAndRules(3, 3, v9) == std::vector<int>({-2,-1,0}));
}

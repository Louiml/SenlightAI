// Write a C++ function `bool areAllFactors(int n, const std::vector<int>& candidates)` that takes a positive integer `n` and a vector of integers, and returns `true` only if every integer in the vector is a positive divisor of `n`, and the vector contains exactly all positive divisors of `n` in ascending order without any omissions or extras. The function must work correctly for any `n` between 1 and 1000, and candidates may be empty (in which case it should return `false`, because an empty set cannot represent all divisors of a positive integer). The function must not rely on any global or static state.

#include <cassert>
#include <vector>

// Declaration of the solution function (assumed to be defined above in the same translation unit).
bool areAllFactors(int n, const std::vector<int>& candidates);

int main() {
    // n = 10, divisors are {1,2,5,10}
    assert(areAllFactors(10, {1,2,5,10}) == true);
    assert(areAllFactors(10, {1,2,5}) == false);       // missing 10
    assert(areAllFactors(10, {1,2,5,10,20}) == false); // extra element
    assert(areAllFactors(10, {2,5,10,1}) == false);    // wrong order

    // n = 1, divisors are {1}
    assert(areAllFactors(1, {1}) == true);
    assert(areAllFactors(1, {}) == false);             // empty list
    assert(areAllFactors(1, {1,1}) == false);          // duplicate

    // n = 12, divisors are {1,2,3,4,6,12}
    assert(areAllFactors(12, {1,2,3,4,6,12}) == true);
    assert(areAllFactors(12, {1,2,3,4,6}) == false);   // missing 12
    assert(areAllFactors(12, {1,2,3,4,5,12}) == false); // 5 not a divisor

    // n = 100, divisors are {1,2,4,5,10,20,25,50,100}
    assert(areAllFactors(100, {1,2,4,5,10,20,25,50,100}) == true);
    assert(areAllFactors(100, {1,2,4,5,10,20,25,50}) == false); // missing 100
    assert(areAllFactors(100, {}) == false);

    return 0;
}

#include <vector>

// Returns true if 'candidates' contains exactly all positive divisors of 'n' in ascending order.
bool areAllFactors(int n, const std::vector<int>& candidates) {
    std::vector<int> divisors;

    for (int i = 1; i <= n; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
        }
    }

    if (candidates.size() != divisors.size()) {
        return false;
    }

    for (std::size_t i = 0; i < divisors.size(); ++i) {
        if (candidates[i] != divisors[i]) {
            return false;
        }
    }

    return true;
}

// The solution computes all positive divisors of `n` by iterating from 1 to `n` and collecting those `i` where `n % i == 0`. Since the iteration is in increasing order, the resulting divisor list is naturally sorted ascending. The function then checks two conditions: (1) the size of the input vector must match the size of the computed divisor list; (2) every corresponding element must be equal. If either condition fails, return `false`; otherwise return `true`. Edge cases include `n = 1` (divisors = {1}), candidates with duplicate values (they would either cause size mismatch or unequal elements), candidates not sorted (size might match but an element will differ), and empty candidates (size mismatch → false). Time complexity is O(n + d) where d is the number of divisors, at most O(n) since d ≤ n. Space complexity is O(d) for the temporary divisor list, which is O(n) worst case. The function uses `const` for the input vector and passes it by reference to avoid copying.

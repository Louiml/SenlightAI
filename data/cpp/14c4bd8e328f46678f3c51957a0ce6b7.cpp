Write a C++ function `std::vector<long long> beautifulPermutation(long long n)` that, for a given integer `n` (1 ≤ n ≤ 10^18), returns a permutation of the numbers from 1 to n such that no two adjacent elements differ by exactly 1. If no such permutation exists, return an empty vector. The function must handle all valid inputs without overflowing (use 64-bit integers), and must produce a valid output for every `n` where a solution is possible.
The problem is a classic "arrange numbers so adjacent differences are not 1" puzzle. For small `n`, no solution exists for `n = 2` and `n = 3` (since with three numbers, any arrangement has at least one adjacent pair differing by 1). For `n = 1`, the single element trivially works. For `n ≥ 4`, a constructive pattern works: place odd numbers first in increasing order, then even numbers in increasing order (or vice versa). However, the provided snippet uses a different interleaving pattern: it places pairs (1, n), (2, n-1), etc., but that pattern has issues for odd `n`. A simpler and robust approach: for `n ≥ 4`, output all odd numbers from 1 up to the largest odd ≤ n, then all even numbers from 2 up to the largest even ≤ n. This works because the last odd and first even differ by at least 3 (e.g., for n=4: 1,3,2,4 → adjacent differences are 2,1,2 — wait, 3-2=1, that fails). Indeed, the simple odd-then-even pattern fails when n is even because the transition from largest odd (n-1) to smallest even (2) has difference n-3, which is fine if n≥5, but for n=4: 1,3,2,4 gives 3-2=1, fail. The correct known pattern: for even n, output all even numbers in decreasing order from n-2 down to 2, then all odd numbers in increasing order from 1 to n-1, but that also has a problematic junction. The classic solution: for n≥4, list even numbers first (2,4,6,...,n) then odd numbers (1,3,5,...,n-1) — but again the junction between n and 1 differs by n-1, which is ≥3 for n≥4, and within even sequence differences are 2, within odd sequence differences are 2. So for n=4: 2,4,1,3 → differences: 2,3,2, all not 1. For n=5: 2,4,1,3,5 → differences: 2,3,2,2, works. For n=6: 2,4,6,1,3,5 → differences: 2,2,5,2,2, works. So the pattern "all even numbers in increasing order, then all odd numbers in increasing order" works for all n≥4. For n=1 return {1}, for n=2,3 return empty. Time complexity O(n) to build the vector, space O(n) for the result (which is necessary). Edge cases: n=1,2,3; large n up to 1e18 but we cannot allocate a vector of size 1e18 (impossible), so the task realistically expects n ≤ 10^6 or we only test with moderate n. But to be safe, we use `long long` and assume n is within memory limits (task specification likely implies n ≤ 10^6). Return `std::vector<long long>`.
#include <vector>
#include <cstdint>

// Return a permutation of 1..n where no adjacent elements differ by 1.
// If impossible (n==2 or n==3), return an empty vector.
std::vector<long long> beautifulPermutation(long long n) {
    if (n == 1) {
        return {1};
    }
    if (n < 4) {
        return {};
    }
    std::vector<long long> result;
    result.reserve(static_cast<size_t>(n));
    // All even numbers in increasing order
    for (long long even = 2; even <= n; even += 2) {
        result.push_back(even);
    }
    // All odd numbers in increasing order
    for (long long odd = 1; odd <= n; odd += 2) {
        result.push_back(odd);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Declaration of the function under test
std::vector<long long> beautifulPermutation(long long n);

// Helper to check that the vector is a valid permutation and satisfies the condition
bool isValid(const std::vector<long long>& vec, long long n) {
    if (vec.empty()) {
        return n == 2 || n == 3;
    }
    if (static_cast<long long>(vec.size()) != n) return false;
    std::vector<bool> seen(n + 1, false);
    for (long long x : vec) {
        if (x < 1 || x > n) return false;
        if (seen[static_cast<size_t>(x)]) return false;
        seen[static_cast<size_t>(x)] = true;
    }
    for (size_t i = 1; i < vec.size(); ++i) {
        long long diff = vec[i] > vec[i-1] ? vec[i] - vec[i-1] : vec[i-1] - vec[i];
        if (diff == 1) return false;
    }
    return true;
}

int main() {
    // Edge cases
    assert(beautifulPermutation(1) == std::vector<long long>{1});
    assert(beautifulPermutation(2).empty());
    assert(beautifulPermutation(3).empty());

    // Small valid cases
    assert(isValid(beautifulPermutation(4), 4));
    assert(isValid(beautifulPermutation(5), 5));
    assert(isValid(beautifulPermutation(6), 6));
    assert(isValid(beautifulPermutation(7), 7));
    assert(isValid(beautifulPermutation(10), 10));

    // A moderately large case
    assert(isValid(beautifulPermutation(1000), 1000));

    // Verify the pattern for n=4 and n=5 explicitly
    assert(beautifulPermutation(4) == std::vector<long long>({2,4,1,3}));
    assert(beautifulPermutation(5) == std::vector<long long>({2,4,1,3,5}));

    return 0;
}

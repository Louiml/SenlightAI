// Write a C++ function that takes a positive integer `n` and returns a `std::vector<int>` containing all positive divisors of `n` in ascending order (from smallest to largest). The function should handle inputs from 1 up to 1,000,000 efficiently. For example, for `n = 12`, the output should be `{1, 2, 3, 4, 6, 12}`. You may assume the input is always a valid positive integer within the given range.
// The simplest approach is to iterate from 1 to `n` and check if `n` is divisible by each `i`, adding `i` when `n % i == 0`. However, for `n` up to 1,000,000, this takes O(n) time, which is acceptable but not optimal. A more efficient approach is to iterate only up to `sqrt(n)`. For each divisor `i` found, we add both `i` and `n / i` (unless they are equal, to avoid duplicates). Then we sort the collected divisors in ascending order, since they are generated in unsorted pairs. This reduces time complexity to O(sqrt(n) + k log k), where `k` is the number of divisors (at most ~240 for n ≤ 1,000,000). Edge cases: for `n = 1`, the only divisor is `1`; for perfect squares like `n = 16`, the square root `4` appears only once. Space complexity is O(k) for storing divisors.
#include <vector>
#include <cmath>
#include <algorithm>

// Return a vector of all positive divisors of n in ascending order.
std::vector<int> getDivisors(int n) {
    std::vector<int> divisors;
    if (n <= 0) return divisors;

    int limit = static_cast<int>(std::sqrt(n));
    for (int i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) {
                divisors.push_back(n / i);
            }
        }
    }
    std::sort(divisors.begin(), divisors.end());
    return divisors;
}
#include <cassert>
#include <vector>

// The solution function must be declared before main, or included here.
// For clarity, we place the function in an included header or define it above.
// Here we assume the function is defined in the same file before main.

int main() {
    // Test n = 1
    std::vector<int> r1 = getDivisors(1);
    assert(r1.size() == 1 && r1[0] == 1);

    // Test n = 12
    std::vector<int> r2 = getDivisors(12);
    std::vector<int> expected2 = {1, 2, 3, 4, 6, 12};
    assert(r2 == expected2);

    // Test n = 16 (perfect square)
    std::vector<int> r3 = getDivisors(16);
    std::vector<int> expected3 = {1, 2, 4, 8, 16};
    assert(r3 == expected3);

    // Test n = 17 (prime)
    std::vector<int> r4 = getDivisors(17);
    std::vector<int> expected4 = {1, 17};
    assert(r4 == expected4);

    // Test n = 100 (larger composite)
    std::vector<int> r5 = getDivisors(100);
    std::vector<int> expected5 = {1, 2, 4, 5, 10, 20, 25, 50, 100};
    assert(r5 == expected5);

    // Test n = 2
    std::vector<int> r6 = getDivisors(2);
    std::vector<int> expected6 = {1, 2};
    assert(r6 == expected6);

    // Test n = 999983 (prime near limit)
    std::vector<int> r7 = getDivisors(999983);
    assert(r7.size() == 2 && r7[0] == 1 && r7[1] == 999983);

    // Test n = 1000000 (max limit)
    std::vector<int> r8 = getDivisors(1000000);
    // Known divisor count for 1,000,000 is 49 (since 10^6 = 2^6 * 5^6, (6+1)*(6+1)=49)
    assert(r8.size() == 49);
    assert(r8.front() == 1);
    assert(r8.back() == 1000000);

    return 0;
}

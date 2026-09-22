// Write a C++ function that takes an integer `n` and returns the number of threads that would be created if we follow the pattern from the code snippet: for each index `i` from 0 to `n-1`, if `i` is even, one thread running a function that prints "Even thread" is created, and if `i` is odd, one thread running a function that prints "Odd thread" is created. The function should not actually create threads (since that would be nondeterministic and hard to test), but instead should return the count of even-indexed iterations and odd-indexed iterations as a pair, i.e., a `std::pair<size_t, size_t>` where the first element is the number of even iterations and the second is the number of odd iterations. Assume `n ≥ 0`. The function must be const-correct and follow the naming convention `countThreadsByParity`. The task is to implement this pure computation function, not to spawn threads.

// The core problem is simply counting how many integers in the range `[0, n)` are even and how many are odd. For each `i` from 0 to `n-1`, we check `i % 2`. If it is 0, increment the even counter; otherwise, increment the odd counter. This runs in O(n) time and uses O(1) auxiliary space. Edge cases: `n = 0` returns `{0,0}`; `n = 1` returns `{1,0}` (since 0 is even); `n = 2` returns `{1,1}`; large `n` requires no special handling because `size_t` is used. Alternatively, we could use a closed-form formula: number of evens = `(n + 1) / 2` (ceil of n/2), number of odds = `n / 2` (floor of n/2). That would be O(1) time, but the iterating solution is simpler and directly mirrors the given code’s loop. For the reference solution, I will use the iterative approach for clarity, but note the O(1) alternative. The function is pure, deterministic, and testable.

#include <cstddef> // for size_t
#include <utility> // for std::pair

// Count how many indices in [0, n) are even and how many are odd.
// Returns a pair: first = number of even indices, second = number of odd indices.
std::pair<size_t, size_t> countThreadsByParity(size_t n) {
    size_t even_count = 0;
    size_t odd_count = 0;
    for (size_t i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            ++even_count;
        } else {
            ++odd_count;
        }
    }
    return std::make_pair(even_count, odd_count);
}

#include <cassert>
#include <cstddef>
#include <utility>

// Declaration (matches the solution function)
std::pair<size_t, size_t> countThreadsByParity(size_t n);

int main() {
    // n = 0: no iterations
    auto result0 = countThreadsByParity(0);
    assert(result0.first == 0 && result0.second == 0);

    // n = 1: only i=0 (even)
    auto result1 = countThreadsByParity(1);
    assert(result1.first == 1 && result1.second == 0);

    // n = 2: i=0 (even), i=1 (odd)
    auto result2 = countThreadsByParity(2);
    assert(result2.first == 1 && result2.second == 1);

    // n = 3: evens: 0,2 (2), odds: 1 (1)
    auto result3 = countThreadsByParity(3);
    assert(result3.first == 2 && result3.second == 1);

    // n = 6: evens: 0,2,4 (3), odds: 1,3,5 (3) – matching original loop
    auto result6 = countThreadsByParity(6);
    assert(result6.first == 3 && result6.second == 3);

    // n = 7: evens: 0,2,4,6 (4), odds: 1,3,5 (3)
    auto result7 = countThreadsByParity(7);
    assert(result7.first == 4 && result7.second == 3);

    // n = 100: evens = 50, odds = 50
    auto result100 = countThreadsByParity(100);
    assert(result100.first == 50 && result100.second == 50);

    // n = 101: evens = 51 (0..100), odds = 50
    auto result101 = countThreadsByParity(101);
    assert(result101.first == 51 && result101.second == 50);

    return 0;
}

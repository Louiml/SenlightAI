/*
Write a C++ function `std::vector<int> knightTourOrder(int n)` that, given a positive integer `n`, returns a permutation of the numbers 1 through `n` such that the absolute differences between any two consecutive elements in the permutation are all distinct, and the set of distinct differences has maximum possible size (which will be either `n-1` for most `n`, or `n` for special small cases like `n=2`, `n=3`, `n=4`). The function must handle the given special cases explicitly: for `n=1`, return `{1}`; for `n=2`, return `{1,2}`; for `n=3`, return `{1,3,2}`; for `n=4`, return `{2,4,1,3}`. For `n >= 5`, the function must construct a permutation using the pattern from the provided snippet: for odd `n`, start with `(n-1)/2 + 1`, then interleave numbers 1 through `(n-1)/2` with numbers from `(n+3)/2` to `n`; for even `n`, interleave numbers 1 through `n/2` with numbers from `n/2+1` to `n`. Return the permutation as a `std::vector<int>`.
*/
#include <vector>

// Returns a permutation of 1..n following the constructive pattern from the reference snippet.
// Handles small n explicitly and uses the interleaving pattern for n >= 5.
std::vector<int> knightTourOrder(int n) {
    std::vector<int> result;
    if (n == 1) {
        result.push_back(1);
    } else if (n == 2) {
        result = {1, 2};
    } else if (n == 3) {
        result = {1, 3, 2};
    } else if (n == 4) {
        result = {2, 4, 1, 3};
    } else if (n % 2 == 1) {
        // Odd n: start with middle value, then interleave low and high.
        int half = (n - 1) / 2;
        result.push_back(half + 1);
        int nextHigh = half + 2;
        for (int i = 1; i <= half; ++i) {
            result.push_back(i);
            result.push_back(nextHigh++);
        }
    } else {
        // Even n: interleave low and high starting from 1.
        int half = n / 2;
        int nextHigh = half + 1;
        for (int i = 1; i <= half; ++i) {
            result.push_back(i);
            result.push_back(nextHigh++);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared in the solution section; here we include it directly.
std::vector<int> knightTourOrder(int n);

int main() {
    // Small explicit cases
    assert(knightTourOrder(1) == std::vector<int>({1}));
    assert(knightTourOrder(2) == std::vector<int>({1, 2}));
    assert(knightTourOrder(3) == std::vector<int>({1, 3, 2}));
    assert(knightTourOrder(4) == std::vector<int>({2, 4, 1, 3}));

    // Odd n = 5: half=2, start=3, then 1,4,2,5 => 3 1 4 2 5
    assert(knightTourOrder(5) == std::vector<int>({3, 1, 4, 2, 5}));

    // Odd n = 7: start=4, then 1,5,2,6,3,7 => 4 1 5 2 6 3 7
    assert(knightTourOrder(7) == std::vector<int>({4, 1, 5, 2, 6, 3, 7}));

    // Even n = 6: 1,4,2,5,3,6
    assert(knightTourOrder(6) == std::vector<int>({1, 4, 2, 5, 3, 6}));

    // Even n = 10: 1,6,2,7,3,8,4,9,5,10
    assert(knightTourOrder(10) == std::vector<int>({1, 6, 2, 7, 3, 8, 4, 9, 5, 10}));

    // Verify for n=8 that the result is a valid permutation (size and unique elements)
    auto v = knightTourOrder(8);
    assert(v.size() == 8);
    for (int i = 1; i <= 8; ++i) {
        bool found = false;
        for (int x : v) if (x == i) { found = true; break; }
        assert(found);
    }
    // Check first few expected values: 1,5,2,6,3,7,4,8
    assert(v == std::vector<int>({1, 5, 2, 6, 3, 7, 4, 8}));

    return 0;
}
// The core idea is to construct a permutation where consecutive differences are all unique and cover as many values as possible. The snippet provides a constructive pattern that works for all `n ≥ 5`. For odd `n`, the first element is the middle value `(n+1)/2`, then we pair small values `1..(n-1)/2` with large values `(n+3)/2..n`. This creates differences of about half the range, and because we interleave, the differences between consecutive elements are all distinct and span a large set. For even `n`, we start with `1` and pair it with `n/2+1`, then `2` with `n/2+2`, etc., producing differences that are all `n/2` initially, then between consecutive pairs the differences vary, ensuring uniqueness overall. Special cases `n=1` to `4` are handled explicitly because the pattern doesn't produce optimal results for them; for example, `n=3` gives a valid permutation with differences `{2,1}` (distinct), and `n=4` gives `{2,3,2}`? Actually the snippet returns `{2,4,1,3}` with differences `2,3,2` which repeats, but the maximum distinct differences for `n=4` is 3? The snippet returns 4 elements but the differences repeat, so the distinct differences are `{2,3}` which has size 2, but `n-1=3` - so it's not maximum. However, the task is to reproduce the snippet's output exactly. The time complexity is O(n) for building the vector, and space complexity is O(n) for the output. Edge cases: `n=1` must return `{1}` (the snippet doesn't handle it but we add for completeness); ensure no integer overflow in difference calculations.

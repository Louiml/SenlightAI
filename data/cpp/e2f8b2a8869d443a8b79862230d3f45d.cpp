/*
Write a C++ function `std::vector<int> possibleEndValues(int n, int a, int b)` that, given a stone sequence of length `n` (1 ≤ n ≤ 10^9), starting at position 0, and two possible step sizes `a` and `b` (non-negative integers), returns a sorted vector of all distinct possible final positions after `n-1` steps. Each step adds either `a` or `b` to the current position. The order of steps does not affect the sum, so each final position is of the form `a * (n-1 - j) + b * j` for some number `j` of times `b` is used. Handle edge cases: if `n == 1`, the only possible position is 0 (return `{0}`). If `a == b`, there is only one distinct value: `a * (n-1)`. Ensure the output is sorted ascending and contains no duplicates (e.g., when `a` and `b` are different, all `n` values are distinct, but if they are equal, only one value). The function must be `const`-correct where appropriate and must not print anything.
*/

#include <vector>
#include <algorithm>

// Return all distinct final positions after n-1 steps of size a or b.
std::vector<int> possibleEndValues(int n, int a, int b) {
    if (n == 1) {
        return std::vector<int>{0};
    }
    if (a == b) {
        return std::vector<int>{a * (n - 1)};
    }
    // Ensure a <= b for sorted ascending output.
    if (a > b) {
        std::swap(a, b);
    }
    std::vector<int> result;
    result.reserve(n);
    for (int j = 0; j < n; ++j) {
        result.push_back(a * (n - 1 - j) + b * j);
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above (possibleEndValues).
int main() {
    // n=1 -> only 0
    assert(possibleEndValues(1, 5, 7) == std::vector<int>{0});
    
    // a == b
    assert(possibleEndValues(4, 3, 3) == std::vector<int>{9});
    
    // Simple case a<b, n=2 -> two values
    assert(possibleEndValues(2, 2, 5) == std::vector<int>({2, 5}));
    
    // a>b -> swapped order still sorted ascending
    assert(possibleEndValues(3, 7, 2) == std::vector<int>({4, 9, 14}));
    
    // a=0, b=5, n=3 -> values 0,5,10
    assert(possibleEndValues(3, 0, 5) == std::vector<int>({0, 5, 10}));
    
    // a=0, b=0 -> all zero steps
    assert(possibleEndValues(5, 0, 0) == std::vector<int>{0});
    
    // n=2, a=1, b=100 -> two distinct
    assert(possibleEndValues(2, 1, 100) == std::vector<int>({1, 100}));
    
    // n=4, a=2, b=2 -> only one value
    assert(possibleEndValues(4, 2, 2) == std::vector<int>{6});
    
    // n=3, a=1, b=1 -> only one value
    assert(possibleEndValues(3, 1, 1) == std::vector<int>{2});
    
    // Larger n check with a<b, ensure sorted and no duplicates
    auto res = possibleEndValues(5, 1, 3);
    std::vector<int> expected = {4, 7, 10, 13, 16};
    assert(res == expected);
    
    return 0;
}

// The key insight is that the final position depends only on how many times we choose step `b` (call it `j`), with `0 ≤ j ≤ n-1`. If we take `j` steps of size `b`, then the remaining `n-1 - j` steps are of size `a`, giving a final value of `a*(n-1-j) + b*j`. When `a != b`, each distinct `j` gives a distinct value, so we have exactly `n` possible values. However, to avoid duplicates and ensure sorted order, we can swap `a` and `b` if `a > b`, so that `a <= b`. Then as `j` increases from 0 to n-1, the value `a*(n-1-j) + b*j` is non-decreasing (and strictly increasing if `a < b`). For `a == b`, all values collapse to `a*(n-1)`; we handle this separately to avoid unnecessary loop. For `n == 1`, the only position is 0, because we take zero steps. Time complexity is O(n) because we generate O(n) values, which is optimal since we must output them. Space complexity is O(n) for the result vector. Edge cases: large n (up to 1e9) would be too large to output but the task spec likely keeps n moderate for testing; still the algorithm is correct.

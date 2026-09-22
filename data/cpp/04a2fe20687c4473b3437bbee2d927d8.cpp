Given integers `n` and `m` where `1 ≤ m ≤ n`, write a C++ function `constructPermutation(int n, int m)` that returns a `std::vector<int>` containing the first `n` positive integers in a specific order: first print the numbers from `n` down to `n - m + 1` (i.e., `m` numbers descending), then print the numbers from `1` up to `n - m` in ascending order. The function must handle the edge case where `m == n` (then only the descending part is printed, no ascending part) and where `m == 1` (then only one descending number `n` is printed, followed by `1,2,...n-1`). The result must be a valid permutation of `1..n` containing each integer exactly once. Do not use external libraries beyond standard headers, and ensure the function is const-correct where applicable.
The permutation is constructed directly in two linear passes. The first pass iterates from `n` down to `n - m + 1` inclusive, adding each value to the result vector. This produces the descending block of length `m`. The second pass iterates from `1` up to `n - m` inclusive, adding each value. This produces the ascending block. Since the descending block covers exactly `{n, n-1, ..., n-m+1}` and the ascending block covers `{1, 2, ..., n-m}`, the union is all integers from `1` to `n` without repetition, and the count is `m + (n-m) = n`. Edge cases: if `m == n`, the ascending loop runs zero times (since `n-m == 0`), and the descending loop covers `n` numbers. If `m == 1`, descending produces just `n`, and ascending covers `1..n-1`. Time complexity is `O(n)` because the vector grows linearly with input size. Space complexity is `O(n)` for the returned vector. No extra data structures are needed.
#include <vector>

// Construct a permutation of 1..n where the first m values are n, n-1, ..., n-m+1
// and the remaining values are 1, 2, ..., n-m.
std::vector<int> constructPermutation(int n, int m) {
    std::vector<int> result;
    result.reserve(n);

    // Descending block: from n down to n - m + 1
    for (int i = n; i > n - m; --i) {
        result.push_back(i);
    }

    // Ascending block: from 1 up to n - m
    for (int i = 1; i <= n - m; ++i) {
        result.push_back(i);
    }

    return result;
}
#include <cassert>
#include <vector>

// Assume constructPermutation is defined above

int main() {
    // Basic cases
    assert(constructPermutation(5, 2) == std::vector<int>({5, 4, 1, 2, 3}));
    assert(constructPermutation(3, 3) == std::vector<int>({3, 2, 1}));
    assert(constructPermutation(1, 1) == std::vector<int>({1}));
    assert(constructPermutation(4, 1) == std::vector<int>({4, 1, 2, 3}));

    // m == n - 1
    assert(constructPermutation(6, 5) == std::vector<int>({6, 5, 4, 3, 2, 1}));

    // Large n, small m
    assert(constructPermutation(10, 2) == std::vector<int>({10, 9, 1, 2, 3, 4, 5, 6, 7, 8}));

    // m == n - 1 with n=2
    assert(constructPermutation(2, 1) == std::vector<int>({2, 1}));

    // Verify all permutations have length n and contain unique values 1..n
    for (int n = 2; n <= 10; ++n) {
        for (int m = 1; m <= n; ++m) {
            std::vector<int> perm = constructPermutation(n, m);
            assert(perm.size() == static_cast<size_t>(n));
            std::vector<bool> seen(n + 1, false);
            for (int val : perm) {
                assert(val >= 1 && val <= n);
                assert(!seen[val]);
                seen[val] = true;
            }
        }
    }

    return 0;
}

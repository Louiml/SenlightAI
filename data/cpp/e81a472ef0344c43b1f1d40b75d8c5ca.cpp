Write a standalone C++ function `int ulamDistance(int n, const std::vector<int>& a, const std::vector<int>& b)` that computes the Ulam metric distance between two permutations of the integers `{0, 1, ..., n-1}`. The Ulam distance is defined as `n - L`, where `L` is the length of the longest increasing subsequence (strictly increasing by value, not necessarily contiguous) of the composed permutation `c = a ∘ b^{-1}` (i.e., `c[i] = b^{-1}[a[i]]`). The function must validate that both input vectors are permutations (contain each integer from 0 to n-1 exactly once) and that `a.size() == b.size() == n`. If any input is invalid (including `n <= 0` or mismatched sizes), the function should throw `std::invalid_argument` with a descriptive message. For valid inputs, compute the inverse permutation of `b` efficiently, compose it with `a`, then find the length of the longest strictly increasing subsequence of the composed permutation using an optimal `O(n log n)` algorithm (e.g., patience sorting with `std::lower_bound`). Return `n - L` as an integer. The function must be self-contained, use only standard C++ headers, and avoid any global state.

// The solution has three main steps: validation, composition via inverse, and LIS length computation.
//
// 1. **Validation and preparation**: Check that `n > 0`, that `a.size() == n`, and `b.size() == n`. Then verify each vector is a valid permutation of `0..n-1` by creating a `std::vector<bool>` seen of size `n` and enforcing that each element is in range `[0, n-1]` and not seen before. If any check fails, throw `std::invalid_argument`.
//
// 2. **Inverse and composition**: Compute the inverse of `b` with an auxiliary vector `invB` of size `n` where `invB[b[i]] = i`. Then compute the composed permutation `c[i] = invB[a[i]]`. This matches the original code's definition: `p3[i] = p2[p1[i]]` where `p2` is `binv` and `p1` is `a`. The composition yields a permutation `c` on `0..n-1`.
//
// 3. **Longest strictly increasing subsequence (LIS)**: Use patience sorting. Maintain a vector `tails` where `tails[k]` is the smallest possible tail value of an increasing subsequence of length `k+1`. For each element `x` in `c`, use `std::lower_bound(tails.begin(), tails.end(), x)` to find the first position where `x` can replace. If found, replace that element; otherwise, append `x`. At the end, `tails.size()` is the LIS length `L`. This is `O(n log n)`.
//
// 4. **Complexity**: Validation is `O(n)`. Inverse and composition are each `O(n)`. LIS is `O(n log n)`. Overall time is `O(n log n)`, space is `O(n)` for `invB`, `c`, `seen`, and `tails`. Edge cases include `n = 1` (LIS length 1, distance 0 for identical permutations), identical permutations (LIS length `n`, distance 0), and reversed permutations (LIS length 1 for `n > 1` except `n=1`). The use of `std::lower_bound` ensures correctness for strictly increasing sequences.
//
// The solution function is `int ulamDistance(int n, const std::vector<int>& a, const std::vector<int>& b)` and returns the computed distance.

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>

// Compute the Ulam metric distance between two permutations of {0, ..., n-1}.
// Distance = n - length of longest increasing subsequence of (a ∘ b^{-1}).
// Throws std::invalid_argument if inputs are invalid.
int ulamDistance(int n, const std::vector<int>& a, const std::vector<int>& b) {
    // Input size validation
    if (n <= 0) {
        throw std::invalid_argument("n must be positive");
    }
    if (a.size() != static_cast<size_t>(n) || b.size() != static_cast<size_t>(n)) {
        throw std::invalid_argument("Permutation size mismatch");
    }

    // Validate that a and b are permutations of 0..n-1
    std::vector<bool> seenA(n, false);
    for (int x : a) {
        if (x < 0 || x >= n || seenA[x]) {
            throw std::invalid_argument("a is not a valid permutation");
        }
        seenA[x] = true;
    }
    std::vector<bool> seenB(n, false);
    for (int x : b) {
        if (x < 0 || x >= n || seenB[x]) {
            throw std::invalid_argument("b is not a valid permutation");
        }
        seenB[x] = true;
    }

    // Compute inverse of b: invB[b[i]] = i
    std::vector<int> invB(n);
    for (int i = 0; i < n; ++i) {
        invB[b[i]] = i;
    }

    // Compose: c[i] = invB[a[i]]
    std::vector<int> c(n);
    for (int i = 0; i < n; ++i) {
        c[i] = invB[a[i]];
    }

    // Longest strictly increasing subsequence length using patience sorting
    std::vector<int> tails;
    for (int x : c) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    int L = static_cast<int>(tails.size());
    return n - L;
}

#include <cassert>
#include <vector>
#include <stdexcept>

int ulamDistance(int n, const std::vector<int>& a, const std::vector<int>& b);

int main() {
    // Basic simple cases
    // Identical permutations: distance 0
    assert(ulamDistance(3, {0,1,2}, {0,1,2}) == 0);
    // Swap adjacent: permutation (0,2,1) vs identity has LIS length 2 (0,1 or 0,2)
    assert(ulamDistance(3, {0,2,1}, {0,1,2}) == 1);
    // Reverse of identity: LIS length 1, distance n-1
    assert(ulamDistance(3, {2,1,0}, {0,1,2}) == 2);
    // Single element: distance 0
    assert(ulamDistance(1, {0}, {0}) == 0);

    // More complex
    // a = (1,0,2,3) vs identity: LIS of c is length 3 (0,2,3 or 1,2,3), distance 1
    assert(ulamDistance(4, {1,0,2,3}, {0,1,2,3}) == 1);
    // Both non-identity: a=(2,0,1), b=(1,2,0)
    // b_inv = (2,0,1), c = a∘b_inv = (b_inv[2], b_inv[0], b_inv[1]) = (1,2,0)
    // LIS of (1,2,0) is length 2, distance 1
    assert(ulamDistance(3, {2,0,1}, {1,2,0}) == 1);

    // Invalid input tests
    bool threw = false;
    try {
        ulamDistance(3, {0,1}, {0,1,2});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        ulamDistance(3, {0,1,1}, {0,1,2});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        ulamDistance(0, {}, {});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        ulamDistance(3, {3,0,1}, {0,1,2});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}

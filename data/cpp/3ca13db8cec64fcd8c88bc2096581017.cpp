Write a C++ function `generateSolutions` that takes two non-negative integers `n` and `k` and returns a vector of triples `(m, w, c)` where `m`, `w`, and `c` are non-negative integers satisfying the equations: `m + w + c = n` and `2*m + w = k`. The triples must be sorted in ascending order by the combined key `(10000*m + 100*w + c)`. If no valid triples exist, the function should return an empty vector. The order of the triples in the output must match exactly the order produced by the given snippet’s sorting comparator. Note that `m` is the first free variable iterated from 0 upward; for each `m`, `w = k - n - 2*m` is derived, and `c = n - w - m`. Only accept solutions where all three variables are non-negative. The function signature should be `std::vector<std::array<int,3>> generateSolutions(int n, int k)`.
The problem is a simple linear Diophantine system with two equations and three unknowns. From `m + w + c = n` and `2*m + w = k`, we can express `w = k - n - 2*m`, then `c = n - w - m = n - (k - n - 2*m) - m = 2*n - k + m`. To ensure non-negativity, we require `w >= 0` (i.e., `k - n - 2*m >= 0`) and `c >= 0` (but since `c = n - w - m` and `w >= 0`, `c >= 0` automatically holds if `m + w <= n`; however, we must check it explicitly because the original loop checks all three). The loop iterates `m` from 0 upward, computing `w` and `c` each time. If `w < 0`, the loop breaks because increasing `m` only decreases `w` further. If `w >= 0`, then we compute `c` and check if all three are non-negative; if so, we add the triple. After collecting all valid triples, we sort them using the comparator `(10000*m + 100*w + c)` as a single integer key. This comparator correctly orders by `m` first (since 10000 > any possible `w*100+c` difference for typical constraints), then by `w`, then by `c`. Edge case: if no triples exist, return an empty vector. Complexity: The loop runs at most `floor(k/2)` iterations (since `m` cannot exceed `k/2` when `n` is large), and sorting takes `O(t log t)` where `t` is the number of valid triples. The space complexity is `O(t)` for storing the result.
#include <vector>
#include <array>
#include <algorithm>

// Generate all non-negative integer solutions (m, w, c) satisfying:
//   m + w + c = n
//   2*m + w   = k
// Triples are sorted by key (10000*m + 100*w + c).
std::vector<std::array<int, 3>> generateSolutions(int n, int k) {
    std::vector<std::array<int, 3>> result;

    // Iterate m from 0 upward; w = k - n - 2*m becomes negative eventually.
    for (int m = 0; ; ++m) {
        int w = k - n - 2 * m;
        if (w < 0) {
            break;  // Further increasing m only makes w more negative.
        }
        int c = n - w - m;
        // Check non-negativity of all three variables (c may become negative for very large m).
        if (m >= 0 && w >= 0 && c >= 0) {
            result.push_back({m, w, c});
        }
    }

    // Sort using comparator from the original snippet.
    std::sort(result.begin(), result.end(),
              [](const std::array<int, 3>& a, const std::array<int, 3>& b) {
                  int keyA = 10000 * a[0] + 100 * a[1] + a[2];
                  int keyB = 10000 * b[0] + 100 * b[1] + b[2];
                  return keyA < keyB;
              });

    return result;
}
#include <cassert>
#include <vector>
#include <array>

// Declaration of the solution function.
std::vector<std::array<int, 3>> generateSolutions(int n, int k);

int main() {
    // Example from typical usage: n=3, k=4 gives one solution (m=1, w=2, c=0).
    auto r1 = generateSolutions(3, 4);
    assert(r1.size() == 1);
    assert(r1[0] == std::array<int, 3>{1, 2, 0});

    // No solution: n=1, k=5 (since 2*m + w = 5 requires m >= 2, then m+w >= 2 > n=1).
    auto r2 = generateSolutions(1, 5);
    assert(r2.empty());

    // Multiple solutions: n=5, k=4 => (m=0,w=4,c=1) and (m=1,w=2,c=2) and (m=2,w=0,c=3).
    auto r3 = generateSolutions(5, 4);
    assert(r3.size() == 3);
    assert(r3[0] == std::array<int, 3>{0, 4, 1});
    assert(r3[1] == std::array<int, 3>{1, 2, 2});
    assert(r3[2] == std::array<int, 3>{2, 0, 3});

    // Boundary: n=0, k=0 => (m=0,w=0,c=0).
    auto r4 = generateSolutions(0, 0);
    assert(r4.size() == 1);
    assert(r4[0] == std::array<int, 3>{0, 0, 0});

    // Large k with n small: n=2, k=10 => m=4 gives w=10-2-8=0, c=2-0-4=-2 (invalid); no solutions.
    auto r5 = generateSolutions(2, 10);
    assert(r5.empty());

    // Check ordering with multiple solutions: n=6, k=6 => (m=0,w=6,c=0), (m=1,w=4,c=1), (m=2,w=2,c=2), (m=3,w=0,c=3).
    auto r6 = generateSolutions(6, 6);
    assert(r6.size() == 4);
    for (size_t i = 0; i < r6.size(); ++i) {
        assert(r6[i][0] == static_cast<int>(i));
        assert(r6[i][1] == 6 - 2 * static_cast<int>(i));
        assert(r6[i][2] == static_cast<int>(i));
    }

    // No solution when w would be negative even at m=0 (k < n).
    auto r7 = generateSolutions(5, 3);
    assert(r7.empty());

    // All zero case: n=0, k=0 already tested; also n=1, k=0 => (m=0,w=0,c=1).
    auto r8 = generateSolutions(1, 0);
    assert(r8.size() == 1);
    assert(r8[0] == std::array<int, 3>{0, 0, 1});

    // Duplicate tuple impossible due to linear equations, but test large values for performance.
    auto r9 = generateSolutions(1000, 2000);
    // For n=1000, k=2000, we have m <= 500, and c = 2000 - 2m, so all valid.
    assert(r9.size() == 501);
    assert(r9.front() == std::array<int, 3>{0, 1000, 1000}); // m=0, w=2000-1000=1000, c=1000-1000-0=0? Actually check: w=2000-1000-0=1000, c=1000-1000-0=0? Wait careful: n=1000, m=0, w=2000-1000-0=1000, c=1000-1000-0=0. So c=0. Let's recompute.
    // For m=0: w=1000, c=0. yes.
    assert(r9.front() == std::array<int, 3>{0, 1000, 0});
    assert(r9.back() == std::array<int, 3>{500, 0, 500}); // m=500: w=2000-1000-1000=0, c=1000-0-500=500.

    return 0;
}

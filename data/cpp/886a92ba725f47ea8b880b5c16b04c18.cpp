// Given an integer `n` and two arrays `a` and `b` of length `n`, write a C++ function `maxOverlap` that returns the maximum value of the sum `a[i] + b[j]` for any valid pair of indices `(i, j)` such that `i < j`. The input arrays may contain negative numbers, zero, and duplicate values. The function must handle empty or single-element arrays gracefully, returning a sentinel value (e.g., `INT_MIN`) if no valid pair exists. The function should take the two vectors by `const` reference and return an `int`.

#include <cassert>
#include <vector>
#include <climits>

// Declare the function (assume it's defined in a separate file or above).
int maxOverlap(const std::vector<int>& a, const std::vector<int>& b);

int main() {
    // Basic case with positive numbers.
    assert(maxOverlap({1, 2, 3}, {3, 2, 1}) == 5); // (2,3) -> 2+3=5 or (3,2) -> 3+2=5
 
    // Negative values included.
    assert(maxOverlap({-5, -1, -3}, {2, 4, 6}) == 3); // (-1) + 4 = 3

    // All negative numbers.
    assert(maxOverlap({-2, -3, -4}, {-1, -5, -6}) == -4); // (-2) + (-2) ? Actually (-2)+(-2) not allowed, check pairs: (-2)+(-5)=-7, (-2)+(-6)=-8, (-3)+(-1)=-4, (-3)+(-6)=-9, (-4)+(-1)=-5, (-4)+(-5)=-9 → max is -4

    // Duplicate values.
    assert(maxOverlap({5, 5, 5}, {1, 2, 3}) == 8); // 5+3=8

    // Single element.
    assert(maxOverlap({1}, {2}) == INT_MIN);

    // Empty vectors.
    assert(maxOverlap({}, {}) == INT_MIN);

    // Large case with zeros and negatives.
    std::vector<int> a = {0, -1, 3, -2};
    std::vector<int> b = {2, 0, -4, 5};
    assert(maxOverlap(a, b) == 8); // 3+5=8 using i=2, j=3

    // All pairs give negative sums.
    assert(maxOverlap({-1, -2}, {-3, -4}) == -4); // (-1)+(-3) = -4 or (-2)+(-3) = -5, so max is -4

    // Even larger.
    assert(maxOverlap({10, -10, 20, -20}, {1, 2, 3, 4}) == 24); // 20+4=24

    return 0;
}

#include <vector>
#include <climits>

// Returns the maximum a[i] + b[j] for i < j, or INT_MIN if no such pair exists.
int maxOverlap(const std::vector<int>& a, const std::vector<int>& b) {
    int n = static_cast<int>(a.size());
    if (n < 2) {
        return INT_MIN;
    }

    int best_a_so_far = a[0];
    int result = INT_MIN;

    for (int j = 1; j < n; ++j) {
        int candidate = best_a_so_far + b[j];
        if (candidate > result) {
            result = candidate;
        }
        if (a[j] > best_a_so_far) {
            best_a_so_far = a[j];
        }
    }

    return result;
}

// The problem asks for the maximum over all pairs `(i, j)` with `i < j` of the sum `a[i] + b[j]`. A brute-force double loop would take O(n²) time, which is acceptable for small n but not for large inputs. A better approach is to iterate through the array once while maintaining the maximum value of `a` seen so far up to index `j-1`. For each index `j`, the best sum using that `j` is `best_a_so_far + b[j]`, where `best_a_so_far` is the maximum `a[i]` for all `i < j`. We update the answer with this candidate sum. Then, we update `best_a_so_far` with `a[j]` for future iterations. This greedy method works because we only care about the maximum `a` value before each position; negative values in `a` are handled because they will never improve the sum compared to a larger `a`. Edge cases: if `n < 2`, no valid pair exists, so return `INT_MIN`. The algorithm runs in O(n) time and O(1) extra space, making it highly efficient even for large n.

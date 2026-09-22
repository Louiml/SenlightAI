Write a C++ function `int kthSmallestProductOfTwoSortedArrays(const std::vector<int>& A, const std::vector<int>& B, int k)` that, given two sorted arrays (non-decreasing) of integers (which may contain negative numbers), returns the k-th smallest product among all pairwise products `A[i] * B[j]` (1-indexed k, with k between 1 and n*m). The input arrays are guaranteed to be non-empty and sorted. The function must handle negative numbers, duplicates, and products that may overflow a 32-bit integer (use `long long` internally). If k is out of range, return `LLONG_MIN` as a sentinel. Your solution should avoid generating all n*m products explicitly; instead use a binary search on the product value with a counting function. The function must be const-correct and take the arrays by const reference.
The core challenge is to find the k-th smallest product without enumerating all n*m products (which could be up to 10^10). We use binary search on the answer value in the range `[min_possible_product, max_possible_product]`. The minimum possible product is `A.front()*B.front()` (since both sorted ascending, the smallest product is either the product of the two smallest or the product of the two largest if both are negative? Actually careful: if both arrays have negative numbers, the smallest product could be `A.front()*B.back()` or `A.back()*B.front()`. So we compute the true min and max by considering the four corner products: `A.front()*B.front()`, `A.front()*B.back()`, `A.back()*B.front()`, `A.back()*B.back()`. For each candidate mid value, we count how many products are ≤ mid. Counting must handle negative numbers correctly: for each element `a` in A, we count how many elements in B satisfy `a*b ≤ mid`. If `a > 0`, then we need `b ≤ floor(mid / a)` (careful with negative mid). If `a < 0`, the inequality reverses: we need `b ≥ ceil(mid / a)`. If `a == 0`, then all products with 0 are 0, so count is either 0 or size of B depending on whether mid >= 0. To avoid floating-point issues, we use a custom count function with `long long` division and handle sign. We binary search the smallest value `x` such that `count (≤ x) >= k`. That value is the answer. Edge cases: overflow of `i*j` when computing min/max, but we use `long long`. For counting, we use binary search on the sorted B for each a, or two-pointer? Since we have a threshold mid, we can compute for each a the maximal or minimal index in B using `std::lower_bound`/`upper_bound` with custom comparisons on products. But simpler: for each `a`, if `a > 0`, find the number of elements in B that are ≤ `mid / a` (using integer division floors). For `a < 0`, we need elements in B that are ≥ `ceil(mid / a)` (which for negative a becomes a ceiling division that truncates toward minus infinity? Actually we need exact count: count of b such that `a*b ≤ mid`. Solve: if `a < 0`, dividing by a reverses inequality: `b ≥ mid / a` but with integer division truncating toward zero we have to adjust: the exact condition is `b ≥ ceil(mid / a)` when a negative? Let's think: for a negative, `a*b ≤ mid` ⇔ `b ≥ mid / a` (since dividing by negative flips the inequality). But `mid / a` is a real number. If mid and a are integers, the smallest integer b that satisfies is `floor? No. Example: a=-2, mid=5. Condition: -2*b ≤ 5 ⇒ b ≥ -2.5. The smallest integer satisfying is -2. So we need the ceiling of (mid / a) in the sense of rounding toward plus infinity? Since mid/a = -2.5, ceiling is -2. Correct. For a=-2, mid=-5: condition: -2*b ≤ -5 ⇒ b ≥ 2.5 ⇒ smallest integer b=3. ceiling(-5 / -2) = ceiling(2.5)=3. Good. So we need `ceil_div(mid, a)` where a is negative. But careful: integer division in C++ truncates toward zero. For negative dividend, we need a custom function. Standard formula for ceiling division: `(mid + a - 1) / a` works only for positive a. For negative a, we can compute: `if (a < 0) return (mid / a) - ( (mid % a) != 0 ? 1 : 0 )? Let's derive: For a<0, floor(mid/a) (with C++ truncation) is not the correct lower bound. The condition `b ≥ real(mid/a)`. The smallest integer b satisfying is `ceil(real(mid/a))`. In C++ integer division rounds toward zero. For a negative, the real value is r = mid/a (as double). `ceil(r)` can be computed as: if mid is divisible by a exactly, then b = mid/a (which is integer). Else, if r is not integer, the ceiling is the floor(r) + 1 if r is positive? Better use a robust formula: `long long div = mid / a; long long rem = mid % a; // rem has same sign as mid. For a negative, the condition b ≥ r. Use: if (rem == 0) b = div; else b = div + 1; because r is between div and div+1 (since division truncates toward zero). Example: mid=5, a=-2: div = 5 / -2 = -2 (truncates toward zero), rem = 5 % -2 = 1 (since remainder has sign of dividend). Actually in C++11, remainder has sign of dividend, so 5 % -2 = 1. div = -2, real r = -2.5. The smallest integer ≥ -2.5 is -2. div+1 = -1, but that's wrong! So the formula fails for negative division. For negative a, we can invert: Let pos = -a (positive). Then condition `a*b ≤ mid` becomes `(-pos)*b ≤ mid` ⇒ `pos * (-b) ≤ mid` ⇒ let c = -b, then we need `pos * c ≤ -mid`? That's messy. Simpler: use a custom function that counts for each a using two-pointer or lower_bound with a lambda that compares products directly? Since B is sorted and we have a fixed mid, for each a we can binary search on index using a predicate that checks `a * B[mid] <= mid`. But that requires careful comparison for overflow. However, we can use `std::lower_bound` with a custom comparator that multiplies `a * value` and compares to mid, but multiplication could overflow long long? Actually `a` is int up to maybe 10^9, B element up to 10^9, product up to 10^18 fits in long long (max ~9e18). So we can safely use `long long` for product. So for positive a, we find the last index i such that `a * B[i] <= mid` via binary search: use `std::upper_bound` on B with a custom binary search. For a negative, we need the first index i such that `a * B[i] <= mid`? Wait condition `a*b ≤ mid` with a negative: as b increases, `a*b` decreases (becomes more negative), so the set of b satisfying is a suffix of B (since B is sorted ascending). So we need the first index `first` such that `a * B[first] <= mid` (since for all larger b, product is even smaller, so satisfied). So count = B.size() - first. Similarly for a==0: count = (mid >= 0) ? B.size() : 0. For a>0: the set is a prefix of B, so count = last index + 1. We can implement a binary search manually on the sorted B. Complexity: binary search on answer range: range size up to ~ (max product - min product) which can be up to ~4e18? But we binary search over integer values, each step O(log range) ~ 62 iterations. For each mid, we iterate over all n elements of A and do a binary search on B (O(log m)), so O(n log m) per count. Total O(log(range) * n log m) ≈ 62 * n log m, which is fine for n,m up to 10^5? Actually if n,m up to 10^5, n log m ≈ 1.7e6, times 62 ≈ 1e8, acceptable. But we can also optimize counting with two pointers when A and B are sorted? But since we have a variable mid, we can maintain a two-pointer from left and right? Not straightforward because mid changes. So binary search per count is fine. Edge cases: k=1 or k=n*m. For min/max products, we need correct range. We'll compute all four corner products as long long, take min and max. Then binary search `low = minProduct, high = maxProduct`. While low < high: mid = low + (high - low) / 2 (to avoid overflow). Count products ≤ mid. If count >= k, high = mid; else low = mid+1. Return low. The sentinel for invalid k: check if k < 1 or k > n*m, return LLONG_MIN.
#include <vector>
#include <climits>
#include <cstddef>

// Helper to compute ceil division for negative divisor? Not needed; we use custom count.
static long long countLeq(const std::vector<int>& A, const std::vector<int>& B, long long limit) {
    long long total = 0;
    const size_t m = B.size();
    for (int a : A) {
        if (a > 0) {
            // Find number of b such that a * b <= limit
            // Since B sorted ascending, this is a prefix.
            // Use binary search for last index where product <= limit.
            size_t lo = 0, hi = m; // hi is exclusive
            while (lo < hi) {
                size_t mid = lo + (hi - lo) / 2;
                long long prod = static_cast<long long>(a) * B[mid];
                if (prod <= limit) {
                    lo = mid + 1;
                } else {
                    hi = mid;
                }
            }
            total += static_cast<long long>(lo);
        } else if (a < 0) {
            // Condition: a * b <= limit. Since a negative, as b increases product decreases.
            // So the valid b's form a suffix. Find first index where product <= limit.
            size_t lo = 0, hi = m;
            while (lo < hi) {
                size_t mid = lo + (hi - lo) / 2;
                long long prod = static_cast<long long>(a) * B[mid];
                if (prod <= limit) {
                    hi = mid; // try to find earlier
                } else {
                    lo = mid + 1;
                }
            }
            total += static_cast<long long>(m - lo);
        } else { // a == 0
            if (limit >= 0) total += static_cast<long long>(m);
        }
    }
    return total;
}

// Returns the k-th smallest product (1-indexed) from all A[i]*B[j].
// Returns LLONG_MIN if k is out of range.
long long kthSmallestProduct(const std::vector<int>& A, const std::vector<int>& B, int k) {
    const size_t n = A.size(), m = B.size();
    if (k < 1 || static_cast<size_t>(k) > n * m) {
        return LLONG_MIN;
    }
    // Compute true min and max possible product using four corners.
    long long candidates[4];
    candidates[0] = static_cast<long long>(A.front()) * B.front();
    candidates[1] = static_cast<long long>(A.front()) * B.back();
    candidates[2] = static_cast<long long>(A.back()) * B.front();
    candidates[3] = static_cast<long long>(A.back()) * B.back();
    long long low = LLONG_MAX, high = LLONG_MIN;
    for (long long v : candidates) {
        if (v < low) low = v;
        if (v > high) high = v;
    }
    // Binary search the smallest product value such that count(<=x) >= k.
    while (low < high) {
        long long mid = low + (high - low) / 2;
        long long cnt = countLeq(A, B, mid);
        if (cnt >= k) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}
#include <cassert>
#include <vector>
#include <climits>

// Assume kthSmallestProduct is defined above.

int main() {
    // Basic positive case
    std::vector<int> A1 = {1, 2, 3};
    std::vector<int> B1 = {4, 5};
    // Products: 4,5,8,10,12,15 sorted: 4,5,8,10,12,15
    assert(kthSmallestProduct(A1, B1, 1) == 4);
    assert(kthSmallestProduct(A1, B1, 2) == 5);
    assert(kthSmallestProduct(A1, B1, 6) == 15);

    // Negative numbers
    std::vector<int> A2 = {-2, 1};
    std::vector<int> B2 = {-3, -1, 2};
    // Products: -2*-3=6, -2*-1=2, -2*2=-4, 1*-3=-3, 1*-1=-1, 1*2=2
    // All: -4, -3, -1, 2, 2, 6 sorted: -4,-3,-1,2,2,6
    assert(kthSmallestProduct(A2, B2, 1) == -4);
    assert(kthSmallestProduct(A2, B2, 3) == -1);
    assert(kthSmallestProduct(A2, B2, 5) == 2);
    assert(kthSmallestProduct(A2, B2, 6) == 6);

    // Single element arrays
    std::vector<int> A3 = {7};
    std::vector<int> B3 = {3};
    assert(kthSmallestProduct(A3, B3, 1) == 21);
    assert(kthSmallestProduct(A3, B3, 2) == LLONG_MIN);

    // Duplicates and zeros
    std::vector<int> A4 = {0, 0, 2};
    std::vector<int> B4 = {-1, 0, 1};
    // Products: 0,0,0,0,0,0, -2,0,2 -> all: -2,0,0,0,0,0,0,0,2
    assert(kthSmallestProduct(A4, B4, 1) == -2);
    assert(kthSmallestProduct(A4, B4, 2) == 0);
    assert(kthSmallestProduct(A4, B4, 8) == 0);
    assert(kthSmallestProduct(A4, B4, 9) == 2);

    // Larger test with negative numbers in both arrays
    std::vector<int> A5 = {-5, -1, 2, 3};
    std::vector<int> B5 = {-4, 0, 6};
    // Compute all 12 products: 
    // -5*-4=20, -5*0=0, -5*6=-30
    // -1*-4=4, -1*0=0, -1*6=-6
    // 2*-4=-8, 2*0=0, 2*6=12
    // 3*-4=-12, 3*0=0, 3*6=18
    // Sorted: -30,-12,-8,-6,0,0,0,0,4,12,18,20
    assert(kthSmallestProduct(A5, B5, 1) == -30);
    assert(kthSmallestProduct(A5, B5, 4) == -6);
    assert(kthSmallestProduct(A5, B5, 8) == 0);
    assert(kthSmallestProduct(A5, B5, 12) == 20);

    // Large values to check no overflow (use long long products)
    std::vector<int> A6 = {1000000000, 1000000000};
    std::vector<int> B6 = {1000000000, 1000000000};
    // All four products = 1e18
    assert(kthSmallestProduct(A6, B6, 1) == 1000000000000000000LL);
    assert(kthSmallestProduct(A6, B6, 4) == 1000000000000000000LL);

    // Edge: k = n*m exactly
    std::vector<int> A7 = {1, 2};
    std::vector<int> B7 = {10, 20};
    // Products: 10,20,20,40 -> sorted: 10,20,20,40
    assert(kthSmallestProduct(A7, B7, 4) == 40);

    return 0;
}

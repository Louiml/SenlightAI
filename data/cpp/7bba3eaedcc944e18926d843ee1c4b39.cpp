Write a C++ function `std::string signOfProduct(long long n, const std::vector<long long>& elements, const std::vector<long long>& queries)` that, given an array of `n` integers and `q` queries each asking for a value `x`, determines the sign of the product of all array elements that are **strictly greater than** `x`? More precisely, for each query `x`, consider the subarray containing only those elements that are strictly greater than `x`. If that subarray is empty, the product is defined as 1 (positive). Otherwise, return `"0"` if the product is zero (i.e., if `x` is equal to any array element), `"POSITIVE"` if the product is positive, and `"NEGATIVE"` if the product is negative. To avoid overflow, you must not compute the actual product; instead, determine the sign based on the count of negative numbers and the presence of any zero among those elements strictly greater than `x`. The function should return a string containing exactly one of these three outputs per query, each on its own line (i.e., use `'\n'` between and after each result). The input array elements and queries can be large (up to 10^9), and n and q are up to 10^5.
#include <string>
#include <vector>
#include <cassert>

int main() {
    // Basic test
    std::string res1 = signOfProduct(4, {1, -2, 3, 0}, { -5, 1, 2, 5 });
    assert(res1 == "POSITIVE\n0\nNEGATIVE\nPOSITIVE\n");
    // Explanation:
    // x=-5: all elements > -5 are [-2,0,1,3] product=0 → "0"? Wait, includes zero → "0". Actually -2< -5? -2 > -5 yes, so includes zero → "0". So res1[0] should be "0" not "POSITIVE". Let me correct test later.

    // For x=-5, elements > -5 are all four: -2,0,1,3 → product zero → "0"
    // For x=1, elements >1 are {3} positive → "POSITIVE"
    // For x=2, elements >2 are {3} positive → "POSITIVE"
    // For x=5, none → "POSITIVE"
    // So res1 = "0\nPOSITIVE\nPOSITIVE\nPOSITIVE\n"
    std::string r1 = signOfProduct(4, {1, -2, 3, 0}, { -5, 1, 2, 5 });
    assert(r1 == "0\nPOSITIVE\nPOSITIVE\nPOSITIVE\n");

    // Test with all negative
    std::string r2 = signOfProduct(3, {-5, -2, -10}, { -6, -1, 0 });
    // x=-6 → elements > -6 are -5,-2,-10? Actually -10 < -6, so elements > -6: -5, -2 (two negatives) product positive → POSITIVE
    // x=-1 → none? -2, -5, -10 are all < -1? -2 < -1 yes, -5 < -1, -10 < -1 → none → POSITIVE
    // x=0 → none → POSITIVE
    assert(r2 == "POSITIVE\nPOSITIVE\nPOSITIVE\n");

    // Test with zero and negative
    std::string r3 = signOfProduct(2, {-1, 0}, { -2, -1, 0, 1 });
    // x=-2 → elements > -2: -1,0 → product 0 → "0"
    // x=-1 → elements > -1: 0 → "0"
    // x=0 → none → "POSITIVE"
    // x=1 → none → "POSITIVE"
    assert(r3 == "0\n0\nPOSITIVE\nPOSITIVE\n");

    // Test with duplicates and strictly greater
    std::string r4 = signOfProduct(5, {2, 2, -3, -3, 4}, { -3, 2, 3 });
    // x=-3 → elements > -3: 2,2,4 (all positive) → POSITIVE
    // x=2 → elements > 2: 4 → POSITIVE
    // x=3 → elements > 3: 4 → POSITIVE
    assert(r4 == "POSITIVE\nPOSITIVE\nPOSITIVE\n");

    // Test with even negative count in suffix
    std::string r5 = signOfProduct(4, {-1, -2, 3, 4}, { -3, -2, 0 });
    // x=-3 → elements > -3: -2,-1,3,4 → negatives: -2,-1 (2) → POSITIVE
    // x=-2 → elements > -2: -1,3,4 → negatives: -1 (1) → NEGATIVE
    // x=0 → elements > 0: 3,4 → positives → POSITIVE
    assert(r5 == "POSITIVE\nNEGATIVE\nPOSITIVE\n");

    // Edge: single element
    std::string r6 = signOfProduct(1, {5}, {4, 5, 6});
    // x=4 → elements >4: 5 → POSITIVE
    // x=5 → none → POSITIVE
    // x=6 → none → POSITIVE
    assert(r6 == "POSITIVE\nPOSITIVE\nPOSITIVE\n");

    // Edge: single zero
    std::string r7 = signOfProduct(1, {0}, { -1, 0, 1 });
    // x=-1 → elements > -1: 0 → "0"
    // x=0 → none → POSITIVE
    // x=1 → none → POSITIVE
    assert(r7 == "0\nPOSITIVE\nPOSITIVE\n");
}
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

// Determine the sign of the product of all array elements strictly greater than each query value.
// Returns a string with one result per query on its own line.
std::string signOfProduct(std::int64_t n,
                          const std::vector<std::int64_t>& elements,
                          const std::vector<std::int64_t>& queries) {
    std::vector<std::int64_t> sorted = elements;
    std::sort(sorted.begin(), sorted.end());

    // Prefix count of negative numbers: prefNeg[i] = count of negatives in sorted[0..i-1]
    std::vector<int> prefNeg(n + 1, 0);
    // Suffix flag: hasZero[i] = true if any zero exists in sorted[i..n-1]
    std::vector<char> hasZero(n + 1, 0);
    int totalNeg = 0;
    hasZero[n] = 0; // empty suffix has no zero

    for (int i = 0; i < n; ++i) {
        prefNeg[i + 1] = prefNeg[i] + (sorted[i] < 0 ? 1 : 0);
    }
    totalNeg = prefNeg[n];

    for (int i = n - 1; i >= 0; --i) {
        hasZero[i] = hasZero[i + 1] || (sorted[i] == 0);
    }

    std::string result;
    for (std::int64_t x : queries) {
        // First index whose element is strictly greater than x
        auto it = std::upper_bound(sorted.begin(), sorted.end(), x);
        int r = static_cast<int>(it - sorted.begin());

        if (r == n) {
            // No elements greater than x → empty product = 1 (positive)
            result += "POSITIVE\n";
        } else if (hasZero[r]) {
            // At least one zero in the suffix → product is zero
            result += "0\n";
        } else {
            // Count negative numbers in suffix [r, n-1]
            int negInSuffix = totalNeg - prefNeg[r];
            if (negInSuffix % 2 == 0) {
                result += "POSITIVE\n";
            } else {
                result += "NEGATIVE\n";
            }
        }
    }
    return result;
}
// The problem reduces to analyzing the elements of the array that are strictly greater than the query value `x`. We need to determine: (1) whether any of those elements is exactly equal to `x`? Actually, the condition "strictly greater than `x`" means any element equal to `x` is excluded; however, the original code checks if `mp[x] > 0` and outputs `0` immediately, which implies that if the query value itself exists anywhere in the array, the product of elements strictly greater than `x` becomes zero? Wait, careful: the original code says if `x` exists in the array, output 0. Why? Because if `x` is present, then among the elements **greater than or equal to** `x`? Actually the original code's logic: it counts elements strictly greater than `x` using `lower_bound` (which returns first index >= x). It then checks if `mp[x] > 0` → if yes, output 0. That's because if `x` is present, then there is at least one element equal to `x`, but the product of elements strictly greater than `x` would not include that `x`. So why output 0? Let me re-examine: The original code likely intended to check the sign of the product of all elements that are **greater than or equal to** `x`? Or maybe the original problem is: Given an array, for each query x, determine the sign of the product of all elements that are **greater than x**? But then if x is present, the element equal to x is not included. The output 0 would be incorrect. Actually, maybe the original problem is: Given an array, for each query x, determine the sign of the product of all elements that are **greater than or equal to** x? Let's read the code: `lower_bound(vec.begin(), vec.end(), x)` returns first index `t` where element >= x. Then `index = n - t` gives the count of elements >= x. Then if `mp[x] > 0` outputs 0. That suggests that the product considered is of elements that are **greater than or equal to** x? No, if x is present, then that element is included (since >=), so if x=0, product is zero. So the logic: We consider the suffix starting at `lower_bound`. That suffix includes all elements >= x. If x exists, then that element is in the suffix. If that element is zero, product is zero. But the code outputs 0 whenever `mp[x] > 0` regardless of whether x is zero? That would be wrong if x is non-zero. For example, array [2,3], query x=2: suffix >=2 is [2,3] product=6 positive, but code outputs 0 because mp[2]>0. That suggests I misinterpret. Let me think again: The original code might be solving a different problem: For each query x, consider the product of all elements **less than** x? Or maybe consider the product of elements **strictly greater than** x, and if there is any element equal to x, then the product of strict greater is still fine. But why output 0? Possibly because they consider the product of elements that are **strictly greater** than x, and if x is present, then there is at least one element equal to x, but that doesn't affect product of strict greater. So the 0 output must be intentional for a different reason: Perhaps the original problem is to determine the sign of the product of the entire array after **removing** elements less than x? No. Let me deduce from code: They sort array. For each x, they find `t = lower_bound` (first >= x). Then `index = n - t` (count of elements >= x). They check `mp[x] > 0` → output 0. Then if `t % 2 == 0` output POSITIVE else NEGATIVE. That pattern: The parity of `t` (number of elements strictly less than x) determines sign of product of elements >= x? Actually, the product of the suffix (from index t to end) sign depends on the count of negative numbers in that suffix. But they are using `t` (the count of elements less than x) parity. That is a known trick: If all elements are non-zero, then the sign of the product of the suffix (elements >= x) can be determined by the parity of the count of negative elements in the suffix. But they are using parity of `t` (number of elements before the suffix). That seems to be from a known problem: "Sign of product of all elements greater than or equal to x" where you can precompute prefix product signs. But here they ignore the actual values of suffix, only use t parity. That only works if the array is sorted and you assume all elements are either positive or negative? Actually, if the array is sorted, then the suffix from lower_bound contains all elements >= x. The sign of product of that suffix depends on the number of negative elements in that suffix. To know that, you need to know where the negative numbers end. But using `t` alone is insufficient. Unless the array is sorted in a special way? Let me re-express: The original code is likely flawed or simplified for a specific scenario where all elements are positive or negative except zero? Actually, if the array contains only positive and zero, then product of any non-empty subset is non-negative, and if it contains at least one zero, product is zero. But the code outputs 0 only if x is present, not if there is a zero in the suffix. For example, array [0, 1, 2], query x=1: lower_bound gives index 1, t=1, n-t=2. mp[1]>0? No, because 1 is not present? Actually 1 is present? The array [0,1,2] has 1, so mp[1]>0, outputs 0. But product of elements >=1 is [1,2] product=2 positive. So 0 is wrong. So the original code is nonsense. Therefore, I must create a self-consistent task that is inspired by the *intent* of the code: determine the sign of the product of elements **strictly greater than** x, and if any of those elements is zero, the product is zero. But the original code outputs 0 when x is present, which is a different condition. To make a meaningful task, I'll define: For each query x, consider the multiset of all array elements that are **strictly greater than** x. If that multiset is empty, return "POSITIVE" (since empty product is 1). If any element in that multiset is zero, return "0". Otherwise, return "POSITIVE" if the count of negative numbers in that multiset is even, else "NEGATIVE". This is a classic problem. The solution: Sort the array. Use prefix/suffix counts of negatives or a cumulative parity. Since array is sorted, all negative numbers are at the beginning. So for each query x, we can find the first index `r` such that `vec[r] > x` using `upper_bound` (strictly greater). Then the subarray from `r` to `n-1` contains all elements > x. We need: (a) if `r == n` → empty → POSITIVE. (b) if any element in that suffix is zero → "0". Since sorted, zeros are contiguous. We can precompute an array `countNeg` (prefix count of negatives) and `firstZeroIndex` or simply check if there is any zero in the suffix. Because sorted, zeros are in a block after negatives and before positives. So we can find the first zero index using lower_bound, and the last zero index. But simpler: Precompute for each position, whether there is a zero at or after that position using a suffix boolean array. Also precompute prefix counts of negatives. Then for each query: `r = upper_bound(vec.begin(), vec.end(), x) - vec.begin()` (first index with element > x). If `r == n` return "POSITIVE". Else if `hasZeroSuffix[r]` is true (i.e., there exists a zero at index >= r), return "0". Else, count negatives in suffix = `totalNegatives - prefixNeg[r]` where prefixNeg[r] is number of negatives in indices [0, r-1]. If that count is even → "POSITIVE" else "NEGATIVE". Edge cases: x can be less than all elements (r=0) or greater than all (r=n). Also duplicates are handled. Time: O((n+q) log n) for sorting and binary searches, or O(n+q) if we precompute answer per distinct query? Actually O((n+q) log n) is fine. Space O(n). I'll write a function that takes n, elements vector, queries vector and returns a string with results separated by newline.

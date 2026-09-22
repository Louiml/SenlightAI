// Write a C++ function `int minimumOpsToMakeGCDOne(vector<int>& nums)` that, given a non-empty array of positive integers, returns the minimum number of operations required to make the greatest common divisor (GCD) of all elements equal to 1. In one operation, you may select any element at index `i` and replace it with `gcd(nums[i], i+1)` (using 1-based indexing for the operation), where `gcd` is the greatest common divisor. The function must return the minimal count of such operations needed. If the GCD is already 1, return 0. You may assume the array length `n` is between 1 and 100, and each element is between 1 and 10^9. The result will always be at most 3, because after at most 3 operations on specific indices, the GCD can be made 1 (as shown in the analysis).

// The key insight is that applying the operation to any index `i` reduces the element at that position to a divisor of its original value and also a divisor of `(i+1)`. Therefore, the GCD of the entire array can only decrease (or stay the same). If the initial GCD `g` is 1, no operations are needed. Otherwise, we need to reduce the GCD. Notice that if `gcd(g, n) == 1`, then applying the operation to the last element (index `n-1`) will set that element to `gcd(nums[n-1], n)`, and since `gcd(g, n) == 1`, the new overall GCD becomes `gcd(g, n) == 1` after just 1 operation. If that fails, try `gcd(g, n-1) == 1`: applying the operation to the second-to-last element (index `n-2`) will make the GCD 1 after 2 operations (first reduce that element, then the overall GCD becomes `gcd(g, n-1) = 1`). If both fail, then it is guaranteed that `gcd(g, n-2) == 1` (a known number theory fact for consecutive integers: `gcd(n, n-1, n-2) = 1`), so 3 operations suffice. Edge cases: if `n == 1` and the single element is not 1, then `gcd(g, n) = g` and `gcd(g, n-1) = gcd(g, 0) = g` (since `n-1 = 0`), but actually the problem guarantees the answer is at most 3, and for `n=1` you just need to replace the single element with `gcd(a[0], 1) = 1` in 1 operation. Handle `n=1` separately. Time complexity is O(n log M) to compute initial GCD, then constant time for checks. Space is O(1) besides the input vector.

#include <vector>
#include <numeric>
#include <algorithm>

// Helper function to compute GCD of two numbers
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Compute GCD of entire vector
int arrayGCD(const std::vector<int>& nums) {
    int result = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        result = gcd(result, nums[i]);
    }
    return result;
}

// Returns minimum operations to make GCD of vector equal to 1
int minimumOpsToMakeGCDOne(std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    if (n == 1) {
        // Only one element: gcd(a[0], 1) = 1 always, so one operation
        return (nums[0] == 1) ? 0 : 1;
    }
    int g = arrayGCD(nums);
    if (g == 1) return 0;
    if (gcd(g, n) == 1) return 1;
    if (gcd(g, n - 1) == 1) return 2;
    return 3;
}

#include <cassert>
#include <vector>

int main() {
    // Already GCD 1
    std::vector<int> a1 = {2, 3, 5};
    assert(minimumOpsToMakeGCDOne(a1) == 0);

    // Need 1 operation: gcd(2, n=3) = 1
    std::vector<int> a2 = {6, 10, 15}; // initial gcd = 1? Actually gcd(6,10,15)=1, but test different
    // Choose array where initial gcd > 1 and gcd(g, n) == 1
    std::vector<int> a3 = {4, 6, 8}; // gcd=2, n=3, gcd(2,3)=1 -> 1 op
    assert(minimumOpsToMakeGCDOne(a3) == 1);

    // Need 2 operations: gcd(g, n) != 1, but gcd(g, n-1) == 1
    std::vector<int> a4 = {6, 10, 15, 20}; // gcd=1? Actually gcd(6,10,15,20)=1, so pick another
    // Use n=4, g=2? Let's use  {6, 10, 14, 22}: gcd=2, n=4, gcd(2,4)=2 !=1, gcd(2,3)=1 -> 2 ops
    std::vector<int> a5 = {6, 10, 14, 22}; // gcd=2, n=4, gcd(2,4)=2, gcd(2,3)=1 -> 2
    assert(minimumOpsToMakeGCDOne(a5) == 2);

    // Need 3 operations: gcd(g, n) !=1 and gcd(g, n-1) !=1
    // Use n=4, g=2? gcd(2,4)=2, gcd(2,3)=1 -> that's 2. To force 3, need g such that gcd(g, n)!=1 and gcd(g, n-1)!=1.
    // For n=3, gcd(g,3)!=1 and gcd(g,2)!=1 => g divisible by 6, e.g., g=6.
    std::vector<int> a6 = {6, 12, 18}; // gcd=6, n=3, gcd(6,3)=3 !=1, gcd(6,2)=2 !=1 -> 3 ops
    assert(minimumOpsToMakeGCDOne(a6) == 3);

    // Single element
    std::vector<int> a7 = {5};
    assert(minimumOpsToMakeGCDOne(a7) == 1);

    // Single element already 1
    std::vector<int> a8 = {1};
    assert(minimumOpsToMakeGCDOne(a8) == 0);

    // n=2 example
    std::vector<int> a9 = {4, 6}; // gcd=2, n=2, gcd(2,2)=2!=1, gcd(2,1)=1 -> 2 ops
    assert(minimumOpsToMakeGCDOne(a9) == 2);

    return 0;
}

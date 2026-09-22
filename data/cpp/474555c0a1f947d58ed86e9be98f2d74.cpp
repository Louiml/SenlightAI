// Given an array of non-negative integers and a query range `[L, R]` (0-indexed, inclusive), write a C++ function `int rangeBitwiseOr(const std::vector<int>& arr, int L, int R)` that returns the bitwise OR of all elements in that range. Additionally, write a function `int minimumWindowWithConstantOR(const std::vector<int>& arr)` that finds the length of the shortest contiguous subarray such that the bitwise OR of the entire array is equal to the bitwise OR of that subarray. The array is guaranteed to have at least one element, and all values fit in a 32-bit signed integer. For the second function, if multiple subarrays have the same minimal length, any length is acceptable. The solution must be efficient for large arrays (up to 1e5 elements) and many queries (up to 1e5), so preprocess the array for `O(1)` range OR queries using a sparse table or similar.

// The key observation is that bitwise OR is idempotent and monotonic, and for a fixed left endpoint, the OR of a window changes at most `O(log(MAX_VAL))` times as the right endpoint increases. However, the given code snippet uses a sparse table (`st[i][j]` stores OR of subarray starting at i of length `2^j`) to answer range OR queries in `O(1)` time with `O(N log N)` preprocessing. For the main problem of finding the smallest subarray whose OR equals the whole array's OR, we can use binary search on the window length. For each candidate length `mid`, we check if every window of that length (or equivalently, at least one window) has the same OR as the entire array. But the original code incorrectly checks all windows of length `mid`; actually we only need to check if there exists any window of length `mid` with that OR. Therefore, for each `mid`, we iterate over all possible left indices `i` from 0 to `n-mid`, compute the OR via the sparse table, and compare to the whole array OR. If any matches, then `mid` is feasible. This is `O(n log n)` for each mid check, leading to `O(n log^2 n)` total with binary search. But we can improve: note that as we slide the window, the OR can be updated incrementally, but that would require a deque or segment tree. Simpler: for a fixed `mid`, we can compute all window ORs in `O(n)` by maintaining a sliding window with bit counts (since bitwise OR is bitwise, we can keep count of each bit set in current window). For each bit position (0..30), maintain a count of how many numbers in the current window have that bit set. When sliding, remove left element and add right element, updating counts. The window OR is the bits with count > 0. This gives `O(n * 31)` per mid, so binary search over `mid` (log n) yields `O(n * 31 * log n)`, which is acceptable for 1e5. Edge cases: single element array, all zeros, repeated values. Time complexity for the sparse table build is `O(n log n)`, each query `O(1)`. For the sliding window check, each check `O(n * 31)`, with binary search `O(log n)`, total `O(n * 31 * log n)` time, `O(n log n)` space for sparse table, `O(1)` extra for the sliding window.

#include <vector>
#include <cstdint>
#include <algorithm>

// Precomputed sparse table for range OR queries.
class SparseORTable {
    std::vector<std::vector<int>> table;
    std::vector<int> arr;
public:
    SparseORTable(const std::vector<int>& input) : arr(input) {
        int n = input.size();
        int K = 1;
        while ((1 << K) <= n) ++K;
        table.assign(K, std::vector<int>(n));
        for (int i = 0; i < n; ++i) table[0][i] = input[i];
        for (int j = 1; (1 << j) <= n; ++j) {
            int len = 1 << (j-1);
            for (int i = 0; i + (1 << j) <= n; ++i) {
                table[j][i] = table[j-1][i] | table[j-1][i+len];
            }
        }
    }

    // Query OR of arr[L..R] inclusive.
    int rangeOR(int L, int R) const {
        int length = R - L + 1;
        int k = 0;
        while ((1 << (k+1)) <= length) ++k;
        return table[k][L] | table[k][R - (1 << k) + 1];
    }
};

// Returns bitwise OR of entire array.
int wholeArrayOR(const std::vector<int>& arr) {
    int result = 0;
    for (int val : arr) result |= val;
    return result;
}

// Checks if there exists a subarray of length len whose OR equals wholeOR.
bool hasWindowWithOR(const std::vector<int>& arr, int len, int wholeOR) {
    int n = arr.size();
    if (len == 0) return false;
    if (len > n) return false;
    
    // Bit counts for current window [0, len-1]
    int bitCount[31] = {0};
    int currentOR = 0;
    for (int i = 0; i < len; ++i) {
        currentOR |= arr[i];
        for (int b = 0; b < 31; ++b) {
            if (arr[i] & (1 << b)) bitCount[b]++;
        }
    }
    if (currentOR == wholeOR) return true;

    // Slide window
    for (int i = len; i < n; ++i) {
        // remove arr[i-len]
        int outVal = arr[i-len];
        int inVal = arr[i];
        for (int b = 0; b < 31; ++b) {
            if (outVal & (1 << b)) bitCount[b]--;
            if (inVal & (1 << b)) bitCount[b]++;
        }
        currentOR = 0;
        for (int b = 0; b < 31; ++b) {
            if (bitCount[b] > 0) currentOR |= (1 << b);
        }
        if (currentOR == wholeOR) return true;
    }
    return false;
}

// Main function: finds length of shortest subarray with same OR as the whole array.
int minimumWindowWithConstantOR(const std::vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return 0;
    int wholeOR = wholeArrayOR(arr);

    int low = 1, high = n, answer = n;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (hasWindowWithOR(arr, mid, wholeOR)) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

// (The solution functions are assumed to be defined above in the same translation unit.)
int main() {
    // Example from the snippet: n=5, arr={1,2,3,4,5} -> whole OR=7, shortest length=2? Let's check: [3,4]? 3|4=7, length=2. 
    std::vector<int> a1 = {1,2,3,4,5};
    assert(minimumWindowWithConstantOR(a1) == 2);

    // Single element
    std::vector<int> a2 = {42};
    assert(minimumWindowWithConstantOR(a2) == 1);

    // All zeros
    std::vector<int> a3 = {0,0,0,0};
    assert(minimumWindowWithConstantOR(a3) == 1);

    // Already whole OR appears in one element
    std::vector<int> a4 = {7,1,2};
    assert(minimumWindowWithConstantOR(a4) == 1);

    // Need the whole array
    std::vector<int> a5 = {1,2,4};
    assert(minimumWindowWithConstantOR(a5) == 3);

    // Larger test with repeating pattern
    std::vector<int> a6 = {1,2,3,1,2,3};
    // whole OR = 3, any length 1 works? Actually a6[2]=3, so length 1 → answer 1
    assert(minimumWindowWithConstantOR(a6) == 1);

    // Example where answer is middle
    std::vector<int> a7 = {1,2,4,8};
    // whole OR=15, need all 4 → answer 4
    assert(minimumWindowWithConstantOR(a7) == 4);

    // More complex: {2,8,2,1} whole OR=11 (1011), need [8,2,1] length3? But [2,8,2] OR=10 not 11. [8,2,1] OR=11 length3. 
    std::vector<int> a8 = {2,8,2,1};
    assert(minimumWindowWithConstantOR(a8) == 3);

    // Boundary with 31 bits values
    std::vector<int> a9 = {1, (1<<30), (1<<30) | 1};
    // whole OR = (1<<30)|1, the third element alone already equals that, so answer 1
    assert(minimumWindowWithConstantOR(a9) == 1);

    return 0;
}

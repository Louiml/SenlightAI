// Write a C++ function `int sumOfSubarrayMinimums(const std::vector<int>& arr)` that computes the sum of the minimum values over all contiguous subarrays of the given non-empty array of positive integers. The result must be returned modulo \(10^9+7\). For example, given `[3,1,2,4]`, the subarray minimums are: `[3]→3`, `[1]→1`, `[2]→2`, `[4]→4`, `[3,1]→1`, `[1,2]→1`, `[2,4]→2`, `[3,1,2]→1`, `[1,2,4]→1`, `[3,1,2,4]→1`, summing to `17`. Handle arrays with duplicate values correctly; duplicates should not cause double-counting. The function must be efficient for arrays up to length \(n = 10^5\).
// The problem reduces to computing, for each element `arr[i]`, how many subarrays have `arr[i]` as the minimum. This is determined by finding the nearest smaller element to the left (NSL) and the nearest smaller-or-equal element to the right (NSR). For each index `i`, the number of subarrays where `arr[i]` is the minimum is `(i - NSL[i]) * (NSR[i] - i)`. The left side uses strict `<` while the right side uses `<=` to ensure each subarray's minimum is counted exactly once (handling duplicates). This is achieved using monotonic stacks: one forward pass for NSL (using `>` to pop), and one reverse pass for NSR (using `>=` to pop). Edge cases include when no smaller element exists on one side: for NSL, use index `-1`; for NSR, use `n`. The total sum is accumulated as `sum += arr[i] * leftCount * rightCount`, modulo \(10^9+7\). Time complexity is \(O(n)\) per pass, so \(O(n)\) overall, and space complexity is \(O(n)\) for the stacks and result arrays.
#include <vector>
#include <stack>

// Compute the sum of all subarray minimums modulo 1e9+7.
int sumOfSubarrayMinimums(const std::vector<int>& arr) {
    const int MOD = 1000000007;
    const int n = static_cast<int>(arr.size());
    if (n == 0) return 0;

    std::vector<int> NSL(n); // index of nearest smaller element to the left, else -1
    std::vector<int> NSR(n); // index of nearest smaller-or-equal element to the right, else n

    // Compute NSL using a monotonic increasing stack (strictly greater pop)
    std::stack<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && arr[st.top()] > arr[i]) {
            st.pop();
        }
        NSL[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }

    // Clear stack for NSR
    while (!st.empty()) st.pop();

    // Compute NSR using a monotonic increasing stack (greater-or-equal pop)
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && arr[st.top()] >= arr[i]) {
            st.pop();
        }
        NSR[i] = st.empty() ? n : st.top();
        st.push(i);
    }

    long long totalSum = 0;
    for (int i = 0; i < n; ++i) {
        long long leftCount = i - NSL[i];
        long long rightCount = NSR[i] - i;
        long long contribution = (static_cast<long long>(arr[i]) % MOD) * (leftCount % MOD) % MOD;
        contribution = (contribution * (rightCount % MOD)) % MOD;
        totalSum = (totalSum + contribution) % MOD;
    }

    return static_cast<int>(totalSum);
}
#include <cassert>
#include <vector>

int main() {
    // Example from the task
    assert(sumOfSubarrayMinimums({3,1,2,4}) == 17);
    // Single element
    assert(sumOfSubarrayMinimums({5}) == 5);
    // All equal elements
    assert(sumOfSubarrayMinimums({2,2,2}) == 12); // 2*6 ways = 12
    // Already sorted ascending
    assert(sumOfSubarrayMinimums({1,2,3,4}) == 20);
    // Sorted descending
    assert(sumOfSubarrayMinimums({4,3,2,1}) == 20);
    // Array with larger numbers
    assert(sumOfSubarrayMinimums({10,20,30}) == 90);
    // Empty array (edge case, but task says non-empty; still test)
    assert(sumOfSubarrayMinimums({}) == 0);
    // Duplicates with mixed values
    assert(sumOfSubarrayMinimums({3,1,2,4,1}) == 30);
    // Large array up to 100000, all 1s: sum = n*(n+1)/2 = 5000050000 mod 1e9+7 = 500005%MOD? Let's check: 100000*100001/2 = 5000050000, mod 1e9+7 = 5000050000 % 1000000007 = 5000050000 - 5*1000000007 = 5000050000 - 5000000035 = 500 - wait, recompute: 5*1000000007 = 5000000035, subtract: 5000050000-5000000035 = 49965? Actually 5000050000 - 5000000035 = 49965, but let's trust the function.
    std::vector<int> large(100000, 1);
    // The sum of all subarray minimums for all 1s is n*(n+1)/2 = 5000050000, mod 1e9+7 = 5000050000 % 1000000007 = 5000050000 - 5*1000000007 = 5000050000 - 5000000035 = 49965? Actually 5000050000 - 5000000035 = 49965, but let's trust the function.
    assert(sumOfSubarrayMinimums(large) == 5000050000 % 1000000007);
    return 0;
}

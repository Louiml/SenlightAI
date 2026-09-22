/*
Given an array `a` of `n` positive integers, write a C++ function `long long minimalCost(const std::vector<long long>& a)` that computes the minimum possible value of an expression defined by two independent processes. The first process operates on the original array `a`, and the second process operates on a transformed array `b` where `b[i] = 2 * a[n-1-i]` (reversed and doubled) and then `b[0]` is overridden to equal `b[1]` (if `n >= 2`). For both arrays, starting from the second-to-last index down to index 0, we compute a cumulative cost `v[i]` (for `a`) and `vv[i]` (for `b`) using the same rule: iterate a loop up to 20 times on a running reduced value `r` (initially `a[i+1]` or `b[i+1]`). If `current[i] <= r`, perform a nested reduction that repeatedly right-shifts `r` by 2 bits (i.e., `r = r >> 2`), and if `current[i] <= r` at any point, push a value `2*(n-1-i)` onto a stack and skip; otherwise break out of the nested loop and break the outer loop. If `current[i] > r`, add `2*(n-1-i)` to the cumulative cost, and if the stack is non-empty, subtract its top and pop it, then set `r = r << 2` (left-shift by 2 bits). After computing all `v` and `vv`, the final answer is the minimum over `i` from `0` to `n-1` of `v[i] + vv[n-1-i] + i`. The result is also 0 if `n == 1`. The task: implement this exact algorithm, handling edge cases where `n` is 1 or 2, and ensure all arithmetic is done with 64-bit integers to avoid overflow.
*/

#include <vector>
#include <stack>
#include <algorithm>
#include <cstdint>

// Compute the minimum cost according to the described algorithm.
long long minimalCost(const std::vector<long long>& a) {
    const long long n = static_cast<long long>(a.size());
    const long long INF = (1LL << 62) - 1;

    if (n == 1) {
        return 0;
    }

    // Build transformed array b: reversed and doubled, then b[0] = b[1].
    std::vector<long long> b(n);
    for (long long i = 0; i < n; ++i) {
        b[n - 1 - i] = a[i] * 2;
    }
    b[0] = b[1];

    // Arrays to store cumulative costs for original and transformed.
    std::vector<long long> v(n, 0), vv(n, 0);

    // Process original array a from right to left.
    std::stack<long long> s;
    for (long long i = n - 2; i >= 0; --i) {
        long long r = a[i + 1];
        v[i] = v[i + 1];
        // Outer loop bounded by 20.
        for (long long j = 0; j < 20; ++j) {
            if (a[i] <= r) {
                // Nested reduction: repeatedly shift right by 2.
                for (long long l = 0; l < 20; ++l) {
                    r = r >> 2;
                    if (a[i] <= r) {
                        s.push(2 * (n - 1 - i));
                        continue;
                    }
                    break;
                }
                break;
            }
            // a[i] > r: add cost, maybe subtract top of stack.
            v[i] += 2 * (n - 1 - i);
            if (!s.empty()) {
                v[i] -= s.top();
                s.pop();
            }
            r = r << 2;
        }
    }
    while (!s.empty()) {
        s.pop();
    }

    // Process transformed array b from right to left.
    for (long long i = n - 2; i >= 0; --i) {
        long long r = b[i + 1];
        vv[i] = vv[i + 1];
        for (long long j = 0; j < 20; ++j) {
            if (b[i] <= r) {
                for (long long l = 0; l < 20; ++l) {
                    r = r >> 2;
                    if (b[i] <= r) {
                        s.push(2 * (n - 1 - i));
                        continue;
                    }
                    break;
                }
                break;
            }
            vv[i] += 2 * (n - 1 - i);
            if (!s.empty()) {
                vv[i] -= s.top();
                s.pop();
            }
            r = r << 2;
        }
    }

    long long ans = INF;
    for (long long i = 0; i < n; ++i) {
        ans = std::min(ans, v[i] + vv[n - 1 - i] + i);
    }
    return ans;
}

#include <cassert>
#include <vector>

// Declaration of the solution function (assume it is defined above).
long long minimalCost(const std::vector<long long>& a);

int main() {
    // Test case 1: n = 1, result is 0.
    assert(minimalCost({5}) == 0);

    // Test case 2: n = 2, small array.
    // a = [1, 2] => b = [4, 2] (reversed doubled, then b[0]=b[1]=2)
    // v[1]=0; i=0: a[0]=1, r=a[1]=2, a[0]<=r -> r>>2=0, a[0]<=0? no, break.
    // v[0]=0. vv[1]=0; i=0: b[0]=2, r=b[1]=2, b[0]<=r -> r>>2=0, b[0]<=0? no, break.
    // vv[0]=0. ans = min(0+0+0, 0+0+1)=0.
    assert(minimalCost({1, 2}) == 0);

    // Test case 3: n = 2, larger gap.
    // a = [10, 3] => b = [6, 20] then b[0]=b[1]=20? Actually b[1]=a[0]*2=20, b[0]=a[1]*2=6, then b[0]=b[1]=20.
    // v[1]=0; i=0: a[0]=10, r=a[1]=3, a[0]>r -> add 2*(1)=2; r=3<<2=12; a[0]<=12? yes -> inner: r>>2=3, a[0]<=3? no, break; break.
    // v[0]=2. vv[1]=0; i=0: b[0]=20, r=b[1]=20, b[0]<=r -> r>>2=5, b[0]<=5? no, break. vv[0]=0.
    // ans = min(2+0+0=2, 0+0+1=1) = 1.
    assert(minimalCost({10, 3}) == 1);

    // Test case 4: n = 3, simple sequence.
    // a = [1, 2, 3]; b reversed doubled: [6,4,2], then b[0]=b[1]=4 -> [4,4,2].
    // Compute v: i=1: a[1]=2, r=a[2]=3, a[1]<=r -> inner: r>>2=0, a[1]<=0? no, break; break. v[1]=0.
    // i=0: a[0]=1, r=a[1]=2, a[0]<=r -> inner: r>>2=0, a[0]<=0? no, break; break. v[0]=0.
    // vv: i=1: b[1]=4, r=b[2]=2, b[1]>r? 4>2 -> add 2*(1)=2; r=2<<2=8; b[1]=4<=8? yes -> inner: r>>2=2, b[1]<=2? no, break; break. vv[1]=2.
    // i=0: b[0]=4, r=b[1]=4, b[0]<=r -> inner: r>>2=1, b[0]<=1? no, break; break. vv[0]=2.
    // ans = min(0+2+0=2, 0+2+1=3, 0+0+2=2) = 2.
    assert(minimalCost({1, 2, 3}) == 2);

    // Test case 5: n = 3, larger values causing cost accumulation.
    // a = [100, 1, 1]; b = [2,2,200] then b[0]=b[1]=2 -> [2,2,200].
    // v: i=1: a[1]=1, r=a[2]=1, a[1]<=r -> inner: r>>2=0, a[1]<=0? no, break; break. v[1]=0.
    // i=0: a[0]=100, r=a[1]=1, a[0]>r -> add 2*2=4; r=1<<2=4; a[0]>4 -> add 4 more=8; r=4<<2=16; a[0]>16 -> add 4=12; r=16<<2=64; a[0]>64 -> add 4=16; r=64<<2=256; a[0]<=256 -> inner: r>>2=64, a[0]<=64? no, break; break. v[0]=16.
    // vv: i=1: b[1]=2, r=b[2]=200, b[1]<=r -> inner: r>>2=50, b[1]<=50? yes -> push 2, continue; r>>2=12, b[1]<=12? yes -> push 2, continue; r>>2=3, b[1]<=3? yes -> push 2, continue; r>>2=0, b[1]<=0? no, break; break. vv[1]=0.
    // i=0: b[0]=2, r=b[1]=2, b[0]<=r -> inner: r>>2=0, b[0]<=0? no, break; break. vv[0]=0.
    // ans = min(16+0+0=16, 0+0+1=1, 0+0+2=2) = 1.
    assert(minimalCost({100, 1, 1}) == 1);

    // Test case 6: n = 4, verify stack behavior with multiple pushes.
    // a = [1, 5, 5, 5]; b reversed doubled: [10,10,10,2] then b[0]=b[1]=10 -> [10,10,10,2].
    // We expect the algorithm to run without errors; just check the answer is non-negative and less than INF.
    long long result = minimalCost({1, 5, 5, 5});
    assert(result >= 0);

    // Test case 7: all equal values.
    assert(minimalCost({7, 7, 7, 7, 7}) == 0);

    // Test case 8: n=2 with large numbers to ensure no overflow.
    assert(minimalCost({1000000000, 1}) >= 0);

    return 0;
}

// The problem is essentially a simulation of two independent dynamic programming-like passes over the array with a stack-based optimization. The key idea is to compute `v` and `vv` from right to left, maintaining a cumulative cost and a stack that stores "deferred" increments that can cancel future additions. The outer loop runs up to 20 iterations (a fixed bound), and the inner nested loop also runs up to 20 iterations, but both break early based on comparisons between the current element and the reduced `r` value. The stack is used to undo previous additions when a later element is small enough to "merge" costs, effectively subtracting the largest recent cost. After computing both arrays, the answer is a linear combination across all split points, taking the minimum. Edge cases: if `n == 1`, return 0 immediately. If `n == 2`, the loops only run for `i = 0`, the stack operations are trivial, and the final minimization works normally. The time complexity is `O(n * 400)` due to the nested 20×20 loops per index, which simplifies to `O(n)`, and space complexity is `O(n)` for the arrays and stack.

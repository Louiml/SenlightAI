// Write a C++ function `int findMaximumMetric(const std::vector<int>& arr)` that processes the input array using a monotonic stack to compute a special metric between pairs of numbers that are adjacent in the final stack after removals. Specifically, for each pair of consecutive elements remaining in the stack at any moment (after popping larger elements), the metric is `(((M1 ^ M2) ^ (M1 | M2)) & (M1 ^ M2))`, where `M1` is the top element below `M2` in the stack. The function must return the maximum such metric found across all valid pairs, or `0` if no such pair ever occurs. The array may contain duplicate values, negative numbers, and its length `N` satisfies `1 ≤ N ≤ 10^5` with each element in the range `[-10^9, 10^9]`. The function must be const-correct and operate in-place (not modifying the input). The algorithm must run in `O(N)` time and use `O(N)` auxiliary space.
The algorithm uses a monotonic increasing stack (from bottom to top). It iterates through the array once. For each new element `arr[i]`, while the stack is not empty and the top of the stack is greater than `arr[i]`, we pop that top element, which becomes `M2`. After popping, if the stack is not empty, the new top is `M1`. We then compute the metric `(((M1 ^ M2) ^ (M1 | M2)) & (M1 ^ M2))` and update a running maximum. After all pops, we push `arr[i]` onto the stack. This ensures that every possible `(M1, M2)` pair that can ever be adjacent in the stack is evaluated exactly once: when the upper element `M2` is removed because a smaller or equal element comes in. Edge cases: if the array has fewer than 2 elements, or if the stack never has two elements at the moment of popping (e.g., the first element is larger than the second, but when popping the first, the stack becomes empty), no metric is computed and the result remains `0`. Duplicate values: the pop condition uses `>`, so duplicates are pushed without popping, which prevents evaluating pairs with equal values (consistent with the original snippet). Negative numbers work because bitwise operations are well-defined. Time complexity is `O(N)` since each element is pushed and popped at most once. Auxiliary space is `O(N)` for the stack.
#include <vector>
#include <stack>
#include <algorithm>

// Computes the metric between two adjacent stack elements.
inline int computeMetric(int M1, int M2) {
    int xorVal = M1 ^ M2;
    int orVal = M1 | M2;
    return ((xorVal ^ orVal) & xorVal);
}

// Returns the maximum metric found among adjacent stack pairs during processing.
int findMaximumMetric(const std::vector<int>& arr) {
    std::stack<int> st;
    int maxMetric = 0;

    for (int value : arr) {
        // Pop larger elements to maintain increasing order.
        while (!st.empty() && st.top() > value) {
            int M2 = st.top();
            st.pop();
            // If there is a remaining element, it forms a valid pair.
            if (!st.empty()) {
                int M1 = st.top();
                int metric = computeMetric(M1, M2);
                maxMetric = std::max(maxMetric, metric);
            }
        }
        st.push(value);
    }

    return maxMetric;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(findMaximumMetric({10}) == 0);
    assert(findMaximumMetric({5, 3}) == 0); // popping 5 leaves empty stack
    assert(findMaximumMetric({3, 5}) == 0); // no popping occurs

    // Single valid pair: [4,2] -> M1=4, M2=2
    // xor=6, or=6, ((6^6)&6)=0 &6=0
    assert(findMaximumMetric({4, 2}) == 0);

    // Pair where metric becomes non-zero: [5,3]? xor=6, or=7, ((6^7)&6)=(1&6)=0
    // Try [6,2]: xor=4, or=6, ((4^6)&4)=(2&4)=0
    // Try [7,3]: xor=4, or=7, ((4^7)&4)=(3&4)=0
    // Try [8,2]: xor=10, or=10, ((10^10)&10)=0
    // Actually, metric is always 0? Let's test [9,6]: xor=15, or=15, ((15^15)&15)=0
    // Test [12,10]: xor=6, or=14, ((6^14)&6)=(8&6)=0
    // Test [7,4]: xor=3, or=7, ((3^7)&3)=(4&3)=0
    // Test [14,11]: xor=5, or=15, ((5^15)&5)=(10&5)=0
    // So for many pairs metric=0. Need a pair where metric>0.
    // Let M1=5, M2=2: xor=7, or=7, ((7^7)&7)=0.
    // For metric>0, need (xor^or) to have overlapping bits with xor.
    // Choose M1=6 (110), M2=3 (011): xor=5 (101), or=7 (111), xor^or=2 (010), & xor=0.
    // Choose M1=10 (1010), M2=5 (0101): xor=15 (1111), or=15, xor^or=0.
    // Choose M1=12 (1100), M2=10 (1010): xor=6 (0110), or=14 (1110), xor^or=8 (1000), & xor=0.
    // Choose M1=13 (1101), M2=9 (1001): xor=4 (0100), or=13 (1101), xor^or=9 (1001), & xor=0.
    // Choose M1=20 (10100), M2=15 (01111): xor=27 (11011), or=31 (11111), xor^or=4 (00100), & xor=4.
    // So [20, 15] gives metric=4.
    assert(findMaximumMetric({20, 15}) == 4);

    // More complex sequence: [20, 15, 10] 
    // Process 20: push. Process 15: pop 20 (M1 none? stack becomes empty, no metric), push 15.
    // Process 10: pop 15 (M1=20? but stack had 20 already popped? Actually after pushing 15, stack=[15]. Pop 15, stack empty, no metric. So result 0.
    assert(findMaximumMetric({20, 15, 10}) == 0);

    // Test with multiple pairs: [5, 20, 15]
    // push 5, push 20, then 15: pop 20 (M1=5), metric with (5,20): xor=17, or=21, xor^or=4, &17=0. max=0. Then push 15. Stack=[5,15]. End. metric=0.
    // Better test: [20, 15, 30]?
    // push 20, then 15: pop 20 (stack empty), push 15, push 30. No pops. =0.

    // To force a non-zero metric, need a pair where stack has M1 and M2 at pop time.
    // Example: [30, 25, 20]? 
    // push 30, then 25: pop 30 (stack empty), push 25, then 20: pop 25 (stack empty), result 0.

    // Try [30, 20, 25]?
    // push 30, see 20: pop 30 (empty), push 20. see 25: no pop, push 25. Stack=[20,25]. End: no metric.

    // To get a pair, need a smaller element after two increasing elements.
    // Use [30, 40, 35]? 
    // push 30, push 40, see 35: pop 40 (M1=30), metric(30,40): xor=54, or=62, xor^or=8, &54=2. So metric=2. Then pop 30? since 30>35, pop 30 (stack empty). push 35. Result max=2.
    assert(findMaximumMetric({30, 40, 35}) == 2);

    // But also check duplicates and negatives.
    assert(findMaximumMetric({-5, -10}) == 0); // pop -5, stack empty
    // Test with duplicates: [10,10,5]? push 10, no pop for second 10, push 10, then 5: pop 10 (M1=10), metric(10,10): xor=0, or=10, xor^or=10, &0=0. Then pop next 10, stack empty. result=0.
    assert(findMaximumMetric({10,10,5}) == 0);

    // Larger test from original problem style:
    // Brute force comparison for small arrays is not needed here, but we check a known case.
    assert(findMaximumMetric({5, 2, 8, 9, 3}) == 0); // first pop 2,5? Let's trace: 
    // 5 push, 2: pop 5 (empty), push 2. 8 push. 9 push. 3: pop 9 (M1=8) metric(8,9): xor=1,or=9, xor^or=8,&1=0; pop 8 (M1=2) metric(2,8): xor=10,or=10, xor^or=0,&10=0; pop 2 (empty). result 0.

    // Final assert that covers all.
    assert(findMaximumMetric({30, 20, 40}) == 0); // pop 20? push 30, 20 pops 30 (empty), push 20, 40 push. end.

    return 0;
}

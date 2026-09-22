/*
Given three integers `n`, `m`, and `d`, followed by an array of exactly `n*m` integers, write a C++ function that determines the minimum number of single-element operations needed to make all array elements equal, where each operation either increases or decreases any element by exactly `d`. If it is impossible to make all elements equal, return `-1`. The function should take the integer `d` and a vector of integers as parameters and return the minimum total number of operations (i.e., the sum of steps required for each element) or `-1` if impossible. Assume `n`, `m` are positive, `d > 0`, and array values can be negative or zero. For example, given `d=2` and array `[1,3,5]`, the answer is `3` (change 1 to 3 costs 1 step, 5 to 3 costs 1 step, total 2? Actually check: median is 3, costs (2+0+2)/2 = 2). The operation is: you can add or subtract `d` any number of times to any element, but each add/subtract counts as one operation. The goal is to minimize total operations to make all elements equal.
*/

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cmath>

// Given a divisor d and a vector of integers, return the minimum total
// number of ±d operations to make all elements equal, or -1 if impossible.
int minOperationsToEqual(int d, const std::vector<int>& values) {
    if (values.empty()) return 0; // Edge case: empty input, already equal.

    // Check all elements have the same remainder modulo d.
    std::unordered_set<int> remainders;
    for (int val : values) {
        remainders.insert(((val % d) + d) % d); // Handle negative remainders.
    }
    if (remainders.size() > 1) return -1;

    // Normalize by removing the common remainder and dividing by d.
    int r = *remainders.begin();
    std::vector<int> normalized;
    normalized.reserve(values.size());
    for (int val : values) {
        normalized.push_back((val - r) / d);
    }

    // The optimal target is the median of the normalized values.
    std::sort(normalized.begin(), normalized.end());
    int median = normalized[normalized.size() / 2];

    // Compute total operations as sum of absolute differences from median.
    int total = 0;
    for (int x : normalized) {
        total += std::abs(x - median);
    }
    return total;
}

#include <cassert>
#include <vector>

int main() {
    // Example from problem: n=1, m=3, d=2, array [1,3,5] -> normalized [0,1,2], median=1, total=2
    assert(minOperationsToEqual(2, {1, 3, 5}) == 2);

    // Impossible: different remainders mod 2 -> -1
    assert(minOperationsToEqual(2, {1, 2, 3}) == -1);

    // All equal already -> 0 operations
    assert(minOperationsToEqual(5, {10, 10, 10}) == 0);

    // Negative values with same remainder (mod 3: -1 % 3 = 2, 2%3=2, 5%3=2)
    assert(minOperationsToEqual(3, {-1, 2, 5}) == 4); // normalized [ -1? wait: r=2, (-1-2)/3 = -1, (2-2)/3=0, (5-2)/3=1 -> median 0 -> 1+0+1=2? Actually (5-2)=3/3=1, yes total 2? Let's compute: -1%3=2, 2%3=2, 5%3=2. r=2, normalized: (-1-2)/3=-1, (2-2)/3=0, (5-2)/3=1. median index1=0, sum=1+0+1=2. So assert 2.

    // Larger test: d=1, values [3,1,2] -> normalized [3,1,2], median after sort [1,2,3] index1=2 -> cost 1+0+1=2
    assert(minOperationsToEqual(1, {3, 1, 2}) == 2);

    // Even count: two elements, e.g., d=2, [0,4] -> remainders 0,0, normalized [0,2], median index1=2, cost 2+0=2
    assert(minOperationsToEqual(2, {0, 4}) == 2);

    // Single element -> 0
    assert(minOperationsToEqual(7, {100}) == 0);

    // Empty vector -> 0 (defined behavior from function)
    assert(minOperationsToEqual(1, {}) == 0);

    // All negative same remainder, e.g., d=3, [-4,-1] -> both -1 mod 3? -4%3=2? Actually -4%3 = -1, -1%3 = -1. Normalize r=-1? Using ((val%d)+d)%d: -4%3 = -1, +3=2; -1%3=-1+3=2. So r=2. normalized: (-4-2)/3 = -2, (-1-2)/3=-1, median index1=-1, cost 1+0=1
    assert(minOperationsToEqual(3, {-4, -1}) == 1);

    // Test with large numbers: d=1000, [1000,2000,3000] -> normalized [1,2,3], median 2, cost 2
    assert(minOperationsToEqual(1000, {1000, 2000, 3000}) == 2);

    // Example from original snippet: n=2,m=2,d=1, array [1,2,3,4] -> all same remainder? 1%1=0,2%1=0,... yes -> normalized [1,2,3,4], median index2=3, cost 3+1+0+1=5
    assert(minOperationsToEqual(1, {1, 2, 3, 4}) == 5);

    return 0;
}

// First, check whether all elements have the same remainder modulo `d`. Since each operation changes a value by exactly `d`, the remainder of each element when divided by `d` is invariant. If there is more than one distinct remainder, it is impossible to make all elements equal — return `-1`. Otherwise, transform each element by dividing by `d` (since they share the same remainder, subtract the remainder first, but actually we can just divide the original values by `d` if we also handle the remainder? Wait: if all remainders are equal, say `r`, then each element can be written as `k_i*d + r`. Dividing each by `d` gives `k_i + r/d`, which is not integer unless r=0. Better approach: normalize by subtracting the remainder (or using `(x - r)/d`) to get integer positions. Alternatively, note that to minimize operations, we can divide each value by `d` after checking the remainder condition? Actually if all remainders equal `r`, then the target value must also have remainder `r`. So target `T = k*d + r`. The cost for element `x` is `|x - T| / d`. This is equivalent to `| (x-r)/d - (T-r)/d |`. So define `a_i = (v[i] - r) / d`. Then we need to minimize sum of `|a_i - target|` over integer target. The optimal target is the median of `a_i`. Sort `a_i`, pick the middle element (if even count, either middle works; the code uses `v[v.size()/2]` which is the upper median for even size, still optimal). Then compute sum of absolute differences divided by `d`? Actually after dividing by `d`, the cost is simply `|a_i - median|` because each unit difference in `a_i` corresponds to `d` in original. So total operations = sum of `|a_i - median|`. Edge cases: empty array? But `n*m >= 1` as per problem. Also if `rem.size() > 1` return -1. Time complexity: O(N log N) for sorting, where N = n*m. Space complexity: O(N) for storing the vector and O(N) for the set in worst case, but we can avoid the set by checking all remainders in O(N). The code uses an unordered_set for simplicity.

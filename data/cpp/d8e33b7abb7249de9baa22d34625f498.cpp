/*
Write a C++ function `long long totalSortedCost(std::vector<long long>& values)` that takes a non-empty vector of positive integers (each between 1 and 10^6). The function must sort the vector in ascending order, then process the elements from largest to smallest, computing a sum where the k-th largest element (1-indexed) contributes `2 * element^(k)` (element raised to the power k, multiplied by 2). Powers must be computed using an efficient binary exponentiation (O(log exponent)) function. If at any point during the accumulation the running sum exceeds 5,000,000, the function must immediately stop and return the sentinel value `-1` to indicate "Too expensive". Otherwise, return the total sum. Handle overflow in intermediate multiplications carefully (use `long long`; the maximum sum cap is 5,000,000, so any exponentiation that exceeds this threshold can be truncated to an unsafe large value early). The function must not modify the input vector if possible (pass by value or copy). Edge cases: single element vector, vector with 1000 elements, values that cause early termination, and values that remain within the cap.
*/

#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>

// Efficient binary exponentiation with early termination when value exceeds cap.
long long pow_with_cap(long long base, int exp, long long cap) {
    long long result = 1;
    long long b = base;
    int e = exp;
    while (e > 0) {
        if (e & 1) {
            // Check overflow before multiplication
            if (b > 0 && result > cap / b) {
                return cap + 1; // signal exceed
            }
            result *= b;
            if (result > cap) return cap + 1;
        }
        e >>= 1;
        if (e > 0) {
            if (b > 0 && b > cap / b) {
                b = cap + 1;
            } else {
                b *= b;
                if (b > cap) b = cap + 1;
            }
        }
    }
    return result;
}

// Returns total sum, or -1 if sum exceeds 5,000,000.
long long totalSortedCost(std::vector<long long> values) {
    const long long LIMIT = 5000000;
    if (values.empty()) return 0; // not expected per spec, but safe

    std::sort(values.begin(), values.end()); // ascending
    long long sum = 0;
    int k = 0;
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        ++k;
        long long base = *it;
        long long pow_val = pow_with_cap(base, k, LIMIT);
        if (pow_val > LIMIT) return -1;
        sum += 2 * pow_val;
        if (sum > LIMIT) return -1;
    }
    return sum;
}

#include <cassert>
#include <vector>
#include <iostream>

// Assume totalSortedCost is defined above.

int main() {
    // Basic case: {1} -> 2*1^1 = 2
    assert(totalSortedCost({1}) == 2);

    // {1,2} sorted asc [1,2] -> process 2^1*2=4, then 1^2*2=2 => total=6
    assert(totalSortedCost({1,2}) == 6);

    // {3,1,2} sorted [1,2,3] -> 3^1*2=6, 2^2*2=8, 1^3*2=2 => 16
    assert(totalSortedCost({3,1,2}) == 16);

    // {10} -> 2*10=20
    assert(totalSortedCost({10}) == 20);

    // {2,2} -> sorted [2,2] -> 2^1*2=4, 2^2*2=8 => 12
    assert(totalSortedCost({2,2}) == 12);

    // {1,1,1} -> 2+2+2=6 (since 1^k=1)
    assert(totalSortedCost({1,1,1}) == 6);

    // Exceeds: {10,10} -> 10^1*2=20, 10^2*2=200 => 220, still ok. But {100,100}? 100*2=200, 100^2*2=20000 => 20200 ok. {1000,1000}? 2000 + 2,000,000 = 2,002,000 ok. {2000,2000}? 4000 + 8,000,000 = 8,004,000 > limit -> -1
    assert(totalSortedCost({2000,2000}) == -1);

    // Many small numbers: 20 ones -> sum = 2*(1+2+...+20) = 2*210=420
    std::vector<long long> twenty_ones(20, 1);
    assert(totalSortedCost(twenty_ones) == 420);

    // Single large number exceeding cap: {5000000}? 2*5e6=10,000,000 > limit -> -1
    assert(totalSortedCost({5000000}) == -1);

    // Single near-cap: {2500000} -> 2*2.5e6=5,000,000 exactly not >, returns 5000000
    assert(totalSortedCost({2500000}) == 5000000);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The core algorithm involves three steps:
// 1. **Sorting**: Sort the vector in ascending order (using `std::sort`). This allows processing from largest to smallest by iterating from `size()-1` down to `0`.
// 2. **Exponentiation**: For each element (from largest to smallest), the exponent `k` starts at 1 and increments each time. Compute `power(element, k)` using binary exponentiation (exponentiation by squaring). Since the cap is 5,000,000, we can early-exit during exponentiation if the intermediate product exceeds 5,000,000 (because if `power` already exceeds 5,000,000, multiplying by 2 will definitely exceed cap). To avoid overflow inside `power`, we can clamp the result to a large value (e.g., `LLONG_MAX` or 5,000,001) when it exceeds cap.
// 3. **Accumulation**: After computing `2 * power`, add to running sum. If sum > 5,000,000, return -1 immediately. Otherwise continue.
// **Edge cases**:
// - Single element: sort, k=1, sum = 2 * element^1. Check if > 5e6.
// - Large exponent: For element=2, k grows fast. For element=1, 1^k = 1 always, so sum = 2*k, capped at 2*? The sum grows linearly with number of elements. For 1000 elements of 1, sum = 2+4+...+2000 = 2*(1000*1001)/2 = 1,001,000 < 5e6, so fine.
// - For element=10, k=6 => 10^6=1,000,000, times 2 = 2,000,000, and cumulative may exceed.
// - Overflow: Since we cap at 5e6, we can safely stop exponentiation when product > 5e6.
// **Complexity**: Sorting takes O(n log n). For each of the n elements, exponentiation takes O(log k) where k <= n (max ~1000 typical? But n can be large per problem constraints; the snippet used a vector of unknown length, but assume n up to 10^5? However cap makes early termination common. Worst-case n small enough that sum stays under cap, but if n large, exponentiation exponent grows to n, so O(log n) per element, total O(n log n). Overall O(n log n) time, O(1) auxiliary space (excluding input storage).

// You are given `n` arrays, each of length `m`, containing integers. Your task is to write a C++ function `long long maximumScore(vector<vector<long long>>& arrays)` that returns the maximum possible sum of prefix sums when you first choose an ordering of all `n` arrays and then concatenate their elements into a single sequence. More precisely, you may permute the arrays arbitrarily, concatenate them in that order, and then compute the sum over all prefix sums of the concatenated sequence. The goal is to maximize this sum. Note that you cannot reorder elements within an array, only the order of the arrays themselves. The function should work for any `n >= 1` and `m >= 1`, with values that fit in `long long`. Return the maximum achievable sum.

The key insight is that when concatenating arrays in a particular order, the total sum of prefix sums can be decomposed into contributions from each array based on its position. Specifically, if an array is placed first in the concatenation, every one of its elements contributes to all `n*m` prefix sums that include it. If it’s placed second, each element contributes to all prefix sums starting from its first element to the end, but the contribution is reduced by the total sum of the arrays placed before it. More formally, for a fixed order, the total sum equals the sum over all elements of their value multiplied by the number of prefix sums that include them, which is `(number_of_elements_before_it + 1)` for the first element of an array, and increases by 1 for each subsequent element in the same array. However, a simpler observation is that the optimal strategy is to sort the arrays by their total sum in ascending order. Why? Because placing an array with a larger sum earlier means its large values will be multiplied by many more prefix counts, inflating the total. Conversely, placing a larger-sum array later reduces its multiplicative weight. Thus, to minimize the weighted sum of each array’s sum, we sort ascending. But wait—the goal is to maximize the sum of prefix sums, so we actually want smaller-sum arrays first? Let’s re-derive: Consider two arrays A and B with sums S_A and S_B. If we place A before B, the contribution of A is multiplied by the number of prefix sums that include its elements, which is `(m + m?)` Actually, each element of A is included in all prefix sums that start at the first element of the concatenation up to its position. For an element at position `p` (1-indexed) in the concatenated sequence, it appears in exactly `(total_length - p + 1)` prefix sums. So the total sum is sum over positions `p` of `value[p] * (total_length - p + 1)`. This is equivalent to sorting descending? Let’s analyze: To maximize sum of `value[p] * weight[p]` where weight[p] decreases with p, we should place larger values at smaller p. But we cannot reorder within arrays, only arrays as a whole. For two arrays A and B, with sums S_A and S_B, and internal elements. If we put A first, the contribution from A is: each element of A at position (1..m) gets weight `(total_length - position + 1)`. Since A’s elements are fixed in order, the sum of weights for A is `m * total_length - (m*(m+1)/2) + m`? Actually, easier: The total sum of prefix sums can be computed as: sum over arrays in order: for the k-th array (0-indexed), its contribution is `(total_length - (k*m) - (m+1)/2?)` No, let’s just compute directly: total length L = n*m. For an array placed at starting index `s` (0-indexed), its elements are at positions s, s+1, ..., s+m-1. The weight for position p is `(L - p)`. So sum of weights for that array is `sum_{i=0}^{m-1} (L - (s+i)) = m*(L - s) - m*(m-1)/2`. This is linear in `-s`, so the earlier the array (smaller s), the larger the weight. Therefore, to maximize the total, we want larger-sum arrays to have larger weights, i.e., place larger-sum arrays earlier. But wait: the weight for each element is multiplied by its own value, not the sum. However, the total contribution of an array is the sum of `value * weight`. Since weight is decreasing with position, to maximize, we should place arrays with larger total sum earlier, because they have more “mass” to be multiplied by larger weights. This is a classic rearrangement inequality: to maximize sum of products, pair the larger sums with the larger weights. So we should sort the arrays in descending order of their sum. But the given code sorts ascending and computes the sum of prefix sums directly, which is what? Actually, the given code sorts ascending then concatenates and computes prefix sum accumulation. That yields the *minimum* possible sum? Let’s check: If you sort ascending, smaller sums get larger weights, that would minimize the total. But the problem might ask for minimum? The original snippet just sorts ascending and computes res. The task asks for maximum, so we should sort descending. Let’s confirm with an example: n=2, m=1, arrays [1] and [100]. If we place [100] first, concatenated = [100,1], prefix sums: 100, 101, total=201. If we place [1] first, concatenated=[1,100], prefix sums:1,101,total=102. So descending gives larger. Thus, the algorithm: compute sum of each array, sort the arrays by sum in descending order, concatenate, compute sum of prefix sums. Edge case: n=1 or m=1. Complexity: sorting O(n log n) for n arrays, plus O(n*m) for concatenation and summing. Space O(n*m) for concatenated vector, but we could also compute directly without concatenating. For simplicity, we concatenate.

#include <vector>
#include <algorithm>
#include <numeric>

// Return the maximum possible sum of all prefix sums after reordering arrays.
long long maximumScore(std::vector<std::vector<long long>>& arrays) {
    int n = static_cast<int>(arrays.size());
    int m = static_cast<int>(arrays[0].size());

    // Create vector of pairs (total_sum, array_index) and sort by sum descending.
    std::vector<std::pair<long long, int>> info;
    for (int i = 0; i < n; ++i) {
        long long sum = 0;
        for (long long x : arrays[i]) {
            sum += x;
        }
        info.emplace_back(sum, i);
    }
    std::sort(info.begin(), info.end(), [](const auto& a, const auto& b) {
        return a.first > b.first; // descending by sum
    });

    // Concatenate arrays in the sorted order.
    std::vector<long long> concat;
    concat.reserve(static_cast<size_t>(n) * m);
    for (const auto& p : info) {
        const auto& arr = arrays[p.second];
        concat.insert(concat.end(), arr.begin(), arr.end());
    }

    // Compute sum of all prefix sums.
    long long total = 0;
    long long current = 0;
    for (long long x : concat) {
        current += x;
        total += current;
    }
    return total;
}

#include <cassert>
#include <vector>

// Include the solution function here (or link).

int main() {
    // Test case 1: simple 2 arrays, m=1
    std::vector<std::vector<long long>> a1 = {{1}, {100}};
    assert(maximumScore(a1) == 201); // [100,1] gives prefix sums 100,101 total 201

    // Test case 2: single array
    std::vector<std::vector<long long>> a2 = {{5, 6, 7}};
    assert(maximumScore(a2) == 5 + 11 + 18); // 34

    // Test case 3: two arrays of length 2, check ordering
    std::vector<std::vector<long long>> a3 = {{1, 2}, {3, 4}};
    // Descending sums: [3,4] (sum7) first, then [1,2] (sum3)
    // Concatenate: 3,4,1,2 => prefix sums:3,7,8,10 total=28
    assert(maximumScore(a3) == 28);

    // Test case 4: equal sums, order doesn't matter
    std::vector<std::vector<long long>> a4 = {{2, 3}, {1, 4}};
    // sums both 5, any order gives same: e.g., [2,3,1,4] => 2+5+6+10=23
    assert(maximumScore(a4) == 23);

    // Test case 5: all negative numbers
    std::vector<std::vector<long long>> a5 = {{-1, -2}, {-3, -4}};
    // sums: -3 and -7, descending places -3 first: [-1,-2,-3,-4] => prefix: -1,-3,-6,-10 total=-20
    // alternative would be -7 first: [-3,-4,-1,-2] => -3,-7,-8,-10 total=-28, so maximum is -20
    assert(maximumScore(a5) == -20);

    // Test case 6: large values
    std::vector<std::vector<long long>> a6 = {{1000000000, 1000000000}, {1000000000, 1000000000}};
    // sums equal, any order: [1e9,1e9,1e9,1e9] prefix: 1e9,2e9,3e9,4e9 total=10e9
    assert(maximumScore(a6) == 10000000000LL);

    return 0;
}

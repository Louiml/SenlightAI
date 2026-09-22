// You are given a sequence of up to 500,000 indices (from 1 to 500,000), each initially with value 0. Process `q` queries, each having one of two forms:  
// - Type 1: `1 x y` — add `y` to the value stored at index `x`.  
// - Type 2: `2 x y` — compute the sum of all current values at indices `i` such that `i % x == y` (using standard C++ modulo with positive `x`), where indices are taken in the range 1..500,000.  
// Write a function `processQueries(const vector<tuple<int,int,int>>& queries)` that returns a vector of answers to the type-2 queries in the order they appear. The inputs satisfy `1 ≤ x ≤ 500,000` in all queries, and `y` may be any integer (for type 2, `y` is in `[0, x-1]` after normal positive modulo). Values and query count `q` fit within 32-bit ints, but intermediate sums may exceed 32-bit, so use 64-bit integers for the answers.

#include <cassert>
#include <vector>
#include <tuple>

// The function definition from the solution above should be placed here.
// (In a real environment, include it.)

int main() {
    // Test 1: simple additions and one query
    std::vector<std::tuple<int,int,int>> q1 = {
        {1, 2, 5},  // add 5 to index 2
        {1, 3, 7},  // add 7 to index 3
        {2, 2, 0}   // indices %2==0: 2 has5, 4 has0, ... sum=5
    };
    auto res1 = processQueries(q1);
    assert(res1.size() == 1 && res1[0] == 5);

    // Test 2: multiple additions at same index
    std::vector<std::tuple<int,int,int>> q2 = {
        {1, 10, 3},
        {1, 10, -2},
        {2, 5, 0} // indices %5==0: 10 has1, 5 has0, ... sum=1
    };
    auto res2 = processQueries(q2);
    assert(res2.size() == 1 && res2[0] == 1);

    // Test 3: large x (brute force path)
    std::vector<std::tuple<int,int,int>> q3 = {
        {1, 500001 - 1, 10}, // index 500000
        {2, 500000, 0}        // index 500000 % 500000 == 0, sum=10
    };
    auto res3 = processQueries(q3);
    assert(res3.size() == 1 && res3[0] == 10);

    // Test 4: zero y, small x
    std::vector<std::tuple<int,int,int>> q4 = {
        {1, 1, 2},
        {1, 2, 3},
        {1, 3, 5},
        {2, 1, 0} // all indices %1==0 -> sum=2+3+5=10
    };
    auto res4 = processQueries(q4);
    assert(res4.size() == 1 && res4[0] == 10);

    // Test 5: mixed, multiple queries
    std::vector<std::tuple<int,int,int>> q5 = {
        {1, 2, 1},
        {1, 4, 2},
        {1, 6, 3},
        {2, 2, 0}, // indices 2,4,6 have sum=1+2+3=6
        {2, 2, 1}, // indices 1,3,5 have sum=0
        {1, 5, 8},
        {2, 5, 0} // indices 5,10,15... : only 5 has8 -> sum=8
    };
    auto res5 = processQueries(q5);
    assert(res5.size() == 3);
    assert(res5[0] == 6);
    assert(res5[1] == 0);
    assert(res5[2] == 8);

    // Test 6: negative y? Not in spec, but ensure no crash; here y is always valid.
    // Test 7: boundary x=1
    std::vector<std::tuple<int,int,int>> q7 = {
        {1, 1, 4},
        {1, 2, 6},
        {2, 1, 0} // sum all =10
    };
    auto res7 = processQueries(q7);
    assert(res7[0] == 10);

    return 0;
}

#include <vector>
#include <tuple>
#include <cstdint>

// Process queries and return answers for type-2 queries.
// Query format: (type, x, y)
std::vector<long long> processQueries(const std::vector<std::tuple<int, int, int>>& queries) {
    const int MAX_VAL = 500000;
    const int B = 500; // bucket size for sqrt decomposition

    // val[i] = current value at index i (1-indexed)
    std::vector<long long> val(MAX_VAL + 1, 0);

    // cnt[mod-1][rem] = sum of values at indices i where i % mod == rem
    std::vector<std::vector<long long>> cnt(B, std::vector<long long>(B + 1, 0));

    std::vector<long long> answers;

    for (const auto& [t, x, y] : queries) {
        if (t == 1) {
            val[x] += y;
            // Update all small mod rows
            for (int m = 1; m <= B; ++m) {
                cnt[m - 1][x % m] += y;
            }
        } else { // t == 2
            long long ans = 0;
            if (x > B) {
                // Brute force over indices with step x
                // First index i such that i % x == y, with i >= 1
                // Since y is guaranteed in [0, x-1], if y==0, first is x, else y
                int first = (y == 0) ? x : y;
                // Ensure first >= 1 (y could be 0; handle)
                for (int i = first; i <= MAX_VAL; i += x) {
                    ans += val[i];
                }
            } else {
                ans = cnt[x - 1][y];
            }
            answers.push_back(ans);
        }
    }
    return answers;
}

// This is a classic “sqrt decomposition” or “bucketing” problem. The naive approach for type 2 would iterate over all indices matching the modulo, which could be up to 500,000 per query, too slow if many such queries. Instead, we maintain two structures:
// - A plain array `val[1..500000]` storing the current value at each index.
// - A 2D table `cnt[mod][rem]` where `mod` ranges 1..500 (or 0..499) and `rem` ranges 0..mod-1. `cnt[mod][rem]` stores the sum of all values at indices `i` such that `i % mod == rem`.
//
// When adding `y` to index `x`, we update `val[x] += y` and also update all rows `m` from 1 to 500 (or 1..B) by adding `y` to `cnt[m-1][x % m]`. For a query `2 x y`:
// - If `x <= B` (say B=500), we can answer directly: `cnt[x-1][y]`.
// - If `x > B`, then the step is large, so we can brute-force iterate over all indices `i` in the range starting from the first index with `i % x == y` up to 500,000, incrementing by `x`, and sum `val[i]` directly. The number of such indices is at most 500,000/B ≈ 1000, which is acceptable.
//
// Edge cases: `y` might be negative? In the problem statement it's guaranteed to be in `[0,x-1]`, so no issue. Also, `x` can be as large as 500,000, but for `x > B` brute-force works fine. The total time for `q` queries is O(q * (B + maxVal/B)) which for B=500 and maxVal=500k gives about O(q*1500) worst-case, fine for typical constraints. Space is O(B * B) for the table plus O(maxVal) for the array.

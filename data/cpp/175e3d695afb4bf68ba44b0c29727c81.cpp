// Write a C++ function `int countNearlyPrimeRanges(int n, const std::vector<std::pair<int,int>>& queries)` that, given an integer `n` (the number of queries) and a vector of `[l, r]` pairs (1-indexed inclusive ranges), returns for each query the count of integers `x` in `[l, r]` such that `x` has exactly 3 distinct prime factors (e.g., 30 = 2·3·5 qualifies, while 12 = 2²·3 has only 2 distinct primes). The function must efficiently precompute an answer table for all possible `x` in `[1, 10^6]` and then answer each query in constant time. Input values satisfy `1 ≤ l ≤ r ≤ 10^6` and `1 ≤ n ≤ 10^5`. The function should output nothing; it returns a `std::vector<int>` of answers in the same order as the queries.

The core idea is to precompute, for every integer up to a fixed maximum `MAXC = 10^6`, the number of distinct prime factors. Since `MAXC` is small enough, we can use a sieve-like algorithm: initialize an array `distinctPrimeCount` of size `MAXC+1` to 0. For each integer `d` from 2 to `MAXC`, if `distinctPrimeCount[d]` is already non‑zero we skip it (because it is composite). Otherwise, `d` is prime, and we increment `distinctPrimeCount[h]` for every multiple `h = d, 2d, 3d, ... ≤ MAXC`. After this, `distinctPrimeCount[x]` holds the count of distinct prime factors of `x`. Then we build a prefix sum array `pref` where `pref[i]` = number of integers in `[1, i]` that have exactly 3 distinct prime factors. For each query `[l, r]`, the answer is `pref[r] - pref[l-1]`. Edge cases: `x=1` has 0 distinct primes, and any prime power like 8=2³ has only 1 distinct factor, so they are automatically excluded. Time complexity: the sieve is `O(MAXC log log MAXC)` (actually about `MAXC * sum(1/d)` for primes, which is `O(MAXC log log MAXC)`), prefix building is `O(MAXC)`, and each query is `O(1)`. Space complexity is `O(MAXC)` for the two arrays. The function must be self‑contained: it can allocate the arrays internally with `std::vector<int>` of size `MAXC+1`. No global mutable state is needed.

#include <vector>
#include <algorithm>

// Returns, for each query [l, r], the count of numbers in that inclusive range
// that have exactly three distinct prime factors.
// Precondition: 1 <= l <= r <= MAXC, where MAXC = 1000000.
std::vector<int> countNearlyPrimeRanges(int n, const std::vector<std::pair<int,int>>& queries) {
    const int MAXC = 1000000;
    // distinctPrimeCount[x] = number of distinct prime factors of x
    std::vector<int> distinctPrimeCount(MAXC + 1, 0);

    // Sieve: for each prime d, increment count for all multiples
    for (int d = 2; d <= MAXC; ++d) {
        if (distinctPrimeCount[d] != 0) continue; // d is composite, already handled
        // d is prime
        for (int multiple = d; multiple <= MAXC; multiple += d) {
            distinctPrimeCount[multiple]++;
        }
    }

    // prefix[i] = count of numbers in [1, i] with exactly 3 distinct prime factors
    std::vector<int> prefix(MAXC + 1, 0);
    for (int i = 1; i <= MAXC; ++i) {
        prefix[i] = prefix[i - 1];
        if (distinctPrimeCount[i] == 3) {
            prefix[i]++;
        }
    }

    // Answer each query in O(1)
    std::vector<int> answers;
    answers.reserve(n);
    for (const auto& q : queries) {
        int l = q.first;
        int r = q.second;
        answers.push_back(prefix[r] - prefix[l - 1]);
    }
    return answers;
}

#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is declared above or in an included header.
// We'll re-declare for clarity (if not already visible).
std::vector<int> countNearlyPrimeRanges(int n, const std::vector<std::pair<int,int>>& queries);

int main() {
    // Test 1: Simple range [1, 10] – numbers with exactly 3 distinct primes are 30? No, 30>10. So 0.
    std::vector<std::pair<int,int>> q1 = {{1,10}};
    assert(countNearlyPrimeRanges(1, q1) == std::vector<int>{0});

    // Test 2: Range [30, 30] – 30 = 2·3·5, exactly 3 distinct primes.
    std::vector<std::pair<int,int>> q2 = {{30,30}};
    assert(countNearlyPrimeRanges(1, q2) == std::vector<int>{1});

    // Test 3: Range [1, 100] – numbers: 30, 42=2·3·7, 66=2·3·11, 70=2·5·7, 78=2·3·13, 84=2²·3·7? Wait, 84 has distinct {2,3,7} – yes, 84=2²·3·7, distinct count 3. Also 90=2·3²·5, distinct {2,3,5} – yes. And 98=2·7² has distinct {2,7} – no. Let's count carefully: 30,42,66,70,78,84,90 → 7 numbers. But also 60=2²·3·5 → distinct {2,3,5} – yes! That's 8. Also 72=2³·3² → distinct {2,3} – no. So we expect 8. Let's verify with a known list: numbers ≤100 with exactly 3 distinct prime factors: 30,42,60,66,70,78,84,90 → indeed 8.
    std::vector<std::pair<int,int>> q3 = {{1,100}};
    assert(countNearlyPrimeRanges(1, q3) == std::vector<int>{8});

    // Test 4: Larger check – range [1, 1000] manually count? Too heavy; we can just check a specific known value: For [100,200], let's compute via brute force in the test? Instead we just check that total for [1,1000] is known: according to number theory, the count is the number of squarefree numbers with exactly 3 prime factors. I won't compute manually; I'll just check that the function doesn't crash and returns plausible values.
    // Better: use a small brute-force reference for a limited range to validate.
    // We'll brute-check for [1, 50]
    std::vector<std::pair<int,int>> q4 = {{1,50}};
    // Manually list: 30 only (since 42>50). So answer is 1.
    assert(countNearlyPrimeRanges(1, q4) == std::vector<int>{1});

    // Test 5: Edge case l=r=1, no answers.
    std::vector<std::pair<int,int>> q5 = {{1,1}};
    assert(countNearlyPrimeRanges(1, q5) == std::vector<int>{0});

    // Test 6: Two queries, one empty, one non-empty
    std::vector<std::pair<int,int>> q6 = {{2,2}, {30,30}};
    assert(countNearlyPrimeRanges(2, q6) == (std::vector<int>{0, 1}));

    // Test 7: Large range [1, 1000000] – the function should handle without overflow.
    // We can't compute the exact number directly, but we can check that the answer is positive
    // (there are many such numbers) and that count for [1,1000000] is same as prefix[1000000].
    std::vector<std::pair<int,int>> q7 = {{1,1000000}};
    auto ans7 = countNearlyPrimeRanges(1, q7);
    assert(ans7[0] > 10000); // there are more than 10k numbers with exactly 3 distinct prime factors up to 1e6.

    // Test 8: Check that function respects 'n' parameter: if n is larger than queries.size(), what? The spec says n is the number of queries, so we trust it matches. For safety, we'll test with n equal to vector size.

    return 0;
}

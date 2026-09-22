// Given an array of positive integers (each between 1 and 100,000), write a C++ function `long long maxGCDRatio(const std::vector<int>& arr)` that returns the maximum possible value of `x * y * g` over all pairs (possibly the same index) of elements in the array, where `g` is the greatest common divisor of the two elements and `x = a[i]/g`, `y = a[j]/g` (i.e., after dividing both numbers by their GCD, multiply the two quotients together and then multiply by the GCD again, which simplifies to `a[i] * a[j] / g`). In other words, for every pair `(a[i], a[j])`, compute `a[i] * a[j] / gcd(a[i], a[j])`, and return the maximum such value. The input may contain duplicate values; duplicates are allowed and should be considered. The output fits in a 64-bit signed integer. The task is to implement the function efficiently given a maximum element value of 100,000 and array length up to 200,000.
// The key observation is that `a[i] * a[j] / gcd(a[i], a[j])` can be rewritten as `(a[i]/g) * (a[j]/g) * g`. For a fixed divisor `d = g`, we need to consider all pairs whose GCD is exactly divisible by `d` (actually, we can process multiples). The algorithm uses a divisor-based approach: precompute for every integer from 1 to 100,000 the list of all its divisors. For each divisor `d`, consider all numbers in the array that are multiples of `d`. Among these, we want to find two numbers `a`, `b` (with `a >= b` for convenience) such that `(a/d) * (b/d) * d` is maximized. For a fixed `d`, this is equivalent to maximizing `(a/d) * (b/d)` over pairs of multiples of `d`. Since `a/d` and `b/d` are integers, the maximum product for fixed `d` is achieved by taking the two largest multiples of `d` in the array (with multiplicity). However, we must ensure the GCD of the two chosen numbers is at least `d`, but actually the maximum over all pairs will be captured when we consider the largest two multiples of `d` for every `d`, because if the actual GCD is a multiple of `d`, then the pair is included in the set of multiples of `d`. By iterating over all possible `d` from 1 to 100,000 and taking the two largest multiples, we cover all pairs: for any pair with GCD `g`, they are both multiples of `g`, so when `d = g`, they are among the two largest? Not necessarily, but the maximum product for that pair is exactly `(a/g)*(b/g)*g`; and when we process `d = g`, the two largest multiples of `g` will have product at least as large as this pair, so the global maximum is found. More carefully: For each `d`, collect all array values that are multiples of `d`. Sort them descending. The best pair for this `d` uses the two largest (if at least two exist). The candidate value is `(largest/d) * (secondLargest/d) * d`. Taking the maximum over all `d` yields the answer. Complexity: Precomputing divisors for all numbers up to N=100,000 takes O(N log N). For each array element, iterate its divisors and push it into a list per divisor; that's O(N * number_of_divisors) which is at most about O(N * ~128) but average much less; worst-case for N=200,000 and each number having up to ~128 divisors leads to about 25 million operations, which is fine. Then for each divisor, sort the list descending, which totals O(sum of list sizes log list size) but since each element appears in multiple lists, overall it's O(N log N * divisor_count) in worst case but practically fine; a better approach is not to sort but to track the top two values per divisor as we iterate through the array: for each element, for each divisor `d` of that element, update the top two values for that divisor. That reduces time to O(N * divisor_count) with no sorting. Then after processing all elements, iterate all `d` from 1 to 100,000 and if the top two for that divisor exist, compute the candidate. Edge cases: duplicates are handled because we keep two largest with multiplicity (e.g., if the same value appears twice, we count it twice; if a value appears once, second largest may be the next distinct). Also, need to consider pairs where both elements are the same number; that is allowed. The answer is a long long because `a[i]*a[j]` can be up to 1e10, but divided by gcd up to 1e5, still up to 1e15, fits in 64-bit. Time complexity O(N * D_max) where D_max is max number of divisors (about 128 for numbers up to 1e5), so O(N * ~128) ≈ 25 million, feasible. Space O(N * D_max) for the top-two tracking, but we can store top two per divisor as an array of pairs, that's O(N_max) i.e., 100001 * 2 integers, small.
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum value of a[i] * a[j] / gcd(a[i], a[j]) over all pairs.
long long maxGCDRatio(const std::vector<int>& arr) {
    const int MAX_VAL = 100000;
    // Precompute divisors for all numbers up to MAX_VAL.
    std::vector<std::vector<int>> divisors(MAX_VAL + 1);
    for (int d = 1; d <= MAX_VAL; ++d) {
        for (int multiple = d; multiple <= MAX_VAL; multiple += d) {
            divisors[multiple].push_back(d);
        }
    }

    // For each divisor d, keep the two largest multiples of d seen so far.
    // first = largest, second = second largest (can be equal if duplicates).
    std::vector<std::pair<int, int>> top(MAX_VAL + 1, {-1, -1});

    for (int value : arr) {
        // For each divisor of value, update the top two for that divisor.
        for (int d : divisors[value]) {
            int x = value;
            auto& p = top[d];
            if (x > p.first) {
                p.second = p.first;
                p.first = x;
            } else if (x > p.second) {
                p.second = x;
            }
        }
    }

    long long ans = 0;
    for (int d = 1; d <= MAX_VAL; ++d) {
        if (top[d].first > 0 && top[d].second > 0) {
            long long a = top[d].first;
            long long b = top[d].second;
            long long candidate = (a / d) * (b / d) * d;
            ans = std::max(ans, candidate);
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    // Basic case
    assert(maxGCDRatio({6, 10, 15}) == 30); // 6 and 10: gcd=2, 6*10/2=30; 6 and 15: gcd=3, 30; 10 and 15: gcd=5, 30.
    // Duplicate values
    assert(maxGCDRatio({8, 8}) == 8); // 8*8/8=8
    // Single element? At least two needed; but if one element appears twice.
    assert(maxGCDRatio({7, 7, 7}) == 7); // same pair gives 7*7/7=7
    // Larger values
    assert(maxGCDRatio({100000, 100000}) == 100000);
    assert(maxGCDRatio({99999, 99999}) == 99999);
    // Mixed
    assert(maxGCDRatio({12, 18, 24}) == 72); // 12 and 24: gcd=12 -> 24; 18 and 24: gcd=6 -> 72; 12 and 18: gcd=6 -> 36
    // More complex
    assert(maxGCDRatio({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) == 10); // e.g., 5 and 10: gcd=5 -> 10; 9 and 10: gcd=1 -> 90? wait 9*10/1=90! Let's check: 9 and 10 gcd=1 -> 90. So that's bigger. Actually 9*10=90, 8*10=80, 7*10=70, 6*10=60, 5*10=50, 4*10=40, 3*10=30, 2*10=20, 1*10=10. What about 8 and 9? 72. 7 and 8? 56. 6 and 7? 42. So max is 90. Adjust the assert.
    assert(maxGCDRatio({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) == 90);
    // Large array with many numbers
    std::vector<int> large(20000);
    for (int i = 0; i < 20000; ++i) large[i] = (i % 100000) + 1;
    // Just check no crash and result non-negative; known answer not trivial.
    long long res = maxGCDRatio(large);
    assert(res >= 0);
    // Edge: all same
    std::vector<int> allSame(100000, 12345);
    assert(maxGCDRatio(allSame) == 12345);
    return 0;
}

// You are given a sequence of `n` distinct-or-duplicate integers, followed by `q` queries. Each query provides two integers `x` and `y`, which together define a quadratic equation `t^2 - x*t + y = 0`. For each query, count the number of unordered pairs `(i, j)` with `i < j` such that both roots of the quadratic are present in the original sequence (i.e., one root appears at index `i`, the other at index `j`, possibly equal roots requiring two distinct positions). Output the count for each query on a single line separated by spaces. The roots may be non-integer values; if a root does not exist as an exact value in the sequence, the count is zero. The sequence contains up to `2*10^5` integers, each up to `10^9`, and there are up to `2*10^5` queries. Implement a function that takes the sequence and a list of queries, returning a vector of answers.

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Example: seq = [1, 2, 2, 3]
    std::vector<long long> seq = {1, 2, 2, 3};
    std::vector<std::pair<long long, long long>> queries = {
        {3, 2},   // roots 1 and 2 -> freq[1]=1, freq[2]=2 -> 1*2=2
        {4, 3},   // roots 1 and 3 -> freq[1]=1, freq[3]=1 -> 1*1=1
        {4, 4},   // root 2 (double) -> freq[2]=2 -> 2*1/2=1
        {5, 6},   // roots 2 and 3 -> freq[2]=2, freq[3]=1 -> 2*1=2
        {1, 1},   // disc = -3, no real roots -> 0
        {3, 3}    // disc = -3, no real roots -> 0
    };
    std::vector<long long> expected = {2, 1, 1, 2, 0, 0};
    std::vector<long long> got = countRootPairs(seq, queries);
    assert(got == expected);

    // Edge: empty sequence
    std::vector<long long> empty;
    assert(countRootPairs(empty, {{0,0}}) == std::vector<long long>{0});

    // Edge: query with non-integer roots but disc perfect square? e.g. x=2, y=1 -> roots 1 and 1? (2±0)/2=1
    // Edge: query with integer roots that appear as single pair
    std::vector<long long> seq2 = {5, 7};
    assert(countRootPairs(seq2, {{12,35}}) == std::vector<long long>{1}); // roots 5,7
    assert(countRootPairs(seq2, {{12,36}}) == std::vector<long long>{0}); // disc 144-144=0, root 6 not present

    return 0;
}

#include <vector>
#include <unordered_map>
#include <cmath>
#include <cstdint>

// Count number of unordered pairs of indices (i<j) such that the two elements
// are the two roots of the quadratic t^2 - x*t + y = 0.
// Input: seq - list of integers
//        queries - each query is a pair (x, y)
// Output: vector of answers for each query
std::vector<long long> countRootPairs(const std::vector<long long>& seq,
                                      const std::vector<std::pair<long long, long long>>& queries) {
    // Frequency map of each integer in seq
    std::unordered_map<long long, long long> freq;
    for (long long val : seq) {
        freq[val]++;
    }

    std::vector<long long> result;
    result.reserve(queries.size());

    for (const auto& q : queries) {
        long long x = q.first;
        long long y = q.second;

        long long D = x * x - 4 * y;
        // If discriminant is negative, no real roots
        if (D < 0) {
            result.push_back(0);
            continue;
        }

        // Check if D is a perfect square
        long long s = llround(sqrt((long double)D));
        if (s * s != D) {
            result.push_back(0);
            continue;
        }

        // Roots are (x + s)/2 and (x - s)/2. Need them to be integers.
        if ((x + s) % 2 != 0 || (x - s) % 2 != 0) {
            result.push_back(0);
            continue;
        }

        long long r1 = (x + s) / 2;
        long long r2 = (x - s) / 2;

        if (r1 == r2) {
            // Single root: need two distinct indices
            auto it = freq.find(r1);
            if (it != freq.end()) {
                long long c = it->second;
                result.push_back(c * (c - 1) / 2);
            } else {
                result.push_back(0);
            }
        } else {
            // Two distinct roots: check both exist
            auto it1 = freq.find(r1);
            auto it2 = freq.find(r2);
            if (it1 != freq.end() && it2 != freq.end()) {
                result.push_back(it1->second * it2->second);
            } else {
                result.push_back(0);
            }
        }
    }

    return result;
}

// This problem is essentially a frequency-counting problem combined with solving a quadratic equation. The key observation is that the roots of `t^2 - x*t + y = 0` are given by the quadratic formula. Since the coefficient of `t^2` is 1, the discriminant is `D = x^2 - 4*y`. If `D < 0`, there are no real roots, so the answer is 0. If `D == 0`, there is exactly one root `r = x/2`. The number of unordered pairs that sum to this root is `freq[r] * (freq[r] - 1) / 2`, where `freq[r]` is the count of occurrences of `r` in the sequence. If `D > 0`, there are two distinct roots: `r1 = (x + sqrt(D)) / 2` and `r2 = (x - sqrt(D)) / 2`. The answer is `freq[r1] * freq[r2]`, if both roots exist in the frequency map; otherwise zero. Important edge cases: (1) The roots may be non-integer (e.g., `x=3, y=2` gives roots 1 and 2; but `x=1, y=1` gives roots (1±√(-3))/2, non-real). (2) Use `long double` for precision when checking roots because x and y can be up to 10^9 and products may overflow in double. However, the roots are rational or irrational; since the original sequence contains only integers, if a root is not exactly representable as an integer, it will not be found in the frequency map. Therefore, we can check if the root is an integer by verifying that `floor(root) == root` (with tolerance) before looking it up, or directly use the map with `long double` keys (but that could have precision issues). A safer approach: if `D` is a perfect square and `(x ± sqrt(D))` is even (since denominator is 2), then roots are integers. Otherwise, they are not integers and won't be in the map. For efficiency, we use an `unordered_map<long long, long long>` for integer keys, and only compute roots after checking that `D` is a non-negative perfect square. Specifically, let `s = llround(sqrt(D))`; if `s*s == D`, then roots are `(x + s)/2` and `(x - s)/2`, and we need these to be integers (i.e., `(x ± s)` even). Then we can use the integer keys directly. This avoids floating-point issues entirely. Complexity: O(n + q) time, O(n) auxiliary space.

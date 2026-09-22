Given a string consisting only of uppercase English letters, write a C++ function that returns the total number of triples `(i, j, k)` with `1 <= i < j < k <= n` (where `n` is the string length) such that `s[i] == s[k]` and the character at position `j` is strictly between positions `i` and `k` alphabetically (i.e., `s[i] < s[j] < s[k]` or `s[k] < s[j] < s[i]`? Actually, the original problem counts triples where `i < j < k` and `s[i] == s[k]` and `s[j]` is any character, not necessarily between – but the given code counts something more specific: it counts the number of ways to choose two occurrences of the same character and an index strictly between them, regardless of the middle character's value. The code sums over each character, for each pair of its occurrences (first occurrence at position `a` and later occurrence at position `b`), the number of indices strictly between them, which is `b - a - 1`. The code uses prefix sums to compute this efficiently. For a string, write a function `long long countTriples(const std::string& s)` that returns the total number of triples `(i, j, k)` with `i < j < k` and `s[i] == s[k]`. The middle character can be any character, including the same one. The function must handle strings up to length 100,000 and return a 64-bit integer. Edge cases: strings of length < 3 return 0, and duplicate characters with only one occurrence contribute nothing.

The problem is to count triples `(i, j, k)` where `i < j < k` and `s[i] == s[k]`. For each character `ch`, we have a list of positions where it occurs: `pos[0], pos[1], ..., pos[m-1]`. For any pair of occurrences `(p, q)` with `p < q`, the number of valid `j` such that `p < j < q` is exactly `q - p - 1`. But we must count each triple exactly once. The naive approach would be O(n^2) per character, which is too slow. The key is to use prefix sums. For each character, process its positions in increasing order. When we are at the `j`-th occurrence (0-indexed position in the list), we want to add the total number of triples where this occurrence is the right endpoint `k`. For each previous occurrence `p` (at index `i` in the list), the contribution is `(pos[j] - pos[i] - 1)`. Summing over all `i < j`, we get: `j * pos[j] - sum_{i=0}^{j-1} pos[i] - j`. Because there are `j` previous occurrences, each contributes `pos[j] - pos[i] - 1`. Using a prefix sum of positions, we can compute `sum_{i=0}^{j-1} pos[i]` in O(1). Thus, for each character, we iterate over its positions once, maintaining prefix sums, and accumulate the answer. This is O(n) overall, since each character position is visited once. Space is O(n) for storing positions, but we can also do it with per-character vectors. The answer can be large (up to ~n^3/27 for a string with many same characters), so use `long long`. Edge cases: if a character occurs fewer than 2 times, it contributes nothing. If the string length < 3, the answer is 0. Time complexity O(n), space O(n).

#include <vector>
#include <string>

// Count triples (i, j, k) with i < j < k and s[i] == s[k].
long long countTriples(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n < 3) return 0LL;

    std::vector<std::vector<int>> positions(26);
    for (int idx = 0; idx < n; ++idx) {
        positions[s[idx] - 'A'].push_back(idx);
    }

    long long total = 0;
    for (int c = 0; c < 26; ++c) {
        const auto& pos = positions[c];
        const int m = static_cast<int>(pos.size());
        if (m < 2) continue;

        // Prefix sum of positions for this character.
        std::vector<long long> prefix(m + 1, 0);
        for (int i = 1; i <= m; ++i) {
            prefix[i] = prefix[i - 1] + pos[i - 1];
        }

        // For each occurrence as the right end k (index j in pos),
        // add contributions from all previous occurrences as left end i.
        for (int j = 1; j < m; ++j) {
            // Number of positions strictly between pos[j] and each previous pos[i].
            // For a fixed j, contribution sum_{i=0}^{j-1} (pos[j] - pos[i] - 1)
            // = j * pos[j] - prefix[j] - j.
            total += static_cast<long long>(j) * pos[j] - prefix[j] - static_cast<long long>(j);
        }
    }

    return total;
}

#include <cassert>

int main() {
    // Simple cases
    assert(countTriples("ABC") == 0); // No equal endpoints
    assert(countTriples("AAA") == 1); // Only triple: i=0,j=1,k=2
    assert(countTriples("ABCA") == 1); // i=0,k=3, j can be 1 or 2? Actually only j=2 gives s[0]==s[3], but j must be strictly between, so j=1 or j=2 both valid -> 2 triples? Let's check: positions: A at 0,3. j can be 1 and 2, so 2 triples: (0,1,3) and (0,2,3). So answer 2.
    assert(countTriples("ABCA") == 2);
    // Multiple same characters
    assert(countTriples("AAAA") == 4); // Pairs of A: (0,1) gives j? Actually triples: (0,1,2),(0,1,3),(0,2,3),(1,2,3) -> 4.
    assert(countTriples("AABB") == 0); // A at 0,1: j between 0 and 1? No. B at 2,3: same. So 0.
    // Mixed case
    assert(countTriples("ABABA") == ?); // Let's compute: A at 0,2,4; B at 1,3. For A: pairs (0,2) -> j=1 (only index between) => 1; (0,4) -> j=1,2,3 => 3; (2,4) -> j=3 => 1. Total A: 5. For B: pair (1,3) -> j=2 =>1. Total 6. So assert(countTriples("ABABA") == 6);
    assert(countTriples("ABABA") == 6);
    // Long string all same
    std::string allSame(1000, 'A');
    // Total triples: C(1000,3) = 1000*999*998/6 = 166167000. Let's compute formula: sum over each pair (i,k) of (k-i-1). For all same, that's the number of triples = n*(n-1)*(n-2)/6.
    long long expected = 1000LL * 999 * 998 / 6;
    assert(countTriples(allSame) == expected);
    // Edge: length 2
    assert(countTriples("AA") == 0);
    return 0;
}

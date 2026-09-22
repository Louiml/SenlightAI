// Write a C++ function that, given a vector of strings representing sentences (each consisting of words separated by spaces), groups sentences into equivalence classes where two sentences are considered "similar" if they differ by at most one word (i.e., their word-level Levenshtein distance is ≤ 1, where insertion, deletion, or substitution of a single word counts as one edit). Return the total number of unordered pairs of sentences (across all sentences, including duplicates of the same sentence) that are similar. For identical sentences, if a sentence appears `k` times in the input, it contributes `k*(k-1)/2` pairs (since each pair of identical sentences is similar with distance 0). For distinct sentences `s1` and `s2` appearing `c1` and `c2` times respectively, if they are similar (distance exactly 1), they contribute `c1*c2` pairs. The function must handle duplicate sentences, sentences of varying word counts, and must only consider pairs of sentences whose word counts differ by at most 1 (otherwise distance ≥ 2). The input vector may be empty or contain sentences with only one word. The function must be efficient for large inputs; an approach based on grouping by word count and using hashing or pairwise comparison within size-constrained groups is acceptable.

#include <cassert>
#include <vector>
#include <string>

// Include the solution function (assume it's defined above).

int main() {
    // Single sentence, no pairs.
    assert(countSimilarPairs({}) == 0);
    assert(countSimilarPairs({"hello"}) == 0);
    assert(countSimilarPairs({"hello world"}) == 0);

    // Two identical sentences: distance 0, pair count = 1.
    assert(countSimilarPairs({"a b c", "a b c"}) == 1);

    // Three identical: 3 choose 2 = 3 pairs.
    assert(countSimilarPairs({"x y", "x y", "x y"}) == 3);

    // Two distinct, same length, distance 1 (substitution).
    assert(countSimilarPairs({"a b c", "a b d"}) == 1);

    // Two distinct, lengths differ by 1, distance 1 (insertion/deletion).
    assert(countSimilarPairs({"a b c", "a b c d"}) == 1);

    // Cases with multiple pairs and duplicates.
    std::vector<std::string> data = {
        "cat dog", "cat dog", "cat dog",  // 3 copies -> 3 pairs
        "cat dot", "cat dot",             // 2 copies, both similar to "cat dog" -> 2*3 = 6 pairs
        "bird fish", "bird fish"          // 2 copies, different from others -> 1 pair (their own)
    };
    // "cat dog" count=3, "cat dot" count=2. Similar (distance 1) -> 3*2=6.
    // "bird fish" count=2 -> 1 pair.
    // Total = 3 + 6 + 1 = 10.
    assert(countSimilarPairs(data) == 10);

    // Edge: sentence with one word vs two words.
    assert(countSimilarPairs({"apple", "apple pie"}) == 1);
    assert(countSimilarPairs({"apple", "orange pie"}) == 0);
    assert(countSimilarPairs({"apple", "apple"}) == 1);

    // No similarity: different lengths by more than 1.
    assert(countSimilarPairs({"a b c", "a b c d e"}) == 0);

    // Multiple lengths but no similar pairs.
    assert(countSimilarPairs({"one two", "one two three four"}) == 0);

    // Large input with no similarity.
    std::vector<std::string> many;
    for (int i = 0; i < 100; ++i) many.push_back("sentence number " + std::to_string(i));
    assert(countSimilarPairs(many) == 0);

    return 0;
}

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>

// Helper: split a sentence string into a vector of words.
static std::vector<std::string> splitWords(const std::string& s) {
    std::istringstream iss(s);
    std::vector<std::string> words;
    std::string w;
    while (iss >> w) {
        words.push_back(w);
    }
    return words;
}

// Helper: compute word-level edit distance between two word vectors.
// Assumes sizeA <= sizeB and sizeB - sizeA <= 1, but works generally.
static int editDistWords(const std::vector<std::string>& A, const std::vector<std::string>& B) {
    size_t n = A.size();
    size_t m = B.size();
    if (m - n > 1) return 2; // too far

    // Simple DP for small sizes (n,m <= 1 difference, but still general).
    std::vector<int> prev(m + 1), curr(m + 1);
    for (size_t j = 0; j <= m; ++j) prev[j] = static_cast<int>(j);
    for (size_t i = 1; i <= n; ++i) {
        curr[0] = static_cast<int>(i);
        for (size_t j = 1; j <= m; ++j) {
            int cost = (A[i-1] == B[j-1]) ? 0 : 1;
            curr[j] = std::min({prev[j] + 1, curr[j-1] + 1, prev[j-1] + cost});
        }
        std::swap(prev, curr);
    }
    return prev[m];
}

// Main function: count pairs of sentences (including duplicates) with edit distance <= 1.
long long countSimilarPairs(const std::vector<std::string>& sentences) {
    if (sentences.empty()) return 0;

    // Map: length -> (sentence-as-key -> count)
    std::unordered_map<size_t, std::unordered_map<std::string, long long>> lengthMap;

    for (const auto& s : sentences) {
        auto words = splitWords(s);
        // Build a canonical key: join words with a space (or use the original string).
        // We use the original string as key; identical strings are duplicates.
        lengthMap[words.size()][s]++;
    }

    long long total = 0;

    // Collect all unique lengths.
    std::vector<size_t> lengths;
    for (const auto& kv : lengthMap) {
        lengths.push_back(kv.first);
    }
    std::sort(lengths.begin(), lengths.end());

    // Process each length independently: pairs within the same length.
    for (size_t L : lengths) {
        auto& bucket = lengthMap[L];
        // Convert to vectors of (wordVec, count).
        std::vector<std::pair<std::vector<std::string>, long long>> items;
        for (const auto& kv : bucket) {
            items.emplace_back(splitWords(kv.first), kv.second);
        }
        size_t m = items.size();
        for (size_t i = 0; i < m; ++i) {
            // Pairs of identical sentences (same key) are handled by count choose 2.
            long long ci = items[i].second;
            total += ci * (ci - 1) / 2;
            for (size_t j = i + 1; j < m; ++j) {
                // Different strings: compute distance.
                auto& A = items[i].first;
                auto& B = items[j].first;
                int d = editDistWords(A, B);
                if (d <= 1) {
                    total += items[i].second * items[j].second;
                }
            }
        }
    }

    // Process cross-length pairs: L and L+1.
    for (size_t idx = 0; idx + 1 < lengths.size(); ++idx) {
        size_t L = lengths[idx];
        size_t L1 = lengths[idx + 1];
        if (L1 != L + 1) continue; // only adjacent lengths

        auto& bucketL = lengthMap[L];
        auto& bucketL1 = lengthMap[L1];
        std::vector<std::pair<std::vector<std::string>, long long>> itemsL;
        std::vector<std::pair<std::vector<std::string>, long long>> itemsL1;
        for (const auto& kv : bucketL) itemsL.emplace_back(splitWords(kv.first), kv.second);
        for (const auto& kv : bucketL1) itemsL1.emplace_back(splitWords(kv.first), kv.second);

        for (const auto& p1 : itemsL) {
            for (const auto& p2 : itemsL1) {
                int d = editDistWords(p1.first, p2.first); // shorter, longer
                if (d == 1) {
                    total += p1.second * p2.second;
                }
            }
        }
    }

    return total;
}

// The core challenge is to count similar sentence pairs without comparing every pair of sentences (which would be O(N²)). Since two sentences can be at most one edit apart only if their word counts differ by at most 1, we can group sentences by their word count. Within each group (same length), we need to find pairs with at most one word substitution; across adjacent length groups (length L and L+1), we need to find pairs where one word is inserted/deleted.
//
// A practical approach:
// 1. Parse each input string into a vector of words. Keep track of the original sentence string and its frequency (count of duplicates).
// 2. Group sentences by word count. For each group, we can compute a hash for the entire sequence of words (e.g., using a standard hash of the concatenated words) to help find near-duplicates. But for a guaranteed correct result, within a group of size `m`, we can still compare all pairs (O(m²)) after filtering by a cheap "length difference ≤1" check – which is already true within the same length group. For length L and L+1 groups, we also compare all pairs. This is acceptable if the input has many distinct lengths but few sentences per length bucket; in the worst case (all sentences same length), it degrades to O(N²). To mitigate, we can use a hash-based bucketing: for each sentence, compute two hashes: one from the first few words (e.g., first half) and one from the last few words, then only compare sentences that share at least one hash bucket. This reduces comparisons but may miss some pairs if the edit is in a position that changes both hashes; however, for distance ≤1, at least one of these hashes (first or last half) must remain identical unless the edit affects both halves, which is impossible if the sentence has more than two words. For simplicity in a standalone task, a direct pairwise comparison within length-based buckets is acceptable and correct; we can mention that for large inputs, hashing reduces complexity.
//
// The algorithm:
// - Convert each string to a vector of words.
// - Build a map from length → map from word-vector (as a string key) → count. This handles duplicates: identical sentences are grouped with their count.
// - For each length L, collect all unique sentence vectors (with counts) in that length. Compare each pair: compute word-level edit distance (with insertion/deletion/substitution) and check if ≤1. If identical, add c1*(c1-1)/2. If distinct and distance == 1, add c1*c2.
// - For each adjacent pair of lengths (L and L+1), compare all unique sentences from length L against all unique sentences from length L+1. Since lengths differ by exactly 1, the distance is 1 if and only if the shorter sentence can be obtained by deleting one word from the longer (or equivalently, inserting one word). This can be checked by a single-pass comparison with one allowed skip.
// - Sum all contributions.
//
// Edge cases: empty input → 0. Sentences with one word: length difference check works. Duplicate handling: we must not double-count identical sentences. For length L vs L+1, a sentence of length L can match multiple sentences of length L+1 (e.g., by deleting different words). We count all such pairs.
//
// Time complexity: Let `u` be the total number of unique sentences across all lengths. In the worst case (all same length), we do O(u²) distance computations, each taking O(L) time (where L is the average number of words), so O(u² * L). For typical data with many lengths, this is much better. Space: O(u * L) for storing word vectors.

// Write a C++ function that extracts n-gram counts and matches from a pair of integer sequences (reference and prediction) after trimming specified padding and end-of-sentence tokens from both ends, then accumulates the results into a provided statistics structure. The function must compute unigram, bigram, trigram, and 4-gram statistics, where each n-gram is identified by a 64-bit hash of the sequence of integer tokens, and matches are counted by comparing the multiset of n-grams from the reference and prediction (each reference n-gram can match at most one prediction n-gram). The statistics structure contains fields for total reference length, total prediction length, and for each n-gram order (1–4) a count (total number of n-grams in prediction) and a match (number of n-grams in prediction that also appear in the reference). The function must handle sequences shorter than the n-gram order gracefully (contributing zero counts and matches for that order) and must not modify the input arrays.

#include <cassert>
#include <vector>

int main() {
    // Simple perfect match after trimming pads and eos.
    {
        NGramStats s = {0,0,0,0,0,0,0,0,0,0};
        std::vector<int> ref = {0, 1, 2, 3, 0, 0}; // pad=0, eos=0 (right trim also removes trailing 0)
        std::vector<int> pred = {0, 1, 2, 3, 0};
        addNGramStatistics(s, ref.size(), ref.data(), pred.size(), pred.data(), 0, 0);
        // After trim both are {1,2,3}, length 3.
        assert(s.totalRefLen == 3);
        assert(s.totalPredLen == 3);
        // Unigrams: pred has 3, all match → count1=3, match1=3
        assert(s.count1 == 3 && s.match1 == 3);
        // Bigrams: pred has 2, all match → count2=2, match2=2
        assert(s.count2 == 2 && s.match2 == 2);
        // Trigrams: pred has 1, match → count3=1, match3=1
        assert(s.count3 == 1 && s.match3 == 1);
        // 4-grams: pred has 0 (len<4) → count4=0, match4=0
        assert(s.count4 == 0 && s.match4 == 0);
    }

    // Prediction longer than reference: partial matches.
    {
        NGramStats s = {0,0,0,0,0,0,0,0,0,0};
        std::vector<int> ref = {1, 2, 3};
        std::vector<int> pred = {1, 2, 3, 4};
        addNGramStatistics(s, ref.size(), ref.data(), pred.size(), pred.data(), -1, -1); // no trimming
        assert(s.totalRefLen == 3 && s.totalPredLen == 4);
        // Unigrams: pred count=4, ref has 1,2,3 → match=3
        assert(s.count1 == 4 && s.match1 == 3);
        // Bigrams: pred count=3 (1-2,2-3,3-4), ref has 1-2 and 2-3 → match=2
        assert(s.count2 == 3 && s.match2 == 2);
        // Trigrams: pred count=2 (1-2-3, 2-3-4), ref has 1-2-3 → match=1
        assert(s.count3 == 2 && s.match3 == 1);
        // 4-grams: pred count=1 (1-2-3-4), ref len=3 → no ref 4-gram → match=0
        assert(s.count4 == 1 && s.match4 == 0);
    }

    // Duplicate n-grams in prediction: each can match only once.
    {
        NGramStats s = {0,0,0,0,0,0,0,0,0,0};
        std::vector<int> ref = {7, 7};
        std::vector<int> pred = {7, 7, 7};
        addNGramStatistics(s, ref.size(), ref.data(), pred.size(), pred.data(), -1, -1);
        // Unigrams: pred count=3, ref has two 7's → match=2
        assert(s.count1 == 3 && s.match1 == 2);
        // Bigrams: pred count=2 (7-7 twice), ref has 1 (7-7) → match=1
        assert(s.count2 == 2 && s.match2 == 1);
        // Trigram: pred count=1 (7-7-7), ref no trigram → match=0
        assert(s.count3 == 1 && s.match3 == 0);
    }

    // Empty after trimming: all zeros.
    {
        NGramStats s = {0,0,0,0,0,0,0,0,0,0};
        std::vector<int> ref = {0, 0};
        std::vector<int> pred = {0, 0};
        addNGramStatistics(s, ref.size(), ref.data(), pred.size(), pred.data(), 0, 0);
        assert(s.totalRefLen == 0 && s.totalPredLen == 0);
        assert(s.count1 == 0 && s.match1 == 0);
        assert(s.count2 == 0 && s.match2 == 0);
        assert(s.count3 == 0 && s.match3 == 0);
        assert(s.count4 == 0 && s.match4 == 0);
    }

    // Trimming a mix of pad and eos.
    {
        NGramStats s = {0,0,0,0,0,0,0,0,0,0};
        // pad=0, eos=9
        std::vector<int> ref = {0, 1, 2, 9, 0};
        std::vector<int> pred = {0, 0, 1, 2, 9};
        addNGramStatistics(s, ref.size(), ref.data(), pred.size(), pred.data(), 0, 9);
        // Both trim to {1,2} length 2.
        assert(s.totalRefLen == 2 && s.totalPredLen == 2);
        assert(s.count1 == 2 && s.match1 == 2);
        assert(s.count2 == 1 && s.match2 == 1);
        assert(s.count3 == 0 && s.match3 == 0);
        assert(s.count4 == 0 && s.match4 == 0);
    }

    return 0;
}

#include <map>
#include <cstddef>
#include <cstring>

struct NGramStats {
    std::size_t totalRefLen;
    std::size_t totalPredLen;
    std::size_t count1, match1;
    std::size_t count2, match2;
    std::size_t count3, match3;
    std::size_t count4, match4;
};

// FNV-1a hash over the raw bytes of len consecutive integers.
static std::size_t hashInts(const int* data, std::size_t len) {
    std::size_t h = 14695981039346656037ull;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(data);
    std::size_t byteLen = len * sizeof(int);
    for (std::size_t i = 0; i < byteLen; ++i) {
        h ^= bytes[i];
        h *= 0x100000001b3ull;
    }
    return h;
}

// Accumulate n-gram counts and matches for one n-gram order.
static void accumulateNGrams(
    std::size_t& totalCount,
    std::size_t& totalMatch,
    std::size_t n,
    std::size_t refLen,
    const int* ref,
    std::size_t predLen,
    const int* pred) {

    if (predLen < n) return;

    std::size_t predNGrams = predLen - n + 1;
    totalCount += predNGrams;

    if (refLen < n) return;

    // Count all n-grams in the prediction.
    std::map<std::size_t, std::size_t> predCounts;
    for (std::size_t i = 0; i < predNGrams; ++i) {
        std::size_t h = hashInts(pred + i, n);
        ++predCounts[h];
    }

    // Match n-grams from the reference, each can be used at most once.
    std::size_t refNGrams = refLen - n + 1;
    for (std::size_t i = 0; i < refNGrams; ++i) {
        std::size_t h = hashInts(ref + i, n);
        auto it = predCounts.find(h);
        if (it != predCounts.end() && it->second > 0) {
            ++totalMatch;
            --it->second;
        }
    }
}

// Trim leading pad tokens and trailing pad/eos tokens from a sequence.
static void trimSequence(std::size_t& len, const int*& data, int pad, int eos) {
    std::size_t start = 0;
    while (start < len && data[start] == pad) ++start;
    data += start;
    len -= start;

    if (len == 0) return;
    std::size_t end = len - 1;
    while (end > 0 && (data[end] == eos || data[end] == pad)) --end;
    len = end + 1;
}

// Update NGramStats with one reference–prediction pair.
void addNGramStatistics(
    NGramStats& stats,
    std::size_t refLen,
    const int* ref,
    std::size_t predLen,
    const int* pred,
    int pad,
    int eos) {

    trimSequence(refLen, ref, pad, eos);
    trimSequence(predLen, pred, pad, eos);

    stats.totalRefLen += refLen;
    stats.totalPredLen += predLen;

    accumulateNGrams(stats.count1, stats.match1, 1, refLen, ref, predLen, pred);
    accumulateNGrams(stats.count2, stats.match2, 2, refLen, ref, predLen, pred);
    accumulateNGrams(stats.count3, stats.match3, 3, refLen, ref, predLen, pred);
    accumulateNGrams(stats.count4, stats.match4, 4, refLen, ref, predLen, pred);
}

// The solution processes each pair independently. First, trim the reference and prediction sequences by advancing the pointer past leading padding tokens and reducing the length to exclude trailing padding and end-of-sentence tokens. Then, for each n from 1 to 4, compute the number of n-grams in the prediction and the number of those n-grams that appear in the reference. The prediction contributes a count equal to `predLen - n + 1` (if non-negative) and the reference contributes at most that same number of matches. To count matches without double-counting, build a frequency map from the prediction's n-gram hashes, then iterate through the reference's n-grams and decrement the map entry each time the hash is found with a positive count. The hash function should be deterministic (e.g., FNV-1a over the raw bytes of the integer array) and must treat sequences of different lengths differently. Edge cases: both sequences may be empty after trimming, in which case all counts and matches remain zero for all n; if one sequence is shorter than n, that order contributes zero; if prediction is shorter than n but reference is longer, count is zero and matches must also be zero. Time complexity is O(predLen + refLen) per n-gram order, totaling O(n * (predLen + refLen)) for the four orders, and space complexity O(predLen) for the hash map.

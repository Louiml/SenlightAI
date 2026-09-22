/*
Write a C++ function `std::vector<std::string> findCommonKmers(const std::string& transcript, const std::string& read, size_t kmer_length)` that takes two DNA sequences (strings consisting only of characters 'A', 'C', 'G', 'T') and an integer `kmer_length` (assumed to be less than or equal to the length of both strings). The function must return a vector of all distinct k-mers (substrings of length `kmer_length`) that appear in both the transcript and the read, in the order they first appear in the transcript. If no common k-mers exist, return an empty vector. The function must handle case-insensitive input by converting all characters to uppercase. Duplicate k-mers within either sequence must be considered only once (i.e., the result contains each common k-mer exactly once). Edge cases: if `kmer_length` is 0 or greater than the length of either string, return an empty vector. The solution must not use any external libraries beyond the standard C++ library.
*/
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cctype>

std::vector<std::string> findCommonKmers(const std::string& transcript, const std::string& read, size_t kmer_length) {
    std::vector<std::string> common_kmers;

    if (kmer_length == 0 || kmer_length > transcript.size() || kmer_length > read.size()) {
        return common_kmers;
    }

    // Normalize to uppercase
    std::string trans = transcript;
    std::string rd = read;
    std::transform(trans.begin(), trans.end(), trans.begin(), ::toupper);
    std::transform(rd.begin(), rd.end(), rd.begin(), ::toupper);

    // Build set of k-mers from read
    std::unordered_set<std::string> read_kmers;
    for (size_t i = 0; i + kmer_length <= rd.size(); ++i) {
        read_kmers.insert(rd.substr(i, kmer_length));
    }

    // Scan transcript in order, add common k-mers once
    std::unordered_set<std::string> seen;
    for (size_t i = 0; i + kmer_length <= trans.size(); ++i) {
        std::string kmer = trans.substr(i, kmer_length);
        if (read_kmers.count(kmer) && seen.insert(kmer).second) {
            common_kmers.push_back(kmer);
        }
    }

    return common_kmers;
}
#include <cassert>
#include <vector>
#include <string>

// Assume the function is defined above or included here.

int main() {
    // Basic case
    std::vector<std::string> result = findCommonKmers("ACGTACGT", "TACGTACG", 4);
    assert((result == std::vector<std::string>{"ACGT", "CGTA", "GTAC", "TACG"}));

    // Case insensitivity
    result = findCommonKmers("acgt", "ACGT", 4);
    assert((result == std::vector<std::string>{"ACGT"}));

    // No common k-mers
    result = findCommonKmers("AAAA", "CCCC", 2);
    assert(result.empty());

    // kmer length larger than string
    result = findCommonKmers("AC", "ACGT", 3);
    assert(result.empty());

    // kmer length zero
    result = findCommonKmers("ACGT", "ACGT", 0);
    assert(result.empty());

    // Duplicate k-mers in transcript appear once
    result = findCommonKmers("AAAA", "AAAA", 2);
    assert((result == std::vector<std::string>{"AA"}));

    // k-mers with overlap and different lengths
    result = findCommonKmers("ATGCATGC", "GCATGCAT", 3);
    assert((result == std::vector<std::string>{"ATG", "TGC", "GCA", "CAT"}));

    // One string shorter than kmer, other valid
    result = findCommonKmers("ATC", "ATCGATC", 4);
    assert(result.empty());

    // All possible k-mers are common
    result = findCommonKmers("AAAA", "AAAA", 1);
    assert((result == std::vector<std::string>{"A"}));

    // Mixed case and spaces (though spaces should not appear, just robust)
    result = findCommonKmers("aCgT", "AcGt", 2);
    assert((result == std::vector<std::string>{"AC", "CG", "GT"}));

    return 0;
}
// The main algorithm involves extracting all distinct k-mers from both input sequences and then finding their intersection. To achieve this efficiently, use `std::unordered_set<std::string>` to store unique k-mers from the transcript and the read. First, normalize both strings to uppercase using `std::transform` with `::toupper`. Then, iterate through the transcript string, extracting each k-mer of length `kmer_length` (from index 0 to `transcript.size() - kmer_length`), and insert each into a transcript set. Similarly, build a read set from the read string. Then, iterate through the transcript string again (from index 0 onward) and for each k-mer that exists in the read set, add it to a result vector and erase it from the read set (or use a `std::unordered_set` for the result to ensure uniqueness while preserving order by only adding when first encountered). To preserve the order of first appearance in the transcript, iterate through transcript in order and use a `std::unordered_set` for seen results. Return the vector. Edge cases: before processing, check if `kmer_length == 0` or `kmer_length > transcript.size()` or `kmer_length > read.size()` — if any, return empty vector. Time complexity: building both sets is O(n + m) where n and m are lengths of transcript and read; the final scan is O(n). Space complexity is O(n + m) for the sets, plus O(k) per stored k-mer (proportional to k) in the worst case. The use of unordered sets gives average O(1) insert/lookup.

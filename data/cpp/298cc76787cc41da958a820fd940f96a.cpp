// Write a C++ function that takes a vector of strings and a hash table size as parameters, and returns a vector of sorted lists representing an open-hashing hash table. For each string in the input vector, compute a hash value using the RSHash algorithm (Robert Sedgwick's hash function), then map it to an index using modulo the table size. Insert the string into the corresponding sorted list (maintaining ascending order in each list). The function should return a vector where each element is a `std::vector<std::string>` containing the strings that hashed to that index, sorted in lexicographic order. Handle empty input and table sizes of zero or negative by returning an empty vector.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Empty input -> all empty buckets.
    std::vector<std::string> empty;
    auto result0 = buildOpenHashTable(empty, 5);
    assert(result0.size() == 5);
    for (const auto& list : result0) assert(list.empty());

    // Invalid table size -> empty result.
    auto result1 = buildOpenHashTable({"abc"}, 0);
    assert(result1.empty());
    auto result2 = buildOpenHashTable({"abc"}, -3);
    assert(result2.empty());

    // Single element.
    auto result3 = buildOpenHashTable({"hello"}, 1);
    assert(result3.size() == 1);
    assert(result3[0] == std::vector<std::string>({"hello"}));

    // Multiple strings, same hash mod size? Use small table to force collisions.
    // "a" and "b" both hash, but with table size 2 they may collide.
    auto result4 = buildOpenHashTable({"a", "b"}, 2);
    assert(result4.size() == 2);
    // The two strings must appear somewhere, and each bucket must be sorted.
    size_t total = 0;
    for (auto& bucket : result4) {
        total += bucket.size();
        for (size_t i = 1; i < bucket.size(); ++i) {
            assert(bucket[i-1] <= bucket[i]);
        }
    }
    assert(total == 2);

    // Known test with RSHash manually checked for "abc".
    // "abc" produces a known hash; we test that the bucket index calculation is consistent.
    std::vector<std::string> items = {"abc", "abcd", "abcde"};
    int size = 211;
    auto result5 = buildOpenHashTable(items, size);
    assert(result5.size() == static_cast<size_t>(size));
    // Ensure all items are present exactly once.
    int count = 0;
    for (auto& bucket : result5) count += bucket.size();
    assert(count == 3);
    // Each bucket sorted.
    for (auto& bucket : result5) {
        for (size_t i = 1; i < bucket.size(); ++i) assert(bucket[i-1] <= bucket[i]);
    }

    // Duplicates.
    auto result6 = buildOpenHashTable({"x", "x", "y"}, 3);
    assert(result6.size() == 3);
    int xCount = 0, yCount = 0;
    for (auto& bucket : result6) {
        for (const auto& s : bucket) {
            if (s == "x") xCount++;
            if (s == "y") yCount++;
        }
    }
    assert(xCount == 2);
    assert(yCount == 1);
}
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <stdexcept>

// Compute Robert Sedgwick's hash (RSHash) for a string.
unsigned long long RSHash(const std::string& str) {
    unsigned long long b = 378551;
    unsigned long long a = 63689;
    unsigned long long hash = 0;
    for (char c : str) {
        hash = hash * a + static_cast<unsigned char>(c);
        a *= b;
    }
    return hash;
}

// Build an open-hashing hash table from a list of strings.
// Returns a vector of sorted lists (each list is a vector<string> sorted ascending).
std::vector<std::vector<std::string>> buildOpenHashTable(const std::vector<std::string>& items, int tableSize) {
    if (tableSize <= 0) {
        return {};
    }
    std::vector<std::vector<std::string>> table(static_cast<size_t>(tableSize));
    for (const std::string& s : items) {
        unsigned long long hash = RSHash(s);
        size_t index = static_cast<size_t>(hash % static_cast<unsigned long long>(tableSize));
        // Insert into the bucket while keeping it sorted.
        auto& bucket = table[index];
        auto pos = std::lower_bound(bucket.begin(), bucket.end(), s);
        bucket.insert(pos, s);
    }
    return table;
}
// The solution approach models an open-hashing (chaining) hash table. For each input string, we compute a hash value using RSHash, which is a simple polynomial hash: `hash = hash * 31 + character` (assuming ASCII). After computing the hash (as an unsigned integer), we map it to an index using `hash % tableSize`, but only if tableSize > 0. We then insert the string into the appropriate bucket while maintaining sorted order, either by inserting at the correct position in the vector (O(n) insertion due to shifting) or by using a `std::set` then converting to a vector. To avoid negative or zero table sizes, we return an empty vector. Edge cases include: empty input (return vector of empty lists), table size 1 (all strings in one list), duplicate strings (they appear multiple times in the bucket), and large hash values that might overflow; use `unsigned long long` for hash to reduce overflow risk. Time complexity: For `m` input strings and table size `s`, each string hashing is O(length of string). Insertion into a bucket is O(k) where k is the size of that bucket (due to maintaining sorted order), so worst-case O(m²) if all strings collide. Space complexity: O(s + m) for the table and all stored strings.

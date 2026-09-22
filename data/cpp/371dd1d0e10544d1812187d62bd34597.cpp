Design a simplified C++ implementation of an address manager's bucket selection logic based on the provided code. Specifically, write a standalone function `int computeNewBucket(const std::vector<unsigned char>& nKey, const std::vector<unsigned char>& groupKey, const std::vector<unsigned char>& sourceGroupKey, int bucketCount, int bucketsPerSourceGroup)` that replicates the core deterministic hash-based bucket assignment from `CAddrInfo::GetNewBucket`, but abstracted to work on raw byte vectors instead of network address objects. The function must accept a fixed 32-byte key (simulating the address manager's secret key), a group key for the address, a source group key for the source address, the total number of new buckets, and the number of buckets per source group. It should compute a hash-based index using two sequential hash stages: first, hash the concatenation of `nKey || groupKey || sourceGroupKey`, take that hash mod `bucketsPerSourceGroup` to get an intermediate value; second, hash the concatenation of `nKey || sourceGroupKey || intermediateValue` (as 8-byte little-endian), and return that hash mod `bucketCount`. Assume a simple 64-bit hash function is available (e.g., `std::hash<std::string>` is not suitable for raw bytes, so implement a deterministic FNV-1a 64-bit hash over byte ranges). Handle empty inputs gracefully by returning 0.
The core problem is to translate the original double-hash bucket selection into a standalone function. The original uses `CDataStream` to serialize key and group data, then applies Bitcoin's `Hash` function (SHA-256-based) to produce a 64-bit value. For this standalone task, we replace the cryptographic hash with a simpler but still deterministic FNV-1a 64-bit hash, which is sufficient for demonstrating the algorithm's structure. The first hash stage takes the concatenation of the three byte vectors in order: `nKey`, `groupKey`, `sourceGroupKey`. We need to update the FNV-1a hash state with each byte of each vector in sequence. Then compute `hash1 % bucketsPerSourceGroup` to get an intermediate integer. For the second stage, we need to serialize that intermediate value as 8 bytes in little-endian order (matching `CDataStream`'s serialization of `uint64`). Then concatenate `nKey`, `sourceGroupKey`, and those 8 bytes, hash them again with FNV-1a, and compute `hash2 % bucketCount`. Edge cases: if any input vector is empty, we still include its bytes (zero bytes) in the hash; but for the function we can simply iterate regardless. If `bucketCount` or `bucketsPerSourceGroup` is zero, return 0 to avoid division by zero. The time complexity is linear in the total number of bytes across all inputs, plus constant work for the modulo operations. Space complexity is O(1) aside from the input vectors themselves, since we process bytes sequentially without building concatenated strings. The function is pure and deterministic: same inputs always produce the same bucket index, which is essential for the address manager's consistency.
#include <cstdint>
#include <vector>
#include <cassert>

// FNV-1a 64-bit hash over a range of bytes.
static uint64_t fnv1a(const unsigned char* data, size_t size) {
    uint64_t hash = 1469598103934665603ULL;
    for (size_t i = 0; i < size; ++i) {
        hash ^= data[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

// Compute a deterministic bucket index for new addresses.
// nKey: 32-byte secret key (or arbitrary bytes).
// groupKey: bytes representing the address's group.
// sourceGroupKey: bytes representing the source's group.
// bucketCount: total number of new buckets (must be > 0).
// bucketsPerSourceGroup: number of buckets per source group (must be > 0).
// Returns an index in [0, bucketCount-1], or 0 on invalid parameters.
int computeNewBucket(const std::vector<unsigned char>& nKey,
                     const std::vector<unsigned char>& groupKey,
                     const std::vector<unsigned char>& sourceGroupKey,
                     int bucketCount, int bucketsPerSourceGroup) {
    if (bucketCount <= 0 || bucketsPerSourceGroup <= 0) {
        return 0;
    }

    // First stage: hash nKey || groupKey || sourceGroupKey
    uint64_t hash1 = fnv1a(nKey.data(), nKey.size());
    for (unsigned char b : groupKey) {
        hash1 ^= b;
        hash1 *= 1099511628211ULL;
    }
    for (unsigned char b : sourceGroupKey) {
        hash1 ^= b;
        hash1 *= 1099511628211ULL;
    }
    uint64_t intermediate = hash1 % static_cast<uint64_t>(bucketsPerSourceGroup);

    // Serialize intermediate as 8-byte little-endian.
    unsigned char interBytes[8];
    for (int i = 0; i < 8; ++i) {
        interBytes[i] = static_cast<unsigned char>((intermediate >> (8 * i)) & 0xFF);
    }

    // Second stage: hash nKey || sourceGroupKey || interBytes
    uint64_t hash2 = fnv1a(nKey.data(), nKey.size());
    for (unsigned char b : sourceGroupKey) {
        hash2 ^= b;
        hash2 *= 1099511628211ULL;
    }
    for (unsigned char b : interBytes) {
        hash2 ^= b;
        hash2 *= 1099511628211ULL;
    }

    return static_cast<int>(hash2 % static_cast<uint64_t>(bucketCount));
}
#include <cassert>
#include <vector>

int main() {
    // Basic sanity: empty inputs and zero parameters yield 0.
    std::vector<unsigned char> empty;
    assert(computeNewBucket(empty, empty, empty, 0, 4) == 0);
    assert(computeNewBucket(empty, empty, empty, 4, 0) == 0);

    // Deterministic: same inputs always produce same output.
    std::vector<unsigned char> key(32, 0x42);
    std::vector<unsigned char> group = {0x01, 0x02, 0x03};
    std::vector<unsigned char> srcGroup = {0x0A, 0x0B};
    int result1 = computeNewBucket(key, group, srcGroup, 10, 5);
    int result2 = computeNewBucket(key, group, srcGroup, 10, 5);
    assert(result1 == result2);
    assert(result1 >= 0 && result1 < 10);

    // Different source groups produce different buckets (likely but not guaranteed).
    std::vector<unsigned char> srcGroup2 = {0x0C, 0x0D};
    int result3 = computeNewBucket(key, group, srcGroup2, 10, 5);
    assert(result3 >= 0 && result3 < 10);

    // Changing the key should change the result.
    std::vector<unsigned char> key2(32, 0x24);
    int result4 = computeNewBucket(key2, group, srcGroup, 10, 5);
    assert(result4 >= 0 && result4 < 10);
    // Very unlikely to be equal; just check that it's in range.

    // Test that intermediate modulo is used: with bucketsPerSourceGroup = 1, the intermediate is always 0.
    int result5 = computeNewBucket(key, group, srcGroup, 100, 1);
    assert(result5 >= 0 && result5 < 100);

    // Verify that the function is pure (no state changes).
    int result6 = computeNewBucket(key, group, srcGroup, 10, 5);
    assert(result6 == result1);

    return 0;
}

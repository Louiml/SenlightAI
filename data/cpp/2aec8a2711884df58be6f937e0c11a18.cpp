// Write a C++ function `map<uint256, CProduct> mergeProductMaps(const map<uint256, CProduct>& mapA, const map<uint256, CProduct>& mapB)` that takes two maps of `CProduct` objects keyed by `uint256` hash values, and returns a new map containing the union of all entries from both inputs. When the same hash key exists in both maps, the product with the higher `nSequence` field should be kept (i.e., the newer product wins). If both have equal `nSequence`, the product from `mapB` should take precedence. The input maps must remain unchanged. You may assume `uint256` supports `operator<` for ordered map ordering, and `CProduct` has public members `uint32_t nSequence` and a default copy constructor. Do not modify the originals; create and return a new merged map.
#include <cassert>
#include <map>

// Include the solution code above (or link appropriately).
// Helper to create a uint256 from an integer for testing.
uint256 makeUint256(int value) {
    uint256 h;
    for (int i = 0; i < 32; ++i) h.data[i] = (unsigned char)((value >> (i*8)) & 0xFF);
    return h;
}

int main() {
    // Test 1: disjoint maps
    std::map<uint256, CProduct> mapA, mapB;
    mapA[makeUint256(1)] = {10};
    mapA[makeUint256(3)] = {30};
    mapB[makeUint256(2)] = {20};
    auto merged = mergeProductMaps(mapA, mapB);
    assert(merged.size() == 3);
    assert(merged[makeUint256(1)].nSequence == 10);
    assert(merged[makeUint256(2)].nSequence == 20);
    assert(merged[makeUint256(3)].nSequence == 30);

    // Test 2: overlapping keys, pick higher sequence
    mapA.clear(); mapB.clear();
    mapA[makeUint256(1)] = {50};
    mapA[makeUint256(2)] = {100};
    mapB[makeUint256(2)] = {200};
    mapB[makeUint256(3)] = {300};
    merged = mergeProductMaps(mapA, mapB);
    assert(merged.size() == 3);
    assert(merged[makeUint256(1)].nSequence == 50);
    assert(merged[makeUint256(2)].nSequence == 200); // B wins because higher seq
    assert(merged[makeUint256(3)].nSequence == 300);

    // Test 3: equal sequence -> prefer from mapB
    mapA.clear(); mapB.clear();
    mapA[makeUint256(7)] = {42};
    mapB[makeUint256(7)] = {42};
    merged = mergeProductMaps(mapA, mapB);
    assert(merged.size() == 1);
    assert(merged[makeUint256(7)].nSequence == 42);
    // To verify B was chosen, we could add a different field, but here both are identical.

    // Test 4: one map empty
    mapA.clear(); mapB.clear();
    mapA[makeUint256(5)] = {55};
    merged = mergeProductMaps(mapA, mapB);
    assert(merged.size() == 1);
    assert(merged[makeUint256(5)].nSequence == 55);

    // Test 5: both empty
    mapA.clear(); mapB.clear();
    merged = mergeProductMaps(mapA, mapB);
    assert(merged.empty());

    // Test 6: A has higher sequence for duplicate key
    mapA.clear(); mapB.clear();
    mapA[makeUint256(9)] = {999};
    mapB[makeUint256(9)] = {1};
    merged = mergeProductMaps(mapA, mapB);
    assert(merged.size() == 1);
    assert(merged[makeUint256(9)].nSequence == 999);

    // Test 7: originals unchanged
    mapA.clear(); mapB.clear();
    mapA[makeUint256(1)] = {10};
    mapB[makeUint256(1)] = {20};
    auto aBefore = mapA;
    auto bBefore = mapB;
    merged = mergeProductMaps(mapA, mapB);
    assert(mapA == aBefore);
    assert(mapB == bBefore);

    return 0;
}
#include <map>

// Assume these types are defined elsewhere (as in the snippet context).
// Here we define placeholder minimal versions for completeness.
struct uint256 {
    unsigned char data[32];
    bool operator<(const uint256& other) const {
        return std::lexicographical_compare(data, data+32, other.data, other.data+32);
    }
    bool operator==(const uint256& other) const {
        return std::equal(data, data+32, other.data);
    }
};

struct CProduct {
    uint32_t nSequence;
    // other fields omitted for brevity; default copy constructor works.
};

// Merge two product maps: for duplicate keys, keep the product with higher nSequence.
// If equal nSequence, prefer product from mapB.
std::map<uint256, CProduct> mergeProductMaps(
    const std::map<uint256, CProduct>& mapA,
    const std::map<uint256, CProduct>& mapB)
{
    std::map<uint256, CProduct> result;

    auto itA = mapA.begin();
    auto itB = mapB.begin();
    auto hint = result.begin(); // hint for efficient insertion at end

    while (itA != mapA.end() && itB != mapB.end()) {
        if (itA->first < itB->first) {
            hint = result.insert(hint, *itA);
            ++itA;
        } else if (itB->first < itA->first) {
            hint = result.insert(hint, *itB);
            ++itB;
        } else {
            // Same key: choose based on nSequence, preference to B on tie.
            const CProduct& chosen = (itB->second.nSequence >= itA->second.nSequence)
                                        ? itB->second : itA->second;
            hint = result.insert(hint, std::make_pair(itA->first, chosen));
            ++itA;
            ++itB;
        }
    }

    // Append remaining entries from either map.
    while (itA != mapA.end()) {
        hint = result.insert(hint, *itA);
        ++itA;
    }
    while (itB != mapB.end()) {
        hint = result.insert(hint, *itB);
        ++itB;
    }

    return result;
}
// The task requires merging two ordered maps (`std::map`) by key, with a conflict resolution rule based on `nSequence`. Since `std::map` keeps keys sorted, the simplest efficient approach is to iterate over both maps simultaneously, leveraging their sorted order to merge in linear time.
//
// We can use two iterators (`itA` and `itB`) pointing to the begins of the two maps. At each step, compare the keys:
// - If `itA->first < itB->first`, insert `*itA` into the result and increment `itA`.
// - If `itB->first < itA->first`, insert `*itB` into the result and increment `itB`.
// - If equal keys, choose which product to keep: if `itA->second.nSequence >= itB->second.nSequence`, keep `itA`'s product (since equal sequence prefers B, but this condition keeps A only if strictly greater; else keep B). More precisely, keep `itB`'s product if `itB->second.nSequence >= itA->second.nSequence`; otherwise keep `itA`. Then insert the chosen product and advance both iterators.
//
// After one iterator exhausts, append all remaining entries from the other map. Since maps are sorted, this yields a correctly ordered result map. Edge cases: empty input maps, identical keys with equal or differing sequences, and one map being a subset of the other.
//
// Time complexity is O(n + m) where n and m are the sizes of the input maps, because each element is visited once in the merge and insertion into the result map costs O(log(n+m)) per element, but since we insert in sorted order, we can use `std::map::insert` with a hint at the end to achieve O(1) amortized per insert. Total time O(n+m) with hint, or O((n+m) log(n+m)) without. Space complexity O(n+m) for the result map.

Write a C++ function `bool compressEnvelope(std::vector<Key>& keys)` that takes a vector of keyframes (each key has `time` and `value` fields) sorted strictly by time, and removes all intermediate keys if all keys in the vector are identical in *value* (i.e., the envelope is constant). The function should return `true` if it modified the vector (i.e., removed intermediate keys), and `false` if the vector was already minimal (fewer than 3 keys or contains at least one differing value). If the vector has more than 2 keys and all values are equal, keep only the first and last keys (preserving their original times), delete the rest, and return `true`. The function must handle empty and single-key vectors without modification. Assume no duplicate times. Do not include a `main` function in your solution—only the function.

The core problem is to detect whether a sorted sequence of keyframes is constant in value. Iterate through the vector, comparing each key's value to the first key's value. If any value differs, return `false` immediately without modification. If the vector has 0, 1, or 2 keys, it is already minimal, so return `false`. If the vector has more than 2 keys and all values are equal, construct a new vector containing only the first and last keys (copying their time and value), then replace the original vector's contents with those two keys (or resize and assign). This is O(n) time and O(1) auxiliary space, since we avoid creating large copies; we can simply erase all but first and last elements in the existing vector. Edge cases include empty vectors, vectors with exactly two keys (should not be modified), and vectors where all values match but there are only 2 keys (no modification needed). The operation is in-place and preserves key order.

#include <vector>
#include <algorithm> // for std::equal, though we can do manually

// Key structure assumed to have time and value.
struct Key {
    float time;
    float value;
};

// Compress a constant-valued envelope: keep only first and last keys if all values equal and size > 2.
// Returns true if the vector was modified (keys removed), false otherwise.
bool compressEnvelope(std::vector<Key>& keys) {
    // Handle trivial cases: 0, 1, or 2 keys are already minimal.
    if (keys.size() <= 2) {
        return false;
    }

    // Check if all values are equal to the first key's value.
    const float firstValue = keys.front().value;
    for (size_t i = 1; i < keys.size(); ++i) {
        if (keys[i].value != firstValue) {
            return false; // Not constant; no modification.
        }
    }

    // All values equal and more than 2 keys: remove all but first and last.
    // Erase elements from index 1 to size-2 (inclusive).
    keys.erase(keys.begin() + 1, keys.end() - 1);
    return true;
}

#include <cassert>
#include <vector>

// Key structure must match the solution's definition; include it here.
struct Key {
    float time;
    float value;
};

// Declaration of the function under test.
bool compressEnvelope(std::vector<Key>& keys);

int main() {
    // Test 1: Empty vector, no modification.
    std::vector<Key> empty;
    assert(compressEnvelope(empty) == false);
    assert(empty.empty());

    // Test 2: Single key, no modification.
    std::vector<Key> single = {{0.0f, 5.0f}};
    assert(compressEnvelope(single) == false);
    assert(single.size() == 1 && single[0].value == 5.0f);

    // Test 3: Two keys, even if same value, no modification.
    std::vector<Key> two = {{0.0f, 1.0f}, {1.0f, 1.0f}};
    assert(compressEnvelope(two) == false);
    assert(two.size() == 2);

    // Test 4: More than 2 keys but not constant → no modification.
    std::vector<Key> varying = {{0.0f, 0.0f}, {1.0f, 1.0f}, {2.0f, 0.0f}};
    assert(compressEnvelope(varying) == false);
    assert(varying.size() == 3);

    // Test 5: More than 2 keys, all constant → compress to 2 keys.
    std::vector<Key> constant = {{0.0f, 7.0f}, {1.0f, 7.0f}, {2.0f, 7.0f}, {3.0f, 7.0f}};
    assert(compressEnvelope(constant) == true);
    assert(constant.size() == 2);
    assert(constant[0].time == 0.0f && constant[0].value == 7.0f);
    assert(constant[1].time == 3.0f && constant[1].value == 7.0f);

    // Test 6: Exactly 3 keys, all constant → compress.
    std::vector<Key> triple = {{0.0f, 2.0f}, {0.5f, 2.0f}, {1.0f, 2.0f}};
    assert(compressEnvelope(triple) == true);
    assert(triple.size() == 2);
    assert(triple[0].time == 0.0f && triple[1].time == 1.0f);

    // Test 7: After compression, the returned vector should stay sorted and preserve endpoints.
    std::vector<Key> large = {{-1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {2.0f, 0.0f}, {10.0f, 0.0f}};
    assert(compressEnvelope(large) == true);
    assert(large.size() == 2);
    assert(large[0].time == -1.0f && large[1].time == 10.0f);

    return 0;
}

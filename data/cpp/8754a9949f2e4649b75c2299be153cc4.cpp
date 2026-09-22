/*
Implement a C++ function that simulates the logic of searching a B+ tree for a key and detecting whether a leaf underflow (or overflow redistribution) occurs, but simplified to work with a fixed-size array representing a sorted list of keys and associated values. Specifically, write a function `findKeyWithRedistribution(const std::vector<int>& keys, const std::vector<std::string>& values, int targetKey)` that returns a `std::pair<bool, std::string>` where the boolean indicates whether a redistribution would be needed (simulating the return of a new child pointer from the recursive search), and the string is the value associated with the target key if found, or an empty string otherwise. The redistribution condition is triggered if the target key is greater than the last key in the array AND the array size equals a threshold `MAX_LEAF_SIZE = 4`. If redistribution is triggered, you must simulate splitting the array into two: the first half (keys[0..1]) stays, the second half (keys[2..3]) moves to a new "sibling", and the discriminator (the smallest key in the sibling) is returned as part of the result. However, for the purpose of this standalone task, the function only needs to return `{true, value}` when redistribution triggers, and `{false, value}` otherwise (where value is found via a linear search). The input vectors are guaranteed to be sorted by key and have equal lengths, with no duplicate keys. The function must be `const`-correct and handle the case where the target key is not found (returning an empty string for the value).
*/

#include <string>
#include <vector>
#include <utility>

// Simulates a B+ tree leaf search and determines if a redistribution would be needed.
// Returns {needsRedistribution, value} where value is the string associated with targetKey
// or an empty string if not found. Redistribution is triggered when targetKey > last key
// and the leaf size equals MAX_LEAF_SIZE (4).
std::pair<bool, std::string> findKeyWithRedistribution(
    const std::vector<int>& keys,
    const std::vector<std::string>& values,
    int targetKey
) {
    constexpr size_t MAX_LEAF_SIZE = 4;
    
    bool needsRedistribution = false;
    std::string resultValue;

    // Check redistribution condition based on the original leaf logic.
    if (!keys.empty() && keys.size() == MAX_LEAF_SIZE && targetKey > keys.back()) {
        needsRedistribution = true;
    }

    // Linear search for the key (simplified from the B+ tree node scan).
    for (size_t i = 0; i < keys.size(); ++i) {
        if (keys[i] == targetKey) {
            resultValue = values[i];
            break;
        }
    }

    return {needsRedistribution, resultValue};
}

#include <cassert>
#include <string>
#include <vector>
#include <utility>

int main() {
    // Test 1: Key found, no redistribution (size < 4 or key <= max).
    std::vector<int> keys1 = {10, 20, 30};
    std::vector<std::string> vals1 = {"a", "b", "c"};
    auto res1 = findKeyWithRedistribution(keys1, vals1, 20);
    assert(res1.first == false);
    assert(res1.second == "b");

    // Test 2: Key not found, no redistribution.
    auto res2 = findKeyWithRedistribution(keys1, vals1, 25);
    assert(res2.first == false);
    assert(res2.second.empty());

    // Test 3: Full node, key greater than max -> redistribution triggers, key not found.
    std::vector<int> keys3 = {1, 2, 3, 4};
    std::vector<std::string> vals3 = {"w", "x", "y", "z"};
    auto res3 = findKeyWithRedistribution(keys3, vals3, 5);
    assert(res3.first == true);
    assert(res3.second.empty());

    // Test 4: Full node, key equal to max -> no redistribution, key found.
    auto res4 = findKeyWithRedistribution(keys3, vals3, 4);
    assert(res4.first == false);
    assert(res4.second == "z");

    // Test 5: Full node, key less than max -> no redistribution, key found.
    auto res5 = findKeyWithRedistribution(keys3, vals3, 2);
    assert(res5.first == false);
    assert(res5.second == "x");

    // Test 6: Empty vectors -> no redistribution, no value.
    std::vector<int> emptyKeys;
    std::vector<std::string> emptyVals;
    auto res6 = findKeyWithRedistribution(emptyKeys, emptyVals, 42);
    assert(res6.first == false);
    assert(res6.second.empty());

    // Test 7: Duplicate keys not allowed per spec, but ensure no crash with size > 4.
    std::vector<int> keys7 = {1, 2, 3, 4, 5};
    std::vector<std::string> vals7 = {"a", "b", "c", "d", "e"};
    auto res7 = findKeyWithRedistribution(keys7, vals7, 6);
    assert(res7.first == false);  // size != 4
    assert(res7.second.empty());

    return 0;
}

// The solution directly maps the given B+ tree search logic to a simpler array-based model. We first check if the target key is greater than the last element of the array and if the array length equals the threshold (4). If so, we set a boolean flag `needsRedistribution = true`. Then we perform a linear search over the array for the target key. If found, we copy the corresponding value into a string; if not, we leave the value empty. The function returns a pair `{needsRedistribution, value}`. Edge cases: empty input arrays (should return `{false, ""}` since no key can match and redistribution condition requires a non-empty last element), target key smaller than first element (no redistribution, possibly not found), target key equal to last element (no redistribution because condition is strict `>`). Time complexity is O(n) due to linear search, and space complexity is O(1) auxiliary (ignoring the returned string copy). The redistribution condition mimics the original code's check `if (!hasOvfl(node) || key <= getMax(node))` – here we simplify by assuming no overflow, so redistribution only happens when key > max and the node is "full" (size 4). The returned boolean corresponds to the `newChld` non-null case in the original, which triggers a new root creation in the parent call.

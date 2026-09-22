Write a C++ function named `hashTableAfterRemoval` that simulates the hash table operations shown in the provided code snippet. Given the table size `m`, an initial list of positive integer keys to insert (terminated by `0`), and a single key to remove, the function must build the hash table using the given double hashing formula `(h(k) + i * h(k)) mod m` where `h(k) = k mod m` (with negative results adjusted by adding `m`), insert keys in the given order using linear probing for collision resolution (with a deleted marker status of 2 and an empty status of 0), and then remove the specified key. Finally, return a `std::vector<int>` containing the table contents after the removal, with empty or deleted slots represented by the value `-1`. If the removal key is not found, the table remains unchanged. The function must handle cases where the insertion fails (table full) by skipping that key and continuing with the next. The input keys are read until a `0` is encountered, and the removal key is guaranteed to be positive. The maximum table size to support is 100.
The solution must replicate the exact behavior of the `Hash` struct from the snippet. The core algorithm: first initialize a table of size `m` with all keys set to `-1` and statuses set to `0` (empty). For each key `k` (stopping at `0`), attempt to insert using the double hash function `make(k, i, m) = (h(k) + i * h(k)) % m`, where `h(k) = k % m` (adjusted to non-negative). For `i` starting at 0, compute the probe index; if the slot's status is not `1` (occupied), place the key there and mark status `1`; otherwise increment `i`. If `i` reaches `m`, insertion fails and the key is skipped. After all insertions, search for the removal key using the same probing sequence; if found, set its key to `-1` and status to `2` (deleted). The final table contents are collected as a vector of keys, with `-1` for empty or deleted slots. Edge cases: a key may collide and require multiple probes; after removal, searches for other keys still work because status `2` is not `0` and the probing continues. Time complexity: each insertion or search is O(m) in the worst case, and the total is O(n*m) for `n` insertions plus one removal/search, but typically O(n+m). Space complexity is O(m) for the table plus O(m) for the returned vector.
#include <vector>

// Simulates the hash table operations and returns the table contents after removal.
std::vector<int> hashTableAfterRemoval(int m, const std::vector<int>& keysToInsert, int removalKey) {
    std::vector<int> table(m, -1);
    std::vector<int> status(m, 0); // 0 = empty, 1 = occupied, 2 = deleted

    // Insert each key until 0 (not included) or until table full.
    for (int k : keysToInsert) {
        if (k == 0) break;
        int i = 0;
        bool inserted = false;
        while (i < m) {
            int h = k % m;
            if (h < 0) h += m;
            int j = (h + i * h) % m;
            if (status[j] != 1) {
                table[j] = k;
                status[j] = 1;
                inserted = true;
                break;
            }
            i++;
        }
        // If not inserted, key is skipped silently.
    }

    // Remove the specified key if present.
    int i = 0;
    while (i < m) {
        int h = removalKey % m;
        if (h < 0) h += m;
        int j = (h + i * h) % m;
        if (table[j] == removalKey) {
            table[j] = -1;
            status[j] = 2;
            break;
        }
        if (status[j] == 0) break; // stop if empty slot found
        i++;
    }

    return table;
}
#include <cassert>
#include <vector>

// The solution function is declared here for completeness (normally included from header).
std::vector<int> hashTableAfterRemoval(int m, const std::vector<int>& keysToInsert, int removalKey);

int main() {
    // Example from the snippet: m=5, insert 1,2,3,4,5 then remove 3.
    std::vector<int> result1 = hashTableAfterRemoval(5, {1,2,3,4,5}, 3);
    assert((result1 == std::vector<int>{1,2,-1,4,5})); // 3 removed, others in same slots.

    // m=3, insert 3,6,9 (all collide to index 0, then probe 0,1,2) then remove 6.
    std::vector<int> result2 = hashTableAfterRemoval(3, {3,6,9}, 6);
    assert((result2 == std::vector<int>{3,-1,9})); // 6 at index 1 is removed.

    // Removal key not found: table unchanged.
    std::vector<int> result3 = hashTableAfterRemoval(4, {1,5,9}, 100);
    assert((result3 == std::vector<int>{1,5,9,-1})); // all inserted.

    // Insertion failure when table full: skip key.
    std::vector<int> result4 = hashTableAfterRemoval(2, {1,2,3}, 999); // only 1 and 2 fit, 3 skipped
    assert((result4 == std::vector<int>{1,2}));

    // Removal after deletion marker: deleted slots do not stop search.
    std::vector<int> result5 = hashTableAfterRemoval(5, {1,6,11}, 1); // insert 1 at 0, 6 at 1, 11 at 2
    // remove 1, then remove 6 (should find 6 probing from index 1)
    result5 = hashTableAfterRemoval(5, {1,6,11}, 6);
    assert((result5 == std::vector<int>{-1,-1,11,-1,-1})); // both 1 and 6 removed.

    // Single element table.
    std::vector<int> result6 = hashTableAfterRemoval(1, {7}, 7);
    assert((result6 == std::vector<int>{-1}));

    return 0;
}

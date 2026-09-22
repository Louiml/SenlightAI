Implement a C++ function named `LinearProbingHashTable` simulation that takes an initial capacity, a list of keys to insert, a list of keys to search for, and a list of keys to delete, and returns a vector of booleans in the order: for each search operation, whether the key was found at the time of the search, given that all insert and delete operations are performed sequentially in the order they appear in their respective lists (first all inserts, then all deletes, then all searches). The hash table uses linear probing with open addressing, where an empty slot is marked with -1 and a deleted slot is marked with -2. If an insert encounters a full table, it fails (no change). If inserting a duplicate key, it fails. If deleting a non-existent key, it fails. The function must return a `std::vector<bool>` where the i-th element corresponds to the success of the i-th search. The initial table is initialized with -1 in all slots. The hash function is `key % capacity`. All keys are non-negative integers.
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// Placeholder declaration to allow compilation; in real use, include the solution.
std::vector<bool> LinearProbingHashTable(int, const std::vector<int>&, const std::vector<int>&, const std::vector<int>&);

int main() {
    // Test 1: Insert 49,56,72 into capacity 7, search 56 -> true, search 99 -> false.
    {
        std::vector<bool> res = LinearProbingHashTable(7, {49,56,72}, {}, {56,99});
        assert(res.size() == 2);
        assert(res[0] == true);
        assert(res[1] == false);
    }

    // Test 2: Insert duplicates fail, search after duplicate insert still finds original.
    {
        std::vector<bool> res = LinearProbingHashTable(5, {1,1,2}, {}, {1,2});
        assert(res == std::vector<bool>({true, true}));
    }

    // Test 3: Delete a key, then search says false; delete non-existent does nothing.
    {
        std::vector<bool> res = LinearProbingHashTable(5, {1,2,3}, {2,99}, {1,2,3});
        assert(res.size() == 3);
        assert(res[0] == true);
        assert(res[1] == false);
        assert(res[2] == true);
    }

    // Test 4: Full table insert fails, search existing keys works.
    {
        std::vector<bool> res = LinearProbingHashTable(3, {1,2,3,4}, {}, {1,4});
        assert(res == std::vector<bool>({true, false})); // 4 not inserted because full.
    }

    // Test 5: Reuse deleted slots after deletion and insertion.
    {
        std::vector<bool> res = LinearProbingHashTable(5, {1,3,5}, {1}, {1,5,3});
        assert(res == std::vector<bool>({false, true, true}));
    }

    // Test 6: Wrap-around probing with collision.
    {
        // capacity=2, keys 2 (hash 0) and 4 (hash 0) cause collision.
        std::vector<bool> res = LinearProbingHashTable(2, {2,4}, {}, {2,4});
        assert(res == std::vector<bool>({true, true}));
    }

    // Test 7: Empty operations.
    {
        std::vector<bool> res = LinearProbingHashTable(5, {}, {}, {});
        assert(res.empty());
    }

    // Test 8: Search on empty table returns false.
    {
        std::vector<bool> res = LinearProbingHashTable(5, {}, {}, {1,2});
        assert(res == std::vector<bool>({false, false}));
    }

    // Test 9: Delete then reinsert same key.
    {
        std::vector<bool> res = LinearProbingHashTable(5, {7}, {7}, {7});
        assert(res == std::vector<bool>({false})); // Deleted before search.
    }

    // Test 10: Multiple deletes and inserts interleaved order (all inserts first, then deletes, then searches)
    {
        std::vector<bool> res = LinearProbingHashTable(10, {10,20,30,40}, {10,30}, {10,20,30,40});
        assert(res == std::vector<bool>({false, true, false, true}));
    }

    return 0;
}
#include <vector>

// Simulate a linear probing hash table with operations: insert, delete, search.
// Returns a vector of booleans for the search results after performing inserts and deletes.
std::vector<bool> LinearProbingHashTable(int capacity, 
                                         const std::vector<int>& inserts, 
                                         const std::vector<int>& deletes, 
                                         const std::vector<int>& searches) {
    // Initialize table with -1 (empty).
    std::vector<int> table(capacity, -1);
    int size = 0;

    // Helper lambda for hashing.
    auto hash = [capacity](int key) { return key % capacity; };

    // Process inserts in order.
    for (int key : inserts) {
        if (size == capacity) {
            continue; // Table full, insert fails.
        }
        int i = hash(key);
        // Probe for either an empty slot (-1) or a deleted slot (-2) or the key itself.
        while (table[i] != -1 && table[i] != -2 && table[i] != key) {
            i = (i + 1) % capacity;
        }
        if (table[i] == key) {
            continue; // Duplicate, insert fails.
        }
        // Insert into the first available slot.
        table[i] = key;
        ++size;
    }

    // Process deletes in order.
    for (int key : deletes) {
        int h = hash(key);
        int i = h;
        // Probe until finding an empty slot or returning to start.
        while (table[i] != -1) {
            if (table[i] == key) {
                table[i] = -2; // Mark as deleted.
                --size;
                break;
            }
            i = (i + 1) % capacity;
            if (i == h) {
                break; // Wrapped around and did not find key.
            }
        }
    }

    // Process searches, record results.
    std::vector<bool> results;
    results.reserve(searches.size());
    for (int key : searches) {
        int h = hash(key);
        int i = h;
        bool found = false;
        // Probe until finding an empty slot or returning to start.
        while (table[i] != -1) {
            if (table[i] == key) {
                found = true;
                break;
            }
            i = (i + 1) % capacity;
            if (i == h) {
                break; // Wrapped around and did not find key.
            }
        }
        results.push_back(found);
    }

    return results;
}
// We need to simulate the described open-addressing hash table with linear probing. The main algorithm is straightforward: create a dynamic array of integers of size `capacity`, initialize all elements to -1 (empty). Process insert operations in order: if the table is full (size == capacity) or the key already exists (found during probing), return false for that insert and move on. Otherwise, probe from `hash(key)` linearly, skipping slots with -1 (empty) and -2 (deleted), stopping at the first slot that is either -1 or -2. Place the key there and increment size. For delete operations: probe from `hash(key)` linearly until we find an empty slot (-1) or wrap around to the starting index. If we find the key, set that slot to -2 and decrement size. If we never find it (encounter -1 or return to start), the delete fails. For search operations: probe from `hash(key)` linearly until we find an empty slot (-1) or wrap around. If we find the key, return true; otherwise false. Important edge cases: full table, duplicate keys, deleting then inserting (to reuse deleted slots), and the wrap-around detection by remembering the starting index and stopping if we return to it. Time complexity is O(n) per operation in the worst case due to probing, but average is O(1) under low load factor. Space complexity is O(capacity) for the table.

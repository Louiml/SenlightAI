/*
Implement a C++ function `analyzeHashProbe` that simulates linear probing in a fixed-size hash table and returns a `std::vector<int>` recording the performance of each key insertion and search in order of operations. The function takes three parameters: an integer `tableSize` (a positive integer representing the hash table capacity), a `std::vector<int> keysToInsert` (keys to insert sequentially using the hash function `key % tableSize` and linear probing with tombstones: empty slots are `-1`, deleted slots are `-2`), and a `std::vector<int> keysToSearch` (keys to search sequentially). For each insertion, record the final index where the key is stored, or `-1` if insertion fails because the table is full (i.e., after probing all slots, no empty or deleted slot is found). For each search, record the index where the key is found, or `-1` if not found (including when the search terminates upon hitting an empty slot or after probing all positions). The returned vector must interleave results in the exact order: first all insertion results (in the order of `keysToInsert`), then all search results (in the order of `keysToSearch`). The function must not output anything to the console and must handle duplicate keys and keys that collide heavily. Assume the table starts with all slots empty (`-1`). Provide a self-contained free function (no `main`) with appropriate documentation and `const` correctness where applicable.
*/
#include <vector>

// Simulates linear probing hash table operations and returns results
// for each insertion (index stored or -1 if full) followed by each
// search (index found or -1 if not found).
std::vector<int> analyzeHashProbe(int tableSize,
                                  const std::vector<int>& keysToInsert,
                                  const std::vector<int>& keysToSearch) {
    // Table initialized with -1 meaning empty slot.
    std::vector<int> table(tableSize, -1);
    std::vector<int> results;

    // ---- Insertions ----
    for (int key : keysToInsert) {
        int start = key % tableSize;
        int index = -1;
        for (int i = 0; i < tableSize; ++i) {
            int pos = (start + i) % tableSize;
            if (table[pos] == -1 || table[pos] == -2) {
                index = pos;
                table[pos] = key;
                break;
            }
        }
        results.push_back(index); // -1 if table was full
    }

    // ---- Searches ----
    for (int key : keysToSearch) {
        int start = key % tableSize;
        int foundIndex = -1;
        for (int i = 0; i < tableSize; ++i) {
            int pos = (start + i) % tableSize;
            if (table[pos] == -1) {
                // Empty slot: key not present further along probe sequence.
                break;
            }
            if (table[pos] == key) {
                foundIndex = pos;
                break;
            }
            // tombstone (-2) is skipped and probing continues
        }
        results.push_back(foundIndex);
    }

    return results;
}
#include <cassert>
#include <vector>

// The solution function is declared here (or included from the solution file).
std::vector<int> analyzeHashProbe(int tableSize,
                                  const std::vector<int>& keysToInsert,
                                  const std::vector<int>& keysToSearch);

int main() {
    // Basic insertion and search with no collisions
    {
        std::vector<int> res = analyzeHashProbe(10, {15, 25, 35}, {15, 25, 50});
        std::vector<int> expected = {5, 5, 5, 5, 5, -1}; // insert indices 5,5,5 and search finds 5,5 and -1 for 50
        assert(res == expected);
    }

    // Collision handling with linear probing
    {
        // Keys 2, 12, 22 all hash to index 2 in a table of size 5
        std::vector<int> res = analyzeHashProbe(5, {2, 12, 22}, {2, 12, 22});
        // Insertions: 2 at idx2, 12 at idx3 (probing), 22 at idx4
        // Searches: find all at respective indices
        std::vector<int> expected = {2, 3, 4, 2, 3, 4};
        assert(res == expected);
    }

    // Tombstone handling: after removal, search continues past deleted slot
    // But our function doesn't remove; it only inserts and searches.
    // So test insertion over deleted slots that are pre-set manually? Not possible via function.
    // Skip that; instead test full table insertion failure.
    {
        // Table of size 3, insert 3 keys into distinct slots (e.g., 0,1,2) then a 4th key
        std::vector<int> res = analyzeHashProbe(3, {0, 3, 6, 9}, {9});
        // Insertions: 0->0, 3->0 (probe to 1), 6->0 (probe to 2), 9-> full -> -1
        // Search for 9 not found -> -1
        std::vector<int> expected = {0, 1, 2, -1, -1};
        assert(res == expected);
    }

    // Duplicate insertion and search
    {
        std::vector<int> res = analyzeHashProbe(5, {7, 7}, {7});
        // First 7 at idx2, second 7 probes to idx3 (since idx2 occupied)
        // Search for 7 will find first occurrence at idx2
        std::vector<int> expected = {2, 3, 2};
        assert(res == expected);
    }

    // Empty insert list, only searches
    {
        std::vector<int> res = analyzeHashProbe(4, {}, {1, 2, 3});
        std::vector<int> expected = {-1, -1, -1}; // nothing found
        assert(res == expected);
    }

    // Search with tombstones is not directly testable, but we can test
    // that search stops at an empty slot after a non-empty prefix
    {
        // Insert key 1 at index 1, then search for 5 (hashes to 1 but not present)
        std::vector<int> res = analyzeHashProbe(5, {1}, {5});
        // Insert 1 at idx1, search 5: check idx1=1 (not 5), idx2=-1 empty -> stop not found
        std::vector<int> expected = {1, -1};
        assert(res == expected);
    }

    return 0;
}
// The solution simulates a hash table with linear probing using a dynamically allocated array (or a `std::vector<int>`) of size `tableSize`. For each insertion, compute the initial index via `key % tableSize`. Probe sequentially with `(initialIndex + i) % tableSize` for `i = 0, 1, ...` until either: (a) an empty slot (`-1`) or a deleted slot (`-2`) is found—then place the key there and record the index; (b) `i` reaches `tableSize` meaning no available slot exists—record `-1` and optionally leave the table unchanged. For searches, probe similarly, but stop early if an empty slot is encountered (since with linear probing, keys are placed contiguously after their hash index, and an empty slot means the key cannot appear later in the probe sequence). If the key is found, record its index; if an empty slot is hit or all positions probed, record `-1`. Important edge cases: duplicate keys (inserting a duplicate will place it in a new slot because the table does not check for existing keys; the problem expects this behavior), tombstones (`-2`) are treated as available for insertion but not for search termination (search must continue past tombstones), and a full table (if all slots are occupied, insertion returns `-1`). Time complexity: each operation is `O(tableSize)` in the worst case due to probing; total is `O((I + S) * tableSize)` for `I` insertions and `S` searches. Space complexity: `O(tableSize)` for the table plus the output vector of size `I + S`, so `O(tableSize + I + S)`.

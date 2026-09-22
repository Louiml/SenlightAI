Implement a C++ function named `openAddressingHashGet` that simulates a hash table with open addressing and linear probing, using the exact same hash function, status array, key array, and value array as the provided snippet. The function must perform one of three operations on a string key and an optional integer value: `"insert"` (insert or update the key with the given value), `"find"` (return the value associated with the key, or a sentinel value `-1` if not present), or `"erase"` (remove the key, marking it as a dummy, and return the previous value or `-1` if not present). Use the global arrays `status`, `key`, and `val` as defined in the snippet (with `M = 1000003`, `a = 1000`, `EMPTY = -1`, `OCCUPY = 0`, `DUMMY = 1`), and assume that before any calls, the caller has initialized `status` to `EMPTY` via `fill(status, status+M, EMPTY)`. The function signature should be `int openAddressingHashGet(const std::string& op, const std::string& k, int v = 0)`. For `"find"` and `"erase"`, the `v` parameter is ignored. For `"insert"`, if the key already exists, update its value and return the old value; if it is new, insert and return `-1` (to indicate no previous value). The function must correctly handle collisions, deletions (dummy slots), and re-insertion after deletion, matching the behavior of the reference implementation.

// The solution uses the provided hash function `my_hash` (polynomial rolling hash with base 1000 modulo M) to compute an initial index. Linear probing is used to resolve collisions: we scan forward (wrapping around) until we find either an `EMPTY` slot (meaning the key is absent) or an `OCCUPY` slot with a matching key. For insertion, we first call `find` logic to see if the key exists; if yes, update value and return old. If not, we start from the hash index and scan for the first slot that is not `OCCUPY` (i.e., `EMPTY` or `DUMMY`) to place the new key. For deletion, we find the index; if found, mark as `DUMMY` and return the old value, else return `-1`. Edge cases: handling duplicate insert (update), erasing a non-existent key (no change), and reinserting after deletion (the dummy slot is reused). Complexity: each operation is average O(1) under low load factor, but worst-case O(M) if the table is nearly full; space is O(M) for arrays. The sentinel `-1` is safe because all values stored are non-negative integers in the test, but note that the reference uses `-1` only for “not found”; actual stored values could be anything, but the task restricts to non-negative to avoid ambiguity.

#include <string>
#include <vector>

const int M = 1000003;
const int a = 1000;
const int EMPTY = -1;
const int OCCUPY = 0;
const int DUMMY = 1;

// Global arrays as per specification (must be initialized externally)
extern int status[M];
extern std::string key[M];
extern int val[M];

// Hash function from the snippet
int my_hash(const std::string& s) {
    int h = 0;
    for (char x : s) {
        h = (h * a + x) % M;
    }
    return h;
}

// Return index of key k if present, else -1
int findIndex(const std::string& k) {
    int idx = my_hash(k);
    while (status[idx] != EMPTY) {
        if (status[idx] == OCCUPY && key[idx] == k) {
            return idx;
        }
        idx = (idx + 1) % M;
    }
    return -1;
}

// Perform insert, find, or erase as described.
// Returns:
//   insert: old value if key existed, else -1 (new insert)
//   find:   value if found, else -1
//   erase:  old value if found, else -1
int openAddressingHashGet(const std::string& op, const std::string& k, int v = 0) {
    if (op == "find") {
        int idx = findIndex(k);
        if (idx != -1) return val[idx];
        return -1;
    }
    if (op == "erase") {
        int idx = findIndex(k);
        if (idx != -1) {
            int old = val[idx];
            status[idx] = DUMMY;
            return old;
        }
        return -1;
    }
    if (op == "insert") {
        int idx = findIndex(k);
        if (idx != -1) {
            int old = val[idx];
            val[idx] = v;
            return old;
        }
        // Insert new key
        idx = my_hash(k);
        while (status[idx] == OCCUPY) {
            idx = (idx + 1) % M;
        }
        key[idx] = k;
        val[idx] = v;
        status[idx] = OCCUPY;
        return -1;
    }
    return -1; // invalid operation treated as no-op
}

#include <cassert>
#include <string>
#include <algorithm>

// Define arrays for testing
int status[M];
std::string key[M];
int val[M];

// Declare the function under test
int openAddressingHashGet(const std::string& op, const std::string& k, int v = 0);

int main() {
    fill(status, status + M, EMPTY);

    // Insert new keys
    assert(openAddressingHashGet("insert", "orange", 724) == -1);
    assert(openAddressingHashGet("insert", "melon", 20) == -1);
    assert(openAddressingHashGet("find", "melon") == 20);

    // Insert more keys
    assert(openAddressingHashGet("insert", "banana", 52) == -1);
    assert(openAddressingHashGet("insert", "cherry", 27) == -1);

    // Update existing key returns old value
    assert(openAddressingHashGet("insert", "orange", 100) == 724);
    assert(openAddressingHashGet("find", "banana") == 52);
    assert(openAddressingHashGet("find", "orange") == 100);

    // Erase non-existent key
    assert(openAddressingHashGet("erase", "wrong_fruit") == -1);
    // Erase existing key
    assert(openAddressingHashGet("erase", "orange") == 100);
    // Now not found
    assert(openAddressingHashGet("find", "orange") == -1);
    // Erase again returns -1
    assert(openAddressingHashGet("erase", "orange") == -1);

    // Reinsert after deletion
    assert(openAddressingHashGet("insert", "orange", 15) == -1);
    assert(openAddressingHashGet("find", "orange") == 15);

    // More insertion and updates
    assert(openAddressingHashGet("insert", "apple", 36) == -1);
    assert(openAddressingHashGet("insert", "lemon", 6) == -1);
    assert(openAddressingHashGet("insert", "orange", 701) == 15);

    // Verify values
    assert(openAddressingHashGet("find", "cherry") == 27);
    assert(openAddressingHashGet("erase", "xxxxxxx") == -1);
    assert(openAddressingHashGet("find", "xxxxxxx") == -1);
    assert(openAddressingHashGet("find", "apple") == 36);
    assert(openAddressingHashGet("find", "melon") == 20);
    assert(openAddressingHashGet("find", "banana") == 52);
    assert(openAddressingHashGet("find", "cherry") == 27);
    assert(openAddressingHashGet("find", "orange") == 701);
    assert(openAddressingHashGet("find", "lemon") == 6);
    return 0;
}

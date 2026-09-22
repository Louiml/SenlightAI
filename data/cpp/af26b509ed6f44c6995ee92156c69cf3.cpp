/*
Implement a hash table class named `IntHashTable` that stores integer keys using open addressing with quadratic probing. The table must support insertion, deletion, lookup, and reporting the current number of stored elements. The table should automatically rehash to the next prime size when the load factor exceeds 0.5. Your implementation must provide a public constructor taking an initial table size, and public methods `insert(int)`, `remove(int)`, `find(int)`, `size()`, and `listAll(std::ostream&)` where `listAll` outputs all active keys in the current table order (the order they appear in the internal vector) separated by spaces. The hash function should be `key % table_size`. The table must handle collisions with quadratic probing and mark deleted slots as "DELETED" rather than "EMPTY" to support correct probing. You must also implement private helper functions for checking primality and finding the next prime, and an internal `findPos` function that returns the index where a key should be (or is). For clarity, assume that each key is unique; duplicate insertions should return 0 and not change the table. Removal of a non-existent key returns 0, otherwise 1.
*/

#include <vector>
#include <string>
#include <ostream>
#include <cstddef>

class IntHashTable {
public:
    explicit IntHashTable(int initialSize) 
        : table(nextPrime(static_cast<unsigned long>(initialSize))), 
          status(nextPrime(static_cast<unsigned long>(initialSize)), "EMPTY"),
          currentSize(0) {}

    int insert(int key) {
        int pos = findPos(key);
        if (status[pos] == "ACTIVE" && table[pos] == key) {
            return 0; // duplicate
        }
        // Either EMPTY or DELETED; place here
        table[pos] = key;
        status[pos] = "ACTIVE";
        ++currentSize;
        if (currentSize > static_cast<int>(table.size()) / 2) {
            rehash();
        }
        return 1;
    }

    int remove(int key) {
        int pos = findPos(key);
        if (status[pos] != "ACTIVE" || table[pos] != key) {
            return 0; // not found
        }
        status[pos] = "DELETED";
        --currentSize;
        return 1;
    }

    int find(int key) const {
        int pos = findPos(key);
        return (status[pos] == "ACTIVE" && table[pos] == key) ? 1 : 0;
    }

    int size() const {
        return currentSize;
    }

    int listAll(std::ostream& os) const {
        int count = 0;
        for (std::size_t i = 0; i < table.size(); ++i) {
            if (status[i] == "ACTIVE") {
                if (count > 0) os << " ";
                os << table[i];
                ++count;
            }
        }
        return count;
    }

private:
    std::vector<int> table;
    std::vector<std::string> status;
    int currentSize;

    int findPos(int key) const {
        int collisionNum = 0;
        int pos = key % static_cast<int>(table.size());
        while (status[pos] == "ACTIVE" && table[pos] != key) {
            ++collisionNum;
            pos = (pos + collisionNum) % static_cast<int>(table.size());
        }
        return pos;
    }

    void rehash() {
        std::vector<int> oldTable = table;
        std::vector<std::string> oldStatus = status;
        std::size_t oldSize = table.size();

        table.assign(nextPrime(2 * oldSize), 0);
        status.assign(nextPrime(2 * oldSize), "EMPTY");
        currentSize = 0;

        for (std::size_t i = 0; i < oldSize; ++i) {
            if (oldStatus[i] == "ACTIVE") {
                insert(oldTable[i]);
            }
        }
    }

    static bool isPrime(unsigned long n) {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (unsigned long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    static unsigned long nextPrime(unsigned long n) {
        while (!isPrime(++n)) {}
        return n;
    }
};

#include <cassert>
#include <sstream>
#include <iostream>

int main() {
    IntHashTable ht(5);
    // Basic insert
    assert(ht.insert(10) == 1);
    assert(ht.insert(20) == 1);
    assert(ht.size() == 2);
    // Duplicate
    assert(ht.insert(10) == 0);
    assert(ht.size() == 2);
    // Find
    assert(ht.find(10) == 1);
    assert(ht.find(30) == 0);
    // Remove
    assert(ht.remove(10) == 1);
    assert(ht.remove(10) == 0);
    assert(ht.find(10) == 0);
    assert(ht.size() == 1);
    // Trigger rehash
    for (int i = 0; i < 10; ++i) {
        ht.insert(100 + i);
    }
    assert(ht.size() == 11); // 1 (20) + 10 new
    assert(ht.find(20) == 1);
    assert(ht.find(109) == 1);
    assert(ht.find(200) == 0);
    // listAll
    std::ostringstream oss;
    int cnt = ht.listAll(oss);
    assert(cnt == 11);
    std::string output = oss.str();
    assert(output.find("20") != std::string::npos);
    assert(output.find("109") != std::string::npos);
    // Equality of output count with size
    std::istringstream iss(output);
    int val, readCount = 0;
    while (iss >> val) ++readCount;
    assert(readCount == 11);
    // Test with prime size to ensure constructor works
    IntHashTable ht2(7);
    assert(ht2.size() == 0);
    ht2.insert(1);
    ht2.insert(2);
    ht2.insert(3);
    assert(ht2.size() == 3);
    assert(ht2.find(2) == 1);
    ht2.remove(2);
    assert(ht2.size() == 2);
    assert(ht2.find(2) == 0);
    std::cout << "All tests passed!\n";
}

// The core challenge is implementing a hash table with quadratic probing and lazy deletion. The table maintains two parallel vectors: one for the integer keys and one for status strings ("EMPTY", "ACTIVE", "DELETED"). For insertion and lookup, `findPos` starts with the base hash index and applies quadratic probing: `current_pos = (base + i*i) % table_size` for i = 0,1,2,... The loop should continue while the current slot is not empty (i.e., status is "ACTIVE" or "DELETED") and the key does not match the target. When inserting, we stop at the first slot that is "EMPTY" or "DELETED" (but prefer "DELETED" if we've already passed it? Actually simpler: stop at the first slot whose key matches or whose status is "EMPTY" or "DELETED" — but we must ensure that if the key exists we find it, so we must continue probing until we find a matching key or an "EMPTY" slot; "DELETED" slots do not stop probing because the key might be later in the probe sequence). So the standard algorithm: probe until we hit an "EMPTY" slot (meaning key not present) or find a matching key. For insertion, if we find a matching active key, return 0; if we find an "EMPTY" slot, insert there. We can also reuse "DELETED" slots but that's optional. The rehash operation doubles the table size and finds the next prime, then re-inserts all active keys. Edge cases: table size must be prime initially; the constructor should call `nextPrime` on the given size. The `findPos` must handle potential infinite loops if the table is full, but because we rehash when load factor > 0.5, the table is never more than half full, so probing always terminates. Time complexity: average O(1) for insert/find/remove, worst-case O(n) when many collisions. Space complexity O(n) for the two vectors. The `listAll` method iterates the vector and prints only active keys.

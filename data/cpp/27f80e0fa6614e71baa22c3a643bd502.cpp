Implement a C++ class named `RandomizedSet` that supports the following operations in average O(1) time: `add(int val)` (insert if not present, return `true` on successful insertion, `false` if already exists), `remove(int val)` (delete if present, return `true` on successful deletion, `false` if not exists), `search(int val)` (return the index of the element if present, otherwise `-1`), and `getRandom()` (return a uniformly random element from the current set). The class should maintain a dynamic collection where inserting an existing value has no effect, removing a non‑existing value is a no‑op, and random access must be efficient. The container must handle arbitrary integers, including duplicates are not allowed (set semantics), and the order of elements is irrelevant. Provide the class definition and all member function implementations.
The core idea is to use a combination of a dynamic array (`std::vector<int>`) and a hash map (`std::unordered_map<int, int>`) that maps each value to its current index in the array. This dual structure gives O(1) average access for all operations:  
- **add(val)**: Check if `val` exists in the map; if yes, return `false`. Otherwise, push `val` to the end of the vector, record the index in the map, and return `true`.  
- **remove(val)**: If `val` not in the map, return `false`. Otherwise, swap the element at the mapped index with the last element in the vector, pop the back, update the map entry for the moved element (the one that was at the last position) to the old index, erase the entry for `val`, and return `true`. This avoids shifting elements.  
- **search(val)**: Return the map’s value for `val` if present, else `-1`.  
- **getRandom()**: Generate a random index between 0 and size-1 using `rand()` (or better, `std::mt19937` with a uniform distribution) and return the element at that index.  
Edge cases: Removing the last element works because the swap is with itself, and the map update sets the same index. Empty container for `getRandom` is undefined, but we can handle it by returning a sentinel or asserting non‑emptiness. Time complexity: all operations are O(1) average; space O(n) for stored elements. The provided snippet uses `map` (tree‑based) but the unordered version is preferred for O(1) average.
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

class RandomizedSet {
private:
    std::vector<int> values;
    std::unordered_map<int, int> valToIndex;

public:
    RandomizedSet() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    // Insert value if not present. Return true if inserted, false if already exists.
    bool add(int val) {
        if (valToIndex.find(val) != valToIndex.end()) {
            return false;
        }
        valToIndex[val] = values.size();
        values.push_back(val);
        return true;
    }

    // Remove value if present. Return true if removed, false if not exists.
    bool remove(int val) {
        auto it = valToIndex.find(val);
        if (it == valToIndex.end()) {
            return false;
        }
        int index = it->second;
        int lastIndex = values.size() - 1;
        if (index != lastIndex) {
            values[index] = values.back();
            valToIndex[values[index]] = index;
        }
        values.pop_back();
        valToIndex.erase(it);
        return true;
    }

    // Search value. Return index if present, else -1.
    int search(int val) const {
        auto it = valToIndex.find(val);
        return it == valToIndex.end() ? -1 : it->second;
    }

    // Return a uniformly random element. Assumes non‑empty container.
    int getRandom() const {
        if (values.empty()) {
            return -1; // or throw
        }
        int randomIndex = std::rand() % values.size();
        return values[randomIndex];
    }

    // Optional: get current size
    int size() const {
        return values.size();
    }
};
#include <cassert>

int main() {
    RandomizedSet rs;
    assert(rs.add(10) == true);
    assert(rs.add(20) == true);
    assert(rs.add(30) == true);
    assert(rs.add(10) == false); // duplicate

    assert(rs.search(20) == 1);
    assert(rs.search(40) == -1);

    assert(rs.remove(20) == true);
    assert(rs.remove(20) == false); // already removed

    assert(rs.search(20) == -1);
    assert(rs.search(30) == 1); // indices shift after removal

    assert(rs.add(50) == true);
    assert(rs.size() == 3);

    // getRandom should return one of the existing elements
    int randomVal = rs.getRandom();
    assert(randomVal == 10 || randomVal == 30 || randomVal == 50);

    // Test removing the last element
    assert(rs.remove(50) == true);
    assert(rs.size() == 2);
    assert(rs.search(50) == -1);

    // Test random with single element
    RandomizedSet single;
    single.add(7);
    assert(single.getRandom() == 7);

    return 0;
}

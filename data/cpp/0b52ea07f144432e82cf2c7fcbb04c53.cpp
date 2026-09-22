// Implement a C++ function `simulate_lru_cache` that takes as input an integer `capacity`, a vector of `long long` keys, and a vector of `long long` values, representing a sequence of `put` operations into an LRU (least recently used) cache. The cache has a fixed maximum capacity, and when a new item is inserted while the cache is full, the least recently used item (the one at the front of the LRU order) is evicted. All `put` operations are performed sequentially starting from an empty cache. After processing all `put` operations, the function must return a vector of pairs (or a vector of `std::pair<long long, long long>`) representing the final contents of the cache in LRU order from most recent to least recent. For example, if the cache contains items with keys `{1,2,3}` and the most recently used is `3` then `2` then `1`, the output vector must be `[(3, data3), (2, data2), (1, data1)]`. If the capacity is zero or the input vectors are empty, return an empty vector. If the same key is inserted multiple times, the existing entry is updated with the new value and its recency is refreshed (moved to the most recent position). Duplicate keys in the input are allowed, and the last value for a given key is the one retained. The solution must use a doubly linked list and a hash map for O(1) average time per operation; do not use `std::list` or other standard containers for the LRU structure—implement your own. The function signature must be: `std::vector<std::pair<long long, long long>> simulate_lru_cache(long long capacity, const std::vector<long long>& put_keys, const std::vector<long long>& put_values)`.

// The problem is a classic LRU cache simulation. The core approach is to use a doubly linked list to maintain the recency order: the head represents the most recently used item, and the tail represents the least recently used. Each node stores a key, value, and pointers to the previous and next nodes. A hash map maps each key to a pointer to its corresponding list node, enabling O(1) lookup. For each `put` operation: if the key already exists (found in the hash map), update the node's value, then move that node to the head (by removing it from its current position and inserting at the front). If the key is new: create a new node and insert at the head. If the cache size equals capacity, remove the tail node (the least recently used) before inserting. If capacity is 0, no items can be stored, so we can simply skip all puts and return empty. After processing all puts, traverse the list from head to tail and collect key-value pairs into a vector. Edge cases include capacity 0, empty inputs, repeated keys, and capacity exactly 1. The time complexity is O(N) for N put operations (each operation is O(1) average due to hash map and list operations), and the space complexity is O(capacity) for storing up to capacity nodes plus the hash map.

#include <vector>
#include <unordered_map>
#include <utility>

// Doubly linked list node for LRU cache
struct LRUNode {
    long long key;
    long long value;
    LRUNode* prev;
    LRUNode* next;
    LRUNode(long long k, long long v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// Simulate an LRU cache with given capacity and a sequence of put operations.
// Returns the final cache contents in order from most recently used to least recently used.
std::vector<std::pair<long long, long long>> simulate_lru_cache(
    long long capacity,
    const std::vector<long long>& put_keys,
    const std::vector<long long>& put_values) {

    // Result vector will be built at the end
    std::vector<std::pair<long long, long long>> result;

    // If capacity is zero, nothing is stored
    if (capacity <= 0) {
        return result;
    }

    // Head points to most recent, tail to least recent
    LRUNode* head = nullptr;
    LRUNode* tail = nullptr;
    long long current_size = 0;
    std::unordered_map<long long, LRUNode*> cache;

    // Process each put operation
    for (size_t i = 0; i < put_keys.size(); ++i) {
        long long key = put_keys[i];
        long long value = put_values[i];

        auto it = cache.find(key);
        if (it != cache.end()) {
            // Key exists: update value and move node to head
            LRUNode* node = it->second;
            node->value = value;

            // If node is already head, nothing to do
            if (node != head) {
                // Unlink node from current position
                if (node->prev) node->prev->next = node->next;
                if (node->next) node->next->prev = node->prev;
                if (node == tail) tail = node->prev;

                // Insert at head
                node->prev = nullptr;
                node->next = head;
                if (head) head->prev = node;
                head = node;
            }
        } else {
            // New key
            LRUNode* node = new LRUNode(key, value);

            // If cache is full, remove least recently used (tail)
            if (current_size == capacity) {
                // Remove tail from map and list
                cache.erase(tail->key);
                LRUNode* old_tail = tail;
                tail = tail->prev;
                if (tail) tail->next = nullptr;
                else head = nullptr; // cache was size 1
                delete old_tail;
                current_size--;
            }

            // Insert new node at head
            node->next = head;
            if (head) head->prev = node;
            head = node;
            if (tail == nullptr) tail = node; // first node

            cache[key] = node;
            current_size++;
        }
    }

    // Build result from head to tail
    LRUNode* curr = head;
    while (curr) {
        result.emplace_back(curr->key, curr->value);
        curr = curr->next;
    }

    // Clean up memory (not strictly necessary for correctness but good practice)
    curr = head;
    while (curr) {
        LRUNode* next = curr->next;
        delete curr;
        curr = next;
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration for testing
std::vector<std::pair<long long, long long>> simulate_lru_cache(
    long long capacity,
    const std::vector<long long>& put_keys,
    const std::vector<long long>& put_values);

int main() {
    // Test 1: Basic insertion with capacity 2
    {
        std::vector<long long> keys = {1, 2, 3};
        std::vector<long long> values = {10, 20, 30};
        auto result = simulate_lru_cache(2, keys, values);
        std::vector<std::pair<long long, long long>> expected = {{3, 30}, {2, 20}};
        assert(result == expected);
    }

    // Test 2: Update existing key and refresh recency
    {
        std::vector<long long> keys = {1, 2, 1};
        std::vector<long long> values = {10, 20, 99};
        auto result = simulate_lru_cache(3, keys, values);
        std::vector<std::pair<long long, long long>> expected = {{1, 99}, {2, 20}};
        assert(result == expected);
    }

    // Test 3: Capacity 0 returns empty
    {
        std::vector<long long> keys = {1, 2};
        std::vector<long long> values = {10, 20};
        auto result = simulate_lru_cache(0, keys, values);
        assert(result.empty());
    }

    // Test 4: Empty inputs
    {
        std::vector<long long> keys;
        std::vector<long long> values;
        auto result = simulate_lru_cache(5, keys, values);
        assert(result.empty());
    }

    // Test 5: Capacity 1 with repeated key
    {
        std::vector<long long> keys = {1, 1, 2, 2};
        std::vector<long long> values = {10, 11, 20, 21};
        auto result = simulate_lru_cache(1, keys, values);
        std::vector<std::pair<long long, long long>> expected = {{2, 21}};
        assert(result == expected);
    }

    // Test 6: Eviction order with capacity 3
    {
        std::vector<long long> keys = {1, 2, 3, 4, 5};
        std::vector<long long> values = {10, 20, 30, 40, 50};
        auto result = simulate_lru_cache(3, keys, values);
        std::vector<std::pair<long long, long long>> expected = {{5, 50}, {4, 40}, {3, 30}};
        assert(result == expected);
    }

    // Test 7: Refreshing a middle element then inserting new
    {
        std::vector<long long> keys = {1, 2, 3, 2, 4};
        std::vector<long long> values = {10, 20, 30, 25, 40};
        auto result = simulate_lru_cache(3, keys, values);
        // After 1,2,3 -> [3,2,1]; then put 2 -> [2,3,1]; then put 4 -> [4,2,3]
        std::vector<std::pair<long long, long long>> expected = {{4, 40}, {2, 25}, {3, 30}};
        assert(result == expected);
    }

    // Test 8: Large key values
    {
        std::vector<long long> keys = {1000000000, 2000000000};
        std::vector<long long> values = {-1, -2};
        auto result = simulate_lru_cache(2, keys, values);
        std::vector<std::pair<long long, long long>> expected = {{2000000000, -2}, {1000000000, -1}};
        assert(result == expected);
    }

    return 0;
}

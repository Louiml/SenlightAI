/*
Write a C++ function `processQueries` that takes a vector of queries as input. Each query is a tuple where the first element is an integer `type` (0 or 1), the second is a string `key`, and the third is an optional integer `value` (used only when type is 0). The function must maintain a simple key-value store (map) with string keys and integer values. For a query with type 0, insert or update the value associated with the key. For a query with type 1, return the current value associated with that key. If the key does not exist for a type 1 query, return -1. The function should process all queries in order and return a vector of integers containing the results of all type-1 queries in the order they appear.
*/
#include <bits/stdc++.h>

// Process a series of key-value store queries.
// Query type 0: insert/update the value for a given key.
// Query type 1: retrieve the value for a given key, or -1 if absent.
// Returns a vector of results for all type-1 queries in order.
std::vector<long long> processQueries(const std::vector<std::tuple<int, std::string, long long>>& queries) {
    std::map<std::string, long long> store;
    std::vector<long long> results;
    
    for (const auto& query : queries) {
        int type = std::get<0>(query);
        const std::string& key = std::get<1>(query);
        
        if (type == 0) {
            long long value = std::get<2>(query);
            store[key] = value;
        } else { // type == 1
            auto it = store.find(key);
            if (it != store.end()) {
                results.push_back(it->second);
            } else {
                results.push_back(-1);
            }
        }
    }
    
    return results;
}
#include <bits/stdc++.h>

std::vector<long long> processQueries(const std::vector<std::tuple<int, std::string, long long>>& queries);

int main() {
    // Test 1: basic insert and retrieve
    std::vector<std::tuple<int, std::string, long long>> q1 = {
        {0, "apple", 5},
        {1, "apple", 0},
        {1, "banana", 0}
    };
    std::vector<long long> r1 = processQueries(q1);
    assert(r1.size() == 2);
    assert(r1[0] == 5);
    assert(r1[1] == -1);

    // Test 2: update existing key
    std::vector<std::tuple<int, std::string, long long>> q2 = {
        {0, "x", 10},
        {0, "x", 20},
        {1, "x", 0}
    };
    std::vector<long long> r2 = processQueries(q2);
    assert(r2.size() == 1);
    assert(r2[0] == 20);

    // Test 3: multiple keys and no type-1 queries
    std::vector<std::tuple<int, std::string, long long>> q3 = {
        {0, "a", 1},
        {0, "b", 2}
    };
    std::vector<long long> r3 = processQueries(q3);
    assert(r3.empty());

    // Test 4: retrieve absent then insert then retrieve
    std::vector<std::tuple<int, std::string, long long>> q4 = {
        {1, "missing", 0},
        {0, "missing", 42},
        {1, "missing", 0}
    };
    std::vector<long long> r4 = processQueries(q4);
    assert(r4.size() == 2);
    assert(r4[0] == -1);
    assert(r4[1] == 42);

    // Test 5: large values and repeated updates
    std::vector<std::tuple<int, std::string, long long>> q5 = {
        {0, "big", 1000000000000LL},
        {1, "big", 0},
        {0, "big", -999999999999LL},
        {1, "big", 0}
    };
    std::vector<long long> r5 = processQueries(q5);
    assert(r5.size() == 2);
    assert(r5[0] == 1000000000000LL);
    assert(r5[1] == -999999999999LL);

    // Test 6: empty query list
    std::vector<std::tuple<int, std::string, long long>> q6 = {};
    std::vector<long long> r6 = processQueries(q6);
    assert(r6.empty());

    // Test 7: key with empty string
    std::vector<std::tuple<int, std::string, long long>> q7 = {
        {0, "", 7},
        {1, "", 0}
    };
    std::vector<long long> r7 = processQueries(q7);
    assert(r7.size() == 1);
    assert(r7[0] == 7);

    // Test 8: duplicates and same name case sensitivity
    std::vector<std::tuple<int, std::string, long long>> q8 = {
        {0, "Value", 1},
        {0, "value", 2},
        {1, "Value", 0},
        {1, "value", 0}
    };
    std::vector<long long> r8 = processQueries(q8);
    assert(r8.size() == 2);
    assert(r8[0] == 1);
    assert(r8[1] == 2);

    return 0;
}
// The solution maintains a `std::map<string, long long>` (or unordered_map) that stores the latest value for each key. We iterate through the query list once. For each query, if type is 0, we assign the value to the map using `map[key] = value` (this handles both insertion and update). If type is 1, we look up the key in the map: if found, we push the stored value to the result vector; if not found, we push -1. The main edge case is a type-1 query for a key that was never inserted—this must return -1 explicitly because `map[key]` would insert it with a default value of 0. Time complexity is O(n log m) where n is the number of queries and m is the number of unique keys (if using std::map) or O(n) average with unordered_map. Space complexity is O(m) for the map and O(r) for the result vector where r is the number of type-1 queries.

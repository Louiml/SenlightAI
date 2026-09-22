Write a C++ function `countQueries(const std::vector<int>& data, const std::vector<int>& queries)` that takes a list of integers `data` (which may contain duplicates) and a list of integers `queries`, and returns a `std::vector<int>` where each element is the number of occurrences of the corresponding query value in `data`. The function must handle arbitrary integer values (including negative and large numbers), and must return `0` for queries that do not appear in `data`. The input vectors are non-empty but may have different lengths, and the order of results must match the order of `queries`.

#include <cassert>
#include <vector>

// Declaration from the solution (here repeated for standalone test)
#include <unordered_map>

std::vector<int> countQueries(const std::vector<int>& data, const std::vector<int>& queries);

int main() {
    // Basic case with duplicates
    std::vector<int> data1 = {1, 2, 2, 3, 3, 3};
    std::vector<int> queries1 = {3, 1, 4, 2};
    assert(countQueries(data1, queries1) == std::vector<int>({3, 1, 0, 2}));

    // Negative and zero values
    std::vector<int> data2 = {-5, 0, -5, 10, 0, -5};
    std::vector<int> queries2 = {-5, 0, 10, 7};
    assert(countQueries(data2, queries2) == std::vector<int>({3, 2, 1, 0}));

    // Data with a single element, multiple queries
    std::vector<int> data3 = {42};
    std::vector<int> queries3 = {42, 42, 1};
    assert(countQueries(data3, queries3) == std::vector<int>({1, 1, 0}));

    // Empty queries vector
    std::vector<int> data4 = {5, 5, 5};
    std::vector<int> queries4 = {};
    assert(countQueries(data4, queries4).empty());

    // All queries present, large numbers
    std::vector<int> data5 = {1000000, -1000000, 1000000, 123456789};
    std::vector<int> queries5 = {1000000, -1000000, 123456789};
    assert(countQueries(data5, queries5) == std::vector<int>({2, 1, 1}));

    return 0;
}

#include <vector>
#include <unordered_map>

// Count occurrences of each query value in the given data vector.
// Returns a vector with the same size as queries, where result[i] is
// the number of times queries[i] appears in data.
std::vector<int> countQueries(const std::vector<int>& data, const std::vector<int>& queries) {
    std::unordered_map<int, int> frequency;
    for (const int value : data) {
        ++frequency[value];
    }

    std::vector<int> result;
    result.reserve(queries.size());
    for (const int query : queries) {
        auto it = frequency.find(query);
        if (it != frequency.end()) {
            result.push_back(it->second);
        } else {
            result.push_back(0);
        }
    }
    return result;
}

// The solution uses an `unordered_map<int, int>` to count the frequency of each element in the `data` vector. First, iterate through `data` once, incrementing the counter for each value. Then, for each query, look up the query in the map: if found, push the stored frequency; otherwise, push `0`. This approach efficiently handles duplicates and arbitrary integer keys. Edge cases include queries that are absent (handled by the lookup check), negative values (works naturally with integer keys), and zero-length queries (the result will be empty). Time complexity is \(O(D + Q)\) where \(D\) is the size of `data` and \(Q\) is the size of `queries`, because each insertion and lookup is average \(O(1)\). Space complexity is \(O(U)\) where \(U\) is the number of unique elements in `data`, for the map, plus the output vector of size \(Q\). The function should accept both vectors by `const` reference to avoid unnecessary copying.

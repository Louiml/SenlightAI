Write a C++ function `frequencyOfQueries` that takes a vector of integers `data` and a vector of integers `queries`, both of length greater than zero, and returns a vector of integers where each element is the number of times the corresponding query value appears in `data`. The values in both `data` and `queries` are guaranteed to be non‑negative and less than or equal to 100. The function must handle duplicate values, queries that do not appear in `data`, and must be efficient even if `data` is large (up to 10^6 elements). The solution should use direct indexing into a fixed‑size frequency array rather than a hash map, and must not modify the input vectors.

#include <cassert>
#include <vector>

// Assume frequencyOfQueries is defined as above.

int main() {
    std::vector<int> data1 = {1, 3, 2, 1, 3};
    std::vector<int> queries1 = {1, 4, 2, 3, 12};
    std::vector<int> expected1 = {2, 0, 1, 2, 0};
    assert(frequencyOfQueries(data1, queries1) == expected1);

    std::vector<int> data2 = {100, 100, 0, 0, 0, 50};
    std::vector<int> queries2 = {0, 100, 50, 1};
    std::vector<int> expected2 = {3, 2, 1, 0};
    assert(frequencyOfQueries(data2, queries2) == expected2);

    std::vector<int> data3 = {7};
    std::vector<int> queries3 = {7, 7, 0};
    std::vector<int> expected3 = {1, 1, 0};
    assert(frequencyOfQueries(data3, queries3) == expected3);

    std::vector<int> data4 = {5, 5, 5, 5};
    std::vector<int> queries4 = {5};
    std::vector<int> expected4 = {4};
    assert(frequencyOfQueries(data4, queries4) == expected4);

    std::vector<int> data5 = {0, 1, 2, 3, 4, 5};
    std::vector<int> queries5 = {5, 4, 3, 2, 1, 0};
    std::vector<int> expected5 = {1, 1, 1, 1, 1, 1};
    assert(frequencyOfQueries(data5, queries5) == expected5);

    // Large data test with 100,000 elements all equal to 99.
    std::vector<int> data6(100000, 99);
    std::vector<int> queries6 = {99, 98, 100};
    std::vector<int> expected6 = {100000, 0, 0};
    assert(frequencyOfQueries(data6, queries6) == expected6);

    return 0;
}

#include <vector>
#include <cstddef>

// Returns a vector where result[i] = count of queries[i] in data.
std::vector<int> frequencyOfQueries(const std::vector<int>& data, const std::vector<int>& queries) {
    // Frequency array for values in range [0, 100].
    int freq[101] = {0};
    
    // Count occurrences in data.
    for (int value : data) {
        freq[value] += 1;
    }
    
    // Build result for each query.
    std::vector<int> result;
    result.reserve(queries.size());
    for (int query : queries) {
        result.push_back(freq[query]);
    }
    
    return result;
}

// The approach is to precompute a frequency array of size 101 (since values are in [0,100]) initialized to zero. First, iterate through the `data` vector and increment `freq[value]` for each element. Then, for each query value in the `queries` vector, simply look up `freq[query]` and append it to the result vector. This avoids any sorting or searching and runs in linear time with respect to the total number of elements in both input vectors. Important edge cases include queries with values that never appear (frequency 0) and duplicate queries (each occurrence yields the same count). Time complexity is O(N + M) where N = size of `data` and M = size of `queries`. Space complexity is O(101) for the frequency array, plus O(M) for the output vector. The fixed size of the frequency array is safe because the problem constraints bound all values to [0,100]. No special handling for empty inputs is required per the task specification, but the implementation will still work correctly for empty vectors.

// Write a C++ function that, given a vector of positive integers representing the prices of items in a shop and a list of queries (each query is a 1-based index into the sorted descending order of the prices), returns for each query the sum of all item prices except the price at that queried position after sorting the prices in non-increasing order. The function should take the original vector of prices and a vector of query indices, and return a vector of long long values, each being the total sum minus the price at the queried sorted position. For example, if prices are {5, 3, 8} and query is 2, after sorting descending we get {8, 5, 3}; the queried price is 5, so the answer is (5+3+8) - 5 = 11. You must handle queries where the index is valid (1 ≤ index ≤ size of prices), and the total sum may exceed the range of a 32-bit integer, so use `long long` for sums and results.

// The key observation is that the total sum of all prices is constant regardless of sorting. Therefore, after sorting the array in descending order, each query simply asks to subtract the price at position `q-1` (0-based) from the total sum. The algorithm is: (1) compute the total sum of all prices as a `long long`, (2) make a copy of the input vector and sort it in descending order using `std::sort` with a greater comparator, (3) for each query index `q`, subtract `sorted[q-1]` from the total sum and push the result into the output vector. Edge cases: if the input vector is empty, but the problem guarantees at least one price; queries are guaranteed to be within 1 to n, so no bounds checking is needed, but robust code could validate. Complexity: sorting takes O(n log n) time, computing the sum takes O(n), and processing m queries takes O(m); overall O(n log n + m) time and O(n) auxiliary space for the sorted copy (the output vector is part of the result).

#include <vector>
#include <algorithm>
#include <cstdint>

// Given original prices and 1-based query indices into the descending-sorted list,
// return for each query the sum of all prices except the price at that sorted position.
std::vector<long long> queryExcludingPrice(const std::vector<int>& prices, const std::vector<int>& queries) {
    long long total = 0;
    for (int price : prices) {
        total += static_cast<long long>(price);
    }

    std::vector<int> sorted = prices;  // copy, because we must not modify input
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());

    std::vector<long long> results;
    results.reserve(queries.size());
    for (int q : queries) {
        // q is 1-based, so subtract the (q-1)-th element (0-based) from total
        results.push_back(total - static_cast<long long>(sorted[q - 1]));
    }
    return results;
}

#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Example from description
    std::vector<int> prices1 = {5, 3, 8};
    std::vector<int> queries1 = {2};
    std::vector<long long> result1 = queryExcludingPrice(prices1, queries1);
    assert(result1.size() == 1 && result1[0] == 11);

    // Single element: query 1 gives sum minus itself = 0
    std::vector<int> prices2 = {7};
    std::vector<int> queries2 = {1};
    std::vector<long long> result2 = queryExcludingPrice(prices2, queries2);
    assert(result2[0] == 0);

    // All duplicates, any query gives same answer: total = n*val, subtract val => (n-1)*val
    std::vector<int> prices3 = {4, 4, 4, 4};
    std::vector<int> queries3 = {1, 2, 3, 4};
    std::vector<long long> result3 = queryExcludingPrice(prices3, queries3);
    for (long long r : result3) assert(r == 12);

    // Large sum to ensure long long usage: prices with large int values
    std::vector<int> prices4 = {1000000, 2000000, 3000000};
    std::vector<int> queries4 = {1, 2, 3};
    std::vector<long long> result4 = queryExcludingPrice(prices4, queries4);
    // Total = 6000000; sorted descending: 3000000, 2000000, 1000000
    assert(result4[0] == 3000000); // 6000000 - 3000000
    assert(result4[1] == 4000000); // 6000000 - 2000000
    assert(result4[2] == 5000000); // 6000000 - 1000000

    // Unsorted input, multiple queries
    std::vector<int> prices5 = {10, 1, 5, 20};
    std::vector<int> queries5 = {1, 4};
    // Sorted descending: {20, 10, 5, 1}, total = 36
    std::vector<long long> result5 = queryExcludingPrice(prices5, queries5);
    assert(result5[0] == 16); // 36 - 20
    assert(result5[1] == 35); // 36 - 1
}

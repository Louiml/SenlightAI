// Write a C++ function that, given a vector of integers (which may contain negative values and duplicates) and a list of queries, each specifying a contiguous subarray by its 1-based inclusive indices `l` and `r`, returns the sum of elements in that subarray for each query. The function should take the original array and a vector of pairs (or a separate structure) representing the queries, and return a vector of sums in the same order as the queries. The array length can be up to \(10^5\) and the number of queries up to \(10^5\), so an efficient prefix-sum approach must be used.

#include <cassert>
#include <vector>
#include <utility>

// forward declaration of the function being tested
std::vector<long long> subarraySums(const std::vector<int>& arr,
                                    const std::vector<std::pair<int, int>>& queries);

int main() {
    // Basic case
    std::vector<int> a1 = {1, 2, 3, 4, 5};
    std::vector<std::pair<int,int>> q1 = {{1, 3}, {2, 5}, {1, 1}};
    std::vector<long long> r1 = subarraySums(a1, q1);
    assert(r1.size() == 3);
    assert(r1[0] == 6);   // 1+2+3
    assert(r1[1] == 14);  // 2+3+4+5
    assert(r1[2] == 1);   // single element

    // Negative and duplicate numbers
    std::vector<int> a2 = {-5, 2, -5, 2, 10};
    std::vector<std::pair<int,int>> q2 = {{1, 5}, {3, 3}, {4, 5}};
    std::vector<long long> r2 = subarraySums(a2, q2);
    assert(r2[0] == 4);   // -5+2-5+2+10 = 4
    assert(r2[1] == -5);  // a2[2] = -5
    assert(r2[2] == 12);  // 2+10

    // Single element array
    std::vector<int> a3 = {100};
    std::vector<std::pair<int,int>> q3 = {{1,1}};
    std::vector<long long> r3 = subarraySums(a3, q3);
    assert(r3[0] == 100);

    // Whole array and first element query
    std::vector<int> a4 = {10, -20, 30, -40, 50};
    std::vector<std::pair<int,int>> q4 = {{1, 5}, {1, 1}, {2, 2}};
    std::vector<long long> r4 = subarraySums(a4, q4);
    assert(r4[0] == 30);  // 10-20+30-40+50 = 30
    assert(r4[1] == 10);
    assert(r4[2] == -20);

    // Long array with several queries (performance sanity not tested here, but logic)
    std::vector<int> a5 = {0, 0, 0, 0};
    std::vector<std::pair<int,int>> q5 = {{1, 4}, {2, 3}, {4, 4}};
    std::vector<long long> r5 = subarraySums(a5, q5);
    for (auto v : r5) assert(v == 0);

    return 0;
}

#include <vector>
#include <cstddef>

// Given an array of integers and a list of queries (1-based inclusive [l, r]),
// return the sum for each query.
std::vector<long long> subarraySums(const std::vector<int>& arr,
                                    const std::vector<std::pair<int, int>>& queries) {
    const std::size_t n = arr.size();
    
    // Build prefix sums: prefix[i] = sum of arr[0..i]
    std::vector<long long> prefix(n);
    if (n > 0) {
        prefix[0] = arr[0];
        for (std::size_t i = 1; i < n; ++i) {
            prefix[i] = prefix[i - 1] + arr[i];
        }
    }
    
    std::vector<long long> answers;
    answers.reserve(queries.size());
    
    for (const auto& q : queries) {
        // Convert 1-based inclusive to 0-based inclusive
        int l = q.first - 1;
        int r = q.second - 1;
        if (l > r || l < 0 || r >= static_cast<int>(n)) {
            // Invalid range, but per constraints we assume valid; still handle safely.
            answers.push_back(0);
        } else if (l == 0) {
            answers.push_back(prefix[r]);
        } else {
            answers.push_back(prefix[r] - prefix[l - 1]);
        }
    }
    
    return answers;
}

// The main algorithm uses a prefix-sum array `pre` of the same size as the input, where `pre[i]` stores the sum of elements from index `0` to `i` inclusive. This allows any subarray sum `[l, r]` (0-based) to be computed in constant time as: if `l == 0`, the sum is `pre[r]`; otherwise, it is `pre[r] - pre[l-1]`. Edge cases include queries where the range is a single element, where `l = 1` (the first element), and when there are negative numbers or duplicates (these are handled naturally by direct summation of the prefix values). The time complexity is \(O(n + q)\) where \(n\) is the length of the array and \(q\) is the number of queries, because building the prefix array takes linear time and each query is answered in O(1). The space complexity is \(O(n)\) for the prefix array, ignoring the input and output vectors.

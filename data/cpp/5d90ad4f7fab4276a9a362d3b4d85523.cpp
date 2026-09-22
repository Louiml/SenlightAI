/*
You are given an array of `n` distinct positions labeled from `1` to `n`, each containing an integer value. You must answer `q` queries. For each query, two integer values `a` and `b` are given. Your task is to write a C++ function that, given the array of values, the number of queries, and a list of query pairs (as two vectors of integers, one for `a` and one for `b`), returns a vector of strings, each being `"YES"` if there exists at least one position `i` containing value `a` and at least one position `j` containing value `b` such that `i < j`; otherwise returns `"NO"`. Note that `a` and `b` may be equal, in which case the condition requires a position with that value appearing before another (different) position with the same value. The array is 0-indexed internally for implementation, but positions for the condition are 1-indexed as given. Values may be large up to 10^9, and the number of positions `n` and queries `q` can be up to 2*10^5.
*/
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// Returns "YES" for each query if some position with value a appears before a position with value b.
// The input `arr` is 1-indexed internally (positions 1..n). Queries are given in two parallel vectors.
std::vector<std::string> answerQueries(const std::vector<int>& arr, const std::vector<int>& aQueries, const std::vector<int>& bQueries) {
    int n = static_cast<int>(arr.size());
    std::unordered_map<int, int> firstPos;
    std::unordered_map<int, int> lastPos;
    for (int i = 0; i < n; ++i) {
        int pos = i + 1; // convert to 1-based
        int value = arr[i];
        if (firstPos.find(value) == firstPos.end()) {
            firstPos[value] = pos;
        }
        lastPos[value] = pos;
    }
    
    std::vector<std::string> result;
    result.reserve(aQueries.size());
    for (size_t q = 0; q < aQueries.size(); ++q) {
        int a = aQueries[q];
        int b = bQueries[q];
        auto itA = firstPos.find(a);
        auto itB = lastPos.find(b);
        if (itA == firstPos.end() || itB == lastPos.end()) {
            result.emplace_back("NO");
        } else {
            result.emplace_back(itA->second < itB->second ? "YES" : "NO");
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here for completeness (in actual test, include the header).
std::vector<std::string> answerQueries(const std::vector<int>& arr, const std::vector<int>& aQueries, const std::vector<int>& bQueries);

int main() {
    // Test 1: Basic case
    std::vector<int> arr1 = {1, 2, 3};
    std::vector<int> a1 = {1, 3, 2};
    std::vector<int> b1 = {2, 1, 1};
    std::vector<std::string> r1 = answerQueries(arr1, a1, b1);
    assert(r1 == std::vector<std::string>({"YES", "YES", "NO"}));
    
    // Test 2: Duplicate values (a == b but appears twice)
    std::vector<int> arr2 = {5, 1, 5, 2};
    std::vector<int> a2 = {5, 5, 2};
    std::vector<int> b2 = {5, 2, 5};
    std::vector<std::string> r2 = answerQueries(arr2, a2, b2);
    assert(r2 == std::vector<std::string>({"YES", "YES", "NO"}));
    
    // Test 3: Missing value
    std::vector<int> arr3 = {10, 20, 30};
    std::vector<int> a3 = {10, 40, 30};
    std::vector<int> b3 = {40, 10, 20};
    std::vector<std::string> r3 = answerQueries(arr3, a3, b3);
    assert(r3 == std::vector<std::string>({"NO", "NO", "YES"}));
    
    // Test 4: Single element array, no pair possible
    std::vector<int> arr4 = {7};
    std::vector<int> a4 = {7};
    std::vector<int> b4 = {7};
    std::vector<std::string> r4 = answerQueries(arr4, a4, b4);
    assert(r4 == std::vector<std::string>({"NO"}));
    
    // Test 5: Large indices and negative values (positions are positive, values can be any int)
    std::vector<int> arr5 = {100, -5, 100, -5, 0};
    std::vector<int> a5 = {100, -5, 0, 100};
    std::vector<int> b5 = {-5, 100, -5, 0};
    std::vector<std::string> r5 = answerQueries(arr5, a5, b5);
    assert(r5 == std::vector<std::string>({"YES", "YES", "YES", "NO"}));
    
    // Test 6: Multiple queries including duplicates
    std::vector<int> arr6 = {1, 1, 1};
    std::vector<int> a6 = {1, 1, 1};
    std::vector<int> b6 = {1, 1, 1};
    std::vector<std::string> r6 = answerQueries(arr6, a6, b6);
    assert(r6 == std::vector<std::string>({"YES", "YES", "YES"}));
    
    // Test 7: All positions distinct, query reverse order
    std::vector<int> arr7 = {4, 3, 2, 1};
    std::vector<int> a7 = {2, 1, 3};
    std::vector<int> b7 = {1, 4, 4};
    std::vector<std::string> r7 = answerQueries(arr7, a7, b7);
    assert(r7 == std::vector<std::string>({"YES", "YES", "NO"}));
    
    return 0;
}
// For each distinct value present in the array, we only need to know its earliest occurrence position and its latest occurrence position. This is because, for a query `(a,b)`, the existence of an index with value `a` before an index with value `b` is equivalent to checking whether the earliest position of `a` is strictly less than the latest position of `b`. If that holds, we can pick those two specific positions as evidence; otherwise, even the earliest `a` is not before the latest `b`, so no such pair can exist. If `a` or `b` does not appear at all, the answer is immediately `"NO"`. The algorithm first scans the array once to record, for each value, the minimum and maximum position (using 1-based indices). Then, for each query, it checks the two index pairs in O(1) time. Edge cases include when `a` equals `b` but the value appears at least twice, and when the same value appears but only once (then the earliest equals the latest, so the strict inequality fails). Time complexity is O(n + q) with O(m) space where m is the number of distinct values (stored in a map). This is optimal because each query is answered in constant time after a single preprocessing pass.

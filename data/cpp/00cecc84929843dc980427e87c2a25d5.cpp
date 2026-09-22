// Write a C++ function that processes a series of database queries about the positions of integers in an initially given sequence. The function receives two integers `n` (number of elements) and `m` (number of queries), followed by a vector of `n` integers (the sequence), and then `m` pairs of integers `(k, value)`. For each query, the function must return the 1-based index (position) of the `k`-th occurrence (1-based, so `k=1` means the first occurrence) of `value` in the original sequence. If `value` occurs fewer than `k` times, return `0`. The function should process queries sequentially and return a vector of integers containing the answer for each query, in order. The sequence may contain any integers (including duplicates), and `n`, `m` can be zero (in which case return an empty vector). The function signature should be: `std::vector<int> queryPositions(int n, int m, const std::vector<int>& seq, const std::vector<std::pair<int,int>>& queries)`. Constraints: `0 ≤ n, m ≤ 100,000`, each integer in sequence and queries fits in a 32-bit signed int.

// The optimal approach is to preprocess the sequence into a map that stores, for each distinct integer, the list of 1-based indices where that integer appears. This is done in a single pass over the sequence, appending `i+1` to the vector associated with `seq[i]`. Afterwards, each query `(k, value)` is answered by checking if the key `value` exists in the map and if its vector size is at least `k`. If yes, the answer is the element at index `k-1` (since the vector is 0-indexed); otherwise, answer is `0`. This is efficient because each query is answered in constant time after the preprocessing. Important edge cases: when `n=0` or `m=0` (trivially return empty), when `k` is larger than the number of occurrences, when `value` does not appear at all, and when `k` is 1 (first occurrence). The time complexity is O(n + m) overall, and space complexity is O(n) for storing all index lists.

#include <vector>
#include <map>
#include <utility>

// Process queries for the k-th occurrence of a value in a sequence.
// Returns a vector where each element corresponds to one query's answer (0 if not enough occurrences).
std::vector<int> queryPositions(int n, int m, const std::vector<int>& seq, const std::vector<std::pair<int,int>>& queries) {
    // Build a map from value to list of positions (1-based) where it appears.
    std::map<int, std::vector<int>> positions;
    for (int i = 0; i < n; ++i) {
        positions[seq[i]].push_back(i + 1);  // i is 0-based, so position is i+1
    }

    std::vector<int> results;
    results.reserve(m);
    for (const auto& q : queries) {
        int k = q.first;
        int value = q.second;
        auto it = positions.find(value);
        if (it != positions.end() && it->second.size() >= static_cast<size_t>(k)) {
            results.push_back(it->second[k - 1]);  // k is 1-based, so use k-1
        } else {
            results.push_back(0);
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (in practice, include it here).
std::vector<int> queryPositions(int n, int m, const std::vector<int>& seq, const std::vector<std::pair<int,int>>& queries);

int main() {
    // Test 1: basic case with duplicates
    std::vector<int> seq1 = {5, 2, 5, 3, 5};
    std::vector<std::pair<int,int>> q1 = {{1,5}, {2,5}, {3,5}, {4,5}, {1,2}, {1,9}};
    std::vector<int> r1 = queryPositions(5, 6, seq1, q1);
    assert((r1 == std::vector<int>{1, 3, 5, 0, 2, 0}));

    // Test 2: empty sequence
    std::vector<int> seq2 = {};
    std::vector<std::pair<int,int>> q2 = {{1,1}, {5,2}};
    std::vector<int> r2 = queryPositions(0, 2, seq2, q2);
    assert((r2 == std::vector<int>{0, 0}));

    // Test 3: no queries
    std::vector<int> seq3 = {7, 7, 7};
    std::vector<std::pair<int,int>> q3 = {};
    std::vector<int> r3 = queryPositions(3, 0, seq3, q3);
    assert(r3.empty());

    // Test 4: unique values, k=1
    std::vector<int> seq4 = {10, 20, 30};
    std::vector<std::pair<int,int>> q4 = {{1,10}, {1,20}, {1,30}, {2,10}};
    std::vector<int> r4 = queryPositions(3, 4, seq4, q4);
    assert((r4 == std::vector<int>{1, 2, 3, 0}));

    // Test 5: negative values and zero
    std::vector<int> seq5 = {-1, -1, 0, -1, 5};
    std::vector<std::pair<int,int>> q5 = {{1,-1}, {3,-1}, {4,-1}, {1,0}, {1,5}};
    std::vector<int> r5 = queryPositions(5, 5, seq5, q5);
    assert((r5 == std::vector<int>{1, 4, 0, 3, 5}));

    // Test 6: large k and large n, many duplicates
    std::vector<int> seq6(100, 42);
    std::vector<std::pair<int,int>> q6 = {{50,42}, {100,42}, {101,42}, {1,42}};
    std::vector<int> r6 = queryPositions(100, 4, seq6, q6);
    assert((r6 == std::vector<int>{50, 100, 0, 1}));

    // Test 7: mixed with multiple values interleaved
    std::vector<int> seq7 = {1, 2, 1, 3, 2, 1, 4, 2};
    std::vector<std::pair<int,int>> q7 = {{1,1}, {2,1}, {3,1}, {4,1}, {2,2}, {3,2}, {1,4}, {1,5}};
    std::vector<int> r7 = queryPositions(8, 8, seq7, q7);
    assert((r7 == std::vector<int>{1, 3, 6, 0, 5, 8, 7, 0}));
}

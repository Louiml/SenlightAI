Write a C++ function named `processQueries` that takes two integers `n` (the number of available slots, labeled 1 through `n`) and `q` (the number of queries), followed by a vector of `q` query values. The function must simulate the following process: if a query value is positive (`v[i] > 0`), that specific slot is selected and its usage count is incremented; if a query value is non-positive (`v[i] <= 0`), the function must select the slot with the minimum usage count so far (if there is a tie, pick the smallest slot index), increment its usage count, and output that slot's number. The function should return a vector of integers containing the output sequence of selected slot numbers (for both positive and non-positive queries) in the order they were processed. The slot numbers for positive queries are simply the query value itself; for non-positive queries, it's the chosen slot number (1-indexed). Assume `1 <= n <= 10^5`, `1 <= q <= 10^5`, and query values are within `[-n, n]` but never `0` (so `v[i] != 0`). If a positive query references a slot outside `[1, n]`, ignore it (output nothing for that query) and do not increment any usage count.
#include <cassert>
#include <vector>

// The solution function is defined above; here we test it.
int main() {
    // Test 1: Basic positive and negative queries
    {
        std::vector<int> q = {1, -1, -1, 2, -1};
        std::vector<int> res = processQueries(3, q);
        std::vector<int> expected = {1, 1, 2, 2, 1};
        assert(res == expected);
    }

    // Test 2: All non-positive queries cycle through slots
    {
        std::vector<int> q = {-1, -1, -1};
        std::vector<int> res = processQueries(3, q);
        std::vector<int> expected = {1, 2, 3};
        assert(res == expected);
    }

    // Test 3: Out-of-range positive ignored
    {
        std::vector<int> q = {5, -1, 1, -1};
        std::vector<int> res = processQueries(3, q);
        std::vector<int> expected = {1, 1, 1}; // first 5 ignored, then -1 picks 1, then 1, then -1 picks 1 again? Let's recompute:
        // Initial counts: all 0
        // q=5: ignored
        // q=-1: pick min (count=0, index=1) -> output 1, count[1]=1
        // q=1: positive valid, output 1, count[1]=2
        // q=-1: pick min (count=0, index=2) -> output 2, count[2]=1
        // So expected: {1,1,2}
        std::vector<int> expected2 = {1, 1, 2};
        assert(res == expected2);
    }

    // Test 4: Ties go to smallest index
    {
        std::vector<int> q = {-1, -1, -1, -1};
        std::vector<int> res = processQueries(2, q);
        std::vector<int> expected = {1, 2, 1, 2};
        assert(res == expected);
    }

    // Test 5: Mixed positive and negative, large n small q
    {
        std::vector<int> q = {10, -1, -1, -1};
        std::vector<int> res = processQueries(5, q);
        std::vector<int> expected = {1, 1, 2, 3}; 
        // 10 out of range ignored, then -1 picks 1, -1 picks 2? Actually after first -1 count[1]=1, min is 2 (count0), so output 2? Let's trace:
        // q=10 ignored
        // q=-1: pick 1, output 1, count[1]=1
        // q=-1: min is slot2 (count0), output 2, count[2]=1
        // q=-1: min is slot3, output 3
        // So expected {1,2,3} not {1,1,2,3}
        std::vector<int> expected_real = {1, 2, 3};
        assert(res == expected_real);
    }

    // Test 6: All positives in range
    {
        std::vector<int> q = {2, 1, 2, 3};
        std::vector<int> res = processQueries(3, q);
        std::vector<int> expected = {2, 1, 2, 3};
        assert(res == expected);
    }

    // Test 7: Single non-positive
    {
        std::vector<int> q = {-1};
        std::vector<int> res = processQueries(1, q);
        std::vector<int> expected = {1};
        assert(res == expected);
    }

    // Test 8: Empty queries
    {
        std::vector<int> q = {};
        std::vector<int> res = processQueries(5, q);
        std::vector<int> expected = {};
        assert(res.empty());
    }
}
#include <vector>
#include <set>
#include <utility>

// Simulate the slot selection process.
// Returns a vector of selected slot numbers in query order.
std::vector<int> processQueries(int n, const std::vector<int>& queries) {
    std::vector<int> result;
    std::vector<int> count(n + 1, 0); // 1-indexed, slot 0 unused

    // Set of (count, slot_index), sorted by count then index
    std::set<std::pair<int, int>> slots;
    for (int i = 1; i <= n; ++i) {
        slots.insert({0, i});
    }

    for (int v : queries) {
        if (v > 0) {
            // Positive query: if valid slot, update its count and output it
            if (v >= 1 && v <= n) {
                auto it = slots.find({count[v], v});
                if (it != slots.end()) {
                    slots.erase(it);
                    ++count[v];
                    slots.insert({count[v], v});
                    result.push_back(v);
                }
            }
            // If out of range, ignore (do nothing)
        } else {
            // Non-positive query: pick slot with minimal count (and smallest index)
            auto it = slots.begin();
            int chosen = it->second;
            slots.erase(it);
            ++count[chosen];
            slots.insert({count[chosen], chosen});
            result.push_back(chosen);
        }
    }

    return result;
}
// The solution maintains a frequency array `count` of size `n+1` (1-indexed) initialized to zero. For each query, if the value is positive and within `[1, n]`, we increment `count[value]` and push `value` to the output vector. If the value is negative (or any non-positive, but the spec says no zero), we must find the slot with the minimum count. A naive linear scan over all `n` slots for each such query would be `O(nq)`, too slow. Instead, we can maintain a sorted structure of slots by (count, index). Because counts only increase by one at a time, we can use a `set<pair<int,int>>` where the first element is the count and the second is the slot index. Initially, all slots have count 0, so the set contains `(0,1), (0,2), ..., (0,n)`. For a non-positive query, we take the first element of the set (which has smallest count, and among ties smallest index, because `std::set` sorts by pair lexicographically), increment its count by one, remove the old pair, insert the new pair `(count+1, index)`, and push `index` to output. For a positive query within range, we similarly locate its current pair `(old_count, value)` in the set, remove it, increment, insert the new pair, and push `value`. If the positive query is out of range, we simply skip it entirely (no set change, no output). This yields `O((n+q) log n)` time and `O(n)` space. Edge cases: multiple non-positive queries in a row will always pick the currently least-used slot, which after the first one will be `1` again if counts are balanced? Actually after picking slot 1, its count becomes 1, so the next non-positive picks slot 2 (since slot 1 now count 1 and slot 2 count 0). Works correctly. Out-of-range positives are ignored. The set automatically handles ties by index order.

/*
Write a standalone C++ function that simulates a sequence of replacement operations on a multiset of numbers. Given an initial list of `len` integers, followed by `q` operations, each operation specifies two integers `b` and `c`. For each operation, replace every occurrence of value `b` in the multiset with value `c` (i.e., remove all `b`s and add the same number of `c`s). After each operation, output the current sum of all elements in the multiset. The function should take as input the initial list and a vector of operation pairs (each pair `(b,c)`), and return a vector of the sums after each operation, in order. Values can be large (up to 10^9) and counts can accumulate, so use appropriate 64-bit types.
*/

#include <vector>
#include <unordered_map>

// Simulate replacement operations on a multiset and return sums after each operation.
// input: initial list of numbers, operations as pairs (b, c).
// Returns: vector of sums after each operation, in the same order as operations.
std::vector<long long> replacementSums(const std::vector<long long>& initial,
                                       const std::vector<std::pair<long long, long long>>& operations) {
    std::unordered_map<long long, long long> freq;
    long long totalSum = 0;

    // Build frequency map and initial total sum
    for (long long val : initial) {
        freq[val] += 1;
        totalSum += val;
    }

    std::vector<long long> results;
    results.reserve(operations.size());

    for (const auto& op : operations) {
        long long b = op.first;
        long long c = op.second;

        long long cnt = 0;
        auto it = freq.find(b);
        if (it != freq.end()) {
            cnt = it->second;
        }

        // Update sum: remove cnt * b, add cnt * c
        totalSum += cnt * (c - b);

        // Transfer counts: add cnt to c's frequency, set b's frequency to zero
        if (cnt > 0) {
            freq[c] += cnt;
            freq[b] = 0; // or erase, but zero is fine
        }

        results.push_back(totalSum);
    }

    return results;
}

#include <cassert>
#include <vector>
#include <utility>

// Assume replacementSums is defined above.

int main() {
    // Test 1: Simple replacement
    std::vector<long long> init1 = {1, 2, 3, 2};
    std::vector<std::pair<long long, long long>> ops1 = {{2, 5}};
    auto res1 = replacementSums(init1, ops1);
    assert(res1.size() == 1 && res1[0] == 1 + 5 + 3 + 5);
    // 14

    // Test 2: Multiple operations, including replacing a value not present
    std::vector<long long> init2 = {10, 10, 20};
    std::vector<std::pair<long long, long long>> ops2 = {{10, 30}, {99, 1}, {30, 5}};
    auto res2 = replacementSums(init2, ops2);
    assert(res2.size() == 3);
    assert(res2[0] == 30 + 30 + 20); // 80
    assert(res2[1] == 80); // b=99 not present, no change
    assert(res2[2] == 5 + 5 + 20); // two 30s become 5s -> 30
    // Actually: after ops2[0], multiset = {30,30,20}; after ops2[2], replace 30->5, so {5,5,20} sum=30

    // Test 3: Replace with same value (no change)
    std::vector<long long> init3 = {7, 7};
    std::vector<std::pair<long long, long long>> ops3 = {{7, 7}};
    auto res3 = replacementSums(init3, ops3);
    assert(res3.size() == 1 && res3[0] == 14);

    // Test 4: Combined counts after multiple transfers
    std::vector<long long> init4 = {1, 1, 2};
    std::vector<std::pair<long long, long long>> ops4 = {{1, 2}, {2, 3}};
    auto res4 = replacementSums(init4, ops4);
    // after first: {2,2,2} sum=6
    // after second: {3,3,3} sum=9
    assert(res4.size() == 2 && res4[0] == 6 && res4[1] == 9);

    // Test 5: Empty initial list (only operations, sums stay 0)
    std::vector<long long> init5 = {};
    std::vector<std::pair<long long, long long>> ops5 = {{5, 10}, {10, 1}};
    auto res5 = replacementSums(init5, ops5);
    assert(res5.size() == 2 && res5[0] == 0 && res5[1] == 0);

    // Test 6: Large numbers to check 64-bit correctness
    std::vector<long long> init6 = {1000000000LL, 1000000000LL};
    std::vector<std::pair<long long, long long>> ops6 = {{1000000000LL, 1LL}};
    auto res6 = replacementSums(init6, ops6);
    assert(res6.size() == 1 && res6[0] == 2);

    return 0;
}

// The core idea is to maintain a frequency map (`unordered_map` or `map`) that counts how many times each distinct value appears in the multiset. Also maintain the current total sum as a 64-bit integer. Initially, populate the frequency map by iterating through the input list, adding each element to the map and accumulating the sum. For each operation `(b, c)`: look up the count `cnt = freq[b]` (if `b` is not present, `cnt` is 0). Update the sum by adding `cnt * (c - b)` because we remove `cnt` copies of `b` and add `cnt` copies of `c`. Then, transfer the count: add `cnt` to `freq[c]` (this vector may combine with existing `c`s) and set `freq[b] = 0` (or erase it to avoid unnecessary memory). Append the new sum to the result vector. Edge cases: (1) If `b == c`, the operation does nothing—the sum change is zero and the frequency map remains effectively unchanged (though moving counts between same key is harmless); (2) If `b` never appeared, `cnt` is zero, sum change is zero, and `freq[c]` gets a zero addition—fine; (3) After many operations, counts can become large (up to `len` times), so use `long long` for frequencies and sums. Time complexity: O(len + q) (each operation is O(1) on average for unordered_map, O(log n) for map). Space complexity: O(distinct values) which is at most O(len + q) after transfers, but usually bounded by initial distinct values plus new `c` values.

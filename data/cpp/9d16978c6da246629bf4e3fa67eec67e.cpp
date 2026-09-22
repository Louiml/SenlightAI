/*
Write a C++ function `int lastRemainingElement(std::vector<int>& encoding, std::vector<int> queries)` that simulates a run-length encoded sequence of integers. The encoding is provided as a vector where pairs `(count, value)` represent `count` consecutive occurrences of `value`. For each query in the queries vector, the function processes `next(n)` operations: it consumes `n` elements from the sequence starting from the current position, and returns the value of the last consumed element, or `-1` if fewer than `n` elements remain. After all queries are processed, the function returns the result of the final `next` call (the last returned value, or `-1` if any query exhausted the sequence). The function must mutate the encoding vector to reflect consumption across successive queries.
*/
#include <vector>

// Simulates an RLE iterator over a sequence of (count, value) pairs.
// Queries consume n elements each; returns the value of the last consumed element,
// or -1 if insufficient elements remain. Mutates encoding to reflect consumption.
int lastRemainingElement(std::vector<int>& encoding, const std::vector<int>& queries) {
    size_t curr = 0; // current pair index (even indices hold counts)
    int lastResult = -1;
    
    for (int n : queries) {
        int remaining = n;
        int result = -1;
        bool found = false;
        
        while (curr < encoding.size() && remaining > 0) {
            int& count = encoding[curr];
            if (count == 0) {
                curr += 2; // skip empty bucket
                continue;
            }
            if (count < remaining) {
                remaining -= count;
                curr += 2; // fully consumed this bucket
            } else {
                // This bucket has enough elements
                count -= remaining;
                result = encoding[curr + 1];
                found = true;
                if (count == 0) {
                    curr += 2; // move past exhausted bucket
                }
                break;
            }
        }
        
        lastResult = found ? result : -1;
        if (!found) {
            // If a query fails, the iterator is exhausted, but we still return -1
            // and leave curr unchanged (it will be at end-of-vector).
        }
    }
    
    return lastResult;
}
#include <cassert>
#include <vector>

int main() {
    // Basic example: encoding = [3, 5, 2, 7] means 5,5,5,7,7
    {
        std::vector<int> enc = {3, 5, 2, 7};
        std::vector<int> queries = {2, 2};
        assert(lastRemainingElement(enc, queries) == 7);
        // After queries: consumed 3 fives and 1 seven, remaining: 0 fives, 1 seven
    }
    
    // Query exactly empties a bucket
    {
        std::vector<int> enc = {2, 10, 1, 20};
        std::vector<int> queries = {2, 1};
        assert(lastRemainingElement(enc, queries) == 20);
    }
    
    // Query larger than total remaining: should return -1
    {
        std::vector<int> enc = {1, 100, 1, 200};
        std::vector<int> queries = {2, 1};
        assert(lastRemainingElement(enc, queries) == -1);
    }
    
    // Zero-frequency buckets are skipped
    {
        std::vector<int> enc = {0, 99, 2, 42, 1, 7};
        std::vector<int> queries = {3};
        assert(lastRemainingElement(enc, queries) == 7);
    }
    
    // Multiple queries, exhausted after first query but second still returns -1
    {
        std::vector<int> enc = {1, 5};
        std::vector<int> queries = {1, 1};
        assert(lastRemainingElement(enc, queries) == -1);
    }
    
    // Single element query, multiple buckets
    {
        std::vector<int> enc = {1, 1, 1, 2, 1, 3};
        std::vector<int> queries = {1, 1, 1};
        assert(lastRemainingElement(enc, queries) == 3);
    }
    
    // Large query spanning multiple buckets
    {
        std::vector<int> enc = {2, 7, 3, 8, 1, 9};
        std::vector<int> queries = {5};
        assert(lastRemainingElement(enc, queries) == 9);
    }
    
    // Query of zero: no consumption, return -1 (no element consumed)
    {
        std::vector<int> enc = {5, 1};
        std::vector<int> queries = {0};
        assert(lastRemainingElement(enc, queries) == -1);
    }
    
    // Empty encoding: always -1
    {
        std::vector<int> enc = {};
        std::vector<int> queries = {1};
        assert(lastRemainingElement(enc, queries) == -1);
    }
    
    // All queries in one go: exhaust exactly
    {
        std::vector<int> enc = {4, 3, 4, 2};
        std::vector<int> queries = {2, 2, 2, 2};
        assert(lastRemainingElement(enc, queries) == 2);
    }
    
    return 0;
}
// The core idea is to maintain a pointer `curr` to the current pair index in the encoding, starting at index 0. For each query `n`, we iterate through pairs from `curr` onward. For each pair, if the remaining count is less than `n`, we subtract that count from `n` and move to the next pair (since that bucket is fully consumed). If the count is greater than or equal to `n`, we have found the bucket containing the last consumed element: we subtract `n` from that bucket's count, record the corresponding value, and if the bucket becomes empty, we advance `curr` past it for the next query. If we exhaust all pairs before `n` reaches zero, return `-1`. Edge cases include zero-frequency buckets (skip them silently), queries that exactly empty a bucket, and queries larger than the total remaining elements. The total complexity over all queries is O(total elements consumed + number of pairs), because each element is processed at most once across all `next` calls, and each pair is visited at most once.

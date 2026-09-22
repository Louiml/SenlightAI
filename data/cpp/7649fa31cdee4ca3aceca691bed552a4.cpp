Write a C++ function that takes a vector of 64-bit integers and returns the total number of "record" events, where a record event occurs each time an element is either strictly greater than the current maximum seen so far (starting from the first element) or strictly less than the current minimum seen so far (also starting from the first element). The first element does not count as a record event for either direction. The function should handle vectors of any length, including empty vectors (returning 0), and should not modify the input vector. Use `long long` for all integer types to avoid overflow.
// The solution iterates through the array starting from the second element (index 1). Maintain two variables: `current_max` initialized to the first element, and `current_min` also initialized to the first element. For each subsequent element, if it is strictly greater than `current_max`, increment a counter and update `current_max` to that element. Similarly, if it is strictly less than `current_min`, increment the same counter and update `current_min`. Note that an element can be both a new maximum and a new minimum simultaneously (which is impossible for a non-empty array with at least two distinct values, but if the array has only one element, the loop doesn't execute, so no issue). Edge cases: empty vector returns 0; single-element vector returns 0; strictly increasing/decreasing sequences cause counts of (n-1) each or combined; duplicates do not affect the count because strict comparisons are used. Time complexity is O(n) for a vector of size n, and auxiliary space is O(1) beyond the input vector.
#include <vector>
#include <cstdint>

// Counts the number of record events (new max or new min) in a vector of 64-bit integers.
// The first element is the initial reference and never counts as a record.
long long countRecordEvents(const std::vector<long long>& values) {
    if (values.size() <= 1) {
        return 0;
    }

    long long current_max = values[0];
    long long current_min = values[0];
    long long record_count = 0;

    for (std::size_t i = 1; i < values.size(); ++i) {
        const long long value = values[i];
        if (value > current_max) {
            current_max = value;
            ++record_count;
        }
        if (value < current_min) {
            current_min = value;
            ++record_count;
        }
    }

    return record_count;
}
#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is assumed to be declared above this main.

int main() {
    // Empty vector
    assert(countRecordEvents({}) == 0);
    
    // Single element
    assert(countRecordEvents({5}) == 0);
    
    // Strictly increasing: 4 new max records
    assert(countRecordEvents({1, 2, 3, 4, 5}) == 4);
    
    // Strictly decreasing: 4 new min records
    assert(countRecordEvents({5, 4, 3, 2, 1}) == 4);
    
    // Alternating high then low: each step is a new max or new min
    assert(countRecordEvents({0, 10, -5, 20, -10}) == 4); // 10 (max), -5 (min), 20 (max), -10 (min)
    
    // Duplicates do not count
    assert(countRecordEvents({3, 3, 3}) == 0);
    
    // Mixed with duplicates and records
    assert(countRecordEvents({2, 2, 5, 1, 5, 0}) == 3); // 5 (max), 1 (min), 0 (min)
    
    // Large values
    assert(countRecordEvents({-1000000000000LL, 1000000000000LL, -999999999999LL}) == 2); // first max, then min
}

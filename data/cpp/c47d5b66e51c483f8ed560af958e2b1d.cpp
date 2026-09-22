Given a vector of sorted vectors (each representing a sequence of unique integer landmark IDs), write a C++ function `size_t count_overlap(const std::vector<std::vector<size_t>>& sorted_lists)` that returns the total number of distinct landmark IDs that appear in more than one vector. Each inner vector is sorted in ascending order and contains no duplicate IDs. The outer vector may be empty, and inner vectors may be empty or contain any number of elements. The function must not modify the input, and must handle the case where a landmark appears in multiple vectors correctly counting each distinct ID only once. For example, if landmark 5 appears in three vectors, it should be counted as one shared landmark.
The core problem is counting how many distinct integers appear in at least two of the given sorted vectors. Because each vector is sorted, we can merge the vectors efficiently using a min-heap (priority queue). The heap stores pairs `(value, list_index)` where `value` is the current element from the list at `list_index`. We push one element from each non-empty list. Then we repeatedly pop the smallest pair. If the popped value equals the previous popped value, then this value is shared across at least two lists; we increment the count and skip all subsequent occurrences of the same value (by popping them) to ensure each value is counted once. After processing all occurrences of a value, we advance the iterator of each list that contributed that value, pushing the next element if it exists. Edge cases: empty input vector (return 0), empty inner vectors (ignored), and a value appearing only once (not counted). Time complexity: O(N log k) where N is total number of elements across all lists and k is the number of non-empty lists, since each element is pushed/popped at most once. Space complexity: O(k) for the heap and iterators.
#include <vector>
#include <queue>
#include <cstddef>

/**
 * Count the number of distinct integers that appear in more than one sorted vector.
 *
 * @param sorted_lists A vector of sorted vectors, each containing unique integers.
 * @return The number of distinct integers that occur in at least two vectors.
 */
size_t count_overlap(const std::vector<std::vector<size_t>>& sorted_lists) {
    using Pair = std::pair<size_t, size_t>; // (value, list_index)
    
    size_t num_lists = sorted_lists.size();
    
    // Min-heap: smaller value on top; if values equal, smaller index on top.
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> min_heap;
    
    // Iterators for each list.
    std::vector<std::vector<size_t>::const_iterator> it_vec;
    std::vector<std::vector<size_t>::const_iterator> it_end;
    it_vec.reserve(num_lists);
    it_end.reserve(num_lists);
    
    // Initialize iterators and push the first element of each non-empty list.
    for (size_t i = 0; i < num_lists; ++i) {
        const auto& lst = sorted_lists[i];
        it_vec.push_back(lst.begin());
        it_end.push_back(lst.end());
        if (it_vec[i] != it_end[i]) {
            min_heap.push({*it_vec[i], i});
        }
    }
    
    size_t overlap_count = 0;
    size_t prev_value = 0; // Dummy initialization.
    bool have_prev = false;
    
    while (!min_heap.empty()) {
        const auto [value, list_idx] = min_heap.top();
        min_heap.pop();
        
        if (have_prev && value == prev_value) {
            // This value appears again; it's shared.
            // Count only first time we encounter the second occurrence.
            // To avoid double counting, we'll mark it and skip all
            // subsequent occurrences until value changes.
            // We'll handle this by checking if prev_value was already counted.
            // Simpler: use a flag; only count once per distinct shared value.
            // Approach: when we pop a value that equals prev_value, we
            // increment count, then pop all remaining occurrences of the same
            // value without counting, then continue.
            ++overlap_count;
            // Pop all other occurrences of the same value.
            while (!min_heap.empty() && min_heap.top().first == value) {
                const auto [v2, idx2] = min_heap.top();
                min_heap.pop();
                // Advance that iterator.
                ++it_vec[idx2];
                if (it_vec[idx2] != it_end[idx2]) {
                    min_heap.push({*it_vec[idx2], idx2});
                }
            }
            have_prev = false; // reset because we've processed this value group
        } else {
            // New value encountered.
            prev_value = value;
            have_prev = true;
        }
        
        // Advance the iterator of the list that we just popped from.
        ++it_vec[list_idx];
        if (it_vec[list_idx] != it_end[list_idx]) {
            min_heap.push({*it_vec[list_idx], list_idx});
        }
    }
    
    return overlap_count;
}
#include <cassert>
#include <vector>
#include <cstddef>

// Assume the solution function is included above.

int main() {
    // Empty input
    assert(count_overlap({}) == 0);
    
    // Single empty vector
    assert(count_overlap({{}}) == 0);
    
    // All vectors empty
    assert(count_overlap({{}, {}, {}}) == 0);
    
    // No overlap
    std::vector<std::vector<size_t>> lists1 = {{1, 2, 3}, {4, 5}, {6}};
    assert(count_overlap(lists1) == 0);
    
    // One shared value between two vectors
    std::vector<std::vector<size_t>> lists2 = {{1, 2, 3}, {3, 4, 5}};
    assert(count_overlap(lists2) == 1);
    
    // Same value in three vectors counted once
    std::vector<std::vector<size_t>> lists3 = {{1, 2}, {2, 3}, {2, 4}, {5}};
    assert(count_overlap(lists3) == 1);
    
    // Multiple shared values, some only in two, some in three
    std::vector<std::vector<size_t>> lists4 = {{1, 2, 3}, {2, 3, 4}, {3, 5}};
    assert(count_overlap(lists4) == 2); // 2 and 3 are shared
    
    // Duplicates within a single vector are not allowed by spec, but function should handle
    // if present (though not required). We'll test without duplicates.
    
    // All values shared between two vectors
    std::vector<std::vector<size_t>> lists5 = {{10, 20}, {20, 30}, {40}};
    assert(count_overlap(lists5) == 1); // only 20
    
    // Larger overlap
    std::vector<std::vector<size_t>> lists6 = {{1, 2, 3, 4}, {2, 4, 6}, {4, 8}};
    assert(count_overlap(lists6) == 2); // 2 and 4
    
    // No overlap with multiple vectors
    std::vector<std::vector<size_t>> lists7 = {{1}, {2}, {3}, {4}};
    assert(count_overlap(lists7) == 0);
    
    // Single value vector with itself (not possible as separate vectors)
    // but test single list
    std::vector<std::vector<size_t>> lists8 = {{5, 6, 7}};
    assert(count_overlap(lists8) == 0);
    
    // Edge: two identical values across two vectors
    std::vector<std::vector<size_t>> lists9 = {{42}, {42}};
    assert(count_overlap(lists9) == 1);
    
    return 0;
}

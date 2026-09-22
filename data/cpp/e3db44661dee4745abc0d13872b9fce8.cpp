Implement a C++ function `insertSortedUnique` that maintains a sorted array of unique integer-distance pairs (represented as a simple struct with `id` and `distance` fields) by inserting a new candidate into the correct position based on ascending `distance`. The function must: (1) reject duplicates by `id` (returning without modification), (2) reject insertions when the array is full and the new candidate has a `distance` greater than or equal to the current maximum (last element), (3) otherwise shift elements right to make room, insert the candidate in sorted position, and maintain a `capacity` limit (if full, the largest element is dropped). The function should operate on a dynamic vector that grows as needed up to a given `capacity` but never exceeds it, and it must handle an initially empty array. The input is a reference to a vector of candidates, a capacity limit, and the new candidate's `id` and `distance`. The function returns `true` if insertion occurred, `false` if rejected (duplicate or full-and-not-better). The array must remain sorted by `distance` ascending after each successful insertion. Use `int32_t` for ids and `float` for distances, and define a `Candidate` struct.

#include <cassert>
#include <vector>
#include <cstdint>

// Assume Candidate and insertSortedUnique are defined above (omitted here for brevity)

int main() {
    std::vector<Candidate> vec;
    
    // Insert into empty
    assert(insertSortedUnique(vec, 5, 10, 2.0f) == true);
    assert(vec.size() == 1 && vec[0].id == 10 && vec[0].distance == 2.0f);
    
    // Insert larger distance at end
    assert(insertSortedUnique(vec, 5, 20, 4.0f) == true);
    assert(vec.size() == 2 && vec[1].id == 20);
    
    // Insert smaller distance at beginning
    assert(insertSortedUnique(vec, 5, 30, 1.0f) == true);
    assert(vec.size() == 3 && vec[0].id == 30 && vec[1].id == 10 && vec[2].id == 20);
    // Distances: 1.0, 2.0, 4.0
    
    // Duplicate id rejected
    assert(insertSortedUnique(vec, 5, 10, 5.0f) == false);
    assert(vec.size() == 3);
    
    // Insert middle
    assert(insertSortedUnique(vec, 5, 40, 3.0f) == true);
    assert(vec.size() == 4 && vec[2].id == 40 && vec[2].distance == 3.0f);
    // Distances: 1.0, 2.0, 3.0, 4.0
    
    // Fill to capacity (5)
    assert(insertSortedUnique(vec, 5, 50, 0.5f) == true);
    assert(vec.size() == 5 && vec[0].id == 50 && vec[0].distance == 0.5f);
    // Distances: 0.5, 1.0, 2.0, 3.0, 4.0
    
    // Full, new is better than last -> should insert and drop last (id 20, distance 4.0)
    assert(insertSortedUnique(vec, 5, 60, 3.5f) == true);
    assert(vec.size() == 5);
    assert(vec[4].id == 60 && vec[4].distance == 3.5f);
    // Check that old last (4.0) is gone, and 60 is now at position 4
    
    // Full, new is worse or equal to last -> reject
    assert(insertSortedUnique(vec, 5, 70, 4.0f) == false); // equal to last
    assert(insertSortedUnique(vec, 5, 80, 5.0f) == false); // worse
    assert(vec.size() == 5);
    
    // Duplicate id even if better distance (still rejected by id)
    assert(insertSortedUnique(vec, 5, 60, 0.1f) == false);
    
    return 0;
}

#include <vector>
#include <cstdint>

struct Candidate {
    int32_t id;
    float distance;
    
    Candidate(int32_t id_, float distance_) : id(id_), distance(distance_) {}
    
    bool operator<(const Candidate& other) const {
        return distance < other.distance;
    }
};

// Insert (id, distance) into candidates maintaining sorted order by distance ascending.
// Capacity is the maximum number of candidates allowed.
// Returns true if inserted, false if rejected (duplicate id or full and not better).
bool insertSortedUnique(std::vector<Candidate>& candidates, int32_t capacity, int32_t id, float distance) {
    // Reject duplicates by id
    for (const auto& c : candidates) {
        if (c.id == id) return false;
    }
    
    // If full and new candidate is not better than the current worst (last element)
    if (static_cast<int32_t>(candidates.size()) == capacity && 
        !(distance < candidates.back().distance)) {
        return false;
    }
    
    Candidate new_candidate(id, distance);
    
    // Binary search for insertion position (first element with distance > new_candidate.distance)
    int32_t lo = 0;
    int32_t hi = static_cast<int32_t>(candidates.size());
    while (lo < hi) {
        int32_t mid = (lo + hi) / 2;
        if (new_candidate < candidates[mid]) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    int32_t pos = lo;
    
    // If at capacity, we must drop the last element when inserting at pos <= size-1
    if (static_cast<int32_t>(candidates.size()) == capacity) {
        // Shift elements from pos to size-2 one position right
        for (int32_t i = static_cast<int32_t>(candidates.size()) - 1; i > pos; --i) {
            candidates[i] = candidates[i - 1];
        }
        candidates[pos] = new_candidate;
    } else {
        // Shift elements from pos to size-1 one position right, then insert
        candidates.resize(candidates.size() + 1);
        for (int32_t i = static_cast<int32_t>(candidates.size()) - 1; i > pos; --i) {
            candidates[i] = candidates[i - 1];
        }
        candidates[pos] = new_candidate;
    }
    return true;
}

// The solution maintains a vector of candidates sorted by `distance` ascending. For each insertion: first check if the candidate's `id` already exists in the array via a linear scan (since typical usage has small sizes, O(n) is acceptable). If duplicate, return false. Then, if the array is already at capacity and the new candidate's distance is greater than or equal to the last element's distance, reject (return false) because it would not improve the set. Otherwise, find the insertion position using binary search on distance (with tie-breaking on id if distances equal; but for simplicity, we can use a stable sort and just compare distance, and if equal, we keep the existing one by rejecting duplicates via id scan already). After finding the position `pos` (where the new candidate should be inserted), if the array is at capacity, we need to shift all elements from `pos` to `capacity-2` one position right, then place the new candidate at `pos`, effectively dropping the last element. If not full, shift from `pos` to `size-1` right, then place, and increment size. The vector size never exceeds `capacity` (ensure we resize up to capacity+1 to allow shifting, but logically keep size ≤ capacity). Edge cases: insert into empty array; insert at beginning (pos=0); insert at end when not full; insert into full array when new is better than some existing but worse than last (e.g., full, new is better than last, we replace last). Time complexity: O(n) for duplicate check + O(log n) for binary search + O(n) for shifting, so O(n). Space complexity: O(1) auxiliary beyond the vector (which is provided).

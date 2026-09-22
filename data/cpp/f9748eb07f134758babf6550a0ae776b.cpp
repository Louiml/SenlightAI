// Write a C++ function that performs a sequential search on an integer array to determine whether a given target value exists and, if so, returns the index of its first occurrence. The function must accept a constant reference to a `std::vector<int>` and a target integer, and return an `int` index if found, or `-1` if not found. The search must scan from the beginning of the vector, stop at the first match, and handle empty vectors correctly. Do not use any standard library search algorithms—implement the linear scan manually.
#include <cassert>
#include <vector>

int main() {
    std::vector<int> a = {3, 4, 2, 1, 5};
    
    // Target present in the middle
    assert(sequentialSearch(a, 4) == 1);
    // Target at the end
    assert(sequentialSearch(a, 5) == 4);
    // Target at the beginning
    assert(sequentialSearch(a, 3) == 0);
    // Target not present
    assert(sequentialSearch(a, 10) == -1);
    
    // Empty vector
    std::vector<int> empty;
    assert(sequentialSearch(empty, 1) == -1);
    
    // Duplicate values – returns first index
    std::vector<int> dup = {7, 5, 7, 9};
    assert(sequentialSearch(dup, 7) == 0);
    
    // Single element vector
    std::vector<int> single = {42};
    assert(sequentialSearch(single, 42) == 0);
    assert(sequentialSearch(single, 0) == -1);
    
    return 0;
}
#include <vector>

// Perform a sequential search for `target` in `data`.
// Returns the index of the first occurrence, or -1 if not found.
int sequentialSearch(const std::vector<int>& data, int target) {
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
// The solution iterates through the vector using a simple `for` loop from index 0 to `size()-1`. For each element, compare it with the target; if equal, return the current index immediately (first match). If the loop completes without a match, return `-1`. This approach naturally handles empty vectors (loop never runs, returns `-1`). Time complexity is O(n) in the worst case (target absent or at end), O(1) in the best case (target at first element). Space complexity is O(1) auxiliary. Edge cases: empty vector, target present multiple times (returns first index), target not present, and target at the last position.

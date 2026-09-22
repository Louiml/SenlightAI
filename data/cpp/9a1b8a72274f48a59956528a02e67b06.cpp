Given a sorted vector of unique `uint16_t` tag values (sorted in ascending order), write a C++ function that efficiently finds the index of a given tag using binary search. If the tag is not found, return `-1`. The input is a `std::vector<uint16_t>` and the target tag is a `uint16_t`. The function must be `const`-qualified for the vector parameter (i.e., accept a `const std::vector<uint16_t>&`). You must not use `std::binary_search` or `std::lower_bound`; implement the binary search manually. Handle empty vectors and edge cases such as the target being the smallest, largest, or absent.

#include <cassert>
#include <cstdint>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // Basic cases
    std::vector<uint16_t> tags1 = {100, 200, 300, 400, 500};
    assert(findTagIndex(tags1, 300) == 2);
    assert(findTagIndex(tags1, 100) == 0);
    assert(findTagIndex(tags1, 500) == 4);

    // Element not present
    assert(findTagIndex(tags1, 250) == -1);
    assert(findTagIndex(tags1, 0) == -1);
    assert(findTagIndex(tags1, 600) == -1);

    // Edge cases: single element
    std::vector<uint16_t> tags2 = {42};
    assert(findTagIndex(tags2, 42) == 0);
    assert(findTagIndex(tags2, 43) == -1);

    // Edge case: empty vector
    std::vector<uint16_t> tags3;
    assert(findTagIndex(tags3, 1) == -1);

    // Larger vector with even and odd size
    std::vector<uint16_t> tags4 = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    assert(findTagIndex(tags4, 10) == 0);
    assert(findTagIndex(tags4, 100) == 9);
    assert(findTagIndex(tags4, 55) == -1);
    assert(findTagIndex(tags4, 90) == 8);

    // Boundaries around present elements
    assert(findTagIndex(tags4, 9) == -1);
    assert(findTagIndex(tags4, 11) == -1);
    assert(findTagIndex(tags4, 101) == -1);

    return 0;
}

#include <cstdint>   // for uint16_t
#include <vector>    // for std::vector

// Finds the index of 'tag' in the sorted 'tags' vector using binary search.
// Returns the index if found, otherwise -1.
// The input vector must be sorted in ascending order and contain unique values.
int findTagIndex(const std::vector<uint16_t>& tags, uint16_t tag) {
    int low = 0;
    int high = static_cast<int>(tags.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;  // Avoid overflow
        uint16_t current = tags[static_cast<size_t>(mid)];

        if (current == tag) {
            return mid;
        } else if (current < tag) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

// The core algorithm is classic binary search on a sorted, unique sequence. Maintain two indices `low` and `high` representing the inclusive search interval `[low, high]`. While `low <= high`, compute the midpoint `mid = low + (high - low) / 2` (this avoids integer overflow compared to `(low+high)/2`). Compare the element at `mid` with the target: if equal, return `mid`; if the element is less than the target, move `low` to `mid + 1`; otherwise move `high` to `mid - 1`. If the loop ends without finding the target, return `-1`. Edge cases: if the vector is empty, the loop condition fails immediately and returns `-1`; if the target is less than the first element, the search rapidly narrows to `high` becoming less than `low`, and returns `-1`; duplicates are not present per constraint, so no special handling is needed. Time complexity is `O(log n)`, where `n` is the size of the vector. Space complexity is `O(1)` auxiliary, since we only use a few integer variables.

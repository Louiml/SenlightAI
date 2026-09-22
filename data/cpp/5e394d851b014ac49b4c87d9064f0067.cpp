Write a standalone C++ function that determines whether a given vector of integers can be rearranged to form an arithmetic progression. A sequence forms an arithmetic progression if the difference between consecutive terms is constant after sorting. The function should accept a `const std::vector<int>&` (the input may be empty, contain 0, 1, or 2 elements, or include duplicate values and negative numbers) and return a `bool` indicating whether the elements can form an arithmetic progression. For empty or single-element vectors, return `true` (since they trivially form an AP). The function must not modify the input vector.

// The standard approach is to sort a copy of the input array, since after sorting, any arithmetic progression will have identical consecutive differences. However, since the input is passed by `const` reference, we must create a local copy to sort. Once sorted, compute the difference between the first two elements; if the vector has fewer than 2 elements, return `true` immediately. Then iterate from index 2 to the end, checking that each adjacent pair has the same difference as the first. If any pair differs, return `false`; otherwise, return `true`. Edge cases: empty vector (`size()==0`) → `true`; single element → `true`; two elements → always `true` (any two numbers form an AP); duplicates are handled naturally because sorting groups equal values, and the difference remains constant. The primary complexity is the sorting step: \(O(n \log n)\) time for \(n\) elements, with \(O(n)\) auxiliary space for the copy (or \(O(1)\) extra if sorting in-place were allowed, but here we copy). The iteration itself is \(O(n)\).

#include <vector>
#include <algorithm>

// Returns true if the given integers can be arranged into an arithmetic progression.
// Empty and single-element vectors are considered valid progressions.
bool canFormArithmeticProgression(const std::vector<int>& arr) {
    if (arr.size() < 2) {
        return true;
    }

    // Work on a copy since the input is const.
    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());

    const int diff = sorted[1] - sorted[0];

    for (std::size_t i = 2; i < sorted.size(); ++i) {
        if (sorted[i] - sorted[i - 1] != diff) {
            return false;
        }
    }

    return true;
}

#include <cassert>
#include <vector>

int main() {
    // Empty and single-element cases
    assert(canFormArithmeticProgression({}) == true);
    assert(canFormArithmeticProgression({5}) == true);

    // Two elements always form an AP
    assert(canFormArithmeticProgression({3, 7}) == true);

    // Standard cases
    assert(canFormArithmeticProgression({3, 5, 1}) == true);          // sorted: 1,3,5 (diff 2)
    assert(canFormArithmeticProgression({1, 2, 4}) == false);         // sorted: 1,2,4 (not constant diff)
    assert(canFormArithmeticProgression({9, -3, 3, 15}) == true);     // sorted: -3,3,9,15 (diff 6)

    // Duplicates
    assert(canFormArithmeticProgression({7, 7, 7}) == true);          // diff 0
    assert(canFormArithmeticProgression({1, 1, 2}) == false);         // sorted: 1,1,2 (diff 0 then 1)

    // Negative numbers and larger size
    assert(canFormArithmeticProgression({-10, -20, -30}) == true);    // sorted: -30,-20,-10 (diff 10)
    assert(canFormArithmeticProgression({1, 3, 5, 7, 9}) == true);
    assert(canFormArithmeticProgression({1, 3, 5, 8}) == false);

    return 0;
}

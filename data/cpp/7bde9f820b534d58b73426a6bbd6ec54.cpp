Write a C++ function named `sortTutorsByRating` that takes a vector of `Tutor` structs (where `Tutor` contains at least an `int tutID` and a `double rating` member) and sorts the vector in ascending order of `rating` using an insertion sort algorithm. The function must handle three cases distinctly: if the vector is empty, it should do nothing and return silently; if the vector has exactly one element, it should leave it unchanged; otherwise, it should sort the vector in place. After sorting, the function should return nothing (void). The sort must be stable, meaning tutors with equal ratings retain their original relative order. The function should be `const`-correct with respect to the input vector (i.e., it may only modify the vector parameter by reference, not any external data). You may assume the `Tutor` struct is defined before your function, but your solution must provide the struct definition as part of the standalone code for the task. The function should live in a self-contained file with necessary headers and no `main` function.

The solution uses the classic insertion sort algorithm, which is appropriate here because the task explicitly requires it and it is stable by nature (when implemented correctly, it preserves the order of equal elements). The function begins by checking `tutors.empty()`; if true, it returns immediately. For a single-element vector, the sole element is already sorted, so we return after doing nothing. For vectors with size ≥ 2, we iterate from index 1 to `n-1`. For each current element at index `i`, we compare it backwards with previous elements. As long as the previous element's rating is greater than the current element's rating, we shift the previous element one position to the right. Once we find the correct position (either we reach the start or the previous rating is less than or equal to the current rating), we place the current element there. This shifting mechanism ensures stability because we only move an element past another if the other's rating is strictly greater, not when equal. Time complexity: worst-case O(n²) when the array is in reverse order, best-case O(n) when already sorted, average O(n²). Space complexity is O(1) auxiliary, since we only use a temporary `Tutor` variable for swapping during sorting. Important edge cases: empty vector, single-element vector, all elements already sorted, reverse-sorted, and duplicate ratings.

#include <vector>
#include <cstddef>

// Tutor struct definition (minimal for sorting by rating)
struct Tutor {
    int tutID;
    double rating;
};

// Sorts the given vector of Tutor objects in-place by ascending rating
// using a stable insertion sort. Handles empty and single-element cases.
void sortTutorsByRating(std::vector<Tutor>& tutors) {
    if (tutors.empty() || tutors.size() == 1) {
        return; // nothing to sort
    }

    // Insertion sort: for each element from index 1 onward,
    // shift larger-rated elements to the right to make room.
    for (std::size_t i = 1; i < tutors.size(); ++i) {
        Tutor current = tutors[i];
        std::size_t j = i;
        // Move backwards while previous rating is greater than current's.
        while (j > 0 && tutors[j - 1].rating > current.rating) {
            tutors[j] = tutors[j - 1];
            --j;
        }
        tutors[j] = current;
    }
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty vector
    std::vector<Tutor> empty;
    sortTutorsByRating(empty);
    assert(empty.empty());

    // Test 2: Single element
    std::vector<Tutor> single{ {1, 4.5} };
    sortTutorsByRating(single);
    assert(single.size() == 1 && single[0].tutID == 1 && single[0].rating == 4.5);

    // Test 3: Already sorted
    std::vector<Tutor> sorted{ {1, 1.0}, {2, 2.0}, {3, 3.0} };
    sortTutorsByRating(sorted);
    assert(sorted[0].tutID == 1 && sorted[1].tutID == 2 && sorted[2].tutID == 3);

    // Test 4: Reverse sorted
    std::vector<Tutor> reverse{ {3, 3.0}, {2, 2.0}, {1, 1.0} };
    sortTutorsByRating(reverse);
    assert(reverse[0].tutID == 1 && reverse[1].tutID == 2 && reverse[2].tutID == 3);

    // Test 5: Duplicate ratings (stability check)
    std::vector<Tutor> dup{ {2, 2.0}, {1, 1.0}, {3, 2.0} };
    sortTutorsByRating(dup);
    assert(dup[0].tutID == 1); // rating 1.0
    // The two tutors with rating 2.0 should keep original order: tutID 2 before 3
    assert(dup[1].tutID == 2 && dup[2].tutID == 3);

    // Test 6: Random mix
    std::vector<Tutor> mix{ {5, 5.5}, {1, 1.1}, {4, 4.4}, {2, 2.2}, {3, 3.3} };
    sortTutorsByRating(mix);
    for (int i = 0; i < 5; ++i) {
        assert(mix[i].tutID == i + 1);
    }

    // Test 7: All equal ratings (stability preserved)
    std::vector<Tutor> equal{ {7, 1.0}, {3, 1.0}, {9, 1.0} };
    sortTutorsByRating(equal);
    assert(equal[0].tutID == 7 && equal[1].tutID == 3 && equal[2].tutID == 9);

    return 0;
}

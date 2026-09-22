Write a C++ function named `sortingDoneMessage` that takes a vector of integers as input and returns a string confirming that sorting has completed. The function must not actually sort the vector, but instead observe that the input represents an arbitrary sequence of numbers and produce the output format `"Sorting... done!"` for each call. Since the original problem reads multiple test cases, your function should operate on a single vector per call and return the fixed confirmation string. The vector may be empty, and if it is empty, the function should still return the same confirmation message, because there are no elements to process and sorting trivially completes.
The core idea is that the problem only requires acknowledging that sorting is finished, not performing any actual sorting work. Therefore, the function simply ignores the contents of the vector entirely and returns the constant string `"Sorting... done!"`. This works for any input size, including empty vectors. The only edge case to consider is an empty vector—this is handled naturally by returning the same message, as there is nothing to iterate over. The function should accept the vector by const reference to avoid unnecessary copying and to respect const correctness. Time complexity is O(1) because we do not iterate over the vector at all; even if we did check the size, it is O(1) to obtain the size. Space complexity is O(1) besides the returned string, which is constant length.
#include <string>
#include <vector>

// Returns a fixed message confirming that sorting has completed.
// The input vector is intentionally ignored because no actual sorting is required.
std::string sortingDoneMessage(const std::vector<int>& /*numbers*/) {
    return "Sorting... done!";
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Non-empty vector with arbitrary integers
    std::vector<int> vec1 = {5, 2, 9, 1};
    assert(sortingDoneMessage(vec1) == "Sorting... done!");

    // Single element vector
    std::vector<int> vec2 = {42};
    assert(sortingDoneMessage(vec2) == "Sorting... done!");

    // Empty vector (trivially sorted)
    std::vector<int> vec3;
    assert(sortingDoneMessage(vec3) == "Sorting... done!");

    // Negative numbers
    std::vector<int> vec4 = {-10, -3, -7};
    assert(sortingDoneMessage(vec4) == "Sorting... done!");

    // Large vector with duplicates
    std::vector<int> vec5(1000, 7);
    assert(sortingDoneMessage(vec5) == "Sorting... done!");

    // Vector with mixed signs and zero
    std::vector<int> vec6 = {0, -1, 1, -100, 100};
    assert(sortingDoneMessage(vec6) == "Sorting... done!");

    // All identical elements
    std::vector<int> vec7 = {8, 8, 8};
    assert(sortingDoneMessage(vec7) == "Sorting... done!");

    // Very large single element
    std::vector<int> vec8 = {1000000000};
    assert(sortingDoneMessage(vec8) == "Sorting... done!");

    // Two elements reverse sorted
    std::vector<int> vec9 = {3, 1};
    assert(sortingDoneMessage(vec9) == "Sorting... done!");

    // Empty vector again to confirm consistency
    std::vector<int> vec10;
    assert(sortingDoneMessage(vec10) == "Sorting... done!");
}

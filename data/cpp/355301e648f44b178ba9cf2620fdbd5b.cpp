/*
Write a C++ function `classifySequence` that takes a `std::vector<int>` of exactly eight integers as input and returns a `std::string` with value `"ascending"`, `"descending"`, or `"mixed"`. The function must check if the input vector is exactly equal to `{1, 2, 3, 4, 5, 6, 7, 8}` (ascending), exactly equal to `{8, 7, 6, 5, 4, 3, 2, 1}` (descending), or neither (mixed). The function should handle a vector of any size gracefully: if the vector size is not exactly 8, return `"mixed"`. You may assume the input contains only integers, but they can be any values (including duplicates, negatives, or zeros). The function must be const-correct, efficient, and self-contained (no global variables, no external dependencies beyond standard headers).
*/

#include <vector>
#include <string>

// Classify a sequence as ascending, descending, or mixed.
// Returns "ascending" if the input equals {1,2,3,4,5,6,7,8}.
// Returns "descending" if the input equals {8,7,6,5,4,3,2,1}.
// Returns "mixed" otherwise, including when the size is not 8.
std::string classifySequence(const std::vector<int>& seq) {
    const std::vector<int> ascending = {1, 2, 3, 4, 5, 6, 7, 8};
    const std::vector<int> descending = {8, 7, 6, 5, 4, 3, 2, 1};
    
    if (seq.size() != ascending.size()) {
        return "mixed";
    }
    
    if (seq == ascending) {
        return "ascending";
    }
    if (seq == descending) {
        return "descending";
    }
    return "mixed";
}

#include <cassert>
#include <vector>
#include <string>

// Declaration for the solution function
std::string classifySequence(const std::vector<int>& seq);

int main() {
    // Exact ascending
    assert(classifySequence({1, 2, 3, 4, 5, 6, 7, 8}) == "ascending");
    // Exact descending
    assert(classifySequence({8, 7, 6, 5, 4, 3, 2, 1}) == "descending");
    // Mixed: not matching either
    assert(classifySequence({1, 2, 3, 4, 5, 6, 7, 9}) == "mixed");
    // Mixed: reversed but not exact (different value)
    assert(classifySequence({8, 7, 6, 5, 4, 3, 2, 0}) == "mixed");
    // Mixed: size too short
    assert(classifySequence({1, 2, 3, 4, 5, 6, 7}) == "mixed");
    // Mixed: size too long
    assert(classifySequence({1, 2, 3, 4, 5, 6, 7, 8, 9}) == "mixed");
    // Mixed: empty vector
    assert(classifySequence({}) == "mixed");
    // Mixed: duplicates (but no match)
    assert(classifySequence({1, 1, 1, 1, 1, 1, 1, 1}) == "mixed");
    // Mixed: all zeros
    assert(classifySequence({0, 0, 0, 0, 0, 0, 0, 0}) == "mixed");
    // Mixed: nearly ascending but first element wrong
    assert(classifySequence({0, 2, 3, 4, 5, 6, 7, 8}) == "mixed");
    return 0;
}

// The solution approach is straightforward: compare the input vector against two pre-defined constant vectors representing the ascending and descending sequences. Since the problem specifies exactly 8 integers for a meaningful check, we first verify the size—if the size is not 8, the sequence is automatically "mixed". For size 8, we can use direct equality comparison (`==`) between `std::vector<int>` objects, which is provided by the standard library and works element-wise. Edge cases include: empty vector, vector of size less than or greater than 8, vectors with duplicate values, and vectors that are partially matching the sequences (e.g., `{1,2,3,4,5,6,7,9}`) — all should return "mixed". The time complexity is O(1) because we only compare fixed-size vectors (maximum 8 elements). The space complexity is O(1) as we only store the constant reference vectors (which are fixed-size) and no additional data structures.

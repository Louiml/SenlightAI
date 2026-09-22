// Write a C++ function that accepts three subject marks (integers) and provides read-only indexed access to them via the `operator[]` semantics, exactly as demonstrated in the provided snippet. Your function should be named `getMarkAt` and take four parameters: three integer marks and an integer index `pos`. It must return the mark at that position (0, 1, or 2) and throw an `std::out_of_range` exception if the index is outside `[0,2]`. The function must be `const`-correct (do not modify input) and handle all integer values including negatives.
The task is straightforward but emphasizes correct bounds checking and const correctness. The solution approach: simply validate the index against the range [0,2]. If valid, return the corresponding mark; if invalid, throw `std::out_of_range` with a descriptive message. Since we’re not using a class, we pass the three marks explicitly. Edge cases: index exactly 0, 1, 2 (valid), and any other integer (invalid). Also negative indices must be rejected. Time complexity is O(1) because we do a constant number of comparisons and returns. Space complexity is O(1) since no extra memory is used beyond input parameters.
#include<stdexcept>

// Return the mark at the given index (0,1,2) from three marks.
// Throws std::out_of_range if the index is not in [0,2].
int getMarkAt(int sub0, int sub1, int sub2, int pos) {
    if (pos == 0) return sub0;
    if (pos == 1) return sub1;
    if (pos == 2) return sub2;
    throw std::out_of_range("Index must be 0, 1, or 2");
}
#include <cassert>
#include <stdexcept>

// Declaration of the function under test
int getMarkAt(int sub0, int sub1, int sub2, int pos);

int main() {
    // Valid indices return correct values
    assert(getMarkAt(34, 38, 43, 0) == 34);
    assert(getMarkAt(34, 38, 43, 1) == 38);
    assert(getMarkAt(34, 38, 43, 2) == 43);
    
    // Negative values should work fine
    assert(getMarkAt(-5, -10, -15, 0) == -5);
    assert(getMarkAt(-5, -10, -15, 1) == -10);
    assert(getMarkAt(-5, -10, -15, 2) == -15);
    
    // Invalid indices throw
    bool threw = false;
    try { getMarkAt(1,2,3,3); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
    
    threw = false;
    try { getMarkAt(1,2,3,-1); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
    
    threw = false;
    try { getMarkAt(1,2,3,100); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
    
    return 0;
}

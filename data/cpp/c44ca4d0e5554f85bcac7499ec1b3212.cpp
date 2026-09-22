Write a standalone C++ function `reshapeArray` that takes a vector of integers (representing a flattened array), a target row count, and a target column count, and returns a new vector that simulates the reshaped 2D array by preserving the order of elements. The function must validate that the product of rows and columns equals the input vector's size, and return an empty vector if the rows or columns are zero or negative. The reshaped result should be returned as a flat vector representing the rows concatenated in order, and the original input vector must remain unmodified.
The core algorithm is straightforward: first validate that `rows >= 0` and `cols >= 0`, and if either is zero, return an empty vector. Then check that `rows * cols` equals `input.size()`, and if not, return an empty vector (or throw an exception; the task requires returning an empty vector on failure for simplicity). If valid, simply return a copy of the input vector, because the flat representation of the reshaped array is identical to the original—reshaping only changes the conceptual dimensions, not the underlying data order. The key edge cases are: zero dimensions (return empty), mismatched total element count (return empty), and negative dimensions (treat as invalid, return empty). The input is passed by const reference to ensure it is not modified. Time complexity is O(n) where n is the input size (due to copying), and space complexity is O(n) for the returned vector.
#include <vector>

// Reshape a flat vector into a new flat vector with given rows and columns.
// If rows or cols are <= 0, or if rows*cols != input.size(), returns an empty vector.
// The original input is not modified.
std::vector<int> reshapeArray(const std::vector<int>& input, int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        return {};
    }
    if (static_cast<size_t>(rows) * static_cast<size_t>(cols) != input.size()) {
        return {};
    }
    // The flat representation of the reshaped array is the same as the input.
    return input;
}
#include <cassert>
#include <vector>

int main() {
    // Basic resize: 2x3 -> 3x2 (same flat data)
    std::vector<int> a = {1, 2, 3, 4, 5, 6};
    assert(reshapeArray(a, 3, 2) == a);
    
    // Same dimensions
    assert(reshapeArray(a, 2, 3) == a);
    
    // Single element reshape
    std::vector<int> b = {42};
    assert(reshapeArray(b, 1, 1) == b);
    
    // Zero rows returns empty
    assert(reshapeArray(a, 0, 3).empty());
    
    // Zero columns returns empty
    assert(reshapeArray(a, 2, 0).empty());
    
    // Negative rows returns empty
    assert(reshapeArray(a, -1, 3).empty());
    
    // Mismatched total elements returns empty
    assert(reshapeArray(a, 2, 4).empty());
    
    // Large reshape: 1x6 -> 6x1
    assert(reshapeArray(a, 6, 1) == a);
    
    // Input is not modified
    std::vector<int> original = {10, 20, 30};
    reshapeArray(original, 3, 1);
    assert(original == std::vector<int>({10, 20, 30}));
    
    // Empty input with valid dimensions returns empty (since 0 != rows*cols)
    std::vector<int> empty;
    assert(reshapeArray(empty, 1, 1).empty());
    
    return 0;
}

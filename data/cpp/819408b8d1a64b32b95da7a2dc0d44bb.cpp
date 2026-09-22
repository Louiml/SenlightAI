// Write a C++ function named `searchInSortedMatrix` that takes a 2D vector of integers (where each row is sorted in ascending order and the first element of each row is greater than the last element of the previous row) and a target integer. The function must return `true` if the target exists in the matrix, otherwise `false`. The matrix is guaranteed to be non-empty (at least one row and one column). You must solve it with a two-step binary search approach: first binary search on the rows to find the potential row using the last element of each row, then binary search within that row. Handle edge cases where the target is less than the smallest element or greater than the largest element without accessing out-of-bounds indices.
// The matrix has a special sorted property: rows are individually sorted, and the matrix is sorted column-wise as well because the last element of row `i` is less than the first element of row `i+1`. This allows a two-phase binary search.  
// - First, perform a binary search over the rows. For each middle row `mid`, compare `matrix[mid][lastColumn]` (the last element) with the target. If equal, return true. If the last element is greater than the target, then the target might be in this row or an earlier row, so set `end = mid - 1`. If the last element is less than the target, then the target must be in a later row, so set `start = mid + 1`.  
// - After the loop, if the target was not found, the row to search is `start` (since the loop exits when `start > end`; `start` is the first row whose last element is ≥ target, or if all last elements are < target, `start` would equal `r` which is out of bounds). To avoid an out-of-bounds access, check if `start < r` before calling the row-search.  
// - Then perform a standard binary search on that row.  
// - Edge cases: empty matrix is not allowed, but a matrix with one row or one column is fine. If the target is smaller than the very first element or larger than the very last element, the row search will either return false immediately because `start` reaches `r` or the row binary search fails. Time complexity: O(log rows + log cols). Space complexity: O(1) auxiliary.
#include <vector>

// Returns true if target exists in the matrix.
bool searchInSortedMatrix(const std::vector<std::vector<int>>& matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) return false;
    
    int rows = matrix.size();
    int cols = matrix[0].size();
    int start = 0;
    int end = rows - 1;
    
    // Binary search for the potential row based on last element.
    while (start <= end) {
        int mid = start + (end - start) / 2;
        int lastVal = matrix[mid][cols - 1];
        if (lastVal == target) return true;
        if (lastVal > target) {
            end = mid - 1;
        } else {
            start = mid + 1;
        }
    }
    
    // If start is out of bounds, target is larger than all elements.
    if (start >= rows) return false;
    
    // Binary search inside the candidate row.
    int left = 0;
    int right = cols - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (matrix[start][mid] == target) return true;
        if (matrix[start][mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return false;
}
#include <cassert>
#include <vector>

// Include the solution function here (or copy it above).
int main() {
    std::vector<std::vector<int>> m1 = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
    assert(searchInSortedMatrix(m1, 3) == true);
    assert(searchInSortedMatrix(m1, 16) == true);
    assert(searchInSortedMatrix(m1, 60) == true);
    assert(searchInSortedMatrix(m1, 0) == false);
    assert(searchInSortedMatrix(m1, 61) == false);
    assert(searchInSortedMatrix(m1, 15) == false);
    
    std::vector<std::vector<int>> m2 = {{1}};
    assert(searchInSortedMatrix(m2, 1) == true);
    assert(searchInSortedMatrix(m2, 2) == false);
    
    std::vector<std::vector<int>> m3 = {{5, 6, 7}, {8, 9, 10}};
    assert(searchInSortedMatrix(m3, 9) == true);
    assert(searchInSortedMatrix(m3, 7) == true);
    assert(searchInSortedMatrix(m3, 4) == false);
    assert(searchInSortedMatrix(m3, 11) == false);
}

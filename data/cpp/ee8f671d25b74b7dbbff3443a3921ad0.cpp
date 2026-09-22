Write a C++ function `std::vector<int> finiteDifferences(const std::vector<int>& sequence)` that, given a non-empty sequence of integers, computes the first element of each successive finite-difference row. Specifically, starting with the input sequence as row 0, each subsequent row is formed by taking the difference between adjacent elements of the previous row (row i element j = row i-1[j+1] - row i-1[j]), and the row length decreases by one each time. The function should return a vector containing the first element of row 0 (the input's first element), then the first element of row 1, and so on, up to and including the final row which contains a single element (the last difference). For example, given input `{5, 8, 12, 7}`, the rows are: row0: `[5,8,12,7]`, row1: `[3,4,-5]`, row2: `[1,-9]`, row3: `[-10]`, so the returned vector is `{5, 3, 1, -10}`. The input can contain negative numbers and duplicates, and must have at least one element.
#include <cassert>
#include <vector>

int main() {
    // Basic example
    std::vector<int> seq1 = {5, 8, 12, 7};
    std::vector<int> expected1 = {5, 3, 1, -10};
    assert(finiteDifferences(seq1) == expected1);

    // Single element
    std::vector<int> seq2 = {42};
    std::vector<int> expected2 = {42};
    assert(finiteDifferences(seq2) == expected2);

    // Two elements with negative difference
    std::vector<int> seq3 = {10, 3};
    std::vector<int> expected3 = {10, -7};
    assert(finiteDifferences(seq3) == expected3);

    // Duplicates and negative values
    std::vector<int> seq4 = {-1, -1, -1};
    std::vector<int> expected4 = {-1, 0, 0};
    assert(finiteDifferences(seq4) == expected4);

    // Longer sequence with varying signs
    std::vector<int> seq5 = {1, 4, -2, 0, 5};
    // rows: [1,4,-2,0,5] -> [3,-6,2,5] -> [-9,8,3] -> [17,-5] -> [-22]
    std::vector<int> expected5 = {1, 3, -9, 17, -22};
    assert(finiteDifferences(seq5) == expected5);

    return 0;
}
#include <vector>

// Computes the first element of each successive finite-difference row.
// Returns a vector where index i is the first element of the i-th difference row.
std::vector<int> finiteDifferences(const std::vector<int>& sequence) {
    std::vector<int> result;
    if (sequence.empty()) {
        return result;
    }

    result.push_back(sequence[0]);
    std::vector<int> current = sequence;

    while (current.size() > 1) {
        std::vector<int> next(current.size() - 1);
        for (size_t j = 0; j + 1 < current.size(); ++j) {
            next[j] = current[j + 1] - current[j];
        }
        result.push_back(next[0]);
        current = std::move(next);
    }

    return result;
}
// The algorithm constructs the difference table row by row. Start by storing the input vector as the current row, and push its first element into the result. Then repeatedly compute the next row by iterating over adjacent pairs in the current row, calculating `next[j] = current[j+1] - current[j]`, until the current row has exactly one element. For a sequence of length `n`, there are `n-1` successive difference rows (since the last row has one element, and we stop after processing it). The total number of difference computations is the sum of `n-1 + n-2 + ... + 1 = n(n-1)/2`, so the time complexity is O(n²). Space complexity is O(n) for storing the result (plus O(n) temporary storage for the next row, which can be reused). Edge cases: if the input has only one element, the function returns a vector with just that element (no differences); if the input has two elements, it returns `{first, second-first}`. Negative and duplicate values require no special handling; difference computations naturally handle them.

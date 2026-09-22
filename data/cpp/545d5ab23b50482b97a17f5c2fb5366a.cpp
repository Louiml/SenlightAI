Write a C++ function named `extractBayerPattern` that takes a grayscale image represented as a 2D vector of floats (values in [0,1]) and returns a 2D vector of floats containing only the green channel pixels from a Bayer RGGB pattern (i.e., the pixels at the positions where the green filter is placed, specifically the same row parity as the column parity? Actually the standard RGGB has green at (even row, even col) and (odd row, odd col) if indexing starts at 0, or equivalently where (row+col)%2==1). The function should extract the green pixels in row-major order, preserving their original order, and return them as a flattened vector? The task must be self-contained. I will create a simpler, clearly specified task: **Write a function that takes a square grayscale matrix (2D `std::vector<std::vector<float>>`) of size N x N (where N is even) representing an image with values in [0,1], and returns a new 2D matrix of size (N/2) x (N/2) containing only the pixels where both row and column indices are even (i.e., the "top-left" green pixels in a Bayer pattern if we consider a 2x2 block with green at (0,0) and (1,1), but let's simplify: extract pixels at positions (2i, 2j) for all valid i,j).** The function must handle empty input by returning an empty matrix. It must not modify the input. Provide a reference solution that is efficient (O(N^2) time, O(N^2) space for output). Then provide test code with assert statements.
// The solution iterates over the input matrix with a step of 2 in both dimensions, collecting the values at positions (row, col) where row % 2 == 0 and col % 2 == 0. Since the input is a square N x N matrix with N even, the output will have dimensions (N/2) x (N/2). We initialize an output matrix of that size and fill it by copying the corresponding elements. Edge cases: if the input is empty (i.e., a zero-sized vector), we return an empty vector. If the input has odd dimensions, we still extract based on even indices, which works fine as long as we check bounds; but the task assumes N is even, so we can rely on that. Complexity: time O(N^2) because we visit every element exactly once (each is either copied or skipped), space O(N^2) for the output. No extra auxiliary space beyond the output.
#include <vector>

// Extracts pixels at even row and even column indices from a square matrix.
// Assumes input is square with even dimension N (N >= 0). Returns a (N/2)x(N/2) matrix.
// If input is empty, returns an empty matrix.
std::vector<std::vector<float>> extractBayerGreen(const std::vector<std::vector<float>>& image) {
    if (image.empty() || image[0].empty()) {
        return {};
    }

    const size_t N = image.size();
    const size_t half = N / 2;
    std::vector<std::vector<float>> result(half, std::vector<float>(half));

    for (size_t i = 0; i < half; ++i) {
        for (size_t j = 0; j < half; ++j) {
            result[i][j] = image[2 * i][2 * j];
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Assume solution function is declared above.

int main() {
    // Test 1: 2x2 input, extract (0,0)
    std::vector<std::vector<float>> img2 = {{0.1f, 0.2f}, {0.3f, 0.4f}};
    auto out2 = extractBayerGreen(img2);
    assert(out2.size() == 1);
    assert(out2[0].size() == 1);
    assert(out2[0][0] == 0.1f);

    // Test 2: 4x4 input
    std::vector<std::vector<float>> img4 = {
        {1.0f, 2.0f, 3.0f, 4.0f},
        {5.0f, 6.0f, 7.0f, 8.0f},
        {9.0f, 10.0f, 11.0f, 12.0f},
        {13.0f, 14.0f, 15.0f, 16.0f}
    };
    auto out4 = extractBayerGreen(img4);
    assert(out4.size() == 2);
    assert(out4[0].size() == 2);
    assert(out4[0][0] == 1.0f);
    assert(out4[0][1] == 3.0f);
    assert(out4[1][0] == 9.0f);
    assert(out4[1][1] == 11.0f);

    // Test 3: Empty input
    std::vector<std::vector<float>> empty;
    auto outEmpty = extractBayerGreen(empty);
    assert(outEmpty.empty());

    // Test 4: Input with one row but zero columns (edge case)
    std::vector<std::vector<float>> emptyRow = {{}};
    auto outEmptyRow = extractBayerGreen(emptyRow);
    assert(outEmptyRow.empty());

    // Test 5: 0x0 input (vector containing zero rows)
    std::vector<std::vector<float>> zeroRows;
    auto outZeroRows = extractBayerGreen(zeroRows);
    assert(outZeroRows.empty());

    // Test 6: Values are preserved exactly (floating point)
    std::vector<std::vector<float>> imgFloat = {
        {0.5f, -1.0f, 0.25f, 3.14f},
        {0.0f, 42.0f, -0.5f, 1.0f},
        {2.5f, 0.75f, 8.0f, -0.25f},
        {10.0f, 20.0f, 30.0f, 40.0f}
    };
    auto outFloat = extractBayerGreen(imgFloat);
    assert(outFloat[0][0] == 0.5f);
    assert(outFloat[0][1] == 0.25f);
    assert(outFloat[1][0] == 2.5f);
    assert(outFloat[1][1] == 8.0f);

    return 0;
}

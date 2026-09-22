Given a vector of unsigned 8-bit integers representing grayscale pixel values, write a C++ function `std::vector<int> computeHistogram(const std::vector<unsigned char>& pixels)` that returns a histogram with exactly 256 bins (indices 0–255), where each bin counts how many times that pixel value appears in the input. The input vector may be empty (in which case all 256 bins should be zero) and may contain duplicate values. The function must not allocate more than O(256) temporary space; the returned histogram itself is of size 256. You may assume the input is non-null and valid. This task isolates the core bin-counting logic from the larger OpenCV histogram implementation and avoids dependencies on image dimensions, channels, masks, or multi-dimensional inputs.
The solution is straightforward: allocate a vector of 256 integers initialized to zero. Iterate over the entire input vector, and for each unsigned char value, increment the bin at that index. Since unsigned char ranges from 0 to 255, each value directly maps to a valid bin without any range checking or clamping. The algorithm runs in \(O(n)\) time where \(n\) is the size of the input vector, and uses \(O(256)\) auxiliary space (the histogram itself plus a small constant). Edge cases: an empty input yields all zeros; duplicate values increment the corresponding bin multiple times; no other special handling is needed. Because the input is `unsigned char`, there is no possibility of out-of-range access, and we can apply `const` reference to avoid copying the input.
#include <vector>
#include <cstddef> // for size_t

// Compute a 256-bin histogram for a vector of 8-bit unsigned pixel values.
// The returned vector has exactly 256 entries, each representing the count of
// occurrences of the corresponding pixel value (0–255) in the input.
std::vector<int> computeHistogram(const std::vector<unsigned char>& pixels) {
    // Initialize all 256 bins to zero.
    std::vector<int> histogram(256, 0);

    // Count occurrences; unsigned char values are implicitly convertible to
    // int indices in [0, 255], so no clamping or bounds checks are needed.
    for (unsigned char value : pixels) {
        ++histogram[value];
    }

    return histogram;
}
#include <cassert>
#include <vector>

// The solution function is expected to be declared above this main.
int main() {
    // Test empty input: all bins zero.
    std::vector<unsigned char> empty;
    auto histEmpty = computeHistogram(empty);
    for (int i = 0; i < 256; ++i) {
        assert(histEmpty[i] == 0);
    }

    // Test a simple sequence with duplicates.
    std::vector<unsigned char> pixels1 = {0, 1, 1, 2, 3, 3, 3, 255, 255};
    auto hist1 = computeHistogram(pixels1);
    assert(hist1[0] == 1);
    assert(hist1[1] == 2);
    assert(hist1[2] == 1);
    assert(hist1[3] == 3);
    assert(hist1[255] == 2);
    assert(hist1[4] == 0);
    assert(hist1[254] == 0);

    // Test a single element.
    std::vector<unsigned char> pixels2 = {128};
    auto hist2 = computeHistogram(pixels2);
    assert(hist2[128] == 1);
    assert(hist2[127] == 0);
    assert(hist2[129] == 0);

    // Test all values from 0 to 255 exactly once.
    std::vector<unsigned char> pixels3(256);
    for (int i = 0; i < 256; ++i) {
        pixels3[i] = static_cast<unsigned char>(i);
    }
    auto hist3 = computeHistogram(pixels3);
    for (int i = 0; i < 256; ++i) {
        assert(hist3[i] == 1);
    }

    // Test continuous zeros.
    std::vector<unsigned char> pixels4(1000, 0);
    auto hist4 = computeHistogram(pixels4);
    assert(hist4[0] == 1000);
    assert(hist4[1] == 0);
    assert(hist4[255] == 0);

    return 0;
}

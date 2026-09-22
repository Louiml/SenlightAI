// Write a standalone C++ function named `medianFilterObservation` that takes a 2D vector of doubles (where each row represents an observation/signal and each column represents a sample index), and returns a 1D vector of doubles containing the median of each row. The function must compute the median correctly for both even and odd numbers of samples: if a row has an odd number of samples (e.g., 5), the median is the middle element after sorting; if even (e.g., 4), the median is the average of the two middle elements after sorting. The function must handle empty rows gracefully by returning `0.0` for that row, and must not modify the input vector (pass by const reference). Also handle the edge case where the input vector itself is empty (return an empty vector).
The solution iterates over each row of the input 2D vector. For each non-empty row, copy the row into a local `std::vector<double>` and sort it using `std::sort`. If the row size is odd, the median is the element at index `(n-1)/2`; if even, it is the average of elements at indices `n/2 - 1` and `n/2`. To avoid integer overflow or sign issues with `size_t` indices, careful indexing is required: use `size_t` and handle `n == 0` separately. The time complexity is `O(m * n log n)` where `m` is the number of rows and `n` is the average row length (dominant cost is sorting each row). Space complexity is `O(n)` per row for the copy, plus `O(m)` for the output vector. Edge cases: empty overall vector → returns empty; empty row → returns `0.0` (as specified). The function should be `const`-correct and use `const` reference for input.
#include <vector>
#include <algorithm>
#include <cstddef>

// Compute the median of each row in a 2D vector of doubles.
// For each row, returns the median. Empty rows produce 0.0.
// The input vector is not modified.
std::vector<double> medianFilterObservation(const std::vector<std::vector<double>>& input) {
    std::vector<double> result;
    result.reserve(input.size());

    for (const auto& row : input) {
        if (row.empty()) {
            result.push_back(0.0);
            continue;
        }

        // Copy and sort to find median without modifying original.
        std::vector<double> sorted = row;
        std::sort(sorted.begin(), sorted.end());

        size_t n = sorted.size();
        double median;
        if (n % 2 == 1) {
            median = sorted[n / 2];
        } else {
            median = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
        }
        result.push_back(median);
    }

    return result;
}
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test with odd-length rows
    std::vector<std::vector<double>> input1 = {{1.0, 3.0, 2.0}, {5.0, 4.0, 6.0}};
    std::vector<double> out1 = medianFilterObservation(input1);
    assert(out1.size() == 2);
    assert(std::fabs(out1[0] - 2.0) < 1e-9);
    assert(std::fabs(out1[1] - 5.0) < 1e-9);

    // Test with even-length rows
    std::vector<std::vector<double>> input2 = {{1.0, 2.0, 3.0, 4.0}, {10.0, 20.0}};
    std::vector<double> out2 = medianFilterObservation(input2);
    assert(out2.size() == 2);
    assert(std::fabs(out2[0] - 2.5) < 1e-9);
    assert(std::fabs(out2[1] - 15.0) < 1e-9);

    // Test with empty row
    std::vector<std::vector<double>> input3 = {{}, {7.0, -3.0}};
    std::vector<double> out3 = medianFilterObservation(input3);
    assert(out3.size() == 2);
    assert(std::fabs(out3[0] - 0.0) < 1e-9);
    assert(std::fabs(out3[1] - 2.0) < 1e-9);  // sorted: -3,7 median=2

    // Test with single-element rows
    std::vector<std::vector<double>> input4 = {{42.0}, {-1.0}};
    std::vector<double> out4 = medianFilterObservation(input4);
    assert(std::fabs(out4[0] - 42.0) < 1e-9);
    assert(std::fabs(out4[1] - (-1.0)) < 1e-9);

    // Test with duplicate values
    std::vector<std::vector<double>> input5 = {{5.0, 5.0, 5.0}, {1.0, 1.0, 2.0}};
    std::vector<double> out5 = medianFilterObservation(input5);
    assert(std::fabs(out5[0] - 5.0) < 1e-9);
    assert(std::fabs(out5[1] - 1.0) < 1e-9);  // sorted: 1,1,2 median=1

    // Test with empty overall input
    std::vector<std::vector<double>> input6 = {};
    std::vector<double> out6 = medianFilterObservation(input6);
    assert(out6.empty());

    // Test with negative and large values
    std::vector<std::vector<double>> input7 = {{-10.0, -20.0, -30.0}, {1000.0, 2000.0, 3000.0, 4000.0}};
    std::vector<double> out7 = medianFilterObservation(input7);
    assert(std::fabs(out7[0] - (-20.0)) < 1e-9);
    assert(std::fabs(out7[1] - 2500.0) < 1e-9);

    // Test that input is not modified
    std::vector<std::vector<double>> input8 = {{3.0, 1.0, 2.0}};
    std::vector<double> original = input8[0];
    medianFilterObservation(input8);
    assert(input8[0] == original);

    return 0;
}

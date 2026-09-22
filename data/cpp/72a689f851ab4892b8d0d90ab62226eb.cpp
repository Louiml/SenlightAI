// Write a C++ function `std::vector<double> signedNumberRatios(const std::vector<int>& numbers)` that takes a non-empty vector of integers and returns a vector of three double values representing, in order: the ratio of positive numbers, the ratio of negative numbers, and the ratio of zeros, each as a fraction of the total count. The ratios must be computed using floating‑point division (e.g., `(double)count / total`) and returned in that exact order. The input vector may contain any mix of positive, negative, and zero integers, including duplicates and extreme values, and there is guaranteed to be at least one element.

#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // Mixed numbers
    std::vector<double> r1 = signedNumberRatios({1, -2, 0, 3, -4});
    assert(r1.size() == 3);
    assert(r1[0] == 2.0 / 5.0); // positives: 1, 3
    assert(r1[1] == 2.0 / 5.0); // negatives: -2, -4
    assert(r1[2] == 1.0 / 5.0); // zero: 0

    // All positives
    std::vector<double> r2 = signedNumberRatios({5, 7, 9});
    assert(r2[0] == 1.0);
    assert(r2[1] == 0.0);
    assert(r2[2] == 0.0);

    // All zeros
    std::vector<double> r3 = signedNumberRatios({0, 0, 0, 0});
    assert(r3[0] == 0.0);
    assert(r3[1] == 0.0);
    assert(r3[2] == 1.0);

    // Single negative
    std::vector<double> r4 = signedNumberRatios({-42});
    assert(r4[0] == 0.0);
    assert(r4[1] == 1.0);
    assert(r4[2] == 0.0);

    // Large mix with duplicates
    std::vector<double> r5 = signedNumberRatios({10, -1, 0, 0, 10, -1, 0});
    assert(r5[0] == 2.0 / 7.0);
    assert(r5[1] == 2.0 / 7.0);
    assert(r5[2] == 3.0 / 7.0);

    // Extreme values
    std::vector<double> r6 = signedNumberRatios({2147483647, -2147483647, -1, 0, 1});
    assert(r6[0] == 2.0 / 5.0);
    assert(r6[1] == 2.0 / 5.0);
    assert(r6[2] == 1.0 / 5.0);

    return 0;
}

#include <vector>

// Returns ratios of positive, negative, and zero elements in the input vector.
// The returned vector has exactly three doubles: [positive_ratio, negative_ratio, zero_ratio].
std::vector<double> signedNumberRatios(const std::vector<int>& numbers) {
    int positiveCount = 0;
    int negativeCount = 0;
    int zeroCount = 0;

    for (const int value : numbers) {
        if (value > 0) {
            ++positiveCount;
        } else if (value < 0) {
            ++negativeCount;
        } else {
            ++zeroCount;
        }
    }

    const double total = static_cast<double>(numbers.size());
    return {positiveCount / total, negativeCount / total, zeroCount / total};
}

// The solution requires a single pass over the input vector to count how many elements are positive, negative, and zero. Initialize three counters to zero, then iterate through all elements, incrementing the appropriate counter based on the sign of each integer (zero is a separate category). After the loop, compute each ratio by dividing the corresponding counter by the total number of elements using a cast to `double` to force floating‑point division. The result is a vector of three doubles in the order positive, negative, zero. Edge cases include all elements being the same sign (e.g., all zeros → the positive and negative ratios are 0.0, zero ratio is 1.0) and a single element (one ratio is 1.0, the others 0.0). The algorithm runs in O(n) time and uses O(1) extra space (excluding the output vector), where n is the number of integers.

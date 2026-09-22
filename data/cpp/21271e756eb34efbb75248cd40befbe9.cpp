/*
Write a C++ function named `averageOfInnerVectors` that takes a `std::vector<std::vector<double>>` as input and returns a `std::vector<double>` where each element is the arithmetic mean of the corresponding inner vector. The input may contain inner vectors of varying sizes, and you may assume the outer vector is non-empty and each inner vector has at least one element. Handle edge cases such as empty inner vectors by treating their mean as 0.0, but if your solution assumes non-empty inner vectors, document that in the analysis. The function should be `const` correct and not modify the input.
*/

#include <vector>

// Computes the mean of each inner vector in a 2D vector.
// Returns a vector of means, with 0.0 for any empty inner vector.
std::vector<double> averageOfInnerVectors(const std::vector<std::vector<double>>& input) {
    std::vector<double> result;
    result.reserve(input.size());

    for (const auto& inner : input) {
        if (inner.empty()) {
            result.push_back(0.0);
            continue;
        }

        double sum = 0.0;
        for (double value : inner) {
            sum += value;
        }
        result.push_back(sum / static_cast<double>(inner.size()));
    }

    return result;
}

#include <cassert>
#include <vector>

// Include the solution function here or link it.

int main() {
    // Basic case: multiple inner vectors of equal size
    std::vector<std::vector<double>> v1 = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    std::vector<double> r1 = averageOfInnerVectors(v1);
    assert(r1.size() == 2);
    assert(r1[0] == 2.0);
    assert(r1[1] == 5.0);

    // Single inner vector with one element
    std::vector<std::vector<double>> v2 = {{7.5}};
    std::vector<double> r2 = averageOfInnerVectors(v2);
    assert(r2.size() == 1);
    assert(r2[0] == 7.5);

    // Inner vectors of varying lengths
    std::vector<std::vector<double>> v3 = {{1.0, 2.0}, {10.0}, {0.0, 0.0, 0.0}};
    std::vector<double> r3 = averageOfInnerVectors(v3);
    assert(r3.size() == 3);
    assert(r3[0] == 1.5);
    assert(r3[1] == 10.0);
    assert(r3[2] == 0.0);

    // Negative and zero values
    std::vector<std::vector<double>> v4 = {{-2.0, 4.0}, {0.0, 0.0}};
    std::vector<double> r4 = averageOfInnerVectors(v4);
    assert(r4[0] == 1.0);
    assert(r4[1] == 0.0);

    // Empty inner vectors (treated as 0.0)
    std::vector<std::vector<double>> v5 = {{}, {1.0, 1.0}};
    std::vector<double> r5 = averageOfInnerVectors(v5);
    assert(r5.size() == 2);
    assert(r5[0] == 0.0);
    assert(r5[1] == 1.0);

    // Large single inner vector
    std::vector<std::vector<double>> v6 = {{1.0, 2.0, 3.0, 4.0}};
    std::vector<double> r6 = averageOfInnerVectors(v6);
    assert(r6.size() == 1);
    assert(r6[0] == 2.5);

    // Multiple empty inner vectors
    std::vector<std::vector<double>> v7 = {{}, {}, {5.0}};
    std::vector<double> r7 = averageOfInnerVectors(v7);
    assert(r7.size() == 3);
    assert(r7[0] == 0.0);
    assert(r7[1] == 0.0);
    assert(r7[2] == 5.0);

    return 0;
}

// The solution iterates over each inner vector, computes the sum of its elements, and divides by its size to obtain the mean. If an inner vector could be empty, we must avoid division by zero; in that case, we set the mean to 0.0. The main algorithm is straightforward: for the outer vector of size \(n\) and the largest inner vector size \(m\), the time complexity is \(O(n \cdot m)\) because we touch every element exactly once. The space complexity is \(O(1)\) auxiliary space aside from the returned vector of size \(n\). Key edge cases include: (1) a single inner vector with one element, where the mean equals that element; (2) inner vectors of different lengths; (3) negative and zero values; and (4) empty inner vectors if we choose to handle them gracefully. The function does not modify the input, so it should take the parameter by const reference.

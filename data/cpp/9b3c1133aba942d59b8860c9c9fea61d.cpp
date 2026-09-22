// Write a C++ function that takes two `std::vector<double>` objects representing predicted values and prediction variances, and returns a `std::pair<std::vector<double>, std::vector<double>>` containing the same data but with any negative variance values replaced by zero and with the vectors resized to have equal length (the shorter one padded with zeros at the end). The function must handle empty input vectors, preserve the original order, and use `const` correctly for inputs.

// The task is straightforward: iterate through both input vectors, copy their elements into output vectors, then apply two transformations. First, for each element in the variance vector, if it is negative, replace it with `0.0`. Second, if the two input vectors have different lengths, pad the shorter output vector with zeros until both have the same length (the maximum of the two original sizes). Edge cases: if both are empty, return two empty vectors; if one is empty and the other is non-empty, pad the empty one to match the non-empty one. The main algorithm is a simple copy-and-patch: time complexity O(n+m) where n and m are input vector sizes, space complexity O(max(n,m)) for the output pair.

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns a pair of vectors: first is the predicted values unchanged,
// second is the variances with negatives set to zero. Both vectors are
// resized to the maximum length, padding with zeros if needed.
std::pair<std::vector<double>, std::vector<double>> 
processPrediction(const std::vector<double>& predictions,
                  const std::vector<double>& variances) {
    // Copy inputs to outputs
    std::vector<double> outPred = predictions;
    std::vector<double> outVar = variances;

    // Replace negative variances with zero
    for (double& v : outVar) {
        if (v < 0.0) {
            v = 0.0;
        }
    }

    // Determine max size
    const std::size_t maxSize = std::max(outPred.size(), outVar.size());
    
    // Pad with zeros to equal lengths
    outPred.resize(maxSize, 0.0);
    outVar.resize(maxSize, 0.0);

    return {outPred, outVar};
}

#include <cassert>
#include <vector>

// Declare solution function (included above for completeness)
std::pair<std::vector<double>, std::vector<double>> 
processPrediction(const std::vector<double>& predictions,
                  const std::vector<double>& variances);

int main() {
    // Basic case: no negatives, equal lengths
    {
        auto result = processPrediction({1.0, 2.0, 3.0}, {0.5, 1.5, 2.5});
        assert(result.first == std::vector<double>({1.0, 2.0, 3.0}));
        assert(result.second == std::vector<double>({0.5, 1.5, 2.5}));
    }

    // Negative variances clamp to zero
    {
        auto result = processPrediction({-1.0, 0.0}, {-0.5, -2.0});
        assert(result.first == std::vector<double>({-1.0, 0.0}));
        assert(result.second == std::vector<double>({0.0, 0.0}));
    }

    // Unequal lengths: predictions shorter
    {
        auto result = processPrediction({5.0}, {0.1, 0.2, 0.3});
        assert(result.first == std::vector<double>({5.0, 0.0, 0.0}));
        assert(result.second == std::vector<double>({0.1, 0.2, 0.3}));
    }

    // Unequal lengths: variances shorter
    {
        auto result = processPrediction({1.0, 2.0, 3.0, 4.0}, {0.0});
        assert(result.first == std::vector<double>({1.0, 2.0, 3.0, 4.0}));
        assert(result.second == std::vector<double>({0.0, 0.0, 0.0, 0.0}));
    }

    // Both empty
    {
        auto result = processPrediction({}, {});
        assert(result.first.empty());
        assert(result.second.empty());
    }

    // One empty, one non-empty
    {
        auto result = processPrediction({}, {1.5, -2.5});
        assert(result.first == std::vector<double>({0.0, 0.0}));
        assert(result.second == std::vector<double>({1.5, 0.0}));
    }

    return 0;
}

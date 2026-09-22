Given a `std::vector<std::pair<std::string, float>>` representing low-level image features (each pair contains a feature name and its numeric value), write a standalone C++ function `double predictTotalQuality(const std::vector<std::pair<std::string, float>>& features, const std::vector<double>& weights, double bias)` that computes a weighted sum of the feature values plus a bias term. The `weights` vector has the same length as the features vector, and the weights are aligned by index (not by feature name). The function must handle an empty features vector by returning the bias alone. Do not use any external machine learning library; implement the calculation manually. The function should be `const`-correct and use only standard headers.
The approach is straightforward: iterate over the features vector, retrieve each feature's numeric value (the second element of the pair), multiply it by the corresponding weight from the weights vector at the same index, and accumulate the sum. Finally, add the bias term and return the result. Edge cases: if the features vector is empty, the loop does not run and the function simply returns the bias. Also, if the weights vector is shorter than the features vector, indexing out of range occurs; to be safe, we can either assert that sizes match or use the smaller size. For robustness in this task, we assume the caller guarantees equal sizes, but we can guard by iterating up to `std::min(features.size(), weights.size())`. Time complexity is O(n) where n is the number of features; space complexity is O(1) aside from the input vectors themselves.
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

// Computes predicted total quality as weighted sum of features plus bias.
// Features and weights must be aligned by index. If lengths differ, only the
// first min(features.size(), weights.size()) pairs are considered.
double predictTotalQuality(const std::vector<std::pair<std::string, float>>& features,
                           const std::vector<double>& weights,
                           double bias) {
    double sum = 0.0;
    const size_t n = std::min(features.size(), weights.size());
    for (size_t i = 0; i < n; ++i) {
        sum += static_cast<double>(features[i].second) * weights[i];
    }
    return sum + bias;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Declaration of the solution function (must match exactly).
double predictTotalQuality(const std::vector<std::pair<std::string, float>>& features,
                           const std::vector<double>& weights,
                           double bias);

int main() {
    // Basic case
    std::vector<std::pair<std::string, float>> feat1 = {{"brightness", 0.5f}, {"contrast", 1.0f}};
    std::vector<double> w1 = {2.0, 3.0};
    assert(predictTotalQuality(feat1, w1, 1.0) == 0.5*2.0 + 1.0*3.0 + 1.0); // 5.0

    // Empty features -> just bias
    std::vector<std::pair<std::string, float>> feat_empty;
    std::vector<double> w_empty;
    assert(predictTotalQuality(feat_empty, w_empty, -2.5) == -2.5);

    // Negative weights and bias
    std::vector<std::pair<std::string, float>> feat2 = {{"noise", 0.1f}};
    std::vector<double> w2 = {-10.0};
    assert(predictTotalQuality(feat2, w2, 5.0) == 0.1 * (-10.0) + 5.0); // 4.0

    // Weights longer than features (should ignore extra weights)
    std::vector<std::pair<std::string, float>> feat3 = {{"a", 2.0f}};
    std::vector<double> w3 = {1.0, 99.0};
    assert(predictTotalQuality(feat3, w3, 0.0) == 2.0 * 1.0); // 2.0

    // Features longer than weights (should ignore extra features)
    std::vector<std::pair<std::string, float>> feat4 = {{"x", 3.0f}, {"y", 4.0f}};
    std::vector<double> w4 = {0.5};
    assert(predictTotalQuality(feat4, w4, 10.0) == 3.0 * 0.5 + 10.0); // 11.5

    // Exact match with floating point (use tolerance if needed)
    double result = predictTotalQuality({{"v", 1.5f}}, {0.2}, 0.0);
    assert(result > 0.299 && result < 0.301); // approximately 0.3

    return 0;
}

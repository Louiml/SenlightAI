// Write a standalone C++ function that implements a brute-force k-nearest neighbors (k-NN) classifier for multi-class classification. The function should accept a matrix of training samples (each row is a data point), a column vector of integer class labels (one per training sample), a matrix of query samples (each row is a point to classify), and an integer k (number of neighbors). For each query point, the function must find the k nearest training points by Euclidean distance, determine the class label that appears most frequently among those k neighbors (breaking ties by choosing the smallest class label), and return a column vector of predicted class labels as integers. For k = 1, the predicted label is simply the label of the nearest neighbor. The function must handle cases where k is larger than the number of training samples (then use all training samples) and where a query point exactly matches a training point (distance zero, which should be included first). Inputs must be real-valued matrices (floating point). The function signature should be: `std::vector<int> knnClassifier(const std::vector<std::vector<float>>& trainSamples, const std::vector<int>& trainLabels, const std::vector<std::vector<float>>& querySamples, int k)`. It should assume all rows have the same number of columns, that `trainSamples` and `trainLabels` have the same length, that `trainSamples` is non-empty, and that `k >= 1`. The output vector should have exactly as many elements as `querySamples`. If `querySamples` is empty, return an empty vector.
The core algorithm is a straightforward brute-force k-NN: for each query point, compute the Euclidean distance to every training point, store pairs of (distance, label), sort them by distance (and by label as a tie-breaker for deterministic ordering), take the first k entries, then count label frequencies among those k neighbors. If k exceeds the training set size, cap k at the number of training samples. For classification, the predicted label is the one with the highest count; if multiple labels share the same maximum count, choose the smallest label value. Edge cases include: identical distances (handled by sorting by distance then label), exact matches (distance zero naturally sorts first), and the case where k = 1 (the nearest label). The time complexity is O(Q * N * D + Q * N log N) where Q is the number of query points, N is the number of training samples, and D is the number of features, because for each query we compute N distances (each O(D)) and then sort N pairs (O(N log N)). Space complexity is O(N) per query for storing pairs (reused), plus O(Q) for the output vector.
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

// Brute-force k-nearest neighbors classifier for multi-class classification.
// Returns predicted class labels for each query sample.
std::vector<int> knnClassifier(
    const std::vector<std::vector<float>>& trainSamples,
    const std::vector<int>& trainLabels,
    const std::vector<std::vector<float>>& querySamples,
    int k
) {
    // Handle empty query set.
    if (querySamples.empty()) return {};

    const size_t numTrain = trainSamples.size();
    const int effectiveK = static_cast<int>(std::min<size_t>(k, numTrain));

    std::vector<int> predictions;
    predictions.reserve(querySamples.size());

    for (const auto& query : querySamples) {
        // Compute distances to all training samples.
        std::vector<std::pair<float, int>> neighbors; // (distance, label)
        neighbors.reserve(numTrain);

        for (size_t i = 0; i < numTrain; ++i) {
            const auto& train = trainSamples[i];
            float distSq = 0.0f;
            for (size_t d = 0; d < train.size(); ++d) {
                float diff = query[d] - train[d];
                distSq += diff * diff;
            }
            neighbors.emplace_back(distSq, trainLabels[i]);
        }

        // Sort by distance, then by label for deterministic ordering.
        std::sort(neighbors.begin(), neighbors.end(),
                  [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
                      if (a.first != b.first) return a.first < b.first;
                      return a.second < b.second;
                  });

        // Count labels among the k nearest.
        std::map<int, int> labelCounts;
        for (int j = 0; j < effectiveK; ++j) {
            labelCounts[neighbors[j].second]++;
        }

        // Find the label with the highest count; ties break by smallest label.
        int bestLabel = -1;
        int bestCount = -1;
        for (const auto& [label, count] : labelCounts) {
            if (count > bestCount) {
                bestCount = count;
                bestLabel = label;
            }
        }

        predictions.push_back(bestLabel);
    }

    return predictions;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (already defined above).
std::vector<int> knnClassifier(
    const std::vector<std::vector<float>>& trainSamples,
    const std::vector<int>& trainLabels,
    const std::vector<std::vector<float>>& querySamples,
    int k
);

int main() {
    // Test 1: Simple two-class problem, k=1
    {
        std::vector<std::vector<float>> train = {{0.0f, 0.0f}, {2.0f, 2.0f}};
        std::vector<int> labels = {0, 1};
        std::vector<std::vector<float>> query = {{0.1f, 0.1f}, {1.9f, 1.9f}, {1.0f, 1.0f}};
        std::vector<int> result = knnClassifier(train, labels, query, 1);
        assert(result == std::vector<int>({0, 1, 0}));
    }

    // Test 2: k > number of training samples, use all samples
    {
        std::vector<std::vector<float>> train = {{1.0f, 1.0f}, {2.0f, 2.0f}};
        std::vector<int> labels = {5, 7};
        std::vector<std::vector<float>> query = {{1.5f, 1.5f}, {100.0f, 100.0f}};
        std::vector<int> result = knnClassifier(train, labels, query, 5);
        // With k=2 (all samples), ties must break to smallest label (5).
        assert(result == std::vector<int>({5, 5}));
    }

    // Test 3: Tie-breaking among neighbors, multiple classes
    {
        std::vector<std::vector<float>> train = {{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {2.0f, 2.0f}};
        std::vector<int> labels = {3, 3, 1, 1};
        std::vector<std::vector<float>> query = {{0.5f, 0.5f}};
        // Distances: to (0,0)=0.707, to (1,0)=0.707, to (0,1)=0.707, to (2,2)=2.121
        // k=3: labels {3,3,1} -> 3 wins
        std::vector<int> result = knnClassifier(train, labels, query, 3);
        assert(result == std::vector<int>({3}));
    }

    // Test 4: Exact match with training point (distance zero)
    {
        std::vector<std::vector<float>> train = {{1.0f, 2.0f, 3.0f}};
        std::vector<int> labels = {42};
        std::vector<std::vector<float>> query = {{1.0f, 2.0f, 3.0f}};
        std::vector<int> result = knnClassifier(train, labels, query, 1);
        assert(result == std::vector<int>({42}));
    }

    // Test 5: Empty query set
    {
        std::vector<std::vector<float>> train = {{1.0f}};
        std::vector<int> labels = {0};
        std::vector<std::vector<float>> query = {};
        std::vector<int> result = knnClassifier(train, labels, query, 1);
        assert(result.empty());
    }

    // Test 6: k=1 with tie-breaking between two identical distances (different labels)
    {
        std::vector<std::vector<float>> train = {{0.0f}, {1.0f}};
        std::vector<int> labels = {10, 20};
        std::vector<std::vector<float>> query = {{0.5f}};
        // Both distances equal 0.5, sort by label picks label 10.
        std::vector<int> result = knnClassifier(train, labels, query, 1);
        assert(result == std::vector<int>({10}));
    }

    return 0;
}

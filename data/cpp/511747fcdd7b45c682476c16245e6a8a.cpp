// Write a standalone C++ function named `knnPredict` that implements a K-nearest neighbors classifier for a single test point. The function takes a training dataset (matrix of features and vector of labels), a single test feature vector, the number of training observations, the number of features, and a maximum neighbor count `k`. It must compute predictions for all values from 1 to `k` (inclusive), storing each prediction in an output array. For each `k`, the prediction is the most frequent label among the `k` nearest training points, with ties broken by choosing the label that appears first among the tied labels when scanning nearest to farthest. The function should return an error code: `0` for success, `1` if no training data, `2` if `k` is less than 1, `3` if `k` exceeds the number of training points.

// The core algorithm involves computing Euclidean distances from the test point to every training point, then sorting the training indices by distance (and for equal distances, by original index order to ensure deterministic tie-breaking). After sorting, for each desired neighbor count `m` from 1 to `k`, we tally label frequencies among the first `m` neighbors and select the label with the highest count. Since ties are broken by earliest appearance in the sorted order, we can iterate through the sorted neighbors and increment counts; the label with the maximum count is chosen, and because we iterate in order, when a tie occurs, the first label to reach the current maximum remains the winner (we update only if a strictly greater count is found). We precompute predictions incrementally: for each step from 1 to `k`, we add one neighbor to a frequency map, then scan the map to find the label with the highest frequency, breaking ties by keeping the label that was seen earliest in the sorted neighbor list. Time complexity is O(n log n + n * k) due to sorting and scanning, but since k ≤ n, this is O(n log n + n^2) in worst case; for typical small k it is O(n log n). Space complexity is O(n) for storing distances, indices, and frequency map.

#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>

// K-NN prediction for a single test point, for all neighbor counts 1..k.
// Returns 0 on success, 1 if n_train <= 0, 2 if k < 1, 3 if k > n_train.
// train_input is a flat array of size n_train * n_features (row-major).
// train_labels has size n_train; test_input has size n_features.
// test_predict output array has size k (one prediction per neighbor count).
int knnPredict(const double* train_input, const double* train_label,
               const double* test_input, int n_train, int n_features, int k,
               double* test_predict) {
    if (n_train <= 0) return 1;
    if (k < 1) return 2;
    if (k > n_train) return 3;

    // Compute distances and store original indices
    std::vector<double> dists(n_train);
    std::vector<int> idx(n_train);
    for (int i = 0; i < n_train; ++i) {
        double sum = 0.0;
        for (int j = 0; j < n_features; ++j) {
            double diff = train_input[i * n_features + j] - test_input[j];
            sum += diff * diff;
        }
        dists[i] = std::sqrt(sum);
        idx[i] = i;
    }

    // Sort indices by distance, then by original index (stable tie-break)
    std::vector<int> sorted_idx(idx);
    std::sort(sorted_idx.begin(), sorted_idx.end(),
        [&](int a, int b) {
            if (dists[a] != dists[b]) return dists[a] < dists[b];
            return a < b;
        });

    // Count labels incrementally
    std::unordered_map<double, int> label_count;
    int current_max_count = 0;
    double current_best_label = 0.0;

    for (int m = 1; m <= k; ++m) {
        int neighbor = sorted_idx[m - 1];
        double label = train_label[neighbor];
        label_count[label]++;
        if (label_count[label] > current_max_count) {
            current_max_count = label_count[label];
            current_best_label = label;
        }
        // Note: tie-breaking is implicit: we only update when strictly greater,
        // so the first label to reach the max remains the best.
        test_predict[m - 1] = current_best_label;
    }
    return 0;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is declared above; assume it is included.

int main() {
    // Test 1: Simple binary classification, k=1..3
    {
        std::vector<double> train_input = {
            0, 0,
            1, 1,
            2, 2,
            10, 10
        }; // 4 training points, 2 features
        std::vector<double> train_label = {0.0, 0.0, 1.0, 1.0};
        std::vector<double> test_input = {1.0, 1.1}; // near points with labels 0 and 0
        int k = 3;
        std::vector<double> predict(k);
        int status = knnPredict(train_input.data(), train_label.data(),
                                test_input.data(), 4, 2, k, predict.data());
        assert(status == 0);
        assert(predict[0] == 0.0); // nearest is (1,1) label 0
        assert(predict[1] == 0.0); // two nearest: (1,1),(0,0) both 0
        assert(predict[2] == 0.0); // three nearest: two 0s, one 1 => 0
    }

    // Test 2: Tie-breaking by order (equal distances)
    {
        std::vector<double> train_input = {
            0, 0,
            0, 0,
            1, 1,
            1, 1
        }; // two identical at (0,0) labels 0,1; two at (1,1) labels 0,1
        std::vector<double> train_label = {1.0, 0.0, 0.0, 1.0};
        std::vector<double> test_input = {0.1, 0.1}; // closer to (0,0)
        int k = 2;
        std::vector<double> predict(k);
        int status = knnPredict(train_input.data(), train_label.data(),
                                test_input.data(), 4, 2, k, predict.data());
        assert(status == 0);
        // Nearest is index0 label 1, second nearest is index1 label 0 -> tie, first seen is 1
        assert(predict[0] == 1.0);
        assert(predict[1] == 1.0); // count 1 each, but first seen (index0) wins
    }

    // Test 3: Error codes
    {
        std::vector<double> train_input = {1.0};
        std::vector<double> train_label = {0.0};
        std::vector<double> test_input = {1.0};
        std::vector<double> predict(1);
        assert(knnPredict(train_input.data(), train_label.data(), test_input.data(), 0, 1, 1, predict.data()) == 1);
        assert(knnPredict(train_input.data(), train_label.data(), test_input.data(), 1, 1, 0, predict.data()) == 2);
        assert(knnPredict(train_input.data(), train_label.data(), test_input.data(), 1, 1, 2, predict.data()) == 3);
    }

    // Test 4: Larger k with multiple labels
    {
        // 5 points: 3 of label A, 2 of label B; test near group A
        std::vector<double> train_input = {0, 1, 2, 10, 11};
        std::vector<double> train_label = {0.0, 0.0, 0.0, 1.0, 1.0};
        std::vector<double> test_input = {2.1};
        int k = 5;
        std::vector<double> predict(k);
        int status = knnPredict(train_input.data(), train_label.data(),
                                test_input.data(), 5, 1, k, predict.data());
        assert(status == 0);
        assert(predict[0] == 0.0);
        assert(predict[1] == 0.0);
        assert(predict[2] == 0.0);
        assert(predict[3] == 0.0); // 4 nearest: 3 zeros, 1 one => zero
        assert(predict[4] == 0.0); // all 5: 3 zeros, 2 ones => zero
    }

    // Test 5: Single training point, k=1
    {
        std::vector<double> train_input = {3.0, 3.0};
        std::vector<double> train_label = {7.5};
        std::vector<double> test_input = {0.0, 0.0};
        int k = 1;
        std::vector<double> predict(k);
        int status = knnPredict(train_input.data(), train_label.data(),
                                test_input.data(), 1, 2, k, predict.data());
        assert(status == 0);
        assert(predict[0] == 7.5);
    }

    return 0;
}

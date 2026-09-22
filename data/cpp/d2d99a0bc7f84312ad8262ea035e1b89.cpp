Write a C++ function that takes as input a vector of doubles representing a dense feature vector and a vector of label values (either +1 or -1), along with a regularization parameter C and an iteration limit. The function must implement a simple binary logistic regression solver using gradient descent with L2 regularization, and return the learned weight vector (excluding any bias term). The objective to minimize is \( \frac{1}{2}\|w\|^2 + C \sum_{i=1}^{n} \log(1+\exp(-y_i (w \cdot x_i))) \). Initialize weights to zero, use a fixed learning rate of 0.1, and stop after the given iteration limit or when the change in objective value between consecutive iterations is below 1e-6, whichever comes first. The input labels are guaranteed to be exactly +1 or -1, and the feature matrix is provided row by row with each row having the same length.

The solution computes the gradient of the regularized logistic loss. The gradient with respect to each weight component \(w_j\) is \( w_j + C \sum_{i} x_{i,j} y_i (\sigma(y_i (w \cdot x_i)) - 1) \), where \(\sigma\) is the sigmoid function. We initialize weights to zero and repeatedly update \(w \leftarrow w - \eta \cdot \text{gradient}\), with \(\eta=0.1\). To check convergence, we compute the loss (objective value) before each update and declare convergence if the absolute difference from the previous loss is less than 1e-6. Edge cases include: when there is only one feature or one sample, the gradient computation still works without modification; labels are strictly ±1 so the sigmoid is well-behaved; the learning rate is fixed and may not guarantee convergence for very large feature magnitudes, but the iteration limit provides a bound. Time complexity is \(O(\text{iterations} \cdot n \cdot m)\) where \(n\) is number of samples and \(m\) is number of features. Space complexity is \(O(m)\) for the weight vector plus \(O(n \cdot m)\) for the input, but the function receives the data already stored, so auxiliary space is \(O(m)\).

#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>

// Computes the sigmoid of x: 1/(1+exp(-x))
double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

// Computes the logistic loss objective: 0.5*||w||^2 + C*sum(log(1+exp(-y_i*(w·x_i))))
double logistic_loss(const std::vector<double>& w,
                     const std::vector<std::vector<double>>& X,
                     const std::vector<double>& y,
                     double C) {
    int n = (int)X.size();
    double loss = 0.0;
    for (double val : w) loss += 0.5 * val * val;
    for (int i = 0; i < n; ++i) {
        double dot = 0.0;
        for (size_t j = 0; j < X[i].size(); ++j) dot += w[j] * X[i][j];
        loss += C * std::log(1.0 + std::exp(-y[i] * dot));
    }
    return loss;
}

// Gradient descent solver for L2-regularized logistic regression.
// X is a row-major vector of feature vectors, y is +1/-1 labels.
// Returns the learned weight vector (no bias term).
std::vector<double> trainLogisticRegression(
    const std::vector<std::vector<double>>& X,
    const std::vector<double>& y,
    double C,
    int maxIterations) {
    int n = (int)X.size();
    int m = (n > 0) ? (int)X[0].size() : 0;
    std::vector<double> w(m, 0.0);
    const double eta = 0.1;
    const double tol = 1e-6;
    double prevLoss = logistic_loss(w, X, y, C);

    for (int iter = 0; iter < maxIterations; ++iter) {
        // Compute gradient
        std::vector<double> grad(m, 0.0);
        for (int i = 0; i < n; ++i) {
            double dot = 0.0;
            for (int j = 0; j < m; ++j) dot += w[j] * X[i][j];
            double factor = C * y[i] * (sigmoid(y[i] * dot) - 1.0);
            for (int j = 0; j < m; ++j) grad[j] += factor * X[i][j];
        }
        for (int j = 0; j < m; ++j) grad[j] += w[j]; // L2 regularization derivative

        // Update weights
        for (int j = 0; j < m; ++j) w[j] -= eta * grad[j];

        // Check convergence
        double newLoss = logistic_loss(w, X, y, C);
        if (std::fabs(newLoss - prevLoss) < tol) break;
        prevLoss = newLoss;
    }
    return w;
}

int main() {
    // Simple synthetic dataset: two features, separable-ish, labels +1/-1
    std::vector<std::vector<double>> X = {
        {1.0, 2.0},
        {2.0, 1.0},
        {3.0, 3.0},
        {-1.0, -2.0},
        {-2.0, -1.0},
        {-3.0, -3.0}
    };
    std::vector<double> y = {1, 1, 1, -1, -1, -1};
    double C = 1.0;
    int maxIter = 1000;

    std::vector<double> w = trainLogisticRegression(X, y, C, maxIter);

    // For this linearly separable dataset, weights should be such that
    // predictions on training data are correct.
    auto predict = [&](const std::vector<double>& x, const std::vector<double>& w) {
        double dot = 0.0;
        for (size_t j = 0; j < x.size(); ++j) dot += w[j] * x[j];
        return dot >= 0 ? 1.0 : -1.0;
    };

    for (size_t i = 0; i < X.size(); ++i) {
        assert(predict(X[i], w) == y[i]);
    }

    // Check weights are finite and not all zero
    for (double val : w) assert(std::isfinite(val));
    bool nonzero = false;
    for (double val : w) if (std::fabs(val) > 1e-9) nonzero = true;
    assert(nonzero);

    // Test convergence: with many iterations, loss should decrease
    double loss1 = logistic_loss(w, X, y, C);
    std::vector<double> w0(X[0].size(), 0.0);
    double loss0 = logistic_loss(w0, X, y, C);
    assert(loss1 <= loss0 + 1e-12);

    // Edge case: single sample, single feature
    std::vector<std::vector<double>> X1 = {{5.0}};
    std::vector<double> y1 = {1.0};
    std::vector<double> w1 = trainLogisticRegression(X1, y1, C, 100);
    assert(w1.size() == 1);
    assert(std::isfinite(w1[0]));

    // Edge case: zero iterations returns zero weights
    std::vector<double> w0iter = trainLogisticRegression(X, y, C, 0);
    for (double val : w0iter) assert(std::fabs(val) < 1e-12);

    // Edge case: all same label
    std::vector<double> y_all_positive(X.size(), 1.0);
    std::vector<double> w_all_pos = trainLogisticRegression(X, y_all_positive, C, 50);
    for (double val : w_all_pos) assert(std::isfinite(val));

    return 0;
}

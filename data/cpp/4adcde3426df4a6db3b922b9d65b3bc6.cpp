Write a standalone C++ function `computePenaltyMap` that implements the size and ratio penalty computation portion of a visual object tracking algorithm. Given a 2D matrix `predictedWidths` of width predictions, a 2D matrix `predictedHeights` of height predictions, a floating-point `targetWidth`, a floating-point `targetHeight`, and a floating-point `penaltyK`, the function must return a 2D matrix of penalty multipliers of the same dimensions. For each element at position `(i,j)`, compute `scale = sizeCal(predictedWidths(i,j), predictedHeights(i,j)) / sizeCal(targetWidth, targetHeight)`, where `sizeCal(w,h) = sqrt((w + (w+h)*0.5) * (h + (w+h)*0.5))`. Then transform `scale` to `s = max(scale, 1/scale)`. Compute `ratio = (targetWidth/targetHeight) / (predictedWidths(i,j) / predictedHeights(i,j))`, then transform similarly to `r = max(ratio, 1/ratio)`. Finally compute `penalty = exp(-penaltyK * (r * s - 1))`. Handle division by zero by defining `ratio = 1.0` when `predictedHeights` is zero, and ensure all computations use `float` precision.

The solution operates element-wise over the two input matrices. For each cell, we first compute a scaled size measure using the provided `sizeCal` formula, which combines the raw dimension with a context padding equal to half the sum of width and height. We then compute the scale penalty factor `s` by taking the reciprocal if `scale < 1` to ensure symmetry. Similarly, for the ratio, we divide target aspect ratio by predicted aspect ratio; if the predicted height is zero, we set the ratio to 1 to avoid division by zero (the size penalty will still handle the invalid prediction). After applying the same reciprocal-max transformation, we combine both factors and apply the exponential decay using `penaltyK`. The algorithm processes each cell in constant time, so for an `m x n` matrix, time complexity is O(mn) and space complexity is O(1) auxiliary, since we output a new matrix of the same size. Edge cases include zero dimensions in predictions (which yield infinite size values but are handled by the reciprocal-max transformation), zero predicted heights, and cases where the target dimensions are zero (though typically the target is valid; if not, the result is undefined but we assume valid inputs).

#include <vector>
#include <cmath>
#include <algorithm>

// Compute size calibration: sqrt((w + (w+h)*0.5) * (h + (w+h)*0.5))
static float sizeCal(float w, float h) {
    float pad = (w + h) * 0.5f;
    float sz2 = (w + pad) * (h + pad);
    return std::sqrt(sz2);
}

// Compute penalty map for tracking predictions.
// predictedWidths, predictedHeights: same-size matrices of float values.
// targetWidth, targetHeight: target object dimensions.
// penaltyK: penalty coefficient.
// Returns a matrix (same dimensions) of penalty multipliers.
std::vector<std::vector<float>> computePenaltyMap(
        const std::vector<std::vector<float>>& predictedWidths,
        const std::vector<std::vector<float>>& predictedHeights,
        float targetWidth,
        float targetHeight,
        float penaltyK) {
    // Assume non-empty and rectangular input.
    int rows = static_cast<int>(predictedWidths.size());
    int cols = static_cast<int>(predictedWidths[0].size());

    // Output matrix initialized to zeros.
    std::vector<std::vector<float>> penalty(rows, std::vector<float>(cols, 0.0f));

    float targetAspect = targetWidth / targetHeight;
    float targetSize = sizeCal(targetWidth, targetHeight);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float predW = predictedWidths[i][j];
            float predH = predictedHeights[i][j];

            // Scale penalty
            float predSize = sizeCal(predW, predH);
            float scale = predSize / targetSize;
            float s = std::max(scale, 1.0f / scale);

            // Ratio penalty
            float ratio;
            if (predH != 0.0f) {
                ratio = targetAspect / (predW / predH);
            } else {
                ratio = 1.0f;  // Avoid division by zero
            }
            float r = std::max(ratio, 1.0f / ratio);

            // Combined penalty
            penalty[i][j] = std::exp(-penaltyK * (r * s - 1.0f));
        }
    }

    return penalty;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// Include the solution function here (or link appropriately)
// ...

int main() {
    // Helper to compare floating-point matrices with tolerance
    auto approxEqual = [](float a, float b, float eps = 1e-4f) {
        return std::fabs(a - b) < eps;
    };

    // Test 1: Simple case with identity predictions
    std::vector<std::vector<float>> w1 = {{10.0f}};
    std::vector<std::vector<float>> h1 = {{10.0f}};
    auto p1 = computePenaltyMap(w1, h1, 10.0f, 10.0f, 0.055f);
    // sizeCal: pad=10, sz2=(20*20)=400, sqrt=20; scale=20/20=1; s=1
    // ratio: (1)/(1)=1; r=1; penalty=exp(0)=1
    assert(approxEqual(p1[0][0], 1.0f));

    // Test 2: Larger predicted size doubles the scale
    std::vector<std::vector<float>> w2 = {{20.0f}};
    std::vector<std::vector<float>> h2 = {{20.0f}};
    auto p2 = computePenaltyMap(w2, h2, 10.0f, 10.0f, 0.055f);
    // sizeCal for pred: pad=20, sz2=(40*40)=1600, sqrt=40; scale=40/20=2; s=2
    // ratio: targetAspect=1, predAspect=1, ratio=1; r=1
    // penalty = exp(-0.055*(2*1-1)) = exp(-0.055) ≈ 0.9465
    assert(approxEqual(p2[0][0], std::exp(-0.055f)));

    // Test 3: Smaller predicted size gives same penalty as larger (symmetry)
    std::vector<std::vector<float>> w3 = {{5.0f}};
    std::vector<std::vector<float>> h3 = {{5.0f}};
    auto p3 = computePenaltyMap(w3, h3, 10.0f, 10.0f, 0.055f);
    // sizeCal for pred: pad=5, sz2=(10*10)=100, sqrt=10; scale=10/20=0.5; s=max(0.5,2)=2
    // penalty same as Test 2
    assert(approxEqual(p3[0][0], p2[0][0]));

    // Test 4: Aspect ratio mismatch
    std::vector<std::vector<float>> w4 = {{10.0f}};
    std::vector<std::vector<float>> h4 = {{5.0f}};
    auto p4 = computePenaltyMap(w4, h4, 10.0f, 10.0f, 0.055f);
    // targetAspect=1, predAspect=2, ratio=0.5, r=max(0.5,2)=2
    // sizeCal pred: pad=7.5, sz2=(17.5*12.5)=218.75, sqrt≈14.79; scale=14.79/20≈0.7395; s≈1.352
    // penalty ≈ exp(-0.055*(2*1.352-1)) ≈ exp(-0.055*1.704) ≈ exp(-0.0937) ≈ 0.9106
    float expected4 = std::exp(-0.055f * (2.0f * (std::sqrt(218.75f)/20.0f) - 1.0f));
    assert(approxEqual(p4[0][0], expected4));

    // Test 5: Zero height in prediction
    std::vector<std::vector<float>> w5 = {{10.0f}};
    std::vector<std::vector<float>> h5 = {{0.0f}};
    auto p5 = computePenaltyMap(w5, h5, 10.0f, 10.0f, 0.055f);
    // sizeCal pred: pad=5, sz2=(15*5)=75, sqrt≈8.66; scale≈0.433; s=max(0.433,2.309)=2.309
    // ratio defaults to 1, r=1
    // penalty = exp(-0.055*(2.309-1)) = exp(-0.055*1.309) ≈ exp(-0.072) ≈ 0.9305
    assert(approxEqual(p5[0][0], std::exp(-0.055f * 1.309f), 1e-3f));

    // Test 6: Matrix dimensions larger
    std::vector<std::vector<float>> w6 = {{10, 20}, {20, 30}};
    std::vector<std::vector<float>> h6 = {{10, 20}, {20, 30}};
    auto p6 = computePenaltyMap(w6, h6, 10, 10, 0.055f);
    assert(p6.size() == 2);
    assert(p6[0].size() == 2);
    // Top-left matches Test 1
    assert(approxEqual(p6[0][0], 1.0f));
    // Top-right matches Test 2
    assert(approxEqual(p6[0][1], std::exp(-0.055f)));
    // Bottom-left matches Test 2 (same dimensions)
    assert(approxEqual(p6[1][0], std::exp(-0.055f)));
    // Bottom-right: pred sizeCal pad=30, sz2=(60*60)=3600 sqrt=60; scale=60/20=3; s=3; ratio=1; penalty=exp(-0.055*2)=exp(-0.11)
    assert(approxEqual(p6[1][1], std::exp(-0.055f * 2.0f)));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

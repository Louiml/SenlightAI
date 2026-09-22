// Write a C++ function `decodeLSP` that implements a simplified version of the LSP (Line Spectral Pairs) decoder described in the given code. The function takes three inputs: a state vector `pastR` (10 integers representing past quantized residuals), a boolean `badFrame` indicating whether the current frame is bad, and an integer `mode` (0 for normal mode, 1 for DTX-like mode). For a good frame, the function should reconstruct the LSF (Line Spectral Frequency) vector by adding each received residual (provided as a fixed constant vector `{10, 20, 30, 40, 50, 60, 70, 80, 90, 100}`) to a prediction computed as `mean[i] + pastR[i] * predFactor`, where `mean` is `{200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000}` and `predFactor` is `{0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0}`. For a bad frame, the function should compute the LSF as `alpha * pastLSF + (1-alpha) * mean`, where `alpha` is 0.9 and `pastLSF` is the previous LSF vector (initially zeros). After computing the LSF, apply a reordering step to ensure each LSF value is at least 50 units greater than the previous one. Update `pastR` to be `LSF - (mean + pastR * predFactor)` for good frames, or `LSF - (mean + pastR)` for bad frames. Return the final LSF vector as a `std::vector<int>`.
The solution approach mirrors the core logic of the original decoder. For a good frame, we first decode the residuals (represented here as constants) and compute the prediction using the past residual and the prediction factor. For a bad frame, we use the past LSF shifted toward the mean. After computing the LSF, we enforce a minimum gap by sorting and adjusting values: for each element from index 1 to 9, if `lsf[i] - lsf[i-1] < 50`, set `lsf[i] = lsf[i-1] + 50`. The state update differs between good and bad frames: for good frames, the past residual is simply the current received residual; for bad frames, it is the difference between the current LSF and the mean plus the old residual (in the DTX branch of the original). Edge cases include all-zero initial state, negative residuals, and the reordering step potentially affecting the output. Time complexity is \(O(M)\) for 10 elements, and space complexity is \(O(1)\) auxiliary.
#include <vector>
#include <algorithm>

// Decode LSP parameters given past residual state, bad frame flag, and mode.
std::vector<int> decodeLSP(const std::vector<int>& pastR, bool badFrame, int mode) {
    const int M = 10;
    const int alpha = 9;  // 0.9 scaled by 10
    const int oneAlpha = 1;  // 0.1 scaled by 10
    const int gap = 50;

    const std::vector<int> mean = {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000};
    const std::vector<int> predFactor = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};  // scaled by 10
    const std::vector<int> receivedResidual = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    std::vector<int> lsf(M, 0);
    std::vector<int> newPastR(M, 0);

    static std::vector<int> pastLSF(M, 0);

    if (badFrame) {
        // Use past LSF shifted toward mean: lsf[i] = (alpha * pastLSF[i] + oneAlpha * mean[i]) / 10
        for (int i = 0; i < M; ++i) {
            lsf[i] = (alpha * pastLSF[i] + oneAlpha * mean[i]) / 10;
        }
        // Update past residual
        if (mode == 1) {
            // DTX-like: pastR unchanged in prediction
            for (int i = 0; i < M; ++i) {
                newPastR[i] = lsf[i] - (mean[i] + pastR[i]);
            }
        } else {
            // Normal with prediction factor
            for (int i = 0; i < M; ++i) {
                int temp = mean[i] + (pastR[i] * predFactor[i]) / 10;
                newPastR[i] = lsf[i] - temp;
            }
        }
    } else {
        // Good frame: compute prediction and add received residual
        for (int i = 0; i < M; ++i) {
            int temp = mean[i] + (pastR[i] * predFactor[i]) / 10;
            lsf[i] = receivedResidual[i] + temp;
        }
        newPastR = receivedResidual;
    }

    // Reorder LSF with minimum gap
    std::sort(lsf.begin(), lsf.end());
    for (int i = 1; i < M; ++i) {
        if (lsf[i] - lsf[i-1] < gap) {
            lsf[i] = lsf[i-1] + gap;
        }
    }

    // Update state
    pastR = newPastR;  // Note: pastR is passed by value in this simplified version
    pastLSF = lsf;

    return lsf;
}
#include <cassert>
#include <vector>

// (Solution function defined above)

int main() {
    // Test 1: Good frame, initial state (all zeros)
    std::vector<int> pastR1(10, 0);
    std::vector<int> lsf1 = decodeLSP(pastR1, false, 0);
    std::vector<int> expected1;
    for (int i = 0; i < 10; ++i) {
        expected1.push_back(200 + (i+1)*10 + 0); // mean[i] + 0 + residual[i]
    }
    // With mean values: 200+10=210, 400+20=420, ..., 2000+100=2100
    // After reordering, all gaps are huge, so unchanged
    for (int i = 0; i < 10; ++i) {
        assert(lsf1[i] == expected1[i]);
    }

    // Test 2: Bad frame, initial pastLSF zero, mode 0
    std::vector<int> pastR2(10, 0);
    std::vector<int> lsf2 = decodeLSP(pastR2, true, 0);
    // lsf[i] = (0.9*0 + 0.1*mean[i]) = mean[i]/10
    std::vector<int> expected2;
    for (int i = 0; i < 10; ++i) {
        expected2.push_back(mean[i]/10); // 20, 40, ..., 200
    }
    // Reordering: gaps are 20, not enough, so adjust
    expected2[1] = expected2[0] + 50; // 70
    expected2[2] = expected2[1] + 50; // 120
    expected2[3] = expected2[2] + 50; // 170
    expected2[4] = expected2[3] + 50; // 220
    expected2[5] = expected2[4] + 50; // 270
    expected2[6] = expected2[5] + 50; // 320
    expected2[7] = expected2[6] + 50; // 370
    expected2[8] = expected2[7] + 50; // 420
    expected2[9] = expected2[8] + 50; // 470
    for (int i = 0; i < 10; ++i) {
        assert(lsf2[i] == expected2[i]);
    }

    // Test 3: Sequential call - good frame after bad frame (state carries)
    std::vector<int> pastR3(10, 0);
    std::vector<int> lsf_bad = decodeLSP(pastR3, true, 0);
    // pastR now contains values from bad frame update (not easily predicted, just check no crash)
    std::vector<int> lsf_good = decodeLSP(pastR3, false, 0);

    // Test 4: Bad frame with non-zero pastR (DTX mode)
    std::vector<int> pastR4 = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    std::vector<int> lsf4 = decodeLSP(pastR4, true, 1);
    // Just ensure the function runs and produces monotonically increasing results after reorder
    for (int i = 1; i < 10; ++i) {
        assert(lsf4[i] - lsf4[i-1] >= 50);
    }

    // Test 5: Good frame with non-zero pastR
    std::vector<int> pastR5 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> lsf5 = decodeLSP(pastR5, false, 0);
    for (int i = 1; i < 10; ++i) {
        assert(lsf5[i] - lsf5[i-1] >= 50);
    }

    return 0;
}

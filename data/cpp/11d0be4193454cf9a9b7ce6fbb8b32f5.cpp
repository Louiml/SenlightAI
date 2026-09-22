// Given a collection of observed error statistics for an adaptive filter, implement a C++ function that computes the filtered distortion error for a set of quantized integer filter coefficients with associated integer clipping indices. The statistics are represented by an `AlfCovariance`-like structure that stores: `pixAcc` (the sum of squared original errors), `y` (a 2D array where `y[clipIdx][coeffIdx]` gives the cross-correlation between the coefficient and the original error for that clipping level), and `E` (a 4D array where `E[clipIdx1][clipIdx2][coeffIdx1][coeffIdx2]` gives the auto-correlation between two coefficients at their respective clipping levels). The function must compute the error using the formula: error = pixAcc - 2 * sum_i( coeff[i] * y[clip[i]][i] ) + sum_i( coeff[i] * sum_j( coeff[j] * E[clip[i]][clip[j]][i][j] ) ), which is equivalent to the squared error between the filtered output and the original signal. The coefficients are scaled by a factor of `1 << (bitDepth - 1)`, so the final result must be divided by that factor (i.e., `error / (1 << (bitDepth - 1))`). The function must handle an arbitrary number of coefficients (at least 1) and correctly account for the symmetric nature of `E` (i.e., `E[a][b][i][j] == E[b][a][j][i]`) by computing the sum efficiently without double-counting off-diagonal terms.

// The core algorithm directly implements the algebraic expansion of the squared error for a linear filter. For a given set of coefficients `coeff` (length `numCoeff`) and clipping indices `clip` (also length `numCoeff`), the distortion can be computed by iterating over all coefficient pairs. The straightforward approach would be to compute a double sum over all `i` and `j`, but since `E` is symmetric (`E[clip[i]][clip[j]][i][j] == E[clip[j]][clip[i]][j][i]`), we only need to iterate over `i <= j` and multiply off-diagonal terms by 2. The formula is: `error = pixAcc - 2 * sum_i(coeff[i] * y[clip[i]][i]) + sum_i sum_j (coeff[i] * coeff[j] * E[clip[i]][clip[j]][i][j])`. To avoid overflow or inefficiency, we can compute the quadratic term as follows: for each `i`, add `coeff[i] * coeff[i] * E[clip[i]][clip[i]][i][i]` and for each pair `i < j`, add `2 * coeff[i] * coeff[j] * E[clip[i]][clip[j]][i][j]`. Then divide the entire result by the scaling factor `1 << (bitDepth - 1)` to get the final distortion in the same scale as `pixAcc`. Edge cases include: `numCoeff` being 0 (return 0), coefficients being zero (which automatically reduce the error), and clipping indices being out of bounds (undefined behavior if not validated, but the function assumes valid indices). The time complexity is O(numCoeff^2) due to the double loop over pairs, and space complexity is O(1) since we only accumulate a few double variables. The implementation should be careful to use `double` for all calculations to preserve precision, as the inputs are doubles. We can structure the function to take arrays as pointers or `std::vector`, but for simplicity and performance, we'll use raw pointers with a size parameter. The function should be `const`-correct and can be a free function named `computeFilteredError`. We also need to include a small helper to handle the case where `numCoeff` is 1.

#include <cstddef>
#include <vector>
#include <cmath>

// Computes the filtered distortion error for an adaptive filter.
// The covariance structure stores statistics for a set of coefficients.
struct Covariance {
    double pixAcc;  // Sum of squared original errors
    // y[clipIdx][coeffIdx] - cross-correlation with original error
    // E[clipIdx1][clipIdx2][coeffIdx1][coeffIdx2] - auto-correlation
    const std::vector<std::vector<double>>& y;
    const std::vector<std::vector<std::vector<std::vector<double>>>>& E;
    
    Covariance(double pix, 
               const std::vector<std::vector<double>>& yRef,
               const std::vector<std::vector<std::vector<std::vector<double>>>>& eRef)
        : pixAcc(pix), y(yRef), E(eRef) {}
};

// Computes error = (pixAcc - 2*sum_i(c_i*y_i) + sum_i sum_j c_i*c_j*E_ij) / (1 << (bitDepth-1))
double computeFilteredError(const Covariance& cov,
                            const int* clip,      // clipping index for each coefficient
                            const double* coeff,  // filter coefficients
                            int numCoeff,         // number of coefficients
                            int bitDepth) {
    if (numCoeff <= 0) {
        return 0.0;
    }
    
    double factor = static_cast<double>(1 << (bitDepth - 1));
    double error = cov.pixAcc;
    
    // Linear term: -2 * sum_i coeff[i] * y[clip[i]][i]
    for (int i = 0; i < numCoeff; ++i) {
        error -= 2.0 * coeff[i] * cov.y[clip[i]][i];
    }
    
    // Quadratic term: sum_i c_i^2 * E[i][i] + 2 * sum_{i<j} c_i * c_j * E[i][j]
    for (int i = 0; i < numCoeff; ++i) {
        double ci = coeff[i];
        int clipI = clip[i];
        // Diagonal term
        error += ci * ci * cov.E[clipI][clipI][i][i];
        // Off-diagonal terms
        for (int j = i + 1; j < numCoeff; ++j) {
            double cj = coeff[j];
            int clipJ = clip[j];
            error += 2.0 * ci * cj * cov.E[clipI][clipJ][i][j];
        }
    }
    
    return error / factor;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Create a small covariance structure for testing
    // numCoeff = 3, numBins = 2 (clipping indices 0 and 1)
    int numCoeff = 3;
    int numBins = 2;
    
    // Initialize y: y[bin][coeff]
    std::vector<std::vector<double>> y(numBins, std::vector<double>(numCoeff, 0.0));
    y[0][0] = 1.0; y[0][1] = 2.0; y[0][2] = 3.0;
    y[1][0] = 4.0; y[1][1] = 5.0; y[1][2] = 6.0;
    
    // Initialize E: E[bin1][bin2][coeff1][coeff2]
    // We'll fill with simple values for testing
    std::vector<std::vector<std::vector<std::vector<double>>>> E(
        numBins, std::vector<std::vector<std::vector<double>>>(
            numBins, std::vector<std::vector<double>>(
                numCoeff, std::vector<double>(numCoeff, 0.0))));
    for (int b1 = 0; b1 < numBins; ++b1) {
        for (int b2 = 0; b2 < numBins; ++b2) {
            for (int i = 0; i < numCoeff; ++i) {
                for (int j = 0; j < numCoeff; ++j) {
                    // Simple deterministic values: E = i + j + b1 + b2
                    E[b1][b2][i][j] = i + j + b1 + b2;
                }
            }
        }
    }
    
    double pixAcc = 10.0;
    Covariance cov(pixAcc, y, E);
    
    // Test 1: All coefficients zero, any clips -> error = pixAcc / factor
    int clip1[3] = {0, 0, 0};
    double coeff1[3] = {0.0, 0.0, 0.0};
    int bitDepth = 8;
    double factor = static_cast<double>(1 << (bitDepth - 1));
    double expected1 = pixAcc / factor;
    double result1 = computeFilteredError(cov, clip1, coeff1, 3, bitDepth);
    assert(std::fabs(result1 - expected1) < 1e-9);
    
    // Test 2: Single coefficient (numCoeff=1)
    int clip2[1] = {1};
    double coeff2[1] = {2.0};
    // error = pixAcc - 2*2*y[1][0] + 2^2*E[1][1][0][0] = 10 - 4*4 + 4*E[1][1][0][0]
    // E[1][1][0][0] = 0+0+1+1 = 2
    // error = 10 - 16 + 8 = 2
    double expected2 = 2.0 / factor;
    double result2 = computeFilteredError(cov, clip2, coeff2, 1, bitDepth);
    assert(std::fabs(result2 - expected2) < 1e-9);
    
    // Test 3: Two coefficients, non-zero values
    int clip3[2] = {0, 1};
    double coeff3[2] = {1.0, 2.0};
    // Compute manually:
    // Linear term: -2*(1*y[0][0] + 2*y[1][1]) = -2*(1*1 + 2*5) = -2*(1+10) = -22
    // Quadratic diagonal: 1^2*E[0][0][0][0] + 2^2*E[1][1][1][1] = 1*0 + 4*4 = 16
    // Off-diagonal: 2*1*2*E[0][1][0][1] = 4*(0+1+0+1) = 4*2 = 8
    // Total error = 10 - 22 + 16 + 8 = 12
    double expected3 = 12.0 / factor;
    double result3 = computeFilteredError(cov, clip3, coeff3, 2, bitDepth);
    assert(std::fabs(result3 - expected3) < 1e-9);
    
    // Test 4: All coefficients with clips, check symmetry handling
    int clip4[3] = {1, 0, 1};
    double coeff4[3] = {0.5, -1.0, 2.0};
    // Compute using the formula directly (brute-force double sum) as reference
    double refError = pixAcc;
    for (int i = 0; i < 3; ++i) {
        refError -= 2.0 * coeff4[i] * y[clip4[i]][i];
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            refError += coeff4[i] * coeff4[j] * E[clip4[i]][clip4[j]][i][j];
        }
    }
    refError /= factor;
    double result4 = computeFilteredError(cov, clip4, coeff4, 3, bitDepth);
    assert(std::fabs(result4 - refError) < 1e-9);
    
    // Test 5: Zero coefficients but non-zero pixAcc
    int clip5[3] = {1, 1, 0};
    double coeff5[3] = {0.0, 0.0, 0.0};
    double expected5 = pixAcc / factor;
    double result5 = computeFilteredError(cov, clip5, coeff5, 3, bitDepth);
    assert(std::fabs(result5 - expected5) < 1e-9);
    
    return 0;
}

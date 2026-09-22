Write a C++ function `correlated_f2_matrix` that takes two matrices of allele frequencies (`afmat1` and `afmat2`) and two matrices of sample counts (`countmat1` and `countmat2`), where each matrix has rows corresponding to SNPs and columns to populations. The function returns a 3D array (a `std::vector<std::vector<std::vector<double>>>` or a simple custom 3D structure you may define) where the element at index `[i][j][k]` equals the squared difference between the allele frequency of population `i` in the first matrix and population `j` in the second matrix, evaluated at SNP `k`, adjusted by subtracting a correction term. Specifically, for each SNP `k`, population pair `(i,j)`, compute `(afmat1[k][i] - afmat2[k][j])^2` minus the sum `(afmat1[k][i]*(1-afmat1[k][i]) / max(1, countmat1[k][i]-1)) + (afmat2[k][j]*(1-afmat2[k][j]) / max(1, countmat2[k][j]-1))`. All inputs are guaranteed to have matching row counts (number of SNPs) and valid column counts. Handle cases where counts are 0 or 1 by using `max(1, count-1)` to avoid division by zero. The output dimensions must be `nc1 × nc2 × nsnp` where `nc1` and `nc2` are the column counts of the input matrices and `nsnp` is the number of SNPs (rows). Use row-major order for the output structure.

The solution iterates over every SNP index `k` from 0 to `nsnp-1`, and for each SNP iterates over all pairs of populations `(i,j)` from the first and second matrices. For each pair, we compute the raw squared difference and the correction term using the given formulas. The correction denominator uses `max(1, count-1)` to ensure we never divide by zero and to treat counts of 0 or 1 as having denominator 1 (since `count-1` could be 0 or negative). The output is a 3D structure with dimensions `nc1` (first index), `nc2` (second index), and `nsnp` (third index). We allocate this structure upfront. The time complexity is O(nc1 * nc2 * nsnp), and the space complexity is also O(nc1 * nc2 * nsnp) for the output. Edge cases include zero rows (empty SNP set) which produce an empty output, and populations with zero counts which are handled by the `max` guard. Since all inputs are assumed to be valid and consistent in row count, no special validity checks are needed beyond the denominator adjustment.

#include <vector>
#include <algorithm>

// Return a 3D vector: dimensions [nc1][nc2][nsnp]
// afmat1 and afmat2 are tsnp x nc1 and nsnp x nc2 matrices in row-major (vector of rows)
std::vector<std::vector<std::vector<double>>> correlated_f2_matrix(
    const std::vector<std::vector<double>>& afmat1,
    const std::vector<std::vector<double>>& afmat2,
    const std::vector<std::vector<double>>& countmat1,
    const std::vector<std::vector<double>>& countmat2) {

    int nsnp = afmat1.size();
    int nc1 = afmat1.empty() ? 0 : afmat1[0].size();
    int nc2 = afmat2.empty() ? 0 : afmat2[0].size();

    // Prepare output with dimensions nc1 x nc2 x nsnp
    std::vector<std::vector<std::vector<double>>> out(
        nc1, std::vector<std::vector<double>>(
            nc2, std::vector<double>(nsnp, 0.0)));

    for (int k = 0; k < nsnp; ++k) {
        for (int i = 0; i < nc1; ++i) {
            double a = afmat1[k][i];
            double c1 = countmat1[k][i];
            double denom1 = std::max(1.0, c1 - 1.0);
            double corr1 = a * (1.0 - a) / denom1;

            for (int j = 0; j < nc2; ++j) {
                double b = afmat2[k][j];
                double c2 = countmat2[k][j];
                double denom2 = std::max(1.0, c2 - 1.0);
                double corr2 = b * (1.0 - b) / denom2;

                double raw = (a - b) * (a - b);
                out[i][j][k] = raw - (corr1 + corr2);
            }
        }
    }

    return out;
}

#include <cassert>
#include <cmath>

int main() {
    // Simple case with 1 SNP, 1 population each
    std::vector<std::vector<double>> af1 = {{0.5}};
    std::vector<std::vector<double>> af2 = {{0.5}};
    std::vector<std::vector<double>> cnt1 = {{10}};
    std::vector<std::vector<double>> cnt2 = {{10}};
    auto out = correlated_f2_matrix(af1, af2, cnt1, cnt2);
    assert(out.size() == 1);
    assert(out[0].size() == 1);
    assert(out[0][0].size() == 1);
    // raw = (0.5-0.5)^2 = 0
    // corr1 = 0.5*0.5/9 = 0.027777..., corr2 same
    // result = 0 - 0.055555... = -0.055555...
    assert(std::abs(out[0][0][0] - (-2.0 * 0.25 / 9.0)) < 1e-9);

    // Two populations each, two SNPs, with count=0 and count=1 to test denominator guard
    std::vector<std::vector<double>> af1 = {{0.0, 0.2}, {0.4, 0.6}}; // 2 SNPs, 2 pops
    std::vector<std::vector<double>> af2 = {{0.1, 0.3}, {0.5, 0.7}};
    std::vector<std::vector<double>> cnt1 = {{0, 1}, {5, 5}};
    std::vector<std::vector<double>> cnt2 = {{1, 2}, {3, 4}};
    auto out2 = correlated_f2_matrix(af1, af2, cnt1, cnt2);
    assert(out2.size() == 2);
    assert(out2[0].size() == 2);
    assert(out2[0][0].size() == 2);

    // Check (i=0,j=0,k=0): af1[0][0]=0, af2[0][0]=0.1
    // raw = 0.01, corr1 = 0*1/max(1,0-1)=0/1=0, corr2 = 0.1*0.9/max(1,1-1)=0.09/1=0.09
    // result = 0.01 - 0.09 = -0.08
    assert(std::abs(out2[0][0][0] - (-0.08)) < 1e-9);

    // Check (i=1,j=1,k=1): af1[1][1]=0.6, af2[1][1]=0.7
    // raw = 0.01, corr1 = 0.6*0.4/max(1,5-1)=0.24/4=0.06
    // corr2 = 0.7*0.3/max(1,4-1)=0.21/3=0.07
    // result = 0.01 - 0.13 = -0.12
    assert(std::abs(out2[1][1][1] - (-0.12)) < 1e-9);

    // Zero SNPs case
    std::vector<std::vector<double>> af_empty;
    std::vector<std::vector<double>> cnt_empty;
    auto out_empty = correlated_f2_matrix(af_empty, af_empty, cnt_empty, cnt_empty);
    assert(out_empty.empty()); // nc1=0

    // Check that output dimensions match expectations for 1 SNP, 2x3 populations
    std::vector<std::vector<double>> af3 = {{0.1, 0.2}}; // 1 SNP, 2 pops
    std::vector<std::vector<double>> af4 = {{0.3, 0.4, 0.5}}; // 1 SNP, 3 pops
    std::vector<std::vector<double>> cnt3 = {{9, 9}};
    std::vector<std::vector<double>> cnt4 = {{9, 9, 9}};
    auto out3 = correlated_f2_matrix(af3, af4, cnt3, cnt4);
    assert(out3.size() == 2);
    assert(out3[0].size() == 3);
    assert(out3[0][0].size() == 1);
    assert(out3[1][2].size() == 1);

    // Manual check for (i=1,j=2,k=0)
    // af1[0][1]=0.2, af2[0][2]=0.5
    // raw = 0.09, corr1=0.2*0.8/8=0.02, corr2=0.5*0.5/8=0.03125
    // result = 0.09 - 0.05125 = 0.03875
    assert(std::abs(out3[1][2][0] - 0.03875) < 1e-9);
}

Write a C++ function `HJM_Yield_to_Forward` that, given an array of zero-coupon yield rates `pdYield` for maturities 1, 2, ..., iN (where index 0 corresponds to maturity 1, index i-1 corresponds to maturity i), and the number of time steps `iN`, computes the corresponding forward rates `pdForward` for the same maturities. The forward rate from time i-1 to i (for i ≥ 1) is defined as `pdForward[i] = (i+1)*pdYield[i] - i*pdYield[i-1]`, and for the first maturity (i=0) the forward rate equals the yield: `pdForward[0] = pdYield[0]`. The function must store the results in the output array `pdForward` and return an integer success flag (1 on success). The input `pdYield` must not be modified. Assume `iN ≥ 1` and that all arrays have at least `iN` elements. This is a standalone mathematical utility function that should not rely on any external libraries other than standard headers.
// The problem is a direct application of the relationship between zero-coupon yields and forward rates in discrete time. For a zero-coupon bond maturing at time t (where t is an integer number of periods), the yield `Y_t` satisfies `(1+Y_t)^t = 1/(P_t)` where `P_t` is the price. The forward rate `F_{t-1,t}` from time t-1 to t is defined by `(1+F_{t-1,t}) = (1+Y_t)^t / (1+Y_{t-1})^{t-1}`. When yields are expressed as simple per-period rates (not continuously compounded), taking logs and linearizing gives the recurrence `F_{t} = t*Y_t - (t-1)*Y_{t-1}` when yields are small, which is the formula given. Here, because indices are zero-based, maturity i corresponds to time i+1, so `pdForward[i] = (i+1)*pdYield[i] - i*pdYield[i-1]` for i ≥ 1, and `pdForward[0] = pdYield[0]` (since there is no prior maturity). The algorithm is a simple loop from i=1 to iN-1. Edge cases: when iN = 1, the loop does not execute, and only `pdForward[0]` is set. Negative or zero yields are allowed; the formula is linear and works for any real numbers. Time complexity is O(iN), space complexity is O(1) beyond the input/output arrays. The function should be `const`-correct by taking `pdYield` as a pointer to const data, and it should not modify the input.
#include <cstddef>

// Computes forward rates from zero-coupon yield rates.
// pdForward: output array of size iN (must be preallocated)
// iN: number of time steps / maturities
// pdYield: input array of size iN, not modified
// Returns 1 on success.
int HJM_Yield_to_Forward(double *pdForward, int iN, const double *pdYield) {
    if (iN <= 0 || pdForward == nullptr || pdYield == nullptr) {
        return 0;
    }
    pdForward[0] = pdYield[0];
    for (int i = 1; i < iN; ++i) {
        pdForward[i] = (i + 1) * pdYield[i] - i * pdYield[i - 1];
    }
    return 1;
}
#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function under test
int HJM_Yield_to_Forward(double *pdForward, int iN, const double *pdYield);

int main() {
    // Test 1: iN = 1, only one maturity
    {
        double yields[] = {0.05};
        double forwards[1];
        int result = HJM_Yield_to_Forward(forwards, 1, yields);
        assert(result == 1);
        assert(std::fabs(forwards[0] - 0.05) < 1e-12);
    }

    // Test 2: iN = 2, basic case
    {
        double yields[] = {0.03, 0.04};
        double forwards[2];
        int result = HJM_Yield_to_Forward(forwards, 2, yields);
        assert(result == 1);
        assert(std::fabs(forwards[0] - 0.03) < 1e-12);
        // forwards[1] = 2*0.04 - 1*0.03 = 0.05
        assert(std::fabs(forwards[1] - 0.05) < 1e-12);
    }

    // Test 3: iN = 4, known values
    {
        double yields[] = {0.01, 0.02, 0.03, 0.04};
        double forwards[4];
        int result = HJM_Yield_to_Forward(forwards, 4, yields);
        assert(result == 1);
        assert(std::fabs(forwards[0] - 0.01) < 1e-12);
        // forwards[1] = 2*0.02 - 1*0.01 = 0.03
        assert(std::fabs(forwards[1] - 0.03) < 1e-12);
        // forwards[2] = 3*0.03 - 2*0.02 = 0.05
        assert(std::fabs(forwards[2] - 0.05) < 1e-12);
        // forwards[3] = 4*0.04 - 3*0.03 = 0.07
        assert(std::fabs(forwards[3] - 0.07) < 1e-12);
    }

    // Test 4: yields with negative rates
    {
        double yields[] = {-0.01, -0.005, 0.0};
        double forwards[3];
        int result = HJM_Yield_to_Forward(forwards, 3, yields);
        assert(result == 1);
        // forwards[0] = -0.01
        assert(std::fabs(forwards[0] - (-0.01)) < 1e-12);
        // forwards[1] = 2*(-0.005) - 1*(-0.01) = -0.01 + 0.01 = 0.0
        assert(std::fabs(forwards[1] - 0.0) < 1e-12);
        // forwards[2] = 3*0.0 - 2*(-0.005) = 0 + 0.01 = 0.01
        assert(std::fabs(forwards[2] - 0.01) < 1e-12);
    }

    // Test 5: invalid input (null pointer) returns 0
    {
        double yields[] = {0.01};
        double forwards[1];
        assert(HJM_Yield_to_Forward(nullptr, 1, yields) == 0);
        assert(HJM_Yield_to_Forward(forwards, 0, yields) == 0);
        assert(HJM_Yield_to_Forward(forwards, 1, nullptr) == 0);
    }

    // Test 6: ensure input array is not modified
    {
        double yields[] = {0.02, 0.03, 0.04};
        double original[] = {0.02, 0.03, 0.04};
        double forwards[3];
        HJM_Yield_to_Forward(forwards, 3, yields);
        for (int i = 0; i < 3; ++i) {
            assert(std::fabs(yields[i] - original[i]) < 1e-12);
        }
    }

    // Test 7: large iN
    {
        int n = 1000;
        std::vector<double> yields(n);
        std::vector<double> forwards(n);
        // Use a simple pattern: yields[i] = 0.001 * i
        for (int i = 0; i < n; ++i) yields[i] = 0.001 * i;
        int result = HJM_Yield_to_Forward(forwards.data(), n, yields.data());
        assert(result == 1);
        // forwards[i] should equal 0.001 for all i? Let's compute: 
        // forwards[i] = (i+1)*0.001*i - i*0.001*(i-1) = 0.001*i^2 + 0.001*i - 0.001*i^2 + 0.001*i = 0.002*i
        for (int i = 1; i < n; ++i) {
            double expected = 0.002 * i;
            assert(std::fabs(forwards[i] - expected) < 1e-9);
        }
        assert(std::fabs(forwards[0] - 0.0) < 1e-12);
    }

    return 0;
}

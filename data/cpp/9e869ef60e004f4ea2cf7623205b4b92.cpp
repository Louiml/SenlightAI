/*
Write a C++ function that takes two real-valued 2D arrays `gammaReal` and `gammaImag` representing the real and imaginary components of a 2D complex field, along with dimensions `Nx` and `Ny`, and transforms them using a Fourier-based operator. Specifically, the function must compute two output real arrays `kappaE` and `kappaB` by: (1) performing a 2D FFT of both input arrays, (2) applying a frequency-domain multiplication with coefficients \( C_1 = (x^2-y^2)/(x^2+y^2) \) and \( C_2 = 2xy/(x^2+y^2) \) (with \( C_1=C_2=0 \) at the zero-frequency bin), and (3) performing an inverse 2D FFT and taking only the real part of each result. The function must allocate and free all temporary Fourier-domain and complex arrays internally, respect row-major indexing where position `(x,y)` maps to `x*Ny+y`, and normalize the result by dividing by the total number of elements `Nx*Ny` because FFTW’s inverse transform is unnormalized. The output arrays must contain exactly the computed real parts, matching the expected physical result for inputs that are localized real-valued signals. Provide only the function implementation, not a `main` function, but structure it so that the test code can call it easily with `float` arrays.
*/
#include <fftw3.h>
#include <cmath>
#include <cstddef>

/**
 * Compute kappaE and kappaB from the real and imaginary parts of gamma
 * using a Fourier-domain operator with coefficients C1 and C2.
 * The inputs are float arrays of size Nx*Ny in row-major order.
 * The outputs are float arrays of the same size, containing the real part
 * of the inverse FFT, normalized by dividing by (Nx*Ny).
 */
void gamma2kappa(const float* gammaReal, const float* gammaImag,
                 float* kappaE, float* kappaB,
                 int Nx, int Ny)
{
    const std::size_t total = static_cast<std::size_t>(Nx) * static_cast<std::size_t>(Ny);

    // Allocate complex arrays for real/imag components of gamma
    fftw_complex* gammaRealComplex = fftw_alloc_complex(total);
    fftw_complex* gammaImagComplex = fftw_alloc_complex(total);

    // Copy real inputs into complex arrays with zero imaginary part
    for (std::size_t i = 0; i < total; ++i) {
        gammaRealComplex[i][0] = static_cast<double>(gammaReal[i]);
        gammaRealComplex[i][1] = 0.0;
        gammaImagComplex[i][0] = static_cast<double>(gammaImag[i]);
        gammaImagComplex[i][1] = 0.0;
    }

    // Allocate Fourier-domain arrays
    fftw_complex* gammaRealHat = fftw_alloc_complex(total);
    fftw_complex* gammaImagHat = fftw_alloc_complex(total);

    // Forward FFTs
    fftw_plan planReal = fftw_plan_dft_2d(Nx, Ny, gammaRealComplex, gammaRealHat,
                                          FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan planImag = fftw_plan_dft_2d(Nx, Ny, gammaImagComplex, gammaImagHat,
                                          FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(planReal);
    fftw_execute(planImag);
    fftw_destroy_plan(planReal);
    fftw_destroy_plan(planImag);

    // Allocate output Fourier-domain arrays
    fftw_complex* kappaEHat = fftw_alloc_complex(total);
    fftw_complex* kappaBHat = fftw_alloc_complex(total);

    // Apply frequency-domain multiplication
    for (int x = 0; x < Nx; ++x) {
        for (int y = 0; y < Ny; ++y) {
            const std::size_t pos = static_cast<std::size_t>(x) * Ny + y;

            double C1, C2;
            if (x == 0 && y == 0) {
                C1 = 0.0;
                C2 = 0.0;
            } else {
                const double denom = static_cast<double>(x) * x + static_cast<double>(y) * y;
                C1 = (static_cast<double>(x) * x - static_cast<double>(y) * y) / denom;
                C2 = 2.0 * static_cast<double>(x) * static_cast<double>(y) / denom;
            }

            const double gR_r = gammaRealHat[pos][0];
            const double gR_i = gammaRealHat[pos][1];
            const double gI_r = gammaImagHat[pos][0];
            const double gI_i = gammaImagHat[pos][1];

            // kappaE = C1*gammaReal + C2*gammaImag
            kappaEHat[pos][0] = C1 * gR_r + C2 * gI_r;
            kappaEHat[pos][1] = C1 * gR_i + C2 * gI_i;

            // kappaB = C2*gammaReal - C1*gammaImag
            kappaBHat[pos][0] = C2 * gR_r - C1 * gI_r;
            kappaBHat[pos][1] = C2 * gR_i - C1 * gI_i;
        }
    }

    // Inverse FFTs
    fftw_complex* kappaEComplex = fftw_alloc_complex(total);
    fftw_complex* kappaBComplex = fftw_alloc_complex(total);
    fftw_plan planE = fftw_plan_dft_2d(Nx, Ny, kappaEHat, kappaEComplex,
                                       FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_plan planB = fftw_plan_dft_2d(Nx, Ny, kappaBHat, kappaBComplex,
                                       FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(planE);
    fftw_execute(planB);
    fftw_destroy_plan(planE);
    fftw_destroy_plan(planB);

    // Extract real parts and normalize
    const double invTotal = 1.0 / static_cast<double>(total);
    for (std::size_t i = 0; i < total; ++i) {
        kappaE[i] = static_cast<float>(kappaEComplex[i][0] * invTotal);
        kappaB[i] = static_cast<float>(kappaBComplex[i][0] * invTotal);
    }

    // Clean up
    fftw_free(kappaBComplex);
    fftw_free(kappaEComplex);
    fftw_free(kappaBHat);
    fftw_free(kappaEHat);
    fftw_free(gammaImagHat);
    fftw_free(gammaRealHat);
    fftw_free(gammaImagComplex);
    fftw_free(gammaRealComplex);
}
#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function under test
void gamma2kappa(const float* gammaReal, const float* gammaImag,
                 float* kappaE, float* kappaB,
                 int Nx, int Ny);

int main() {
    // Test 1: all-zero input yields all-zero output
    {
        const int Nx = 4, Ny = 4;
        std::vector<float> gammaReal(Nx*Ny, 0.0f);
        std::vector<float> gammaImag(Nx*Ny, 0.0f);
        std::vector<float> kappaE(Nx*Ny), kappaB(Nx*Ny);
        gamma2kappa(gammaReal.data(), gammaImag.data(), kappaE.data(), kappaB.data(), Nx, Ny);
        for (float v : kappaE) assert(v == 0.0f);
        for (float v : kappaB) assert(v == 0.0f);
    }

    // Test 2: single nonzero at center (dx=2,dy=2) for 4x4; check output equals input (since C1=C2=0 at DC only, but here the delta gives constant Fourier → inverse gives original)
    {
        const int Nx = 4, Ny = 4;
        std::vector<float> gammaReal(Nx*Ny, 0.0f), gammaImag(Nx*Ny, 0.0f);
        // A single impulse at (1,1) (since C1 and C2 are non-zero for that freq, but we test all-zero imag)
        gammaReal[1*Ny+1] = 2.0f;
        std::vector<float> kappaE(Nx*Ny), kappaB(Nx*Ny);
        gamma2kappa(gammaReal.data(), gammaImag.data(), kappaE.data(), kappaB.data(), Nx, Ny);
        // For an impulse, the output is scaled by sum of coefficients? Since C1+C2 not constant. So just check finite.
        // Better: test identity for a constant field? Use only DC? But DC is zeroed. So test that output does not exceed input magnitude * something.
        // For impulse at (1,1) with Ny=4, the Fourier transform has magnitude 2 at every freq, and applying C1,C2 then inverse gives sum of C1*2+N*... This is complex. Simpler: test with delta at (0,0) but that's DC zeroed.
        // Instead, use a known analytic: put a delta at (1,0) for Nx=4,Ny=4. Compute expected manually? Too messy.
        // Simpler test: use a constant real field of 1.0, but then zero DC kills it. So test with a smooth function like cos(2πx/Nx) — but that's still messy.
        // To keep tests simple, we only check that zero input gives zero output, and that output is finite (no NaN) for random input.
    }

    // Test 3: random input, ensure output is finite and array sizes correct
    {
        const int Nx = 3, Ny = 5;
        std::vector<float> gammaReal(Nx*Ny), gammaImag(Nx*Ny);
        for (int i = 0; i < Nx*Ny; ++i) {
            gammaReal[i] = static_cast<float>(std::sin(i * 0.1));
            gammaImag[i] = static_cast<float>(std::cos(i * 0.2));
        }
        std::vector<float> kappaE(Nx*Ny), kappaB(Nx*Ny);
        gamma2kappa(gammaReal.data(), gammaImag.data(), kappaE.data(), kappaB.data(), Nx, Ny);
        for (float v : kappaE) assert(std::isfinite(v));
        for (float v : kappaB) assert(std::isfinite(v));
    }

    // Test 4: verify normalization: if gammaReal is a constant 1.0 and gammaImag zero, then Fourier domain DC is Nx*Ny, but C1=C2=0 at DC, so output should be zero everywhere (since only DC contributes). Let's test.
    {
        const int Nx = 2, Ny = 2;
        std::vector<float> gammaReal(Nx*Ny, 1.0f), gammaImag(Nx*Ny, 0.0f);
        std::vector<float> kappaE(Nx*Ny), kappaB(Nx*Ny);
        gamma2kappa(gammaReal.data(), gammaImag.data(), kappaE.data(), kappaB.data(), Nx, Ny);
        for (float v : kappaE) assert(std::fabs(v) < 1e-5f);
        for (float v : kappaB) assert(std::fabs(v) < 1e-5f);
    }

    // Test 5: a simple delta at (1,0) for Nx=2,Ny=2: Fourier of delta is 1 everywhere. Applying C1,C2:
    // For (0,0): C1=C2=0.
    // For (1,0): x=1,y=0 -> C1=1, C2=0. Then KappaE_hat = gammaReal_hat (since imag zero) * C1 = 1. Inverse FFT of 1 at freq (1,0) gives a pattern. But we can compute expected manually for 2x2.
    // 2x2 forward FFT of delta at (1,0) gives [1,1;1,1]? Actually delta at (1,0) in 2x2: input array indices: (0,0)=0, (0,1)=0, (1,0)=1, (1,1)=0. Forward FFT (unnormalized) gives:
    // F(0,0)=sum =1, F(0,1)= (0+0*W+1*W^0+0) = 1? Actually for 2-point FFT, but 2D: compute directly. Using formula: F(k,l)=sum_{x,y} f(x,y)*exp(-2πi(kx/Nx+ly/Ny)). For Nx=2,Ny=2, deltas: only (1,0) contributes. So F(k,l)=exp(-2πi(k*1/2+ l*0/2)) = exp(-πik). For k=0:1, k=1: -1. For l=0:1, l=1:1. So array: F(0,0)=1, F(0,1)=1, F(1,0)=-1, F(1,1)=-1.
    // Then apply C1,C2: at (0,0):0; (0,1): denom=1, C1=-1, C2=0. So kappaE_hat = C1*F = -1 * (1) = -1 for (0,1)? Wait gammaReal_hat at (0,1) is 1? Yes. So kappaE_hat(0,1)=-1. Similarly (1,0): C1=1, F=-1 -> -1. (1,1): C1=0? x=1,y=1: denom=2, C1=(1-1)/2=0, C2=2/2=1. So kappaE_hat = C2*gammaImag_hat + C1*gammaReal_hat = 1*0 +0*F=0? Actually imag zero, so 0. So kappaE_hat = [0, -1; -1, 0] in (x,y) order. Inverse FFT of that: unnormalized. Compute inverse of [0,-1;-1,0] for 2x2: Inverse formula: f(x,y)= (1/4) sum k,l F(k,l) exp(+2πi(kx/2+ly/2)). For each (x,y), sum F(k,l)*exp(πi(kx+ly)). For (0,0): sum = 0-1-1+0 = -2. /4 = -0.5. For (0,1): exp factors: for k=0,l=1: exp(πi*1)= -1, times -1 = 1; others: (0,0)=0, (1,0) exp(πi*1*0)=1 times -1 = -1, (1,1) exp(πi(1*0+1*1))= -1 times 0=0. Sum=0+1-1+0=0 -> 0. For (1,0): (0,1) exp(πi*1*0)=1 times -1 = -1; (1,0) exp(πi*1*1)= -1 times -1 = 1; others 0. Sum=0, so 0. For (1,1): (0,1) exp(πi(0+1))= -1 times -1=1; (1,0) exp(πi(1+0))= -1 times -1=1; (1,1) exp(πi(1+1))=1 times 0=0. Sum=2, /4=0.5. So kappaE = [-0.5, 0; 0, 0.5] (row x, col y). Similarly kappaB: at (0,0)=0; (0,1): C2*gammaReal_hat - C1*gammaImag_hat = 0*1 - (-1)*0 =0; (1,0): C2*gammaReal_hat - C1*gammaImag_hat =0* -1 -1*0 =0; (1,1): C2=1, C1=0 -> 1*gammaReal_hat = 1*(-1) = -1. So kappaB_hat = [0,0;0,-1]. Inverse: F = [0,0;0,-1]. Compute: (0,0): sum F*exp(0)= -1, /4 = -0.25; others: (0,1): (1,1) exp(πi)= -1 times -1 = 1? Actually for (x=0,y=1): (1,1) exp(πi(1*0+1*1))= -1 times -1 = 1; sum=1, /4=0.25; (1,0): (1,1) exp(πi(1*1+1*0))= -1 times -1 =1; sum=1/4=0.25; (1,1): (1,1) exp(πi(1+1))=1 times -1 = -1; sum=-1/4= -0.25. So kappaB = [-0.25, 0.25; 0.25, -0.25].
    // We can test these values with tolerance.
    {
        const int Nx = 2, Ny = 2;
        std::vector<float> gammaReal(Nx*Ny, 0.0f), gammaImag(Nx*Ny, 0.0f);
        gammaReal[1*Ny+0] = 1.0f; // index (1,0)
        std::vector<float> kappaE(Nx*Ny), kappaB(Nx*Ny);
        gamma2kappa(gammaReal.data(), gammaImag.data(), kappaE.data(), kappaB.data(), Nx, Ny);
        // Expected kappaE: row-major (x*Ny+y): (0,0):-0.5, (0,1):0, (1,0):0, (1,1):0.5
        assert(std::fabs(kappaE[0] - (-0.5f)) < 1e-4f);
        assert(std::fabs(kappaE[1] - 0.0f) < 1e-4f);
        assert(std::fabs(kappaE[2] - 0.0f) < 1e-4f);
        assert(std::fabs(kappaE[3] - 0.5f) < 1e-4f);
        // Expected kappaB: (0,0):-0.25, (0,1):0.25, (1,0):0.25, (1,1):-0.25
        assert(std::fabs(kappaB[0] - (-0.25f)) < 1e-4f);
        assert(std::fabs(kappaB[1] - 0.25f) < 1e-4f);
        assert(std::fabs(kappaB[2] - 0.25f) < 1e-4f);
        assert(std::fabs(kappaB[3] - (-0.25f)) < 1e-4f);
    }

    return 0;
}
// The solution requires implementing the forward and inverse 2D FFT using the FFTW library. The main algorithm is: first, copy each input float array (real and imaginary components of the gamma field) into separate FFTW complex arrays, with the imaginary part set to zero for each component. Then, perform a forward 2D FFT on each to obtain their Fourier transforms, which are unnormalized complex arrays. Next, in Fourier space, for each frequency bin `(x,y)` from `0` to `Nx-1` and `0` to `Ny-1`, compute the coefficients `C1` and `C2` (with special handling for `x==0 && y==0` to set both to zero to avoid division by zero). Then, linearly combine the real and imaginary parts of the two Fourier-transformed gamma arrays to produce the Fourier-domain representations of `kappaE` and `kappaB` (as two complex arrays). After that, perform an inverse 2D FFT on these two complex arrays, which yields unnormalized results scaled by `Nx*Ny`. Finally, extract only the real components of the inverse transforms, scale them by dividing by `Nx*Ny`, and store them into the output `float` arrays. Edge cases include: the zero-frequency bin requires explicit zeroing of output Fourier coefficients; dimensions must be positive; memory allocation failures should be handled by checking `fftw_malloc` return values (though for simplicity in a reference solution, they can be assumed to succeed). Time complexity is \(O(Nx \cdot Ny \log(Nx \cdot Ny))\) dominated by the FFTs, and space complexity is \(O(Nx \cdot Ny)\) for the temporary complex arrays.

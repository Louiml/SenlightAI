// Write a C++ function named `maskedHioError` that takes four parameters: a pointer to an array of complex numbers (`std::complex<float>*`) representing the current estimate `g'`, a pointer to a float array (`const float*`) representing a real-valued mask where each element lies in `[0, 1]` and indicates the proportion by which that pixel is considered "masked" (i.e., where the object should be zero), the total number of elements (`unsigned int`), and a boolean flag `invertMask`. The function must compute and return a float representing the normalized object-domain error defined as `sqrt(totalError) / nMaskedPixels`, where `totalError` is the sum over all pixels of `(shouldBeZero * |g'[i]|)`, with `shouldBeZero` equal to `mask[i]` if `invertMask` is `false`, or `1 - mask[i]` if `invertMask` is `true`. `nMaskedPixels` is the sum of all `shouldBeZero` values. If `nMaskedPixels` is zero, the function should return `0.0f` to avoid division by zero. The function must not modify any input data and should be safe to call with `nullptr` mask or data pointers (in which case it returns `0.0f`). Additionally, the function must use `sqrtf` and compute the magnitude of each complex number as `sqrtf(re*re + im*im)` where `re` and `im` are the real and imaginary parts. The solution must be standalone, use only standard C++ headers, and not rely on OpenMP or FFTW.
// The core of the task is straightforward: iterate over all `nElements` positions, compute a weighting factor `shouldBeZero` based on the mask and the `invertMask` flag, accumulate the weighted magnitude into `totalError`, and accumulate the weight itself into `nMaskedPixels`. The function must handle degenerate cases: if either pointer is null or `nElements` is zero, return `0.0f` immediately. For each element, the mask value is assumed to be in `[0,1]`; we don't need to validate it, but we must handle the case where `shouldBeZero` is zero (no contribution). After the loop, if `nMaskedPixels` is zero, return `0.0f` to avoid division by zero. Otherwise, compute `sqrtf(totalError) / nMaskedPixels`. The time complexity is O(n) where n is `nElements`, and space complexity is O(1) beyond the input pointers. The solution should use `std::complex<float>` from the `<complex>` header and `sqrtf` from `<cmath>`. We'll write the function as a free function in a namespace perhaps, but the task only requires a descriptively named free function. Since the snippet uses `T_COMPLEX` template, we can provide a template version, but to keep it simple and self-contained, we provide a concrete overload for `std::complex<float>`. The solution must be const-correct: both input pointers are `const` (data and mask are read-only). Use `unsigned int` for the loop index and `float` accumulators.
#include <complex>
#include <cmath>

/**
 * Computes the normalized object-domain error for HIO-style phase retrieval.
 *
 * @param gPrime        Pointer to array of complex numbers (size nElements).
 * @param mask          Pointer to float array in [0,1] indicating masked region.
 * @param nElements     Number of elements in both arrays.
 * @param invertMask    If true, use (1 - mask) as the weight; else use mask.
 * @return              sqrt(totalError) / nMaskedPixels, or 0.0f if no masked pixels.
 */
float maskedHioError(
    const std::complex<float>* const gPrime,
    const float* const mask,
    unsigned int const nElements,
    bool const invertMask
) {
    if (gPrime == nullptr || mask == nullptr || nElements == 0) {
        return 0.0f;
    }

    float totalError = 0.0f;
    float nMaskedPixels = 0.0f;

    for (unsigned int i = 0; i < nElements; ++i) {
        const float shouldBeZero = invertMask ? (1.0f - mask[i]) : mask[i];
        if (shouldBeZero == 0.0f) {
            continue;
        }
        const float re = gPrime[i].real();
        const float im = gPrime[i].imag();
        const float magnitude = sqrtf(re * re + im * im);
        totalError += shouldBeZero * magnitude;
        nMaskedPixels += shouldBeZero;
    }

    if (nMaskedPixels == 0.0f) {
        return 0.0f;
    }

    return sqrtf(totalError) / nMaskedPixels;
}
#include <cassert>
#include <complex>
#include <vector>

int main() {
    // Test 1: All mask zeros (nothing masked) -> no masked pixels -> return 0.
    std::vector<std::complex<float>> data1 = {{1.0f, 0.0f}, {2.0f, 0.0f}, {3.0f, 0.0f}};
    std::vector<float> mask1 = {0.0f, 0.0f, 0.0f};
    assert(maskedHioError(data1.data(), mask1.data(), 3, false) == 0.0f);

    // Test 2: All mask ones, invertMask false -> all pixels contribute.
    // totalError = 1 + 2 + 3 = 6, nMaskedPixels = 3, result = sqrt(6)/3 ≈ 0.81649658.
    std::vector<float> mask2 = {1.0f, 1.0f, 1.0f};
    float result2 = maskedHioError(data1.data(), mask2.data(), 3, false);
    float expected2 = sqrtf(6.0f) / 3.0f;
    assert(fabs(result2 - expected2) < 1e-6f);

    // Test 3: Invert mask: mask = {1,0,1}, invertMask=true -> shouldBeZero = {0,1,0}.
    // Only middle element contributes: totalError = 2, nMaskedPixels = 1, result = sqrt(2)/1.
    std::vector<float> mask3 = {1.0f, 0.0f, 1.0f};
    float result3 = maskedHioError(data1.data(), mask3.data(), 3, true);
    assert(fabs(result3 - sqrtf(2.0f)) < 1e-6f);

    // Test 4: Null pointers -> return 0.
    assert(maskedHioError(nullptr, mask1.data(), 3, false) == 0.0f);
    assert(maskedHioError(data1.data(), nullptr, 3, false) == 0.0f);
    assert(maskedHioError(nullptr, nullptr, 3, false) == 0.0f);

    // Test 5: Zero elements -> return 0.
    assert(maskedHioError(data1.data(), mask1.data(), 0, false) == 0.0f);

    // Test 6: Fractional mask values. mask = {0.5, 0.0, 0.5}, invert=false.
    // shouldBeZero = {0.5, 0, 0.5}, totalError = 0.5*1 + 0 + 0.5*3 = 2.0, nMasked=1.0, result=sqrt(2)/1.
    std::vector<float> mask6 = {0.5f, 0.0f, 0.5f};
    float result6 = maskedHioError(data1.data(), mask6.data(), 3, false);
    assert(fabs(result6 - sqrtf(2.0f)) < 1e-6f);

    // Test 7: Complex with imaginary parts. data = {1+i, 0+0i, 0+2i}, mask all ones.
    // magnitudes: sqrt(2), 0, 2. totalError = sqrt(2)+2, nMasked=3, result = sqrt(sqrt2+2)/3.
    std::vector<std::complex<float>> data7 = {{1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 2.0f}};
    std::vector<float> mask7 = {1.0f, 1.0f, 1.0f};
    float expected7 = sqrtf(sqrtf(2.0f) + 2.0f) / 3.0f;
    float result7 = maskedHioError(data7.data(), mask7.data(), 3, false);
    assert(fabs(result7 - expected7) < 1e-6f);

    return 0;
}

Write a C++ function named `mixedPrecisionDotProduct` that takes two vectors of single-precision `float` values, along with stride parameters `incx` and `incy`, and computes their dot product using double-precision accumulation. The function must handle all combinations of positive and negative strides, including zero strides, and return `0.0` if the vector length `n` is `0` or negative. For negative strides, the vector elements must be accessed in reverse order starting from the last element in logical order (index `(n-1) * stride`). The function should return a `double` and be `const`-correct with respect to the input arrays and strides.
#include <cassert>

int main() {
    // Basic positive strides
    float x1[] = {1.0f, 2.0f, 3.0f};
    float y1[] = {4.0f, 5.0f, 6.0f};
    assert(mixedPrecisionDotProduct(x1, 3, 1, y1, 1) == 32.0);

    // Negative strides (reverse order)
    // x reversed: 3,2,1 ; y reversed: 6,5,4 => 3*6+2*5+1*4 = 18+10+4=32
    assert(mixedPrecisionDotProduct(x1, 3, -1, y1, -1) == 32.0);

    // Mixed strides
    // x forward: 1,2,3 ; y reversed: 6,5,4 => 1*6+2*5+3*4 = 6+10+12=28
    assert(mixedPrecisionDotProduct(x1, 3, 1, y1, -1) == 28.0);
    // x reversed: 3,2,1 ; y forward: 4,5,6 => 3*4+2*5+1*6 = 12+10+6=28
    assert(mixedPrecisionDotProduct(x1, 3, -1, y1, 1) == 28.0);

    // n <= 0
    assert(mixedPrecisionDotProduct(x1, 0, 1, y1, 1) == 0.0);
    assert(mixedPrecisionDotProduct(x1, -2, 1, y1, 1) == 0.0);

    // single element
    float x2[] = {2.5f};
    float y2[] = {4.0f};
    assert(mixedPrecisionDotProduct(x2, 1, 1, y2, 1) == 10.0);

    // zero stride (guard)
    assert(mixedPrecisionDotProduct(x1, 3, 0, y1, 1) == 0.0);
}
#include <cstddef>

// Compute the dot product of two float vectors with arbitrary strides,
// accumulating in double precision. Returns 0.0 for n <= 0 or zero strides.
double mixedPrecisionDotProduct(const float* x, int n, int incx,
                                const float* y, int incy) {
    if (n <= 0 || incx == 0 || incy == 0) {
        return 0.0;
    }

    // Determine starting pointer and positive stride for x
    const float* x_start = x;
    int x_stride = incx;
    if (incx < 0) {
        x_start = x + (n - 1) * incx; // points to last logical element
        x_stride = -incx;
    }

    // Same for y
    const float* y_start = y;
    int y_stride = incy;
    if (incy < 0) {
        y_start = y + (n - 1) * incy;
        y_stride = -incy;
    }

    double sum = 0.0;
    const float* x_ptr = x_start;
    const float* y_ptr = y_start;
    for (int i = 0; i < n; ++i) {
        sum += static_cast<double>(*x_ptr) * static_cast<double>(*y_ptr);
        x_ptr += x_stride;
        y_ptr += y_stride;
    }
    return sum;
}
// The main algorithm iterates over the logical indices `0` to `n-1`, mapping each logical index `i` to physical memory positions `x[i * incx]` and `y[i * incy]`. For negative strides, accessing `x[i * incx]` where `incx` is negative would produce negative indices, so the mapping must be adjusted: for a negative stride `s`, the physical starting pointer is `x` but logical index `i` corresponds to `x[(n-1 - i) * (-s)]`. However, a simpler approach is to compute the effective starting pointer and use a positive stride magnitude. For example, if `incx < 0`, the first element to read is at index `(n-1) * incx` (which is negative), so we adjust the pointer to `x + (n-1) * incx` and then use stride `-incx` to move forward positively. This avoids negative indexing. All combinations (both positive, both negative, one positive one negative) can be unified by computing a starting index and a positive magnitude for each vector. Edge cases: `n <= 0` returns `0.0`; zero strides are technically undefined in BLAS but here they would cause infinite loops or repeated elements—we treat zero stride as a special case where we read the same element for all `i` (but the task expects a well-defined result, so we can return `0.0` for zero strides as a guard). The complexity is `O(n)` time and `O(1)` extra space.

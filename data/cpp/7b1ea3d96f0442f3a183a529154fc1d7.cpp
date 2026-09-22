/*
Write a C++ function named `arrayMemoryAddresses` that takes a fixed-size array of exactly three `double` elements by const reference, prints the memory address of the array itself (using `&array`) and the memory addresses of each element (`&array[0]`, `&array[1]`, `&array[2]`), and returns the difference (in bytes) between the address of the last element and the address of the first element, as a `ptrdiff_t`. The function must not modify the array contents and must work correctly regardless of the values stored in the array. The output format should be exactly: `"array address: <address>\n"`, `"a[0] address: <address>\n"`, `"a[1] address: <address>\n"`, `"a[2] address: <address>\n"` without extra spaces. Assume the array is contiguous in memory; the returned difference should always be `2 * sizeof(double)` when the array is allocated as a normal local array.
*/

#include <cstddef>   // for ptrdiff_t
#include <iostream>  // for std::cout, std::endl

// Prints memory addresses of the array and its three elements.
// Returns the byte difference between a[2] and a[0].
// The array is passed by const reference to preserve its size and modifiability.
ptrdiff_t arrayMemoryAddresses(const double (&arr)[3]) {
    // Print addresses as const void* to avoid arithmetic on double*.
    std::cout << "array address: " << &arr << '\n';
    std::cout << "a[0] address: " << &arr[0] << '\n';
    std::cout << "a[1] address: " << &arr[1] << '\n';
    std::cout << "a[2] address: " << &arr[2] << std::endl;

    // Compute byte difference using char* pointer arithmetic.
    const char* first = reinterpret_cast<const char*>(&arr[0]);
    const char* last  = reinterpret_cast<const char*>(&arr[2]);
    return last - first;
}

#include <cassert>
#include <cstddef>

// The free function is declared here (in a real test it would be included).
ptrdiff_t arrayMemoryAddresses(const double (&arr)[3]);

int main() {
    // Test 1: Typical values, verify byte difference is exactly 2 * sizeof(double).
    double a1[3] = {11.11, 33.11, 55.55};
    ptrdiff_t diff1 = arrayMemoryAddresses(a1);
    assert(diff1 == static_cast<ptrdiff_t>(2 * sizeof(double)));

    // Test 2: Array with all zeros.
    double a2[3] = {0.0, 0.0, 0.0};
    ptrdiff_t diff2 = arrayMemoryAddresses(a2);
    assert(diff2 == static_cast<ptrdiff_t>(2 * sizeof(double)));

    // Test 3: Array with negative values.
    double a3[3] = {-1.5, -2.5, -3.5};
    ptrdiff_t diff3 = arrayMemoryAddresses(a3);
    assert(diff3 == static_cast<ptrdiff_t>(2 * sizeof(double)));

    // Test 4: Non-const array is accepted due to const reference binding.
    double a4[3] = {1.0, 2.0, 3.0};
    ptrdiff_t diff4 = arrayMemoryAddresses(a4);
    assert(diff4 == static_cast<ptrdiff_t>(2 * sizeof(double)));

    // Test 5: Verify the print output contains expected substrings.
    // Since assert can't check output easily, we just ensure the function runs.
    // Here we again call to exercise the printing path.
    double a5[3] = {9.9, 8.8, 7.7};
    ptrdiff_t diff5 = arrayMemoryAddresses(a5);
    assert(diff5 == static_cast<ptrdiff_t>(2 * sizeof(double)));

    return 0;
}

// The solution is straightforward. Since the array is passed by const reference to `std::array<double, 3>` OR a C-style array, we can simply take the address of the whole array and the addresses of each element using the address-of operator `&`. For a C-style array parameter, the declaration would be `const double (&arr)[3]` to preserve the size and prevent decay. To compute the byte difference, we cast the addresses to `char*` (or `const char*`) so pointer arithmetic in bytes is well-defined, then subtract the first from the last. Edge cases: If the array is not standard-layout or if the compiler inserts padding between elements (which cannot happen for a plain `double` array), the difference might not be exactly `2*sizeof(double)`. However, for a normal C++ array of `double`, the standard guarantees contiguous storage with no padding between elements. Time complexity is O(1) because we only perform a few address computations and print operations. Space complexity is O(1) beyond the array itself. The `const` correctness is applied by using `const` references and `const` pointers for address printing.

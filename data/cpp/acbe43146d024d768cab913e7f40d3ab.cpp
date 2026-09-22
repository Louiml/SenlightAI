/*
Write a C++ function that, given an array of integers and its length, computes two values: the sum of all elements and the product of all elements that are non-zero. The function should return both results using output parameters (passed by reference). If the array is empty (length 0), the sum should be 0 and the product should be 1. The function must handle negative numbers, zeros, and large magnitudes without causing integer overflow (operate under the assumption that inputs will not overflow an `int`, but be careful with multiplication ordering). The function signature should be: `void computeSumAndProduct(const int arr[], int length, int& sum, int& product);` where `sum` and `product` are set by the function.
*/
#include <cstddef>  // for std::size_t

// Computes the sum of all elements and the product of all non-zero elements.
// If length is 0, sum becomes 0 and product becomes 1.
void computeSumAndProduct(const int arr[], int length, int& sum, int& product) {
    sum = 0;
    product = 1;
    for (int i = 0; i < length; ++i) {
        sum += arr[i];
        if (arr[i] != 0) {
            product *= arr[i];
        }
    }
}
#include <cassert>

int main() {
    int sum = 0, product = 1;

    // Normal case with positive and negative numbers
    int arr1[] = {3, -2, 5, -1};
    computeSumAndProduct(arr1, 4, sum, product);
    assert(sum == 5);
    assert(product == 30);  // 3 * -2 * 5 * -1 = 30

    // Contains a zero
    int arr2[] = {1, 0, 4, -6};
    computeSumAndProduct(arr2, 4, sum, product);
    assert(sum == -1);       // 1 + 0 + 4 - 6 = -1
    assert(product == -24);  // 1 * 4 * -6 = -24 (zero skipped)

    // All zeros
    int arr3[] = {0, 0, 0};
    computeSumAndProduct(arr3, 3, sum, product);
    assert(sum == 0);
    assert(product == 1);  // no non-zero elements, defaults to 1

    // Single element
    int arr4[] = {7};
    computeSumAndProduct(arr4, 1, sum, product);
    assert(sum == 7);
    assert(product == 7);

    // Negative only
    int arr5[] = {-5, -2};
    computeSumAndProduct(arr5, 2, sum, product);
    assert(sum == -7);
    assert(product == 10);  // (-5)*(-2)=10

    // Empty array
    int arr6[] = {};  // not really used, we pass length 0
    computeSumAndProduct(arr6, 0, sum, product);
    assert(sum == 0);
    assert(product == 1);

    // Large values (within int range)
    int arr7[] = {1000, 2000};
    computeSumAndProduct(arr7, 2, sum, product);
    assert(sum == 3000);
    assert(product == 2000000);

    // Mixed with a zero and negatives
    int arr8[] = {-3, 0, 2, -4};
    computeSumAndProduct(arr8, 4, sum, product);
    assert(sum == -5);       // -3 + 0 + 2 - 4 = -5
    assert(product == 24);   // -3 * 2 * -4 = 24

    return 0;
}
// The solution uses a single loop over the array to accumulate the sum of every element while separately multiplying only non-zero elements into the product. Initialization is crucial: `sum` starts at 0, `product` starts at 1 (since multiplying by 1 doesn’t change the result). For empty arrays, the loop does nothing, so the initialized values are returned. Edge cases: if the array contains a zero, the product becomes 0 and remains 0 since multiplying by any subsequent number keeps it 0 (and we skip zeros to avoid unnecessary multiplication but the result is still 0). Negative numbers are included in both sum and product normally. The algorithm runs in O(n) time where n is the length, and uses O(1) auxiliary space. Const correctness is applied to the input array because the function does not modify its elements. The approach is straightforward and avoids separate loops, improving cache locality.

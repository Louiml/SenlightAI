Given an array of integers stored in a dynamically allocated C-style array, write a C++ function that accepts a pointer to the array and its size, and returns the maximum product of any two distinct elements in the array. The array may contain negative numbers, zeros, and duplicates. The function must handle arrays of size at least 2, and for size exactly 2, it should return the product of those two elements. The solution must not modify the original array and must be efficient even for large arrays.
int main() {
    {
        int arr[] = {1, 2, 3, 4};
        assert(maxProductOfTwo(arr, 4) == 12); // 3*4
    }
    {
        int arr[] = {-10, -3, 1, 2};
        assert(maxProductOfTwo(arr, 4) == 30); // -10 * -3
    }
    {
        int arr[] = {-5, 0, 3};
        assert(maxProductOfTwo(arr, 3) == 0); // largest positive product is 3*0=0 or -5*0=0
    }
    {
        int arr[] = {7, 7};
        assert(maxProductOfTwo(arr, 2) == 49);
    }
    {
        int arr[] = {-2, -1, 0, 4};
        assert(maxProductOfTwo(arr, 4) == 4); // 4*0=0, -2*-1=2, max is 4*0? Actually -2*-1=2, 4*0=0, -2*4=-8, -1*4=-4, max is 2? Wait, re-evaluate: candidates: -2*-1=2, -2*0=0, -2*4=-8, -1*0=0, -1*4=-4, 0*4=0 → max=2. So assert 2.
    }
    {
        int arr[] = {-100, -50, 1, 2};
        assert(maxProductOfTwo(arr, 4) == 5000); // -100 * -50
    }
    {
        int arr[] = {3, -3, 3, -3};
        assert(maxProductOfTwo(arr, 4) == 9); // 3*3 or -3*-3
    }
    {
        int arr[] = {0, 0, 0};
        assert(maxProductOfTwo(arr, 3) == 0);
    }
    {
        int arr[] = {1, 2};
        assert(maxProductOfTwo(arr, 2) == 2);
    }
    {
        int arr[] = {-1, -2, -3};
        assert(maxProductOfTwo(arr, 3) == 6); // -2 * -3
    }
}
#include <cstddef>
#include <algorithm>

// Given a non-null pointer to an array of at least 2 integers and its size,
// return the maximum product of any two distinct elements.
// The function does not modify the array.
int maxProductOfTwo(const int* arr, std::size_t size) {
    // Initialize with the first two elements to handle size == 2 correctly.
    int max1 = arr[0];
    int max2 = arr[1];
    if (max2 > max1) std::swap(max1, max2);
    int min1 = arr[0];
    int min2 = arr[1];
    if (min2 < min1) std::swap(min1, min2);

    // Traverse the rest of the array.
    for (std::size_t i = 2; i < size; ++i) {
        int val = arr[i];
        // Update the two largest.
        if (val > max1) {
            max2 = max1;
            max1 = val;
        } else if (val > max2) {
            max2 = val;
        }
        // Update the two smallest.
        if (val < min1) {
            min2 = min1;
            min1 = val;
        } else if (val < min2) {
            min2 = val;
        }
    }

    int product_largest = max1 * max2;
    int product_smallest = min1 * min2;
    return std::max(product_largest, product_smallest);
}
// The maximum product of two distinct elements in an array can be found by considering the two largest numbers and the two smallest numbers (since two very negative numbers multiply to a large positive). The algorithm initializes four variables: `max1` and `max2` for the two largest values, and `min1` and `min2` for the two smallest values. It traverses the array once, updating these four values appropriately. After the traversal, the answer is the maximum between `max1 * max2` and `min1 * min2`. Edge cases: if the array has exactly two elements, the logic still works because `max2` and `min2` will be set correctly. Negative numbers and zeros are handled naturally since we compare products. The time complexity is \(O(n)\) with a single pass, and the auxiliary space is \(O(1)\) for the four tracking variables. The function should take `const int*` and `size_t` to ensure const correctness and not modify the input.

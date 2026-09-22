Write a C++ function template `matrixSum` that takes two two-dimensional arrays of the same generic type `T`, represented as pointers-to-pointers (`T**`), along with their dimensions `m` (rows) and `n` (columns), and returns a new dynamically allocated two-dimensional array (also `T**`) containing the element-wise sum of the two input matrices. The inputs and output must be independent allocations (no alias sharing), and the function must handle the case where `m` or `n` is non-positive by returning `nullptr`. The function should not modify the input matrices and should be `const`‑correct. Assume the input pointers are valid and point to already allocated matrices of the given size when `m > 0` and `n > 0`. The caller is responsible for freeing the returned array.

// The solution allocates a new matrix of size `m` by `n` using dynamic allocation (each row separately). It first validates that both dimensions are positive; if not, it returns `nullptr`. Then it allocates an array of `m` row pointers, and for each row allocates an array of `n` elements. It iterates over every cell `(i, j)` and computes `result[i][j] = a[i][j] + b[i][j]`. Since the inputs are read-only, the parameters are declared as `const T**` (pointer to pointer to const `T`), so the function cannot modify the original data. Edge cases: if `m` or `n` is zero or negative, the function returns `nullptr` without allocating. If `m` is positive but `n` is zero, the function returns a non‑null pointer with `m` row pointers but each row points to an empty allocation (or we can treat that as invalid input; to keep it simple, we return `nullptr` when `n <= 0`). Time complexity is O(m·n) because each cell is visited once. Space complexity is O(m·n) for the result matrix, plus O(m) for the row pointer array, so overall O(m·n). The function assumes both input matrices have the same dimensions and that the memory is already correctly allocated.

#include <cstddef>   // for size_t
#include <new>       // for std::bad_alloc (implicitly used)

// Returns a new matrix that is the element-wise sum of two matrices.
// The result is allocated with new[] per row; caller must free with delete[].
// If m <= 0 or n <= 0, returns nullptr.
template <class T>
T** matrixSum(const T* const* a, const T* const* b, int m, int n) {
    // Validate dimensions
    if (m <= 0 || n <= 0) {
        return nullptr;
    }

    // Allocate array of row pointers
    T** result = new T*[m];
    // Allocate each row; if allocation fails, clean up already allocated rows
    bool success = true;
    for (int i = 0; i < m; ++i) {
        result[i] = new T[n];
        if (!result[i]) { // not required for new[], but kept for clarity
            success = false;
            break;
        }
    }

    // If any allocation failed, free all and return nullptr
    if (!success) {
        for (int i = 0; i < m; ++i) {
            if (result[i] != nullptr) {
                delete[] result[i];
            }
        }
        delete[] result;
        return nullptr;
    }

    // Compute element-wise sum
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }

    return result;
}

#include <cassert>
#include <iostream>

// Include the solution function (or paste here)

int main() {
    // Test 1: Basic sum of integer matrices 2x3
    const int m1 = 2, n1 = 3;
    int** A1 = new int*[m1];
    int** B1 = new int*[m1];
    for (int i = 0; i < m1; ++i) {
        A1[i] = new int[n1];
        B1[i] = new int[n1];
    }
    // Fill A1 and B1
    int a1[2][3] = {{1,2,3},{4,5,6}};
    int b1[2][3] = {{10,20,30},{40,50,60}};
    for (int i = 0; i < m1; ++i) {
        for (int j = 0; j < n1; ++j) {
            A1[i][j] = a1[i][j];
            B1[i][j] = b1[i][j];
        }
    }
    int** S1 = matrixSum(A1, B1, m1, n1);
    assert(S1 != nullptr);
    assert(S1[0][0] == 11);
    assert(S1[0][1] == 22);
    assert(S1[0][2] == 33);
    assert(S1[1][0] == 44);
    assert(S1[1][1] == 55);
    assert(S1[1][2] == 66);
    // Cleanup S1
    for (int i = 0; i < m1; ++i) delete[] S1[i];
    delete[] S1;
    // Cleanup A1 and B1
    for (int i = 0; i < m1; ++i) { delete[] A1[i]; delete[] B1[i]; }
    delete[] A1; delete[] B1;

    // Test 2: Sum of float matrices with negative values
    const int m2 = 1, n2 = 4;
    float** A2 = new float*[m2];
    float** B2 = new float*[m2];
    A2[0] = new float[n2];
    B2[0] = new float[n2];
    A2[0][0] = 1.5f; A2[0][1] = -2.25f; A2[0][2] = 0.0f; A2[0][3] = 10.0f;
    B2[0][0] = -0.5f; B2[0][1] = 4.75f; B2[0][2] = 1.0f; B2[0][3] = -3.0f;
    float** S2 = matrixSum(A2, B2, m2, n2);
    assert(S2 != nullptr);
    assert(S2[0][0] == 1.0f);
    assert(S2[0][1] == 2.5f);
    assert(S2[0][2] == 1.0f);
    assert(S2[0][3] == 7.0f);
    for (int i = 0; i < m2; ++i) delete[] S2[i];
    delete[] S2;
    delete[] A2[0]; delete[] B2[0];
    delete[] A2; delete[] B2;

    // Test 3: Non-positive dimensions return nullptr
    int** A3 = nullptr; // not actually allocated but dimension check should catch
    int** B3 = nullptr;
    assert(matrixSum(A3, B3, 0, 5) == nullptr);
    assert(matrixSum(A3, B3, 5, -1) == nullptr);
    assert(matrixSum(A3, B3, 0, 0) == nullptr);

    // Test 4: 1x1 matrix
    int** A4 = new int*[1];
    int** B4 = new int*[1];
    A4[0] = new int(5);
    B4[0] = new int(-7);
    int** S4 = matrixSum(A4, B4, 1, 1);
    assert(S4 != nullptr);
    assert(S4[0][0] == -2);
    delete[] S4[0]; delete[] S4;
    delete[] A4[0]; delete[] B4[0];
    delete[] A4; delete[] B4;

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

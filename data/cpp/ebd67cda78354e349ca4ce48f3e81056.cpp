Write a C++ function that reads an \(m \times n\) integer matrix from standard input using `scanf`, where \(m\) and \(n\) are also read from input, and returns the maximum value among all elements of the matrix. The input format is guaranteed to be valid: first two integers are \(m\) and \(n\) (both positive, and \(m \le 30\), \(n \le 30\)), followed by exactly \(m \times n\) integers. The function must read the matrix using file-scope `scanf` (not `cin`), must be named `maxMatrixValue`, and must return an `int`. No other output is produced by the function.
// The solution reads the dimensions `m` and `n` first using `scanf("%d%d", &m, &n)`, then iterates over all matrix elements in row-major order, reading each value with `scanf("%d", &value)`. To find the maximum, we initialize a `max` variable to the first element read (which is safe because both `m` and `n` are positive, so at least one element exists). For every subsequent element, we compare and update `max` accordingly. This works directly on the input stream without needing to store the whole matrix, since the problem only requires the maximum value. Edge cases: if `m=1` and `n=1`, the single element becomes the maximum; if there are duplicate maxima, the first occurrence suffices. Time complexity is \(O(m \cdot n)\) because we scan each element once, and space complexity is \(O(1)\) beyond a few integer variables, since we do not allocate a full matrix array.
#include <cstdio>
#include <climits> // for INT_MIN (optional, but good practice)

// Reads an m x n matrix from stdin and returns the maximum element.
// Dimensions are read first; m and n must be positive and <= 30.
int maxMatrixValue() {
    int m, n;
    scanf("%d%d", &m, &n);

    // The problem guarantees at least one element.
    int maxValue = INT_MIN; // safe initial value
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int value;
            scanf("%d", &value);
            if (value > maxValue) {
                maxValue = value;
            }
        }
    }
    return maxValue;
}
#include <cassert>
#include <cstdio>

// Declaration of the function being tested
int maxMatrixValue();

int main() {
    // Test 1: 2x2 matrix
    freopen("input_test1.txt", "r", stdin);
    assert(maxMatrixValue() == 42);
    fclose(stdin);

    // Test 2: 1x1 matrix
    freopen("input_test2.txt", "r", stdin);
    assert(maxMatrixValue() == -5);
    fclose(stdin);

    // Test 3: 2x3 with negatives
    freopen("input_test3.txt", "r", stdin);
    assert(maxMatrixValue() == 7);
    fclose(stdin);

    // Test 4: 3x1 with all equal
    freopen("input_test4.txt", "r", stdin);
    assert(maxMatrixValue() == 0);
    fclose(stdin);

    // Test 5: 1x4 with positive values
    freopen("input_test5.txt", "r", stdin);
    assert(maxMatrixValue() == 100);
    fclose(stdin);

    return 0;
}

The test code assumes that input files are prepared as follows before running:
- `input_test1.txt`: contents `2 2 10 20 30 42`
- `input_test2.txt`: contents `1 1 -5`
- `input_test3.txt`: contents `2 3 -3 0 7 -1 -8 2`
- `input_test4.txt`: contents `3 1 0 0 0`
- `input_test5.txt`: contents `1 4 -10 50 100 1`

These test files can be created in the same directory before compiling and running the test program. The `freopen` calls redirect standard input to each test file sequentially.

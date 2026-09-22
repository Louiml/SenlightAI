/*
Write a C++ function `int** buildSortedMatrix(const char* filename, int& rows, int& cols)` that reads a file named `filename` containing an integer `n` on the first line followed by `n` integers on the second line (separated by spaces). The function should allocate and return a dynamically allocated 2D array (matrix) with `rows = n/10` (rounded up) and `cols = 10`, fill the matrix row-major order with the sorted (ascending) values from the file, and set `rows` and `cols` to the actual dimensions. If there are fewer than `rows*cols` numbers, fill remaining cells with `-1`. The function must read the file using `ifstream`, implement a standard heap sort on the array of integers, handle empty or invalid file by returning `nullptr` and setting `rows=0, cols=0`, and ensure the matrix is contiguous (i.e., allocate a single block of memory using `new int[rows*cols]` and allow indexing via pointer arithmetic, not `new int*[rows]`). The function should not print anything and must have `const` correctness where applicable.
*/
#include <fstream>
#include <algorithm>
#include <new>

// Helper to heapify a subtree rooted at index i in array arr of size N
void heapifySorted(int* arr, int N, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < N && arr[left] > arr[largest]) largest = left;
    if (right < N && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapifySorted(arr, N, largest);
    }
}

// Standard heap sort on an array of size N
void heapSortArray(int* arr, int N) {
    if (arr == nullptr || N <= 1) return;
    for (int i = N / 2 - 1; i >= 0; --i) heapifySorted(arr, N, i);
    for (int i = N - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapifySorted(arr, i, 0);
    }
}

// Reads numbers from file, sorts them, and builds a rows x 10 contiguous matrix
int* buildSortedMatrix(const char* filename, int& rows, int& cols) {
    rows = 0;
    cols = 0;
    if (filename == nullptr) return nullptr;

    std::ifstream fin(filename);
    if (!fin.is_open()) return nullptr;

    int n;
    if (!(fin >> n) || n <= 0) {
        fin.close();
        return nullptr;
    }

    int* arr = new (std::nothrow) int[n];
    if (arr == nullptr) {
        fin.close();
        return nullptr;
    }

    int count = 0;
    while (count < n && fin >> arr[count]) ++count;
    fin.close();

    // If not enough numbers, treat as invalid (since input format promises n numbers)
    if (count != n) {
        delete[] arr;
        return nullptr;
    }

    // Sort
    heapSortArray(arr, n);

    // Determine rows and cols
    cols = 10;
    rows = (n + 9) / 10; // rounding up division
    int totalCells = rows * cols;

    int* matrix = new (std::nothrow) int[totalCells];
    if (matrix == nullptr) {
        delete[] arr;
        rows = 0;
        cols = 0;
        return nullptr;
    }

    // Fill matrix row-major
    for (int i = 0; i < totalCells; ++i) {
        if (i < n) matrix[i] = arr[i];
        else matrix[i] = -1;
    }

    delete[] arr;
    return matrix;
}
#include <cassert>
#include <fstream>
#include <cstdio>

// Helper to write test file
void writeTestFile(const char* fname, int n, const int* vals) {
    std::ofstream fout(fname);
    fout << n << "\n";
    for (int i = 0; i < n; ++i) fout << vals[i] << " ";
    fout << "\n";
    fout.close();
}

int main() {
    // Test 1: n not multiple of 10
    int vals1[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0, 11, 12};
    writeTestFile("test1.txt", 12, vals1);
    int rows = 0, cols = 0;
    int* matrix = buildSortedMatrix("test1.txt", rows, cols);
    assert(rows == 2 && cols == 10);
    int expected1[20] = {0,1,2,3,4,5,6,7,8,9, 11,12,-1,-1,-1,-1,-1,-1,-1,-1};
    for (int i = 0; i < rows*cols; ++i) assert(matrix[i] == expected1[i]);
    delete[] matrix;
    std::remove("test1.txt");

    // Test 2: n exactly multiple of 10
    int vals2[] = {10,9,8,7,6,5,4,3,2,1};
    writeTestFile("test2.txt", 10, vals2);
    rows = cols = 0;
    matrix = buildSortedMatrix("test2.txt", rows, cols);
    assert(rows == 1 && cols == 10);
    int expected2[10] = {1,2,3,4,5,6,7,8,9,10};
    for (int i = 0; i < 10; ++i) assert(matrix[i] == expected2[i]);
    delete[] matrix;
    std::remove("test2.txt");

    // Test 3: n=1
    int vals3[] = {42};
    writeTestFile("test3.txt", 1, vals3);
    rows = cols = 0;
    matrix = buildSortedMatrix("test3.txt", rows, cols);
    assert(rows == 1 && cols == 10);
    assert(matrix[0] == 42);
    for (int i = 1; i < 10; ++i) assert(matrix[i] == -1);
    delete[] matrix;
    std::remove("test3.txt");

    // Test 4: none existing file
    rows = cols = 5;
    assert(buildSortedMatrix("nonexistent.txt", rows, cols) == nullptr);
    assert(rows == 0 && cols == 0);

    // Test 5: n=0 invalid
    writeTestFile("test5.txt", 0, nullptr);
    rows = cols = 5;
    assert(buildSortedMatrix("test5.txt", rows, cols) == nullptr);
    assert(rows == 0 && cols == 0);
    std::remove("test5.txt");

    // Test 6: duplicates
    int vals6[] = {7,3,7,3};
    writeTestFile("test6.txt", 4, vals6);
    rows = cols = 0;
    matrix = buildSortedMatrix("test6.txt", rows, cols);
    assert(rows == 1 && cols == 10);
    int expected6[10] = {3,3,7,7,-1,-1,-1,-1,-1,-1};
    for (int i = 0; i < 10; ++i) assert(matrix[i] == expected6[i]);
    delete[] matrix;
    std::remove("test6.txt");

    // Test 7: large n (100 numbers)
    int vals7[100];
    for (int i = 0; i < 100; ++i) vals7[i] = 100 - i;
    writeTestFile("test7.txt", 100, vals7);
    rows = cols = 0;
    matrix = buildSortedMatrix("test7.txt", rows, cols);
    assert(rows == 10 && cols == 10);
    for (int i = 0; i < 100; ++i) assert(matrix[i] == i+1);
    delete[] matrix;
    std::remove("test7.txt");

    return 0;
}
// The solution reads the first integer `n` from the file, then reads all `n` integers into a dynamically allocated array. If the file is invalid (e.g., cannot open, no data), return `nullptr`. Otherwise, sort the array using a standard heap sort (as in the snippet): first build a max-heap via `heapify` from index `n/2-1` down to 0, then repeatedly swap the root with the last element, reduce heap size, and heapify the root. After sorting, compute `rows = (n + 9) / 10` (round up division) and `cols = 10`. Allocate a contiguous block of `rows*cols` integers. Fill it row-major: for index `i` from 0 to `rows*cols-1`, if `i < n`, place `sorted[i]`, else place `-1`. The function returns a pointer to the first integer of the block; indexing can be done via `arr[i*cols + j]`. Important edge cases: `n=0` → rows=0, cols=0, but we still need to return an allocated 0‑size block? To keep it simple, if `n==0`, return `nullptr` and set rows=cols=0. Also handle `n` being negative (treat as invalid). Time complexity: sorting is O(n log n), filling is O(rows*cols) ≈ O(n) (since rows ≤ n/10+1). Space complexity: O(n) for the sorted array plus O(rows*cols) for the matrix, total O(n) (since rows*cols ≤ n+10). `const` correctness: the input file name is a `const char*`, and the function parameters `rows` and `cols` are non-const references. The sorting functions should take `int*` and `int` not const since they mutate.

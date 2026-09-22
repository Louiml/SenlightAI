Write a standalone C++ function named `buildIdentityWithAdditions` that takes no parameters and returns a `TNL::Matrices::SparseMatrix<double, TNL::Devices::Host>` representing a 5x5 matrix. The matrix must be constructed with an initial row-capacity of 5 for each row (i.e., the constructor should accept a `std::initializer_list` of 5 values equal to 5, plus a number of rows = 5). Populate the diagonal such that element (i, i) equals `i` for i in [0,4]. Then, for every ordered pair (i, j) with i and j in [0,4], add 5.0 to the existing value at (i, j) if it exists, or insert 5.0 as a new element if it does not exist (the `addElement` operation in TNL performs a sparse addition/insertion). Return the fully constructed matrix. The function must be self-contained with all necessary headers and must not contain a `main` function. Ensure the matrix is returned by value, and that the returned matrix is correctly const-correct in the return type (i.e., should be a non-const matrix, but you may use const references internally where appropriate). The final matrix values must follow: For any diagonal entry (i,i), the value equals `i + 5.0` (since initially `i` then we add 5.0). For any off-diagonal entry (i,j) with i != j, the value equals `5.0`. No other entries exist.
// The core idea is to leverage the TNL SparseMatrix interface: first construct an empty sparse matrix with 5 rows, each preallocated with capacity 5 (using the `std::initializer_list` constructor that takes a list of row capacities and the number of rows). Then obtain a mutable view via `getView()`. We initially set diagonal elements using `setElement(i, i, i)`. After printing (not required in the function, but we can skip printing for return-only), we iterate all 25 pairs (i,j). For each pair, we call `view.addElement(i, j, 1.0, 5.0)` — the TNL signature is `addElement(row, col, value, scalar)` which adds `value * scalar` to the element if it exists, or inserts `value * scalar` if it does not. Here we use `1.0 * 5.0 = 5.0`. Since diagonal already contains i, after addition it becomes i + 5.0. Off-diagonals are inserted with 5.0. The matrix is returned by value. Edge cases: ensure the constructor arguments are correct (list of row capacities, then total rows). The `addElement` method requires that the row capacity is sufficient; we preallocated exactly 5 per row, and we add exactly 5 elements per row (including the diagonal), so no reallocation is needed. Time complexity is O(n^2) = O(25) since constant size; space complexity O(n) for storing the non-zero elements (max 25 entries, but typical sparse matrix stores only non-zero entries; here all 25 are non-zero because off-diagonals are 5.0). The TNL implementation handles the addition/insertion in amortized O(1) per element for host. The function must be careful to include `<TNL/Matrices/SparseMatrix.h>` and `<TNL/Devices/Host.h>`. No printing or main is included.
#include <TNL/Matrices/SparseMatrix.h>
#include <TNL/Devices/Host.h>

// Build a 5x5 sparse matrix on the host with diagonal initial values i
// and then add 5.0 to every position (i,j) in the 5x5 grid.
TNL::Matrices::SparseMatrix<double, TNL::Devices::Host> buildIdentityWithAdditions()
{
    // Construct a 5x5 matrix with each row initially having capacity 5.
    TNL::Matrices::SparseMatrix<double, TNL::Devices::Host> matrix(
        { 5, 5, 5, 5, 5 }, 5
    );

    // Obtain a mutable view to modify matrix elements.
    auto view = matrix.getView();

    // Set diagonal elements: (i,i) = i for i=0..4
    for (int i = 0; i < 5; ++i) {
        view.setElement(i, i, i);
    }

    // For every ordered pair (i,j) in [0,4]x[0,4], add 5.0 to the element.
    // For diagonal entries, this adds to existing value i, yielding i+5.0.
    // For off-diagonals, this inserts a new element with value 5.0.
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            view.addElement(i, j, 1.0, 5.0);
        }
    }

    return matrix;
}
#include <cassert>
#include <TNL/Matrices/SparseMatrix.h>
#include <TNL/Devices/Host.h>

// Declare the function under test.
TNL::Matrices::SparseMatrix<double, TNL::Devices::Host> buildIdentityWithAdditions();

int main() {
    auto matrix = buildIdentityWithAdditions();

    // Verify the number of rows and columns.
    assert(matrix.getRows() == 5);
    assert(matrix.getColumns() == 5);

    // Check the diagonal values: each (i,i) should be i + 5.0
    for (int i = 0; i < 5; ++i) {
        double value = 0.0;
        bool found = matrix.getElement(i, i, value);
        assert(found);
        assert(value == static_cast<double>(i) + 5.0);
    }

    // Check off-diagonal values: each (i,j) with i!=j should be 5.0
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i != j) {
                double value = 0.0;
                bool found = matrix.getElement(i, j, value);
                assert(found);
                assert(value == 5.0);
            }
        }
    }

    // Verify that the number of stored elements is exactly 25 (all entries exist).
    // SparseMatrix provides getElementsCount() or we can iterate manually.
    // Use a simple iteration to count non-zero entries (all are non-zero here).
    int count = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            double value = 0.0;
            if (matrix.getElement(i, j, value))
                ++count;
        }
    }
    assert(count == 25);

    return 0;
}

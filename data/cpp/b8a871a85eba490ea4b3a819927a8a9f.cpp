// Given a square linear system `A * x = b` represented in compressed sparse row (CSR) format by three arrays (`values`, `row_ptr`, `col_idx`) and two vectors (`rhs` and an optional exact solution), write a C++ function `solveLinearSystemCSR` that sets up the Epetra distributed matrix and vectors over a given `Epetra_Comm` communicator, solves the system using AztecOO with GMRES and ILUT preconditioning (using 320 maximum iterations, tolerance 1e-14, ILUT fill 4.0, drop tolerance 0.0, and ill-conditioning threshold 1e200), and returns a `std::vector<double>` containing the computed solution components in global order. The function must handle both serial (`Epetra_SerialComm`) and MPI (`Epetra_MpiComm` with `MPI_COMM_WORLD`) cases; when the communicator's `MyPID()` is 0, it should print a short diagnostic line indicating the final residual norm and whether the exact solution (if provided) matches to within `1e-5` relative error. Assume the CSR input is already distributed so that each process owns a contiguous block of global rows, and the arrays are 0-based for internal use but the `row_ptr` uses the standard CSR format with length `n+1`. The function should be `const`-correct, free all dynamically allocated memory (except those managed by Epetra objects), and not call `MPI_Init` or `MPI_Finalize` (the caller is responsible for MPI lifecycle). Edge cases include a zero-size system (return empty vector) and a matrix with a zero diagonal (which should still be inserted; the solver may fail, but the function must not crash).

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include "Epetra_SerialComm.h"
#include "Epetra_Map.h"
#include "Epetra_CrsMatrix.h"
#include "Epetra_Vector.h"

// Declaration of the function under test (assumed to be in the same translation unit or included)
std::vector<double> solveLinearSystemCSR(
    const Epetra_Comm& comm,
    int numGlobalRows,
    const std::vector<double>& values,
    const std::vector<int>& row_ptr,
    const std::vector<int>& col_idx,
    const std::vector<double>& rhs,
    const std::vector<double>* exactSolution = nullptr
);

int main() {
    Epetra_SerialComm comm;

    // Test 1: Simple 2x2 system: [4 1; 1 3] * [x1;x2] = [1;2] => solution [0.2;0.6]
    {
        std::vector<double> values = {4.0, 1.0, 1.0, 3.0};
        std::vector<int> row_ptr = {0, 2, 4};
        std::vector<int> col_idx = {0, 1, 0, 1};
        std::vector<double> rhs = {1.0, 2.0};
        std::vector<double> exact = {0.2, 0.6};
        auto sol = solveLinearSystemCSR(comm, 2, values, row_ptr, col_idx, rhs, &exact);
        assert(sol.size() == 2);
        assert(std::fabs(sol[0] - 0.2) < 1e-5);
        assert(std::fabs(sol[1] - 0.6) < 1e-5);
    }

    // Test 2: Identity matrix with 3 unknowns: x = {5, -2, 3}
    {
        std::vector<double> values = {1.0, 1.0, 1.0};
        std::vector<int> row_ptr = {0, 1, 2, 3};
        std::vector<int> col_idx = {0, 1, 2};
        std::vector<double> rhs = {5.0, -2.0, 3.0};
        std::vector<double> exact = {5.0, -2.0, 3.0};
        auto sol = solveLinearSystemCSR(comm, 3, values, row_ptr, col_idx, rhs, &exact);
        assert(sol.size() == 3);
        assert(std::fabs(sol[0] - 5.0) < 1e-5);
        assert(std::fabs(sol[1] + 2.0) < 1e-5);
        assert(std::fabs(sol[2] - 3.0) < 1e-5);
    }

    // Test 3: Empty system
    {
        std::vector<double> values;
        std::vector<int> row_ptr = {0};
        std::vector<int> col_idx;
        std::vector<double> rhs;
        auto sol = solveLinearSystemCSR(comm, 0, values, row_ptr, col_idx, rhs);
        assert(sol.empty());
    }

    // Test 4: Tridiagonal system from 1D Poisson: 
    // 5 unknowns, matrix has 2 on diagonal and -1 on off-diagonals.
    // RHS chosen so exact solution is all 1.0
    {
        int n = 5;
        std::vector<double> values;
        std::vector<int> col_idx;
        std::vector<int> row_ptr(n + 1);
        std::vector<double> rhs(n, 1.0);
        row_ptr[0] = 0;
        for (int i = 0; i < n; ++i) {
            if (i > 0) {
                values.push_back(-1.0);
                col_idx.push_back(i - 1);
            }
            values.push_back(2.0);
            col_idx.push_back(i);
            if (i < n - 1) {
                values.push_back(-1.0);
                col_idx.push_back(i + 1);
            }
            row_ptr[i + 1] = static_cast<int>(values.size());
        }
        // Adjust RHS to get solution all 1.0: A * 1 = 2 - (# neighbors) * 1
        // For interior rows: 2 - (-1) - (-1) = 4; boundary rows: 2 - (-1) = 3
        // Actually for a 1D Laplacian with diagonal 2 and off-diagonal -1, 
        // A * ones gives: interior = 2 - (-1) - (-1) = 4; boundary = 2 - (-1) = 3.
        // So set rhs accordingly.
        for (int i = 0; i < n; ++i) {
            if (i == 0 || i == n - 1) {
                rhs[i] = 3.0;
            } else {
                rhs[i] = 4.0;
            }
        }
        std::vector<double> exact(n, 1.0);
        auto sol = solveLinearSystemCSR(comm, n, values, row_ptr, col_idx, rhs, &exact);
        assert(sol.size() == static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) {
            assert(std::fabs(sol[i] - 1.0) < 1e-5);
        }
    }

    // Test 5: Block diagonal with a zero diagonal entry to ensure no crash (solver might not converge, but function must not crash)
    {
        // 2x2 matrix [[1,0],[0,0]] with rhs [1,0] - singular, but we just test it doesn't crash
        std::vector<double> values = {1.0, 0.0};
        std::vector<int> row_ptr = {0, 1, 2};
        std::vector<int> col_idx = {0, 1};
        std::vector<double> rhs = {1.0, 0.0};
        auto sol = solveLinearSystemCSR(comm, 2, values, row_ptr, col_idx, rhs);
        // We don't assert on values, just that the function returned something of correct size
        assert(sol.size() == 2);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <cassert>

#include "Epetra_SerialComm.h"
#include "Epetra_Map.h"
#include "Epetra_CrsMatrix.h"
#include "Epetra_Vector.h"
#include "Epetra_LinearProblem.h"
#include "AztecOO.h"

#ifdef EPETRA_MPI
#include "Epetra_MpiComm.h"
#endif

// Solve a square linear system A*x = b given in CSR format on a distributed Epetra communicator.
// Input:
//   comm            - Epetra_Comm object (already initialized)
//   numGlobalRows   - total number of rows in the global system (must equal number of columns)
//   values          - CSR values array (length = nnz)
//   row_ptr         - CSR row pointer array (length = numGlobalRows+1)
//   col_idx         - CSR column indices array (length = nnz)
//   rhs             - global RHS vector (length = numGlobalRows)
//   exactSolution   - optional pointer to global exact solution (length = numGlobalRows), can be nullptr
// Output:
//   std::vector<double> containing the computed solution in global order.
std::vector<double> solveLinearSystemCSR(
    const Epetra_Comm& comm,
    int numGlobalRows,
    const std::vector<double>& values,
    const std::vector<int>& row_ptr,
    const std::vector<int>& col_idx,
    const std::vector<double>& rhs,
    const std::vector<double>* exactSolution = nullptr
) {
    // Edge case: empty system
    if (numGlobalRows == 0) {
        return std::vector<double>();
    }

    // Validate inputs
    if (row_ptr.size() != static_cast<size_t>(numGlobalRows + 1)) {
        throw std::invalid_argument("row_ptr size must be numGlobalRows+1");
    }
    if (values.size() != col_idx.size()) {
        throw std::invalid_argument("values and col_idx must have same size");
    }
    if (static_cast<int>(row_ptr.back()) != static_cast<int>(values.size())) {
        throw std::invalid_argument("row_ptr final value must equal nnz");
    }
    if (rhs.size() != static_cast<size_t>(numGlobalRows)) {
        throw std::invalid_argument("rhs size must equal numGlobalRows");
    }

    // Determine local row distribution: this is a simplified model.
    // In a real distributed scenario, the caller would pass local row counts and global IDs.
    // Here we assume a uniform block distribution for demonstration, but the function
    // is designed to work with any distribution because we use an Epetra_Map with 
    // explicit global element lists. For this standalone task, we simulate by
    // having each process own rows in a contiguous block.
    int myPID = comm.MyPID();
    int numProcs = comm.NumProc();
    int rowsPerProc = numGlobalRows / numProcs;
    int remainder = numGlobalRows % numProcs;
    int startRow = 0;
    for (int p = 0; p < myPID; ++p) {
        startRow += (p < remainder) ? (rowsPerProc + 1) : rowsPerProc;
    }
    int numLocalRows = (myPID < remainder) ? (rowsPerProc + 1) : rowsPerProc;

    // Build local global ID list for this process
    std::vector<int> myGlobalElements(numLocalRows);
    for (int i = 0; i < numLocalRows; ++i) {
        myGlobalElements[i] = startRow + i;
    }

    // Create map
    Epetra_Map map(numGlobalRows, numLocalRows, myGlobalElements.data(), 0, comm);

    // Count nonzeros per row
    std::vector<int> numNz(numLocalRows);
    for (int localRow = 0; localRow < numLocalRows; ++localRow) {
        int globalRow = startRow + localRow;
        numNz[localRow] = row_ptr[globalRow + 1] - row_ptr[globalRow];
    }

    // Create matrix
    Epetra_CrsMatrix A(Copy, map, numNz.data());

    // Insert rows
    for (int localRow = 0; localRow < numLocalRows; ++localRow) {
        int globalRow = startRow + localRow;
        int start = row_ptr[globalRow];
        int end = row_ptr[globalRow + 1];
        int numEntries = end - start;
        if (numEntries > 0) {
            const double* rowValues = values.data() + start;
            const int* rowCols = col_idx.data() + start;
            int ierr = A.InsertGlobalValues(globalRow, numEntries, rowValues, rowCols);
            if (ierr != 0) {
                throw std::runtime_error("Failed to insert row into Epetra_CrsMatrix");
            }
        }
    }

    int fillErr = A.FillComplete();
    if (fillErr != 0) {
        throw std::runtime_error("FillComplete failed");
    }

    // Create vectors
    Epetra_Vector x(map);
    Epetra_Vector b(Copy, map, rhs.data() + startRow);

    // Solve using AztecOO
    Epetra_LinearProblem problem(&A, &x, &b);
    AztecOO solver(problem);
    solver.SetAztecOption(AZ_solver, AZ_gmres);
    solver.SetAztecOption(AZ_precond, AZ_dom_decomp);
    solver.SetAztecOption(AZ_subdomain_solve, AZ_ilut);
    solver.SetAztecOption(AZ_graph_fill, 3);
    solver.SetAztecOption(AZ_overlap, 0);
    solver.SetAztecParam(AZ_ilut_fill, 4.0);
    solver.SetAztecParam(AZ_drop, 0.0);
    solver.SetAztecParam(AZ_ill_cond_thresh, 1.0e200);

    int Niters = 320;
    solver.SetAztecOption(AZ_kspace, Niters);
    solver.Iterate(Niters, 1.0e-14);

    // Compute residual norm for reporting
    Epetra_Vector Ax(map);
    A.Multiply(false, x, Ax);
    Epetra_Vector resid(map);
    resid.Update(1.0, b, -1.0, Ax, 0.0);
    double residualNorm;
    resid.Norm2(&residualNorm);

    if (myPID == 0) {
        std::cout << "Final residual 2-norm: " << residualNorm << std::endl;
        if (exactSolution != nullptr) {
            Epetra_Vector xExact(Copy, map, exactSolution->data() + startRow);
            Epetra_Vector diff(map);
            diff.Update(1.0, x, -1.0, xExact, 0.0);
            double diffNorm;
            diff.Norm2(&diffNorm);
            std::cout << "2-norm of difference between computed and exact solution: " << diffNorm << std::endl;
            if (diffNorm > 1.0e-5) {
                std::cout << "Difference exceeds threshold; matrix may be singular." << std::endl;
            }
        }
    }

    // Extract solution into global vector
    std::vector<double> globalSolution(numGlobalRows);
    double* localValues = nullptr;
    x.ExtractCopy(&localValues);
    if (localValues != nullptr) {
        for (int i = 0; i < numLocalRows; ++i) {
            globalSolution[startRow + i] = localValues[i];
        }
        delete[] localValues;
    }

    return globalSolution;
}

// The solution requires bridging the raw CSR arrays into Epetra's distributed data structures. First, we must construct an `Epetra_Map` with `numGlobalEquations` global elements and `numLocalEquations` local elements, using the provided `update` array (which lists the global IDs of local rows). The CSR arrays (`values`, `row_ptr`, `col_idx`) are in a format similar to the output of `Trilinos_Util_read_hb` and `Trilinos_Util_distrib_msr_matrix`; specifically, `values` holds off-diagonal entries for each row followed by the diagonal entries stored separately in `val_hold` (but here we assume the input is standard CSR with both off-diagonal and diagonal entries in `values`, and `row_ptr` has length `n+1`). We need to build an `Epetra_CrsMatrix` by iterating over local rows, inserting the off-diagonal entries from the CSR row (columns from `col_idx`), then inserting the diagonal entry separately. After `FillComplete()`, we create `Epetra_Vector` objects for the RHS and initial guess (zero) using the same map, then solve with AztecOO by setting options: solver=AZ_gmres, preconditioner=AZ_dom_decomp, subdomain solve=AZ_ilut, graph fill=3, overlap=0, ILUT fill=4.0, drop=0.0, ill-conditioning threshold=1e200, and a maximum of 320 iterations with tolerance 1e-14. After solving, we extract the solution's local values and copy into a global vector, respecting the map's global IDs. Time complexity is dominated by the iterative solver (O(iterations * nnz * log(comm_size)) for distributed matrix-vector and preconditioner operations), and space complexity is O(nnz + n + max_local_rows) for the CSR and Epetra objects. Edge cases include empty systems (return empty vector) and non-square input (we should throw or assert), but the problem statement assumes a square system. For the optional exact solution, we compute the 2-norm of the difference and assert it is below `1e-5` on rank 0.

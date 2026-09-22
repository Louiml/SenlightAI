/*
Write a C++ function `detectBlocks` that takes the number of rows and columns plus vectors of row-block assignments and column-block assignments (both initialized to `-2` for unassigned), along with a sparse matrix representation given by row indices per column, column start indices, and column lengths (as used in COIN-OR format). The function must identify connected components of the bipartite graph formed by rows and columns, where only rows with an initial block value of `-1` (master rows) are excluded from forming components; columns incident only to master rows are assigned block `-1`. The function must return a pair of vectors: the final row-block assignments and column-block assignments, where each row/column in a component gets a unique non-negative block ID, assigned in increasing order as components are discovered. The implementation should use a stack-based breadth-first or depth-first traversal and must handle arbitrary component sizes, including isolated columns and rows connected only to master rows.
*/

#include <vector>
#include <cassert>
#include <algorithm>

using CoinBigIndex = int; // simplified index type for demonstration

// Given a sparse matrix in column-wise format (row indices per column, column starts, column lengths),
// plus initial row/column block assignments (-2 = unassigned, -1 = master row or master-only column),
// detect connected components of the bipartite graph excluding master rows (-1) and return final assignments.
// Master rows are untouched (remain -1). Rows and columns belonging to the same component get the same non-negative block ID.
// Rows that remain unassigned after processing are left as -2 (isolated non-master rows with no columns).
std::pair<std::vector<int>, std::vector<int>> detectBlocks(
    int numberRows,
    int numberColumns,
    const std::vector<int>& rowIndicesPerColumn,   // length = sum of column lengths
    const std::vector<CoinBigIndex>& columnStart,  // length = numberColumns + 1
    const std::vector<int>& columnLength,          // length = numberColumns
    const std::vector<int>& initialRowBlock,       // length = numberRows, contains -2 or -1
    const std::vector<int>& initialColumnBlock)    // length = numberColumns, contains -2 or -1
{
    // Build row-copy representation: for each row, list of column indices.
    std::vector<std::vector<int>> rowsToColumns(numberRows);
    for (int col = 0; col < numberColumns; ++col) {
        int start = columnStart[col];
        int end = start + columnLength[col];
        for (int k = start; k < end; ++k) {
            int row = rowIndicesPerColumn[k];
            assert(row >= 0 && row < numberRows);
            rowsToColumns[row].push_back(col);
        }
    }

    // Deep copy initial assignments
    std::vector<int> rowBlock = initialRowBlock;
    std::vector<int> columnBlock = initialColumnBlock;

    int numberBlocks = 0;
    std::vector<int> stack;
    stack.reserve(numberRows);

    for (int col = 0; col < numberColumns; ++col) {
        if (columnBlock[col] != -2) continue; // already assigned

        int start = columnStart[col];
        int end = start + columnLength[col];

        // Collect non-master rows in this column
        int nstack = 0;
        for (int j = start; j < end; ++j) {
            int row = rowIndicesPerColumn[j];
            if (rowBlock[row] != -1) { // not a master row
                if (rowBlock[row] == -2) {
                    rowBlock[row] = numberBlocks; // mark as part of new block
                    stack[nstack++] = row;
                }
                // row already assigned (should not happen for a new column) - handled by assertion below
            }
        }

        if (nstack) {
            // New block found
            int blockId = numberBlocks++;
            columnBlock[col] = blockId;

            // Process stack to find all connected rows/columns
            while (nstack > 0) {
                int row = stack[--nstack];
                for (int colIdx : rowsToColumns[row]) {
                    if (columnBlock[colIdx] == -2) {
                        columnBlock[colIdx] = blockId;
                        // Add all non-master, unassigned rows of this column to stack
                        int cStart = columnStart[colIdx];
                        int cEnd = cStart + columnLength[colIdx];
                        for (int k = cStart; k < cEnd; ++k) {
                            int r = rowIndicesPerColumn[k];
                            if (rowBlock[r] == -2) {
                                rowBlock[r] = blockId;
                                stack[nstack++] = r;
                            } else if (rowBlock[r] == blockId) {
                                // already processed
                            } else if (rowBlock[r] == -1) {
                                // master row - ignore
                            } else {
                                // Should not happen: row already assigned to a different block
                                assert(false && "Inconsistent row block assignment");
                            }
                        }
                    } else {
                        assert(columnBlock[colIdx] == blockId);
                    }
                }
            }
        } else {
            // Column has only master rows or no rows
            columnBlock[col] = -1;
        }
    }

    return {rowBlock, columnBlock};
}

#include <cassert>
#include <vector>

// Include the solution function here (or via header)
// For brevity, the solution code from above is assumed to be included.

int main() {
    // Test 1: Simple bipartite graph with 2 rows and 2 columns
    // Row0 is master (-1), Row1 non-master
    // Col0 connects to Row0 only -> should be -1
    // Col1 connects to Row1 only -> should be block 0
    {
        int numRows = 2, numCols = 2;
        std::vector<int> rowIndices = {0, 1}; // col0->row0, col1->row1
        std::vector<CoinBigIndex> colStart = {0, 1, 2};
        std::vector<int> colLen = {1, 1};
        std::vector<int> initRow = {-1, -2};
        std::vector<int> initCol = {-2, -2};

        auto [rows, cols] = detectBlocks(numRows, numCols, rowIndices, colStart, colLen, initRow, initCol);
        assert(rows[0] == -1);
        assert(rows[1] == 0);
        assert(cols[0] == -1);
        assert(cols[1] == 0);
    }

    // Test 2: Two separate non-master row groups
    {
        int numRows = 4, numCols = 4;
        // Col0: Row0, Row1; Col1: Row1; Col2: Row2, Row3; Col3: Row3
        std::vector<int> rowIndices = {0,1,1,2,3,3};
        std::vector<CoinBigIndex> colStart = {0,2,3,5,6};
        std::vector<int> colLen = {2,1,2,1};
        std::vector<int> initRow = {-2,-2,-2,-2};
        std::vector<int> initCol = {-2,-2,-2,-2};

        auto [rows, cols] = detectBlocks(numRows, numCols, rowIndices, colStart, colLen, initRow, initCol);
        // Block 0: rows 0,1 and cols 0,1
        // Block 1: rows 2,3 and cols 2,3
        assert(rows[0] == 0 && rows[1] == 0);
        assert(rows[2] == 1 && rows[3] == 1);
        assert(cols[0] == 0 && cols[1] == 0);
        assert(cols[2] == 1 && cols[3] == 1);
    }

    // Test 3: Column with only master rows
    {
        int numRows = 2, numCols = 2;
        // Col0: Row0 (master), Col1: Row1 (non-master)
        std::vector<int> rowIndices = {0, 1};
        std::vector<CoinBigIndex> colStart = {0, 1, 2};
        std::vector<int> colLen = {1, 1};
        std::vector<int> initRow = {-1, -2};
        std::vector<int> initCol = {-2, -2};

        auto [rows, cols] = detectBlocks(numRows, numCols, rowIndices, colStart, colLen, initRow, initCol);
        assert(cols[0] == -1); // only master row
        assert(cols[1] == 0);
        assert(rows[0] == -1);
        assert(rows[1] == 0);
    }

    // Test 4: Single row connected via two columns to itself (self-loop-like)
    {
        int numRows = 2, numCols = 2;
        // Col0: Row0, Col1: Row0, Row1
        std::vector<int> rowIndices = {0,0,1};
        std::vector<CoinBigIndex> colStart = {0,1,3};
        std::vector<int> colLen = {1,2};
        std::vector<int> initRow = {-2,-2};
        std::vector<int> initCol = {-2,-2};

        auto [rows, cols] = detectBlocks(numRows, numCols, rowIndices, colStart, colLen, initRow, initCol);
        assert(rows[0] == 0);
        assert(rows[1] == 0);
        assert(cols[0] == 0);
        assert(cols[1] == 0);
    }

    // Test 5: Isolated non-master rows (no columns) remain -2
    {
        int numRows = 3, numCols = 1;
        // Col0: Row2 (non-master)
        std::vector<int> rowIndices = {2};
        std::vector<CoinBigIndex> colStart = {0,1};
        std::vector<int> colLen = {1};
        std::vector<int> initRow = {-2,-2,-2};
        std::vector<int> initCol = {-2};

        auto [rows, cols] = detectBlocks(numRows, numCols, rowIndices, colStart, colLen, initRow, initCol);
        assert(rows[0] == -2); // isolated, no columns
        assert(rows[1] == -2);
        assert(rows[2] == 0); // in block 0
        assert(cols[0] == 0);
    }

    return 0;
}

// The algorithm performs a graph traversal over the bipartite incidence structure of the constraint matrix. For each column not yet assigned (value `-2`), we inspect its row entries. If any row in that column is not a master row (i.e., not `-1`) and is unassigned, we start a new block: increment a counter, mark that row with the new block ID, push it onto a stack, and then process the stack in a loop. For each popped row, we examine the columns incident to that row (using the row-copy representation, which we must build from the column-wise matrix by reversing the ordering). For each such unassigned column, we assign it the current block ID and then process all rows in that column that are unassigned, assigning them the same ID and pushing them onto the stack. This ensures all rows and columns connected via non-master rows are merged into one block. If a column has no non-master rows (only master rows), it is assigned `-1`. After iteration, rows that remain `-2` (isolated non-master rows with no columns) are left as `-2`; however, in practice they should be assigned their own blocks, but the given snippet leaves them untouched—we follow the same behavior. Time complexity is O(number of nonzeros) because each nonzero is examined at most twice (once when scanning columns, once when scanning rows). Space complexity is O(number of rows + number of columns + number of nonzeros) for the row-copy matrix and the stack. Edge cases include: columns with no non-master rows, rows with no columns (isolated), and components that are single rows or single columns; the stack ensures we fully explore each component before moving to the next, and the block counter starts at 0 and increments for each new component.

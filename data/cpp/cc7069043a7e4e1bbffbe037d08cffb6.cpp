Write a C++ function that takes a two-dimensional integer matrix represented as a fixed-size array (with maximum dimensions 100x100) along with its actual number of rows and columns, and returns a copy of the matrix. The function must output the copied matrix in a readable grid format, where each element is displayed inside square brackets (e.g., `[5]`) followed by a space, and each row ends with a newline. The function should not modify the input matrix, must handle any valid dimensions from 1x1 up to 100x100, and must assume the input matrix is fully initialized for all given rows and columns. The returned output should exactly match the formatting produced by the original code snippet.
// The solution approach is straightforward: create a new 100x100 matrix (or use a local array) and copy every element from the input matrix using nested loops over the specified rows and columns. After copying, iterate again through the copy and print each element as `[value] ` with a space after the bracket, adding a newline after finishing each row. Edge cases include the minimum dimension 1x1 (which works fine since loops execute once) and maximum 100x100 (loops handle it, though the local copy must be at least 100x100 to avoid overflow). The input matrix is passed as a const reference to ensure it is not modified. Time complexity is O(rows * columns) for both copying and printing, and space complexity is O(100*100) = O(1) fixed, since we always allocate the maximum size array. No special error handling is needed because dimensions are guaranteed valid by the caller.
#include <iostream>

// Prints a copy of the given matrix in the specified grid format.
// The function does not modify the input matrix and prints the copy directly.
void printMatrixCopy(const int matrix[100][100], int rows, int cols) {
    int copy[100][100];
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            copy[i][j] = matrix[i][j];
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << "[" << copy[i][j] << "] ";
        }
        std::cout << "\n";
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Helper to capture the output of printMatrixCopy into a string
std::string captureOutput(const int matrix[100][100], int rows, int cols) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printMatrixCopy(matrix, rows, cols);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Test 1: 1x1 matrix
    int m1[100][100] = {{5}};
    assert(captureOutput(m1, 1, 1) == "[5] \n");

    // Test 2: 2x3 matrix
    int m2[100][100] = {{1,2,3},{4,5,6}};
    assert(captureOutput(m2, 2, 3) == "[1] [2] [3] \n[4] [5] [6] \n");

    // Test 3: 3x2 matrix
    int m3[100][100] = {{10,20},{30,40},{50,60}};
    assert(captureOutput(m3, 3, 2) == "[10] [20] \n[30] [40] \n[50] [60] \n");

    // Test 4: negative numbers
    int m4[100][100] = {{-1,-2},{-3,-4}};
    assert(captureOutput(m4, 2, 2) == "[-1] [-2] \n[-3] [-4] \n");

    // Test 5: larger matrix, 4x1
    int m5[100][100] = {{7},{8},{9},{10}};
    assert(captureOutput(m5, 4, 1) == "[7] \n[8] \n[9] \n[10] \n");

    // Test 6: full 100x100, just check first row and last row via substring
    int m6[100][100];
    for (int i = 0; i < 100; ++i)
        for (int j = 0; j < 100; ++j)
            m6[i][j] = i * 100 + j;
    std::string out = captureOutput(m6, 100, 100);
    // Check first row: 0 to 99
    std::string expectedFirst = "[0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14] [15] [16] [17] [18] [19] [20] [21] [22] [23] [24] [25] [26] [27] [28] [29] [30] [31] [32] [33] [34] [35] [36] [37] [38] [39] [40] [41] [42] [43] [44] [45] [46] [47] [48] [49] [50] [51] [52] [53] [54] [55] [56] [57] [58] [59] [60] [61] [62] [63] [64] [65] [66] [67] [68] [69] [70] [71] [72] [73] [74] [75] [76] [77] [78] [79] [80] [81] [82] [83] [84] [85] [86] [87] [88] [89] [90] [91] [92] [93] [94] [95] [96] [97] [98] [99] \n";
    assert(out.substr(0, expectedFirst.size()) == expectedFirst);
    // Check last row: 9900 to 9999
    std::string expectedLast = "[9900] [9901] [9902] [9903] [9904] [9905] [9906] [9907] [9908] [9909] [9910] [9911] [9912] [9913] [9914] [9915] [9916] [9917] [9918] [9919] [9920] [9921] [9922] [9923] [9924] [9925] [9926] [9927] [9928] [9929] [9930] [9931] [9932] [9933] [9934] [9935] [9936] [9937] [9938] [9939] [9940] [9941] [9942] [9943] [9944] [9945] [9946] [9947] [9948] [9949] [9950] [9951] [9952] [9953] [9954] [9955] [9956] [9957] [9958] [9959] [9960] [9961] [9962] [9963] [9964] [9965] [9966] [9967] [9968] [9969] [9970] [9971] [9972] [9973] [9974] [9975] [9976] [9977] [9978] [9979] [9980] [9981] [9982] [9983] [9984] [9985] [9986] [9987] [9988] [9989] [9990] [9991] [9992] [9993] [9994] [9995] [9996] [9997] [9998] [9999] \n";
    assert(out.substr(out.size() - expectedLast.size()) == expectedLast);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

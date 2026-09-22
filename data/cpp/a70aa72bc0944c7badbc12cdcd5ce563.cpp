/*
Write a standalone C++ function named `computeQRCodeMaskPenalty` that takes a 2D matrix of 0/1 values (represented as a `std::vector<std::vector<int>>` where 0 = light, 1 = dark) and an integer mask pattern between 0 and 7 inclusive, and returns the total penalty score for that mask pattern using QR Code mask evaluation rules 1–4. The function must: (a) first compute and sum the penalties from rules 1–4 directly from the matrix, and (b) then, for each cell in the matrix, apply the mask formula based on the given pattern (formulas: pattern 0: `(y+x)&1`, 1: `y&1`, 2: `x%3`, 3: `(y+x)%3`, 4: `((y>>1)+(x/3))&1`, 5: `((y*x)&1)+((y*x)%3)` returning 0 if the result is 0, 6: `(((y*x)&1)+((y*x)%3))&1`, 7: `(((y*x)%3)+((y+x)&1))&1`; result is 1 if the formula evaluates to non-zero, 0 if zero) and XOR that mask bit with the original cell value (i.e., `newBit = originalBit XOR maskBit`), then recompute the total penalty of the XOR-ed matrix using the same four rules and return the sum of the penalty from the original matrix and the penalty from the XOR-ed matrix. The input matrix is guaranteed to be non-empty, rectangular (all rows same length), and contain only 0 or 1 values. The mask pattern is guaranteed to be in [0,7]. Edge cases: matrices of size 1x1 or very small dimensions should still correctly apply all four penalty rules (rules that require adjacent cells or long patterns simply yield 0 if the condition cannot be met). For rule 4, use `abs(static_cast<int>(darkRatio * 100 - 50) / 5) * 5 * 10` where `darkRatio = numDarkCells / totalCells`. For rule 1, count consecutive same bits in each row and each column separately: when a run reaches length 5, add 3; for each additional bit beyond length 5 (i.e., length 6, 7, ...), add 1 per extra bit; reset run count to 0 at the start of each row/column. For rule 2, for every 2x2 block of identical bits (all four equal), add 3 per block. For rule 3, scan for the pattern 1011101 with four zeros on either side (either immediately before or after, but not both sides simultaneously; only one side must have exactly four consecutive zeros), in both horizontal and vertical directions, add 40 per occurrence; ignore positions that lie within the first/last 7 rows/columns if they coincide with the finder patterns (i.e., skip positions where x==0 and y in [0,6] or [height-7, height-1], x==width-7 and y in [0,6], y==0 and x in [0,6] or [width-7, width-1], y==height-7 and x in [0,6]). The function must be `const`-correct and take the matrix by `const` reference, and the mask pattern as an `int`. Do not use any external libraries beyond standard headers.
*/
#include <vector>
#include <cstdlib>  // for abs

// Compute penalty rule 1: consecutive same-bit runs in rows and columns.
static int penaltyRule1(const std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int penalty = 0;
    
    // Horizontal runs (rows)
    for (int y = 0; y < rows; ++y) {
        int run = 0;
        int prev = -1;
        for (int x = 0; x < cols; ++x) {
            int bit = matrix[y][x];
            if (bit == prev) {
                ++run;
                if (run == 5) {
                    penalty += 3;
                } else if (run > 5) {
                    penalty += 1;
                }
            } else {
                run = 1;
                prev = bit;
            }
        }
    }
    
    // Vertical runs (columns)
    for (int x = 0; x < cols; ++x) {
        int run = 0;
        int prev = -1;
        for (int y = 0; y < rows; ++y) {
            int bit = matrix[y][x];
            if (bit == prev) {
                ++run;
                if (run == 5) {
                    penalty += 3;
                } else if (run > 5) {
                    penalty += 1;
                }
            } else {
                run = 1;
                prev = bit;
            }
        }
    }
    return penalty;
}

// Compute penalty rule 2: 2x2 blocks of identical bits.
static int penaltyRule2(const std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int penalty = 0;
    for (int y = 0; y < rows - 1; ++y) {
        for (int x = 0; x < cols - 1; ++x) {
            int v = matrix[y][x];
            if (v == matrix[y][x+1] && v == matrix[y+1][x] && v == matrix[y+1][x+1]) {
                ++penalty;
            }
        }
    }
    return 3 * penalty;
}

// Compute penalty rule 3: 1011101 with four zeros on one side.
static int penaltyRule3(const std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int penalty = 0;
    
    auto skipFinder = [&](int x, int y) -> bool {
        if (x == 0 && ((y >= 0 && y <= 6) || (y >= rows - 7 && y <= rows - 1))) return true;
        if (x == cols - 7 && (y >= 0 && y <= 6)) return true;
        if (y == 0 && ((x >= 0 && x <= 6) || (x >= cols - 7 && x <= cols - 1))) return true;
        if (y == rows - 7 && (x >= 0 && x <= 6)) return true;
        return false;
    };
    
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (skipFinder(x, y)) continue;
            
            // Horizontal pattern
            if (x + 6 < cols &&
                matrix[y][x] == 1 && matrix[y][x+1] == 0 && matrix[y][x+2] == 1 &&
                matrix[y][x+3] == 1 && matrix[y][x+4] == 1 && matrix[y][x+5] == 0 &&
                matrix[y][x+6] == 1) {
                bool hasRightZeros = (x + 10 < cols) &&
                    matrix[y][x+7] == 0 && matrix[y][x+8] == 0 &&
                    matrix[y][x+9] == 0 && matrix[y][x+10] == 0;
                bool hasLeftZeros = (x - 4 >= 0) &&
                    matrix[y][x-1] == 0 && matrix[y][x-2] == 0 &&
                    matrix[y][x-3] == 0 && matrix[y][x-4] == 0;
                if (hasRightZeros || hasLeftZeros) {
                    penalty += 40;
                }
            }
            
            // Vertical pattern
            if (y + 6 < rows &&
                matrix[y][x] == 1 && matrix[y+1][x] == 0 && matrix[y+2][x] == 1 &&
                matrix[y+3][x] == 1 && matrix[y+4][x] == 1 && matrix[y+5][x] == 0 &&
                matrix[y+6][x] == 1) {
                bool hasBottomZeros = (y + 10 < rows) &&
                    matrix[y+7][x] == 0 && matrix[y+8][x] == 0 &&
                    matrix[y+9][x] == 0 && matrix[y+10][x] == 0;
                bool hasTopZeros = (y - 4 >= 0) &&
                    matrix[y-1][x] == 0 && matrix[y-2][x] == 0 &&
                    matrix[y-3][x] == 0 && matrix[y-4][x] == 0;
                if (hasBottomZeros || hasTopZeros) {
                    penalty += 40;
                }
            }
        }
    }
    return penalty;
}

// Compute penalty rule 4: proportion of dark cells.
static int penaltyRule4(const std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int darkCount = 0;
    for (const auto& row : matrix) {
        for (int val : row) {
            if (val == 1) ++darkCount;
        }
    }
    int total = rows * cols;
    double darkRatio = static_cast<double>(darkCount) / total;
    return abs(static_cast<int>(darkRatio * 100 - 50) / 5) * 5 * 10;
}

// Compute the total penalty for a matrix using all four rules.
static int totalPenalty(const std::vector<std::vector<int>>& matrix) {
    return penaltyRule1(matrix) + penaltyRule2(matrix) +
           penaltyRule3(matrix) + penaltyRule4(matrix);
}

// Compute total penalty after applying a mask pattern to the original matrix.
int computeQRCodeMaskPenalty(const std::vector<std::vector<int>>& matrix, int maskPattern) {
    // Original penalty
    int originalPenalty = totalPenalty(matrix);
    
    // Apply mask: XOR each cell with mask bit, then compute penalty of masked matrix.
    int rows = matrix.size();
    int cols = matrix[0].size();
    std::vector<std::vector<int>> maskedMatrix(rows, std::vector<int>(cols, 0));
    
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int temp = 0;
            int maskBit = 0;
            switch (maskPattern) {
                case 0: maskBit = ((y + x) & 1); break;
                case 1: maskBit = (y & 1); break;
                case 2: maskBit = (x % 3); break;
                case 3: maskBit = ((y + x) % 3); break;
                case 4: maskBit = (((y >> 1) + (x / 3)) & 1); break;
                case 5: {
                    temp = y * x;
                    maskBit = ((temp & 1) + (temp % 3)) == 0 ? 0 : 1;
                    break;
                }
                case 6: {
                    temp = y * x;
                    maskBit = (((temp & 1) + (temp % 3)) & 1);
                    break;
                }
                case 7: {
                    temp = y * x;
                    maskBit = (((temp % 3) + ((y + x) & 1)) & 1);
                    break;
                }
                default: break; // Should not happen
            }
            // XOR: if original is 1 and mask bit is 1 -> 0; if 0 and 1 -> 1; etc.
            int originalBit = matrix[y][x];
            int newBit = (originalBit != maskBit) ? 1 : 0;
            maskedMatrix[y][x] = newBit;
        }
    }
    
    int maskedPenalty = totalPenalty(maskedMatrix);
    return originalPenalty + maskedPenalty;
}
#include <cassert>
#include <vector>

// Forward declaration of the solution function (already defined above).
int computeQRCodeMaskPenalty(const std::vector<std::vector<int>>&, int);

int main() {
    // Test 1: All zeros 2x2 matrix, any mask pattern yields zeros after XOR (mask bits may be 1 but XOR with 0 gives 1, so not all zeros). Let's pick pattern 0.
    std::vector<std::vector<int>> zeros2x2 = {{0,0},{0,0}};
    // Original penalty: rule1=0, rule2=0, rule3=0, rule4=abs(0*100-50)/5*5*10 = abs(-50)/5*50 = 10*50? Wait careful: abs(0-50)/5 = 10, *5*10 = 500? Actually formula: abs((int)(darkRatio*100 - 50)/5)*5*10. darkRatio=0, so 0*100-50 = -50, int division by 5 = -10, abs=10, *5*10 = 500. So original penalty = 500.
    // After XOR with mask pattern 0: mask bit = (y+x)&1. Cells: (0,0)=0 XOR 0=0, (0,1)=0 XOR 1=1, (1,0)=0 XOR 1=1, (1,1)=0 XOR 0=0. So matrix becomes {{0,1},{1,0}}. Penalty of that: rule1: rows: 0,1 (no run), 1,0 (no run); cols: 0,1 (no), 1,0 (no) -> 0. Rule2: no 2x2 all same -> 0. Rule3: no pattern -> 0. Rule4: dark=2, total=4, ratio=0.5, 0.5*100-50=0, /5=0, abs=0, *5*10=0. So maskedPenalty=0. Total=500+0=500. We'll assert.
    assert(computeQRCodeMaskPenalty(zeros2x2, 0) == 500);

    // Test 2: All ones 1x1 matrix with pattern 7.
    std::vector<std::vector<int>> oneOne = {{1}};
    // Original: rule1=0, rule2=0, rule3=0, rule4: dark=1, total=1, ratio=1.0, 1.0*100-50=50, /5=10, abs=10, *5*10=500. Original=500.
    // Mask pattern 7: temp=0*0=0, maskBit = (0%3 + (0+0)&1)&1 = (0+0)&1=0. XOR 1^0=1.
    // Masked matrix same {{1}}, penalty same =500. Total=1000.
    assert(computeQRCodeMaskPenalty(oneOne, 7) == 1000);

    // Test 3: 3x3 with a horizontal 1011101 pattern including zeros. Use pattern that doesn't alter much.
    // Construct matrix: row 0: 1,0,1,1,1,0,1,0,0,0,0,0 (12 columns) – but we need at least 11 columns for zeros on one side. Let's make 12 columns and 1 row for simplicity, but matrix must be rectangular with at least 1 row, 1 col. We'll do 12x1.
    std::vector<std::vector<int>> pattern3 = {{1,0,1,1,1,0,1,0,0,0,0,0}};
    // Original penalty: rule1: one row: bits: 1,0,1,1,1,0,1,0,0,0,0,0. Runs: 1 (len1), 0 (1), 1 (len3? 1,1,1 = len3), 0 (1), 1 (1), 0 (len5? 0,0,0,0,0 = 5 zeros – run length 5 gives +3). So rule1=3. Rule2: no 2x2 because only 1 row -> 0. Rule3: horizontal pattern at x=0: 1,0,1,1,1,0,1 then zeros to right (7 zeros? Actually x+7..x+10 are 4 zeros, yes) -> +40. Also other positions? x=1 doesn't start with 1? So total 40. Rule4: dark count? Values: five 1's (positions 0,2,3,4,6) = 5, total 12, ratio=5/12≈0.4167, *100=41.67, minus 50 = -8.33, int cast to -8, /5 = -1 (integer division truncates toward zero? In C++ -8/5 = -1), abs=1, *5*10=50. So original penalty = 3+0+40+50=93.
    // Now apply mask pattern 1 (y&1). Since y=0 for all, maskBit=0 for all x. XOR with original gives same matrix. Masked penalty same =93. Total=186.
    assert(computeQRCodeMaskPenalty(pattern3, 1) == 186);

    // Test 4: 2x2 with all same bits, check rule2 contribution.
    std::vector<std::vector<int>> allOnes2x2 = {{1,1},{1,1}};
    // Original: rule1: each row has run of 2 (less than 5) -> 0; each col run of 2 -> 0. Rule2: one 2x2 block all same -> penalty=3*1=3. Rule3: 0. Rule4: dark=4, total=4, ratio=1, 1*100-50=50, /5=10, abs=10, *5*10=500. Original=3+500=503.
    // Mask pattern 2 (x%3): mask bits: x=0 ->0, x=1 ->1. Cells: (0,0)=1^0=1, (0,1)=1^1=0, (1,0)=1^0=1, (1,1)=1^1=0. Masked matrix {{1,0},{1,0}}. Penalty: rule1: rows: 1,0 (no run), 1,0 (no), cols: 1,1 (run of 2, not >5), 0,0 (run of 2) ->0. Rule2: no 2x2 all same because 1,0/1,0 not all same ->0. Rule3: 0. Rule4: dark=2, total=4, ratio=0.5, 0.5*100-50=0, /5=0, abs=0, *5*10=0. Masked=0. Total=503+0=503.
    assert(computeQRCodeMaskPenalty(allOnes2x2, 2) == 503);

    // Test 5: Random small matrix with pattern 5. We'll just ensure no crash and a non-negative result.
    std::vector<std::vector<int>> small = {{0,1},{1,0}};
    int result = computeQRCodeMaskPenalty(small, 5);
    assert(result >= 0);

    return 0;
}
// The solution must implement four penalty rule functions that each accept a `const std::vector<std::vector<int>>&` and return an `int`. Rule 1 scans each row and each column separately, tracking the length of consecutive identical bits; when a run of length 5 is reached, add 3, and for each extra bit beyond length 5, add 1; after finishing a line (row or column), reset the run counter. Rule 2 iterates over all top-left corners of 2x2 blocks and checks if all four cells are equal; if so, add 3 (since the rule originally adds 1 per block and multiplies by 3). Rule 3 is the most complex: for each cell, first skip positions that are inside the finder pattern zones (as specified), then check horizontally if the pattern 1,0,1,1,1,0,1 appears starting at (x,y) and has exactly four zeros either immediately to the right (positions x+7..x+10) or immediately to the left (x-1..x-4) — not both; if yes add 40. Then check vertically similarly. The skip condition prevents counting finder patterns as penalty patterns. Rule 4 counts dark (value 1) cells, computes darkRatio, applies `abs(static_cast<int>(darkRatio * 100 - 50) / 5) * 5 * 10`; note integer division truncates toward zero, and `abs` is from `<cstdlib>`. After computing the original penalty, we apply the mask: for each cell (x,y), compute the mask bit using the given formula, XOR with the original bit (1 if they differ, 0 if same), store in a new matrix, then compute the penalty of that new matrix using the same four rules. The result is the sum of the original penalty and the masked penalty. Time complexity: each rule scans the entire matrix in O(rows*cols) for rules 1–4, and rule 3 also scans per cell but with constant work, so O(rows*cols) per rule. We run each rule twice (once on original, once on masked), so total O(rows*cols) time; space O(rows*cols) for the masked matrix copy. Edge cases include 1x1 matrices (rules 1–3 yield 0, rule 4 works), and small matrices where adjacent patterns cannot occur; the implementation must correctly handle out-of-bounds conditions by condition checks.

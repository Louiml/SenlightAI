Write a C++ function `generateAntiDiagonalPattern` that takes a positive integer `n` and returns a `std::vector<std::vector<int>>` representing the triangular pattern shown in the snippet, where the first row contains `n` integers, the second row `n-1`, and so on, with each row's elements computed using the rule that the first element of each row is the next natural number after the last element of the previous row (starting from 1), and each subsequent element in a row is obtained by adding `(rowIndex + columnIndex + 2)` to the previous element (using 0-based indices). For example, for `n=5`, the output should be `{{1,3,6,10,15}, {2,5,9,14}, {4,8,13}, {7,12}, {11}}`. Handle edge cases like `n=1` (return `{{1}}`) and ensure the function works for any positive `n` without overflow for reasonable values (e.g., up to 1000).

// The pattern is constructed row by row. For each row `i` (0-indexed), we need `n - i` elements. The first element of row `i` is determined by a cumulative offset: after finishing row `i-1`, the next natural number is the last element of row `i-1` incremented by 1. In the snippet, this is handled by keeping a `num` variable that is reset to `num1++ + y` where `num1` and `y` track a running offset. Simpler: we can precompute all values by noticing that element at row `i`, column `j` (0-indexed) equals a base value $B_i + \sum_{k=0}^{j-1} (i + k + 2)$? Actually, the snippet uses `num += i+j` where `num` was initially set to the first element of the row. The first element of row `i` is `1 + sum_{r=0}^{i-1} (n - r)`? Let's derive: Row 0 starts at 1. Row 1 starts at 2 (1+1). Row 2 starts at 4 (2+2). Row 3 starts at 7 (4+3). So the first element of row `i` is `1 + sum_{r=0}^{i-1} (r+1) = 1 + i*(i+1)/2`. Then for each subsequent column `j` (starting from 0), we add `(i + j + 2)`? Check: For row 0, j=0→element 1, j=1→add (0+0+2)=2? But snippet adds `i+j` to `num` after starting at the first element. In snippet, for row 0, `num` starts at 1, then for j=0 (first iteration) prints `num` (1), then updates `num += i+j`? Actually in snippet, inside the inner loop they print `num`, then after printing they do `num += i+j`. For row 0, j=0: print 1, then add 0+0=0? That would keep 1? But the output shows 1,3,6,... So they must add after printing? Let's trace: For row 0, i=0, j runs 0..4. They print `num` (starts 1). Then `num += i+j` → num += 0+0=0 → num stays 1. Next iteration j=1, print 1? But output has 3. So my interpretation is wrong. Looking at snippet: They do `cout << num << ' '; num += i+j;` So for row 0, i=0, j=0: print 1, then num+=0 → num=1. j=1: print 1? That can't be. Perhaps they intended `num += i+j+1`? The comment shows pattern, but the snippet has a bug? Let's check: For row 0, expected 1,3,6,10,15 differences: +2,+3,+4,+5. That equals i+j+2 for j=0..3 (since i=0, j=0 gives 2). So they should add `i+j+2`. In snippet they wrote `num += i+j;` which only adds 0 for first pair, causing repeated 1? Actually let's test: If you print then add, for row 0, after printing 1, add 0+0=0, so next print 1 again. But the output comment shows 1,3,6,... So either the snippet is wrong or I misread. The code as given would produce all 1s for first row. That's a bug. But the task is to generate the pattern shown in the comment. So we need to implement correct pattern: For row `i`, starting value = `1 + i*(i+1)/2`. Then for each subsequent element, add `(i + j + 2)` where `j` is the previous column index? More precisely: element at (i,j) = start_i + sum_{k=0}^{j-1} (i + k + 2). For example, row 0: start=1, then for j=1: add (0+0+2)=2 →3; j=2: add (0+1+2)=3 →6; j=3: add 4 →10; j=4: add 5 →15. Correct. Row 1: start=2, then add (1+0+2)=3 →5; add (1+1+2)=4 →9; add 5 →14; add 6 →20? But expected row 1 is 2,5,9,14 (only 4 elements, since n=5, row 1 has 4). So for j=1: add 3→5; j=2: add 4→9; j=3: add 5→14. Yes. Row 2: start=4, add (2+0+2)=4→8; add 5→13; add 6→19? Expected 4,8,13 (3 elements). So correct. Thus algorithm: For each row i from 0 to n-1, compute start = 1 + i*(i+1)/2. Then for j from 0 to n-i-1: value = start + sum_{k=0}^{j-1} (i + k + 2). We can compute iteratively: current = start; for each j, push current, then add (i + j + 2) to current for next iteration (but note for j=0, we add (i+0+2) after pushing, for next j=1). So implement: vector row; int current = start; for j in 0..n-i-1: row.push_back(current); current += (i + j + 2); // after pushing, update for next iteration. Edge cases: n=1 gives start=1, row has 1 element. Time complexity: O(n^2) because total number of elements is n(n+1)/2, each O(1). Space: O(n^2) for result.

#include <vector>

// Generate the anti-diagonal triangular number pattern for n rows.
// Each row i (0-indexed) has n-i elements. First element of row i is 1 + i*(i+1)/2.
// Each subsequent element is obtained by adding (i + previous_column + 2) to the previous element.
std::vector<std::vector<int>> generateAntiDiagonalPattern(int n) {
    std::vector<std::vector<int>> result;
    result.reserve(n);
    for (int i = 0; i < n; ++i) {
        int start = 1 + i * (i + 1) / 2;
        std::vector<int> row;
        row.reserve(n - i);
        int current = start;
        for (int j = 0; j < n - i; ++j) {
            row.push_back(current);
            // Update for next element in this row.
            current += (i + j + 2);
        }
        result.push_back(std::move(row));
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume generateAntiDiagonalPattern is defined above.

int main() {
    // n=1: single element
    auto r1 = generateAntiDiagonalPattern(1);
    assert(r1.size() == 1 && r1[0] == std::vector<int>{1});

    // n=2: pattern {{1,3},{2}}
    auto r2 = generateAntiDiagonalPattern(2);
    assert(r2.size() == 2);
    assert(r2[0] == std::vector<int>({1, 3}));
    assert(r2[1] == std::vector<int>({2}));

    // n=3: {{1,3,6},{2,5},{4}}
    auto r3 = generateAntiDiagonalPattern(3);
    assert(r3.size() == 3);
    assert(r3[0] == std::vector<int>({1, 3, 6}));
    assert(r3[1] == std::vector<int>({2, 5}));
    assert(r3[2] == std::vector<int>({4}));

    // n=5: full example from snippet
    auto r5 = generateAntiDiagonalPattern(5);
    assert(r5.size() == 5);
    assert(r5[0] == std::vector<int>({1, 3, 6, 10, 15}));
    assert(r5[1] == std::vector<int>({2, 5, 9, 14}));
    assert(r5[2] == std::vector<int>({4, 8, 13}));
    assert(r5[3] == std::vector<int>({7, 12}));
    assert(r5[4] == std::vector<int>({11}));

    // n=4: check row lengths and last element
    auto r4 = generateAntiDiagonalPattern(4);
    assert(r4.size() == 4);
    assert(r4[0].size() == 4);
    assert(r4[1].size() == 3);
    assert(r4[2].size() == 2);
    assert(r4[3].size() == 1);
    assert(r4[3][0] == 7); // 1+2+4? actually for n=4: row0:1,3,6,10; row1:2,5,9; row2:4,8; row3:7
    return 0;
}

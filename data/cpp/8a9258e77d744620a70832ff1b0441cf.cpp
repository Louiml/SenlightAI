Write a C++ function that simulates the process of repeatedly cutting a rectangle into a square and a smaller rectangle, always cutting along the shorter side, until one dimension reaches zero. Given the initial height and width (both positive integers), the function should return the total number of square pieces obtained after repeatedly removing the largest possible square from the remaining rectangle. For example, for a 2019×324 rectangle, the process counts the number of squares needed to tile the rectangle completely.

The core idea is a greedy approach: at each step, we remove the largest square possible, which has side length equal to `min(height, width)`. We increment a counter for each square removed, then subtract that side length from the longer dimension. This is essentially Euclidean algorithm–like division: rather than subtracting one square at a time, we could use integer division to speed up, but the simple iterative subtraction works fine for moderate values and is straightforward to implement. Edge cases include when one dimension becomes zero (termination), and when both are equal (one final square). The process always terminates because each step reduces at least one dimension by a positive amount. Time complexity is O(h + w) in the worst case (e.g., 1×n), but using division would be O(log(min(h,w))). Space complexity is O(1) auxiliary.

#include <cstdint>

/*
* Count the number of squares obtained by repeatedly cutting the largest
* possible square from a rectangle of given height and width.
* Both dimensions must be positive.
* Returns the total number of square pieces.
*/
int countSquares(int height, int width) {
    int count = 0;
    while (height > 0 && width > 0) {
        if (height > width) {
            count += height / width;
            height %= width;
        } else {
            count += width / height;
            width %= height;
        }
    }
    return count;
}

#include <cassert>

int countSquares(int height, int width);

int main() {
    // Test with the original example: 2019x324
    assert(countSquares(2019, 324) == 15);
    
    // Square rectangle: one square
    assert(countSquares(5, 5) == 1);
    
    // Thin strip: 1x100 gives 100 squares of size 1x1
    assert(countSquares(1, 100) == 100);
    
    // Simple 2x3 rectangle: one 2x2 square + two 1x1 squares = 3
    assert(countSquares(2, 3) == 3);
    
    // 3x5 rectangle: one 3x3 + one 2x2 + two 1x1 = 4
    assert(countSquares(3, 5) == 4);
    
    // Large equal dimensions
    assert(countSquares(10, 10) == 1);
    
    // Swapped dimensions should give same result
    assert(countSquares(324, 2019) == countSquares(2019, 324));
    
    // 6x4: one 4x4 + two 2x2 = 3 squares
    assert(countSquares(6, 4) == 3);
    
    // 7x3: two 3x3 + one 1x1 = 3 squares
    assert(countSquares(7, 3) == 3);
    
    // 13x8: one 8x8 + one 5x5 + one 3x3 + one 2x2 + two 1x1 = 6
    assert(countSquares(13, 8) == 6);
    
    return 0;
}

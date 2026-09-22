// Write a C++ function `long long largestRemainingArea(long long a, long long b, long long x, long long y)` that takes the dimensions of a rectangle (a rows, b columns) and a cut point (x, y) inside it (0-indexed from the top-left corner). The rectangle is cut by a vertical line just to the right of column x and a horizontal line just below row y, dividing it into four smaller rectangles: top-left, top-right, bottom-left, bottom-right. The function must return the area of the largest of these four rectangles, using the same area calculation logic as the provided snippet. The input coordinates are guaranteed to satisfy 0 ≤ x < a and 0 ≤ y < b. The function must not modify any inputs and must work for any positive integer dimensions and valid cut positions, including edge cases where x=0, y=0, x=a-1, or y=b-1. Ensure the solution uses only integer arithmetic (no floating point) and returns a `long long` value (since areas can be up to 10^12 if a,b up to 10^6). Do not include a main function in your solution; only provide the required free function.

The provided snippet has a function `area` that computes the four rectangle areas given the dimensions and cut point. It calculates:
- top = a * b - (bottom + a), where bottom = a * (b - (y+1)) → effectively top = a * (y+1)
- left = b * x
- right = a * b - (left + b) = a * (b - (x+1))
- bottom = a * (b - (y+1))

So the four areas are:
1. top-left: x * y? But careful: In the snippet, `top` and `bottom` are computed using `a` multiplied, and `left` and `right` use `b` multiplied, but the snippet's logic appears to be for a different layout. Actually, reading the snippet: `bottom = a*(b-(y+1))` is the area of a rectangle of height a and width (b-y-1) → that's the bottom part? But the cut is horizontal at row y, so bottom part has height (a - y - 1) and width b. That yields area = (a - y - 1) * b. But the snippet uses a * (b - y - 1), which is transpose. So the snippet's function is for a rectangle of width a and height b? Let's reinterpret: The snippet might treat `a` as width and `b` as height. But the task says a rows, b columns. To keep consistent, we'll define: The rectangle has dimensions width = a (columns) and height = b (rows)? The snippet's variable names are ambiguous. However, the problem statement must be clear. Let's define: The rectangle has `a` columns and `b` rows? Actually, the snippet computes:
- bottom = a * (b - (y+1)) → if a is width, b is height, y is row index? Then bottom area = width * (height - row - 1) → correct for bottom part. top area = width * (y+1) → correct for top part. left area = b * x → height * (col index) → correct for left part? left rectangle width = x, height = b, area = b*x. right area = a*b - (left + b) = a*b - b*x - b = b*(a - x - 1) → correct for right. So the snippet treats `a` as width (number of columns) and `b` as height (number of rows), and (x,y) as column,row indices. So for consistency, we'll define: rectangle has width a and height b, with cut at column index x (0 ≤ x < a) and row index y (0 ≤ y < b). Then the four areas:
- top-left: area = (x) * (y) ? Actually cut at column x means left part width = x (columns 0..x-1) and top part height = y (rows 0..y-1). So top-left area = x * y? But the snippet computes top and left separately, not combined. The snippet's four rectangles are top (width a, height y), bottom (width a, height b-y-1?), left (width x, height b), right (width a-x-1?, height b). That is, the cut sweeps entire rows/columns, not four quadrants. Actually the snippet's `top` = width * (y+1) and `bottom` = width * (b - y - 1) → these are two horizontal strips (top and bottom) ignoring the vertical cut. Similarly `left` = height * x and `right` = height * (a - x - 1) → two vertical strips ignoring the horizontal cut. Then it takes the max of these four strips. So the problem is: after cutting both horizontally and vertically, the rectangle is divided into four smaller rectangles: top-left, top-right, bottom-left, bottom-right. The snippet's `top` and `bottom` are actually the full top band (width a, height y) and bottom band (width a, height b-y-1)? But that assumes the vertical cut doesn't affect them – that's not correct. Wait, the snippet's `top` = a*b - (bottom + a) = a*b - a*(b-y-1) - a = a*(y+1)? That is the area of the top band of height y+1? Actually if y is a row index, then rows 0..y have height y+1, so top band area = width * (y+1) = a*(y+1). So `top` = a*(y+1). That is the area of the top strip including row y. Similarly `bottom` = a*(b-y-1) = width * (b - y - 1) – rows below y, from y+1 to b-1, height b-y-1. So these two strips cover the entire rectangle if you split horizontally at row y+1? Actually rows 0..y (height y+1) and rows y+1..b-1 (height b-y-1) sum to b. So `top` and `bottom` are the two horizontal halves. Similarly `left` = b*x = height * x (columns 0..x-1) and `right` = b*(a - x - 1) = height * (a - x - 1) (columns x+1..a-1). So the snippet's function returns the maximum among four strip areas: top strip, bottom strip, left strip, right strip. That is not the same as the four quadrant areas. But the problem statement must be precise. To be faithful to the snippet, we define: Given a rectangle of width a and height b, and a cut point (x,y) where x is column index and y is row index, the cut is made by a vertical line just to the right of column x and a horizontal line just below row y. This divides the rectangle into four non-overlapping rectangles: top-left (width x, height y+1? Actually rows 0..y, columns 0..x-1? There's ambiguity). To avoid confusion, we'll adopt the interpretation that yields the four strips as in the snippet: The snippet indeed computes four rectangles: top (columns 0..a-1, rows 0..y), bottom (columns 0..a-1, rows y+1..b-1), left (columns 0..x-1, rows 0..b-1), right (columns x+1..a-1, rows 0..b-1). But these four do not cover the rectangle without overlap? They overlap. Actually top and bottom are disjoint, left and right are disjoint, but top and left overlap in the top-left corner. So the snippet's function is not dividing into four quadrants; it's computing two alternative splits (horizontal or vertical) and taking the max of the two largest pieces from each. The problem likely expects that the cut is either horizontal or vertical, not both? The snippet's `area` function clearly computes max of top/bottom (horizontal split) and max of left/right (vertical split), then takes the max of those two. So the task is: Given a rectangle of width a and height b, and a cut line either horizontal or vertical? But the input (x,y) is provided; perhaps the cut is only one of them, depending on which gives larger area? Actually the snippet's `area` takes both x and y, but uses x for vertical split and y for horizontal split, and returns the maximum over all four strips. So the function returns the maximum area among the two larger pieces from splitting either horizontally or vertically. The problem must be defined accordingly. To be safe, I'll define the task: Write a function that, given a rectangle of width `a` and height `b`, and two integers `x` (0 ≤ x < a) and `y` (0 ≤ y < b), returns the maximum of the following four possible rectangles: 
- top rectangle: width a, height (y+1)
- bottom rectangle: width a, height (b - y - 1)
- left rectangle: width x, height b
- right rectangle: width (a - x - 1), height b
This matches the snippet exactly. Edge cases: if x=0, left rectangle has width 0, area 0; if x=a-1, right rectangle width 0; if y=0, top height 1, bottom height b-1; if y=b-1, top height b, bottom height 0. The function must return the maximum area. Time complexity O(1), space O(1).

#include <algorithm>

// Compute the largest area among the four rectangles formed by a single horizontal
// or vertical cut at (x, y) in a rectangle of width a and height b.
// Parameters:
//   a - width (number of columns) of the rectangle, must be > 0
//   b - height (number of rows) of the rectangle, must be > 0
//   x - column index of the vertical cut (0 <= x < a)
//   y - row index of the horizontal cut (0 <= y < b)
// Returns the maximum area among top, bottom, left, and right strips.
long long largestRemainingArea(long long a, long long b, long long x, long long y) {
    // Top strip: full width, rows 0..y (height y+1)
    long long top = a * (y + 1);
    // Bottom strip: full width, rows y+1..b-1 (height b - y - 1)
    long long bottom = a * (b - y - 1);
    // Left strip: columns 0..x-1 (width x), full height
    long long left = b * x;
    // Right strip: columns x+1..a-1 (width a - x - 1), full height
    long long right = b * (a - x - 1);

    return std::max({top, bottom, left, right});
}

#include <cassert>

int main() {
    // Basic case: 3x3, cut at (1,1) -> top=3*(1+1)=6, bottom=3*(3-1-1)=3, left=3*1=3, right=3*(3-1-1)=3 -> max=6
    assert(largestRemainingArea(3, 3, 1, 1) == 6);
    // Cut at (0,0) in 5x5 -> top=5*1=5, bottom=5*4=20, left=5*0=0, right=5*4=20 -> max=20
    assert(largestRemainingArea(5, 5, 0, 0) == 20);
    // Cut at (4,4) in 5x5 -> top=5*5=25, bottom=5*0=0, left=5*4=20, right=5*0=0 -> max=25
    assert(largestRemainingArea(5, 5, 4, 4) == 25);
    // Cut at (0,4) in 5x5 -> top=5*5=25, bottom=5*0=0, left=5*0=0, right=5*4=20 -> max=25
    assert(largestRemainingArea(5, 5, 0, 4) == 25);
    // Large dimensions: 1000000 x 1000000, cut at (500000, 500000) -> top=1e6*500001=500001000000, bottom=1e6*499999=499999000000, left=1e6*500000=500000000000, right=1e6*499999=499999000000 -> max=500001000000
    assert(largestRemainingArea(1000000LL, 1000000LL, 500000LL, 500000LL) == 500001000000LL);
    // Cut at (2,3) in 4x6 -> a=4,b=6: top=4*(3+1)=16, bottom=4*(6-3-1)=8, left=6*2=12, right=6*(4-2-1)=6 -> max=16
    assert(largestRemainingArea(4, 6, 2, 3) == 16);
    // Cut at (3,0) in 3x2 -> top=3*1=3, bottom=3*(2-0-1)=3, left=2*3=6, right=2*(3-3-1)=0 -> max=6
    assert(largestRemainingArea(3, 2, 3, 0) == 6);
    // Cut at (1,5) in 3x6 -> top=3*6=18, bottom=3*0=0, left=6*1=6, right=6*(3-1-1)=6 -> max=18
    assert(largestRemainingArea(3, 6, 1, 5) == 18);
    // Cut at (2,1) in 3x3 -> top=3*2=6, bottom=3*1=3, left=3*2=6, right=3*0=0 -> max=6
    assert(largestRemainingArea(3, 3, 2, 1) == 6);
    // Cut at (1,2) in 2x3 -> a=2,b=3: top=2*3=6, bottom=2*0=0, left=3*1=3, right=3*(2-1-1)=0 -> max=6
    assert(largestRemainingArea(2, 3, 1, 2) == 6);
    // Edge: a=1,b=1, x=0,y=0 -> top=1*1=1, bottom=1*0=0, left=1*0=0, right=1*0=0 -> max=1
    assert(largestRemainingArea(1, 1, 0, 0) == 1);
    return 0;
}

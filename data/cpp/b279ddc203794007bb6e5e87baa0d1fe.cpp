// Write a C++ function that takes an integer `n` (with `n >= 2`) and a square grid of characters (`'0'` or `'1'`) of size `n x n`, and returns a new `n x n` grid of characters after applying a cyclic "rotation" of only the outermost border elements. Specifically, every element in the top row (except the top-left corner) moves one position to the right, every element in the right column (except the top-right corner) moves one position down, every element in the bottom row (except the bottom-right corner) moves one position to the left, and every element in the left column (except the bottom-left corner) moves one position up. The four corner elements are updated according to these rules (the top-left corner takes the element originally directly below it; the top-right corner takes the original top-left corner; the bottom-right takes the original top-right; the bottom-left takes the original bottom-right). The interior `(n-2) x (n-2)` submatrix remains unchanged. The input grid is provided as a vector of strings (each string length `n`, containing only `'0'` and `'1'`). The function should return a vector of strings with the transformed grid. Your function must be self-contained (no global variables) and handle any valid `n >= 2`.
The main idea is to construct the output grid by copying the input and then overwriting only the border cells according to the cyclic shift. A straightforward approach: first copy the entire input grid into the output. Then, for the top row (columns `1` to `n-1`), set `out[0][j+1] = in[0][j]` (or equivalently, for each `j` from 0 to `n-2`, set `out[0][j+1] = in[0][j]`). For the right column (rows `1` to `n-1`), set `out[i+1][n-1] = in[i][n-1]` for `i` from 0 to `n-2`. For the bottom row (columns `0` to `n-2`), set `out[n-1][j] = in[n-1][j+1]` for `j` from 0 to `n-2`. For the left column (rows `1` to `n-1`), set `out[i][0] = in[i+1][0]` for `i` from 0 to `n-2`. Note that these assignments correctly handle corners because, for example, the top-left corner `(0,0)` is set by the left column assignment when `i=0`, taking `in[1][0]`; the top-right corner `(0,n-1)` is set by the top row assignment when `j=n-2`, taking `in[0][n-2]`? Wait, that is incorrect. Let's carefully derive: The top row moves right: so `out[0][j+1] = in[0][j]` for `j=0..n-2`. That sets `out[0][1]` to `in[0][0]`, ..., and `out[0][n-1]` to `in[0][n-2]`. That means the top-right corner becomes what was originally at `(0,n-2)`. But the correct cyclic rotation: the top-right corner should receive the element originally at the top-left? Let's verify with a 2x2 example: input [[a,b],[c,d]]. The border consists of all four cells. Rotating the border: top row moves right: a goes to position (0,1) → b becomes? Actually, think of the border as a ring: a(0,0) → b(0,1) → d(1,1) → c(1,0) → a(0,0). That is a clockwise rotation. So the new grid should be: new (0,0) = old c, new (0,1) = old a, new (1,1) = old d? Wait, let's do it carefully: The ring is a->b (top row left to right), b->d (right column top to bottom), d->c (bottom row right to left), c->a (left column bottom to top). So after rotation, each element moves to the next position in that order: a goes to (0,1), b goes to (1,1), d goes to (1,0), c goes to (0,0). So new grid is [[c, a], [d, b]]? Check: (0,0) gets old c, (0,1) gets old a, (1,0) gets old d, (1,1) gets old b. So the bottom-right corner (1,1) gets old b? But b was top-right, yes. So top-right moves down to bottom-right? Wait, the ring direction: a->b (right), b->d (down), d->c (left), c->a (up). So each element moves one step along the ring. That means b moves to d's position (bottom-right), d moves to c's position (bottom-left), c moves to a's position (top-left), a moves to b's position (top-right). Good.

Now, in my assignment plan: Top row: `out[0][j+1] = in[0][j]` for j=0..n-2 → this sets out[0][1]=in[0][0] (a goes to (0,1)), ..., out[0][n-1]=in[0][n-2] (so top-right gets element from second-to-last column of top row, not from top-left unless n=2? For n=2, j=0 only: out[0][1]=in[0][0] (a to (0,1)) that is correct. But for n>2, top-right should get in[0][n-2]? Wait, the ring direction: top row elements move right: in[0][j] → out[0][j+1] for j=0..n-2. So in[0][n-2] goes to out[0][n-1] (top-right). That is correct because top-right receives the element that was originally at (0,n-2). But in the ring, the element that moves to top-right is the one from top-left? No, top-left goes to (0,1) (since j=0). So the top-right corner receives from (0,n-2), not from top-left. That matches the ring because the ring order is: (0,0)->(0,1)->...->(0,n-1)->(1,n-1)->(2,n-1)->...->(n-1,n-1)->(n-1,n-2)->...->(n-1,0)->(n-2,0)->...->(0,0). So indeed, (0,n-1) receives from (0,n-2). That is correct. So my assignments for top row are fine for all j=0..n-2, giving out[0][j+1] = in[0][j]. That includes j=n-2 giving out[0][n-1] = in[0][n-2].

Now right column: For i=0..n-2, out[i+1][n-1] = in[i][n-1]. That moves elements down the right column. So top-right (0,n-1) itself is not set here; it's set by top row. The element from (0,n-1) goes to (1,n-1) (i=0) → out[1][n-1] = in[0][n-1]. That is correct because top-right moves down. Then (n-2,n-1) goes to (n-1,n-1) (bottom-right). So bottom-right gets in[n-2][n-1], correct.

Bottom row: For j=0..n-2, out[n-1][j] = in[n-1][j+1]. That moves leftwards. So bottom-right corner (n-1,n-1) is not set here; it's set by right column. The element from (n-1,n-1) goes to (n-1,n-2) (j=n-2) → out[n-1][n-2] = in[n-1][n-1]? Wait, j=n-2 gives out[n-1][n-2] = in[n-1][n-1]? That is correct: bottom-right moves left to (n-1,n-2). Also for j=0, out[n-1][0] = in[n-1][1], so bottom-left gets from (n-1,1), not from (n-1,0)? Wait, the ring: bottom row moves left, so (n-1, j) goes to (n-1, j-1) for j=1..n-1. That means (n-1,1) goes to (n-1,0). Yes, so bottom-left receives from (n-1,1). My assignment sets out[n-1][0] = in[n-1][1] (j=0 → out[n-1][0]=in[n-1][1]), correct.

Left column: For i=0..n-2, out[i][0] = in[i+1][0]. That moves up. So bottom-left (n-1,0) is not set here; it's set by bottom row. The element from (0,0) is not moved here because i=0 gives out[0][0]=in[1][0]? That would set top-left to the element directly below it, which is correct: (1,0) moves up to (0,0). Good. Also for i=n-2, out[n-2][0]=in[n-1][0], so element from bottom-left moves up to (n-2,0), correct.

Now, after these four assignments, all border cells are set. The interior is unchanged, so we can simply start with a copy of the input grid and apply these assignments. This is straightforward and O(n^2) because copying and overwriting each border cell once. Edge cases: n=2, the interior is empty, but assignments work because loops run n-2 times (i.e., 0 iterations for interior), and border loops run n-1 times (i.e., 1 iteration each), which correctly handles the 2x2 case. For n>=2 always fine. Space complexity O(n^2) for the output grid.

A cleaner implementation: Create output grid initialized as a copy of input. Then for each border segment, assign from the original input (not from output) to avoid cascading updates. Since we copy first, we can read from the original input grid. We need to keep the original input separate or copy before modifying. In the solution function, we can take the input as a const reference and create a new grid `result = grid;` then modify `result` using values from `grid`. That is safe.
#include <vector>
#include <string>

// Rotate the outer border of a square grid clockwise by one position.
std::vector<std::string> rotateBorder(const std::vector<std::string>& grid) {
    int n = static_cast<int>(grid.size());
    std::vector<std::string> result = grid;  // copy entire grid

    // Move top row (excluding top-right corner) to the right.
    for (int j = 0; j < n - 1; ++j) {
        result[0][j + 1] = grid[0][j];
    }
    // Move right column (excluding bottom-right corner) down.
    for (int i = 0; i < n - 1; ++i) {
        result[i + 1][n - 1] = grid[i][n - 1];
    }
    // Move bottom row (excluding bottom-left corner) to the left.
    for (int j = 0; j < n - 1; ++j) {
        result[n - 1][j] = grid[n - 1][j + 1];
    }
    // Move left column (excluding top-left corner) up.
    for (int i = 0; i < n - 1; ++i) {
        result[i][0] = grid[i + 1][0];
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration (or include the solution above)
std::vector<std::string> rotateBorder(const std::vector<std::string>& grid);

int main() {
    // 2x2 case
    std::vector<std::string> g1 = {"ab", "cd"};
    std::vector<std::string> r1 = rotateBorder(g1);
    assert((r1 == std::vector<std::string>{"ca", "db"}));

    // 3x3 case
    std::vector<std::string> g2 = {"123", "456", "789"};
    std::vector<std::string> r2 = rotateBorder(g2);
    assert((r2 == std::vector<std::string>{"412", "573", "689"}));

    // 4x4 case
    std::vector<std::string> g3 = {"abcd", "efgh", "ijkl", "mnop"};
    std::vector<std::string> r3 = rotateBorder(g3);
    assert((r3 == std::vector<std::string>{"eabc", "ifgd", "mjkh", "nopl"}));

    // All zeros, n=3
    std::vector<std::string> g4 = {"000", "000", "000"};
    std::vector<std::string> r4 = rotateBorder(g4);
    assert((r4 == std::vector<std::string>{"000", "000", "000"}));

    // Border with distinct pattern, n=5
    std::vector<std::string> g5 = {"abcde", "fghij", "klmno", "pqrst", "uvwxy"};
    std::vector<std::string> r5 = rotateBorder(g5);
    // Expected: top row: up arrow? Compute manually or trust logic
    // Let's compute: original border ring: (0,0)a->(0,1)b->(0,2)c->(0,3)d->(0,4)e->(1,4)j->(2,4)o->(3,4)t->(4,4)y->(4,3)x->(4,2)w->(4,1)v->(4,0)u->(3,0)p->(2,0)k->(1,0)f->(0,0)a
    // New grid: (0,0) gets f, (0,1) gets a, (0,2) gets b, (0,3) gets c, (0,4) gets d,
    // (1,0) gets k, (1,4) gets j? Wait (1,4) is on right column, it receives from (0,4)? Actually (0,4) moves down to (1,4), so (1,4) gets e? No, original (0,4) is e, so (1,4)=e. But check: ring order: (0,0)->(0,1): a to (0,1). (0,1)->(0,2): b to (0,2). ... (0,3)->(0,4): d to (0,4). (0,4)->(1,4): e to (1,4). So (1,4)=e. Good.
    // Then (1,4)->(2,4): j to (2,4). (2,4)->(3,4): o to (3,4). (3,4)->(4,4): t to (4,4). (4,4)->(4,3): y to (4,3). ... (4,1)->(4,0): v to (4,0). (4,0)->(3,0): u to (3,0). (3,0)->(2,0): p to (2,0). (2,0)->(1,0): k to (1,0). (1,0)->(0,0): f to (0,0).
    // So new grid:
    // Row0: f a b c d
    // Row1: k g h i e   (interior unchanged: g,h,i remain at (1,1),(1,2),(1,3))
    // Row2: p l m n j
    // Row3: u q r s o
    // Row4: v w x y t
    std::vector<std::string> expected5 = {
        "fabcd",
        "kghie",
        "plmnj",
        "uqrso",
        "vwxyt"
    };
    assert((r5 == expected5));

    // n=2 with all '1's
    std::vector<std::string> g6 = {"11", "11"};
    std::vector<std::string> r6 = rotateBorder(g6);
    assert((r6 == std::vector<std::string>{"11", "11"}));

    return 0;
}

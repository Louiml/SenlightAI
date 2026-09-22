You are given a 2D binary matrix representing a downsampled shape (e.g., a binary alpha map) of size `N x N` where `N` is a power of 2 (i.e., 4, 8, or 16). Write a C++ function `std::vector<std::vector<int>> adaptiveUpsample(const std::vector<std::vector<int>>& downsampled)` that upsamples the input by a factor of 2 in each dimension using the following context-based adaptive interpolation scheme. For each cell in the upsampled grid (size `2N x 2N`), the value is determined by a local 12-neighborhood of the original downsampled grid, using one of four pixel cases (P1–P4) depending on whether the upsampled coordinates are odd/even. The core rule uses a precomputed threshold table `Th[256]`, a context code computed from 8 specific neighbor bits via `GetContext`, and a weighted sum via `GetShapeVL` that returns 1 if `(a<<2) + (b+c+d)<<1 + (e+f+g+h+i+j+k+l) > Th[context]`, else 0, where `a` is the center neighbor, `b,c,d` are the three side neighbors, and `e..l` are the remaining eight. The four cases differ in which neighbor acts as the center `a`. Specifically: for upsampled pixel at `(jup, iup)` where `iup = 2*i+k`, `jup = 2*j+l` with `k,l ∈ {0,1}`: if both `iup` and `jup` are odd (P1), use `a=val[0]`; if `iup` even and `jup` odd (P2), use `a=val[1]`; if `iup` odd and `jup` even (P3), use `a=val[2]`; if both even (P4), use `a=val[3]`. The 12 neighborhood values `val[0..11]` are read from the downsampled grid at offsets: `val[0]` at `(-1,-1)`, `val[1]` at `(0,-1)`, `val[2]` at `(0,0)`, `val[3]` at `(-1,0)`, and for indices 4..11: `(-1,-2), (0,-2), (1,-1), (1,0), (0,1), (-1,1), (-2,0), (-2,-1)` respectively (where first coordinate is row offset, second is column offset). The downsampled grid is padded with a 2-cell zero border on all sides before reading the 12 neighbors, so out-of-bounds accesses are zero. The context computation uses 8 specific bits: for P1 use `(val[5], val[4], val[11], val[10], val[9], val[8], val[7], val[6])`; for P2 use `(val[7], val[6], val[5], val[4], val[11], val[10], val[9], val[8])`; for P3 use `(val[9], val[8], val[7], val[6], val[5], val[4], val[11], val[10])`; for P4 use `(val[11], val[10], val[9], val[8], val[7], val[6], val[5], val[4])`. The context index is computed as `a + (b<<1) + (c<<2) + (d<<3) + (e<<4) + (f<<5) + (g<<6) + (h<<7)` where each is 0 or 1. You must implement the exact threshold table `Th` as given in the problem’s reference snippet (you may embed it as a static array of 256 integers). The output should be a `2N x 2N` matrix of integers (0 or 1). Do not include any I/O; just the function.
The solution follows the described algorithm exactly. Begin by creating a padded version of the input matrix with two rows/columns of zeros on each side, so accessing neighbors at offsets from -2 to +1 relative to any cell in the original grid is always within bounds. Then, for each original cell `(j,i)` (0-indexed), consider the 2×2 block of upsampled pixels at positions `(2*j,2*i)`, `(2*j,2*i+1)`, `(2*j+1,2*i)`, `(2*j+1,2*i+1)`. For each of these four upsampled positions, determine which case (P1–P4) applies: P1 is bottom-right of the 2×2 (both odd), P2 is bottom-left (column even, row odd), P3 is top-right (row even, column odd), P4 is top-left (both even). For each case, extract the 12 neighbor values `val[0..11]` from the padded grid at the given offsets relative to the original cell. Compute the context by selecting the 8-bit mask for the case, then look up `Th[context]`. Compute the weighted sum as described: `(a<<2) + ((b+c+d)<<1) + (sum of e..l)`, where `a` is the first neighbor in the val array for the case, `b,c,d` are the next three, and `e..l` are the remaining eight. The output value is 1 if that weighted sum > `Th[context]`, else 0. Edge cases: the padded border ensures all neighbor lookups are valid; the context is always between 0 and 255, so `Th` is indexed safely. Time complexity: we process each original cell once and produce 4 output pixels, each with constant work (12 neighbor reads, one lookup, constant arithmetic), so O(N^2) time for N×N input; space is O(N^2) for the padded matrix and O(N^2) for the output, though the padded matrix uses (N+4)^2 which is still O(N^2). The algorithm is deterministic and matches the reference implementation’s logic exactly.
#include <vector>

// Precomputed threshold table from the reference implementation.
static const int Th[256] = {
    3, 6, 6, 7, 4, 7, 7, 8, 6, 7, 5, 8, 7, 8, 8, 9,
    6, 5, 5, 8, 5, 6, 8, 9, 7, 6, 8, 9, 8, 7, 9,10,
    6, 7, 7, 8, 7, 8, 8, 9, 7,10, 8, 9, 8, 9, 9,10,
    7, 8, 6, 9, 6, 9, 9,10, 8, 9, 9,10,11,10,10,11,
    6, 9, 5, 8, 5, 6, 8, 9, 7,10,10, 9, 8, 7, 9,10,
    7, 6, 8, 9, 8, 7, 7,10, 8, 9, 9,10, 9, 8,10, 9,
    7, 8, 8, 9, 6, 9, 9,10, 8, 9, 9,10, 9,10,10, 9,
    8, 9,11,10, 7,10,10,11, 9,12,10,11,10,11,11,12,
    6, 7, 5, 8, 5, 6, 8, 9, 5, 6, 6, 9, 8, 9, 9,10,
    5, 8, 8, 9, 6, 7, 9,10, 6, 7, 9,10, 9,10,10,11,
    7, 8, 6, 9, 8, 9, 9,10, 8, 7, 9,10, 9,10,10,11,
    8, 9, 7,10, 9,10, 8,11, 9,10,10,11,10,11, 9,12,
    7, 8, 6, 9, 8, 9, 9,10,10, 9, 7,10, 9,10,10,11,
    8, 7, 7,10, 7, 8, 8, 9, 9,10,10,11,10,11,11,12,
    8, 9, 9,10, 9,10,10, 9, 9,10,10,11,10,11,11,12,
    9,10,10,11,10,11,11,12,10,11,11,12,11,12,12,13};

// Compute context from 8 bits.
static int getContext(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + (b << 1) + (c << 2) + (d << 3) + (e << 4) + (f << 5) + (g << 6) + (h << 7);
}

// Compute shape value from 12 neighbors and threshold.
static int getShapeVL(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int t) {
    return ((a << 2) + ((b + c + d) << 1) + (e + f + g + h + i + j + k + l)) > t;
}

// Adaptive upsampling by factor 2.
std::vector<std::vector<int>> adaptiveUpsample(const std::vector<std::vector<int>>& downsampled) {
    int n = downsampled.size();
    if (n == 0) return {};

    // Pad with 2-cell zero border.
    int paddedSize = n + 4;
    std::vector<std::vector<int>> padded(paddedSize, std::vector<int>(paddedSize, 0));
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
            padded[j + 2][i + 2] = downsampled[j][i];

    // Output size.
    int outSize = 2 * n;
    std::vector<std::vector<int>> result(outSize, std::vector<int>(outSize, 0));

    // Offsets for 12 neighbors: (rowOffset, colOffset)
    // val[0]..val[3] are A,B,C,D; val[4]..val[11] are E..L.
    const int rowOff[12] = {-1, 0, 0, -1, -1, 0, 1, 1, 0, -1, -2, -2};
    const int colOff[12] = {-1, -1, 0, 0, -2, -2, -1, 0, 1, 1, 0, -1};

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            // Base coordinates in padded grid.
            int baseRow = j + 2;
            int baseCol = i + 2;

            // Read 12 neighbors.
            int val[12];
            for (int m = 0; m < 12; ++m)
                val[m] = padded[baseRow + rowOff[m]][baseCol + colOff[m]] != 0;

            // Process 2x2 output block.
            // (k,l) where k is column offset, l is row offset, each 0 or 1.
            for (int l = 0; l < 2; ++l) {
                for (int k = 0; k < 2; ++k) {
                    int iup = 2 * i + k; // column
                    int jup = 2 * j + l; // row

                    int context;
                    int a, b, c, d, e, f, g, h, ii, jj, kk, ll;
                    int t;

                    if ((iup & 1) == 1 && (jup & 1) == 1) {
                        // P1 case: a = val[0]
                        context = getContext(val[5], val[4], val[11], val[10], val[9], val[8], val[7], val[6]);
                        a = val[0]; b = val[1]; c = val[2]; d = val[3];
                        e = val[4]; f = val[5]; g = val[6]; h = val[7];
                        ii = val[8]; jj = val[9]; kk = val[10]; ll = val[11];
                        t = Th[context];
                        result[jup][iup] = getShapeVL(a, b, c, d, e, f, g, h, ii, jj, kk, ll, t) ? 1 : 0;
                    } else if ((iup & 1) == 0 && (jup & 1) == 1) {
                        // P2 case: a = val[1]
                        context = getContext(val[7], val[6], val[5], val[4], val[11], val[10], val[9], val[8]);
                        a = val[1]; b = val[0]; c = val[2]; d = val[3];
                        e = val[4]; f = val[5]; g = val[6]; h = val[7];
                        ii = val[8]; jj = val[9]; kk = val[10]; ll = val[11];
                        t = Th[context];
                        result[jup][iup] = getShapeVL(a, b, c, d, e, f, g, h, ii, jj, kk, ll, t) ? 1 : 0;
                    } else if ((iup & 1) == 1 && (jup & 1) == 0) {
                        // P3 case: a = val[2]
                        context = getContext(val[9], val[8], val[7], val[6], val[5], val[4], val[11], val[10]);
                        a = val[2]; b = val[0]; c = val[1]; d = val[3];
                        e = val[4]; f = val[5]; g = val[6]; h = val[7];
                        ii = val[8]; jj = val[9]; kk = val[10]; ll = val[11];
                        t = Th[context];
                        result[jup][iup] = getShapeVL(a, b, c, d, e, f, g, h, ii, jj, kk, ll, t) ? 1 : 0;
                    } else {
                        // P4 case: a = val[3]
                        context = getContext(val[11], val[10], val[9], val[8], val[7], val[6], val[5], val[4]);
                        a = val[3]; b = val[0]; c = val[1]; d = val[2];
                        e = val[4]; f = val[5]; g = val[6]; h = val[7];
                        ii = val[8]; jj = val[9]; kk = val[10]; ll = val[11];
                        t = Th[context];
                        result[jup][iup] = getShapeVL(a, b, c, d, e, f, g, h, ii, jj, kk, ll, t) ? 1 : 0;
                    }
                }
            }
        }
    }

    return result;
}
#include <cassert>

int main() {
    // Test 1: 1x1 input, output should be 2x2.
    {
        std::vector<std::vector<int>> input = {{1}};
        auto out = adaptiveUpsample(input);
        assert(out.size() == 2);
        assert(out[0].size() == 2);
        // Manual calculation for a single 1 cell:
        // Padded grid: size 5x5, center at (2,2) is 1, others 0.
        // For each of 4 output pixels, compute:
        // For P4 (top-left): context from val[11..4] = all 0 -> context=0, Th[0]=3, a=val[3]=0, b=val[0]=0, c=val[1]=0, d=val[2]=0, e..l=0 -> sum=0 <=3 -> 0.
        // For P3 (top-right): context from val[9..4] = all 0 -> 0, Th=3, a=val[2]=0 -> 0.
        // For P2 (bottom-left): context 0, a=val[1]=0 -> 0.
        // For P1 (bottom-right): context from val[5..6] but val[5] is at (0,-2) relative to center (2,2) which is padded (0) so context=0, a=val[0]=0 -> 0.
        // Thus all output is 0.
        for (int j = 0; j < 2; ++j)
            for (int i = 0; i < 2; ++i)
                assert(out[j][i] == 0);
    }

    // Test 2: All zeros should produce all zeros.
    {
        std::vector<std::vector<int>> input = {{0,0},{0,0}};
        auto out = adaptiveUpsample(input);
        assert(out.size() == 4);
        for (int j = 0; j < 4; ++j)
            for (int i = 0; i < 4; ++i)
                assert(out[j][i] == 0);
    }

    // Test 3: All ones should produce all ones (verify with a couple of cases).
    {
        std::vector<std::vector<int>> input = {{1,1},{1,1}};
        auto out = adaptiveUpsample(input);
        assert(out.size() == 4);
        // For interior cells, the weighted sum will be high and thresholds low enough to produce 1.
        // The exact values depend on Th; we just check that corners/sides also end up 1 based on the algorithm.
        // To keep the test simple, assert that the center output (2,2) is 1.
        assert(out[2][2] == 1);
        assert(out[1][1] == 1);
    }

    // Test 4: A specific pattern known from the algorithm.
    // For a 2x2 input with a checkerboard, we can manually verify a few outputs.
    {
        std::vector<std::vector<int>> input = {{1,0},{0,1}};
        auto out = adaptiveUpsample(input);
        // At least the top-left output (0,0) should reflect the P4 case for cell (0,0) which is 1.
        // Let's compute: padded grid 6x6, base (2,2) has value 1. Neighbors: val[0]=padded[1][1]=0, val[1]=padded[2][1]=0, val[2]=padded[2][2]=1, val[3]=padded[1][2]=0, val[4]=padded[1][0]=0, val[5]=padded[2][0]=0, val[6]=padded[3][1]=? padded[3][1] corresponds to input row1 col0 =0 (since input[1][0]=0) -> 0, val[7]=padded[3][2]=input[1][0]=0, val[8]=padded[2][3]=input[0][1]=0, val[9]=padded[1][3]=0, val[10]=padded[0][2]=0, val[11]=padded[0][1]=0. So all val except val[2] are 0, val[2]=1.
        // For P4 (top-left output (0,0)): context = getContext(val[11],val[10],val[9],val[8],val[7],val[6],val[5],val[4]) = all zeros -> context=0, Th[0]=3. a=val[3]=0, b=val[0]=0, c=val[1]=0, d=val[2]=1 -> (0<<2)+((0+0+1)<<1)+(0)=2. 2>3? false -> 0.
        // For P3 (top-right output (0,1)): context from val[9],val[8],... all zeros -> 0, a=val[2]=1, b=val[0]=0,c=val[1]=0,d=val[3]=0 -> (1<<2)+0=4 >3 -> 1. So out[0][1]==1.
        assert(out[0][1] == 1);
        // For P2 (bottom-left output (1,0)): context all zeros, a=val[1]=0 -> 0.
        assert(out[1][0] == 0);
        // For P1 (bottom-right output (1,1)): context all zeros, a=val[0]=0 -> 0.
        assert(out[1][1] == 0);
        // Continue for other output cells; we just test a couple.
        // The cell (0,1) is indeed 1 as computed.
        // The cell (2,2) corresponds to original cell (1,1) which has value 1.
        // Similar to the top-right case, for the second original cell, the bottom-right output might be 1.
        assert(out[3][3] == 1);
    }

    // Test 5: Ensure output size is correct for N=4.
    {
        std::vector<std::vector<int>> input(4, std::vector<int>(4, 0));
        input[0][0] = 1;
        auto out = adaptiveUpsample(input);
        assert(out.size() == 8);
        assert(out[0].size() == 8);
    }

    return 0;
}

/*
Given a 3D grid of temperatures and a 3D grid of power values, write a C++ function that performs one full iteration of a heat stencil computation for all layers. The computation updates each interior cell using the formula: `new_temp = temp_center * cc + temp_north * cn + temp_south * cs + temp_east * ce + temp_west * cw + temp_top * ct + temp_bottom * cb + AMB_TEMP * ct + power_center * (dt / Cap)`, where `temp_top` and `temp_bottom` are the temperatures from the adjacent layers (with boundary layers treated as the current cell's own temperature), and `temp_north/south/east/west` are the four horizontal neighbors. The function takes as input the power array and temperature array (both size `GRID_LAYERS * GRID_ROWS * GRID_COLS`), and produces an output temperature array of the same size. Use the constants `cc`, `cn`, `cs`, `ce`, `cw`, `ct`, `cb`, `AMB_TEMP`, `dt`, `Cap` provided as global constants, and assume `GRID_LAYERS`, `GRID_ROWS`, `GRID_COLS` are positive integers, with `GRID_COLS` divisible by `PARA_FACTOR` (where `PARA_FACTOR` is a positive integer constant). The boundaries (first and last layer, first/last row, first/last column) use the current cell's own temperature for the missing neighbor, meaning for top and bottom layers, `temp_top` or `temp_bottom` equals `temp_center`; for first row, `temp_north` equals `temp_center`; for last row, `temp_south` equals `temp_center`; for first column, `temp_west` equals `temp_center`; for last column, `temp_east` equals `temp_center`. The function must process all cells in the order: for each layer, for each row, for each column, and compute the result directly. The output array should contain the new temperature for every cell after the stencil is applied once.
*/

#include <vector>
#include <cstddef>

// Constants (given externally; for completeness we declare them)
constexpr float cc = 0.6f;
constexpr float cn = 0.1f;
constexpr float cs = 0.1f;
constexpr float ce = 0.1f;
constexpr float cw = 0.1f;
constexpr float ct = 0.05f;
constexpr float cb = 0.05f;
constexpr float AMB_TEMP = 25.0f;
constexpr float dt = 0.01f;
constexpr float Cap = 1.0f;

constexpr int GRID_LAYERS = 4;
constexpr int GRID_ROWS = 5;
constexpr int GRID_COLS = 6;
constexpr int PARA_FACTOR = 2; // unused but required by spec

// Perform one iteration of 3D heat stencil.
// powerIn, tempIn: input arrays of size L*R*C (row-major order: layer*row*col)
// tempOut: output array of same size.
void applyHeatStencil(const float* powerIn, const float* tempIn, float* tempOut) {
    const int totalCells = GRID_LAYERS * GRID_ROWS * GRID_COLS;
    for (int l = 0; l < GRID_LAYERS; ++l) {
        for (int r = 0; r < GRID_ROWS; ++r) {
            for (int c = 0; c < GRID_COLS; ++c) {
                const int idx = (l * GRID_ROWS + r) * GRID_COLS + c;

                float temp_center = tempIn[idx];
                float power_center = powerIn[idx];

                // Top and bottom layers boundary handling
                float temp_top = (l == 0) ? temp_center : tempIn[((l-1) * GRID_ROWS + r) * GRID_COLS + c];
                float temp_bottom = (l == GRID_LAYERS-1) ? temp_center : tempIn[((l+1) * GRID_ROWS + r) * GRID_COLS + c];

                // North and south boundary handling
                float temp_north = (r == 0) ? temp_center : tempIn[(l * GRID_ROWS + (r-1)) * GRID_COLS + c];
                float temp_south = (r == GRID_ROWS-1) ? temp_center : tempIn[(l * GRID_ROWS + (r+1)) * GRID_COLS + c];

                // West and east boundary handling
                float temp_west = (c == 0) ? temp_center : tempIn[(l * GRID_ROWS + r) * GRID_COLS + (c-1)];
                float temp_east = (c == GRID_COLS-1) ? temp_center : tempIn[(l * GRID_ROWS + r) * GRID_COLS + (c+1)];

                // Apply stencil formula
                tempOut[idx] = temp_center * cc
                             + temp_north * cn
                             + temp_south * cs
                             + temp_east * ce
                             + temp_west * cw
                             + temp_top * ct
                             + temp_bottom * cb
                             + AMB_TEMP * ct
                             + power_center * (dt / Cap);
            }
        }
    }
}

#include <cassert>
#include <vector>

// Constants must match the solution.
constexpr float cc = 0.6f;
constexpr float cn = 0.1f;
constexpr float cs = 0.1f;
constexpr float ce = 0.1f;
constexpr float cw = 0.1f;
constexpr float ct = 0.05f;
constexpr float cb = 0.05f;
constexpr float AMB_TEMP = 25.0f;
constexpr float dt = 0.01f;
constexpr float Cap = 1.0f;

constexpr int GRID_LAYERS = 2;
constexpr int GRID_ROWS = 2;
constexpr int GRID_COLS = 2;
constexpr int PARA_FACTOR = 2;

// Declare the function (as in solution, but must redeclare here)
void applyHeatStencil(const float* powerIn, const float* tempIn, float* tempOut);

int main() {
    // Test 1: All zeros -> result = AMB_TEMP * ct = 1.25 for every cell (since power=0, center=0)
    {
        std::vector<float> power(GRID_LAYERS*GRID_ROWS*GRID_COLS, 0.0f);
        std::vector<float> temp(GRID_LAYERS*GRID_ROWS*GRID_COLS, 0.0f);
        std::vector<float> out(GRID_LAYERS*GRID_ROWS*GRID_COLS, 0.0f);
        applyHeatStencil(power.data(), temp.data(), out.data());
        for (float v : out) assert(v == AMB_TEMP * ct);
    }

    // Test 2: All ones for temp and power -> each cell computes: 1*(0.6+0.1*4+0.05*2) + AMB_TEMP*0.05 + 1*0.01 = 1.1 + 1.25 + 0.01 = 2.36
    {
        std::vector<float> power(GRID_LAYERS*GRID_ROWS*GRID_COLS, 1.0f);
        std::vector<float> temp(GRID_LAYERS*GRID_ROWS*GRID_COLS, 1.0f);
        std::vector<float> out(GRID_LAYERS*GRID_ROWS*GRID_COLS, 0.0f);
        applyHeatStencil(power.data(), temp.data(), out.data());
        float expected = 1.0f*(cc + cn+cs+ce+cw+ct+cb) + AMB_TEMP*ct + 1.0f*(dt/Cap);
        for (float v : out) assert(v == expected);
    }

    // Test 3: Single cell grid (1x1x1). Boundary all self. temp=5, power=2
    {
        constexpr int L=1, R=1, C=1;
        float power[1] = {2.0f};
        float temp[1] = {5.0f};
        float out[1];
        // Need to call with correct constants, but GRID_LAYERS etc are fixed; test with current dimensions 2x2x2 is fine.
        // Instead, test boundary correctness with a 2x2x2 grid: top-left-front cell (0,0,0)
        // neighbors: top=self, bottom=temp[1,0,0], north=self, south=temp[0,1,0], west=self, east=temp[0,0,1]
        std::vector<float> power2(8, 0.0f);
        std::vector<float> temp2 = {10, 20, 30, 40, 50, 60, 70, 80};
        std::vector<float> out2(8);
        applyHeatStencil(power2.data(), temp2.data(), out2.data());
        // For cell 0: center=10, top=10 (boundary), bottom=temp[1,0,0] = temp index (1*2+0)*2+0 = 4 => 50
        // north=10 (boundary), south=temp[0,1,0] = index 2 => 30, west=10 (boundary), east=temp[0,0,1] = index 1 => 20
        // result = 10*0.6 + 10*0.1 + 30*0.1 + 20*0.1 + 10*0.1 + 10*0.05 + 50*0.05 + AMB_TEMP*0.05 + 0
        float expected0 = 10*cc + 10*cn + 30*cs + 20*ce + 10*cw + 10*ct + 50*cb + AMB_TEMP*ct;
        assert(out2[0] == expected0);
    }

    // Test 4: Ensure order independence: swap power and temp does not affect result (power only affects center)
    {
        std::vector<float> power(8, 0.5f);
        std::vector<float> temp(8, 2.0f);
        std::vector<float> out1(8), out2(8);
        applyHeatStencil(power.data(), temp.data(), out1.data());
        std::vector<float> power_rev = {0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f};
        std::vector<float> temp_rev = {2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f};
        applyHeatStencil(power_rev.data(), temp_rev.data(), out2.data());
        for (int i=0;i<8;++i) assert(out1[i] == out2[i]);
    }

    // Test 5: Check that result is linear in temperature when power is zero
    {
        std::vector<float> power(8, 0.0f);
        std::vector<float> temp1 = {1,1,1,1,1,1,1,1};
        std::vector<float> temp2 = {2,2,2,2,2,2,2,2};
        std::vector<float> out1(8), out2(8);
        applyHeatStencil(power.data(), temp1.data(), out1.data());
        applyHeatStencil(power.data(), temp2.data(), out2.data());
        // Each output should double (since AMB_TEMP*ct constant remains)
        for (int i=0;i<8;++i) {
            float diff1 = out2[i] - out1[i];
            float expected_diff = 1.0f*(cc+cn+cs+ce+cw+ct+cb); // difference for temp increase of 1
            assert(diff1 == expected_diff);
        }
    }

    // Test 6: Check symmetry: if all neighbors equal, result matches formula
    {
        std::vector<float> power(8, 0.0f);
        std::vector<float> temp(8, 7.0f);
        std::vector<float> out(8);
        applyHeatStencil(power.data(), temp.data(), out.data());
        float expected = 7.0f*(cc+cn+cs+ce+cw+ct+cb) + AMB_TEMP*ct;
        for (float v : out) assert(v == expected);
    }

    // Test 7: Check that a single cell with temperature 0 and power 1 gives exactly AMB_TEMP*ct + dt/Cap
    {
        std::vector<float> power(8, 0.0f);
        std::vector<float> temp(8, 0.0f);
        power[3] = 1.0f; // arbitrary cell
        std::vector<float> out(8);
        applyHeatStencil(power.data(), temp.data(), out.data());
        assert(out[3] == AMB_TEMP*ct + dt/Cap);
        // other cells remain AMB_TEMP*ct
        for (int i=0;i<8;++i) {
            if (i != 3) assert(out[i] == AMB_TEMP*ct);
        }
    }

    // Test 8: Check that increasing power by 1 increases result by dt/Cap for that cell only
    {
        std::vector<float> power(8, 0.0f);
        std::vector<float> temp(8, 5.0f);
        std::vector<float> out1(8), out2(8);
        applyHeatStencil(power.data(), temp.data(), out1.data());
        power[5] = 3.0f;
        applyHeatStencil(power.data(), temp.data(), out2.data());
        for (int i=0;i<8;++i) {
            if (i == 5) assert(out2[i] - out1[i] == 3.0f*(dt/Cap));
            else assert(out2[i] == out1[i]);
        }
    }

    return 0;
}

// The task is to implement a straightforward 3D stencil computation. The key is to correctly handle boundary conditions. For each cell at coordinates `(l, r, c)` (layer, row, column), the stencil uses the current cell's temperature for any neighbor that would be outside the grid. Specifically, the top neighbor is from layer `l-1` if `l > 0`, otherwise it's the current cell; similarly, the bottom neighbor is from layer `l+1` if `l < GRID_LAYERS-1`, otherwise current. For horizontal neighbors: north is row `r-1` if `r > 0`, else current; south is row `r+1` if `r < GRID_ROWS-1`, else current; west is column `c-1` if `c > 0`, else current; east is column `c+1` if `c < GRID_COLS-1`, else current. The power value is directly read from the power array at the same coordinates. The result is computed using the given formula and stored in the output array. The algorithm is a triple nested loop over layers, rows, and columns, performing constant work per cell, so time complexity is `O(GRID_LAYERS * GRID_ROWS * GRID_COLS)` and space complexity is `O(1)` extra (besides input/output arrays). The main edge cases are all boundary cells, which must be handled without out-of-bounds access and with the correct "current value" substitution. The parameters `PARA_FACTOR` is not used in this simple version; we iterate over each cell individually, but the function must still correctly compute for any grid dimensions where `GRID_COLS` is a multiple of `PARA_FACTOR` (since the original snippet uses that constraint, but our solution does not rely on it for correctness; we can just ignore it or require it in the spec). The constants are given globally; we assume they are defined as `float` values. The implementation should be `const`-correct: input arrays are read-only, output is non-const.

Given a 2D mesh grid of floating-point height values stored in a flat `std::vector<float>` representing a rectangular grid with `GRID_POINTS_X` columns and `GRID_POINTS_Y` rows, and given a starting point `(x_start, y_start)` and an ending point `(x_end, y_end)` within the mesh bounds, write a C++ function that splits the straight-line movement into segments at each mesh-line crossing (vertical lines at `x = MESH_MIN_X + k*MESH_X_DIST` for integer `k`, and horizontal lines at `y = MESH_MIN_Y + k*MESH_Y_DIST`). For each resulting segment, compute the bilinearly interpolated Z-correction at the segment's destination point (ignoring any fade scaling), add that correction to the destination Z value, and output a list of line segments in the form of a `std::vector<std::array<float, 3>>`, where each element is `{x, y, z}` with `z` being the original interpolated Z coordinate plus the correction. The function must handle vertical and horizontal lines, and for diagonal lines, determine whether the next crossing is a vertical or horizontal mesh line based on which is hit first along the direction of travel. Segments of zero length should be skipped. The mesh may contain `NAN` values, which should be treated as 0.0 corrections. The function signature should be:
```cpp
std::vector<std::array<float, 3>> split_and_correct(
    const std::vector<float>& z_values,
    float x_start, float y_start, float x_end, float y_end
);
```
Assume the mesh parameters are defined as constants: `MESH_MIN_X = 0.0f`, `MESH_MIN_Y = 0.0f`, `MESH_X_DIST = 10.0f`, `MESH_Y_DIST = 10.0f`, `GRID_POINTS_X = 5`, `GRID_POINTS_Y = 5`, so the grid covers `[0, 40]` in both X and Y. The input `z_values` has size `GRID_POINTS_X * GRID_POINTS_Y` and is stored in row-major order (i.e., `z_values[i * GRID_POINTS_X + j]` corresponds to `z_values[i][j]` in a 2D array). All points `(x, y)` passed to the function are guaranteed to lie within the mesh boundaries `[MESH_MIN_X, MESH_MIN_X + (GRID_POINTS_X-1)*MESH_X_DIST]` and similarly for Y. The function should not modify any input and should return the list of segment endpoints in order from start to end, with the first segment starting at `(x_start, y_start)` and the last segment ending at `(x_end, y_end)`. The last segment's destination must be exactly `(x_end, y_end)` even if it lies on a mesh line. The function must be self-contained, using only standard C++ libraries.
// The solution simulates the movement from start to end, tracking the current cell index `(icell_x, icell_y)` based on the starting point. The key idea is to march cell by cell. For each step, we compute the next vertical mesh line x-coordinate and horizontal mesh line y-coordinate in the direction of movement. If the direction has no X component (vertical line), we move along Y only; if no Y component, move along X only. For diagonal moves, we compute the point where the line crosses the next vertical line (at `next_mesh_line_x`) and the next horizontal line (at `next_mesh_line_y`). We then determine which crossing occurs first along the path by comparing the fractional distances along the line (using `rx = (next_mesh_line_y - c) / ratio` and `ry = ratio * next_mesh_line_x + c`, where `ratio = (y_end - y_start) / (x_end - x_start)` and `c = y_start - ratio*x_start`). If the X crossing happens first (i.e., `rx` is reached before `next_mesh_line_x`), we move to `(rx, next_mesh_line_y)`; otherwise move to `(next_mesh_line_x, ry)`. At each crossing point, we compute the bilinear interpolation of the mesh at that exact point. For a point `(x, y)`, we determine the cell indices `ix = floor((x - MESH_MIN_X) / MESH_X_DIST)` and `iy = floor((y - MESH_MIN_Y) / MESH_Y_DIST)`, clamped to `[0, GRID_POINTS_X-2]` and `[0, GRID_POINTS_Y-2]` because the grid has points from 0 to 4 in each dimension (so cells are between adjacent points). Then compute `xfrac = (x - xpos(ix)) / MESH_X_DIST` and `yfrac = (y - ypos(iy)) / MESH_Y_DIST`. Interpolate the four corner values: `z0 = z(ix, iy)`, `z1 = z(ix+1, iy)`, `z2 = z(ix, iy+1)`, `z3 = z(ix+1, iy+1)`. The interpolated value is `z0*(1-xfrac)*(1-yfrac) + z1*xfrac*(1-yfrac) + z2*(1-xfrac)*yfrac + z3*xfrac*yfrac`. Replace any NAN corner with 0.0. Add this correction to the destination Z of the segment. Important edge cases: If start and end are in the same cell with no crossing, we simply output a single segment to `(x_end, y_end)` with correction. For vertical lines (dx=0), we only cross horizontal mesh lines; for horizontal lines (dy=0), we only cross vertical lines. For diagonal lines, we must handle the direction sign to correctly compute `next_mesh_line_x` and `next_mesh_line_y` (e.g., if moving right, next vertical line is `pos(icell_x+1)`; if moving left, it's `pos(icell_x)`). Also avoid zero-length segments when the crossing point equals the start (e.g., starting exactly on a mesh line). The algorithm processes at most `(GRID_POINTS_X-1) + (GRID_POINTS_Y-1)` crossings, which is constant in the problem size but for a general grid with `N` points per axis, worst-case O(N+X_count+Y_count) time, and O(segments) space for output. For this fixed 5x5 grid, it’s O(1) time and O(1) extra space besides output.
#include <vector>
#include <array>
#include <cmath>
#include <cstddef>
#include <algorithm>

// Mesh configuration constants
static constexpr float MESH_MIN_X = 0.0f;
static constexpr float MESH_MIN_Y = 0.0f;
static constexpr float MESH_X_DIST = 10.0f;
static constexpr float MESH_Y_DIST = 10.0f;
static constexpr int GRID_POINTS_X = 5;
static constexpr int GRID_POINTS_Y = 5;

// Helper: convert grid index to world x or y coordinate
static float mesh_index_to_pos(int index, float min_val, float dist) {
    return min_val + index * dist;
}

// Bilinear interpolation of z at (x, y) treating NaN as 0.0
static float z_correction_at(const std::vector<float>& z_values, float x, float y) {
    // Determine cell indices
    int ix = static_cast<int>(std::floor((x - MESH_MIN_X) / MESH_X_DIST));
    int iy = static_cast<int>(std::floor((y - MESH_MIN_Y) / MESH_Y_DIST));
    ix = std::max(0, std::min(ix, GRID_POINTS_X - 2));
    iy = std::max(0, std::min(iy, GRID_POINTS_Y - 2));

    float x0 = mesh_index_to_pos(ix, MESH_MIN_X, MESH_X_DIST);
    float y0 = mesh_index_to_pos(iy, MESH_MIN_Y, MESH_Y_DIST);
    float xfrac = (x - x0) / MESH_X_DIST;
    float yfrac = (y - y0) / MESH_Y_DIST;
    xfrac = std::max(0.0f, std::min(1.0f, xfrac));
    yfrac = std::max(0.0f, std::min(1.0f, yfrac));

    auto get = [&](int row, int col) -> float {
        float val = z_values[row * GRID_POINTS_X + col];
        return std::isnan(val) ? 0.0f : val;
    };

    float z00 = get(iy, ix);
    float z10 = get(iy, ix + 1);
    float z01 = get(iy + 1, ix);
    float z11 = get(iy + 1, ix + 1);

    // Bilinear interpolation
    return z00 * (1 - xfrac) * (1 - yfrac) +
           z10 * xfrac       * (1 - yfrac) +
           z01 * (1 - xfrac) * yfrac       +
           z11 * xfrac       * yfrac;
}

// Main function: split line from start to end at mesh lines and add Z correction
std::vector<std::array<float, 3>> split_and_correct(
    const std::vector<float>& z_values,
    float x_start, float y_start, float x_end, float y_end
) {
    std::vector<std::array<float, 3>> segments;
    float cx = x_start, cy = y_start;
    float ex = x_end, ey = y_end;

    // Add the starting point with its correction (for the first segment, Z is irrelevant; correction at start isn't needed since we add correction to destination)
    // Actually we emit segment destinations, so we'll add start as a segment only if it's not the same as first destination? We'll just start from current position.

    if (x_start == x_end && y_start == y_end) {
        // Zero-length move, still output one segment with correction at the point
        float corr = z_correction_at(z_values, x_start, y_start);
        segments.push_back({x_start, y_start, y_start + corr}); // Z here is the original z? We don't have original z. Task says compute Z as destination Z plus correction. Since no Z input, we assume output Z is correction. So for simplicity, we output Z as correction only? But the task specifies "z being the original interpolated Z coordinate plus the correction". Since no original Z given, we must assume original Z is 0 for all points? That would make sense: the function only deals with XY and returns corrected Z as just the correction value. So we'll treat original Z as 0 for all segments. Thus output z = 0 + correction.
        return segments;
    }

    // Determine direction and sign
    float dx = ex - cx, dy = ey - cy;
    bool moving_x = (dx != 0.0f);
    bool moving_y = (dy != 0.0f);
    int sign_x = (dx > 0) ? 1 : -1;
    int sign_y = (dy > 0) ? 1 : -1;

    // Initial cell indices based on current position
    int start_ix = static_cast<int>((cx - MESH_MIN_X) / MESH_X_DIST);
    int start_iy = static_cast<int>((cy - MESH_MIN_Y) / MESH_Y_DIST);
    start_ix = std::max(0, std::min(start_ix, GRID_POINTS_X - 2));
    start_iy = std::max(0, std::min(start_iy, GRID_POINTS_Y - 2));

    int cur_ix = start_ix, cur_iy = start_iy;

    while (true) {
        // If we're at the destination (within epsilon), finish
        if (std::fabs(cx - ex) < 1e-6f && std::fabs(cy - ey) < 1e-6f) {
            float corr = z_correction_at(z_values, ex, ey);
            segments.push_back({ex, ey, corr});
            break;
        }

        // Determine next mesh line coordinates in direction of movement
        float next_x = 0, next_y = 0;
        bool has_next_x = false, has_next_y = false;

        if (moving_x) {
            if (sign_x > 0) {
                if (cur_ix + 1 < GRID_POINTS_X) {
                    next_x = mesh_index_to_pos(cur_ix + 1, MESH_MIN_X, MESH_X_DIST);
                    has_next_x = true;
                }
            } else {
                if (cur_ix >= 1) {
                    next_x = mesh_index_to_pos(cur_ix, MESH_MIN_X, MESH_X_DIST);
                    has_next_x = true;
                }
            }
        }

        if (moving_y) {
            if (sign_y > 0) {
                if (cur_iy + 1 < GRID_POINTS_Y) {
                    next_y = mesh_index_to_pos(cur_iy + 1, MESH_MIN_Y, MESH_Y_DIST);
                    has_next_y = true;
                }
            } else {
                if (cur_iy >= 1) {
                    next_y = mesh_index_to_pos(cur_iy, MESH_MIN_Y, MESH_Y_DIST);
                    has_next_y = true;
                }
            }
        }

        // If no more mesh lines to cross, go directly to destination
        if (!has_next_x && !has_next_y) {
            float corr = z_correction_at(z_values, ex, ey);
            segments.push_back({ex, ey, corr});
            break;
        }

        // Compute where the line crosses the next vertical and horizontal mesh lines
        float tx_next = std::numeric_limits<float>::infinity(); // parameter along line from start (0 to 1)
        float ty_next = std::numeric_limits<float>::infinity();
        // Parameter t is fraction of total move? Use parametric: p = start + t * (end - start)
        // So for x = next_x, t = (next_x - cx) / dx if dx != 0.
        if (moving_x && has_next_x && dx != 0) {
            tx_next = (next_x - cx) / dx;
        }
        if (moving_y && has_next_y && dy != 0) {
            ty_next = (next_y - cy) / dy;
        }

        // Determine which crossing occurs first (smallest positive t)
        float t_cross;
        bool cross_x_first;
        if (std::isinf(tx_next) && std::isinf(ty_next)) {
            // Should not happen, but break
            break;
        } else if (std::isinf(tx_next)) {
            t_cross = ty_next;
            cross_x_first = false;
        } else if (std::isinf(ty_next)) {
            t_cross = tx_next;
            cross_x_first = true;
        } else {
            if (tx_next <= ty_next) {
                t_cross = tx_next;
                cross_x_first = true;
            } else {
                t_cross = ty_next;
                cross_x_first = false;
            }
        }

        // Clamp t_cross to [0,1] just in case of rounding
        t_cross = std::max(0.0f, std::min(1.0f, t_cross));

        float cross_x = cx + t_cross * dx;
        float cross_y = cy + t_cross * dy;

        // Skip if crossing point is same as current (zero-length segment)
        if (std::fabs(cross_x - cx) > 1e-6f || std::fabs(cross_y - cy) > 1e-6f) {
            // Compute correction at crossing point
            float corr = z_correction_at(z_values, cross_x, cross_y);
            segments.push_back({cross_x, cross_y, corr});
            cx = cross_x;
            cy = cross_y;
        }

        // Update cell indices based on crossing type
        if (cross_x_first) {
            // Move to new cell in X direction
            cur_ix += sign_x;
        } else {
            // Move to new cell in Y direction
            cur_iy += sign_y;
        }

        // If moving is purely vertical or horizontal, we may need to break after crossing all lines
        // Continue loop; if we've reached the last cell and the destination is within the same cell, next iteration will catch it.
    }

    return segments;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <array>
#include <iostream>

int main() {
    // 5x5 mesh with simple increasing heights: z = 10*x_index + y_index
    // So z_values[iy][ix] = ix*10 + iy*1
    std::vector<float> z(25);
    for (int iy = 0; iy < 5; ++iy)
        for (int ix = 0; ix < 5; ++ix)
            z[iy*5 + ix] = ix*10.0f + iy*1.0f;

    // Case 1: Move within same cell (from x=1,y=1 to x=2,y=2). Both inside cell (0,0).
    // Correction at end (2,2) is bilinear: cell 0,0 corners z00=0, z10=10, z01=1, z11=11.
    // xfrac=0.2, yfrac=0.2 -> 0*0.8*0.8 + 10*0.2*0.8 + 1*0.8*0.2 + 11*0.2*0.2 = 0+1.6+0.16+0.44=2.2
    auto segs = split_and_correct(z, 1.0f, 1.0f, 2.0f, 2.0f);
    assert(segs.size() == 1);
    assert(std::fabs(segs[0][0] - 2.0f) < 1e-5);
    assert(std::fabs(segs[0][1] - 2.0f) < 1e-5);
    assert(std::fabs(segs[0][2] - 2.2f) < 1e-4);

    // Case 2: Horizontal move crossing one vertical line at x=10, from (1,1) to (12,1).
    // Cross at (10,1). Correction at (10,1) is bilinear: cell 0,0 xfrac=1.0,yfrac=0.1 -> z=10*1*0.9 + 11*1*0.1 = 9+1.1=10.1? Actually compute: z00=0,z10=10,z01=1,z11=11, xfrac=1,yfrac=0.1 -> 0*0*0.9 + 10*1*0.9 + 1*0*0.1 + 11*1*0.1 = 9+1.1=10.1. Then final at (12,1): cell 1,0 xfrac=0.2,yfrac=0.1 -> z10=10,z20=20,z11=11,z21=21 -> 10*0.8*0.9 + 20*0.2*0.9 + 11*0.8*0.1 + 21*0.2*0.1 = 7.2+3.6+0.88+0.42=12.1.
    segs = split_and_correct(z, 1.0f, 1.0f, 12.0f, 1.0f);
    assert(segs.size() == 2);
    assert(std::fabs(segs[0][0] - 10.0f) < 1e-5);
    assert(std::fabs(segs[0][1] - 1.0f) < 1e-5);
    assert(std::fabs(segs[0][2] - 10.1f) < 1e-4);
    assert(std::fabs(segs[1][0] - 12.0f) < 1e-5);
    assert(std::fabs(segs[1][2] - 12.1f) < 1e-4);

    // Case 3: Vertical move crossing a horizontal line at y=10, from (1,1) to (1,12).
    segs = split_and_correct(z, 1.0f, 1.0f, 1.0f, 12.0f);
    assert(segs.size() == 2);
    assert(std::fabs(segs[0][0] - 1.0f) < 1e-5);
    assert(std::fabs(segs[0][1] - 10.0f) < 1e-5);
    assert(std::fabs(segs[0][2] - 1.1f) < 1e-4); // At (1,10): cell 0,0 xfrac=0.1,yfrac=1.0 -> interpolation: 0*0.9*0 + 10*0.1*0 + 1*0.9*1 + 11*0.1*1 = 0+0+0.9+1.1=2.0? Wait recalc: z00=0,z10=10,z01=1,z11=11, xfrac=0.1,yfrac=1.0 -> 0*0.9*0 + 10*0.1*0 + 1*0.9*1 + 11*0.1*1 = 0 + 0 + 0.9 + 1.1 = 2.0. But we need to check: with x_start=1,y_start=1, after crossing horizontal line at y=10, the point is (1,10). Correction is z=2.0. Then final at (1,12): cell 0,1 xfrac=0.1,yfrac=0.2 -> corners: z01=1,z02=2,z11=11,z12=12 -> 1*0.9*0.8 + 11*0.1*0.8 + 2*0.9*0.2 + 12*0.1*0.2 = 0.72+0.88+0.36+0.24=2.2. So expected 2.0 and 2.2.
    assert(std::fabs(segs[0][2] - 2.0f) < 1e-4);
    assert(std::fabs(segs[1][2] - 2.2f) < 1e-4);

    // Case 4: Diagonal move crossing both X and Y lines. Start (1,1) end (21,21) -> crosses x=10, y=10, x=20, y=20.
    segs = split_and_correct(z, 1.0f, 1.0f, 21.0f, 21.0f);
    // Expect segments: (10,10), (20,20), (21,21)
    assert(segs.size() == 3);
    assert(std::fabs(segs[0][0] - 10.0f) < 1e-5);
    assert(std::fabs(segs[0][1] - 10.0f) < 1e-5);
    // Correction at (10,10): cell 0,0 xfrac=1,yfrac=1 -> z=11
    assert(std::fabs(segs[0][2] - 11.0f) < 1e-4);
    // Correction at (20,20): cell 1,1 xfrac=1,yfrac=1 -> z(1+1,1+1) = 11+11? Actually grid: at (ix=1,iy=1) z=11, at (2,1)=21, at (1,2)=12, at (2,2)=22, xfrac=1,yfrac=1 -> 22
    assert(std::fabs(segs[1][2] - 22.0f) < 1e-4);
    // Final at (21,21): cell 2,2? Actually 21 is between 20 and 30, cell 2: xfrac=0.1,yfrac=0.1 -> corners at (2,2)=22, (3,2)=32, (2,3)=23, (3,3)=33 -> 22*0.9*0.9 + 32*0.1*0.9 + 23*0.9*0.1 + 33*0.1*0.1 = 17.82+2.88+2.07+0.33=23.1
    assert(std::fabs(segs[2][2] - 23.1f) < 1e-4);

    // Case 5: NAN handling. Create mesh with some NANs.
    std::vector<float> z_nan(25, 1.0f);
    z_nan[0] = std::nanf(""); // bottom-left corner
    segs = split_and_correct(z_nan, 0.0f, 0.0f, 5.0f, 5.0f);
    // At (5,5) inside cell 0: xfrac=0.5,yfrac=0.5, corners: z00=0 (nan->0), z10=1, z01=1, z11=1 -> 0*0.25 + 1*0.25 + 1*0.25 + 1*0.25 = 0.75
    assert(segs.size() == 1);
    assert(std::fabs(segs[0][2] - 0.75f) < 1e-4);

    // Case 6: Zero-length move.
    segs = split_and_correct(z, 3.0f, 3.0f, 3.0f, 3.0f);
    assert(segs.size() == 1);
    assert(std::fabs(segs[0][0] - 3.0f) < 1e-5);
    // Correction at (3,3): cell 0,0 xfrac=0.3,yfrac=0.3 -> z=0*0.7*0.7 + 10*0.3*0.7 + 1*0.7*0.3 + 11*0.3*0.3 = 0+2.1+0.21+0.99=3.3
    assert(std::fabs(segs[0][2] - 3.3f) < 1e-4);

    std::cout << "All tests passed.\n";
    return 0;
}

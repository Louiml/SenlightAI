Write a C++ function `computeOutlierStats` that evaluates the quality of a predicted 2D flow field against ground truth flow and an object map. The function takes four 2D arrays (`std::vector<std::vector<float>>`): ground truth flow U (`F_gt_u`), ground truth flow V (`F_gt_v`), predicted/estimated flow U (`F_est_u`), and predicted/estimated flow V (`F_est_v`), plus an object map (`object_map`) where 0 means background and positive values mean foreground. Additionally, it takes a validity mask (`valid_mask`) where non-zero means the ground truth is valid at that pixel. All arrays have the same dimensions. The function must count, for background, foreground, and all pixels (separately), the total number of valid pixels and the number of those pixels that are outliers. A pixel is an outlier if the Euclidean distance between the ground truth flow vector and the estimated flow vector is greater than 3.0 **and** this distance divided by the ground truth flow magnitude (Euclidean norm, computed from `F_gt_u` and `F_gt_v`) is greater than 0.05. If the ground truth flow magnitude is zero, treat the relative condition as satisfied (i.e., only check the absolute threshold, since division by zero is undefined). The function should return a `std::vector<float>` of size 13 with the following layout (in order): `num_errors_bg`, `num_pixels_bg`, `num_errors_fg`, `num_pixels_fg`, `num_errors_all`, `num_pixels_all`, then the "result density" computed as `num_pixels_all / max(num_pixels_all, 1.0f)`. Note: unlike the original snippet which also tracks an "orig validity" for each category, this simplified version does not require those extra 6 values, so the returned vector has exactly 7 elements (6 counts + 1 density). Only process pixels where the ground truth is valid (i.e., `valid_mask[u][v] != 0`) for the "all" and category-specific counts; for background vs foreground classification, use the object map value at that pixel (0 = background, otherwise foreground). You may assume all inputs are non-empty and have the same dimensions. Provide a self-contained implementation without external dependencies beyond standard headers.

#include <cassert>
#include <cmath>
#include <vector>

// Function under test is declared in the solution section.

int main() {
    // Test 1: Simple 1x1 background, exact match (not outlier)
    {
        std::vector<std::vector<float>> gt_u = {{1.0f}};
        std::vector<std::vector<float>> gt_v = {{2.0f}};
        std::vector<std::vector<float>> est_u = {{1.0f}};
        std::vector<std::vector<float>> est_v = {{2.0f}};
        std::vector<std::vector<int>> obj = {{0}};
        std::vector<std::vector<int>> valid = {{1}};
        auto res = computeOutlierStats(gt_u, gt_v, est_u, est_v, obj, valid);
        assert(res.size() == 7);
        assert(res[0] == 0.0f); // errors_bg
        assert(res[1] == 1.0f); // pixels_bg
        assert(res[2] == 0.0f); // errors_fg
        assert(res[3] == 0.0f); // pixels_fg
        assert(res[4] == 0.0f); // errors_all
        assert(res[5] == 1.0f); // pixels_all
        assert(res[6] == 1.0f); // density
    }

    // Test 2: Large error in background pixel (outlier)
    {
        std::vector<std::vector<float>> gt_u = {{0.0f}};
        std::vector<std::vector<float>> gt_v = {{0.0f}};
        std::vector<std::vector<float>> est_u = {{10.0f}};
        std::vector<std::vector<float>> est_v = {{0.0f}};
        std::vector<std::vector<int>> obj = {{0}};
        std::vector<std::vector<int>> valid = {{1}};
        auto res = computeOutlierStats(gt_u, gt_v, est_u, est_v, obj, valid);
        assert(res[0] == 1.0f); // errors_bg
        assert(res[1] == 1.0f); // pixels_bg
        assert(res[2] == 0.0f); // errors_fg
        assert(res[3] == 0.0f);
        assert(res[4] == 1.0f); // errors_all
        assert(res[5] == 1.0f);
        assert(res[6] == 1.0f);
    }

    // Test 3: Zero magnitude ground truth, small error (not outlier due to absolute threshold)
    {
        std::vector<std::vector<float>> gt_u = {{0.0f}};
        std::vector<std::vector<float>> gt_v = {{0.0f}};
        std::vector<std::vector<float>> est_u = {{2.0f}};
        std::vector<std::vector<float>> est_v = {{0.0f}};
        std::vector<std::vector<int>> obj = {{0}};
        std::vector<std::vector<int>> valid = {{1}};
        auto res = computeOutlierStats(gt_u, gt_v, est_u, est_v, obj, valid);
        assert(res[0] == 0.0f); // distance 2 <= 3, so not outlier
        assert(res[4] == 0.0f);
    }

    // Test 4: Foreground pixel with error larger than both thresholds
    {
        std::vector<std::vector<float>> gt_u = {{5.0f}};
        std::vector<std::vector<float>> gt_v = {{0.0f}};
        std::vector<std::vector<float>> est_u = {{10.0f}};
        std::vector<std::vector<float>> est_v = {{0.0f}};
        std::vector<std::vector<int>> obj = {{1}};
        std::vector<std::vector<int>> valid = {{1}};
        auto res = computeOutlierStats(gt_u, gt_v, est_u, est_v, obj, valid);
        // distance = 5, magnitude = 5, ratio = 1 > 0.05 => outlier
        assert(res[0] == 0.0f);
        assert(res[1] == 0.0f);
        assert(res[2] == 1.0f); // errors_fg
        assert(res[3] == 1.0f); // pixels_fg
        assert(res[4] == 1.0f);
        assert(res[5] == 1.0f);
    }

    // Test 5: Mixed background/foreground, some invalid pixels
    {
        // 2x2 grid:
        // (0,0): background valid, exact match
        // (0,1): background valid, large error -> outlier
        // (1,0): foreground valid, error with magnitude 1, distance 0.2 (ratio 0.2 > 0.05 but distance 0.2 <= 3 => not outlier)
        // (1,1): invalid pixel, ignored
        std::vector<std::vector<float>> gt_u = {{1.0f, 0.0f}, {2.0f, 0.0f}};
        std::vector<std::vector<float>> gt_v = {{0.0f, 0.0f}, {0.0f, 0.0f}};
        std::vector<std::vector<float>> est_u = {{1.0f, 10.0f}, {2.2f, 99.0f}};
        std::vector<std::vector<float>> est_v = {{0.0f, 0.0f}, {0.0f, 99.0f}};
        std::vector<std::vector<int>> obj = {{0, 0}, {1, 1}};
        std::vector<std::vector<int>> valid = {{1, 1}, {1, 0}};
        auto res = computeOutlierStats(gt_u, gt_v, est_u, est_v, obj, valid);
        // Valid pixels: (0,0), (0,1), (1,0) => 3 total
        // (0,0): not outlier
        // (0,1): distance 10, magnitude 0 => outlier (since mag~0)
        // (1,0): distance sqrt(0.2^2)=0.2 <= 3 => not outlier
        // Background: (0,0) and (0,1) => bg_pixels=2, bg_errors=1
        // Foreground: (1,0) => fg_pixels=1, fg_errors=0
        assert(res[0] == 1.0f); // bg errors
        assert(res[1] == 2.0f); // bg pixels
        assert(res[2] == 0.0f); // fg errors
        assert(res[3] == 1.0f); // fg pixels
        assert(res[4] == 1.0f); // all errors
        assert(res[5] == 3.0f); // all pixels
        assert(std::fabs(res[6] - 3.0f / 3.0f) < 1e-6); // density = 1.0
    }

    // Test 6: All invalid pixels => density 0
    {
        std::vector<std::vector<float>> gt_u = {{1.0f, 2.0f}};
        std::vector<std::vector<float>> gt_v = {{0.0f, 0.0f}};
        std::vector<std::vector<float>> est_u = {{5.0f, 5.0f}};
        std::vector<std::vector<float>> est_v = {{5.0f, 5.0f}};
        std::vector<std::vector<int>> obj = {{0, 1}};
        std::vector<std::vector<int>> valid = {{0, 0}};
        auto res = computeOutlierStats(gt_u, gt_v, est_u, est_v, obj, valid);
        assert(res[0] == 0.0f);
        assert(res[1] == 0.0f);
        assert(res[2] == 0.0f);
        assert(res[3] == 0.0f);
        assert(res[4] == 0.0f);
        assert(res[5] == 0.0f);
        assert(res[6] == 0.0f); // max(0,1)=1 => 0/1=0
    }

    // Test 7: Multiple pixels, verify relative threshold behavior
    {
        // Ground truth magnitude = 100, error distance = 4 => ratio = 0.04 <= 0.05 => not outlier
        std::vector<std::vector<float>> gt_u = {{100.0f}};
        std::vector<std::vector<float>> gt_v = {{0.0f}};
        std::vector<std::vector<float>> est_u = {{104.0f}};
        std::vector<std::vector<float>> est_v = {{0.0f}};
        std::vector<std::vector<int>> obj = {{0}};
        std::vector<std::vector<int>> valid = {{1}};
        auto res = computeOutlierStats(gt_u, gt_v, est_u, est_v, obj, valid);
        assert(res[0] == 0.0f); // distance 4 > 3 but ratio 0.04 <= 0.05 => not outlier
        assert(res[4] == 0.0f);
    }

    // Test 8: Relative threshold condition triggers when distance>3 and ratio>0.05
    {
        // magnitude 10, error distance 4 => ratio 0.4 > 0.05 => outlier
        std::vector<std::vector<float>> gt_u = {{10.0f}};
        std::vector<std::vector<float>> gt_v = {{0.0f}};
        std::vector<std::vector<float>> est_u = {{14.0f}};
        std::vector<std::vector<float>> est_v = {{0.0f}};
        std::vector<std::vector<int>> obj = {{0}};
        std::vector<std::vector<int>> valid = {{1}};
        auto res = computeOutlierStats(gt_u, gt_v, est_u, est_v, obj, valid);
        assert(res[0] == 1.0f);
        assert(res[4] == 1.0f);
    }

    return 0;
}

#include <vector>
#include <cmath>
#include <algorithm>

// Computes outlier statistics for a 2D flow field comparison.
// Inputs:
//   F_gt_u, F_gt_v: ground truth flow components (u, v) as 2D vectors.
//   F_est_u, F_est_v: estimated flow components.
//   object_map: 2D integer map (0=background, >0=foreground).
//   valid_mask: 2D integer map (non-zero means ground truth valid at pixel).
// All inputs have identical dimensions (non-empty).
// Returns a vector<float> of size 7:
//   [0]=num_errors_bg, [1]=num_pixels_bg,
//   [2]=num_errors_fg, [3]=num_pixels_fg,
//   [4]=num_errors_all, [5]=num_pixels_all,
//   [6]=density (num_pixels_all / max(num_pixels_all,1)).
std::vector<float> computeOutlierStats(
    const std::vector<std::vector<float>>& F_gt_u,
    const std::vector<std::vector<float>>& F_gt_v,
    const std::vector<std::vector<float>>& F_est_u,
    const std::vector<std::vector<float>>& F_est_v,
    const std::vector<std::vector<int>>& object_map,
    const std::vector<std::vector<int>>& valid_mask) {
    
    // Determine dimensions (assume all inputs have same size)
    int width = static_cast<int>(F_gt_u.size());
    int height = static_cast<int>(F_gt_u[0].size());
    
    // Initialize counters
    int num_errors_bg = 0;
    int num_pixels_bg = 0;
    int num_errors_fg = 0;
    int num_pixels_fg = 0;
    int num_errors_all = 0;
    int num_pixels_all = 0;
    
    const float ABS_THRESH = 3.0f;
    const float REL_THRESH = 0.05f;
    const float EPS = 1e-12f;
    
    // Iterate over all pixels
    for (int u = 0; u < width; ++u) {
        for (int v = 0; v < height; ++v) {
            // Only process pixels where ground truth is valid
            if (valid_mask[u][v] != 0) {
                // Compute flow difference
                float fu = F_gt_u[u][v] - F_est_u[u][v];
                float fv = F_gt_v[u][v] - F_est_v[u][v];
                float f_dist = std::sqrt(fu*fu + fv*fv);
                
                // Compute ground truth flow magnitude
                float gt_u = F_gt_u[u][v];
                float gt_v = F_gt_v[u][v];
                float f_mag = std::sqrt(gt_u*gt_u + gt_v*gt_v);
                
                // Determine if pixel is an outlier
                // Absolute threshold must be exceeded.
                // Relative threshold: if magnitude is ~0, treat relative condition as true.
                bool is_outlier = f_dist > ABS_THRESH;
                if (is_outlier) {
                    if (f_mag > EPS) {
                        // If magnitude is significant, check relative threshold
                        if (f_dist / f_mag <= REL_THRESH) {
                            is_outlier = false;
                        }
                    }
                    // If magnitude is ~0, keep is_outlier=true (already set)
                }
                
                // Update "all" counters
                if (is_outlier) {
                    num_errors_all++;
                }
                num_pixels_all++;
                
                // Update background/foreground counters based on object map
                if (object_map[u][v] == 0) {
                    // Background pixel
                    if (is_outlier) {
                        num_errors_bg++;
                    }
                    num_pixels_bg++;
                } else {
                    // Foreground pixel
                    if (is_outlier) {
                        num_errors_fg++;
                    }
                    num_pixels_fg++;
                }
            }
        }
    }
    
    // Build result vector
    std::vector<float> result;
    result.push_back(static_cast<float>(num_errors_bg));
    result.push_back(static_cast<float>(num_pixels_bg));
    result.push_back(static_cast<float>(num_errors_fg));
    result.push_back(static_cast<float>(num_pixels_fg));
    result.push_back(static_cast<float>(num_errors_all));
    result.push_back(static_cast<float>(num_pixels_all));
    
    // Density: number of valid pixels / max(num_pixels_all, 1)
    float density = static_cast<float>(num_pixels_all) / std::max(num_pixels_all, 1);
    result.push_back(density);
    
    return result;
}

// The solution iterates over every pixel position in a nested loop. For each pixel, we first check if `valid_mask[u][v]` is non-zero; if not, we skip it entirely. If valid, we compute the flow difference vector `(fu, fv)` as `F_gt_u[u][v] - F_est_u[u][v]` and `F_gt_v[u][v] - F_est_v[u][v]`. The Euclidean distance `f_dist` is `sqrt(fu*fu + fv*fv)`. The ground truth magnitude `f_mag` is `sqrt(F_gt_u[u][v]*F_gt_u[u][v] + F_gt_v[u][v]*F_gt_v[u][v])`. The pixel is an outlier if `f_dist > 3.0` AND (`f_mag == 0` OR `f_dist / f_mag > 0.05`). Using `f_mag == 0` requires careful floating-point comparison; a safer approach is to check a small epsilon, but given the problem statement explicitly says "If the ground truth flow magnitude is zero", we can use `fabs(f_mag) < 1e-12` or simply `f_mag == 0.0f` for simplicity in a teaching context, but we'll use a small epsilon to avoid precision issues. Then, based on `object_map[u][v] == 0` or not, we increment the appropriate counts: num_errors_bg and num_pixels_bg for background, or num_errors_fg and num_pixels_fg for foreground. We always increment num_errors_all and num_pixels_all for any valid pixel. After processing all pixels, we push the six counts into the result vector, then compute the density as `(float)num_pixels_all / max(num_pixels_all, 1.0f)`, where `max` is from `<algorithm>`. Edge cases: all ground truth magnitudes are zero (then only the absolute threshold matters); all pixels are invalid (then all counts remain zero and density becomes 0 because `max(0,1)=1`); the object map may contain only 0 or only positive values (then one of bg/fg counts is zero). Time complexity is O(W*H) where W and H are the dimensions, since each pixel is visited once. Space complexity is O(1) excluding the input arrays and the output vector, which is O(1) in size (7 floats).

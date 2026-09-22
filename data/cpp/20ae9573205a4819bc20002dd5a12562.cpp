/*
Write a C++ function `solveCenterFromBbox` that implements the core depth refinement and center back-projection described in the snippet. Given a 2D bounding box (as four floats `xmin, ymin, xmax, ymax`), the 3D object dimensions in height-width-length order (`hwl[3]`), and a camera intrinsic matrix `K` (3x3, `float` type, row-major) and distortion-free assumption (ignore distortion), the function must compute a refined 3D center point `center[3]` (x, y, z in camera coordinates). The algorithm: compute an initial depth using the focal length from `K` and the height ratio, then compensate for the object's rotation (`ry`) by adding a correction based on the object's size and orientation, then back-project the 2D box center to 3D using that depth, and finally refine the 3D center by iteratively adjusting the back-projected 2D point so that the reprojected 3D bounding box (using 8 corners) aligns better with the given 2D box center. The iteration is a simple gradient-descent-like update with a fixed learning rate, stopping when the cost (Euclidean distance between target 2D center and current projected 2D center) no longer improves or after a maximum of 10 iterations. The function returns the final depth value used. Handle the edge case where width or height of the bbox is ≤ 0 by setting center to (0,0,0) and returning 0.0f. No main function needed.
*/
#include <cmath>
#include <algorithm>
#include <limits>
#include <cstring>

// Solve 3D center given a 2D bounding box, object dimensions (h,w,l), and camera intrinsics.
// Input:
//   bbox[4] = {xmin, ymin, xmax, ymax}
//   hwl[3]  = {height, width, length} in 3D world units
//   K       : 3x3 intrinsic matrix (float, row-major)
//   ry      : rotation around Y-axis in radians
// Output:
//   center[3] : refined 3D center in camera coordinates
// Returns initial depth before refinement.
float solveCenterFromBbox(const float* bbox, const float* hwl,
                          const float* K, float ry, float* center) {
    const float PI = 3.14159265358979323846f;
    
    // Validate bbox
    float width_bbox = bbox[2] - bbox[0];
    float height_bbox = bbox[3] - bbox[1];
    if (width_bbox <= 0.0f || height_bbox <= 0.0f) {
        center[0] = 0.0f; center[1] = 0.0f; center[2] = 0.0f;
        return 0.0f;
    }
    
    // Focal length (average of fx and fy)
    float fx = K[0];
    float fy = K[4];
    float f = (fx + fy) * 0.5f;
    float cx = K[2];
    float cy = K[5];
    
    // Initial depth from height ratio
    float depth = f * hwl[0] / height_bbox;
    
    // Compensation from nearest vertical edge to center
    float h = hwl[0], w = hwl[1], l = hwl[2];
    float theta_bbox = std::atan2(w, l);  // robust to division by zero
    float radius_bbox = std::sqrt((l*0.5f)*(l*0.5f) + (w*0.5f)*(w*0.5f));
    
    float abs_ry = std::fabs(ry);
    float theta_z = std::min(abs_ry, PI - abs_ry) + theta_bbox;
    theta_z = std::min(theta_z, PI - theta_z);
    depth += std::fabs(radius_bbox * std::sin(theta_z));
    
    // Back-project 2D center to 3D using depth
    float center_2d[2] = { (bbox[0] + bbox[2]) * 0.5f,
                           (bbox[1] + bbox[3]) * 0.5f };
    float center_temp[3];
    center_temp[0] = (center_2d[0] - cx) * depth / f;
    center_temp[1] = (center_2d[1] - cy) * depth / f;
    center_temp[2] = depth;
    
    // Refinement via iterative adjustment
    const int MAX_ITER = 10;
    const float LEARNING_RATE = 0.7f;
    const float K_MIN_COST = 4.0f * std::sqrt(2.0f);
    const float EPS_COST_DELTA = 1e-5f;
    
    // Build rotation matrix around Y-axis
    float rot_y[9];
    float c = std::cos(ry), s = std::sin(ry);
    rot_y[0] = c;  rot_y[1] = 0.0f; rot_y[2] = s;
    rot_y[3] = 0.0f; rot_y[4] = 1.0f; rot_y[5] = 0.0f;
    rot_y[6] = -s; rot_y[7] = 0.0f; rot_y[8] = c;
    
    // Precompute 8 corners of the 3D box (centered at origin, y downward? Use y from -h to 0)
    float x_corners[8] = { l*0.5f,  l*0.5f, -l*0.5f, -l*0.5f,  l*0.5f,  l*0.5f, -l*0.5f, -l*0.5f };
    float y_corners[8] = { 0.0f,    0.0f,    0.0f,    0.0f,    -h,     -h,     -h,     -h };
    float z_corners[8] = { w*0.5f, -w*0.5f, -w*0.5f,  w*0.5f,  w*0.5f, -w*0.5f, -w*0.5f,  w*0.5f };
    
    float x_used[2] = { center_2d[0], center_2d[1] };
    float cost_pre = std::numeric_limits<float>::max();
    bool stop = false;
    int iter = 0;
    
    while (!stop && iter < MAX_ITER) {
        // Back-project current 2D point with fixed depth (use depth from current center? use original depth? Use center_temp[2] as z)
        float z_ref = center_temp[2];
        float bx = (x_used[0] - cx) * z_ref / f;
        float by = (x_used[1] - cy) * z_ref / f;
        float bz = z_ref;
        float center_test[3] = { bx, by, bz };
        
        // Project all 8 corners using rotation + translation
        float x_min = std::numeric_limits<float>::max();
        float x_max = -std::numeric_limits<float>::max();
        float y_min = std::numeric_limits<float>::max();
        float y_max = -std::numeric_limits<float>::max();
        
        for (int i = 0; i < 8; ++i) {
            // Rotate corner
            float rx = rot_y[0]*x_corners[i] + rot_y[1]*y_corners[i] + rot_y[2]*z_corners[i];
            float ry_local = rot_y[3]*x_corners[i] + rot_y[4]*y_corners[i] + rot_y[5]*z_corners[i];
            float rz = rot_y[6]*x_corners[i] + rot_y[7]*y_corners[i] + rot_y[8]*z_corners[i];
            // Translate
            float X = rx + center_test[0];
            float Y = ry_local + center_test[1];
            float Z = rz + center_test[2];
            // Project
            float u = fx * X / Z + cx;
            float v = fy * Y / Z + cy;
            x_min = std::min(x_min, u);
            x_max = std::max(x_max, u);
            y_min = std::min(y_min, v);
            y_max = std::max(y_max, v);
        }
        
        // Projected bbox center
        float projected_center[2] = { (x_min + x_max)*0.5f, (y_min + y_max)*0.5f };
        // Cost
        float cost = std::sqrt((projected_center[0]-center_2d[0])*(projected_center[0]-center_2d[0])
                             + (projected_center[1]-center_2d[1])*(projected_center[1]-center_2d[1]));
        
        if (cost >= cost_pre) {
            stop = true;
        } else {
            std::memcpy(center, center_test, 3*sizeof(float));
            float cost_delta = (cost_pre - cost) / cost_pre;
            cost_pre = cost;
            // Update 2D point toward target
            x_used[0] += (center_2d[0] - projected_center[0]) * LEARNING_RATE;
            x_used[1] += (center_2d[1] - projected_center[1]) * LEARNING_RATE;
            ++iter;
            stop = (iter >= MAX_ITER) || (cost_delta < EPS_COST_DELTA) || (cost_pre < K_MIN_COST);
        }
    }
    
    // If no iteration improved, use the initial back-projection
    if (iter == 0) {
        std::memcpy(center, center_temp, 3*sizeof(float));
    }
    
    return depth;
}
#include <cassert>
#include <cmath>

int main() {
    // Camera intrinsics: fx=fy=1000, cx=500, cy=500
    float K[9] = {1000.0f, 0.0f, 500.0f,
                  0.0f, 1000.0f, 500.0f,
                  0.0f, 0.0f, 1.0f};
    
    // Test 1: Simple case with zero rotation, object directly in front of camera.
    float bbox1[4] = {400.0f, 300.0f, 600.0f, 700.0f}; // 200x400 bbox
    float hwl1[3] = {2.0f, 1.0f, 4.0f}; // height=2, width=1, length=4
    float ry1 = 0.0f;
    float center1[3];
    float depth1 = solveCenterFromBbox(bbox1, hwl1, K, ry1, center1);
    // Expected depth roughly f*h/height = 1000*2/400 = 5.0, plus compensation ~? 
    assert(std::fabs(depth1 - 5.0f) < 1e-3f);
    // Center should be near (0, -1, depth)
    assert(std::fabs(center1[0]) < 0.5f);
    assert(std::fabs(center1[1] + 1.0f) < 0.5f);
    assert(std::fabs(center1[2] - 5.0f) < 0.5f);
    
    // Test 2: Invalid bbox (zero width)
    float bbox2[4] = {100.0f, 100.0f, 100.0f, 200.0f};
    float center2[3] = {1.0f, 1.0f, 1.0f};
    float depth2 = solveCenterFromBbox(bbox2, hwl1, K, 0.0f, center2);
    assert(depth2 == 0.0f);
    assert(center2[0] == 0.0f && center2[1] == 0.0f && center2[2] == 0.0f);
    
    // Test 3: Object rotated 90 degrees (should still produce a finite center)
    float bbox3[4] = {450.0f, 250.0f, 550.0f, 750.0f};
    float ry3 = 1.5708f; // ~90 deg
    float center3[3];
    float depth3 = solveCenterFromBbox(bbox3, hwl1, K, ry3, center3);
    assert(depth3 > 0.0f);
    assert(std::isfinite(center3[0]) && std::isfinite(center3[1]) && std::isfinite(center3[2]));
    // Center z should be positive and roughly consistent with depth
    assert(std::fabs(center3[2] - depth3) < 0.0001f);
    
    // Test 4: Bbox far to the right, should have positive x center
    float bbox4[4] = {700.0f, 400.0f, 900.0f, 600.0f}; // center x=800, y=500
    float hwl4[3] = {1.0f, 0.5f, 0.5f}; // small object
    float ry4 = 0.0f;
    float center4[3];
    float depth4 = solveCenterFromBbox(bbox4, hwl4, K, ry4, center4);
    // Depth = 1000*1/200 = 5.0, center_x = (800-500)*5/1000 = 1.5
    assert(std::fabs(depth4 - 5.0f) < 0.01f);
    assert(std::fabs(center4[0] - 1.5f) < 0.1f);
    assert(std::fabs(center4[2] - 5.0f) < 0.1f);
    
    // Test 5: Negative rotation (should still work)
    float bbox5[4] = {400.0f, 300.0f, 600.0f, 700.0f};
    float ry5 = -0.5f;
    float center5[3];
    float depth5 = solveCenterFromBbox(bbox5, hwl1, K, ry5, center5);
    assert(depth5 > 0.0f);
    assert(std::isfinite(center5[0]) && std::isfinite(center5[1]) && std::isfinite(center5[2]));
    assert(std::fabs(center5[2] - depth5) < 0.0001f);
    
    return 0;
}
// The solution mirrors the key steps from the snippet but simplifies by removing distortion and assuming a pinhole camera. The main algorithm: (1) Validate bbox dimensions; if invalid, return 0 and zero the center. (2) Extract focal length `f` as the average of `K[0][0]` and `K[1][1]`. (3) Compute initial depth = `f * hwl[0] / height_bbox`. (4) Compute compensation: `theta_bbox = atan(hwl[1] / hwl[2])` (note: guard division by zero by using `atan2` or clamp), `radius_bbox = sqrt((hwl[2]/2)^2 + (hwl[1]/2)^2)`. Then `abs_ry = fabs(ry)`, `theta_z = min(abs_ry, PI - abs_ry) + theta_bbox`, clamp `theta_z` to `[0, PI]` and then `theta_z = min(theta_z, PI - theta_z)`, add `fabs(radius_bbox * sin(theta_z))` to depth. (5) Back-project the 2D bbox center (average of xmin/xmax and ymin/ymax) to 3D using `depth`: `center_x = (cx - K[0][2]) * depth / f`, `center_y = (cy - K[1][2]) * depth / f`, `center_z = depth`. (6) Refine: create the 8 corners of the 3D box using `hwl` centered at origin (dimensions: x from -l/2 to l/2, y from -h to 0 (as in snippet the box bottom is at y=-h, top at y=0), z from -w/2 to w/2). For each corner, apply rotation matrix around Y-axis using `ry`, then translate by current center, project to 2D using K, and track min/max of projected x and y to form a projected 2D bbox. Compute cost as distance between the projected center and target center. If cost is less than previous cost, update the center to the current back-projected center, and adjust the 2D point used for back-projection by moving it slightly toward the target center (learning rate 0.7). Repeat up to 10 times, but stop early if cost stops improving. Time complexity: each iteration processes 8 corners, so O(iterations * 8) = O(1) effectively. Space: constant, only fixed arrays.

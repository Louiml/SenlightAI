/*
Write a C++ function that performs a single iteration of robust bundle adjustment for a pinhole camera model with known intrinsic parameters (focal length `fx`, principal point `cx`, `cy`, and skew `s`). Given camera rotation matrices and translation vectors, 3D point coordinates, and 2D observations with per-point weights, update weights based on a robust Cauchy-like cost function and return the updated weights and outlier flags. The function must handle a varying number of observations per point, use only the provided measurements (no new observations), and treat points with no observations as unchanged. The interface should be: `void robustWeights(const std::vector<Camera>& cams, const std::vector<Point3D>& points, std::vector<std::vector<Observation>>& obs, double maxErr)`, where `Camera` holds a 3x3 rotation matrix (row-major), a 3-element translation, and intrinsic parameters; `Point3D` holds 3 doubles; `Observation` holds 2D coordinates, a weight, a view index, and an outlier flag. After the call, each observation's weight is set to `(1 - (err/maxErr)^2)^2` if `err < maxErr`, else weight is 0 and outlier is 1, where `err` is squared Euclidean distance between the projected point (using the camera model `x' = fx * (R*p + t)_x / (R*p + t)_z + cx + s * (R*p + t)_y / (R*p + t)_z`, `y' = fy * (R*p + t)_y / (R*p + t)_z + cy`) and the observation.
*/

#include <vector>
#include <cmath>

struct Camera {
    std::vector<double> R; // 3x3 row-major
    std::vector<double> t; // 3 elements
    double fx, fy, cx, cy, skew; // intrinsic parameters
};

struct Point3D {
    double x, y, z;
};

struct Observation {
    double x, y;
    double weight;
    int viewId;
    bool outlier;
};

// Update weights and outlier flags for all observations based on reprojection error.
void robustWeights(const std::vector<Camera>& cams,
                   const std::vector<Point3D>& points,
                   std::vector<std::vector<Observation>>& obs,
                   double maxErr) {
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = 0; j < obs[i].size(); ++j) {
            Observation& ob = obs[i][j];
            const Camera& cam = cams[ob.viewId];
            
            // Project the 3D point using the camera model.
            double Px = cam.R[0] * points[i].x + cam.R[1] * points[i].y + cam.R[2] * points[i].z + cam.t[0];
            double Py = cam.R[3] * points[i].x + cam.R[4] * points[i].y + cam.R[5] * points[i].z + cam.t[1];
            double Pz = cam.R[6] * points[i].x + cam.R[7] * points[i].y + cam.R[8] * points[i].z + cam.t[2];
            
            if (std::abs(Pz) < 1e-12) {
                ob.weight = 0.0;
                ob.outlier = true;
                continue;
            }
            
            double invZ = 1.0 / Pz;
            double projX = cam.fx * Px * invZ + cam.cx + cam.skew * Py * invZ;
            double projY = cam.fy * Py * invZ + cam.cy;
            
            double err = (projX - ob.x) * (projX - ob.x) + (projY - ob.y) * (projY - ob.y);
            
            if (err >= maxErr) {
                ob.weight = 0.0;
                ob.outlier = true;
            } else {
                double s = err / maxErr;
                double w = (1.0 - s * s);
                ob.weight = w * w;
                ob.outlier = false;
            }
        }
    }
}

#include <cassert>
#include <cmath>

// Assume the solution code is above here.

int main() {
    // Setup: one camera with identity rotation, no translation, focal length 1, principal point 0.
    Camera cam;
    cam.R = {1,0,0, 0,1,0, 0,0,1};
    cam.t = {0,0,0};
    cam.fx = 1.0; cam.fy = 1.0; cam.cx = 0.0; cam.cy = 0.0; cam.skew = 0.0;
    
    // One point at (2, 3, 4).
    Point3D p = {2.0, 3.0, 4.0};
    
    // Observations: one accurate, one offset.
    std::vector<Observation> obsList;
    obsList.push_back({0.5, 0.75, 1.0, 0, false}); // exactly projected (2/4=0.5, 3/4=0.75)
    obsList.push_back({10.0, 10.0, 1.0, 0, false}); // far off
    
    std::vector<Point3D> points = {p};
    std::vector<std::vector<Observation>> obs = {obsList};
    
    double maxErr = 1.0; // error squared threshold
    
    robustWeights({cam}, points, obs, maxErr);
    
    // First observation: error is 0, weight = (1-0)^2 = 1, not outlier.
    assert(obs[0][0].outlier == false);
    assert(std::abs(obs[0][0].weight - 1.0) < 1e-9);
    
    // Second observation: error is huge, weight 0, outlier true.
    assert(obs[0][1].outlier == true);
    assert(obs[0][1].weight == 0.0);
    
    // Test with a small error that gives a non-zero weight.
    std::vector<Observation> obs2 = {{0.6, 0.75, 1.0, 0, false}};
    std::vector<std::vector<Observation>> obs2list = {obs2};
    robustWeights({cam}, points, obs2list, 1.0);
    double err = (0.5-0.6)*(0.5-0.6) + (0.75-0.75)*(0.75-0.75); // 0.01
    double s = 0.01;
    double expected = (1.0 - s*s)*(1.0 - s*s); // (1-0.0001)^2 ≈ 0.9998
    assert(std::abs(obs2list[0][0].weight - expected) < 1e-6);
    assert(obs2list[0][0].outlier == false);
    
    // Test with zero depth (division by zero case).
    cam.t = {0,0,-4}; // move camera so point is at z=0
    std::vector<Observation> obs3 = {{1.0,1.0,1.0,0,false}};
    std::vector<std::vector<Observation>> obs3list = {obs3};
    robustWeights({cam}, points, obs3list, 1.0);
    assert(obs3list[0][0].outlier == true);
    assert(obs3list[0][0].weight == 0.0);
    
    return 0;
}

// The solution must iterate over each 3D point and each associated observation. For each observation, we need to identify the corresponding camera (via `viewId`) and apply the projection formula using the known intrinsics. The projection requires computing `P = R * point + t`, then `x = fx * P.x / P.z + cx + s * P.y / P.z`, `y = fy * P.y / P.z + cy`. Edge cases include: points with zero `P.z` (avoid division by zero by setting the weight to 0 and outlier to 1), observations with weight already 0 (we still update them), and missing cameras (though the input is assumed consistent). Time complexity is O(total number of observations) because each observation is processed once, and the projection is constant-time. Space complexity is O(1) extra (only local variables), as we modify weights in place.

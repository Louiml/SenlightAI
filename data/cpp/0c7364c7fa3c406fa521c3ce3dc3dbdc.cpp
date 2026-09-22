// Write a standalone C++ function named `selectCamerasAngleWeight` that takes a vector of camera objects (each with a 3D position, a 3D direction, a 3D rotation quaternion, and an active flag), a reference "eye" camera, and an unsigned integer `count`. The function must return a vector of indices (as `unsigned int`) of the `count` best cameras to use for image-based rendering, selected by a weighted combination of normalized distance and angular difference to the eye camera. The selection is restricted to cameras whose direction forms an angle of at most 45 degrees with the eye's direction (i.e., dot product > 0.707). For each eligible camera, compute a factor as `(1 - angleWeight) * normalizedDistance + angleWeight * normalizedAngle`, where `angleWeight = 0.3`, normalizedDistance is the squared distance linearly interpolated between 0 and the maximum squared distance among all active cameras, and normalizedAngle is the angle (in radians) between the eye's rotation and the camera's rotation, linearly interpolated between 0 and `4.71239` (270 degrees). Sort the eligible cameras by this factor in ascending order and return the indices of the first `count` (or fewer if fewer eligible). If no cameras are eligible, return an empty vector. Use `float` for all numeric calculations.

// The solution iterates over all cameras twice. First pass computes the squared Euclidean distance from each active camera's position to the eye's position and tracks the maximum squared distance. Second pass, for each active camera, checks the dot product of its direction with the eye's direction; if > 0.707, it computes the angle between the two rotation quaternions (using the standard `acos(2*dot^2 - 1)` formula or a library function), normalizes distance and angle using linear interpolation with the precomputed max distance and a fixed maximum angle of 4.71239 radians, combines them with the fixed angle weight (0.3), and inserts into a `std::multimap<float, unsigned int>` keyed by factor to keep sorted ascending. Finally, take the first up to `count` entries from the multimap. Edge cases include: all cameras inactive (return empty); fewer active cameras than `count` (return all eligible); maxdist = 0 (division by zero avoided by maxdist being at least 0; if maxdist = 0, all distances are 0, normalized distance becomes 0 via `inverseLerp` returning 0/0? We need to handle by defining `inverseLerp` to return 0 when max == min). Time complexity is O(N log N) due to sorting in multimap (insertion is O(log N) each); space is O(N) for the multimap and distance vector. The function uses only `const` references and `float` arithmetic.

#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

// Minimal structures to represent camera data; adjust to match actual library.
struct CameraData {
    float pos[3];
    float dir[3];
    float quat[4]; // quaternion (x,y,z,w)
    bool active;
};

// Helper to compute dot product of two 3D vectors (arrays).
float dot3(const float* a, const float* b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

// Helper to compute squared distance between two 3D points.
float sqDist3(const float* a, const float* b) {
    float dx = a[0]-b[0], dy = a[1]-b[1], dz = a[2]-b[2];
    return dx*dx + dy*dy + dz*dz;
}

// Helper to compute angle (radians) between two quaternions.
// Quaternions are [x,y,z,w]; assumes both are unit quaternions.
float angleBetweenQuats(const float* q1, const float* q2) {
    float dot = q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2] + q1[3]*q2[3];
    // Clamp to avoid domain error due to floating point
    dot = std::max(-1.0f, std::min(1.0f, dot));
    return 2.0f * std::acos(dot);
}

// Helper: linearly interpolate value from [min, max] to [0,1]; if max==min returns 0.
float inverseLerp(float min, float max, float value) {
    if (max <= min) return 0.0f;
    return (value - min) / (max - min);
}

// Main function: select up to 'count' camera indices based on weighted distance and angle.
std::vector<unsigned int> selectCamerasAngleWeight(
    const std::vector<CameraData>& cams,
    const CameraData& eye,
    unsigned int count)
{
    const float angleWeight = 0.3f;
    const float midAngle = 4.71239f; // ~270 degrees in radians

    // First pass: compute squared distances and max distance
    std::vector<float> sqrDists(cams.size(), 0.0f);
    float maxdist = 0.0f;
    for (size_t i = 0; i < cams.size(); ++i) {
        if (cams[i].active) {
            float sqrDist = sqDist3(cams[i].pos, eye.pos);
            sqrDists[i] = sqrDist;
            maxdist = std::max(maxdist, sqrDist);
        }
    }

    // Second pass: compute factors and store in multimap for sorting
    std::multimap<float, unsigned int> factors;
    for (size_t i = 0; i < cams.size(); ++i) {
        if (!cams[i].active) continue;
        float a = dot3(cams[i].dir, eye.dir);
        if (a > 0.707f) { // within 45 degrees
            float sqrDist = sqrDists[i];
            float currNormalDist = inverseLerp(0.0f, maxdist, sqrDist);
            float angle = angleBetweenQuats(cams[i].quat, eye.quat);
            float currNormalAngle = inverseLerp(0.0f, midAngle, angle);
            float factor = currNormalDist * (1.0f - angleWeight) + currNormalAngle * angleWeight;
            factors.insert(std::make_pair(factor, static_cast<unsigned int>(i)));
        }
    }

    // Take first up to 'count' indices
    std::vector<unsigned int> result;
    for (auto it = factors.begin(); it != factors.end() && result.size() < count; ++it) {
        result.push_back(it->second);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <cmath>

// Assume the solution function and helpers are available above (include the code or header).

int main() {
    // Helper to create a camera
    auto makeCam = [](float px, float py, float pz,
                      float dx, float dy, float dz,
                      float qx, float qy, float qz, float qw,
                      bool active) {
        CameraData c;
        c.pos[0]=px; c.pos[1]=py; c.pos[2]=pz;
        c.dir[0]=dx; c.dir[1]=dy; c.dir[2]=dz;
        // normalize direction
        float len = std::sqrt(dx*dx+dy*dy+dz*dz);
        if(len>0){ c.dir[0]/=len; c.dir[1]/=len; c.dir[2]/=len; }
        c.quat[0]=qx; c.quat[1]=qy; c.quat[2]=qz; c.quat[3]=qw;
        c.active = active;
        return c;
    };

    // Eye camera at origin, looking along +Z, rotation identity (0,0,0,1)
    CameraData eye = makeCam(0,0,0, 0,0,1, 0,0,0,1, true);

    std::vector<CameraData> cams;
    // Camera 0: close, same direction, identity rotation -> best
    cams.push_back(makeCam(0,0,1, 0,0,1, 0,0,0,1, true));
    // Camera 1: farther, same direction, identity rotation
    cams.push_back(makeCam(0,0,5, 0,0,1, 0,0,0,1, true));
    // Camera 2: close but rotated 90 deg around X -> angle too big, dot product 0 -> excluded
    cams.push_back(makeCam(0,0,1, 0,1,0, 0.7071f,0,0,0.7071f, true));
    // Camera 3: inactive, ignore
    cams.push_back(makeCam(0,0,1, 0,0,1, 0,0,0,1, false));
    // Camera 4: very far but same direction, rotation 10 deg -> should be worse than 0 and 1
    cams.push_back(makeCam(0,0,100, 0,0,1, 0,0.0872f,0,0.9962f, true));

    // Select 2 cameras
    std::vector<unsigned int> sel = selectCamerasAngleWeight(cams, eye, 2);
    assert(sel.size() == 2);
    assert(sel[0] == 0); // best factor (close, no angle)
    // The second should be camera 1 (closer than 4 despite 4 having bigger angle? Actually 4 is very far, so distance dominates)
    // Check that selected indices are {0,1} or {0,4}? Let's compute: camera1 dist=25, angle=0; camera4 dist=10000, angle small; normalized dist for 1=25/10000=0.0025, for 4=1.0; normalized angle ~0.033; factor1=0.0025*0.7+0=0.00175; factor4=1*0.7+0.033*0.3≈0.71. So camera1 better. 
    assert(sel[0] == 0 && sel[1] == 1);

    // Test selection count larger than eligible (eligible: 0,1,4 => 3)
    sel = selectCamerasAngleWeight(cams, eye, 10);
    assert(sel.size() == 3);

    // Test no eligible (all inactive)
    std::vector<CameraData> cams2;
    cams2.push_back(makeCam(0,0,1, 0,0,1, 0,0,0,1, false));
    sel = selectCamerasAngleWeight(cams2, eye, 2);
    assert(sel.empty());

    // Test maxdist = 0 (all cameras at same position as eye)
    std::vector<CameraData> cams3;
    cams3.push_back(makeCam(0,0,0, 0,0,1, 0,0,0,1, true));
    cams3.push_back(makeCam(0,0,0, 0,0,1, 0,0,0,1, true));
    sel = selectCamerasAngleWeight(cams3, eye, 1);
    assert(sel.size() == 1 && sel[0] == 0); // distance normalized to 0, all equal, first in order

    // Test angle weight: camera with small angle vs small distance? Hard to unit test exactly, but check ordering with far small-angle vs close large-angle (but still under 45 deg)
    std::vector<CameraData> cams4;
    // Close camera with 30 deg rotation (dot ~0.866>0.707), quat rotation around Y by 30 deg: q = (0, sin15,0,cos15)
    float avg = 0.2588f; // sin(15°)
    float cosv = 0.9659f; // cos(15°)
    cams4.push_back(makeCam(0,0,1, 0,0,1, 0,avg,0,cosv, true));
    // Far camera with 0 deg rotation
    cams4.push_back(makeCam(0,0,50, 0,0,1, 0,0,0,1, true));
    sel = selectCamerasAngleWeight(cams4, eye, 2);
    assert(sel.size() == 2);
    // The far camera may be selected before close due to angle penalty? Compute factor:
    // close: dist=1, angle ~0.524 rad (30 deg); normalized angle=0.524/4.712=0.1111; normalized dist=1/2500=0.0004; factor=0.0004*0.7+0.1111*0.3≈0.0336
    // far: dist=2500, angle=0; norm dist=1.0; factor=1.0*0.7=0.7 -> far is worse. So close first.
    assert(sel[0] == 0 && sel[1] == 1);

    // Test count=0 returns empty
    sel = selectCamerasAngleWeight(cams, eye, 0);
    assert(sel.empty());

    return 0;
}

// Given a 3D volume represented as a function that returns a voxel intensity at integer coordinates `(x, y, z)`, and a set of axis-aligned sample points along a ray starting from a given origin with a given direction vector, write a standalone C++ function `computeMIPValue` that performs maximum intensity projection (MIP) along the ray. The function must accept a callback or function object for voxel access, a 3D origin, a 3D direction, and a maximum number of steps, and return the maximum intensity encountered while stepping along the ray in unit-length increments. The direction vector may be non-normalized, but the step size must be fixed to one unit in the direction of the ray. The ray must be clipped to a bounding box defined by `[0, volumeSize)` in each dimension, and samples outside the volume are ignored. The function must return 0 if no valid samples are found. Ensure the function handles negative direction components correctly, and that it stops after processing at most `maxSteps` steps, even if the ray remains inside the volume.

The solution requires iterating along a ray from the origin in the direction vector, taking unit-length steps. To handle a potentially non-unit direction vector, we normalize it to get a unit step direction. Then we repeatedly step from the origin: at each step, compute the integer coordinates by rounding down (floor) the floating-point position, check if they lie within the bounding box `[0, volumeSize)`, and if so, query the voxel intensity via the provided callback. Keep track of the maximum intensity seen. Stop when we exceed the maximum number of steps or when the ray has left the bounding box (we can cheaply check the current position against the box after each step, but we also need to ensure we do not sample outside). Since the direction may have negative components, floor is correct to map the continuous positions to integer voxel indices. Edge cases: zero-length direction vector should return 0 immediately; if origin is already outside the box, the first sample will be ignored; if all samples are outside, return 0. Time complexity is O(maxSteps), space complexity is O(1) auxiliary (excluding the callback). The callback is called at most maxSteps times.

#include <cmath>
#include <functional>
#include <algorithm>

// Compute MIP along a ray.
// Parameters:
//   voxelAccess: callable object taking (int x, int y, int z) and returning a voxel intensity value.
//   originX, originY, originZ: starting point of the ray (floating-point).
//   dirX, dirY, dirZ: direction vector, need not be normalized.
//   volumeSize: the volume is a cube of size volumeSize in each dimension, coordinates 0..volumeSize-1.
//   maxSteps: maximum number of unit steps to take along the ray.
// Returns the maximum intensity found along the ray, or 0 if no valid samples are encountered.
template<typename VoxelAccessor>
int computeMIPValue(VoxelAccessor voxelAccess,
                    float originX, float originY, float originZ,
                    float dirX, float dirY, float dirZ,
                    int volumeSize, int maxSteps) {
    if (maxSteps <= 0 || volumeSize <= 0) return 0;
    
    float len = std::sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);
    if (len < 1e-6f) return 0; // Zero direction vector
    
    // Unit step direction
    float stepX = dirX / len;
    float stepY = dirY / len;
    float stepZ = dirZ / len;
    
    float posX = originX;
    float posY = originY;
    float posZ = originZ;
    
    int maxVal = 0;
    
    for (int i = 0; i < maxSteps; ++i) {
        int ix = static_cast<int>(std::floor(posX));
        int iy = static_cast<int>(std::floor(posY));
        int iz = static_cast<int>(std::floor(posZ));
        
        if (ix >= 0 && ix < volumeSize &&
            iy >= 0 && iy < volumeSize &&
            iz >= 0 && iz < volumeSize) {
            int val = voxelAccess(ix, iy, iz);
            maxVal = std::max(maxVal, val);
        }
        
        // Advance one unit step
        posX += stepX;
        posY += stepY;
        posZ += stepZ;
    }
    
    return maxVal;
}

#include <cassert>

int main() {
    // Simple 3x3x3 volume where voxel values equal x+y+z
    auto volume = [](int x, int y, int z) { return x + y + z; };
    
    // Ray from (0,0,0) in direction (1,0,0), steps of 1 along x
    assert(computeMIPValue(volume, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 5) == 2);
    // Explanation: samples at (0,0,0)=0, (1,0,0)=1, (2,0,0)=2, then out of bounds -> max=2
    
    // Ray from (0.5,0.5,0.5) in direction (1,0,0) with maxSteps=2
    // Samples: floor(0.5)=0 -> (0,0,0)=0, then (1,0,0)=1 -> max=1
    assert(computeMIPValue(volume, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 3, 2) == 1);
    
    // Ray starting outside volume, going inward
    assert(computeMIPValue(volume, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 10) == 2);
    
    // Ray with negative direction
    assert(computeMIPValue(volume, 2.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 3, 5) == 2);
    // Samples: (2,0,0)=2, (1,0,0)=1, (0,0,0)=0, then out -> max=2
    
    // Non-normalized direction
    assert(computeMIPValue(volume, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 3, 3) == 2); // same as unit
    
    // All samples out of bounds
    assert(computeMIPValue(volume, -5.0f, -5.0f, -5.0f, 1.0f, 1.0f, 1.0f, 3, 5) == 0);
    
    // Zero direction
    assert(computeMIPValue(volume, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3, 5) == 0);
    
    // maxSteps = 1
    assert(computeMIPValue(volume, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 1) == 0);
    
    // volumeSize = 1, ray along diagonal
    assert(computeMIPValue(volume, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1, 5) == 0); // only (0,0,0) value 0, but max is 0
    
    // Check with a custom volume that has negative values
    auto negativeVolume = [](int x, int y, int z) { return (x + y + z) - 10; };
    assert(computeMIPValue(negativeVolume, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 5) == -8);
    // max of -10, -9, -8 is -8
    
    return 0;
}

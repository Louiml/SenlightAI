// Given a `BoundingBox` represented by two `Vector3F` corner points and a target sample spacing `h`, write a C++ function that computes and returns the required Hilbert space-filling curve level (an integer between 4 and 8 inclusive) such that the cell size at that level is at least the given spacing. The cell size is defined as the box's longest diagonal divided by \(2^{level}\). Use the provided helper functions `boundingBoxCenter`, `boundingBoxLongestDistance`, and a `roundDownToMultipleOf16` function. The function must handle edge cases where the closest valid level falls outside the 4–8 range by clamping to the nearest bound.
#include <cassert>
#include <cmath>

int main() {
    // Normal case: box of size 10x10x10, spacing 2.
    BoundingBox b1{{0,0,0},{10,10,10}};
    // Longest distance ~17.32, round down to 16. Need 2*2^l >= 16 => 2^(l+1) >=16 => l+1>=4 => l>=3, so clamp to 4.
    assert(computeSampleLevel(b1, 2.0f) == 4);

    // Box with longest distance 100, spacing 1. 1*2^l >= 100 => l>=7 (since 128>=100). Expected level 7.
    BoundingBox b2{{0,0,0},{100,0,0}}; // diagonal = 100, roundDown to 96? 100/16=6.25, floor=6, *16=96. Then 1*2^l >=96 => l>=7 (128). So level=7.
    assert(computeSampleLevel(b2, 1.0f) == 7);

    // Very small spacing: should clamp to 8.
    BoundingBox b3{{0,0,0},{1000,1000,1000}}; // diagonal ~1732, roundDown=1728. Need h*2^8 >=1728 => h>=6.75. With h=1, level=8.
    assert(computeSampleLevel(b3, 1.0f) == 8);

    // Very large spacing: should clamp to 4.
    BoundingBox b4{{0,0,0},{1,1,1}}; // diagonal ~1.73, roundDown=0 (since 1.73/16<1). Per our degenerate handling, returns 4.
    assert(computeSampleLevel(b4, 100.0f) == 4);

    // Edge case: spacing exactly satisfies at level 4.
    BoundingBox b5{{0,0,0},{16,0,0}}; // diagonal=16, roundDown=16. h=4 => 4*16=64>=16, level=4.
    assert(computeSampleLevel(b5, 4.0f) == 4);

    // Edge case: spacing fails at level 4 but works at level 5.
    BoundingBox b6{{0,0,0},{32,0,0}}; // diagonal=32, roundDown=32. h=1 => 1*32=32>=32, actually level=5? Wait 1*2^5=32>=32, level=5. Check: 1*2^4=16<32, so break at 5.
    assert(computeSampleLevel(b6, 1.0f) == 5);

    // Degenerate zero-area box.
    BoundingBox b7{{0,0,0},{0,0,0}};
    assert(computeSampleLevel(b7, 1.0f) == 4);

    // Negative spacing should return 8 (as per our design).
    assert(computeSampleLevel(b1, -1.0f) == 8);

    return 0;
}
#include <algorithm>
#include <cmath>
#include <cstdint>

// Simple 3D vector type for the task.
struct Vector3F {
    float x, y, z;
};

// Axis-aligned bounding box with min and max corners.
struct BoundingBox {
    Vector3F minCorner;
    Vector3F maxCorner;
};

// Return the center of the box.
Vector3F boundingBoxCenter(const BoundingBox& b) {
    return { (b.minCorner.x + b.maxCorner.x) * 0.5f,
             (b.minCorner.y + b.maxCorner.y) * 0.5f,
             (b.minCorner.z + b.maxCorner.z) * 0.5f };
}

// Return the longest diagonal length of the box.
float boundingBoxLongestDistance(const BoundingBox& b) {
    float dx = b.maxCorner.x - b.minCorner.x;
    float dy = b.maxCorner.y - b.minCorner.y;
    float dz = b.maxCorner.z - b.minCorner.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

// Round down to nearest multiple of 16 (as in original snippet's Round16).
float roundDownToMultipleOf16(float value) {
    return std::floor(value / 16.0f) * 16.0f;
}

// Compute the required space-filling curve level (4..8) for given box and spacing.
int computeSampleLevel(const BoundingBox& box, float sampleSpacing) {
    if (sampleSpacing <= 0.0f) {
        // Invalid spacing: fall back to the finest level? The original code
        // would start at 4 and break only if h*2^l >= spanL, which for negative
        // h never happens, so it would go to 8. We'll mirror that.
        return 8;
    }

    const float spanL = roundDownToMultipleOf16(boundingBoxLongestDistance(box));
    if (spanL <= 0.0f) {
        // Degenerate box (zero volume) – return the coarsest level.
        return 4;
    }

    int level = 4;
    for (; level < 9; ++level) {
        if (sampleSpacing * (1 << level) >= spanL) {
            break;
        }
    }
    // level is now 4..8 (loop stops at 9 if condition never met, then clamp to 8).
    return std::min(level, 8);
}
// The solution computes the longest distance (diagonal length) of the axis-aligned bounding box. The space-filling curve subdivides the box into \(2^{level}\) cells per dimension, so the cell diagonal is approximately the box diagonal divided by \(2^{level}\). We need the smallest level `l` (starting from lower bound 4) such that `h * 2^l >= longestDistance`. Equivalently, `2^l >= longestDistance / h`. We start at `l=4` and increment until the condition holds or until we reach the upper bound (8). If even at level 8 the condition is not met, we clamp to 8. Similarly, if at level 4 the condition is already met, we choose 4. This guarantees the output is in [4,8]. The algorithm is a simple loop with constant iterations (at most 5 checks), so time complexity is O(1) and space complexity O(1). Edge cases: `h` could be very large (`h * 2^4` already exceeds longest distance, so level=4) or very small (`h * 2^8` still less than longest distance, so level=8). Also `h` must be positive; if not, we treat it as invalid and return the lower bound.

Write a standalone C++ function `findClosestRoom` that, given a set of axis-aligned bounding boxes (AABBs) representing rooms, an arbitrary 3D point, and the index of the previously known containing room (or -1 if unknown), returns the index of the room whose AABB contains the point. If the point is outside all rooms, return the index of the room whose AABB is closest to the point in terms of Euclidean distance to the nearest point on the AABB surface (distance 0 means inside). The function must first check the "previous room" shortcut: if the previous room's AABB contains the point, return its index immediately. Otherwise, evaluate all rooms and choose: (1) any room whose AABB contains the point, preferring the one with the smallest AABB volume (to approximate "internal room" priority); if multiple contain the point and have equal volume, prefer the smallest index. (2) If no room contains the point, return the room with the smallest distance from the point to the AABB (if ties, smallest index). The input is a vector of `AABB` structs, each with `min` and `max` corner points as `double` coordinates. The function should handle empty input by returning -1, and should treat points exactly on a boundary as inside. Provide a reference implementation and assertions. Time complexity should be O(n) per query with O(1) extra space, and the solution must not modify the input.

#include <cassert>
#include <vector>

// AABB struct and findClosestRoom function must be included above.

int main() {
    // Three rooms: room0 large (0..10), room1 medium (5..15), room2 small (8..9)
    std::vector<AABB> rooms = {
        {{0.0, 0.0, 0.0}, {10.0, 10.0, 10.0}},
        {{5.0, 5.0, 5.0}, {15.0, 15.0, 15.0}},
        {{8.0, 8.0, 8.0}, {9.0, 9.0, 9.0}}
    };

    // Test 1: Point inside previous room -> returns previous room.
    double p1[3] = {1.0, 1.0, 1.0};
    assert(findClosestRoom(rooms, p1, 0) == 0);

    // Test 2: Point inside room1 and room0, but room1 smaller volume -> room1.
    double p2[3] = {6.0, 6.0, 6.0};
    assert(findClosestRoom(rooms, p2, -1) == 1);

    // Test 3: Point inside small room2 only (also in larger rooms but smallest volume wins).
    double p3[3] = {8.5, 8.5, 8.5};
    assert(findClosestRoom(rooms, p3, -1) == 2);

    // Test 4: Point on boundary is considered inside.
    double p4[3] = {10.0, 5.0, 5.0};
    assert(findClosestRoom(rooms, p4, -1) == 0);

    // Test 5: Point outside all rooms -> nearest by distance.
    // p5 = (20,20,20): distance to room1 is 5 in each axis -> dist^2 = 75, to room0 is 10 in x only and 10 in y,z? Actually room0 max 10, so dx=10, dy=10, dz=10 -> 300. So room1 closest.
    double p5[3] = {20.0, 20.0, 20.0};
    assert(findClosestRoom(rooms, p5, -1) == 1);

    // Test 6: Point outside, closer to room0 than room1.
    double p6[3] = {11.0, 0.0, 0.0};
    // room0: dx=1, dy=0, dz=0 -> dist^2=1; room1: dx=0? x=11 is inside 5..15, y=0<5 dy=5, z=0<5 dz=5 -> 50. So room0.
    assert(findClosestRoom(rooms, p6, -1) == 0);

    // Test 7: Empty list returns -1.
    std::vector<AABB> empty;
    assert(findClosestRoom(empty, p1, -1) == -1);

    // Test 8: Previous room index out of range is ignored.
    assert(findClosestRoom(rooms, p5, 99) == 1);

    // Test 9: Point exactly at a corner shared by two rooms, tie volume? 
    // Rooms: two boxes (0..1) and (1..2). Point (1,1,1) is on boundary of both, equal volume -> smallest index 0.
    std::vector<AABB> twoRooms = {
        {{0.0,0.0,0.0},{1.0,1.0,1.0}},
        {{1.0,1.0,1.0},{2.0,2.0,2.0}}
    };
    double corner[3] = {1.0, 1.0, 1.0};
    assert(findClosestRoom(twoRooms, corner, -1) == 0);

    // Test 10: Non-overlapping rooms, nearest by distance with a point exactly between.
    std::vector<AABB> separated = {
        {{0.0,0.0,0.0},{1.0,1.0,1.0}},
        {{10.0,10.0,10.0},{11.0,11.0,11.0}}
    };
    double mid[3] = {5.5, 5.5, 5.5};
    // Both distance^2 = (4.5)^2*3 = 60.75, tie -> smaller index 0.
    assert(findClosestRoom(separated, mid, -1) == 0);

    return 0;
}

#include <vector>
#include <cmath>

struct AABB {
    double min[3]; // x, y, z minimum corner
    double max[3]; // x, y, z maximum corner
};

// Returns true if point p is inside (or on boundary of) the given box.
static inline bool pointInAABB(const AABB& box, const double p[3]) {
    return (p[0] >= box.min[0] && p[0] <= box.max[0] &&
            p[1] >= box.min[1] && p[1] <= box.max[1] &&
            p[2] >= box.min[2] && p[2] <= box.max[2]);
}

// Computes squared Euclidean distance from point p to the box surface.
// Returns 0.0 if point is inside the box.
static inline double squaredDistanceToAABB(const AABB& box, const double p[3]) {
    double dx = 0.0, dy = 0.0, dz = 0.0;
    if (p[0] < box.min[0]) dx = box.min[0] - p[0];
    else if (p[0] > box.max[0]) dx = p[0] - box.max[0];
    if (p[1] < box.min[1]) dy = box.min[1] - p[1];
    else if (p[1] > box.max[1]) dy = p[1] - box.max[1];
    if (p[2] < box.min[2]) dz = box.min[2] - p[2];
    else if (p[2] > box.max[2]) dz = p[2] - box.max[2];
    return dx*dx + dy*dy + dz*dz;
}

// Finds the room (AABB) that contains the point p.
// If none contains it, finds the nearest room by distance to surface.
// The previousRoomIndex is a hint; if it contains p, return it immediately.
// Returns -1 if no rooms exist.
int findClosestRoom(const std::vector<AABB>& rooms, const double p[3], int previousRoomIndex) {
    const int numRooms = static_cast<int>(rooms.size());
    if (numRooms == 0) return -1;

    // Shortcut: check previous room first.
    if (previousRoomIndex >= 0 && previousRoomIndex < numRooms) {
        if (pointInAABB(rooms[previousRoomIndex], p)) {
            return previousRoomIndex;
        }
    }

    // First pass: find any containing room, prefer smallest volume.
    int bestContainIndex = -1;
    double bestVolume = 0.0;
    for (int i = 0; i < numRooms; ++i) {
        if (pointInAABB(rooms[i], p)) {
            double vol = (rooms[i].max[0] - rooms[i].min[0]) *
                         (rooms[i].max[1] - rooms[i].min[1]) *
                         (rooms[i].max[2] - rooms[i].min[2]);
            if (bestContainIndex == -1 || vol < bestVolume) {
                bestContainIndex = i;
                bestVolume = vol;
            }
        }
    }
    if (bestContainIndex != -1) return bestContainIndex;

    // No containment: find nearest by squared distance, ties by smaller index.
    double bestDistSq = -1.0;
    int bestDistIndex = -1;
    for (int i = 0; i < numRooms; ++i) {
        double distSq = squaredDistanceToAABB(rooms[i], p);
        if (bestDistIndex == -1 || distSq < bestDistSq) {
            bestDistSq = distSq;
            bestDistIndex = i;
        }
    }
    return bestDistIndex;
}

// The problem is a simplified version of spatial point-location used in the original Godot portal system, but here we avoid building a BSP tree and instead use a brute-force linear scan. The main algorithm is straightforward: first check the previous room's AABB for containment, using the condition `min.x <= p.x <= max.x` and similarly for y and z, with inclusive bounds. If this succeeds, return that room index. Otherwise, iterate over all rooms. For each room, compute a containment test (returns true/false) and, if contained, compute its volume as `(max.x-min.x)*(max.y-min.y)*(max.z-min.z)`. Track the smallest volume among containing rooms; if a smaller volume is found, update the candidate. If volumes are equal, keep the smaller index (since we iterate in increasing index order, we can use strict less than comparison). If no room contains the point, compute the squared Euclidean distance from the point to the AABB: for each axis, if the point coordinate is below min, add `(min - p)^2`; if above max, add `(p - max)^2`; otherwise contribution is 0. Take the square root only at the end (or keep squared distances, since comparing squared distances is equivalent and avoids floating error). Track the smallest squared distance and the corresponding room index, breaking ties by smaller index. Edge cases: empty input returns -1; a point inside multiple rooms prefers the smaller volume; point exactly on boundary counts as contained; if previous room index is out of bounds (e.g., negative or beyond vector size), skip the shortcut. For `n` rooms, time is O(n) and space is O(1). The solution uses `const` references and a `std::vector<AABB>`.

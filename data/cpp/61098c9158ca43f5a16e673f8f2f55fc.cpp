Implement a C++ function that simulates the core logic of the `SlideMove` and `StepMove` routines from monster physics: given a starting 2D position, a velocity vector, a movement delta vector, a set of axis-aligned rectangles (obstacles), a maximum step height, and a minimum floor cosine (for slope validity), return the final position after attempting to move the delta. The movement must first try a direct slide move (up to 3 collision iterations, projecting velocity and remaining delta onto the collision normal with an overclip factor of 1.001); if blocked, it must try stepping up by the max step height, slide at the elevated position, then step back down, and accept the stepped path only if the horizontal distance traveled is strictly greater than the direct path and the final ground slope cosine is at least the minimum floor cosine. The function should also report whether the move was `OK`, `SLIDING`, `STEPPED`, or `BLOCKED` (using an output enum). For simplicity, treat gravity as pointing downward (0,1) in 2D, collisions only against axis-aligned rectangles, and ignore entity dynamics. The input obstacles are given as `{x, y, width, height}` rectangles; the starting point and all vectors are 2D. The function signature is: `MoveResult simulateMove(Point2& pos, Vec2& velocity, const Vec2& delta, const std::vector<Rect>& obstacles, float maxStepHeight, float minFloorCosine)`. Provide a free function only; no main.

#include <cassert>
#include <cmath>

// The solution code is assumed to be included above.

int main() {
    // Case 1: Open move, no obstacles
    {
        Point2 p = {0, 0};
        Vec2 v = {1, 0};
        Vec2 d = {5, 0};
        std::vector<Rect> obs;
        MoveResult r = simulateMove(p, v, d, obs, 18.0f, 0.7f);
        assert(r == MoveResult::OK);
        assert(std::abs(p.x - 5.0f) < 1e-3f);
        assert(std::abs(p.y - 0.0f) < 1e-3f);
        assert(std::abs(v.x - 1.0f) < 1e-3f);
    }

    // Case 2: Sliding along a wall (blocked in x, slides in y)
    {
        Point2 p = {0, 0};
        Vec2 v = {0, 0};
        Vec2 d = {3, 2};
        std::vector<Rect> obs = {{2, 0, 1, 10}}; // wall at x=2
        MoveResult r = simulateMove(p, v, d, obs, 18.0f, 0.7f);
        assert(r == MoveResult::SLIDING || r == MoveResult::STEPPED); // may step if beneficial
        // With step height 18, stepping is not beneficial (wall is tall), so should slide.
        assert(r == MoveResult::SLIDING);
        assert(std::abs(p.x - 2.0f) < 1e-3f);
        assert(std::abs(p.y - 2.0f) < 1e-3f);
    }

    // Case 3: Step up onto a small step
    {
        Point2 p = {0, 0};
        Vec2 v = {1, 0};
        Vec2 d = {3, 0};
        std::vector<Rect> obs = {{2, 1, 1, 1}}; // step from y=1 to y=2
        MoveResult r = simulateMove(p, v, d, obs, 18.0f, 0.7f);
        assert(r == MoveResult::STEPPED);
        assert(p.x > 2.0f && p.x <= 3.0f);
        assert(std::abs(p.y - 2.0f) < 1e-3f); // stepped to top of step, then ground at y=2
    }

    // Case 4: No step if direct path is longer (e.g., moving away from obstacle)
    {
        Point2 p = {0, 0};
        Vec2 v = {1, 0};
        Vec2 d = {3, 0};
        std::vector<Rect> obs = {{2, 1, 1, 1}};
        MoveResult r = simulateMove(p, v, d, obs, 18.0f, 0.7f);
        // Direct move would slide along the front face and end at x=2, y=0
        // Step move would go to top, ending further away, so it should step.
        // But wait: direct distance = 2, stepped distance = 3, so step is accepted.
        assert(r == MoveResult::STEPPED);
    }

    // Case 5: Delta zero returns OK immediately
    {
        Point2 p = {1, 1};
        Vec2 v = {0, 0};
        Vec2 d = {0, 0};
        std::vector<Rect> obs = {{0,0,10,10}};
        MoveResult r = simulateMove(p, v, d, obs, 18.0f, 0.7f);
        assert(r == MoveResult::OK);
        assert(p.x == 1.0f && p.y == 1.0f);
    }

    // Case 6: Blocked by a full wall (no step possible), returns BLOCKED or SLIDING
    {
        Point2 p = {0, 0};
        Vec2 v = {0, 0};
        Vec2 d = {5, 0};
        std::vector<Rect> obs = {{3, -10, 1, 20}}; // tall wall
        MoveResult r = simulateMove(p, v, d, obs, 18.0f, 0.7f);
        // After 3 iterations, blocked; but since stepping up also blocked, returns result1 (likely BLOCKED)
        assert(r == MoveResult::BLOCKED || r == MoveResult::SLIDING);
        assert(std::abs(p.x - 3.0f) < 1e-3f);
    }

    // Case 7: Step down from a ledge (direct move goes off edge, then step down)
    {
        Point2 p = {0, 0};
        Vec2 v = {1, 0};
        Vec2 d = {3, 0};
        std::vector<Rect> obs = {{1, -10, 10, 10}}; // floor starts at y=0 (top at y=-10), no obstacle above
        // Actually, to simulate a ledge, place floor from x=0 to x=1 at y=0, then drop
        std::vector<Rect> obs2 = {{0, 0, 1, 1}, {1, 1, 10, 1}}; // step down
        MoveResult r = simulateMove(p, v, d, obs2, 18.0f, 0.7f);
        // Direct move OK, then step down to lower floor
        assert(r == MoveResult::STEPPED);
        assert(p.y > 0.0f); // moved down
    }
}

#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

struct Point2 {
    float x, y;
};

struct Vec2 {
    float x, y;
};

struct Rect {
    float x, y, w, h; // x,y is top-left corner; y increases downward
};

enum class MoveResult {
    OK,
    SLIDING,
    STEPPED,
    BLOCKED,
    FALLING
};

static const float OVERCLIP = 1.001f;
static const float EPSILON = 1e-6f;

// Helper: raycast a straight segment from origin (start) along direction (dir, assumed unit length not required)
// against a list of AABBs. Returns the nearest hit t in [0,1] and sets normal to the face normal (unit axis aligned).
static bool raycast(const Point2& start, const Vec2& dir, const std::vector<Rect>& obstacles, float& outT, Vec2& outNormal) {
    bool hit = false;
    float bestT = 1.0f;
    Vec2 bestNormal = {0,0};
    float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    if (len < EPSILON) {
        outT = 1.0f;
        outNormal = {0,0};
        return false;
    }
    Vec2 invDir = {1.0f / dir.x, 1.0f / dir.y};

    for (const auto& r : obstacles) {
        float tmin = 0.0f, tmax = 1.0f;
        Vec2 normal = {0,0};
        // X slab
        float tx1 = (r.x - start.x) * invDir.x;
        float tx2 = (r.x + r.w - start.x) * invDir.x;
        if (tx1 > tx2) std::swap(tx1, tx2);
        float nx = (tx1 == (r.x - start.x) * invDir.x) ? -1.0f : 1.0f;
        if (tx1 > tmin) { tmin = tx1; normal = {nx, 0}; }
        if (tx2 < tmax) tmax = tx2;
        // Y slab
        float ty1 = (r.y - start.y) * invDir.y;
        float ty2 = (r.y + r.h - start.y) * invDir.y;
        if (ty1 > ty2) std::swap(ty1, ty2);
        float ny = (ty1 == (r.y - start.y) * invDir.y) ? -1.0f : 1.0f;
        if (ty1 > tmin) { tmin = ty1; normal = {0, ny}; }
        if (ty2 < tmax) tmax = ty2;

        if (tmin <= tmax && tmin < bestT) {
            bestT = tmin;
            bestNormal = normal;
            hit = true;
        }
    }

    outT = bestT;
    outNormal = bestNormal;
    return hit && bestT < 1.0f - EPSILON;
}

// Helper: overlap test for a point inside a rectangle (for ground checks, use a small downward ray)
static bool pointInRect(const Point2& p, const Rect& r) {
    return p.x >= r.x && p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h;
}

// Helper: ground raycast downward (gravity direction (0,1)) from pos by distance d. Returns true if hits a floor,
// and outputs the floor normal and the collision point.
static bool groundRay(const Point2& pos, float d, const std::vector<Rect>& obstacles, Point2& groundPos, Vec2& groundNormal) {
    float t;
    Vec2 n;
    Vec2 dir = {0, 1.0f};
    Point2 end = {pos.x, pos.y + d};
    Vec2 delta = {end.x - pos.x, end.y - pos.y};
    // Use raycast: simplest is to cast a tiny segment; here we manually check if the segment crosses any rectangle top face.
    // Since gravity is axis-aligned, we can just check if any rectangle overlaps the vertical segment.
    bool hit = false;
    float bestDist = d;
    Vec2 bestN = {0, -1}; // default floor normal is up
    for (const auto& r : obstacles) {
        // Check if segment x overlaps rectangle x range
        if (pos.x >= r.x && pos.x <= r.x + r.w) {
            // Intersection with the top face of the rectangle (y = r.y)
            if (pos.y <= r.y && pos.y + d >= r.y) {
                float dist = r.y - pos.y;
                if (dist < bestDist) {
                    bestDist = dist;
                    bestN = {0, -1}; // floor normal points up (negative y because y increases down)
                    hit = true;
                }
            }
        }
    }
    if (hit) {
        groundPos = {pos.x, pos.y + bestDist};
        groundNormal = bestN;
        return true;
    }
    return false;
}

// Helper: normal projection of vector v onto plane with normal n, with overclip factor
static void projectOntoPlane(Vec2& v, const Vec2& n) {
    float dot = v.x * n.x + v.y * n.y;
    v.x = v.x - dot * n.x * OVERCLIP;
    v.y = v.y - dot * n.y * OVERCLIP;
}

// SlideMove: attempts to move from start by delta, clipping velocity and remaining move against obstacles.
// Returns final position and velocity. Returns MoveResult: OK, SLIDING, or BLOCKED.
static MoveResult slideMove(Point2& start, Vec2& velocity, const Vec2& delta, const std::vector<Rect>& obstacles) {
    Vec2 move = delta;
    for (int i = 0; i < 3; ++i) {
        float t;
        Vec2 normal;
        bool hit = raycast(start, move, obstacles, t, normal);
        start.x += move.x * t;
        start.y += move.y * t;

        if (!hit) {
            if (i > 0) return MoveResult::SLIDING;
            return MoveResult::OK;
        }

        // Remaining move after collision
        move.x = move.x * (1.0f - t);
        move.y = move.y * (1.0f - t);

        // Project velocity and remaining delta onto collision normal
        projectOntoPlane(move, normal);
        projectOntoPlane(velocity, normal);
    }
    return MoveResult::BLOCKED;
}

// Main simulation function: attempts step move (with stepping up) and returns final position and velocity.
MoveResult simulateMove(Point2& pos, Vec2& velocity, const Vec2& delta, const std::vector<Rect>& obstacles, float maxStepHeight, float minFloorCosine) {
    if (delta.x == 0.0f && delta.y == 0.0f) {
        return MoveResult::OK;
    }

    Point2 noStepPos = pos;
    Vec2 noStepVel = velocity;
    MoveResult result1 = slideMove(noStepPos, noStepVel, delta, obstacles);
    if (result1 == MoveResult::OK) {
        velocity = noStepVel;
        // Try to step down
        Point2 groundPos;
        Vec2 groundNormal;
        if (groundRay(noStepPos, maxStepHeight, obstacles, groundPos, groundNormal)) {
            pos = groundPos;
            return MoveResult::STEPPED;
        } else {
            pos = noStepPos;
            return MoveResult::OK;
        }
    }

    // Try to step up: move upward by maxStepHeight
    Point2 upPos = pos;
    upPos.y -= maxStepHeight;
    // Check if stepping up is blocked immediately; we use a raycast upward
    float t;
    Vec2 normal;
    Vec2 upDelta = {0, -maxStepHeight};
    bool upBlocked = raycast(pos, upDelta, obstacles, t, normal);
    if (t == 0.0f) { // blocked immediately
        pos = noStepPos;
        velocity = noStepVel;
        return result1;
    }
    Point2 stepPos = upPos;
    Vec2 stepVel = velocity;
    MoveResult result2 = slideMove(stepPos, stepVel, delta, obstacles);
    if (result2 == MoveResult::BLOCKED) {
        pos = noStepPos;
        velocity = noStepVel;
        return result1;
    }

    // Step down from the elevated position
    Point2 groundPos;
    Vec2 groundNormal;
    if (groundRay(stepPos, maxStepHeight, obstacles, groundPos, groundNormal)) {
        stepPos = groundPos;
    }

    // Compare distances
    float nostepdist = (noStepPos.x - pos.x)*(noStepPos.x - pos.x) + (noStepPos.y - pos.y)*(noStepPos.y - pos.y);
    float stepdist = (stepPos.x - pos.x)*(stepPos.x - pos.x) + (stepPos.y - pos.y)*(stepPos.y - pos.y);
    // Floor cosine: since gravity normal is (0,1) downward, -gravityNormal = (0,-1), so cosine = -normal.y
    float cosine = -groundNormal.y;
    if (nostepdist >= stepdist || cosine < minFloorCosine) {
        pos = noStepPos;
        velocity = noStepVel;
        return MoveResult::SLIDING;
    }

    pos = stepPos;
    velocity = stepVel;
    return MoveResult::STEPPED;
}

// The solution mirrors the original Doom 3 physics code but simplifies to 2D and axis-aligned rectangles. The main algorithm consists of two phases. First, attempt a `SlideMove`: loop up to 3 times, each time performing a raycast from the current position along the remaining delta against all obstacles. For raycasting against an AABB, treat the rectangle as solid; compute the first intersection point using a standard slab method. If no collision (`t >= 1`), the move completes: if this was the first iteration, return `OK`; otherwise, return `SLIDING`. If collisions occur, project the remaining delta and velocity onto the collision normal (the normal of the rectangle face hit, i.e., ±x or ±y) using the formula `v = v - (v·n)*n * OVERCLIP`, and continue. If after 3 iterations still blocked, return `BLOCKED`.
//
// If the initial `SlideMove` fails and the obstacle blocking is an actor (in this task, we ignore this branch; treat all obstacles as static), skip that special case. Otherwise, attempt `StepMove`: first compute the direct no-step result (already done by the initial `SlideMove`). If the direct result was `OK`, try to step down: from the final position of the direct move, raycast downward by `maxStepHeight`; if a floor is found, move there and return `STEPPED`; else keep the no-step position and return `OK`. If the direct result failed, try stepping up: raycast from the start position upward by `maxStepHeight`; if blocked immediately (fraction zero), return the direct result. Otherwise, at the elevated position, run `SlideMove` again. If that second slide move returns `BLOCKED`, revert to the direct no-step position/velocity and return the direct result. Otherwise, step down from the highest final position by raycasting downward `maxStepHeight`; if a floor is found, set the final position there. Then compare squared horizontal distances: `nostepdist = (noStepPos - start).LengthSqr()` and `stepdist = (stepPos - start).LengthSqr()`. Accept the stepped path only if `stepdist > nostepdist` and the floor normal cosine (dot of floor normal with -gravity, which in 2D is `normal.y` since gravity is (0,1), so cosine = `-normal.y`) is at least `minFloorCosine`. If accepted, return `STEPPED`; else revert to no-step and return `SLIDING`. The gravity normal is (0,-1) in 2D (down), so `-gravityNormal` is (0,1), and the floor cosine is `normal.y` (the y component of the hit normal). Edge cases include delta zero (return `OK`), obstacles that exactly touch, and step heights that are too large relative to obstacles (should not step when not beneficial). Time complexity: each raycast is O(R) for R rectangles, and at most 2 slide moves (each up to 3 iterations) plus up to 3 ground raycasts, so O(R) time; space O(1) auxiliary. The function must handle the overclip constant as `1.001f`.

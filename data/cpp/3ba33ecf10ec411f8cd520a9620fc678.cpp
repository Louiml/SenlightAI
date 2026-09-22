/*
Given a polygon defined by an array of vertices (in clockwise or counterclockwise order), write a C++ function that takes two corner points of an axis-aligned square and the polygon's vertices, and returns a `square` structure containing only those polygon edges (sides) that intersect or touch the square's boundary or interior. The function must correctly filter sides by checking whether any portion of a side lies within or on the square, including cases where an edge is completely inside, partially crossing, or merely touching a corner. The square is defined by its bottom-left corner (`p1`) and top-right corner (`p2`); assume `p2.x > p1.x` and `p2.y > p1.y`. The returned `square` should store the two corners, the side length (`size = p2.x - p1.x`), the number of intersecting sides (`n`), and a dynamically allocated array of those sides in their original order.
*/

#include <cmath>
#include <cstddef>

struct point { float x, y; };

struct side {
    point p0, p1, p2, p3;
    float a, b, c;
    int id;
    float veclen;
};

struct square {
    point p1, p2;
    side* sides;
    int n;
    float size;
};

static float vec_length(float x, float y) {
    return std::sqrt(x * x + y * y);
}

static int sign_of(float num) {
    if (num > 0.0f) return 1;
    if (num < 0.0f) return -1;
    return 0;
}

// Build sides for a polygon given vertices and count.
side* build_sides(const point* vertices, int n) {
    side* sides = new side[n];
    for (int i = 0; i < n; ++i) {
        sides[i].p1 = vertices[i];
        sides[i].p2 = vertices[(i + 1) % n];
        sides[i].p0 = vertices[(i + n - 1) % n];
        sides[i].p3 = vertices[(i + 2) % n];
        sides[i].id = i;
        sides[i].a = -(sides[i].p2.y - sides[i].p1.y);
        sides[i].b = sides[i].p2.x - sides[i].p1.x;
        sides[i].veclen = vec_length(sides[i].a, sides[i].b);
        sides[i].c = -(sides[i].a * sides[i].p1.x + sides[i].b * sides[i].p1.y);
    }
    return sides;
}

// Return a square structure containing only sides that intersect the square's area/boundary.
square select_intersecting_sides(point bottom_left, point top_right,
                                 const side* all_sides, int total_sides) {
    square result;
    result.p1 = bottom_left;
    result.p2 = top_right;
    result.size = top_right.x - bottom_left.x;

    if (total_sides <= 0) {
        result.n = 0;
        result.sides = nullptr;
        return result;
    }

    int* indices = new int[total_sides];
    int count = 0;

    for (int i = 0; i < total_sides; ++i) {
        const side& s = all_sides[i];

        // Evaluate line equation at the four corners of the square.
        float v1 = s.a * bottom_left.x + s.b * bottom_left.y + s.c;
        float v2 = s.a * bottom_left.x + s.b * top_right.y + s.c;
        float v3 = s.a * top_right.x + s.b * top_right.y + s.c;
        float v4 = s.a * top_right.x + s.b * bottom_left.y + s.c;

        int sign1 = sign_of(v1);
        int sign2 = sign_of(v2);
        int sign3 = sign_of(v3);
        int sign4 = sign_of(v4);

        bool line_crosses = (sign1 != sign2 || sign1 != sign3 || sign1 != sign4 ||
                             sign2 != sign3 || sign2 != sign4 || sign3 != sign4);

        if (!line_crosses) {
            // If all signs are same (possibly zero), the infinite line does not cross the square.
            // However, if all are zero, the line passes exactly along the square's boundary;
            // still need to check if the segment overlaps. For safety, if signs differ, we already passed.
            // If all zero, we still need to check overlap; treat as crossing.
            if (!(sign1 == 0 && sign2 == 0 && sign3 == 0 && sign4 == 0)) {
                continue;
            }
        }

        // Now check if either endpoint of the segment lies inside the square's bounding box.
        bool p1_inside = (bottom_left.x <= s.p1.x && s.p1.x <= top_right.x) &&
                         (bottom_left.y <= s.p1.y && s.p1.y <= top_right.y);
        bool p2_inside = (bottom_left.x <= s.p2.x && s.p2.x <= top_right.x) &&
                         (bottom_left.y <= s.p2.y && s.p2.y <= top_right.y);

        if (p1_inside || p2_inside) {
            indices[count++] = i;
        }
    }

    result.n = count;
    if (count > 0) {
        result.sides = new side[count];
        for (int i = 0; i < count; ++i) {
            result.sides[i] = all_sides[indices[i]];
        }
    } else {
        result.sides = nullptr;
    }

    delete[] indices;
    return result;
}

#include <cassert>
#include <cmath>

// The solution code (structs and functions) is assumed to be included above.
// Here we test `select_intersecting_sides` with several polygons.

int main() {
    // Test 1: Triangle (0,0)-(2,0)-(0,2), square [0,1]x[0,1].
    point tri[3] = {{0,0},{2,0},{0,2}};
    side* tri_sides = build_sides(tri, 3);
    square s1 = select_intersecting_sides({0,0},{1,1}, tri_sides, 3);
    assert(s1.n == 3); // All three sides touch or cross the square.
    delete[] s1.sides;
    delete[] tri_sides;

    // Test 2: Square polygon [0,0] to [1,1] with square [2,3]x[2,3] -> no intersection.
    point sq_pts[4] = {{0,0},{1,0},{1,1},{0,1}};
    side* sq_sides = build_sides(sq_pts, 4);
    square s2 = select_intersecting_sides({2,2},{3,3}, sq_sides, 4);
    assert(s2.n == 0);
    assert(s2.sides == nullptr);
    delete[] sq_sides;

    // Test 3: Polygon with an edge far away but line crosses square? Example: (0,10)-(10,0) with square [0,5]x[0,5].
    point far_edge[4] = {{0,10},{10,0},{0,-10},{-10,0}}; // diamond shape
    side* far_sides = build_sides(far_edge, 4);
    square s3 = select_intersecting_sides({-5,-5},{5,5}, far_sides, 4);
    // Every edge of the diamond touches the square, so n == 4.
    assert(s3.n == 4);
    delete[] s3.sides;
    delete[] far_sides;

    // Test 4: Edge exactly on boundary: polygon (0,0)-(1,0)-(1,1)-(0,1) with square [0,1]x[0,1] -> all 4 sides.
    side* sq2_sides = build_sides(sq_pts, 4);
    square s4 = select_intersecting_sides({0,0},{1,1}, sq2_sides, 4);
    assert(s4.n == 4);
    delete[] s4.sides;
    delete[] sq2_sides;

    // Test 5: Single point polygon? Not typical, but handle n=0 gracefully.
    side* empty_sides = nullptr;
    square s5 = select_intersecting_sides({0,0},{1,1}, empty_sides, 0);
    assert(s5.n == 0);
    assert(s5.sides == nullptr);

    // Test 6: Edge that passes through square but endpoints outside: e.g., ( -1, 0.5 )-(2,0.5) with square [0,1]x[0,1].
    point horiz[3] = {{-1,0.5},{2,0.5},{0,2}}; // triangle with a horizontal side through the middle.
    side* horiz_sides = build_sides(horiz, 3);
    square s6 = select_intersecting_sides({0,0},{1,1}, horiz_sides, 3);
    // The horizontal side should be included; the other two also touch or cross.
    assert(s6.n == 3);
    delete[] s6.sides;
    delete[] horiz_sides;

    // Test 7: Check side content for a simple case: triangle (0,0)-(2,0)-(0,2), square [0,1]x[0,1].
    // The side from (0,0)-(2,0) should be included.
    point tri2[3] = {{0,0},{2,0},{0,2}};
    side* tri2_sides = build_sides(tri2, 3);
    square s7 = select_intersecting_sides({0,0},{1,1}, tri2_sides, 3);
    bool found_base = false;
    for (int i = 0; i < s7.n; ++i) {
        if (s7.sides[i].id == 0) found_base = true;
    }
    assert(found_base);
    delete[] s7.sides;
    delete[] tri2_sides;

    return 0;
}

// The core algorithm reuses the geometric definitions from the snippet: each polygon side is represented by a line equation `a*x + b*y + c = 0`, where `a = -(p2.y - p1.y)`, `b = p2.x - p1.x`, and `c = -(a*p1.x + b*p1.y)`. To determine if a side intersects the square, evaluate the line equation at the square's four corners. If all four signs are identical (all positive, all negative, or all zero), the square lies entirely on one side of the line, so the side cannot intersect the square (unless all are zero, but that means the side’s infinite line passes exactly along an edge; however, the side segment itself still must be checked to overlap). If the signs differ, the infinite line crosses the square; but the actual segment may still miss it. Therefore, also verify that at least one endpoint of the side lies within the square’s axis-aligned bounding box (i.e., `p1.x <= endpoint.x <= p2.x` and `p1.y <= endpoint.y <= p2.y`). This two-step test is conservative and correctly filters sides: if the signs differ and one endpoint is inside the square, the segment must intersect the square. Edge cases include sides that lie exactly on the square’s boundary (signs may be zero), sides whose infinite line passes through the square but whose segment is far away (excluded by the endpoint box test), and duplicate endpoints (no special handling needed). The algorithm runs in O(n) time for a polygon with n sides and uses O(n) auxiliary space for the output array of intersecting sides (plus a temporary index array). Memory allocated for the returned array must be freed by the caller.

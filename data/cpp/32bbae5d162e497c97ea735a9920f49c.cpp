Write a C++ function named `generateSierpinskiTriangle` that takes an integer recursion depth `n` (where `n >= 1`) and the three vertices of an equilateral triangle as `Vertex` structs (each containing a `glm::vec3` for position and a `glm::vec3` for color). The function must recursively subdivide the triangle into 3 smaller triangles (top, bottom-left, bottom-right) by connecting the midpoints of the original triangle's edges, and at the deepest recursion level (when `n == 1`), assign a single random color to all three vertices of that smallest triangle. Return a `std::vector<Vertex>` containing all vertices of the final Sierpinski triangle (i.e., 3 vertices for each of the `3^(n-1)` smallest triangles at depth `n`), with the recursion order being top triangle first, then left, then right. Use a global random number generator seeded once at program startup, and a helper function `getRandomColor()` that returns a random `glm::vec3` with components between 0 and 1. The function must not use any external libraries beyond standard C++ and a minimal `glm`-like vector definition (you may use a simple struct with three `float` members instead of glm). The function should be `const`-correct where applicable and must not have any side effects other than reading randomness.

#include <cassert>
#include <vector>
#include <iostream>

// Include the solution's definitions here (for demonstration; in practice, put it in a header)

int main() {
    // Seed random generator once (used by getRandomColor)
    std::srand(static_cast<unsigned>(std::time(0)));

    // Define an initial equilateral triangle (2D in x-y, z=0)
    Vec3 upPos(0.0f, 1.0f, 0.0f);
    Vec3 leftPos(-1.0f, -0.5f, 0.0f);
    Vec3 rightPos(1.0f, -0.5f, 0.0f);
    Vertex up(upPos, Vec3(1,1,1));
    Vertex left(leftPos, Vec3(1,1,1));
    Vertex right(rightPos, Vec3(1,1,1));

    // Depth 1: exactly one triangle, 3 vertices
    std::vector<Vertex> gen1 = generateSierpinskiTriangle(1, up, left, right);
    assert(gen1.size() == 3);
    // The three vertices should match exactly the input positions
    assert(gen1[0].pos.x == 0.0f && gen1[0].pos.y == 1.0f);
    assert(gen1[1].pos.x == -1.0f && gen1[1].pos.y == -0.5f);
    assert(gen1[2].pos.x == 1.0f && gen1[2].pos.y == -0.5f);
    // The color must be identical for all three vertices
    assert(gen1[0].color.x == gen1[1].color.x && gen1[0].color.x == gen1[2].color.x);
    assert(gen1[0].color.y == gen1[1].color.y && gen1[0].color.y == gen1[2].color.y);
    assert(gen1[0].color.z == gen1[1].color.z && gen1[0].color.z == gen1[2].color.z);
    // Colors are in [0,1] range
    for (size_t i = 0; i < 3; ++i) {
        assert(gen1[i].color.x >= 0.0f && gen1[i].color.x <= 1.0f);
        assert(gen1[i].color.y >= 0.0f && gen1[i].color.y <= 1.0f);
        assert(gen1[i].color.z >= 0.0f && gen1[i].color.z <= 1.0f);
    }

    // Depth 2: 3 triangles => 9 vertices
    std::vector<Vertex> gen2 = generateSierpinskiTriangle(2, up, left, right);
    assert(gen2.size() == 9);

    // Depth 3: 9 triangles => 27 vertices
    std::vector<Vertex> gen3 = generateSierpinskiTriangle(3, up, left, right);
    assert(gen3.size() == 27);

    // Depth 4: 27 triangles => 81 vertices
    std::vector<Vertex> gen4 = generateSierpinskiTriangle(4, up, left, right);
    assert(gen4.size() == 81);

    // Check that all vertices have coordinates that are sums of powers of 1/2 (i.e., not NaN)
    for (const auto& v : gen4) {
        assert(v.pos.x == v.pos.x); // not NaN
        assert(v.pos.y == v.pos.y);
        assert(v.pos.z == v.pos.z);
    }

    std::cout << "All assertions passed!" << std::endl;
    return 0;
}

#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>

// Minimal 3D vector type (replaces glm::vec3)
struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }
};

// Vertex structure: position and color
struct Vertex {
    Vec3 pos;
    Vec3 color;
    Vertex() : pos(), color() {}
    Vertex(const Vec3& p, const Vec3& c) : pos(p), color(c) {}
};

// Helper: generate a random color (components between 0 and 1)
Vec3 getRandomColor() {
    return Vec3(
        static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX),
        static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX),
        static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)
    );
}

/**
 * Recursively generate the vertices of a Sierpinski triangle.
 * @param n recursion depth (>= 1)
 * @param up top vertex
 * @param left bottom-left vertex
 * @param right bottom-right vertex
 * @return vector of vertices: for each smallest triangle, 3 vertices (up, left, right)
 */
std::vector<Vertex> generateSierpinskiTriangle(int n, const Vertex& up, const Vertex& left, const Vertex& right) {
    assert(n >= 1);
    std::vector<Vertex> vertices;

    if (n == 1) {
        Vec3 color = getRandomColor();
        Vertex vUp(up.pos, color);
        Vertex vLeft(left.pos, color);
        Vertex vRight(right.pos, color);
        vertices.push_back(vUp);
        vertices.push_back(vLeft);
        vertices.push_back(vRight);
        return vertices;
    }

    // Compute midpoints
    Vec3 upLeft = (left.pos + up.pos) / 2.0f;
    Vec3 upRight = (right.pos + up.pos) / 2.0f;
    Vec3 leftRight = (right.pos + left.pos) / 2.0f;

    // Top sub-triangle
    Vertex topUp(up.pos, Vec3());
    Vertex topLeft(upLeft, Vec3());
    Vertex topRight(upRight, Vec3());
    auto topVertices = generateSierpinskiTriangle(n - 1, topUp, topLeft, topRight);
    vertices.insert(vertices.end(), topVertices.begin(), topVertices.end());

    // Left sub-triangle
    Vertex leftUp(upLeft, Vec3());
    Vertex leftLeft(left.pos, Vec3());
    Vertex leftRightV(leftRight, Vec3());
    auto leftVertices = generateSierpinskiTriangle(n - 1, leftUp, leftLeft, leftRightV);
    vertices.insert(vertices.end(), leftVertices.begin(), leftVertices.end());

    // Right sub-triangle
    Vertex rightUp(upRight, Vec3());
    Vertex rightLeft(leftRight, Vec3());
    Vertex rightRight(right.pos, Vec3());
    auto rightVertices = generateSierpinskiTriangle(n - 1, rightUp, rightLeft, rightRight);
    vertices.insert(vertices.end(), rightVertices.begin(), rightVertices.end());

    return vertices;
}

// The solution follows a classic divide-and-conquer recursion. The base case is when the depth `n` equals 1: at that point, the three given vertices form a single smallest triangle, and we assign a random color to all three (so the triangle has uniform color) and return them in order (up, left, right). For deeper recursion, we compute the three midpoints: `upLeft = (left + up)/2`, `upRight = (right + up)/2`, `leftRight = (right + left)/2`. Then we define the three sub-triangles:
// - Top: vertices (up, upLeft, upRight)
// - Left: vertices (upLeft, left, leftRight)
// - Right: vertices (upRight, leftRight, right)
// We recursively call the function on each sub-triangle with depth `n-1`, concatenating the results in the order top, left, right. This yields exactly `3^(n-1)` smallest triangles at depth `n`, each contributing 3 vertices, so the total output size is `3^n` vertices. Edge cases: `n` must be positive; if `n` is 0 the function’s behavior is undefined and we can assert it. Randomness: use `std::rand()` seeded once (e.g., via `std::srand(std::time(0))` in the test's `main`), but the function itself should not reseed. The color of a smallest triangle is chosen per base case, so each detail at the finest level is uniformly colored. Time complexity is `O(3^n)` because each recursion level multiplies the number of calls by 3, and each call either does O(1) work (base case) or O(3) work to compute midpoints and concatenate vectors. Space complexity is also `O(3^n)` for the returned vector, plus recursion stack depth `O(n)`. For `n` up to about 15, the vector size is `3^15 ≈ 14 million` vertices, which might be large but is manageable with 64-bit systems.

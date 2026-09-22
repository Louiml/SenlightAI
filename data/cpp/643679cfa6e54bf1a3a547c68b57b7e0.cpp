/*
Write a C++ function named `computeTangentBasis` that takes four `std::vector` references as inputs: `vertices` (list of 3D positions), `uvs` (list of 2D texture coordinates), and `normals` (list of 3D normals), and produces two output vectors `tangents` and `bitangents` (both of 3D vectors). The inputs are assumed to describe a triangle list where every consecutive group of three vertices forms a triangle, and each vertex has a corresponding UV coordinate and normal. The function must compute per-vertex tangent and bitangent vectors for each triangle using the standard tangent-space derivation: calculate the position and UV deltas for the triangle’s edges, solve for tangent and bitangent using the UV delta determinant (guard against division by zero by skipping degenerate UV triangles), assign the same tangent and bitangent to all three vertices of the triangle, then orthonormalize each tangent against its corresponding normal using Gram-Schmidt, and finally adjust the tangent’s handedness by checking the sign of the cross product of the normal and tangent against the bitangent. The output vectors should be cleared and resized to match the number of vertices before filling. Use `glm::vec3` and `glm::vec2` types from the GLM library (include `<glm/glm.hpp>`), and apply `const` correctness where appropriate for input parameters.
*/
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

/**
 * Compute per-vertex tangent and bitangent vectors for a triangle list.
 * @param vertices  Input positions (size must be a multiple of 3)
 * @param uvs       Input texture coordinates (same size as vertices)
 * @param normals   Input per-vertex normals (same size as vertices)
 * @param tangents  Output tangents (will be resized to vertices.size())
 * @param bitangents Output bitangents (will be resized to vertices.size())
 */
void computeTangentBasis(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec2>& uvs,
    const std::vector<glm::vec3>& normals,
    std::vector<glm::vec3>& tangents,
    std::vector<glm::vec3>& bitangents
) {
    // Clear and pre-size outputs
    tangents.clear();
    bitangents.clear();
    tangents.reserve(vertices.size());
    bitangents.reserve(vertices.size());

    for (size_t i = 0; i + 2 < vertices.size(); i += 3) {
        const glm::vec3& v0 = vertices[i];
        const glm::vec3& v1 = vertices[i + 1];
        const glm::vec3& v2 = vertices[i + 2];

        const glm::vec2& uv0 = uvs[i];
        const glm::vec2& uv1 = uvs[i + 1];
        const glm::vec2& uv2 = uvs[i + 2];

        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float determinant = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
        glm::vec3 tangent(0.0f);
        glm::vec3 bitangent(0.0f);

        // Guard against degenerate UV triangles
        if (std::fabs(determinant) > 1e-8f) {
            float r = 1.0f / determinant;
            tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
        }

        // Push same tangent/bitangent for all three vertices
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    // Orthonormalize tangents per vertex
    for (size_t i = 0; i < vertices.size(); ++i) {
        const glm::vec3& n = normals[i];
        glm::vec3& t = tangents[i];
        glm::vec3& b = bitangents[i];

        // Gram-Schmidt orthogonalize
        t = glm::normalize(t - n * glm::dot(n, t));

        // Adjust handedness
        if (glm::dot(glm::cross(n, t), b) < 0.0f) {
            t = -t;
        }
    }
}
#include <cassert>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

// Include the solution function here (or link it)
void computeTangentBasis(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec2>& uvs,
    const std::vector<glm::vec3>& normals,
    std::vector<glm::vec3>& tangents,
    std::vector<glm::vec3>& bitangents
);

int main() {
    // Test 1: Simple axis-aligned triangle with flat normal
    {
        std::vector<glm::vec3> verts = {
            glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,1,0)
        };
        std::vector<glm::vec2> uvs = {
            glm::vec2(0,0), glm::vec2(1,0), glm::vec2(0,1)
        };
        std::vector<glm::vec3> normals = {
            glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1)
        };
        std::vector<glm::vec3> t, b;
        computeTangentBasis(verts, uvs, normals, t, b);
        assert(t.size() == 3);
        // Tangent should be approx (1,0,0) for all vertices
        for (int i = 0; i < 3; ++i) {
            assert(glm::dot(t[i], glm::vec3(1,0,0)) > 0.99f);
            assert(glm::dot(b[i], glm::vec3(0,1,0)) > 0.99f);
        }
    }

    // Test 2: Degenerate UVs (zero area) → tangents should be zero
    {
        std::vector<glm::vec3> verts = {
            glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,1,0)
        };
        std::vector<glm::vec2> uvs = {
            glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0)
        };
        std::vector<glm::vec3> normals = {
            glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1)
        };
        std::vector<glm::vec3> t, b;
        computeTangentBasis(verts, uvs, normals, t, b);
        for (int i = 0; i < 3; ++i) {
            assert(glm::length(t[i]) < 1e-5f);
            assert(glm::length(b[i]) < 1e-5f);
        }
    }

    // Test 3: Larger mesh (two triangles) and normal not aligned with Z
    {
        std::vector<glm::vec3> verts = {
            glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,1,0),
            glm::vec3(1,0,0), glm::vec3(1,1,0), glm::vec3(0,1,0)
        };
        std::vector<glm::vec2> uvs = {
            glm::vec2(0,0), glm::vec2(1,0), glm::vec2(0,1),
            glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1)
        };
        std::vector<glm::vec3> normals(6, glm::vec3(0,0,1));
        std::vector<glm::vec3> t, b;
        computeTangentBasis(verts, uvs, normals, t, b);
        assert(t.size() == 6);
        assert(b.size() == 6);
        // All tangents should be orthogonal to normal
        for (size_t i = 0; i < t.size(); ++i) {
            assert(std::fabs(glm::dot(t[i], normals[i])) < 1e-5f);
        }
    }

    // Test 4: Handedness flip when bitangent points opposite to cross(n, t)
    {
        std::vector<glm::vec3> verts = {
            glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,1,0)
        };
        // Swap UVs to invert determinant
        std::vector<glm::vec2> uvs = {
            glm::vec2(0,1), glm::vec2(1,1), glm::vec2(0,0)
        };
        std::vector<glm::vec3> normals = {
            glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1)
        };
        std::vector<glm::vec3> t, b;
        computeTangentBasis(verts, uvs, normals, t, b);
        // Tangent should point in negative X direction
        assert(t[0].x < -0.99f);
    }

    // Test 5: Empty input should not crash
    {
        std::vector<glm::vec3> verts;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> t, b;
        computeTangentBasis(verts, uvs, normals, t, b);
        assert(t.empty());
        assert(b.empty());
    }

    return 0;
}
// The algorithm processes the input as a triangle list, iterating in steps of three vertices. For each triangle, it computes the edge vectors `deltaPos1 = v1 - v0` and `deltaPos2 = v2 - v0`, and the corresponding UV deltas `deltaUV1 = uv1 - uv0` and `deltaUV2 = uv2 - uv0`. The tangent and bitangent are derived from the equations:
// `tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) / r`
// `bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) / r`
// where `r = 1 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x)`. If the determinant (the denominator) is zero or extremely small, the UV triangle is degenerate (e.g., UVs are collinear or identical), and the tangent basis is undefined; in that case, we should skip the triangle (output zero vectors or a safe default) to avoid division by zero or NaN. After computing per-triangle basis, we push the same values for all three vertices. After processing all triangles, we iterate over every vertex and orthogonalize the tangent against the normal using `t = normalize(t - n * dot(n, t))`, then check handedness: if `dot(cross(n, t), b) < 0`, flip the tangent sign. Complexity: `O(V)` time where `V` is the number of vertices (since each vertex appears exactly once in a triangle list and we do constant work per triangle and per vertex), and `O(V)` auxiliary space for the output vectors (plus the input vectors themselves). Edge cases: degenerate UV triangles causing division by zero, zero-length normals, and triangles with zero area. The solution should also clear the output vectors before reuse, and resize to match input size after filling.

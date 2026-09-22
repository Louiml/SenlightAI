Write a C++ function named `computeTriangleCentroids` that accepts a raw interleaved vertex buffer represented as a `const float*`, a stride in bytes between consecutive vertices, a separate index buffer of type `const unsigned int*` (each triangle specified as three consecutive indices), the number of triangles, and a 3-component scaling vector. The function must compute the centroid (average of the three vertex positions, scaled component-wise by the scaling vector) for every triangle and store the results into an output array of `float` with 3 floats per triangle (x, y, z). The output array must be pre-allocated by the caller with a size of at least `numTriangles * 3`. The function must return `void` and should handle interleaved vertex data where each vertex starts at `vertexbase + index * stride` and contains at least three consecutive floats for x, y, z at the beginning of that stride segment.
// The core algorithm is straightforward: for each triangle `t` from `0` to `numTriangles-1`, read the three indices `i0 = indices[t*3]`, `i1 = indices[t*3+1]`, `i2 = indices[t*3+2]`. For each of these indices, locate the vertex float pointer as `const float* v = reinterpret_cast<const float*>(reinterpret_cast<const char*>(vertexbase) + index * stride)`. Then extract the x, y, z floats at positions 0, 1, 2 of that vertex. Apply scaling by multiplying each component by the corresponding scaling vector component. Sum the three vertices component-wise, then divide each sum by 3.0f to get the centroid. Write these three values into the output array at positions `t*3`, `t*3+1`, `t*3+2`. No special handling is required beyond ensuring that the stride is at least 12 bytes (3 floats) and that the indices are valid; the function trusts the caller for valid ranges. Time complexity is O(numTriangles) because each triangle requires constant work (6 vertex reads). Space complexity is O(1) auxiliary, excluding the output array provided by the caller.
#include <cstddef>

/**
 * Computes the centroid of each triangle from an interleaved vertex buffer.
 * 
 * @param vertexbase Pointer to the first byte of the vertex buffer.
 * @param stride     Byte offset between consecutive vertices.
 * @param indices    Array of triangle indices (3 per triangle).
 * @param numTriangles Number of triangles to process.
 * @param scaling    3-component scaling vector (applied to each vertex coordinate).
 * @param outCentroids Output array with at least numTriangles*3 floats.
 */
void computeTriangleCentroids(
    const float* vertexbase,
    std::size_t stride,
    const unsigned int* indices,
    std::size_t numTriangles,
    const float scaling[3],
    float* outCentroids)
{
    for (std::size_t t = 0; t < numTriangles; ++t)
    {
        const unsigned int i0 = indices[t * 3];
        const unsigned int i1 = indices[t * 3 + 1];
        const unsigned int i2 = indices[t * 3 + 2];

        // Retrieve vertex positions, applying scaling.
        const float* v0 = reinterpret_cast<const float*>(
            reinterpret_cast<const char*>(vertexbase) + i0 * stride);
        const float* v1 = reinterpret_cast<const float*>(
            reinterpret_cast<const char*>(vertexbase) + i1 * stride);
        const float* v2 = reinterpret_cast<const float*>(
            reinterpret_cast<const char*>(vertexbase) + i2 * stride);

        float cx = (v0[0] + v1[0] + v2[0]) * scaling[0] / 3.0f;
        float cy = (v0[1] + v1[1] + v2[1]) * scaling[1] / 3.0f;
        float cz = (v0[2] + v1[2] + v2[2]) * scaling[2] / 3.0f;

        outCentroids[t * 3]     = cx;
        outCentroids[t * 3 + 1] = cy;
        outCentroids[t * 3 + 2] = cz;
    }
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: simple triangle with unit scaling, stride = 3 floats
    float vertices1[] = {
        0.0f, 0.0f, 0.0f,
        3.0f, 0.0f, 0.0f,
        0.0f, 3.0f, 0.0f
    };
    unsigned int indices1[] = {0, 1, 2};
    float scaling1[] = {1.0f, 1.0f, 1.0f};
    float out1[3];
    computeTriangleCentroids(vertices1, sizeof(float)*3, indices1, 1, scaling1, out1);
    assert(std::fabs(out1[0] - 1.0f) < 1e-6f);
    assert(std::fabs(out1[1] - 1.0f) < 1e-6f);
    assert(std::fabs(out1[2] - 0.0f) < 1e-6f);

    // Test 2: two triangles with non-unit scaling and stride = 5 floats (interleaved extra data)
    float vertices2[] = {
        1.0f, 2.0f, 3.0f, 99.0f, 98.0f, // vertex 0
        4.0f, 5.0f, 6.0f, 97.0f, 96.0f, // vertex 1
        7.0f, 8.0f, 9.0f, 95.0f, 94.0f, // vertex 2
        10.0f, 11.0f, 12.0f, 93.0f, 92.0f // vertex 3
    };
    unsigned int indices2[] = {0, 1, 2, 1, 2, 3};
    float scaling2[] = {2.0f, 0.5f, 3.0f};
    float out2[6];
    computeTriangleCentroids(vertices2, sizeof(float)*5, indices2, 2, scaling2, out2);
    // Triangle 0: vertices 0,1,2 -> avg (4,5,6) scaled -> (8, 2.5, 18)
    assert(std::fabs(out2[0] - 8.0f) < 1e-6f);
    assert(std::fabs(out2[1] - 2.5f) < 1e-6f);
    assert(std::fabs(out2[2] - 18.0f) < 1e-6f);
    // Triangle 1: vertices 1,2,3 -> avg (7,8,9) scaled -> (14, 4.0, 27)
    assert(std::fabs(out2[3] - 14.0f) < 1e-6f);
    assert(std::fabs(out2[4] - 4.0f) < 1e-6f);
    assert(std::fabs(out2[5] - 27.0f) < 1e-6f);

    // Test 3: zero triangles should do nothing (no crash)
    float vertices3[] = {0.0f, 0.0f, 0.0f};
    unsigned int indices3[] = {};
    float scaling3[] = {1.0f, 1.0f, 1.0f};
    float out3[1] = {123.0f};
    computeTriangleCentroids(vertices3, sizeof(float)*3, indices3, 0, scaling3, out3);
    assert(out3[0] == 123.0f);

    // Test 4: degenerate triangle where all vertices are the same
    float vertices4[] = {5.0f, 5.0f, 5.0f};
    unsigned int indices4[] = {0, 0, 0};
    float scaling4[] = {1.0f, 2.0f, 0.5f};
    float out4[3];
    computeTriangleCentroids(vertices4, sizeof(float)*3, indices4, 1, scaling4, out4);
    assert(std::fabs(out4[0] - 5.0f) < 1e-6f);
    assert(std::fabs(out4[1] - 10.0f) < 1e-6f);
    assert(std::fabs(out4[2] - 2.5f) < 1e-6f);

    return 0;
}

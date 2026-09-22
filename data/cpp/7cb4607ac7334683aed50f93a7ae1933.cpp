Write a standalone C++ function that generates a deterministic 2D Perlin simplex noise value at a given `(x, y)` coordinate using a fixed permutation table. The function must accept two float coordinates and return a `float` in the range approximately `[-1, 1]`. It should implement the core simplex noise algorithm: skew the input space, determine the containing simplex triangle, compute contributions from the three corners using a gradient hash table, and apply radial falloff. The solution must handle negative coordinates correctly, use a hard-coded permutation table of 256 entries duplicated to 512, and avoid any external dependencies beyond standard headers. The implementation should be self-contained in a single free function named `simplexNoise2D` with parameters `(float x, float y)` and return the noise value. No global state or context objects are allowed; the permutation table must be stored as a static local or file-scope constant.
The core algorithm is 2D simplex noise as described by Ken Perlin. First, the input coordinates are skewed by adding `(x+y)*F2`, where `F2 = 0.5*(sqrt(3)-1)`, to map integer lattice points to a simplex grid. The skewed coordinates are then floored to determine the containing cell `(i, j)`. The origin of the cell is unskewed back to the original space using `G2 = (3-sqrt(3))/6`. Distances from the cell origin `(x0, y0)` are computed. Based on whether `x0 > y0`, the simplex is split into two triangles; the middle corner is offset accordingly. Each corner’s distance vector is used to compute a radial falloff `t = 0.5 - x^2 - y^2`; if `t` is negative, the contribution is zero, otherwise it is raised to the fourth power. The gradient is fetched from the permutation table using the integer corner indices (masked with `& 0xff`) and computed via a hash-based dot product with the distance vector. The contributions are summed and scaled by `40.0` to normalize to `[-1, 1]`. Edge cases include negative inputs, which are handled by a correct floor function (using `std::floor` or a custom fast floor that subtracts one for negative fractions), and coordinates on the boundary where `t` may be exactly zero, which naturally produce zero contribution. Time complexity is `O(1)` per call, and space complexity is `O(1)` aside from the static 512-entry permutation table.
#include <cmath>
#include <cstdint>

// Fixed permutation table used for gradient hashing (first 256 values repeated to 512)
static const unsigned char permTable[512] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

// Returns a gradient dot product based on hash (low 4 bits) and 2D distance vector
static float grad2D(int hash, float x, float y) {
    int h = hash & 7;           // Use low 3 bits for 8 gradient directions
    float u = h < 4 ? x : y;
    float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f*v : 2.0f*v);
}

// 2D simplex noise. Returns a float approximately in [-1, 1].
float simplexNoise2D(float x, float y) {
    const float F2 = 0.366025403f;  // 0.5 * (sqrt(3) - 1)
    const float G2 = 0.211324865f;  // (3 - sqrt(3)) / 6

    // Skew input space to determine simplex cell
    float s = (x + y) * F2;
    int i = static_cast<int>(std::floor(x + s));
    int j = static_cast<int>(std::floor(y + s));

    // Unskew cell origin back to original space
    float t = static_cast<float>(i + j) * G2;
    float x0 = x - (i - t);
    float y0 = y - (j - t);

    // Determine which triangle (simplex) we are in
    int i1, j1;
    if (x0 > y0) { i1 = 1; j1 = 0; }  // Lower triangle
    else         { i1 = 0; j1 = 1; }  // Upper triangle

    // Offsets for middle corner
    float x1 = x0 - i1 + G2;
    float y1 = y0 - j1 + G2;
    // Offsets for last corner
    float x2 = x0 - 1.0f + 2.0f * G2;
    float y2 = y0 - 1.0f + 2.0f * G2;

    // Wrap indices to valid permutation range
    int ii = i & 0xff;
    int jj = j & 0xff;

    float n0, n1, n2;

    // Contribution from first corner
    float t0 = 0.5f - x0*x0 - y0*y0;
    if (t0 < 0.0f) n0 = 0.0f;
    else {
        t0 *= t0;
        n0 = t0 * t0 * grad2D(permTable[ii + permTable[jj]], x0, y0);
    }

    // Contribution from second corner
    float t1 = 0.5f - x1*x1 - y1*y1;
    if (t1 < 0.0f) n1 = 0.0f;
    else {
        t1 *= t1;
        n1 = t1 * t1 * grad2D(permTable[ii + i1 + permTable[jj + j1]], x1, y1);
    }

    // Contribution from third corner
    float t2 = 0.5f - x2*x2 - y2*y2;
    if (t2 < 0.0f) n2 = 0.0f;
    else {
        t2 *= t2;
        n2 = t2 * t2 * grad2D(permTable[ii + 1 + permTable[jj + 1]], x2, y2);
    }

    // Sum and scale to approximately [-1, 1]
    return 40.0f * (n0 + n1 + n2);
}
#include <cassert>
#include <cmath>

// Declaration of the solution function
float simplexNoise2D(float x, float y);

int main() {
    // Known approximate values for specific coordinates (based on standard simplex noise)
    assert(std::fabs(simplexNoise2D(0.0f, 0.0f)) < 0.001f);  // Origin is near zero
    assert(std::fabs(simplexNoise2D(1.5f, 2.0f)) <= 1.0f);    // Output within range
    assert(std::fabs(simplexNoise2D(-1.5f, -2.0f)) <= 1.0f);  // Negative coordinates
    assert(std::fabs(simplexNoise2D(0.5f, 0.5f)) < 0.5f);     // Small values produce moderate output

    // Deterministic check: same input yields same output (call twice)
    float first = simplexNoise2D(3.7f, -4.2f);
    float second = simplexNoise2D(3.7f, -4.2f);
    assert(first == second);

    // Boundary condition: integer lattice points should return near-zero (noise at lattice corners is zero)
    float latticeValue = simplexNoise2D(1.0f, 1.0f);
    assert(std::fabs(latticeValue) < 0.001f);

    // Continuity: nearby points should produce similar values (not equal, but close)
    float val1 = simplexNoise2D(0.25f, 0.25f);
    float val2 = simplexNoise2D(0.25f + 0.01f, 0.25f);
    assert(std::fabs(val1 - val2) < 0.5f);

    // Value at exact simplex corner should be zero (e.g., x=y but not at lattice)
    float cornerVal = simplexNoise2D(0.0f, 0.0f + 1e-6f);
    assert(std::fabs(cornerVal) < 0.001f);

    return 0;
}

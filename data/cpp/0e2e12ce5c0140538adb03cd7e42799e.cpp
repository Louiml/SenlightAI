Write a C++ function that analyzes a grayscale image represented as a 2D vector of `uint8_t` values. For each pixel in the image, compute a 3x3 neighborhood (including the pixel itself), with border clamping (mirroring at edges). From each neighborhood, extract the center pixel as the prediction value and all 9 pixels as the joint data vector. Then group all pixels into equivalence classes based on identical joint data vectors (i.e., identical 3x3 neighborhoods). The function should return a 2D vector of `size_t` where each entry is the cluster ID (0-based index) assigned to the corresponding pixel, with cluster IDs assigned in order of first appearance when scanning rows top-to-bottom and columns left-to-right. The cluster IDs must represent exact equality of the entire 9-element neighborhood. Because the input is small and the problem is conceptually simple, focus on correctness, clarity, and proper use of `const` correctness. The function signature should be: `std::vector<std::vector<size_t>> clusterBy3x3Neighborhood(const std::vector<std::vector<uint8_t>>& image)`. Handle the edge case of a 1x1 image gracefully. Do not include any `main` function in the solution; only provide the free function. The function must be self-contained with all necessary headers included.

The task requires grouping pixels by their exact 3x3 neighborhood patterns. The main algorithm is straightforward: iterate over every pixel in row-major order. For each pixel `(r,c)`, extract the 3x3 window centered at that pixel. Use mirror clamping at borders: if an index is negative, take its absolute value; if it exceeds the dimension, reflect it as `2*(dim-1) - index` (the same formula used in the original snippet's `getPixel` method). Collect the nine values in a fixed order (e.g., row-major within the 3x3 window). To find the cluster ID for a neighborhood, maintain a map from the 9-element pattern (encoded as an array or `std::array<uint8_t,9>`) to an integer cluster ID. For each new pattern, assign the next available ID. Otherwise, reuse the existing ID. Finally, fill the output grid with these IDs. The time complexity is \(O(H \times W \times 9)\) because each pixel visits 9 neighbors, plus map overhead (amortized \(O(1)\) for lookup/insertion). Space complexity is \(O(H \times W)\) for the output plus \(O(K \times 9)\) where \(K\) is the number of unique neighborhoods. Edge cases: images smaller than 3x3 still work because clamping handles out-of-bounds indices by mirroring, and a 1x1 image yields one unique neighborhood (the pixel itself repeated 9 times in the clamped window). Also note that the prediction value (center pixel) is implicitly part of the 3x3 window, so it does not need separate handling; we simply compare whole neighborhoods.

#include <vector>
#include <cstdint>
#include <map>
#include <array>
#include <algorithm>

// Cluster pixels based on identical 3x3 neighborhoods with mirror border clamping.
// Returns a grid of cluster IDs (0-based) assigned in order of first occurrence (row-major scan).
std::vector<std::vector<size_t>> clusterBy3x3Neighborhood(const std::vector<std::vector<uint8_t>>& image) {
    const int rows = static_cast<int>(image.size());
    const int cols = static_cast<int>(image[0].size());

    auto clamped = [&](int r, int c) -> uint8_t {
        // Mirror at borders: if negative, take abs; if >= dim, reflect.
        if (r < 0) r = -r;
        if (c < 0) c = -c;
        if (r >= rows) r = (rows - 1) * 2 - r;
        if (c >= cols) c = (cols - 1) * 2 - c;
        return image[r][c];
    };

    // Map from 3x3 pattern to cluster ID.
    std::map<std::array<uint8_t, 9>, size_t> patternToId;
    std::vector<std::vector<size_t>> result(rows, std::vector<size_t>(cols, 0));
    size_t nextId = 0;

    // Scan in row-major order.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // Extract 3x3 window with clamping.
            std::array<uint8_t, 9> pattern;
            int idx = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    pattern[idx++] = clamped(r + dr, c + dc);
                }
            }
            // Assign or retrieve cluster ID.
            auto it = patternToId.find(pattern);
            if (it == patternToId.end()) {
                patternToId[pattern] = nextId;
                result[r][c] = nextId;
                ++nextId;
            } else {
                result[r][c] = it->second;
            }
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <cstdint>

// Forward declaration of the solution function.
std::vector<std::vector<size_t>> clusterBy3x3Neighborhood(const std::vector<std::vector<uint8_t>>& image);

int main() {
    // Test 1: 1x1 image → single cluster ID 0.
    {
        std::vector<std::vector<uint8_t>> img = {{42}};
        auto res = clusterBy3x3Neighborhood(img);
        assert(res.size() == 1 && res[0].size() == 1 && res[0][0] == 0);
    }

    // Test 2: Uniform 2x2 image (all same value) → one cluster.
    {
        std::vector<std::vector<uint8_t>> img = {{7, 7}, {7, 7}};
        auto res = clusterBy3x3Neighborhood(img);
        assert(res[0][0] == 0 && res[0][1] == 0 && res[1][0] == 0 && res[1][1] == 0);
    }

    // Test 3: 2x2 with distinct pixels → all 3x3 neighborhoods (with clamping) must differ.
    {
        std::vector<std::vector<uint8_t>> img = {{1, 2}, {3, 4}};
        auto res = clusterBy3x3Neighborhood(img);
        // All four should be distinct IDs.
        assert(res[0][0] != res[0][1]);
        assert(res[0][0] != res[1][0]);
        assert(res[0][0] != res[1][1]);
        assert(res[0][1] != res[1][0]);
        assert(res[0][1] != res[1][1]);
        assert(res[1][0] != res[1][1]);
        // Check unique IDs are 0,1,2,3 in some order.
        size_t present[4] = {0,0,0,0};
        present[res[0][0]] = 1; present[res[0][1]] = 1; present[res[1][0]] = 1; present[res[1][1]] = 1;
        assert(present[0] && present[1] && present[2] && present[3]);
    }

    // Test 4: 3x3 image with two identical 3x3 neighborhoods (e.g., all zeros except center in one spot).
    {
        std::vector<std::vector<uint8_t>> img(3, std::vector<uint8_t>(3, 0));
        img[1][1] = 5; // center pixel differs
        auto res = clusterBy3x3Neighborhood(img);
        // Center pixel's neighborhood is unique, all others (edges) mirror to same pattern.
        // Edge pixels (none are at exact center) share the same clamped neighborhood.
        size_t centerId = res[1][1];
        for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) {
            if (r == 1 && c == 1) { assert(res[r][c] == centerId); }
            else { assert(res[r][c] != centerId); }
        }
        // All non-center pixels share the same ID.
        size_t edgeId = res[0][0];
        assert(res[0][1] == edgeId && res[0][2] == edgeId && res[1][0] == edgeId);
        assert(res[1][2] == edgeId && res[2][0] == edgeId && res[2][1] == edgeId && res[2][2] == edgeId);
    }

    // Test 5: Known simple 3x3 image where all neighborhoods are identical (all pixels same value).
    {
        std::vector<std::vector<uint8_t>> img(3, std::vector<uint8_t>(3, 9));
        auto res = clusterBy3x3Neighborhood(img);
        for (auto& row : res) for (auto id : row) assert(id == 0);
    }

    return 0;
}

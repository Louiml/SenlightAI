// Write a C++ function named `analyzeContours` that takes a grayscale image represented as a 2D vector of unsigned char values (where 0 represents black and 255 represents white) and a threshold value. The function should perform binary thresholding (values >= threshold become 255, others become 0), then find connected components of white pixels (8-connectivity). For each connected component, compute its bounding box (min row, min col, max row, max col) and its area (number of pixels). The function must return a vector of `ContourInfo` structs (each containing `Rect`-like integer fields: `minRow`, `minCol`, `maxRow`, `maxCol`, and `area`), sorted by area in descending order. If the input is empty or the threshold is outside the valid range [0,255], return an empty vector. The function must be `const`-correct and not modify the input image.

The solution requires a two-pass connected-component labeling algorithm with 8-connectivity. First, perform thresholding to create a binary image where pixel values >= threshold become white (1 for processing) and others become black (0). Then implement connected components: iterate through every pixel, when a white pixel is encountered that hasn't been visited, start a flood-fill (BFS or DFS) to collect all connected white pixels, marking them visited. During the flood-fill, track the minimum and maximum row and column indices and count the pixels to compute area. After collecting a component, store its bounding box and area. After all components are found, sort the vector by area in descending order (use `std::sort` with a lambda comparing areas). Edge cases: empty input, threshold outside [0,255], fully black or fully white images (should produce zero or one component), and single-pixel components. Time complexity is O(R*C) for thresholding + O(R*C) for labeling, total O(R*C). Space complexity is O(R*C) for the visited array and the result vector, though the result vector typically holds far fewer entries than pixels. The flood-fill can be done iteratively using a stack to avoid recursion depth issues on large images.

#include <vector>
#include <algorithm>
#include <stack>
#include <cstdint>

struct ContourInfo {
    int minRow;
    int minCol;
    int maxRow;
    int maxCol;
    int area;
};

// Compute contours as connected components from a grayscale image using a threshold.
std::vector<ContourInfo> analyzeContours(const std::vector<std::vector<unsigned char>>& image, int threshold) {
    if (image.empty() || image[0].empty() || threshold < 0 || threshold > 255) {
        return {};
    }

    int rows = static_cast<int>(image.size());
    int cols = static_cast<int>(image[0].size());

    // Binary threshold: 1 for white, 0 for black
    std::vector<std::vector<char>> binary(rows, std::vector<char>(cols, 0));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            binary[r][c] = (image[r][c] >= threshold) ? 1 : 0;
        }
    }

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<ContourInfo> contours;

    // Direction vectors for 8-connectivity
    const int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (binary[r][c] == 1 && !visited[r][c]) {
                // Start flood-fill for this component
                std::stack<std::pair<int, int>> stack;
                stack.push({r, c});
                visited[r][c] = true;

                int minR = r, maxR = r, minC = c, maxC = c;
                int area = 0;

                while (!stack.empty()) {
                    auto [cr, cc] = stack.top();
                    stack.pop();
                    area++;

                    minR = std::min(minR, cr);
                    maxR = std::max(maxR, cr);
                    minC = std::min(minC, cc);
                    maxC = std::max(maxC, cc);

                    for (int d = 0; d < 8; ++d) {
                        int nr = cr + dr[d];
                        int nc = cc + dc[d];
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                            binary[nr][nc] == 1 && !visited[nr][nc]) {
                            visited[nr][nc] = true;
                            stack.push({nr, nc});
                        }
                    }
                }

                contours.push_back({minR, minC, maxR, maxC, area});
            }
        }
    }

    // Sort by area descending
    std::sort(contours.begin(), contours.end(),
              [](const ContourInfo& a, const ContourInfo& b) {
                  return a.area > b.area;
              });

    return contours;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty image
    {
        std::vector<std::vector<unsigned char>> img;
        auto result = analyzeContours(img, 100);
        assert(result.empty());
    }

    // Test 2: Invalid threshold
    {
        std::vector<std::vector<unsigned char>> img(2, std::vector<unsigned char>(2, 0));
        assert(analyzeContours(img, 256).empty());
        assert(analyzeContours(img, -1).empty());
    }

    // Test 3: Single white pixel
    {
        std::vector<std::vector<unsigned char>> img(3, std::vector<unsigned char>(3, 0));
        img[1][1] = 255;
        auto result = analyzeContours(img, 128);
        assert(result.size() == 1);
        assert(result[0].area == 1);
        assert(result[0].minRow == 1 && result[0].maxRow == 1);
        assert(result[0].minCol == 1 && result[0].maxCol == 1);
    }

    // Test 4: Two separate components, sorted by area descending
    {
        std::vector<std::vector<unsigned char>> img(5, std::vector<unsigned char>(5, 0));
        // Large component at top-left: 2x2 square
        img[0][0] = 255; img[0][1] = 255;
        img[1][0] = 255; img[1][1] = 255;
        // Small component at bottom-right: 1x1
        img[4][4] = 200; // should be included because >= threshold 100
        auto result = analyzeContours(img, 100);
        assert(result.size() == 2);
        assert(result[0].area == 4);
        assert(result[1].area == 1);
        assert(result[0].minRow == 0 && result[0].maxRow == 1);
        assert(result[0].minCol == 0 && result[0].maxCol == 1);
        assert(result[1].minRow == 4 && result[1].maxRow == 4);
        assert(result[1].minCol == 4 && result[1].maxCol == 4);
    }

    // Test 5: Diagonal connectivity (8-connectivity)
    {
        std::vector<std::vector<unsigned char>> img(2, std::vector<unsigned char>(2, 0));
        img[0][0] = 255;
        img[1][1] = 255;
        auto result = analyzeContours(img, 128);
        assert(result.size() == 1);
        assert(result[0].area == 2);
        assert(result[0].minRow == 0 && result[0].maxRow == 1);
        assert(result[0].minCol == 0 && result[0].maxCol == 1);
    }

    // Test 6: Fully white image – one component covering all
    {
        std::vector<std::vector<unsigned char>> img(3, std::vector<unsigned char>(4, 255));
        auto result = analyzeContours(img, 0);
        assert(result.size() == 1);
        assert(result[0].area == 12);
        assert(result[0].minRow == 0 && result[0].maxRow == 2);
        assert(result[0].minCol == 0 && result[0].maxCol == 3);
    }

    // Test 7: Fully black image – no components
    {
        std::vector<std::vector<unsigned char>> img(3, std::vector<unsigned char>(3, 0));
        auto result = analyzeContours(img, 1);
        assert(result.empty());
    }

    return 0;
}

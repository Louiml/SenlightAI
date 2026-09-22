Write a C++ function that takes a binary image represented as a vector of vectors of integers (0 for black, 1 for white) and performs connected-component labeling using 4-connectivity (up, down, left, right). The function should return a label matrix of the same dimensions where each white pixel is assigned a positive integer label indicating its connected component, and black pixels are assigned 0. Labels must be assigned starting from 1, increasing sequentially for each new component discovered during a row-major scan (top to bottom, left to right). The function should handle empty images and images with unequal row lengths gracefully by returning an empty label matrix if the input is empty or non-rectangular.
// The solution uses breadth-first search (BFS) for each unlabeled white pixel encountered during a row-major scan. For each white pixel with label 0, we increment the current label counter, assign it to that pixel, push it into a queue, and process its 4-neighbors. For each neighbor that is white and unlabeled, we assign the same label and enqueue it. This continues until the queue is empty, completing one connected component. The algorithm ensures each white pixel is visited exactly once. Edge cases include empty input (return empty matrix), non-rectangular input (return empty matrix), and images with no white pixels (return all zeros). Time complexity is O(R*C) for an R×C image, as each pixel is visited once during labeling and once during queue processing. Space complexity is O(R*C) for the label matrix and potentially O(R*C) for the queue in the worst case (e.g., a fully white image).
#include <vector>
#include <queue>
#include <utility>

// Label connected components of white pixels (value 1) using 4-connectivity.
// Returns a label matrix where each white pixel gets a positive component label,
// and black pixels (value 0) get 0. Labels are assigned in row-major order.
// Returns an empty matrix if input is empty or non-rectangular.
std::vector<std::vector<int>> labelConnectedComponents4(const std::vector<std::vector<int>>& binaryImage) {
    if (binaryImage.empty()) return {};
    const size_t rows = binaryImage.size();
    const size_t cols = binaryImage[0].size();
    // Check rectangularity
    for (const auto& row : binaryImage) {
        if (row.size() != cols) return {};
    }

    std::vector<std::vector<int>> labels(rows, std::vector<int>(cols, 0));
    int currentLabel = 0;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (binaryImage[i][j] == 1 && labels[i][j] == 0) {
                ++currentLabel;
                std::queue<std::pair<size_t, size_t>> q;
                labels[i][j] = currentLabel;
                q.push({i, j});

                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();

                    // Up
                    if (x > 0 && binaryImage[x-1][y] == 1 && labels[x-1][y] == 0) {
                        labels[x-1][y] = currentLabel;
                        q.push({x-1, y});
                    }
                    // Down
                    if (x + 1 < rows && binaryImage[x+1][y] == 1 && labels[x+1][y] == 0) {
                        labels[x+1][y] = currentLabel;
                        q.push({x+1, y});
                    }
                    // Left
                    if (y > 0 && binaryImage[x][y-1] == 1 && labels[x][y-1] == 0) {
                        labels[x][y-1] = currentLabel;
                        q.push({x, y-1});
                    }
                    // Right
                    if (y + 1 < cols && binaryImage[x][y+1] == 1 && labels[x][y+1] == 0) {
                        labels[x][y+1] = currentLabel;
                        q.push({x, y+1});
                    }
                }
            }
        }
    }
    return labels;
}
#include <cassert>
#include <vector>

int main() {
    // Empty input
    std::vector<std::vector<int>> empty;
    assert(labelConnectedComponents4(empty).empty());

    // Non-rectangular input
    std::vector<std::vector<int>> nonRect = {{1,0}, {1}};
    assert(labelConnectedComponents4(nonRect).empty());

    // All black
    std::vector<std::vector<int>> allBlack = {{0,0},{0,0}};
    auto labels = labelConnectedComponents4(allBlack);
    assert(labels.size() == 2 && labels[0].size() == 2);
    assert(labels == std::vector<std::vector<int>>(2, std::vector<int>(2, 0)));

    // Single white pixel
    std::vector<std::vector<int>> singleWhite = {{0,0},{0,1}};
    labels = labelConnectedComponents4(singleWhite);
    assert(labels[1][1] == 1 && labels[0][0] == 0 && labels[0][1] == 0 && labels[1][0] == 0);

    // Two separate components
    std::vector<std::vector<int>> twoComp = {{1,0,1},{0,0,0},{1,0,1}};
    labels = labelConnectedComponents4(twoComp);
    // Top-left, bottom-left, and bottom-right are separated, but bottom-left and bottom-right are not 4-connected
    // Top-left (0,0), bottom-left (2,0), bottom-right (2,2) are separate: 3 components
    int maxLabel = 0;
    for (const auto& row : labels) for (int v : row) if (v > maxLabel) maxLabel = v;
    assert(maxLabel == 3);
    assert(labels[0][0] == 1 && labels[2][0] == 2 && labels[2][2] == 3);
    // 4-connectivity: center (1,1) is not connected to any white

    // Component connected diagonally should be separate under 4-connectivity
    std::vector<std::vector<int>> diag = {{1,0},{0,1}};
    labels = labelConnectedComponents4(diag);
    assert(labels[0][0] == 1 && labels[1][1] == 2);

    // Fully connected block
    std::vector<std::vector<int>> block = {{1,1,0},{1,1,0},{0,0,0}};
    labels = labelConnectedComponents4(block);
    int maxL = 0;
    for (const auto& row : labels) for (int v : row) if (v > maxL) maxL = v;
    assert(maxL == 1);
    assert(labels[0][0] == 1 && labels[0][1] == 1 && labels[1][0] == 1 && labels[1][1] == 1);

    // L-shaped component
    std::vector<std::vector<int>> lShape = {{1,1,0},{0,1,0},{0,1,1}};
    labels = labelConnectedComponents4(lShape);
    maxL = 0;
    for (const auto& row : labels) for (int v : row) if (v > maxL) maxL = v;
    assert(maxL == 1);
    assert(labels[0][0] == 1 && labels[0][1] == 1 && labels[1][1] == 1 && labels[2][1] == 1 && labels[2][2] == 1);

    return 0;
}

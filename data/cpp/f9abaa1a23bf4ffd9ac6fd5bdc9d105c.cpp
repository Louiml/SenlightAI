/*
Write a C++ function that, given a vector of positive integers representing the side lengths of square tiles, determines and returns the largest possible side length of a square that can be formed by arranging a subset of the tiles. The square must be built such that the number of tiles used along each side is exactly equal to the side length of the square. You may use any subset of the provided tiles, and you must use whole tiles (no cutting). For example, with tiles of lengths [3, 3, 3, 3, 2], you can form a 3×3 square using four tiles of length 3, so the answer is 3. With [1, 1, 2, 2], the best you can do is a 1×1 square (using one tile of length 1) because you cannot arrange two tiles of length 2 to make a 2×2 square (you would need 4 tiles of length 2). The function should return 0 if no square can be formed (e.g., all tiles are too short to pair with enough others). The input vector may contain duplicates and is not guaranteed to be sorted.
*/
#include <vector>
#include <algorithm>

// Returns the largest side length L such that we can select at least L tiles
// each of length at least L. Tiles are square and cannot be cut.
// If no such L exists (including empty input), returns 0.
int largestSquareSide(std::vector<int> tiles) {
    // Sort in descending order so the largest tiles come first.
    std::sort(tiles.begin(), tiles.end(), std::greater<int>());
    
    int best = 0;
    int count = 0;  // number of tiles considered so far (candidate side length)
    
    for (int tile : tiles) {
        // If we have taken 'count' tiles and the next tile is at least
        // (count+1), then we can extend the square side to count+1.
        if (count + 1 <= tile) {
            ++count;
            best = count;  // count is now a valid side length
        } else {
            // Since tiles are sorted descending, all remaining are even smaller.
            // No larger side is possible, so we can stop early.
            break;
        }
    }
    
    return best;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function declaration here (or paste the function above).
// This test file uses the function correctly.

int largestSquareSide(std::vector<int> tiles); // assume defined elsewhere

int main() {
    // Basic cases
    assert(largestSquareSide({}) == 0);
    assert(largestSquareSide({5}) == 1);          // one tile of length 5 can make a 1x1 square
    assert(largestSquareSide({1}) == 1);
    assert(largestSquareSide({1, 1}) == 1);       // only 1x1 possible
    assert(largestSquareSide({2, 2}) == 2);       // two tiles of length 2 -> 2x2 square
    assert(largestSquareSide({3, 3, 3}) == 3);    // three tiles of length 3 -> 3x3
    assert(largestSquareSide({3, 3, 3, 3}) == 3); // four tiles allow 3x3 (need 3 tiles)
    assert(largestSquareSide({2, 2, 2, 2, 2}) == 2); // at most 2x2
    assert(largestSquareSide({10, 1, 1, 1, 1}) == 1); // only one large tile
    assert(largestSquareSide({5, 5, 4, 3, 2, 1}) == 3); // three tiles >=3? actually 5,5,4 -> side 3 works? Need 3 tiles each >=3: yes 5,5,4 -> side 3. But can we do 4? Need 4 tiles >=4: only 5,5,4 (3 tiles) no. So answer 3.
    
    // Additional edge: duplicates and unsorted input
    assert(largestSquareSide({1, 2, 3, 4, 5, 6, 7, 8}) == 4); // tiles 8,7,6,5 -> side 4 works, side 5? need 5 tiles >=5: have 8,7,6,5,4 -> only 4 tiles >=5? Actually 8,7,6,5 are 4 tiles, so no. So 4.
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The problem reduces to finding the maximum integer `s` such that there exist at least `s` tiles each with length ≥ `s` (since each side of the square must be built from a single tile of that side length, and you need one tile per side, so 4 tiles if s>1? Wait—carefully: The original code snippet uses a different interpretation. Let's re-interpret: The original snippet sorts tiles descending, then for each tile at index i (0-based) checks if (i+1) ≤ tile[i]. This means: after sorting descending, if the number of tiles considered so far (i+1) is ≤ the current tile's length, then you can form a square of side length (i+1) because you have at least (i+1) tiles each of length ≥ (i+1). The answer is the maximum such (i+1). But note: the original snippet also pushes into a vector and takes the minimum of that and `no[0]`—which is buggy. Actually the correct logic is: sort descending, then iterate through, keeping track of the current count of tiles processed. For each tile, if `count <= tile_value`, then you can achieve a square of side `count`. The maximum valid `count` is the answer. Because if you have a square of side s, you need s tiles each of length ≥ s. Sorting descending and checking the prefix condition (the k-th largest tile ≥ k) is the standard condition for the maximum k such that there are at least k elements each ≥ k. So the solution: sort descending, then scan from index 0 upward, increment a counter, and if `counter <= tiles[counter-1]` (since index is 0-based, counter is the number of tiles taken), then update answer = counter; else break (since further tiles are even smaller, condition cannot hold). Edge cases: empty vector returns 0. All tile lengths positive. Duplicates allowed. Time complexity O(n log n) due to sorting, space O(1) aside from input vector (or O(n) if we copy). We can also do counting sort if lengths are bounded, but not needed.

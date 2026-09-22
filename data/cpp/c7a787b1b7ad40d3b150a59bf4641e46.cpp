Write a C++ function `int countOctopusFlashes(const std::vector<std::vector<int>>& grid, int targetStep, int& syncStep)` that simulates a 10x10 grid of bioluminescent octopuses. Each cell contains an energy level from 0 to 9. Each time step, all cells increase by 1. Any cell reaching exactly 10 flashes once during that step, incrementing a global flash counter, and then all 8 adjacent neighbors (including diagonals) increase by 1; if any neighbor reaches exactly 10 during this cascade, it also flashes and propagates. A cell can flash at most once per step. After all flash processing is complete, every cell that flashed (energy ≥ 10) is reset to 0. The function should return the total number of flashes after `targetStep` steps (where step 1 is the first simulation step). Additionally, it should set `syncStep` to the first step number at which all cells are simultaneously 0 (synchronized), or to `-1` if synchronization has not occurred by step `targetStep`. The input grid is guaranteed to be exactly 10x10 with values 0-9. Assume the target step is at least 1.
#include <cassert>
#include <vector>

int main() {
    // Test case 1: simple all zeros grid. Step 1: all become 1, no flashes. Step 2: all become 2, ... 
    // After 10 steps, all become 10 and flash together, then reset to 0. So sync at step 10.
    std::vector<std::vector<int>> grid1(10, std::vector<int>(10, 0));
    int sync1;
    int flashes1 = countOctopusFlashes(grid1, 10, sync1);
    assert(flashes1 == 100); // 100 flashes at step 10
    assert(sync1 == -1); // sync happens exactly at step 10, but we only check after each step, and step 10 is included, so sync should be 10
    // Actually let's recompute: The function checks after each step, so syncStep=10.
    // Correct it: The above assertion is wrong. Let's adjust.

    // We'll rewrite cleaner tests below.
}

// We need to redo the test properly.
#include <cassert>
#include <vector>

int main() {
    // Case 1: All zeros, target=10. At step 10, all flash and reset to 0.
    // The function should return total flashes = 100, syncStep=10.
    std::vector<std::vector<int>> grid1(10, std::vector<int>(10, 0));
    int sync1;
    int flashes1 = countOctopusFlashes(grid1, 10, sync1);
    assert(flashes1 == 100);
    assert(sync1 == 10);

    // Case 2: All zeros, target=9. Not yet synchronized.
    int sync2;
    int flashes2 = countOctopusFlashes(grid1, 9, sync2);
    assert(flashes2 == 0);
    assert(sync2 == -1);

    // Case 3: Grid with single 1 in center, others 0. 
    // Step 1: center becomes 2, others 1. No flashes.
    // Step 2: center becomes 3, others 2. ...
    // Step 9: center becomes 10, others 9. Flash center, neighbors become 10, cascade. Some flashes.
    // Let's just test a small known case: a 10x10 grid with all values 1. 
    // Step 1: all become 2, no flashes. ... Step 9: all become 10, all flash (100 flashes), reset to 0, sync.
    std::vector<std::vector<int>> grid3(10, std::vector<int>(10, 1));
    int sync3;
    int flashes3 = countOctopusFlashes(grid3, 9, sync3);
    assert(flashes3 == 100);
    assert(sync3 == 9);

    // Case 4: A grid with one cell = 9, rest = 0. Step 1: that cell becomes 10 and flashes,
    // affecting neighbors which become 1, others 1. Total flashes = 1. 
    std::vector<std::vector<int>> grid4(10, std::vector<int>(10, 0));
    grid4[5][5] = 9;
    int sync4;
    int flashes4 = countOctopusFlashes(grid4, 1, sync4);
    assert(flashes4 == 1);
    assert(sync4 == -1); // not sync after step 1

    // Case 5: Ensure multiple steps accumulate correctly. Use previous grid4, but target=2.
    // After step 1: flashes=1. Step 2: all cells increase by 1. The center cell is now 0 (reset),
    // neighbors are 2 (from 1), others 1. So no new flashes. Total=1. 
    std::vector<std::vector<int>> grid5 = grid4;
    int sync5;
    int flashes5 = countOctopusFlashes(grid5, 2, sync5);
    assert(flashes5 == 1);
    assert(sync5 == -1);

    // Case 6: Check that a cell flashing multiple times from neighbors isn't double-counted.
    // Make a 3x3 sub-grid pattern but full 10x10 with zeros. Set center to 9 and one neighbor to 9.
    // Step 1: both become 10, they flash, affecting each other. But they are already flashed, so no double count.
    std::vector<std::vector<int>> grid6(10, std::vector<int>(10, 0));
    grid6[5][5] = 9;
    grid6[5][6] = 9;
    int sync6;
    int flashes6 = countOctopusFlashes(grid6, 1, sync6);
    // Both flash once, plus neighbors might reach 10? Center neighbor (5,7) gets +2 from both? Actually each flash adds 1, so (5,7) gets +1 from (5,6) and +1 from (5,5)? No, (5,5) neighbors don't include (5,7) because diagonal? (5,7) is 2 away. So only (5,6) affects (5,7). So only 2 flashes.
    assert(flashes6 == 2);
    assert(sync6 == -1);

    return 0;
}
#include <vector>
#include <queue>
#include <utility>

// Simulates octopus flashes on a 10x10 grid.
// Returns total flashes after targetStep steps, and sets syncStep to first step
// where all cells are 0 (or -1 if not synchronized by targetStep).
int countOctopusFlashes(const std::vector<std::vector<int>>& grid, int targetStep, int& syncStep) {
    const int N = 10;
    const std::vector<int> dx = {0, 0, 1, -1, 1, -1, 1, -1};
    const std::vector<int> dy = {-1, 1, 0, 0, 1, -1, -1, 1};

    // Copy grid to modify
    std::vector<std::vector<int>> g = grid;

    // Check if all cells are zero
    auto isSync = [&]() {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (g[i][j] != 0) return false;
        return true;
    };

    int totalFlashes = 0;
    syncStep = -1;

    for (int step = 1; step <= targetStep; ++step) {
        // Increase all cells by 1
        std::queue<std::pair<int, int>> q;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                g[i][j]++;
                if (g[i][j] == 10) {
                    q.push({i, j});
                }
            }

        // Process flashes
        int stepFlashes = q.size();
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            for (int k = 0; k < 8; ++k) {
                int nx = x + dx[k];
                int ny = y + dy[k];
                if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                    g[nx][ny]++;
                    if (g[nx][ny] == 10) {
                        stepFlashes++;
                        q.push({nx, ny});
                    }
                }
            }
        }

        // Reset flashed cells to 0
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (g[i][j] >= 10) g[i][j] = 0;

        totalFlashes += stepFlashes;

        if (isSync() && syncStep == -1) {
            syncStep = step;
        }
    }

    if (syncStep == -1) {
        syncStep = -1; // not synchronized by targetStep
    }
    return totalFlashes;
}
// The solution simulates the grid step by step using a BFS-like flood fill for each step’s flash propagation. For each step, first increment all cells by 1. Then, maintain a queue of cells that have exactly energy 10 after the global increment. Pop cells, and for each of the 8 neighbors (if in bounds), increment the neighbor by 1; if that neighbor’s energy becomes exactly 10 (meaning it hasn’t flashed yet this step), push it to the queue and count the flash. This ensures each cell is processed at most once per step because once a cell reaches 10, it is pushed and never pushed again in the same step (since further increments would make it ≥11, which we ignore). After the queue is empty, reset all cells with energy ≥ 10 to 0. Track the total flash count across all steps, and after each step check if all cells are 0 (synchronized). If step equals `targetStep`, record the total flashes. The complexity is O(targetStep * N^2) where N=10, since each step processes each cell a constant number of times (at most once for flashing and once for being incremented). Space is O(N^2) for the grid.
//
// Important edge cases: cells that exceed 10 from multiple neighbor flashes should not be re-queued; we only push when energy becomes exactly 10. Also, synchronization can occur before or exactly at `targetStep`; we must set `syncStep` accordingly. If not synchronized by `targetStep`, return `-1`. Also note that the initial grid may not be all zeros; the first step starts by incrementing everything.

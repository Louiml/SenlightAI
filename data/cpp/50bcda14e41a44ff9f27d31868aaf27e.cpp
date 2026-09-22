Complete the implementation of the `canUnlock` function that determines whether a given binary `key` matrix can perfectly open a binary `lock` matrix. A lock is considered opened when all the `0` cells (holes) in the lock are exactly covered by the `1` cells (teeth) of the key after translating (moving left, right, up, down) and/or rotating the key by 90°, 180°, or 270°. Translations and rotations can be applied in any sequence and any number of times. The key may extend beyond the boundaries of the lock during the process, but any key tooth that lands inside the lock's bounds must correspond to a lock hole (i.e., a `0`); it cannot overlap a lock protrusion (`1`). The key must have at least as many teeth as the lock has holes, and every lock hole must be covered exactly once. The function should return `true` if such an alignment exists and `false` otherwise. Both matrices are square and have size between 1 and 20.

The problem is a search over all possible transformations of the key shape (rotations and translations). The key is represented as a set of coordinates of its `1` cells, and the lock holes as a set of coordinates of its `0` cells. The core idea is to treat the problem as finding any translation and rotation of the key such that, when positioned relative to the lock, every key tooth that lies inside the lock bounds matches a lock hole, and every lock hole is covered by at least one key tooth (in fact, exactly one, since the key teeth are unique positions and we require all holes to be covered and no extra key tooth inside the lock).  
We perform a breadth-first search (BFS) over all reachable states, where each state is the current set of key coordinates after some sequence of rotations and unit translations. The starting state is the key's original shape. From any state, we generate four rotations (90°, 180°, 270°) and four translations (up, down, left, right by one cell). To avoid revisiting the same shape (set of coordinates), we use a set of coordinate sets (or equivalently, a `std::set<std::vector<coordinate>>` with a proper comparator). For each state, we check if it satisfies the lock condition by iterating over its teeth: if a tooth lies inside the lock's bounds, it must be a hole (present in the lock hole set); if any tooth inside the lock is not a hole, the state is invalid. Also, the number of teeth that lie inside the lock must equal the number of lock holes. If both conditions hold, we return `true`. Important edge cases include: lock with no holes (then any key that has no teeth inside the lock works, but also an empty key shape might be valid, though typically a key has at least one tooth); key larger than lock but can be shifted so that all its teeth are outside the lock, which would be invalid because the lock has holes that need covering; and rotations may produce duplicate shapes due to symmetry, which are pruned by the visited set.  
The BFS terminates when the queue is empty, after exploring all reachable shapes. The state space is bounded: each coordinate is in a range of roughly `[-n, n]` for a lock of size `n`, and the number of teeth is at most `n^2`, so the number of distinct shapes is exponential in theory but practically limited by the small matrix size (≤20). The time complexity is O(S * T * k) where S is the number of distinct shapes visited, T is the number of teeth in each shape (≤400), and k is a constant for rotation/translation generation. Space complexity is O(S * T) for the visited set and queue.

#include <vector>
#include <queue>
#include <set>
#include <cstddef>

struct Cell {
    int x;
    int y;
    Cell(int x, int y) : x(x), y(y) {}
    bool operator<(const Cell& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
};

// Generates all four rotations (90, 180, 270 degrees) of the given shape.
// The rotation is performed around the origin (0,0) and then normalized
// by shifting so that the minimum coordinates become (0,0) to ensure
// that the shape's set representation is consistent regardless of where it is translated.
std::vector<std::vector<Cell>> rotateShape(const std::vector<Cell>& shape) {
    std::vector<std::vector<Cell>> rotations;
    std::vector<Cell> current = shape;
    
    for (int rot = 0; rot < 3; ++rot) {
        std::vector<Cell> next;
        for (const Cell& c : current) {
            next.push_back(Cell(c.y, -c.x));
        }
        // Normalize: shift so minimum x and y become 0.
        int minX = next[0].x, minY = next[0].y;
        for (const Cell& c : next) {
            if (c.x < minX) minX = c.x;
            if (c.y < minY) minY = c.y;
        }
        std::vector<Cell> normalized;
        for (const Cell& c : next) {
            normalized.push_back(Cell(c.x - minX, c.y - minY));
        }
        // Keep normalized shape as a rotation state.
        rotations.push_back(normalized);
        current = normalized;
    }
    return rotations;
}

// Returns true if the key can open the lock.
bool canUnlock(const std::vector<std::vector<int>>& key, const std::vector<std::vector<int>>& lock) {
    int sizeKey = static_cast<int>(key.size());
    int sizeLock = static_cast<int>(lock.size());

    // Extract key teeth coordinates.
    std::vector<Cell> keyShape;
    for (int i = 0; i < sizeKey; ++i) {
        for (int j = 0; j < sizeKey; ++j) {
            if (key[i][j] == 1) {
                keyShape.push_back(Cell(i, j));
            }
        }
    }

    // Extract lock holes coordinates.
    std::set<Cell> lockHoles;
    for (int i = 0; i < sizeLock; ++i) {
        for (int j = 0; j < sizeLock; ++j) {
            if (lock[i][j] == 0) {
                lockHoles.insert(Cell(i, j));
            }
        }
    }

    // If no holes in lock, it's already open.
    if (lockHoles.empty()) return true;

    // BFS over all reachable shapes (rotations + translations).
    // We store shapes as normalized coordinate lists (min x and y shifted to 0).
    // But translations are also normalized to avoid infinite loop of shifting.
    // However, to allow alignment with the lock, we need absolute positions.
    // So we store the shape as a set of absolute coordinates relative to a global origin.
    // We'll use a visited set of sorted vectors of Cell (absolute coordinates).
    std::set<std::vector<Cell>> visited;
    std::queue<std::vector<Cell>> q;

    // Initial shape: absolute coordinates from key (top-left at (0,0)).
    std::vector<Cell> initial = keyShape;
    visited.insert(initial);
    q.push(initial);

    while (!q.empty()) {
        std::vector<Cell> current = q.front();
        q.pop();

        // Count how many teeth are inside lock bounds and whether they match holes.
        int insideCount = 0;
        bool validPlacement = true;
        for (const Cell& c : current) {
            if (c.x >= 0 && c.x < sizeLock && c.y >= 0 && c.y < sizeLock) {
                insideCount++;
                if (lockHoles.find(c) == lockHoles.end()) {
                    validPlacement = false;
                    break;
                }
            }
        }
        if (validPlacement && insideCount == static_cast<int>(lockHoles.size())) {
            return true;
        }

        // Generate all rotations of the current shape.
        // We need to rotate around a pivot. Since our coordinate system is absolute,
        // we rotate each point around (0,0) and then normalize the shape by translating
        // so that its minimum coordinates are at (0,0). This is acceptable because
        // rotation around origin followed by translation is equivalent to rotating around
        // any pivot. But we must ensure that we explore all possible translations separately.
        // To avoid infinite translations, we only generate unit translations in BFS.
        // For rotations, we normalize after rotating, then push as new state.
        std::vector<std::vector<Cell>> rotations = rotateShape(current);
        for (auto& rotated : rotations) {
            // rotated is already normalized to have min at (0,0). But we need to
            // shift it to the same "position" as current? Actually rotating around
            // a pivot changes the shape's position. To explore all placements,
            // we treat the normalized shape as a new state and allow translations from there.
            // The BFS will also try translations from this state. So we add it directly.
            if (visited.find(rotated) == visited.end()) {
                visited.insert(rotated);
                q.push(rotated);
            }
        }

        // Translations: move left, right, up, down by one unit.
        // Left (y-1)
        std::vector<Cell> left;
        for (const Cell& c : current) left.push_back(Cell(c.x, c.y - 1));
        if (visited.find(left) == visited.end()) {
            visited.insert(left);
            q.push(left);
        }
        // Right (y+1)
        std::vector<Cell> right;
        for (const Cell& c : current) right.push_back(Cell(c.x, c.y + 1));
        if (visited.find(right) == visited.end()) {
            visited.insert(right);
            q.push(right);
        }
        // Up (x-1)
        std::vector<Cell> up;
        for (const Cell& c : current) up.push_back(Cell(c.x - 1, c.y));
        if (visited.find(up) == visited.end()) {
            visited.insert(up);
            q.push(up);
        }
        // Down (x+1)
        std::vector<Cell> down;
        for (const Cell& c : current) down.push_back(Cell(c.x + 1, c.y));
        if (visited.find(down) == visited.end()) {
            visited.insert(down);
            q.push(down);
        }
    }
    return false;
}

#include <cassert>
#include <vector>

int main() {
    // Example from the original snippet.
    std::vector<std::vector<int>> key1 = {{0,0,0},{1,0,0},{0,1,1}};
    std::vector<std::vector<int>> lock1 = {{1,1,1},{1,1,0},{1,0,1}};
    assert(canUnlock(key1, lock1) == true);

    // Simple case: key exactly matches lock hole pattern with no protrusions inside.
    std::vector<std::vector<int>> key2 = {{1,0},{0,1}};
    std::vector<std::vector<int>> lock2 = {{0,1},{1,0}};
    // Key rotated 90° becomes {{0,1},{1,0}} which fits directly. So true.
    assert(canUnlock(key2, lock2) == true);

    // Key that cannot fit because extra tooth lands on protrusion.
    std::vector<std::vector<int>> key3 = {{1,1},{0,0}};
    std::vector<std::vector<int>> lock3 = {{0,0},{1,1}};
    // The key has two adjacent teeth in a row. The lock holes are diagonal. No rotation/translation can cover both holes without overlapping a protrusion.
    assert(canUnlock(key3, lock3) == false);

    // Lock with no holes is always open.
    std::vector<std::vector<int>> key4 = {{1}};
    std::vector<std::vector<int>> lock4 = {{1}};
    assert(canUnlock(key4, lock4) == true);

    // Key has more teeth than holes but can be placed so extra teeth are outside lock.
    std::vector<std::vector<int>> key5 = {{1,1},{1,1}};
    std::vector<std::vector<int>> lock5 = {{0,1},{1,1}};
    // Key has 4 teeth, lock has 1 hole. Place key so that only one tooth covers the hole and the rest are outside? But the key is 2x2, to cover the single hole at (0,0), you would need to place one tooth there, but other teeth will be at (0,1),(1,0),(1,1) which are inside the lock and all are protrusions (1), so invalid. Could place the key shifted so that the hole is covered by one tooth and other teeth are outside? The key is 2x2, to have a tooth at (0,0), the other teeth are forced to be at (0,1),(1,0),(1,1) which are inside lock if the key is not shifted out. But we can shift the key so that only one tooth is inside the lock? The key shape is a 2x2 block, any placement of a 2x2 block that covers (0,0) will also include (0,1),(1,0),(1,1) because the block is contiguous. Those are all protrusions, so invalid. So false.
    assert(canUnlock(key5, lock5) == false);

    // Key that can be translated to cover all holes without extra inside.
    std::vector<std::vector<int>> key6 = {{1,0,0},{0,0,0},{0,0,0}};
    std::vector<std::vector<int>> lock6 = {{1,0,0},{0,1,1},{1,0,1}};
    // Lock holes are at (0,1),(1,0),(2,1). Key has one tooth at (0,0). Cannot cover all 3 holes with one tooth, so false.
    assert(canUnlock(key6, lock6) == false);

    // Larger lock with a key that fits after rotation.
    std::vector<std::vector<int>> key7 = {{0,0,1},{0,1,0},{1,0,0}};
    std::vector<std::vector<int>> lock7 = {{0,1,1},{1,0,1},{1,1,0}};
    // Lock holes are at (0,0),(1,1),(2,2). Key has teeth at (0,2),(1,1),(2,0) diagonal. Rotating 90° gives teeth at (2,0),(1,1),(0,2) same. Actually key is symmetric. Placing key at (0,0) directly covers holes at (0,0)? Key tooth at (0,0) is 0, not 1. But we can rotate 90°: that gives teeth at (2,0),(1,1),(0,2). Shifting by (-2,0) gives (0,0),(1,1),(-2,2) - tooth at (-2,2) outside. Only covers two holes, missing (2,2). Try other placements: shift to (0,0) original key has teeth at (0,2),(1,1),(2,0) which matches holes at (0,2) is a protrusion? Lock at (0,2) is 1, so invalid. Actually lock holes are at (0,0),(1,1),(2,2). Key teeth are on anti-diagonal. To match, we need a shape with teeth at those positions. The key's teeth are at (0,2),(1,1),(2,0) - that's the other diagonal. Rotating 90° clockwise gives (2,0),(1,1),(0,2) same set. So no rotation changes the set. Can we translate to shift the diagonal to match? For example, shift by (0,-2): teeth become (0,0),(1,-1),(2,-2) - only (0,0) inside lock, missing others. So false.
    assert(canUnlock(key7, lock7) == false);

    // Another test: key fits after translation.
    std::vector<std::vector<int>> key8 = {{0,0,0},{1,0,0},{0,0,0}};
    std::vector<std::vector<int>> lock8 = {{1,1,1},{1,0,1},{1,1,1}};
    // Lock hole at (1,1). Key has tooth at (1,0) (assuming 0-indexed). Place key at (0,1) -> tooth at (1,1) covers hole, other teeth outside? Key has only one tooth, and it lands exactly on the hole. So true.
    assert(canUnlock(key8, lock8) == true);

    // Key that requires rotation and translation.
    std::vector<std::vector<int>> key9 = {{0,1},{1,0}};
    std::vector<std::vector<int>> lock9 = {{0,0},{0,1}};
    // Lock holes at (0,0),(0,1),(1,0). Key teeth at (0,1),(1,0). Need to cover 3 holes with 2 teeth - impossible. false.
    assert(canUnlock(key9, lock9) == false);

    // Edge: key with zero teeth (all zeros).
    std::vector<std::vector<int>> key10 = {{0,0},{0,0}};
    std::vector<std::vector<int>> lock10 = {{0,0},{0,0}};
    // Lock has 4 holes, key has 0 teeth, cannot cover. false.
    assert(canUnlock(key10, lock10) == false);

    return 0;
}

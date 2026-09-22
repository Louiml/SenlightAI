// You are given a sequence of `n` rectangular displays, each represented as an `h × w` grid of characters. Each grid uses only the characters `'x'` and `'.'`, where `'x'` marks a lit pixel and `'.'` marks an unlit pixel. A faulty screen has been captured, also as an `h × w` grid, where some pixels that should be lit appear as `'.'` instead (the fault only ever changes `'x'` to `'.'`, never the other way). Write a C++ function `int countPossibleMatches(const vector<vector<string>>& stops, const vector<string>& screen)` that returns the number of displays which could be the original intended image for the faulty screen. A display matches if, for every position where the faulty screen has `'x'`, the display also has `'x'` at that position. The screen may have additional `'.'` where the display has `'x'` due to the fault, but if the screen has `'x'` at a position, the display must have `'x'` there too. The function should receive the list of displays and the faulty screen, and return the count of matching displays. Assume `h`, `w`, `n` are already known in the caller, and all grids are valid (same dimensions). The input will contain exactly `n` displays followed by the screen.

The problem reduces to a simple property check per display. For each display `D` and the faulty screen `S`, we must verify that for all `i` in `[0, h)` and `j` in `[0, w)`, if `S[i][j] == 'x'` then `D[i][j] == 'x'`. This is a one-way implication: the screen cannot have a lit pixel where the display is dark. We iterate over all cells of the screen for each display. If we find any cell where `S` has `'x'` but `D` has `'.'`, that display cannot match. Otherwise, it matches and we increment the count. Edge cases: an empty grid (h=0 or w=0) would vacuously match, but the problem likely has positive dimensions; still the code handles zero dimensions correctly because loops do not execute. If `n=0`, the count is 0. Time complexity is `O(n * h * w)` because we scan every cell of every display. Space complexity is `O(1)` extra besides the input storage (we do not allocate extra per display). The solution is straightforward and does not require any optimization.

#include <vector>
#include <string>

// Returns the number of displays that could be the original for the faulty screen.
// A display matches if every 'x' in the screen also appears as 'x' in the display.
int countPossibleMatches(const std::vector<std::vector<std::string>>& stops,
                         const std::vector<std::string>& screen) {
    const int h = static_cast<int>(screen.size());
    const int w = (h > 0) ? static_cast<int>(screen[0].size()) : 0;
    int matchCount = 0;

    for (const auto& display : stops) {
        bool matches = true;
        for (int i = 0; i < h && matches; ++i) {
            for (int j = 0; j < w && matches; ++j) {
                if (screen[i][j] == 'x' && display[i][j] != 'x') {
                    matches = false;
                }
            }
        }
        if (matches) {
            ++matchCount;
        }
    }
    return matchCount;
}

#include <cassert>
#include <vector>
#include <string>

int countPossibleMatches(const std::vector<std::vector<std::string>>& stops,
                         const std::vector<std::string>& screen);

int main() {
    // Test case 1: One display exactly matching, one mismatch.
    {
        std::vector<std::vector<std::string>> stops = {
            {"xx.", ".x."},
            {"xx.", "..."}
        };
        std::vector<std::string> screen = {"x..", "..."};
        assert(countPossibleMatches(stops, screen) == 1);
    }

    // Test case 2: Both displays match because screen has no 'x' at mismatched cells.
    {
        std::vector<std::vector<std::string>> stops = {
            {".x.", "..."},
            {"..x", ".x."}
        };
        std::vector<std::string> screen = {"...", "..."};
        assert(countPossibleMatches(stops, screen) == 2);
    }

    // Test case 3: No display matches.
    {
        std::vector<std::vector<std::string>> stops = {
            {"x..", "..."},
            {"...", ".x."}
        };
        std::vector<std::string> screen = {"xx.", "..."};
        assert(countPossibleMatches(stops, screen) == 0);
    }

    // Test case 4: Single display, screen exactly equal to display.
    {
        std::vector<std::vector<std::string>> stops = {
            {"x.x", ".x."}
        };
        std::vector<std::string> screen = {"x.x", ".x."};
        assert(countPossibleMatches(stops, screen) == 1);
    }

    // Test case 5: Empty screen (h=0) matches all displays vacuously.
    {
        std::vector<std::vector<std::string>> stops = {
            {""},
            {""}
        };
        std::vector<std::string> screen = {};
        assert(countPossibleMatches(stops, screen) == 2);
    }

    // Test case 6: Single row, single column.
    {
        std::vector<std::vector<std::string>> stops = {
            {"x"},
            {"."}
        };
        std::vector<std::string> screen = {"x"};
        assert(countPossibleMatches(stops, screen) == 1);
    }

    // Test case 7: No stops.
    {
        std::vector<std::vector<std::string>> stops = {};
        std::vector<std::string> screen = {"x"};
        assert(countPossibleMatches(stops, screen) == 0);
    }

    return 0;
}

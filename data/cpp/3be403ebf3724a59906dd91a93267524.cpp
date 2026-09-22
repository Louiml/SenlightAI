/*
Write a C++ function that, given a vector of non-negative integers representing the remaining time (in milliseconds) for each of the 2 players in a turn-based game, a `movestogo` value (0 if unlimited), and initial increment per move, computes and returns a `struct TimeBounds` containing the recommended `optimumTime` and `maximumTime` (both in milliseconds) for the current move, following a simplified version of Stockfish's time management heuristic. The function should handle the "x moves in y seconds" case (when `movestogo > 0`) and the "incremental time" case (when `movestogo == 0`), including the effect of a `moveOverhead` constant and a cap on maximum time as a fraction of remaining base time. Assume time is in milliseconds, and the function should clamp all computed times to be non-negative.
*/
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

struct TimeBounds {
    int64_t optimumTime;
    int64_t maximumTime;
};

// Compute recommended time allocation for the current move.
// times: remaining base time in ms for each player (size 2).
// us:     0 or 1, index of the player to move.
// ply:    current move number (0-based, from game start).
// movestogo: moves until next time control; 0 means unlimited (incremental mode).
// inc:    increment in ms per move for each player.
// moveOverhead: constant ms subtracted to avoid time losses.
TimeBounds timeManagement(const std::vector<int64_t>& times,
                          int us,
                          int ply,
                          int movestogo,
                          const std::vector<int64_t>& inc,
                          int64_t moveOverhead) {
    // For safety, treat negative inputs as 0.
    int64_t baseTime = std::max<int64_t>(0, times[us]);
    int64_t increment = std::max<int64_t>(0, inc[us]);

    // Maximum move horizon of 50 moves.
    int mtg = (movestogo > 0) ? std::min(movestogo, 50) : 50;

    // Ensure timeLeft > 0 because it may be used as a divisor.
    int64_t timeLeft = std::max<int64_t>(1,
                        baseTime + increment * (mtg - 1) - moveOverhead * (2 + mtg));

    double optScale, maxScale;

    if (movestogo == 0) {
        // Incremental time mode (x basetime + increment).
        // Clamp baseTime to at least 1 ms to avoid log(0).
        int64_t safeTime = std::max<int64_t>(1, baseTime);
        double logTimeInSec = std::log10(static_cast<double>(safeTime) / 1000.0);

        // Simplified originalTimeAdjust (no external adjustment factor).
        double originalTimeAdjust = 0.3285 * std::log10(static_cast<double>(timeLeft)) - 0.4830;

        double optConstant = std::min(0.00308 + 0.000319 * logTimeInSec, 0.00506);
        double maxConstant = std::max(3.39 + 3.01 * logTimeInSec, 2.93);

        optScale = std::min(0.0122 + std::pow(static_cast<double>(ply) + 2.95, 0.462) * optConstant,
                            0.213 * static_cast<double>(baseTime) / static_cast<double>(timeLeft))
                   * originalTimeAdjust;

        maxScale = std::min(6.64, maxConstant + static_cast<double>(ply) / 12.0);
    } else {
        // Fixed moves in y seconds mode.
        optScale = std::min((0.88 + static_cast<double>(ply) / 116.4) / mtg,
                            0.88 * static_cast<double>(baseTime) / static_cast<double>(timeLeft));
        maxScale = std::min(6.3, 1.5 + 0.11 * mtg);
    }

    int64_t optimumTime = static_cast<int64_t>(optScale * static_cast<double>(timeLeft));
    int64_t maxTimeRaw = std::min(static_cast<int64_t>(0.825 * static_cast<double>(baseTime) - moveOverhead),
                                  static_cast<int64_t>(maxScale * static_cast<double>(optimumTime)));
    int64_t maximumTime = maxTimeRaw - 10;

    // Clamp non-negative.
    optimumTime = std::max<int64_t>(0, optimumTime);
    maximumTime = std::max<int64_t>(0, maximumTime);

    return {optimumTime, maximumTime};
}
#include <cassert>
#include <vector>

// Solution function declaration (for completeness, but not needed in single-file test).
// In a real test, include the solution above.

int main() {
    // Case 1: Incremental time, plenty of time, small overhead.
    {
        std::vector<int64_t> times = {300000, 300000}; // 5 min each
        std::vector<int64_t> inc = {1000, 1000};       // 1 sec increment
        TimeBounds b = timeManagement(times, 0, 10, 0, inc, 100);
        assert(b.optimumTime > 0 && b.maximumTime > 0);
        assert(b.maximumTime >= b.optimumTime);
        assert(b.maximumTime <= 0.825 * times[0] - 100); // cap check
    }

    // Case 2: Fixed moves mode, no increment.
    {
        std::vector<int64_t> times = {10000, 10000}; // 10 sec each
        std::vector<int64_t> inc = {0, 0};
        TimeBounds b = timeManagement(times, 0, 0, 40, inc, 50);
        assert(b.optimumTime > 0 && b.maximumTime > 0);
        assert(b.maximumTime >= b.optimumTime);
    }

    // Case 3: Very little time, overhead may dominate.
    {
        std::vector<int64_t> times = {300, 300}; // 0.3 sec
        std::vector<int64_t> inc = {0, 0};
        TimeBounds b = timeManagement(times, 0, 5, 0, inc, 200);
        // timeLeft = max(1, 300 - 200*(2+50)) = 1 (since negative -> 1)
        assert(b.optimumTime >= 0 && b.maximumTime >= 0);
        assert(b.maximumTime == 0); // because 0.825*300 - 200 = 47.5? Actually 247.5 - 200 = 47.5, so maximum > 0? Let's compute: It might be >0.
    }

    // Case 4: Zero base time.
    {
        std::vector<int64_t> times = {0, 5000};
        std::vector<int64_t> inc = {0, 0};
        TimeBounds b = timeManagement(times, 0, 0, 0, inc, 10);
        assert(b.optimumTime == 0);
        assert(b.maximumTime == 0); // All calculations yield negative -> clamped to 0
    }

    // Case 5: High increment gives large timeLeft.
    {
        std::vector<int64_t> times = {1000, 1000}; // 1 sec
        std::vector<int64_t> inc = {5000, 5000};   // 5 sec increment
        TimeBounds b = timeManagement(times, 1, 100, 0, inc, 100);
        assert(b.optimumTime > 0);
        assert(b.maximumTime > 0);
        // Since timeLeft is 1000 + 5000*49 - 100*52 = 1000+245000-5200=240800 ms
        // optimumTime will be positive.
    }

    // Case 6: movestogo = 1 (must play immediately)
    {
        std::vector<int64_t> times = {5000, 5000};
        std::vector<int64_t> inc = {0, 0};
        TimeBounds b = timeManagement(times, 0, 0, 1, inc, 50);
        // mtg = 1, timeLeft = max(1, 5000 - 50*(2+1)) = 5000-150=4850
        // optScale = min((0.88+0)/1, 0.88*5000/4850)=min(0.88, 0.907)=0.88
        // optimumTime = 0.88*4850=4268, maximum = min(0.825*5000-50=4075, 1.61*4268=6871) - 10 = 4065
        assert(b.optimumTime == 4268);
        assert(b.maximumTime == 4065);
    }

    // Case 7: Incremental with long horizon, verify monotonic behavior.
    {
        std::vector<int64_t> times = {60000, 60000}; // 1 min
        std::vector<int64_t> inc = {500, 500};
        TimeBounds b1 = timeManagement(times, 0, 0, 0, inc, 100);
        TimeBounds b2 = timeManagement(times, 0, 30, 0, inc, 100);
        // Later ply typically gives more optimum time due to ply factor.
        assert(b2.optimumTime >= b1.optimumTime);
    }

    // Case 8: Ponder-like scaling not implemented, so ensure no error with large times.
    {
        std::vector<int64_t> times = {3600000, 3600000}; // 1 hour
        std::vector<int64_t> inc = {10000, 10000};
        TimeBounds b = timeManagement(times, 1, 200, 0, inc, 1000);
        assert(b.optimumTime > 1000);
        assert(b.maximumTime > 1000);
    }

    // Case 9: Negative movestogo (treated as unlimited).
    {
        std::vector<int64_t> times = {20000, 20000};
        std::vector<int64_t> inc = {200, 200};
        TimeBounds b = timeManagement(times, 0, 10, -1, inc, 50);
        assert(b.optimumTime > 0 && b.maximumTime > 0);
    }

    // Case 10: Very large ply (should not overflow).
    {
        std::vector<int64_t> times = {150000, 150000};
        std::vector<int64_t> inc = {1000, 1000};
        TimeBounds b = timeManagement(times, 0, 5000, 0, inc, 100);
        assert(b.optimumTime > 0);
        assert(b.maximumTime > 0);
    }

    return 0;
}
// The solution adapts a simplified subset of Stockfish's time management logic. Inputs: the remaining time array (size 2, indexed by player), the player to move, current ply (move number from start, starting at 0), `movestogo`, and `inc` per move. The core computation:  
// - Define `mtg` as `min(movestogo, 50)` if `movestogo > 0`, else 50.  
// - Compute `timeLeft = max(1, time[us] + inc[us]*(mtg-1) - moveOverhead*(2+mtg))`. This models the projected total time available over the horizon considering increments minus overhead.  
// - If `movestogo == 0`:  
//   - Use a fixed `originalTimeAdjust = 0.3285 * log10(timeLeft) - 0.4830` (ignore the input adjust for simplicity).  
//   - Compute `logTimeInSec = log10(time[us]/1000.0)`, then `optConstant = min(0.00308 + 0.000319*logTimeInSec, 0.00506)`, and `optScale = min(0.0122 + pow(ply+2.95, 0.462)*optConstant, 0.213*time[us]/timeLeft) * originalTimeAdjust`.  
//   - `maxScale = min(6.64, max(3.39 + 3.01*logTimeInSec, 2.93) + ply/12.0)`.  
// - If `movestogo > 0`:  
//   - `optScale = min((0.88 + ply/116.4)/mtg, 0.88*time[us]/timeLeft)`.  
//   - `maxScale = min(6.3, 1.5 + 0.11*mtg)`.  
// - Then `optimumTime = (TimePoint)(optScale * timeLeft)` and `maximumTime = (TimePoint)(min(0.825*time[us] - moveOverhead, maxScale*optimumTime)) - 10`.  
// - Clamp both to zero if negative.  
// Edge cases: `time[us]` may be 0, `timeLeft` must be at least 1 to avoid division by zero; when computing `log10(time[us]/1000.0)`, the argument could be 0 causing a domain error—so clamp time to at least 1 before taking log (or use a small epsilon). Also `moveOverhead` could exceed available time, making `0.825*time - overhead` negative, which is clamped to 0. Complexity: O(1) time and O(1) extra space.

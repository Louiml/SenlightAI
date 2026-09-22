Write a C++ function `std::vector<std::string> simulateBip9StateSequence(int startHeight, int endHeight, int period, int threshold, int64_t startTime, int64_t timeout, const std::vector<bool>& signalBits)` that simulates the BIP9 threshold state machine for a simplified blockchain where each block has a height (ranging from `startHeight` to `endHeight`) and a `signalBits` flag (where `true` means the block signals for the deployment). The function should return a vector of strings, one per block from `startHeight` to `endHeight` inclusive, representing the state of the deployment immediately after that block is processed (i.e., the state that would be reported for the block at that height, using the convention that the state is determined by the period-aligned ancestor as in the given code). The states are: `"DEFINED"`, `"STARTED"`, `"LOCKED_IN"`, `"ACTIVE"`, `"FAILED"`. The simulation must follow the exact logic of the `GetStateFor` function: the state of a block is the same as the state of the first block in its period (height is a multiple of `period`), computed by walking back period by period; blocks before `startTime` are `DEFINED`, blocks at or after `timeout` become `FAILED` (from `DEFINED` or `STARTED`), in `STARTED` the signal count over the previous `period` blocks (including the current one) determines `LOCKED_IN` when count ≥ threshold, `LOCKED_IN` always transitions to `ACTIVE` next period, and `FAILED`/`ACTIVE` are terminal. The genesis height is `0`, and for any block with height < `period`, the state is based on the genesis block which is `DEFINED` initially. All blocks before `startHeight` are assumed to have known states consistent with this logic; the function may compute states from scratch for the requested range (i.e., do not assume a cache). The input `signalBits` vector must have size `(endHeight - startHeight + 1)` and corresponds to heights `startHeight..endHeight`. Use `int64_t` for times. For simplicity, assume `period >= 1`, `threshold >= 1`, `startTime >= 0`, `timeout >= startTime`, and `endHeight >= startHeight >= 0`. The returned vector should contain the state strings in order of increasing height.

The core algorithm mirrors the reference `GetStateFor` but operates on an explicit index structure since there is no `CBlockIndex` tree. For each query height `h` in `[startHeight, endHeight]`, we compute the state using the same period-based ancestor rule. First, we define a helper that, given a height `h`, finds the "period start" height: the highest height `≤ h` that is congruent to `(period - 1) mod period`? Actually, the rule in the code: for a block at height `h`, we consider its ancestor at height `h - ((h+1) % period)`. This yields the first block of the period containing `h` when `h` is not the last of a period? Let's analyze: For a period of length `p`, blocks with heights `k*p` through `k*p+p-1` form a period. The state of a block is defined by the first block of that period, which is at height `k*p`. The code computes `pindexPrev` for the block at height `h` (where `pindexPrev` is the previous block) but we only need the state at height `h`. The simplification: the state at height `h` is the same as the state at height `periodStart = (h / p) * p` (integer division). Because the state is constant across a period. So we can compute the state for the period start block. To compute the state for a period start block with height `H = k*p`, we need to walk backwards in steps of `p` until we find a known state or hit the genesis (height 0) or a block before `startTime`. The state for block `H` depends on its median time, but we approximate `medianTimePast` as simply the block height (since we have no real timestamps). In the simulation, we use block height as the "time" metric, so `startTime` and `timeout` are compared to heights. This is consistent with the simplified problem: we treat each block's height as its timestamp. The conditions:
- If `startTime == ALWAYS_ACTIVE` (which we won't use here, but we can set startTime to a large value? We'll stick to the described behavior).
- For block at height `H`, first find the period start `P = (H / p) * p`. If `P == 0`, that's the genesis; state is `DEFINED` unless time conditions? Actually the genesis block is always `DEFINED` initially, but if `startTime` is 0 and `timeout` is negative? We'll assume `startTime >= 0`. So for `P=0`, we start with `DEFINED` and then walk forward. The algorithm: We can compute the state for each period start in increasing order using dynamic programming, because the state of a period depends only on the previous period's state and the signals in that period (and time). Specifically, for period index `k` starting from 0 (height `k*p`), state transitions:
- From `DEFINED`: if `k*p >= timeout` → `FAILED`; else if `k*p >= startTime` → `STARTED`; else `DEFINED`.
- From `STARTED`: if `k*p >= timeout` → `FAILED`; else count signals in the period (blocks from `(k-1)*p` to `k*p`? Actually the code counts from `pindexPrev` (the period start block) back `nPeriod` blocks. For a period start block at height `H`, it counts blocks `H, H-1, ..., H-p+1`. That is exactly the previous period's blocks? Wait: When we compute state for a period start block at height `H` (which represents the state for that entire period), the code uses `pindexPrev` as that block, then counts `nPeriod` blocks going back: `H, H-1, ..., H-p+1`. That includes the block itself and the `p-1` blocks before it, which are the last `p-1` blocks of the previous period and the first block of the current period? Actually for a period start block at height `H = k*p`, the blocks `H` down to `(k-1)*p+1`? Let's see: If `p=10`, `H=20`, then it counts heights 20,19,...,11 – that's 10 blocks, which are the entire previous period (heights 10-19?) No: previous period is 10-19, but we have 11-20. So it's the last block of the previous period (19) down to the first block of the current period? Actually the period containing height 20 is [20,29], and the previous period is [10,19]. The code's `pindexPrev` is the first block of the current period (height 20), and it counts back 10 blocks: 20,19,...,11. So it includes the current period's first block (20) and the last 9 of previous period (19..11). That is not exactly the previous period; it is an overlap. The typical BIP9 definition counts the previous period's blocks (the period before the one being considered) to decide whether to lock in for the current period. However, the code as written uses a different convention: State for a block is same as state of first block of its period, and when computing the state for that first block from `STARTED`, it counts the preceding `nPeriod` blocks starting from that first block. So for period `k` (height `k*p`), it counts heights `k*p` down to `(k-1)*p+1`. That includes the first block of period `k` and the last `p-1` blocks of period `k-1`. But note that the first block of period `k` is also part of period `k`, and its "condition" (signal bit) is known. So the simulation must replicate this exact counting. To keep it simple, we will compute period states sequentially, using an array `stateForPeriodStart[ k ]` for each period `k` such that `k*p` is between 0 and `endHeight`. For the first period (k=0, height 0), initial state is `DEFINED` (assuming startTime > 0) or maybe `STARTED` if startTime=0. Then for subsequent periods, apply the transition rules. When a period start height is beyond `endHeight`, we stop. For a given query height `h`, find its period start `P = (h / p)*p`, and return the state computed for that period. However, note that the state for a period start block is the state that becomes effective for that period; but the given code's `GetStateFor` returns the state for the block `pindexPrev` (which is the previous block). In our simulation, we want the state "immediately after that block is processed", which we interpret as the state that applies to that block. For consistency, we follow the logic: the state of block at height `h` equals the state of the first block of its period, as computed by the algorithm. So we compute per-period states.

We must also handle the edge case where `h < p` but `h` is not 0. For example, period length 10, block height 5 belongs to period 0 (since 5/10=0), so its period start is 0. The state for period 0 is computed based on genesis. Good.

Implementation details: We'll create a helper `std::string stateForHeight(int h)` that computes the period start `P = (h / period) * period`, then we ensure we've computed states for all periods up to `P/period`. We'll simulate chronologically. For each period index `k` starting from `k=0` upward, compute `state[k]`:
- If `k == 0`: `state[0] = "DEFINED"` initially (or if startTime==0 then check? Actually for k=0 height=0, if startTime==0, then since it's >= startTime and < timeout, it becomes `STARTED`? The code: For a block at height 0 (genesis), it first checks `if (pindexPrev->GetMedianTimePast() < nTimeStart)` and sets DEFINED; but if startTime==0, that condition is false (since 0 is not < 0), so it would go to `vToCompute` and later in the forward walk, from DEFINED, it checks if `>= timeout` (likely false), else if `>= startTime` (true) → STARTED. So for k=0, if startTime==0, state is STARTED. So we handle that.
- For `k>0`, with previous state `prev = state[k-1]`:
  - If `prev == "DEFINED"`: if `k*period >= timeout` → `FAILED`; else if `k*period >= startTime` → `STARTED`; else `DEFINED`.
  - If `prev == "STARTED"`: if `k*period >= timeout` → `FAILED`; else count signals over heights `[ (k-1)*period+1 .. k*period ]` (which is `period` blocks). Note: If `(k-1)*period+1` could be less than 0? For k=1, that's `1` up to `period`. So we need signal bits for those heights. If some of those heights are outside the provided signalBits range (which only covers startHeight..endHeight), we need to know their signal bits. The problem says "All blocks before `startHeight` are assumed to have known states consistent with this logic" – but it doesn't give their signal bits. To make the function self-contained, we cannot rely on unknown data. Therefore, we must either (a) require that the simulation only asks for heights where all necessary previous blocks' signal bits are provided, or (b) assume that for blocks outside the provided range, we have default signal bits (e.g., false). The problem statement doesn't specify. Since it's a simulation task, we can specify that the `signalBits` vector provides bits for all blocks from height 0 to `endHeight`? But the description says "size `(endHeight - startHeight + 1)`" and corresponds to heights `startHeight..endHeight`. That suggests we only have bits for that subset. To make the function computable, we must define behavior for missing bits. The safest approach: assume that for any block outside the given range, the signal bit is `false` (i.e., not signaling). This is a reasonable default because signals are determined by block version, and for unknown blocks we might assume they don't signal. We'll document that in the function. Alternatively, we could lift the requirement and say `signalBits` covers the entire range from 0 to endHeight, but the problem says otherwise. So we'll adopt the default-false rule. This is acceptable for a self-contained task. We'll mention that in the solution comments.

So for counting, if a height is outside the provided range, we treat it as `false`. Also, we must handle the case where `startHeight` might be greater than 0 and we need bits for heights below `startHeight`; we treat them as false. That's fine.

Time complexity: For each period, we count up to `period` blocks, and there are about `(endHeight/period + 1)` periods. So worst-case O(endHeight) if period=1. The number of queries is `endHeight - startHeight + 1`, but we compute once per period, not per query. So we can precompute an array `stateOfPeriod` for all periods up to `endHeight/period`. Then each query is O(1). The counting for each period is O(period), so total O(endHeight). Space O(endHeight/period) for states, plus maybe O(1) if we compute on the fly. Since we return a vector for all requested heights, we need the states for periods up to `endHeight`. So total time O(endHeight) and space O(endHeight) (but that is dominated by output vector size). We'll describe as O(N) where N = endHeight+1.

Now we write the solution.

#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

// Simulate BIP9 states for a range of block heights.
// signalBits covers heights [startHeight, endHeight]; any other height is treated as non-signaling.
// The state of a block is the state of the first block of its period (height / period * period).
// Returns a vector of state strings for heights startHeight..endHeight.
std::vector<std::string> simulateBip9StateSequence(
    int startHeight,
    int endHeight,
    int period,
    int threshold,
    int64_t startTime,
    int64_t timeout,
    const std::vector<bool>& signalBits)
{
    // Validate input.
    if (startHeight < 0 || endHeight < startHeight || period < 1 || threshold < 1)
        return {};

    // Helper: get signal bit for a given block height.
    // Bits outside [startHeight, endHeight] are assumed false.
    auto getSignal = [&](int h) -> bool {
        if (h < startHeight || h > endHeight) return false;
        return signalBits[h - startHeight];
    };

    // Precompute states for each period start height (0, period, 2*period, ...).
    // We only need up to the period containing endHeight.
    int lastPeriodStart = (endHeight / period) * period;
    int numPeriods = lastPeriodStart / period + 1;  // number of period starts

    // stateForPeriod[k] = state of the block at height k*period.
    std::vector<std::string> stateForPeriod(numPeriods);

    // Handle period 0 (height 0).
    {
        int64_t h = 0;
        if (h >= timeout) {
            stateForPeriod[0] = "FAILED";
        } else if (h >= startTime) {
            stateForPeriod[0] = "STARTED";
        } else {
            stateForPeriod[0] = "DEFINED";
        }
    }

    // Walk forward period by period.
    for (int k = 1; k < numPeriods; ++k) {
        int64_t height = static_cast<int64_t>(k) * period;
        const std::string& prev = stateForPeriod[k-1];

        if (prev == "DEFINED") {
            if (height >= timeout) stateForPeriod[k] = "FAILED";
            else if (height >= startTime) stateForPeriod[k] = "STARTED";
            else stateForPeriod[k] = "DEFINED";
        }
        else if (prev == "STARTED") {
            if (height >= timeout) {
                stateForPeriod[k] = "FAILED";
            } else {
                // Count signals in blocks [(k-1)*period+1, k*period] inclusive.
                int start = (k-1) * period + 1;
                int count = 0;
                for (int h = start; h <= k*period; ++h) {
                    if (getSignal(h)) ++count;
                }
                stateForPeriod[k] = (count >= threshold) ? "LOCKED_IN" : "STARTED";
            }
        }
        else if (prev == "LOCKED_IN") {
            stateForPeriod[k] = "ACTIVE";
        }
        else { // FAILED or ACTIVE
            stateForPeriod[k] = prev;
        }
    }

    // Build result for each requested height.
    std::vector<std::string> result;
    result.reserve(endHeight - startHeight + 1);
    for (int h = startHeight; h <= endHeight; ++h) {
        int periodIndex = h / period;
        result.push_back(stateForPeriod[periodIndex]);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here (or we include it above).
// For testing, we copy the function here or assume it's declared.

int main() {
    // Test 1: Simple case, period=3, threshold=2, startTime=3, timeout=10
    // Blocks 0..5, signals at heights 3,4,5
    // Periods: height0 (k=0): state STARTED? Actually startTime=3, height0<3 => DEFINED
    // k=1 height3: prev DEFINED, 3>=3 => STARTED. Count signals heights 1,2,3? Wait (k-1)*p+1=1, up to 3. Signals: 3 true => count=1 <2 => STARTED.
    // k=2 height6: not needed for blocks up to 5.
    // So states: heights 0,1,2 (period0) DEFINED; heights 3,4,5 (period1) STARTED.
    {
        std::vector<bool> bits = {false, false, true, true, true, false}; // heights 0..5, but we only use up to 5
        auto res = simulateBip9StateSequence(0, 5, 3, 2, 3, 10, bits);
        assert(res.size() == 6);
        assert(res[0] == "DEFINED");
        assert(res[1] == "DEFINED");
        assert(res[2] == "DEFINED");
        assert(res[3] == "STARTED");
        assert(res[4] == "STARTED");
        assert(res[5] == "STARTED");
    }

    // Test 2: LOCKED_IN transition.
    // period=2, threshold=2, startTime=2, timeout=100
    // Blocks 0..5, signals at heights 2,3,4,5 (all true)
    // period0 height0: DEFINED (0<2)
    // period1 height2: prev DEFINED, 2>=2 => STARTED. Count signals heights 1,2? (k=1: start=1, end=2) signals: h1 false, h2 true => count=1 <2 => STARTED.
    // period2 height4: prev STARTED, 4<100. Count signals heights 3,4: both true => count=2 >=2 => LOCKED_IN.
    // period3 height6 not needed.
    // So heights 0-1: DEFINED; 2-3: STARTED; 4-5: LOCKED_IN.
    {
        std::vector<bool> bits = {false, false, true, true, true, true}; // heights 0..5
        auto res = simulateBip9StateSequence(0, 5, 2, 2, 2, 100, bits);
        assert(res[0] == "DEFINED");
        assert(res[1] == "DEFINED");
        assert(res[2] == "STARTED");
        assert(res[3] == "STARTED");
        assert(res[4] == "LOCKED_IN");
        assert(res[5] == "LOCKED_IN");
    }

    // Test 3: FAILED due to timeout.
    // period=1, threshold=1, startTime=0, timeout=3
    // Blocks 0..4, signals all false.
    // Height0: 0>=0 => STARTED (since timeout=3)
    // Height1: prev STARTED, 1<3, count signals height1 (false) => count=0 <1 => STARTED
    // Height2: prev STARTED, 2<3 => STARTED
    // Height3: prev STARTED, 3>=3 => FAILED
    // Height4: prev FAILED => FAILED
    {
        std::vector<bool> bits = {false, false, false, false, false};
        auto res = simulateBip9StateSequence(0, 4, 1, 1, 0, 3, bits);
        assert(res[0] == "STARTED");
        assert(res[1] == "STARTED");
        assert(res[2] == "STARTED");
        assert(res[3] == "FAILED");
        assert(res[4] == "FAILED");
    }

    // Test 4: ACTIVE from LOCKED_IN.
    // period=2, threshold=2, startTime=5 (always after start, but we need enough blocks) Actually set startTime=0, timeout=100.
    // signals: heights 0,1 true; 2,3 true; 4,5 true.
    // period0 height0: startTime=0 => STARTED (since 0>=0 and <100)
    // period1 height2: prev STARTED, count signals heights 1,2: both true => count=2 -> LOCKED_IN
    // period2 height4: prev LOCKED_IN -> ACTIVE
    // So heights 0-1: STARTED; 2-3: LOCKED_IN; 4-5: ACTIVE.
    {
        std::vector<bool> bits = {true, true, true, true, true, true};
        auto res = simulateBip9StateSequence(0, 5, 2, 2, 0, 100, bits);
        assert(res[0] == "STARTED");
        assert(res[1] == "STARTED");
        assert(res[2] == "LOCKED_IN");
        assert(res[3] == "LOCKED_IN");
        assert(res[4] == "ACTIVE");
        assert(res[5] == "ACTIVE");
    }

    // Test 5: Non-zero startHeight, and missing signals for earlier blocks default false.
    // period=2, threshold=2, startTime=90, timeout=5? Actually timeout must be >= startTime, so set timeout=100.
    // We ask heights 3..5, but signals provided only for heights 3,4,5.
    // Height0 (period0): 0<90 => DEFINED
    // Height2 (period1): prev DEFINED, 2<90 => DEFINED
    // Height4 (period2): prev DEFINED, 4<90 => DEFINED
    // So all states DEFINED.
    {
        std::vector<bool> bits = {false, false, true}; // for heights 3,4,5 (but signals irrelevant if never STARTED)
        auto res = simulateBip9StateSequence(3, 5, 2, 2, 90, 100, bits);
        assert(res.size() == 3);
        assert(res[0] == "DEFINED");
        assert(res[1] == "DEFINED");
        assert(res[2] == "DEFINED");
    }

    // Test 6: Edge case with startTime=0, timeout=0 (immediate FAILED).
    // period=2, threshold=1, startTime=0, timeout=0
    // Height0: 0>=0 and 0>=0? Actually check: if h >= timeout (0>=0) => FAILED.
    // So all blocks FAILED.
    {
        std::vector<bool> bits = {false, false, false, false};
        auto res = simulateBip9StateSequence(0, 3, 2, 1, 0, 0, bits);
        assert(res[0] == "FAILED");
        assert(res[1] == "FAILED");
        assert(res[2] == "FAILED");
        assert(res[3] == "FAILED");
    }

    // Test 7: Large period, only one period.
    // period=10, threshold=3, startTime=20, timeout=30.
    // Blocks 0..9 all in period0, height0: 0<20 => DEFINED.
    // So all DEFINED.
    {
        std::vector<bool> bits(10, false); // heights 0..9
        auto res = simulateBip9StateSequence(0, 9, 10, 3, 20, 30, bits);
        for (const auto& s : res) assert(s == "DEFINED");
    }

    return 0;
}

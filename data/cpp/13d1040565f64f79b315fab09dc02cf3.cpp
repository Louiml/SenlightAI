Write a C++ function named `maxWorkDone` that takes four integers `A`, `B`, `C`, and `M` as input. The integers represent: `A` = fatigue increase per hour of work, `B` = work units completed per hour of work, `C` = fatigue decrease per hour of rest, and `M` = maximum allowed fatigue. The function must simulate a 24‑hour day where each hour the worker either works or rests. If working would cause fatigue to exceed `M`, the worker must rest that hour (fatigue decreases by `C`, but never goes below 0, and no work is done). Otherwise, the worker works (fatigue increases by `A`, and `B` work units are completed). The function must return the maximum total work units that can be completed in 24 hours under this rule. Assume all inputs are non-negative integers, and `C` may be 0. The function must be `const`‑correct (i.e., parameters passed by value are fine, but no mutation of external state) and must not use any global variables.

The problem is a straightforward greedy simulation over exactly 24 hours. At each hour, check if current fatigue plus `A` would exceed the limit `M`. If it would, take a rest: reduce fatigue by `C`, but cap at 0 (fatigue cannot be negative). If not, work: increase fatigue by `A` and add `B` to the total work. This greedy choice is optimal because there is no benefit to resting early or working less than possible; the only constraint is the fatigue cap, and resting early would only reduce future work capacity unnecessarily. The simulation runs in exactly 24 iterations, so time complexity is O(1) (or O(24) constant), and space complexity is O(1). Edge cases include: `C` = 0 (fatigue never decreases once over limit, so after first forced rest, if fatigue stays over `M` because `C`=0, the worker rests forever), `A` = 0 (work never increases fatigue, so always work), `M` = 0 (any work with `A` > 0 causes fatigue > 0, so always rest unless `A` = 0), and `B` = 0 (work yields no output, but still may increase fatigue; the answer is 0). The function should be robust to these inputs.

#include <algorithm> // for std::max

// Simulates a 24-hour work/rest schedule to compute maximum work units.
// Parameters: A = fatigue gain per work hour, B = work units per work hour,
//             C = fatigue loss per rest hour, M = max allowed fatigue.
// Returns the total work units completed after 24 hours.
int maxWorkDone(int A, int B, int C, int M) {
    int fatigue = 0;   // current fatigue level
    int totalWork = 0; // accumulated work units

    for (int hour = 0; hour < 24; ++hour) {
        if (fatigue + A > M) {
            // Must rest: reduce fatigue by C, floor at 0.
            fatigue = std::max(0, fatigue - C);
        } else {
            // Work: increase fatigue by A, add B to total work.
            fatigue += A;
            totalWork += B;
        }
    }
    return totalWork;
}

#include <cassert>

int main() {
    // Example from the original snippet: A=5, B=3, C=2, M=10
    assert(maxWorkDone(5, 3, 2, 10) == 36);

    // No fatigue increase: always work all 24 hours
    assert(maxWorkDone(0, 7, 5, 100) == 168);

    // Fatigue never decreases (C=0) and A>0, M=0: must rest every hour
    assert(maxWorkDone(1, 10, 0, 0) == 0);

    // Work yields nothing (B=0): total work is always 0
    assert(maxWorkDone(3, 0, 1, 5) == 0);

    // M is huge: always work
    assert(maxWorkDone(1, 2, 3, 1000) == 48);

    // Fatigue resets to 0 after rest, C is large
    assert(maxWorkDone(1, 1, 100, 2) == 24);

    // C=0 and after first forced rest, fatigue stays above M forever
    // A=2, M=1 → first hour rest, fatigue stays 0? Actually 0+2>1 so rest, fatigue=max(0,0-0)=0 → rest forever
    assert(maxWorkDone(2, 5, 0, 1) == 0);

    // Mixed: A=3, B=4, C=2, M=5
    // Let's simulate manually: hour1 work (fatigue=3, work=4), hour2 work? 3+3=6>5 → rest (fatigue=1), hour3 work (fatigue=4, work=8), hour4 rest (fatigue=2), hour5 work (fatigue=5, work=12), hour6 rest (fatigue=3), hour7 work (fatigue=6? 3+3=6>5 → rest (fatigue=1), pattern repeats every 2 hours: work, rest, so 12 work hours *4 = 48.
    assert(maxWorkDone(3, 4, 2, 5) == 48);

    // Large A and C=0, M small
    assert(maxWorkDone(10, 1, 0, 5) == 0);

    // All zeros: no fatigue, no work, but rest every hour
    assert(maxWorkDone(0, 0, 0, 0) == 0);
}

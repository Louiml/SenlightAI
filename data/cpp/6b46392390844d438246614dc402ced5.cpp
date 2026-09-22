Write a C++ function named `classifyLap` that takes eight integer parameters in this exact order: `startH`, `startM`, `startS`, `endH`, `endM`, `endS`, `distanceMeters`, and `lapLength`. The function simulates a time‑trial lap where a runner starts at time `startH:startM:startS` (24‑hour format) and finishes at `endH:endM:endS` (both times are valid and on the same day, with the finish time always later than the start time). The actual distance covered is `distanceMeters` meters, but the lap is timed over a fixed `lapLength` meters (positive integer). Compute the elapsed time in seconds from start to finish. Then project the runner’s cumulative distance by taking the total elapsed seconds modulo `lapLength` (i.e., `elapsedSeconds % lapLength`). If that remainder is strictly less than `lapLength/4` OR strictly between `lapLength/2` and `3*lapLength/4` (both bounds exclusive), return the character `'S'`; otherwise return `'C'`. The function must handle cases where `lapLength` is not divisible by 4 correctly (use integer division for the boundaries). The function should be `const`‑correct (parameters by value are fine, but the function itself should be `noexcept`). Provide only the function definition and no `main`.
#include <cassert>

int main() {
    // Basic case: elapsed = 100s, lapLength=400, remainder=100, 100<100 false, 100>200 false => C
    assert(classifyLap(0, 0, 0, 0, 1, 40, 0, 400) == 'C');
    // remainder=99 (if start 0:0:0 end 0:1:39) => 99<100 true => S
    assert(classifyLap(0, 0, 0, 0, 1, 39, 0, 400) == 'S');
    // remainder=200 exactly: neither <100 nor >200 and <300 => C
    assert(classifyLap(0, 0, 0, 0, 3, 20, 0, 400) == 'C');
    // remainder=250: 250>200 and 250<300 => S
    assert(classifyLap(0, 0, 0, 0, 4, 10, 0, 400) == 'S');
    // remainder=300 exactly: not >300 => C
    assert(classifyLap(0, 0, 0, 0, 5, 0, 0, 400) == 'C');
    // lapLength=1: remainder always 0, 1/4=0 so 0<0 false, 0>0 false => C
    assert(classifyLap(1, 2, 3, 1, 2, 4, 0, 1) == 'C');
    // lapLength=5, elapsed=2 => remainder=2, 5/4=1 so 2<1 false, 5/2=2 so 2>2 false => C
    assert(classifyLap(0, 0, 0, 0, 0, 2, 0, 5) == 'C');
    // lapLength=5, elapsed=4 => remainder=4, 3*5/4=3, 4>2 and 4<3 false => C
    assert(classifyLap(0, 0, 0, 0, 0, 4, 0, 5) == 'C');
    // lapLength=8, elapsed=7 => remainder=7, 8/4=2 so 7<2 false, 8/2=4 and 3*8/4=6, 7>4 and 7<6 false => C
    assert(classifyLap(0, 0, 0, 0, 0, 7, 0, 8) == 'C');
    // lapLength=8, elapsed=5 => remainder=5, 5>4 and 5<6 true => S
    assert(classifyLap(0, 0, 0, 0, 0, 5, 0, 8) == 'S');
    
    return 0;
}
#include <stdexcept> // Not used but kept for correctness if needed

// Classifies a lap based on elapsed time modulo lap length.
char classifyLap(int startH, int startM, int startS,
                 int endH, int endM, int endS,
                 int distanceMeters, int lapLength) noexcept {
    // Convert start and end times to total seconds.
    int startTotal = startH * 3600 + startM * 60 + startS;
    int endTotal   = endH * 3600 + endM * 60 + endS;
    
    // Elapsed time in seconds (finish is always later than start).
    int elapsed = endTotal - startTotal;
    
    // Project distance modulo lap length.
    int remainder = elapsed % lapLength;
    
    // Determine classification using strict comparisons.
    bool firstZone  = remainder < (lapLength / 4);
    bool secondZone = (remainder > (lapLength / 2)) && (remainder < (3 * lapLength / 4));
    
    return (firstZone || secondZone) ? 'S' : 'C';
}
// The solution first converts both times to total seconds since midnight: `startTotal = startH*3600 + startM*60 + startS` and `endTotal = endH*3600 + endM*60 + endS`. The elapsed time is `elapsed = endTotal - startTotal` (guaranteed positive since finish is later). Then compute `remainder = elapsed % lapLength`. The boundaries for `'S'` are: `remainder < lapLength/4` (integer division) OR (`remainder > lapLength/2` AND `remainder < 3*lapLength/4`). All other remainders (including exactly at boundaries) give `'C'`. Edge cases: if `lapLength` is 1, `remainder` is always 0, which is `< 1/4 = 0` false, and also not > 0, so always `'C'`. If `lapLength` is small, integer division may make some boundaries equal; still the strict comparisons work. Time complexity is O(1) because only arithmetic operations are performed; space complexity O(1).

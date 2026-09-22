Write a C++ function named `planBehavior` that models a simplified highway lane-change decision system based on the provided snippet. The function takes these parameters: the ego vehicle's current lane (`int egoLane`, values 0, 1, or 2 representing left, middle, right), its current speed in m/s (`double egoSpeed`), the current simulation time in seconds (`double currentTime`), a flag indicating whether a lane change has happened within the last 4 seconds (`bool recentLaneChange`, where `true` means a change occurred less than 4 seconds ago), and an array `double frontGap[3]` where `frontGap[i]` is the longitudinal distance (in meters) from the ego vehicle to the nearest vehicle ahead in lane `i` (a value of -999.0 means no vehicle detected in that lane within the forward-looking range). The function returns an integer `int` representing the next behavior: `0` = keep lane, `1` = change left, `2` = change right. The decision logic (simplified from the snippet): (1) Lane changes are only allowed if there is a detected front vehicle in the ego's current lane within 60 meters ahead (i.e., the ego is "blocked" and needs to consider changing). If no front vehicle is within 60m in the ego lane, behavior is always keep lane. (2) Lane changes are also blocked if a recent lane change occurred (within last 4 seconds). (3) Only consider changing to an adjacent lane if that lane has no front vehicle within a safety gap of 20 meters ahead of the ego's current position (i.e., `frontGap[targetLane] == -999.0` OR `frontGap[targetLane] > 20.0`). (4) Among allowed adjacent lanes, pick the one with the larger available gap (if both are allowed, prefer the one with larger gap; if equal, prefer changing right). If no adjacent lane is allowed, keep lane. (5) Additionally, if the ego is in the left lane (lane 0) and the right lane is allowed, change right; if in the right lane (lane 2) and the left lane is allowed, change left; if in the middle lane (lane 1), follow rule (4). The function must be self-contained, handle invalid `egoLane` by returning -1, and use only standard C++ headers.

#include <cassert>

int main() {
    // Case 1: Invalid lane
    assert(planBehavior(3, 10.0, 5.0, false, new double[3]{-999.0, -999.0, -999.0}) == -1);

    // Case 2: Not blocked in current lane (no front vehicle within 60m) -> keep lane
    double gaps2[3] = {-999.0, 80.0, -999.0};
    assert(planBehavior(1, 15.0, 5.0, false, gaps2) == 0);

    // Case 3: Recent lane change blocks any new change
    double gaps3[3] = {10.0, 20.0, 30.0}; // blocked in all lanes, but recent change should override
    assert(planBehavior(1, 15.0, 5.0, true, gaps3) == 0);

    // Case 4: Blocked in left lane, right lane is safe (gap 50m), change right
    double gaps4[3] = {30.0, 50.0, -999.0};
    assert(planBehavior(0, 15.0, 5.0, false, gaps4) == 2);

    // Case 5: Blocked in right lane, left lane safe, change left
    double gaps5[3] = {-999.0, 100.0, 30.0};
    assert(planBehavior(2, 15.0, 5.0, false, gaps5) == 1);

    // Case 6: Blocked in middle, both sides safe but left has larger gap -> change left
    double gaps6[3] = {50.0, 30.0, 40.0};
    assert(planBehavior(1, 15.0, 5.0, false, gaps6) == 1);

    // Case 7: Blocked in middle, both sides safe but right has larger gap -> change right
    double gaps7[3] = {40.0, 30.0, 50.0};
    assert(planBehavior(1, 15.0, 5.0, false, gaps7) == 2);

    // Case 8: Blocked in middle, both sides safe with equal gaps -> prefer right
    double gaps8[3] = {35.0, 30.0, 35.0};
    assert(planBehavior(1, 15.0, 5.0, false, gaps8) == 2);

    // Case 9: Blocked in middle, neither adjacent lane safe -> keep lane
    double gaps9[3] = {10.0, 30.0, 15.0};
    assert(planBehavior(1, 15.0, 5.0, false, gaps9) == 0);

    // Case 10: Blocked in left lane, right lane safe due to no vehicle (-999.0) -> change right
    double gaps10[3] = {40.0, -999.0, -999.0};
    assert(planBehavior(0, 15.0, 5.0, false, gaps10) == 2);

    // Clean up dynamically allocated array from case 1 (though not needed in real code)
    // Note: In the test above, we used new double[3] but didn't store pointer; it's leaked.
    // To avoid leak, we rewrite case 1 with a local array:
    double gaps1[3] = {-999.0, -999.0, -999.0};
    assert(planBehavior(3, 10.0, 5.0, false, gaps1) == -1);

    return 0;
}

#include <vector>
#include <algorithm>

// Plan the next lane-change behavior for an ego vehicle.
// egoLane: 0 = left, 1 = middle, 2 = right.
// egoSpeed: current speed in m/s (used only to infer blocking, but not directly in decision).
// currentTime: simulation time in seconds (not used directly; recentLaneChange flag captures recency).
// recentLaneChange: true if a lane change occurred within the last 4 seconds.
// frontGap: array of size 3, frontGap[i] = distance to nearest vehicle ahead in lane i,
//           or -999.0 if no vehicle detected in that lane's forward-looking range.
// Returns: 0 = keep lane, 1 = change left, 2 = change right, -1 = invalid egoLane.
int planBehavior(int egoLane, double egoSpeed, double currentTime, bool recentLaneChange, const double frontGap[3]) {
    // Validate lane index
    if (egoLane < 0 || egoLane > 2) {
        return -1;
    }

    // If a lane change just happened recently, do not change again
    if (recentLaneChange) {
        return 0;
    }

    // Check if the ego is blocked in its current lane: a front vehicle within 60m
    bool blocked = (frontGap[egoLane] != -999.0) && (frontGap[egoLane] < 60.0);
    if (!blocked) {
        return 0;
    }

    // Determine which adjacent lanes exist
    bool leftExists = (egoLane > 0);
    bool rightExists = (egoLane < 2);

    // Safety check for a candidate lane: no vehicle within 20m ahead
    auto isLaneSafe = [&](int lane) -> bool {
        return (frontGap[lane] == -999.0) || (frontGap[lane] > 20.0);
    };

    // Collect safe adjacent lanes and their gaps
    int leftLane = egoLane - 1;
    int rightLane = egoLane + 1;
    bool leftSafe = leftExists && isLaneSafe(leftLane);
    bool rightSafe = rightExists && isLaneSafe(rightLane);

    // Edge lanes: only one possible change
    if (egoLane == 0) {
        // left lane: only right is possible
        if (rightSafe) return 2;
        return 0;
    } else if (egoLane == 2) {
        // right lane: only left is possible
        if (leftSafe) return 1;
        return 0;
    } else {
        // middle lane: consider both
        if (leftSafe && rightSafe) {
            // Choose the lane with the larger gap; if equal, prefer right
            double leftGap = (frontGap[leftLane] == -999.0) ? 1e9 : frontGap[leftLane];
            double rightGap = (frontGap[rightLane] == -999.0) ? 1e9 : frontGap[rightLane];
            if (rightGap > leftGap) {
                return 2;
            } else if (leftGap > rightGap) {
                return 1;
            } else {
                return 2; // tie -> prefer right
            }
        } else if (leftSafe) {
            return 1;
        } else if (rightSafe) {
            return 2;
        } else {
            return 0;
        }
    }
}

// The solution processes the ego lane and the three front-gap values to decide the next behavior. The main algorithm follows a priority decision tree: first validate `egoLane` (must be 0, 1, or 2; otherwise return -1). Then determine whether the ego is "blocked" in its current lane—this happens if there is a detected front vehicle within 60 meters (`frontGap[egoLane] != -999.0 && frontGap[egoLane] < 60.0`). If not blocked, or if a recent lane change occurred, return `0` (keep lane). Otherwise, evaluate the left and right adjacent lanes (if they exist) for safety: a lane is safe to enter if `frontGap[adjacentLane] == -999.0 || frontGap[adjacentLane] > 20.0`. For the middle lane, both left and right are candidates; for edge lanes, only one candidate exists. If only one adjacent lane is safe, choose it. If both are safe, pick the one with the larger gap (strictly greater; if equal, prefer right). If neither is safe, keep lane. Edge cases: `frontGap` values of -999.0 mean no vehicle, so the lane is always safe; negative or zero gap values (if they occur) are treated as unsafe because they are not -999 and not >20.0; `egoLane` out of range returns -1. Time complexity is O(1) because only a fixed number of comparisons are performed; space complexity is O(1) as only a few local variables are used. The logic mirrors the snippet's core safety checks (20m forward gap, 60m blocking threshold) but simplifies the speed-based and timing-factor logic into a simpler recent-change flag, making the task standalone and focused on lane-change safety and gap selection.

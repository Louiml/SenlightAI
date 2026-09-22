Write a C++ function named `generateSelfPassCandidates` that takes a vector of `CandidateAction` objects (representing possible self-pass/dribble actions) and a `GameState` struct (containing a game-mode string and a boolean for penalty-kick mode), and returns a new vector of `CandidateResult` objects. Each `CandidateResult` pairs a `CandidateAction` with a predicted duration (an integer) and a target point (a pair of doubles). The function must only generate candidates when the game mode is exactly "PlayOn" or when the penalty-kick flag is true, and it must ignore any non-empty "path" input (a vector of size_t indices representing already-chosen actions), returning an empty result if that path is non-empty. For each accepted candidate action, the output result should contain the original action, a duration that is the action’s duration plus 1, and a target point that is the action’s target point scaled by 2.0 in both coordinates. The input vectors may be empty, in which case the result is an empty vector. The function must not modify its inputs and must be `const`-correct.
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Include the solution declaration here (in practice, would be in a header)
// For brevity, we assume the above function is defined above this point.

int main() {
    using namespace std;

    // Sample candidates
    vector<CandidateAction> cands = {
        {5, {1.0, 2.0}},
        {3, {-1.5, 0.5}},
        {0, {0.0, 0.0}}
    };

    // Test 1: valid PlayOn, empty path -> all three results
    vector<CandidateResult> res1 = generateSelfPassCandidates(cands, "PlayOn", false, {});
    assert(res1.size() == 3);
    assert(res1[0].predictedDuration == 6);
    assert(res1[0].predictedTarget.first == 2.0);
    assert(res1[0].predictedTarget.second == 4.0);
    assert(res1[1].predictedDuration == 4);
    assert(res1[1].predictedTarget.first == -3.0);
    assert(res1[1].predictedTarget.second == 1.0);
    assert(res1[2].predictedDuration == 1);
    assert(res1[2].predictedTarget.first == 0.0);
    assert(res1[2].predictedTarget.second == 0.0);

    // Test 2: non-empty path -> empty result even with valid game mode
    vector<size_t> path = {0};
    vector<CandidateResult> res2 = generateSelfPassCandidates(cands, "PlayOn", false, path);
    assert(res2.empty());

    // Test 3: invalid game mode (neither PlayOn nor penalty) -> empty
    vector<CandidateResult> res3 = generateSelfPassCandidates(cands, "KickOff", false, {});
    assert(res3.empty());

    // Test 4: penalty kick mode accepted regardless of game mode string
    vector<CandidateResult> res4 = generateSelfPassCandidates(cands, "Whatever", true, {});
    assert(res4.size() == 3);

    // Test 5: empty candidates -> empty result (valid game mode)
    vector<CandidateAction> emptyCands;
    vector<CandidateResult> res5 = generateSelfPassCandidates(emptyCands, "PlayOn", false, {});
    assert(res5.empty());

    // Test 6: penalty kick + non-empty path -> empty
    vector<CandidateResult> res6 = generateSelfPassCandidates(cands, "Penalty", true, path);
    assert(res6.empty());

    // Test 7: invalid game mode + non-empty path -> empty (both conditions fail)
    vector<CandidateResult> res7 = generateSelfPassCandidates(cands, "Foul", false, path);
    assert(res7.empty());

    // Test 8: verify original actions are unchanged (const correctness)
    assert(cands[0].duration == 5);
    assert(cands[0].targetPoint.first == 1.0);
    assert(cands[2].duration == 0);

    return 0;
}
#include <vector>
#include <string>
#include <utility>

struct CandidateAction {
    int duration;
    std::pair<double, double> targetPoint;
};

struct CandidateResult {
    CandidateAction action;
    int predictedDuration;
    std::pair<double, double> predictedTarget;
};

// Generates self-pass candidates based on game state and path emptiness.
// Returns an empty vector if path is non-empty or game mode is not valid.
std::vector<CandidateResult> generateSelfPassCandidates(
    const std::vector<CandidateAction>& candidates,
    const std::string& gameMode,
    bool penaltyKickMode,
    const std::vector<size_t>& path)
{
    // Generate only first actions: if path is non-empty, we are not at the start.
    if (!path.empty()) {
        return {};
    }

    // Check game mode: must be "PlayOn" or penalty-kick mode.
    if (gameMode != "PlayOn" && !penaltyKickMode) {
        return {};
    }

    std::vector<CandidateResult> results;
    results.reserve(candidates.size());

    for (const auto& act : candidates) {
        // Create a simplified prediction: duration +1, target scaled by 2.
        CandidateResult r;
        r.action = act; // copy the original action
        r.predictedDuration = act.duration + 1;
        r.predictedTarget = { act.targetPoint.first * 2.0,
                              act.targetPoint.second * 2.0 };
        results.push_back(r);
    }

    return results;
}
// The solution iterates over all candidate actions, but only after verifying two preconditions: the path must be empty (meaning this is the first action in a sequence) and the game mode must be either "PlayOn" or penalty-kick mode. If either fails, return an empty vector immediately. Otherwise, for each action in the input vector, construct a `CandidateResult` with the action itself, a duration equal to `action.duration + 1` (to simulate an additional step), and a target point where both coordinates are multiplied by 2.0. The algorithm is a simple linear scan with constant work per element, so time complexity is O(n) where n is the number of candidates, and space complexity is O(n) for the output vector (excluding input storage). Edge cases are: empty input (returns empty), non-empty path (returns empty regardless of game mode), invalid game mode (returns empty), and penalty-kick mode with any game mode string (should still accept, as per the condition). The function must be `const` and take const references to avoid copies.

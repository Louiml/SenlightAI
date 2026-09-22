// Write a C++ function `bool isFullCircleComplete(const std::string& inputs, bool isLeftDirection)` that simulates the `FullCircleProcessor` logic. The input is a string of directional tokens: `"R"` (Right), `"DR"` (DownRight), `"D"` (Down), `"DL"` (DownLeft), `"L"` (Left), `"UL"` (UpLeft), `"U"` (Up), `"UR"` (UpRight). The function should track a motion progress index starting at 0. When the first token is `"R"` (or `"L"` if `isLeftDirection` is true, because the direction is mirrored), the index increments to 1. From then on, each subsequent token must match the expected next step in the canonical clockwise circle: `R → DR → D → DL → L → UL → U → UR → R` (and when `isLeftDirection` is true, the expected sequence is mirrored to `L → DL → D → DR → R → UR → U → UL → L`). Only exact sequential matches (step by step) increment the index; any mismatch resets the index back to 0. The function returns `true` only if the full circle of 8 directional steps (from index 1 through 8) is completed in sequence, meaning the index reaches 8. If the input is empty or contains any unknown token, return `false`. The function must be `const`-correct (it does not modify its inputs) and should handle the mirroring exactly as described.
#include <cassert>
#include <string>

// Declaration of the function under test
bool isFullCircleComplete(const std::string& inputs, bool isLeftDirection);

int main() {
    // Right-direction full circle
    assert(isFullCircleComplete("R DR D DL L UL U UR", false) == true);
    // Left-direction full circle (mirrored)
    assert(isFullCircleComplete("L DL D DR R UR U UL", true) == true);

    // Incomplete: only first step
    assert(isFullCircleComplete("R", false) == false);
    assert(isFullCircleComplete("L", true) == false);

    // Wrong starting direction for right
    assert(isFullCircleComplete("L DR D DL L UL U UR", false) == false);
    // Wrong starting direction for left
    assert(isFullCircleComplete("R DL D DR R UR U UL", true) == false);

    // Break in middle (resets and restarts correctly)
    // "R DR D DL L" starts right, but then L breaks at index 4 (expect DL), resets, then L is a start? 
    // Actually after reset, L is start only for left, so no progress. Then UL U UR also not start, so false.
    assert(isFullCircleComplete("R DR D DL L UL U UR", false) == false);
    // But a valid restart after break:
    // "R DR D L R DR D DL L UL U UR" — first part breaks at L, then R restarts and completes.
    assert(isFullCircleComplete("R DR D L R DR D DL L UL U UR", false) == true);

    // Empty input
    assert(isFullCircleComplete("", false) == false);
    assert(isFullCircleComplete("   ", true) == false);

    // Unknown token
    assert(isFullCircleComplete("R X D DL L UL U UR", false) == false);

    // Exact 8 steps only, no extra after completion (we return early, extra ignored)
    assert(isFullCircleComplete("R DR D DL L UL U UR R", false) == true);

    return 0;
}
#include <string>
#include <vector>

// Returns true if the token sequence completes a full circle motion,
// starting with the correct direction and following the mirror logic.
bool isFullCircleComplete(const std::string& inputs, bool isLeftDirection) {
    // Canonical clockwise sequence (right direction)
    static const std::vector<std::string> seqRight = {
        "R", "DR", "D", "DL", "L", "UL", "U", "UR"
    };
    // Mirrored sequence (left direction)
    static const std::vector<std::string> seqLeft = {
        "L", "DL", "D", "DR", "R", "UR", "U", "UL"
    };

    const std::vector<std::string>& seq = isLeftDirection ? seqLeft : seqRight;
    std::string startToken = isLeftDirection ? "L" : "R";

    // Simple tokenizer: split by space
    std::vector<std::string> tokens;
    std::string current;
    for (char c : inputs) {
        if (c == ' ') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) tokens.push_back(current);

    int progress = 0;
    for (const std::string& tok : tokens) {
        if (progress == 0) {
            // Need a fresh start token
            if (tok == startToken) {
                progress = 1;
            }
            // else stays 0
            continue;
        }

        // Check expected next token at current progress index (progress 1..8)
        // Note: seq[progress-1] is the expected token for progress index
        if (progress >= 1 && progress <= 8 && tok == seq[progress - 1]) {
            progress++;
            if (progress == 8) {
                // After incrementing from 7 to 8, last token matched
                return true;
            }
        } else {
            // Mismatch: reset, then possibly start anew with this token
            progress = 0;
            if (tok == startToken) {
                progress = 1;
            }
        }
    }
    return false;
}
// The core algorithm processes the token string sequentially, maintaining a `progress` variable (an integer from 0 to 8). The first step is special: only when the first token equals the `startToken` (`"R"` for right, `"L"` for left) does `progress` become 1; otherwise it stays 0 and no further progress is possible until that start appears. After the first successful start, for each subsequent token, we check if it equals the expected token at the current progress index. The expected sequence is predefined for both directions: for `isLeftDirection == false`, the sequence is `["R","DR","D","DL","L","UL","U","UR"]` (indices 1-8); for `isLeftDirection == true`, the mirrored sequence is `["L","DL","D","DR","R","UR","U","UL"]`. At each step, if the current token matches `seq[progress]` (where `progress` ranges 1..8), we increment `progress`. If it matches but we are at progress 8 (i.e., the last expected token `"UR"` or `"UL"` has been seen), we return `true` immediately because the circle is complete. If the token does not match the expected next token, we reset `progress` to 0 and then check if the current token could be a new start (i.e., does it equal `startToken`?). This allows overlapping patterns. Any unknown token (not one of the 8 valid) also resets to 0. Edge cases: empty input returns false; a single `"R"` (or `"L"` for left) alone is not enough because it only gets progress to 1; the sequence must be exactly 8 steps, not more or fewer, though extra steps after completion are ignored because we return as soon as index hits 8. Time complexity is O(n) where n is the number of tokens, and space complexity is O(1) besides the constant lookup tables.

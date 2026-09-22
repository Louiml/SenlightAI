// Write a C++ function that simulates the core gesture-based image navigation logic from the provided OpenCV snippet, but without any camera or GUI dependencies. The function takes a sequence of image brightness values (integers representing a grayscale or brightness measure for each of 7 images), a target brightness value (representing the user's hand/fist size), and a sequence of movement directions (strings like "LEFT", "RIGHT", "UP", "DOWN", "CENTER", or "NONE") that indicate detected hand motions. The function should process the movements in order, updating the current image index (0-based, wrapping around cyclically), the scale factor (initially 1.0, increasing by 0.5 up to 2.6 when "UP" is received, and decreasing by 0.5 down to 0.9 when "DOWN" is received), and the intensity offset (computed from the target brightness value: if the target is at least 60, compute `(target - 60) / (350 - 60) * 100`; otherwise keep the previous offset). The movements follow a state-machine rule: a direction other than "CENTER" is only processed if the previous movement was "CENTER" (i.e., the hand must pass through the center zone first); a "CENTER" movement only sets a flag to allow the next non-center movement. The function returns a struct containing the final current index, scale, and intensity offset. The function must handle wrapper indexing and enforce scale bounds.
The solution requires maintaining three state variables: `currentImg` (integer index into 0..6), `scale` (float), `intensity` (float), and a boolean flag `passByCenter`. For each movement direction in the input vector: if the direction is "CENTER", set `passByCenter = true`. Otherwise, if `passByCenter` is true, process the movement: "LEFT" decrements the index (wrapping to 6 if below 0), "RIGHT" increments (wrapping to 0 if above 6), "UP" increases scale by 0.5 but caps at 2.6, "DOWN" decreases scale by 0.5 but floors at 0.9; after processing any non-center movement, reset `passByCenter = false`. For intensity: whenever the target brightness value is >= 60, update intensity to `(target - 60) / 290 * 100`, but if target < 60, keep the current intensity value (do not reset to 0). The initial intensity is 0. The order of operations matters: the movement processing for the current step should use the state from before that step, and intensity is updated after movement processing but before moving to the next step. Edge cases: an empty movement sequence returns initial state; consecutive non-center movements without a preceding center are ignored; multiple centers in a row just keep the flag true. Time complexity is O(M) where M is the number of movements; space complexity O(1) storing only state variables and returning a struct.
#include <vector>
#include <string>
#include <algorithm>

// Struct to hold final state of the image navigation simulation.
struct NavigationResult {
    int currentIndex;
    float scale;
    float intensity;
};

// Simulates gesture-driven image navigation without camera or GUI.
// Movements: "LEFT", "RIGHT", "UP", "DOWN", "CENTER", otherwise ignored (e.g., "NONE").
// The targetBrightness is used to update intensity when >= 60.
NavigationResult simulateNavigation(const std::vector<std::string>& movements, int targetBrightness) {
    const int imageCount = 7;
    const float minScale = 0.9f;
    const float maxScale = 2.6f;
    const float scaleStep = 0.5f;
    const float fistSizeMin = 60.0f;
    const float fistSizeMax = 350.0f;

    int currentIndex = 0;
    float scale = 1.0f;
    float intensity = 0.0f;
    bool passByCenter = false;

    for (const auto& movement : movements) {
        if (movement == "CENTER") {
            passByCenter = true;
        } else if (passByCenter) {
            if (movement == "LEFT") {
                currentIndex = (currentIndex == 0) ? imageCount - 1 : currentIndex - 1;
            } else if (movement == "RIGHT") {
                currentIndex = (currentIndex == imageCount - 1) ? 0 : currentIndex + 1;
            } else if (movement == "UP") {
                scale = std::min(maxScale, scale + scaleStep);
            } else if (movement == "DOWN") {
                scale = std::max(minScale, scale - scaleStep);
            }
            // Any non-center movement resets the flag (even if direction is unrecognized like "NONE").
            passByCenter = false;
        }

        // Update intensity based on target brightness (only when >= fistSizeMin).
        if (targetBrightness >= static_cast<int>(fistSizeMin)) {
            intensity = (static_cast<float>(targetBrightness) - fistSizeMin) / (fistSizeMax - fistSizeMin) * 100.0f;
        }
        // If targetBrightness < fistSizeMin, intensity remains unchanged.
    }

    return {currentIndex, scale, intensity};
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or link it).

int main() {
    // Empty movements: initial state.
    NavigationResult r0 = simulateNavigation({}, 100);
    assert(r0.currentIndex == 0);
    assert(r0.scale == 1.0f);
    assert(r0.intensity == 0.0f);

    // Basic navigation with center-passing rule.
    NavigationResult r1 = simulateNavigation({"CENTER", "RIGHT"}, 100);
    assert(r1.currentIndex == 1);
    assert(r1.scale == 1.0f);
    assert(r1.intensity == 40.0f / 290.0f * 100.0f); // (100-60)/(350-60)*100

    // Non-center without preceding center is ignored.
    NavigationResult r2 = simulateNavigation({"LEFT", "CENTER", "LEFT"}, 100);
    assert(r2.currentIndex == 6);  // Only the second LEFT counts after CENTER.

    // Wrapping: from 0 LEFT goes to 6, then RIGHT comes back to 0.
    NavigationResult r3 = simulateNavigation({"CENTER", "LEFT", "CENTER", "RIGHT"}, 100);
    assert(r3.currentIndex == 0);

    // Scale bounds: UP beyond max caps at 2.6, DOWN below min floors at 0.9.
    NavigationResult r4 = simulateNavigation({"CENTER", "UP", "UP", "UP", "UP", "UP", "CENTER", "DOWN", "DOWN", "DOWN", "DOWN", "DOWN", "DOWN"}, 60);
    assert(r4.scale == 2.6f - 0.5f); // Five UPs reach 2.6, then six DOWNs go to 0.9? Let's compute: start 1.0, 5 UPs -> 3.5 capped 2.6, then 3 DOWNs from 2.6 -> 1.1 (since floor 0.9 after 4th DOWN). Actually careful: after 5 UPs scale=2.6, then 1st DOWN=2.1, 2nd=1.6, 3rd=1.1, 4th=0.9, 5th stays 0.9, 6th stays 0.9. So final 0.9.
    assert(r4.scale == 0.9f);
    assert(r4.intensity == 0.0f); // target = 60, so (60-60)/290*100 = 0.

    // Intensity update only when target >= 60; below keeps previous.
    NavigationResult r5 = simulateNavigation({"CENTER", "UP"}, 50);
    assert(r5.intensity == 0.0f); // target < 60, stays 0.
    NavigationResult r6 = simulateNavigation({"CENTER", "RIGHT"}, 200);
    assert(r6.intensity == (200.0f - 60.0f) / (350.0f - 60.0f) * 100.0f);

    // Multiple centers and unrecognized movements.
    NavigationResult r7 = simulateNavigation({"CENTER", "NONE", "CENTER", "LEFT"}, 100);
    assert(r7.currentIndex == 6); // NONE after CENTER resets flag, then next CENTER sets flag again, LEFT works.
    assert(r7.scale == 1.0f);
    assert(r7.intensity == 40.0f / 290.0f * 100.0f);

    return 0;
}

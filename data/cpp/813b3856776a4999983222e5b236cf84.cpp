// Write a C++ function that simulates the encounter-state tracking logic of the Old Hillsbrad instance script. The function should accept a series of event commands (encoded as integers) and maintain an array of six encounter states. The commands are: 0 = barrel diversion progress with a barrel count as the next argument, 1 = thrall event outcome (0 = not started, 1 = in progress, 2 = done, 3 = fail), 2/3/4/5 = thrall parts 1-4 outcomes (same encoding). For barrel diversion commands, increment the barrel count (starting at 0); if the count reaches 5, set encounter[0] to done (2) and also trigger a quest credit (simply record that a credit happened). For thrall event command with outcome 3 (fail), if the current fail count is ≤ 20, increment the fail count and reset encounters[1] through [5] to 0 (not started); if fail count exceeds 20, set encounters[1]-[5] to 3. For any other outcome, set the corresponding encounter state. Additionally, support a query command (code 10) that returns the current barrel count; code 11 returns the fail count; codes 20-25 return encounter states 0-5 respectively; code 30 returns whether a quest credit was triggered (1) or not (0). The function should process a sequence of commands (represented as a `std::vector<int>` where command and its argument are interleaved, e.g., "0 3" means a barrel diversion with count 3) and return a `std::string` with the final state formatted as: "barrelCount:failCount:questCredit:enc0:enc1:enc2:enc3:enc4:enc5". Assume all inputs are valid (commands will always have an argument when needed, and command codes are limited to the set above). The function must be `const`-correct and should not modify the input vector.

// The solution simulates an instance state machine with a fixed-size array of 6 encounter states, a barrel counter, a fail counter, and a quest-credit flag. The main algorithm iterates over the command vector, processing pairs of (command, argument). For each command, it updates the corresponding state or counter. The key edge case is the thrall failure logic: when a fail occurs, we must check the fail count before incrementing it. If the count is ≤ 20, we increment and reset encounters 1-5 to not started; if it is already > 20, we set all to fail. Importantly, the condition uses the current count before incrementing, so the 21st failure triggers the ">20" branch (since after incrementing the 21st time, the count becomes 21). The barrel diversion only moves to done when the barrel count reaches exactly 5; further barrel commands should not change it from done, but the code as given simply increments the count; we mirror that behavior but note that encounter[0] becomes 2 only when count == 5. For simplicity, we do not cap the count; if more barrel commands come after reaching 5, the count keeps growing but encounter[0] stays done. The quest credit is set when barrel count first reaches 5; we use a boolean flag. Time complexity is O(n) where n is the number of commands, and space is O(1) beyond the input vector.

#include <vector>
#include <string>
#include <sstream>
#include <cstring>

// Simulates old-hillsbrad instance state tracking.
// Commands are encoded as pairs: (commandCode, argument).
// Returns a formatted string with final state.
std::string simulateInstanceCommands(const std::vector<int>& commands) {
    int encounters[6] = {0, 0, 0, 0, 0, 0};
    int barrelCount = 0;
    int failCount = 0;
    bool questCredit = false;

    for (size_t i = 0; i < commands.size(); i += 2) {
        int command = commands[i];
        int arg = commands[i + 1];

        switch (command) {
            case 0: { // barrel diversion
                barrelCount += arg;
                if (barrelCount >= 5) {
                    encounters[0] = 2; // done
                    questCredit = true;
                } else {
                    encounters[0] = 1; // in progress
                }
                break;
            }
            case 1: { // thrall event
                if (arg == 3) { // fail
                    if (failCount <= 20) {
                        failCount++;
                        encounters[1] = 0;
                        encounters[2] = 0;
                        encounters[3] = 0;
                        encounters[4] = 0;
                        encounters[5] = 0;
                    } else {
                        encounters[1] = 3;
                        encounters[2] = 3;
                        encounters[3] = 3;
                        encounters[4] = 3;
                        encounters[5] = 3;
                    }
                } else {
                    encounters[1] = arg;
                }
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5: {
                encounters[command] = arg;
                break;
            }
            default:
                break; // ignore unknown commands
        }
    }

    std::ostringstream oss;
    oss << barrelCount << ":" << failCount << ":" << (questCredit ? 1 : 0) << ":"
        << encounters[0] << ":" << encounters[1] << ":" << encounters[2] << ":"
        << encounters[3] << ":" << encounters[4] << ":" << encounters[5];
    return oss.str();
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test basic barrel diversion reaching 5
    std::vector<int> cmd1 = {0, 2, 0, 3};
    assert(simulateInstanceCommands(cmd1) == "5:0:1:2:0:0:0:0:0");

    // Test thrall fail with reset (few failures)
    std::vector<int> cmd2 = {1, 3, 2, 1, 3, 1, 1, 1};
    // After first fail: failCount=1, all part states reset to 0, then part1 set to 1
    // After second fail: failCount=2, all part states reset again to 0, then part1 set to 1
    assert(simulateInstanceCommands(cmd2) == "0:2:0:0:0:1:0:0:0");

    // Test thrall fail beyond 20 (21 failures)
    std::vector<int> cmd3;
    for (int i = 0; i < 21; i++) {
        cmd3.push_back(1);
        cmd3.push_back(3);
    }
    // After 21 failures, failCount=21, all encounters 1-5 set to 3
    assert(simulateInstanceCommands(cmd3) == "0:21:0:0:3:3:3:3:3");

    // Test mixed: barrel then thrall success
    std::vector<int> cmd4 = {0, 1, 0, 4, 1, 2, 2, 2};
    // barrel count 5, quest credit, encounter[0]=2, thrall event done, part1 done
    assert(simulateInstanceCommands(cmd4) == "5:0:1:2:2:2:0:0:0");

    // Test no commands
    std::vector<int> cmd5;
    assert(simulateInstanceCommands(cmd5) == "0:0:0:0:0:0:0:0:0");

    // Test barrel doesn't go over when exact 5 reached then extra
    std::vector<int> cmd6 = {0, 5, 0, 2};
    // barrel count becomes 7 but encounter[0] stays done, quest credit already set
    assert(simulateInstanceCommands(cmd6) == "7:0:1:2:0:0:0:0:0");

    // Test part states set individually
    std::vector<int> cmd7 = {2, 1, 3, 2, 4, 1, 5, 2};
    assert(simulateInstanceCommands(cmd7) == "0:0:0:0:0:1:2:1:2");

    // Test thrall success does not reset parts
    std::vector<int> cmd8 = {1, 1, 2, 1, 1, 3, 3, 2};
    // thrall in progress, part1 in progress, then thrall fail (failCount=1, reset parts), then part3 done
    assert(simulateInstanceCommands(cmd8) == "0:1:0:0:0:0:0:2:0");

    // Test barrel with single increment then partial
    std::vector<int> cmd9 = {0, 1, 0, 1, 0, 1, 0, 1};
    assert(simulateInstanceCommands(cmd9) == "4:0:0:1:0:0:0:0:0");

    // Test exactly 5 via multiple increments
    std::vector<int> cmd10 = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    assert(simulateInstanceCommands(cmd10) == "5:0:1:2:0:0:0:0:0");

    return 0;
}

/*
Write a standalone C++ function named `maximumPressureRelease` that, given a description of a network of pressure-release valves (with travel times between adjacent valves, a starting valve `"AA"`, a distinct pressure value per valve, and the ability to open a valve in one minute to gain its pressure per minute for all remaining time), computes the maximum total pressure that can be released. The input will be provided as a `std::istream` containing a sequence of valve records, one per line, formatted as in the classic Advent of Code 2022 Day 16 puzzle: each line starts with `Valve XX has flow rate=N; tunnels lead to valves AA, BB` (singular/plural variants allowed). The function must return an `int` representing the optimal total pressure released given a time limit of 30 minutes and only one agent starting at `AA`. The network is small (≤ 50 valves), but the solution must handle any valid input structure, including valves with zero flow that can be compressed away for efficiency. The function should be deterministic and handle cases where some valves are unreachable, multiple zero-flow valves, and the starting valve `AA` potentially having zero flow. Assume the input is well-formed and complete; do not modify the input stream. Provide the solution as a single free function with the signature `int maximumPressureRelease(std::istream& input)`, implementing any necessary auxiliary structures.
*/
#include <bits/stdc++.h>

// Core structure representing a valve.
struct Valve {
    std::string name;
    int pressure = 0;
    std::vector<std::string> neighbors;
    size_t idx = 0;
};

// The full network and compressed graph.
struct ValveNetwork {
    // Original mapping from name to valve.
    std::unordered_map<std::string, Valve> raw;
    // After compression: only positive-pressure valves and "AA".
    std::unordered_map<std::string, Valve> compressed;
    // Distance matrix after Floyd-Warshall on compressed graph.
    std::unordered_map<std::string, std::unordered_map<std::string, int>> dist;
    // Number of compressed valves.
    size_t n = 0;
};

// Parsing a single valve record from a line like:
// Valve AA has flow rate=0; tunnels lead to valves BB, CC
std::istream& operator>>(std::istream& is, Valve& v) {
    if (!is.good()) return is;
    // "Valve "
    std::string tmp;
    is >> tmp; // "Valve"
    is >> v.name;
    is >> tmp; // "has"
    is >> tmp; // "flow"
    is >> tmp; // "rate="
    // Now read the integer after '='. The '=' is part of tmp? Actually we read tmp as "rate=", then need int.
    // Simpler: skip until we find '=' using chars.
    // But since input format is fixed, we can do:
    // Actually our parsing will use getline for each line, so this operator is not used directly.
    // We'll implement a manual parsing in buildNetwork.
    return is;
}

// Build the network from an input stream containing one valve per line.
ValveNetwork buildNetwork(std::istream& input) {
    ValveNetwork network;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string token;
        iss >> token; // "Valve"
        Valve v;
        iss >> v.name;
        iss >> token; // "has"
        iss >> token; // "flow"
        iss >> token; // "rate=5;" - token contains "rate=5;"
        // Extract the integer after '=' and before ';'
        size_t eqPos = token.find('=');
        size_t semiPos = token.find(';');
        std::string numStr = token.substr(eqPos + 1, semiPos - eqPos - 1);
        v.pressure = std::stoi(numStr);
        // Read "tunnels lead to valves" or "tunnel leads to valve"
        iss >> token; // "tunnels" or "tunnel"
        iss >> token; // "lead" or "leads"
        iss >> token; // "to"
        iss >> token; // "valves" or "valve"
        // Now read remaining comma-separated neighbor names.
        std::string neighbor;
        while (std::getline(iss, neighbor, ',')) {
            // Remove leading space.
            size_t start = neighbor.find_first_not_of(' ');
            if (start != std::string::npos) {
                neighbor = neighbor.substr(start);
            }
            // Remove any trailing whitespace.
            size_t end = neighbor.find_last_not_of(' ');
            if (end != std::string::npos) {
                neighbor = neighbor.substr(0, end + 1);
            }
            if (!neighbor.empty()) {
                v.neighbors.push_back(neighbor);
            }
            // The getline with comma will stop after comma; but the last one has no comma, so it will read to end.
        }
        // The above loop actually reads until end of line, splitting on commas. That's fine.
        // But we need to handle the case where there's one neighbor only.
        // Actually the getline approach works: first call gets " BB", next " CC", etc.
        // For last, it gets " DD" and then no comma, but getline still reads to end.
        network.raw[v.name] = v;
    }
    return network;
}

// Compress the graph: remove all zero-pressure valves except "AA".
void compressGraph(ValveNetwork& network) {
    // Build adjacency for raw graph.
    std::unordered_map<std::string, std::unordered_map<std::string, int>> rawDist;
    for (const auto& [name, v] : network.raw) {
        rawDist[name][name] = 0;
        for (const auto& n : v.neighbors) {
            rawDist[name][n] = 1;
            rawDist[n][name] = 1;
        }
    }

    // Add all names to rawDist for completeness.
    for (const auto& [name, v] : network.raw) {
        for (const auto& [name2, v2] : network.raw) {
            if (rawDist[name].find(name2) == rawDist[name].end()) {
                rawDist[name][name2] = 1000; // large number
            }
        }
    }

    // Floyd-Warshall on raw graph.
    for (const auto& [k, vk] : network.raw) {
        for (const auto& [i, vi] : network.raw) {
            for (const auto& [j, vj] : network.raw) {
                int dik = rawDist[i][k];
                int dkj = rawDist[k][j];
                if (dik + dkj < rawDist[i][j]) {
                    rawDist[i][j] = dik + dkj;
                }
            }
        }
    }

    // Now build compressed map: only keep "AA" and positive pressure valves.
    for (const auto& [name, v] : network.raw) {
        if (name == "AA" || v.pressure > 0) {
            network.compressed[name] = v;
        }
    }

    // Assign indices.
    size_t idx = 0;
    for (auto& [name, v] : network.compressed) {
        v.idx = idx++;
    }
    network.n = idx;

    // Build distance map among compressed valves using raw shortest paths.
    for (const auto& [a, va] : network.compressed) {
        for (const auto& [b, vb] : network.compressed) {
            int d = rawDist[a][b];
            if (d >= 1000) d = 1000; // treat as unreachable
            network.dist[a][b] = d;
        }
    }
}

// Memoization for DFS.
struct State {
    std::string loc;
    std::bitset<64> open; // using 64 bits, but n <= ~16
    int minutesLeft;
    bool operator==(const State& o) const {
        return loc == o.loc && open == o.open && minutesLeft == o.minutesLeft;
    }
    struct Hash {
        size_t operator()(const State& s) const {
            size_t h = std::hash<std::string>{}(s.loc);
            h ^= std::hash<unsigned long long>{}(s.open.to_ullong());
            h ^= std::hash<int>{}(s.minutesLeft);
            return h;
        }
    };
};

int dfs(const ValveNetwork& net, State state, std::unordered_map<State, int, State::Hash>& memo) {
    auto it = memo.find(state);
    if (it != memo.end()) return it->second;

    int best = 0;

    // If no time left or all positive valves opened, return 0.
    if (state.minutesLeft <= 0) {
        memo[state] = 0;
        return 0;
    }

    const auto& curValve = net.compressed.at(state.loc);
    bool allOpened = true;
    for (const auto& [name, v] : net.compressed) {
        if (v.pressure > 0 && !state.open[v.idx]) {
            allOpened = false;
            break;
        }
    }
    if (allOpened) {
        memo[state] = 0;
        return 0;
    }

    // Option: open current valve if not opened and has positive pressure.
    if (curValve.pressure > 0 && !state.open[curValve.idx]) {
        State newState = state;
        newState.minutesLeft -= 1;
        newState.open[curValve.idx] = true;
        int gain = curValve.pressure * newState.minutesLeft;
        best = std::max(best, gain + dfs(net, newState, memo));
    }

    // Option: move to another positive-pressure valve that is not opened.
    for (const auto& [name, v] : net.compressed) {
        if (name == state.loc) continue;
        if (v.pressure == 0) continue; // only go to interesting ones (but AA may be zero, though we keep it; but AA pressure is 0, skip)
        if (state.open[v.idx]) continue;
        int travel = net.dist.at(state.loc).at(name);
        if (travel >= state.minutesLeft) continue; // cannot reach and open in time? Actually if travel > minutesLeft, skip; if == minutesLeft, you arrive at exactly time 0, no opening possible, so skip.
        State newState = state;
        newState.minutesLeft -= travel;
        newState.loc = name;
        best = std::max(best, dfs(net, newState, memo));
    }

    memo[state] = best;
    return best;
}

// Main solution function.
int maximumPressureRelease(std::istream& input) {
    ValveNetwork net = buildNetwork(input);
    compressGraph(net);
    // If no compressed valves other than AA? AA might be alone.
    // Start at "AA"
    State start{"AA", std::bitset<64>(), 30};
    std::unordered_map<State, int, State::Hash> memo;
    return dfs(net, start, memo);
}
#include <bits/stdc++.h>

// Assume the solution function is declared above in the same translation unit.
// We'll test with small hand-crafted inputs.

int main() {
    // Test 1: Trivial: only AA, no other valves, no pressure.
    {
        std::istringstream input("Valve AA has flow rate=0; tunnels lead to valves\n");
        assert(maximumPressureRelease(input) == 0);
    }

    // Test 2: Single valve with pressure, directly connected to AA.
    // AA has 0 flow, one neighbor with flow 10. Distance 1.
    // Starting at AA with 30 min. Go to that valve takes 1 min, open takes 1 min -> 28 min left -> 10*28=280.
    {
        std::istringstream input(
            "Valve AA has flow rate=0; tunnels lead to valves BB\n"
            "Valve BB has flow rate=10; tunnels lead to valves AA\n");
        assert(maximumPressureRelease(input) == 280);
    }

    // Test 3: Two valves, both positive, connected in a line: AA - B - C.
    // B pressure 5, C pressure 20. AA to B distance 1, B to C distance 1.
    // Optimal: go to B (1), open B (1) -> 28 left, gain 5*28=140. Then go to C (1), open C (1) -> 26 left, gain 20*26=520. Total 660.
    {
        std::istringstream input(
            "Valve AA has flow rate=0; tunnels lead to valves BB\n"
            "Valve BB has flow rate=5; tunnels lead to valves AA, CC\n"
            "Valve CC has flow rate=20; tunnels lead to valves BB\n");
        assert(maximumPressureRelease(input) == 660);
    }

    // Test 4: Two independent branches from AA: branch to B (dist 1) and branch to C (dist 1).
    // B pressure 10, C pressure 10. With 30 minutes, visiting both is possible.
    // Strategy: open B (1+1=2 min) gain 10*28=280, open C (1+1=2) gain 10*26=260 total 540.
    // Or open C first, same. So answer 540.
    {
        std::istringstream input(
            "Valve AA has flow rate=0; tunnels lead to valves BB, CC\n"
            "Valve BB has flow rate=10; tunnels lead to valves AA\n"
            "Valve CC has flow rate=10; tunnels lead to valves AA\n");
        assert(maximumPressureRelease(input) == 540);
    }

    // Test 5: Unreachable positive valve.
    // AA connected to BB (flow 5), but CC (flow 100) is isolated (no connection). CC unreachable.
    // Should only open BB: 5 * (30-2) = 5*28=140.
    {
        std::istringstream input(
            "Valve AA has flow rate=0; tunnels lead to valves BB\n"
            "Valve BB has flow rate=5; tunnels lead to valves AA\n"
            "Valve CC has flow rate=100; tunnels lead to valves\n");
        assert(maximumPressureRelease(input) == 140);
    }

    // Test 6: Zero-pressure valve in the middle acts as a tunnel.
    // AA - Z (0) - B (10). Distance from AA to B is 2.
    // Starting AA, travel to B takes 2 min, open takes 1 -> 27 min left -> 10*27=270.
    // Also can't gain from Z. So answer 270.
    {
        std::istringstream input(
            "Valve AA has flow rate=0; tunnels lead to valves ZZ\n"
            "Valve ZZ has flow rate=0; tunnels lead to valves AA, BB\n"
            "Valve BB has flow rate=10; tunnels lead to valves ZZ\n");
        assert(maximumPressureRelease(input) == 270);
    }

    // Test 7: All valves already opened? Actually only one positive valve, open it immediately.
    // AA connected to B (flow 1). Distance 1. Open at minute 2 -> 28 left -> 28.
    {
        std::istringstream input(
            "Valve AA has flow rate=0; tunnels lead to valves BB\n"
            "Valve BB has flow rate=1; tunnels lead to valves AA\n");
        assert(maximumPressureRelease(input) == 28);
    }

    // Test 8: Multiple lines with trailing spaces and singular "tunnel leads to valve".
    {
        std::istringstream input(
            "Valve AA has flow rate=0; tunnel leads to valve BB\n"
            "Valve BB has flow rate=7; tunnel leads to valve AA\n");
        // distance 1, open at minute 2 -> 7*28=196
        assert(maximumPressureRelease(input) == 196);
    }

    // Test 9: Large graph with 3 positive valves in a star shape.
    // AA connected to A, B, C each with distance 1. Pressures A=3, B=4, C=5.
    // Best order: open highest pressure first? Actually compute all.
    // Option 1: open C first: go 1, open 1 -> 28 left, gain 5*28=140. Then B: go 1 (now at AA? Actually from C to AA then to B takes 2) Wait, star: A,B,C all connected to AA, not to each other. So from C to B need to go back to AA (dist 1) then to B (dist 1) total 2. So after opening C at minute 2, travel to B takes 2 min -> minute 4, open at 5 -> 25 left, gain 4*25=100. Then A: travel 2, open at 8 -> 22 left, gain 3*22=66. Total 306.
    // Option 2: open B first: 4*28=112, then C: travel 2+1=3 -> 4+3=7? Actually let's compute: Start minute 0, go to B (1), open at 2 -> 28 left, gain 4*28=112. From B to C: B->AA (1), AA->C (1) total 2, arrive minute 4, open at 5 -> 25 left, gain 5*25=125. Then A: from C to A travel 2, arrive minute 7, open at 8 -> 22 left, gain 3*22=66. Total 303.
    // Option 3: open A first: 3*28=84, then C: travel 2, open at 5 -> 25 left, gain 5*25=125, then B: travel 2, open at 8 -> 22 left, gain 4*22=88 total 297.
    // Best is 306. Let's test.
    {
        std::istringstream input(
            "Valve AA has flow rate=0; tunnels lead to valves A, B, C\n"
            "Valve A has flow rate=3; tunnels lead to valves AA\n"
            "Valve B has flow rate=4; tunnels lead to valves AA\n"
            "Valve C has flow rate=5; tunnels lead to valves AA\n");
        assert(maximumPressureRelease(input) == 306);
    }

    // Test 10: All positive valves already open? Not possible. But test scenario where AA itself has pressure.
    // AA has flow 5 and connected to itself? Actually typically AA not connected to itself. But we can have AA positive.
    // AA with flow 5, and other valve B with flow 2 connected to AA.
    // Starting at AA, open AA (1 min) -> 29 left, gain 5*29=145. Then go to B (1), open (1) -> 27 left, gain 2*27=54 total 199.
    // Without opening AA, go to B: travel 1, open at 2 -> 28 left, gain 2*28=56, then come back AA? But open AA later: travel 1, open at 4 -> 26 left, gain 5*26=130 total 186. So better to open AA first. So answer 199.
    {
        std::istringstream input(
            "Valve AA has flow rate=5; tunnels lead to valves BB\n"
            "Valve BB has flow rate=2; tunnels lead to valves AA\n");
        assert(maximumPressureRelease(input) == 199);
    }

    return 0;
}
// The core problem is a variant of the traveling salesman problem with rewards, where the reward for visiting a valve is its pressure multiplied by the remaining time after opening it, and travel consumes time. Since the graph is small (at most ~15 nonzero-flow valves after compression), we can model the search as a depth-first traversal with memoization over the current location, the set of already opened valves (as a bitset), and the remaining time. The main algorithmic steps:
//
// 1. **Parsing**: Read each valve record from the input stream, extract the two-letter name, the flow rate (pressure), and the list of adjacent valve names. Store these in a mapping from name to a `Valve` structure containing the pressure, a list of neighbors with distance 1, and an index for the bitset.
//
// 2. **Compression**: Remove all valves with zero pressure except the starting valve `AA`, because they provide no reward and only add travel time. For each removed zero-flow valve, connect all its neighbors to each other with distances equal to the sum of the two path segments, effectively shortcutting through the removed valve. This reduces the graph to only "interesting" valves, making the search feasible.
//
// 3. **All-pairs shortest paths**: After compression, compute the shortest travel time between every pair of remaining valves using Floyd-Warshall. This gives a distance map that the search will use to decide which valve to visit next and how much time it costs.
//
// 4. **Depth-first search with memoization**: Define a state as `{current location, bitset of opened valves, minutes remaining}`. Starting at `AA` with 30 minutes, recursively explore two types of actions at each step:
//    - If the current valve is not yet opened and has positive pressure, open it (spend 1 minute, gain `pressure * (remaining-1)` reward, mark it opened).
//    - Move to any other unopened valve with positive pressure, spending the shortest travel time `dist` minutes (but no opening cost yet). If the travel time exceeds remaining minutes, skip that move.
//    
//    The recursion terminates when no more moves are possible (time ≤ 0, or all interesting valves opened). Memoize results to avoid recomputation, since the number of states is bounded by `O(V * 2^P * T)` where `V` is number of valves after compression (≤ 16), `P` is number of positive-pressure valves (≤ 15), and `T` is max minutes (30). This is efficient enough.
//
// 5. **Edge cases**: 
//    - The starting valve `AA` may have zero flow; compression keeps it. 
//    - Some positive-pressure valves may be unreachable from `AA`; the distance matrix will have infinity for them, and the search will simply not visit them. 
//    - If there are no positive-pressure valves, the answer is 0. 
//    - The time limit is exactly 30 minutes; opening a valve at minute `m` means it contributes for `30 - m` minutes.
//
// 6. **Complexity**: After compression, let `P` be the number of valves with positive flow (plus possibly `AA`). The number of states is at most `P * 2^P * 30`, which for `P ≤ 15` is about 15 * 32768 * 30 ≈ 14.7 million, manageable. Each state transition iterates over all other valves (≤ 15). Total time is `O(P^2 * 2^P * T)`, which is fine. Space is `O(P * 2^P * T)` for memoization.

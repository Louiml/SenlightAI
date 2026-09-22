/*
You are given a configuration system that reads physical parameters from a session. Write a standalone C++ function `loadFreestreamParameters` that takes: a `std::map<std::string, double>` representing session parameters, an integer `spaceDim` (1, 2, or 3), and a double reference `gamma`. The function must load the following parameters with specified defaults: `gamma` (default 1.4), `rhoInf` (default 1.225), `pInf` (default 101325), `uInf` (default 0.1), `vInf` (default 0.0, only loaded when spaceDim >= 2), and `wInf` (default 0.0, only loaded when spaceDim == 3). The function should return a struct `FreestreamParams` containing: `gamma`, `rhoInf`, `pInf`, and a `std::vector<double> velInf` of size `spaceDim` holding the velocity components in order (u, v, w) with the appropriate defaults. Missing keys must use defaults; present keys must override defaults. The function must be const-correct and not modify the input map.
*/
#include <map>
#include <vector>
#include <string>
#include <algorithm>

struct FreestreamParams {
    double gamma;
    double rhoInf;
    double pInf;
    std::vector<double> velInf;
};

// Load freestream parameters from a session map with defaults.
FreestreamParams loadFreestreamParameters(
    const std::map<std::string, double>& session,
    int spaceDim,
    double& gamma) {
    
    // Helper to get value with default if key missing
    auto getParam = [&session](const std::string& key, double defaultValue) {
        auto it = session.find(key);
        return (it != session.end()) ? it->second : defaultValue;
    };

    // Load scalar parameters
    gamma = getParam("Gamma", 1.4);
    double rhoInf = getParam("rhoInf", 1.225);
    double pInf = getParam("pInf", 101325);

    // Build velocity vector based on space dimension
    std::vector<double> velInf(spaceDim, 0.0);
    if (spaceDim >= 1) {
        velInf[0] = getParam("uInf", 0.1);
    }
    if (spaceDim >= 2) {
        velInf[1] = getParam("vInf", 0.0);
    }
    if (spaceDim == 3) {
        velInf[2] = getParam("wInf", 0.0);
    }

    return FreestreamParams{gamma, rhoInf, pInf, velInf};
}
#include <cassert>
#include <map>
#include <vector>
#include <string>

// Include the solution code here or via header

int main() {
    // Test 1: All defaults, 1D
    {
        std::map<std::string, double> session; // empty
        double gamma = 0.0;
        FreestreamParams p = loadFreestreamParameters(session, 1, gamma);
        assert(gamma == 1.4);
        assert(p.gamma == 1.4);
        assert(p.rhoInf == 1.225);
        assert(p.pInf == 101325);
        assert(p.velInf.size() == 1);
        assert(p.velInf[0] == 0.1);
    }

    // Test 2: Custom values, 2D
    {
        std::map<std::string, double> session = {
            {"Gamma", 1.3},
            {"rhoInf", 0.5},
            {"uInf", 2.0},
            {"vInf", -3.5}
        };
        double gamma = 0.0;
        FreestreamParams p = loadFreestreamParameters(session, 2, gamma);
        assert(gamma == 1.3);
        assert(p.pInf == 101325); // default
        assert(p.velInf.size() == 2);
        assert(p.velInf[0] == 2.0);
        assert(p.velInf[1] == -3.5);
    }

    // Test 3: 3D with wInf specified, pInf overridden
    {
        std::map<std::string, double> session = {
            {"pInf", 80000.0},
            {"wInf", 1.5}
        };
        double gamma = 0.0;
        FreestreamParams p = loadFreestreamParameters(session, 3, gamma);
        assert(gamma == 1.4); // default
        assert(p.rhoInf == 1.225);
        assert(p.pInf == 80000.0);
        assert(p.velInf.size() == 3);
        assert(p.velInf[0] == 0.1); // default
        assert(p.velInf[1] == 0.0); // default
        assert(p.velInf[2] == 1.5);
    }

    // Test 4: vInf should not be present when spaceDim=1, wInf ignored
    {
        std::map<std::string, double> session = {
            {"vInf", 9.9},
            {"wInf", -1.0}
        };
        double gamma = 2.0;
        FreestreamParams p = loadFreestreamParameters(session, 1, gamma);
        assert(gamma == 2.0);
        assert(p.velInf.size() == 1);
        assert(p.velInf[0] == 0.1);
    }

    return 0;
}
// The solution involves iterating over a fixed set of parameter names with their corresponding defaults. For each parameter, check if the key exists in the map; if yes, use the stored value, otherwise use the default. For velocity, handle each component conditionally based on `spaceDim`: always read `uInf`, read `vInf` only when spaceDim >= 2, and read `wInf` only when spaceDim == 3. The `velInf` vector is resized to `spaceDim` and filled in order. Important edge cases: `spaceDim` must be 1, 2, or 3 (we can assert or assume valid input; if invalid, we could default to 1 for safety). Missing keys are common and defaults must be applied. Duplicate keys are not possible in a `std::map`. Time complexity is O(1) (constant number of lookups) and space complexity is O(spaceDim) for the velocity vector.

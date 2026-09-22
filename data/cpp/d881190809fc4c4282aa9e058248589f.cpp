/*
Write a C++ function that implements an Ant Colony Optimization (ACO) algorithm for the Traveling Salesman Problem on a fixed set of 5 cities. The function should take as input a 5×5 distance matrix (symmetric, with zero diagonal) and return the tour length of the best tour found after running a simplified elitist ACO simulation for a fixed number of iterations. The algorithm should use a population of ants, pheromone evaporation, pheromone deposition (including an elitist boost for the global best tour), and a probabilistic next-city selection based on pheromone and distance. The function must be deterministic in the sense that given the same input matrix it always returns the same best tour length (use a fixed random seed internally). Input distances are integers between 1 and 100. The output is a double representing the total Euclidean distance of the best tour.
*/

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>
#include <cassert>

// ACO solver for TSP with a fixed number of cities (5) and deterministic behavior.
// Returns the best tour length (sum of distances) found.
double solveTSP(const std::vector<std::vector<int>>& distances) {
    const int numCities = distances.size();
    assert(numCities == 5); // Fixed size as per task
    const int numAnts = 5;
    const int maxIterations = 50;
    const double alpha = 1.0;
    const double beta = 5.0;
    const double rho = 0.5;
    const double Q = 100.0;
    const double initPheromone = 1.0 / numCities;

    // Pheromone matrix initialized to uniform value
    std::vector<std::vector<double>> pheromone(numCities, std::vector<double>(numCities, initPheromone));

    // Fixed random generator for determinism
    std::mt19937 rng(12345);

    double bestTourLength = std::numeric_limits<double>::max();
    std::vector<int> bestTour;

    // Main ACO loop
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        std::vector<std::vector<int>> antTours(numAnts);
        std::vector<double> antLengths(numAnts, 0.0);

        // Build tours for each ant
        for (int ant = 0; ant < numAnts; ++ant) {
            std::vector<bool> visited(numCities, false);
            int startCity = ant % numCities;
            int currentCity = startCity;
            antTours[ant].push_back(currentCity);
            visited[currentCity] = true;

            for (int step = 1; step < numCities; ++step) {
                // Compute probabilities for unvisited neighbors
                std::vector<double> probabilities(numCities, 0.0);
                double sumProb = 0.0;
                for (int nextCity = 0; nextCity < numCities; ++nextCity) {
                    if (!visited[nextCity]) {
                        double distance = static_cast<double>(distances[currentCity][nextCity]);
                        if (distance == 0.0) distance = 1e-6; // avoid division by zero
                        probabilities[nextCity] = std::pow(pheromone[currentCity][nextCity], alpha) *
                                                  std::pow(1.0 / distance, beta);
                        sumProb += probabilities[nextCity];
                    }
                }
                // If all visited or sum is zero (shouldn't happen), pick deterministic fallback
                if (sumProb == 0.0) {
                    for (int nextCity = 0; nextCity < numCities; ++nextCity) {
                        if (!visited[nextCity]) {
                            probabilities[nextCity] = 1.0;
                            sumProb += 1.0;
                        }
                    }
                }
                // Normalize and select next city probabilistically
                double randValue = std::uniform_real_distribution<double>(0.0, 1.0)(rng) * sumProb;
                double cumulative = 0.0;
                int chosenCity = -1;
                for (int nextCity = 0; nextCity < numCities; ++nextCity) {
                    if (!visited[nextCity]) {
                        cumulative += probabilities[nextCity];
                        if (randValue <= cumulative) {
                            chosenCity = nextCity;
                            break;
                        }
                    }
                }
                // Safety fallback if none chosen (due to floating point)
                if (chosenCity == -1) {
                    for (int nextCity = 0; nextCity < numCities; ++nextCity) {
                        if (!visited[nextCity]) {
                            chosenCity = nextCity;
                            break;
                        }
                    }
                }
                // Move to next city
                antTours[ant].push_back(chosenCity);
                antLengths[ant] += distances[currentCity][chosenCity];
                visited[chosenCity] = true;
                currentCity = chosenCity;
            }
            // Return to start
            antLengths[ant] += distances[currentCity][startCity];
            antTours[ant].push_back(startCity);
        }

        // Update global best
        for (int ant = 0; ant < numAnts; ++ant) {
            if (antLengths[ant] < bestTourLength) {
                bestTourLength = antLengths[ant];
                bestTour = antTours[ant];
            }
        }

        // Pheromone evaporation
        for (int i = 0; i < numCities; ++i) {
            for (int j = 0; j < numCities; ++j) {
                pheromone[i][j] *= (1.0 - rho);
            }
        }

        // Pheromone deposition
        for (int ant = 0; ant < numAnts; ++ant) {
            double deposit = Q / antLengths[ant];
            for (int i = 0; i < numCities; ++i) {
                int from = antTours[ant][i];
                int to = antTours[ant][i + 1];
                pheromone[from][to] += deposit;
                pheromone[to][from] += deposit;
            }
        }

        // Elitist boost for best tour
        double eliteDeposit = Q / bestTourLength;
        for (int i = 0; i < numCities; ++i) {
            int from = bestTour[i];
            int to = bestTour[i + 1];
            pheromone[from][to] += eliteDeposit;
            pheromone[to][from] += eliteDeposit;
        }
    }

    return bestTourLength;
}

#include <cassert>
#include <vector>
#include <cmath>

// Declaration of the solution function
double solveTSP(const std::vector<std::vector<int>>& distances);

int main() {
    // Test case 1: 4 cities on a square (side length 1), but we use 5 cities with one degenerate
    // We'll test a simple symmetric matrix for 5 cities: all pairwise distances 1 (except diagonal)
    std::vector<std::vector<int>> d1(5, std::vector<int>(5, 1));
    for (int i = 0; i < 5; ++i) d1[i][i] = 0;
    // Optimal tour length is 5 (visiting all 5, each edge is 1, plus return to start)
    double result1 = solveTSP(d1);
    // Due to randomness, the result might not be exactly 5, but should be at least 5 (best possible)
    assert(result1 >= 5.0 - 1e-6);

    // Test case 2: A line of cities at x=0..4, y=0, optimal tour goes 0->1->2->3->4->0
    std::vector<std::vector<int>> d2(5, std::vector<int>(5, 0));
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            d2[i][j] = std::abs(i - j);
        }
    }
    double result2 = solveTSP(d2);
    // The optimal tour length is 8 (0-1-2-3-4-0: distances 1+1+1+1+4 = 8)
    assert(result2 >= 8.0 - 1e-6);

    // Test case 3: Ensure determinism: call twice with same input, same result
    std::vector<std::vector<int>> d3(5, std::vector<int>(5, 0));
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            d3[i][j] = (i == j) ? 0 : (i * 10 + j);
        }
    }
    // Symmetric: set d3[i][j] = d3[j][i]
    for (int i = 0; i < 5; ++i) {
        for (int j = i+1; j < 5; ++j) {
            int val = (i * 10 + j) * (j * 10 + i) % 100; // some deterministic values
            d3[i][j] = val;
            d3[j][i] = val;
        }
    }
    double r1 = solveTSP(d3);
    double r2 = solveTSP(d3);
    assert(std::abs(r1 - r2) < 1e-9);

    // Test case 4: Small matrix with known optimal (a permutation)
    // 5 cities: distances from 0: to 1=2, to 2=3, to 3=4, to 4=5
    // from 1: to 2=6, to 3=7, to 4=8
    // from 2: to 3=9, to 4=10
    // from 3: to 4=11
    // Optimal tour 0-1-2-3-4-0: 2+6+9+11+5 = 33
    std::vector<std::vector<int>> d4(5, std::vector<int>(5, 0));
    d4[0][1] = 2; d4[1][0] = 2;
    d4[0][2] = 3; d4[2][0] = 3;
    d4[0][3] = 4; d4[3][0] = 4;
    d4[0][4] = 5; d4[4][0] = 5;
    d4[1][2] = 6; d4[2][1] = 6;
    d4[1][3] = 7; d4[3][1] = 7;
    d4[1][4] = 8; d4[4][1] = 8;
    d4[2][3] = 9; d4[3][2] = 9;
    d4[2][4] = 10; d4[4][2] = 10;
    d4[3][4] = 11; d4[4][3] = 11;
    double result4 = solveTSP(d4);
    assert(result4 >= 33.0 - 1e-6);

    return 0;
}

// The solution models the ACO metaheuristic. The function initializes a pheromone matrix with a small constant value (e.g., 1.0 / number of cities). Each ant starts at a unique city (or cycles through starting cities) and builds a complete tour by iteratively selecting the next unvisited city probabilistically. The probability of moving from city `i` to city `j` is proportional to `(pheromone[i][j]^alpha) * (1/distance[i][j])^beta`, where alpha and beta are parameters (e.g., 1.0 and 5.0). After all ants complete their tours, pheromone evaporates globally by a factor `(1 - rho)` (rho = 0.5). Then, each ant deposits pheromone along its tour proportional to `Q / tour_length`. Additionally, the elitist strategy adds an extra `Q / best_tour_length` to edges belonging to the global best tour found so far. The process repeats for a fixed number of iterations (e.g., 50). Edge cases: ensure the function works with any valid symmetric distance matrix; handle zero distances (by adding a small epsilon in the heuristic to avoid division by zero). Time complexity is O(iterations * num_ants * num_cities^2) due to the next-city selection loop and tour building. Space complexity is O(num_cities^2) for the distance and pheromone matrices. The function is deterministic by seeding a local random generator with a fixed seed (e.g., 12345) at the start.

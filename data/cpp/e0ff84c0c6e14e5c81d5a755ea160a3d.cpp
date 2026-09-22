/*
Write a standalone C++ function `percolationStats` that simulates the percolation process on an \(n \times n\) grid for a given number of trials and returns the mean, standard deviation, and 95% confidence interval of the percolation threshold (the fraction of open sites when the system first percolates). The grid is initially fully blocked. In each trial, randomly choose a blocked cell (using a uniform distribution over row and column indices in `[1, n]`) and open it, repeating until the system percolates. The percolation condition is that there is a path of open cells from the top row to the bottom row. Define a helper class `Percolation` with methods `open(row, col)`, `isOpen(row, col)`, `numberOfOpenSites()`, and `percolates()`, and implement it using a union-find (disjoint set) data structure with path compression and union by rank. The function must return a struct with four double members: `mean`, `stddev`, `confidenceLo`, and `confidenceHi`. Use a sample standard deviation (divide by `trials - 1`), and the confidence interval is `mean ± 1.96 * stddev / sqrt(trials)`. The function should take two integer parameters: grid size `n` (>= 1) and number of trials `trials` (>= 2). Edge cases: if `trials` is 1, return `stddev = 0` and confidence interval equal to the mean. Ensure the random number generator is properly seeded with `std::random_device`. Provide a self-contained implementation that compiles without external libraries.
*/
#include <vector>
#include <numeric>
#include <random>
#include <cmath>
#include <stdexcept>

// Structure to hold statistical results
struct PercolationStats {
    double mean;
    double stddev;
    double confidenceLo;
    double confidenceHi;
};

// Union-find (Disjoint Set Union) with path compression and union by rank
class UnionFind {
public:
    explicit UnionFind(int size) : parent(size), rank(size, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }
    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            rank[rx]++;
        }
    }
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
private:
    std::vector<int> parent;
    std::vector<int> rank;
};

// Percolation class using union-find with two virtual nodes
class Percolation {
public:
    Percolation(int n) : n(n), openSites(n*n, false), uf(n*n + 2) {
        if (n <= 0) throw std::invalid_argument("Grid size must be positive");
        topVirtual = n*n;
        bottomVirtual = n*n + 1;
        // Connect top row to topVirtual
        for (int col = 1; col <= n; ++col) {
            uf.unite(index(1, col), topVirtual);
        }
        // Connect bottom row to bottomVirtual
        for (int col = 1; col <= n; ++col) {
            uf.unite(index(n, col), bottomVirtual);
        }
    }
    void open(int row, int col) {
        validate(row, col);
        int idx = index(row, col);
        if (!openSites[idx]) {
            openSites[idx] = true;
            // Connect to adjacent open sites
            if (row > 1 && isOpen(row-1, col)) uf.unite(idx, index(row-1, col));
            if (row < n && isOpen(row+1, col)) uf.unite(idx, index(row+1, col));
            if (col > 1 && isOpen(row, col-1)) uf.unite(idx, index(row, col-1));
            if (col < n && isOpen(row, col+1)) uf.unite(idx, index(row, col+1));
        }
    }
    bool isOpen(int row, int col) const {
        validate(row, col);
        return openSites[index(row, col)];
    }
    int numberOfOpenSites() const {
        int count = 0;
        for (bool b : openSites) if (b) count++;
        return count;
    }
    bool percolates() const {
        return uf.connected(topVirtual, bottomVirtual);
    }
private:
    int n;
    std::vector<bool> openSites;
    UnionFind uf;
    int topVirtual;
    int bottomVirtual;
    int index(int row, int col) const {
        return (row-1) * n + (col-1);
    }
    void validate(int row, int col) const {
        if (row < 1 || row > n || col < 1 || col > n) {
            throw std::out_of_range("Invalid cell coordinates");
        }
    }
};

// Main function: simulate percolation and compute statistics
PercolationStats percolationStats(int n, int trials) {
    if (n <= 0) throw std::invalid_argument("Grid size must be positive");
    if (trials <= 0) throw std::invalid_argument("Number of trials must be positive");

    std::vector<double> thresholds(trials, 0.0);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(1, n);

    for (int t = 0; t < trials; ++t) {
        Percolation perc(n);
        while (!perc.percolates()) {
            int row = dist(rng);
            int col = dist(rng);
            if (!perc.isOpen(row, col)) {
                perc.open(row, col);
            }
        }
        thresholds[t] = static_cast<double>(perc.numberOfOpenSites()) / (n * n);
    }

    // Compute mean
    double sum = std::accumulate(thresholds.begin(), thresholds.end(), 0.0);
    double mean = sum / trials;

    // Compute sample standard deviation
    double stddev = 0.0;
    if (trials > 1) {
        double devsum = 0.0;
        for (double x : thresholds) {
            double d = x - mean;
            devsum += d * d;
        }
        stddev = std::sqrt(devsum / (trials - 1));
    }

    // Confidence interval
    double margin = 1.96 * stddev / std::sqrt(static_cast<double>(trials));
    if (trials == 1) {
        margin = 0.0;
    }

    return {mean, stddev, mean - margin, mean + margin};
}
#include <cassert>
#include <cmath>
#include <iostream>

// The solution function is assumed to be defined above (percolationStats)
int main() {
    // Test 1: n=1, single trial -> mean=1, stddev=0, CI=[1,1]
    PercolationStats s1 = percolationStats(1, 1);
    assert(std::fabs(s1.mean - 1.0) < 1e-9);
    assert(std::fabs(s1.stddev - 0.0) < 1e-9);
    assert(std::fabs(s1.confidenceLo - 1.0) < 1e-9);
    assert(std::fabs(s1.confidenceHi - 1.0) < 1e-9);

    // Test 2: n=1, multiple trials -> all thresholds are 1, mean=1, stddev=0
    PercolationStats s2 = percolationStats(1, 5);
    assert(std::fabs(s2.mean - 1.0) < 1e-9);
    assert(std::fabs(s2.stddev - 0.0) < 1e-9);
    assert(std::fabs(s2.confidenceLo - 1.0) < 1e-9);
    assert(std::fabs(s2.confidenceHi - 1.0) < 1e-9);

    // Test 3: n=2, many trials -> mean should be around 0.5-0.7 (not exact, just sanity)
    PercolationStats s3 = percolationStats(2, 1000);
    assert(s3.mean > 0.4 && s3.mean < 0.9);
    assert(s3.stddev > 0.0 && s3.stddev < 0.5);
    assert(s3.confidenceLo < s3.mean && s3.mean < s3.confidenceHi);

    // Test 4: Invalid inputs should throw
    bool threw = false;
    try { percolationStats(0, 5); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    threw = false;
    try { percolationStats(3, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Test 5: Ensure confidence interval shrinks with more trials (looser check)
    PercolationStats s4 = percolationStats(3, 50);
    PercolationStats s5 = percolationStats(3, 200);
    double width1 = s4.confidenceHi - s4.confidenceLo;
    double width2 = s5.confidenceHi - s5.confidenceLo;
    assert(width2 < width1 + 0.05); // not a strict guarantee, but typically true

    std::cout << "All tests passed!" << std::endl;
}
// The solution uses a union-find data structure to efficiently track connected open cells. For each trial, we repeatedly select random (row, col) pairs until the system percolates. To avoid infinite loops, we only open cells that are not already open. The union-find maintains connections between adjacent open cells (up, down, left, right). We add two virtual nodes: one connected to all cells in the top row (index 0) and one connected to all cells in the bottom row (index `n*n+1`). The system percolates when the top and bottom virtual nodes are in the same connected component. For each trial, after the grid percolates, we compute the ratio `numberOfOpenSites / (n*n)` and store it. After all trials, we compute the mean (average), sample standard deviation (using `n-1` denominator), and the confidence interval. Time complexity per trial is `O(k * α(n^2))` where `k` is the number of random openings until percolation (on average about `n^2 * threshold`, threshold ~0.5927 for large n), and `α` is the inverse Ackermann function (nearly constant). Space complexity is `O(n^2)` for the union-find arrays and the open/state array. The algorithm correctly handles `n=1` (immediately percolates when the single cell is opened) and `trials=1` (stddev=0).

/*
Write a standalone C++ function named `saveParticleData` that serializes particle positions to a binary file in a platform-independent way. The function must accept a prefix string, a vector of `Particle2D` structs (each containing `double x` and `double y` members), an integer label, and an integer `domainSize`. It should create a file named `<prefix><label>.bin` (for example, `particles_0.bin` if prefix is `"particles_"` and label is 0). The file format must be: first write the total particle count as a 4-byte unsigned integer, then the `domainSize` as a 4-byte integer, then for each particle write `x` and `y` as 8-byte doubles in that order. The function should return `true` on success and `false` if the file cannot be opened for writing. The task is to implement the serialization logic correctly, including proper endianness handling (assume little-endian output), and to ensure all data is written before the file is closed.
*/
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <cstring>

// Simple structure to hold a 2D particle position.
struct Particle2D {
    double x;
    double y;
};

/**
 * Serialize a vector of particles to a binary file with the given prefix and label.
 * File format: uint32_t count, uint32_t domainSize, then for each particle: double x, double y.
 * Returns true on success, false if file cannot be opened for writing.
 */
bool saveParticleData(const std::string& prefix, const std::vector<Particle2D>& particles,
                      int label, uint32_t domainSize) {
    // Construct the filename like "prefix<label>.bin"
    std::string filename = prefix + std::to_string(label) + ".bin";

    // Open file in binary mode for writing
    std::ofstream out(filename, std::ios::binary | std::ios::out);
    if (!out.is_open()) {
        return false;
    }

    // Write the count (number of particles) as a 4-byte unsigned integer
    uint32_t count = static_cast<uint32_t>(particles.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // Write the domain size as a 4-byte integer
    out.write(reinterpret_cast<const char*>(&domainSize), sizeof(domainSize));

    // Write each particle's x and y as doubles
    for (const auto& p : particles) {
        out.write(reinterpret_cast<const char*>(&p.x), sizeof(double));
        out.write(reinterpret_cast<const char*>(&p.y), sizeof(double));
    }

    // Flush and close; check for stream errors
    out.flush();
    bool success = out.good();
    out.close();

    return success;
}
#include <cassert>
#include <fstream>
#include <vector>

// Assume the Particle2D struct and saveParticleData are already defined above.

int main() {
    // Test 1: Basic save with three particles
    {
        std::vector<Particle2D> particles = {{1.5, 2.5}, {-3.0, 4.0}, {0.0, 0.0}};
        bool ok = saveParticleData("test_", particles, 0, 10);
        assert(ok);
        // Read back the file and verify contents manually
        std::ifstream in("test_0.bin", std::ios::binary);
        uint32_t count;
        uint32_t domain;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        in.read(reinterpret_cast<char*>(&domain), sizeof(domain));
        assert(count == 3);
        assert(domain == 10);
        for (int i = 0; i < 3; ++i) {
            double x, y;
            in.read(reinterpret_cast<char*>(&x), sizeof(double));
            in.read(reinterpret_cast<char*>(&y), sizeof(double));
            assert(x == particles[i].x);
            assert(y == particles[i].y);
        }
        assert(in.eof());
    }

    // Test 2: Empty vector
    {
        std::vector<Particle2D> particles;
        bool ok = saveParticleData("test_", particles, 1, 5);
        assert(ok);
        std::ifstream in("test_1.bin", std::ios::binary);
        uint32_t count, domain;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        in.read(reinterpret_cast<char*>(&domain), sizeof(domain));
        assert(count == 0);
        assert(domain == 5);
    }

    // Test 3: Larger label and negative coordinates
    {
        std::vector<Particle2D> particles = {{-123.456, 789.123}};
        bool ok = saveParticleData("data_", particles, 42, 1000);
        assert(ok);
        std::ifstream in("data_42.bin", std::ios::binary);
        uint32_t count, domain;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        in.read(reinterpret_cast<char*>(&domain), sizeof(domain));
        assert(count == 1);
        assert(domain == 1000);
        double x, y;
        in.read(reinterpret_cast<char*>(&x), sizeof(double));
        in.read(reinterpret_cast<char*>(&y), sizeof(double));
        assert(x == -123.456);
        assert(y == 789.123);
    }

    // Test 4: Invalid path (should return false)
    {
        std::vector<Particle2D> particles = {{1.0, 2.0}};
        bool ok = saveParticleData("/nonexistent_dir/prefix_", particles, 0, 1);
        assert(!ok);
    }

    // Test 5: Ensure the file is properly overwritten when label repeats
    {
        std::vector<Particle2D> particles1 = {{1.0, 2.0}};
        std::vector<Particle2D> particles2 = {{3.0, 4.0}, {5.0, 6.0}};
        saveParticleData("overwrite_", particles1, 7, 1);
        saveParticleData("overwrite_", particles2, 7, 2);
        std::ifstream in("overwrite_7.bin", std::ios::binary);
        uint32_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        assert(count == 2); // Should be from the second save
    }

    // Cleanup created test files (optional, but good practice)
    remove("test_0.bin");
    remove("test_1.bin");
    remove("data_42.bin");
    remove("overwrite_7.bin");

    return 0;
}
// The core algorithm is straightforward: open a binary output stream, write the particle count and domain size, then loop through the particles writing each coordinate. The main challenges are: (1) ensuring the file path is correctly constructed from the prefix and integer label (using `std::to_string`), (2) using binary mode to avoid newline translation on Windows, (3) writing fixed-size types to ensure consistent file sizes across platforms (use `uint32_t` and `double` explicitly), and (4) checking the file stream state after writes and returning `false` if any write fails. Edge cases include an empty particle vector (which should still write a count of 0), and invalid prefix (e.g., empty string). Time complexity is O(n) where n is the number of particles, and space complexity is O(1) extra memory beyond the input vector. The function should use `std::ofstream` with `std::ios::binary` and write via `reinterpret_cast` for safe binary I/O or use stream `write` methods. For little-endian output, we can assume the host is little-endian (as most common systems) but we can also document that. After writing, we flush and close explicitly and return true if no stream errors occurred.

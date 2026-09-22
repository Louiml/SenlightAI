// Write a standalone C++ function named `buildGraphSummary` that reads a directed graph description from a text file and returns a `std::string` containing a formatted summary of the graph. The input file format is as follows: each vertex is represented by a single uppercase letter (A-Z), followed by its out-degree (a non-negative integer), then that many adjacent vertex letters (each a single uppercase letter), all separated by whitespace. The file may contain multiple vertices, each on its own line or spanning multiple lines, and the number of vertices is not given in the file—the function must read until the end of file. The function should accept a `const std::string&` parameter for the file name and return a string that lists each vertex in the order it appears, along with its out-degree and the list of its adjacent vertices, formatted exactly as: `Vertex: <letter>, OutDegree: <n>, Adjacent: <adj1> <adj2> ...` with each vertex's summary on a new line. If the file cannot be opened, or if any vertex letter is not an uppercase letter (A-Z) or any adjacent letter is not uppercase, or if the out-degree is negative, the function should throw a `std::runtime_error` with an appropriate descriptive message. The function must use proper `const` correctness (e.g., parameters by const reference, and the function itself does not mutate any external state) and must not rely on any external class definitions—use only standard library features. The graph is assumed to be simple (no self-loops) and the total number of vertices in the file is at most 26 (one per uppercase letter), but you cannot assume the vertices are distinct (they may repeat, in which case each occurrence is treated as a separate line in the output). Edge cases include: a file with zero vertices (should return an empty string), a vertex with out-degree 0 (adjacent list empty), and whitespace variations (extra spaces, tabs, newlines) between tokens.
The solution reads the file line by line or token by token using an `std::ifstream` combined with an `std::istringstream` for robust whitespace handling. The main algorithm is straightforward: open the file, verify it is open (otherwise throw `std::runtime_error`), then repeatedly attempt to read a vertex letter (as a `char`), an out-degree (as an `int`), and then exactly out-degree adjacent letters. For each successfully read vertex, validate that the letter is in 'A'–'Z' and the out-degree is non-negative; if invalid, throw `std::runtime_error`. For each adjacent letter, validate it is uppercase 'A'–'Z'; otherwise throw. Collect the formatted string for that vertex and append it to a result string with a newline. Continue until reaching end-of-file. A subtle edge case is detecting malformed data: if after reading a vertex letter the out-degree cannot be read (or is negative), or if the required number of adjacent letters is not available before EOF, the function should throw. To handle this, check `fin` after each read operation and also verify that the number of successfully read adjacent letters equals the declared out-degree; if EOF occurs prematurely, throw. Time complexity is O(V * d) where V is the number of vertex lines and d is the maximum out-degree, because each token is processed once; space complexity is O(V * d) for the returned string and the temporary storage used by the stream buffers.
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

// Reads a directed graph from a text file and returns a formatted summary string.
// Format per vertex: "Vertex: <letter>, OutDegree: <n>, Adjacent: <adj1> <adj2> ..."
// Throws std::runtime_error on file-open failure or invalid data.
std::string buildGraphSummary(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string result;
    std::string line;

    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        char vertex;
        int outDegree;

        // Try to read the vertex letter and out-degree; skip empty lines.
        if (!(iss >> vertex >> outDegree)) {
            continue; // empty or malformed line with no tokens, skip
        }

        // Validate vertex letter.
        if (vertex < 'A' || vertex > 'Z') {
            throw std::runtime_error("Invalid vertex letter: " + std::string(1, vertex));
        }
        // Validate out-degree.
        if (outDegree < 0) {
            throw std::runtime_error("Negative out-degree for vertex " + std::string(1, vertex));
        }

        // Read adjacent letters.
        std::string adjList;
        for (int i = 0; i < outDegree; ++i) {
            char adj;
            if (!(iss >> adj)) {
                throw std::runtime_error("Insufficient adjacent vertices for vertex " + std::string(1, vertex));
            }
            if (adj < 'A' || adj > 'Z') {
                throw std::runtime_error("Invalid adjacent vertex: " + std::string(1, adj));
            }
            if (i > 0) {
                adjList += " ";
            }
            adjList += adj;
        }

        // Append formatted line to result.
        result += "Vertex: ";
        result += vertex;
        result += ", OutDegree: ";
        result += std::to_string(outDegree);
        result += ", Adjacent: ";
        if (outDegree > 0) {
            result += adjList;
        }
        result += "\n";
    }

    // The output should not have a trailing newline if it's empty; otherwise keep as is.
    return result;
}
#include <cassert>
#include <fstream>
#include <string>
#include <stdexcept>

// Global main function with assert checks
int main() {
    // Test 1: Simple graph with three vertices
    {
        std::ofstream f("test1.txt");
        f << "A 2 B C\nB 1 D\nC 0\n";
        f.close();
        std::string result = buildGraphSummary("test1.txt");
        assert(result == "Vertex: A, OutDegree: 2, Adjacent: B C\nVertex: B, OutDegree: 1, Adjacent: D\nVertex: C, OutDegree: 0, Adjacent: \n");
    }
    // Test 2: Empty file (zero vertices)
    {
        std::ofstream f("test2.txt");
        f.close();
        std::string result = buildGraphSummary("test2.txt");
        assert(result == "");
    }
    // Test 3: Whitespace variations and duplicate vertex letters
    {
        std::ofstream f("test3.txt");
        f << "  A   1   Z   \n\tB 2 A Z\nA 1 B\n";
        f.close();
        std::string result = buildGraphSummary("test3.txt");
        assert(result == "Vertex: A, OutDegree: 1, Adjacent: Z\nVertex: B, OutDegree: 2, Adjacent: A Z\nVertex: A, OutDegree: 1, Adjacent: B\n");
    }
    // Test 4: Invalid vertex letter
    {
        std::ofstream f("test4.txt");
        f << "a 1 B\n";
        f.close();
        bool threw = false;
        try {
            buildGraphSummary("test4.txt");
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()).find("Invalid vertex") != std::string::npos);
        }
        assert(threw);
    }
    // Test 5: Negative out-degree
    {
        std::ofstream f("test5.txt");
        f << "A -1 B\n";
        f.close();
        bool threw = false;
        try {
            buildGraphSummary("test5.txt");
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()).find("Negative") != std::string::npos);
        }
        assert(threw);
    }
    // Test 6: Incomplete adjacency list (EOF before all adjacents)
    {
        std::ofstream f("test6.txt");
        f << "A 2 B\n";
        f.close();
        bool threw = false;
        try {
            buildGraphSummary("test6.txt");
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()).find("Insufficient") != std::string::npos);
        }
        assert(threw);
    }
    // Test 7: File not found
    {
        bool threw = false;
        try {
            buildGraphSummary("nonexistent_file.txt");
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()).find("Cannot open") != std::string::npos);
        }
        assert(threw);
    }
    // Test 8: Invalid adjacent letter
    {
        std::ofstream f("test8.txt");
        f << "A 1 b\n";
        f.close();
        bool threw = false;
        try {
            buildGraphSummary("test8.txt");
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()).find("Invalid adjacent") != std::string::npos);
        }
        assert(threw);
    }
    return 0;
}

// Write a standalone C++ function that simulates bounds inference and execution for an image-processing pipeline with two 2D integer buffers (`in1`, `in2`) and an output buffer (`out`). The function takes four arguments: three descriptors for the input/output buffers (each providing `host` pointer, `extent[2]`, `min[2]`, `stride[2]`, `elem_size`) and an integer scalar parameter. It must perform two modes: (1) a bounds-query mode where, given the output’s extent and min, it computes and writes into `in1` the extent/min required (specifically: `in1` extent = out extent, `in1` min = out min minus 1 in both dimensions) and into `in2` the extent/min (specifically: `in2` extent = out extent, `in2` min = out min plus 7 in y, and same min as out in x); and (2) an execution mode where it fills `out` such that `out(x,y) = in1(x-1,y-1) + in2(x,y+7) + scalar_param` for all x,y in the out region. In execution mode, assume that `in2` has already been filled with constant byte value `0x02` and `in1` with constant byte `0x01` (each `int` element will be a repeating pattern). The function must return 0 on success and a negative error code if dimensions are not 2. You may use a helper to access pixels given a descriptor and coordinates, handling negative mins via strided indexing. The function must handle both modes and produce correct results as verified by a test that mimics the original snippet’s checks.
The core is to distinguish bounds-query mode from execution mode by checking whether each input’s `host` pointer is null after a special flag (in a real Halide-style API, `is_bounds_query()` checks `host == nullptr` and a flag; here we can use a convention: if `in1.host == nullptr && in2.host == nullptr`, treat as bounds query; otherwise execution). In bounds query, set only extents and mins for both inputs based on the output’s extents and mins. In execution, iterate over the output’s region using its min and extent; for each (x,y), compute the value as sum of three integers: from `in1` at coordinates `(x-1, y-1)` (which are within `in1` bounds because its min is out min minus 1 and extent matches out extent), from `in2` at `(x, y+7)` (within `in2` bounds because minY = out.minY + 7), and the scalar. Since the inputs contain `int` values formed by repeating bytes (e.g., `0x01010101` for byte 1), the sum yields `0x01010101 + 0x02020202 + scalar`. The main algorithm is O(N) where N = output extent[0]*extent[1], and memory access uses stride-based indexing. Edge cases: negative mins must be respected; ensure bounds inference sets correct mins/extents. Time complexity O(N) for execution, O(1) for bounds query; space O(1) auxiliary.
#include <cstddef>
#include <cstdint>
#include <cstring>

// Descriptor for a 2D buffer (simplified from Halide's buffer_t)
struct Buffer2D {
    uint8_t* host;        // null in bounds query, non-null in execution
    int32_t min[2];
    int32_t extent[2];
    int32_t stride[2];
    int32_t elem_size;    // should be 4
};

// Helper: access pixel (x,y) from a 2D integer buffer descriptor
static int& pixel_at(Buffer2D& buf, int x, int y) {
    int32_t* base = reinterpret_cast<int32_t*>(buf.host);
    int idx = (x - buf.min[0]) * buf.stride[0] + (y - buf.min[1]) * buf.stride[1];
    return base[idx];
}

// Simulates bounds inference and execution for the pipeline.
// Returns 0 on success, negative error code if dimensions != 2.
int run_pipeline(Buffer2D& in1, Buffer2D& in2, int scalar_param, Buffer2D& out) {
    // Standard check: all buffers must be 2D
    if (out.dimensions != 2 || in1.dimensions != 2 || in2.dimensions != 2) {
        return -1; // bad dimensions
    }
    
    // Determine mode: if in1 and in2 have null hosts, this is bounds query
    bool bounds_query = (in1.host == nullptr && in2.host == nullptr);
    
    if (bounds_query) {
        // Set in1: extent equals out extent, min = out.min - 1 in both dims
        in1.extent[0] = out.extent[0];
        in1.extent[1] = out.extent[1];
        in1.min[0] = out.min[0] - 1;
        in1.min[1] = out.min[1] - 1;
        
        // Set in2: extent equals out extent in x, min[0] = out.min[0]; extent[1] = out.extent[1], min[1] = out.min[1] + 7
        in2.extent[0] = out.extent[0];
        in2.extent[1] = out.extent[1];
        in2.min[0] = out.min[0];
        in2.min[1] = out.min[1] + 7;
        
        return 0;
    }
    
    // Execution: fill out based on in1 and in2
    for (int y = out.min[1]; y < out.min[1] + out.extent[1]; ++y) {
        for (int x = out.min[0]; x < out.min[0] + out.extent[0]; ++x) {
            int val = pixel_at(in1, x-1, y-1) + pixel_at(in2, x, y+7) + scalar_param;
            pixel_at(out, x, y) = val;
        }
    }
    return 0;
}
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

// Forward declaration (or include the solution header)
// Assume solution function and Buffer2D are available from above.
// For testing, we copy the Buffer2D struct and run_pipeline here.

int main() {
    // Setup output buffer: 60x40, min (0,0), stride (1,60), elem_size 4
    Buffer2D out;
    out.host = (uint8_t*)malloc(60*40*sizeof(int));
    out.min[0] = 0; out.min[1] = 0;
    out.extent[0] = 60; out.extent[1] = 40;
    out.stride[0] = 1;  out.stride[1] = 60;
    out.elem_size = 4;
    out.dimensions = 2;
    
    // Bounds query: in1 and in2 with null hosts
    Buffer2D in1, in2;
    in1.host = nullptr; in1.dimensions = 2; in1.elem_size = 4;
    in2.host = nullptr; in2.dimensions = 2; in2.elem_size = 4;
    
    int scalar = 4;
    int err = run_pipeline(in1, in2, scalar, out);
    assert(err == 0);
    // Check in1 bounds: min = (-1,-1), extent = (60,40)
    assert(in1.min[0] == -1 && in1.min[1] == -1);
    assert(in1.extent[0] == 60 && in1.extent[1] == 40);
    // Check in2 bounds: min[0] = 0, min[1] = 7, extent = (60,40)
    assert(in2.min[0] == 0 && in2.min[1] == 7);
    assert(in2.extent[0] == 60 && in2.extent[1] == 40);
    
    // Allocate actual input buffers with the inferred bounds
    in1.host = (uint8_t*)malloc(in1.extent[0]*in1.extent[1]*in1.elem_size);
    in2.host = (uint8_t*)malloc(in2.extent[0]*in2.extent[1]*in2.elem_size);
    // Fill with repeated byte patterns: in1 byte 0x01, in2 byte 0x02
    memset(in1.host, 0x01, in1.extent[0]*in1.extent[1]*4);
    memset(in2.host, 0x02, in2.extent[0]*in2.extent[1]*4);
    in1.stride[0] = 1; in1.stride[1] = in1.extent[0];
    in2.stride[0] = 1; in2.stride[1] = in2.extent[0];
    
    // Reset output? No need, we overwrite all.
    
    // Execute
    err = run_pipeline(in1, in2, scalar, out);
    assert(err == 0);
    
    // Verify every output pixel: each int = 0x01010101 + 0x02020202 + 4 = 0x03030303 + 4 = 0x03030307
    int expected = 0x01010101 + 0x02020202 + scalar;
    for (int y = 0; y < 40; ++y) {
        for (int x = 0; x < 60; ++x) {
            int result = pixel_at(out, x, y);
            assert(result == expected);
        }
    }
    
    // Additionally test a different offset output to ensure min handling
    Buffer2D out2;
    out2.host = (uint8_t*)malloc(20*10*sizeof(int));
    out2.min[0] = -5; out2.min[1] = 100;
    out2.extent[0] = 20; out2.extent[1] = 10;
    out2.stride[0] = 1; out2.stride[1] = 20;
    out2.elem_size = 4; out2.dimensions = 2;
    
    // Bounds query for out2
    Buffer2D in1b, in2b;
    in1b.host = nullptr; in1b.dimensions = 2; in1b.elem_size = 4;
    in2b.host = nullptr; in2b.dimensions = 2; in2b.elem_size = 4;
    err = run_pipeline(in1b, in2b, scalar, out2);
    assert(err == 0);
    assert(in1b.min[0] == -6 && in1b.min[1] == 99);
    assert(in1b.extent[0] == 20 && in1b.extent[1] == 10);
    assert(in2b.min[0] == -5 && in2b.min[1] == 107);
    assert(in2b.extent[0] == 20 && in2b.extent[1] == 10);
    
    // Allocate and execute
    in1b.host = (uint8_t*)malloc(20*10*4);
    in2b.host = (uint8_t*)malloc(20*10*4);
    memset(in1b.host, 0x01, 20*10*4);
    memset(in2b.host, 0x02, 20*10*4);
    in1b.stride[0] = 1; in1b.stride[1] = 20;
    in2b.stride[0] = 1; in2b.stride[1] = 20;
    
    err = run_pipeline(in1b, in2b, scalar, out2);
    assert(err == 0);
    for (int y = 100; y < 110; ++y) {
        for (int x = -5; x < 15; ++x) {
            int result = pixel_at(out2, x, y);
            assert(result == expected);
        }
    }
    
    free(out.host);
    free(out2.host);
    free(in1.host);
    free(in2.host);
    free(in1b.host);
    free(in2b.host);
    
    printf("All tests passed!\n");
    return 0;
}

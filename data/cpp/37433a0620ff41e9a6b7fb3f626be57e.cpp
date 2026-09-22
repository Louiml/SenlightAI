// Write a C++ function `uint8_t propagate_deblock_flags(int mx, int my, const uint8_t* current_row, const uint8_t* previous_frame, const int* block_offsets, int mv_dx, int mv_dy, int blocks_per_row, int frame_width, int frame_height)` that simulates the semaphore propagation logic from MPEG-4 post-processing. The function receives: `mx`, `my` as the half-pixel resolution coordinates of the motion-compensated block (ranging from `-2*MB_SIZE` to `2*(width-MB_SIZE)`), `current_row` pointing to a 4-byte semaphore buffer for the current macroblock (each byte corresponds to one 8x8 block, with bit 2 (value 4) indicating deringing and bit 0-1 used for deblocking), `previous_frame` pointing to a 1D array of `blocks_per_row * (frame_height/8)` bytes representing previous frame block semaphores (row-major, one byte per 8x8 block), `block_offsets` pointing to an array of 4 integers that give byte offsets to each of the 4 8x8 blocks in the current MB (typically {0, 8, 128, 136} for a 16x16 MB with 8x8 blocks), `mv_dx`, `mv_dy` as the motion vector components in half-pixel units, `blocks_per_row` as the number of 8x8 blocks per row, `frame_width` and `frame_height` in pixels. The function must return a `uint8_t` mask (0x0 or 0x3) as the deblocking decision flag. The logic is: If the (mx,my) point lies within the valid motion search area (from 0 to `2*(frame_width - 16)` for x and similarly for y), treat it as interior and use the block at `(mx>>4, my>>4)` as the primary reference block, selecting up to 4 neighboring blocks based on whether `mv_dx` and `mv_dy` are multiples of 16 (half-pixel units). If outside, clamp coordinates to valid block range for each of the 4 sub-blocks in the MB and apply the same neighbor-selection logic with boundary checks. For each of the 4 blocks in the current MB, if its current semaphore byte does not have bit 2 set, OR it with the bit-2 values from the selected neighbor blocks (using bitwise AND with 4). If the returned mask is 0, then clear the current block's semaphore byte entirely (set to 0). The function must correctly handle all combinations of motion vector components and boundary conditions.

#include <cassert>
#include <cstdint>

// Assume the function is already declared above

int main() {
    // Test 1: Interior, both MV components multiples of 16 (dx=32, dy=32)
    // mx=32, my=32 -> inside frame for width=64, height=64
    {
        uint8_t prev[4*4] = {0}; // 4x4 blocks, 64x64 pixels -> blocks_per_row=8? Actually 64/8=8 blocks per row
        // Let's set up 8x8 block grid for 64x64: 8 blocks per row, 8 rows
        uint8_t prev_frame[8*8] = {0};
        prev_frame[0] = 4; // block (0,0) has deringing flag
        prev_frame[1] = 4; // block (1,0)
        prev_frame[8] = 4; // block (0,1)
        prev_frame[9] = 4; // block (1,1)
        
        uint8_t current[4] = {0,0,0,0};
        int offsets[4] = {0, 1, 8, 9}; // for 8x8 blocks, but MB is 16x16 -> block offsets within MB: 0,1,8,9? Actually MB spans 2x2 blocks, offsets to each block's semaphore: block0 offset 0, block1 offset 1, block2 offset 8, block3 offset 9
        // But current_row is a pointer to the 4-byte array for MB, so offsets are 0,1,2,3? The original uses ll[kk] as stride between MB blocks. We'll use {0,1,2,3} for simplicity.
        int mb_offsets[4] = {0,1,2,3};
        
        uint8_t mask = propagate_deblock_flags(32, 32, current, prev_frame, mb_offsets, 32, 32, 8, 64, 64);
        assert(mask == 0x3);
        // All blocks should get deringing from prev_frame[0..1] and [8..9]
        // Since prev blocks have bit2, current blocks should have 4 set
        assert(current[0] == 4);
        assert(current[1] == 4);
        assert(current[2] == 4);
        assert(current[3] == 4);
    }
    
    // Test 2: Interior, dx not multiple, dy multiple
    {
        uint8_t prev_frame[8*8] = {0};
        prev_frame[0] = 4; // (0,0)
        prev_frame[1] = 0; // (1,0) no flag
        prev_frame[8] = 4; // (0,1)
        prev_frame[9] = 0; // (1,1)
        
        uint8_t current[4] = {0,0,0,0};
        int mb_offsets[4] = {0,1,2,3};
        // mx=32, my=32, dx=48 (not mult of 16), dy=32 (mult)
        uint8_t mask = propagate_deblock_flags(32, 32, current, prev_frame, mb_offsets, 48, 32, 8, 64, 64);
        assert(mask == 0x0);
        // For each block, pp_prev1 is at (2,2) block index? Actually mmvx=2, mmvy=2, block index 2+2*8=18
        // prev_frame[18] is 0, prev_frame[19] is 0, prev_frame[26] is 0, prev_frame[27] is 0
        // So deringing flags should be 0
        assert(current[0] == 0);
        assert(current[1] == 0);
        assert(current[2] == 0);
        assert(current[3] == 0);
    }
    
    // Test 3: Outside frame, clamping works
    {
        uint8_t prev_frame[8*8] = {0};
        prev_frame[0] = 4; // top-left block
        prev_frame[1] = 4;
        prev_frame[8] = 4;
        prev_frame[9] = 4;
        
        uint8_t current[4] = {0,0,0,0};
        int mb_offsets[4] = {0,1,2,3};
        // mx=-8, my=-8 (outside), dx=0, dy=0
        uint8_t mask = propagate_deblock_flags(-8, -8, current, prev_frame, mb_offsets, 0, 0, 8, 64, 64);
        // Clamped to block (0,0) for all sub-blocks
        assert(mask == 0x0);
        // All blocks should get deringing from (0,0) and neighbors? Since dx,dy mult of 16 -> pp_prev1=pp_prev2=pp_prev3=pp_prev4 all at (0,0)
        // All have 4, so current blocks should have 4
        assert(current[0] == 4);
        assert(current[1] == 4);
        assert(current[2] == 4);
        assert(current[3] == 4);
    }
    
    // Test 4: Interior, both not multiples, external blocks have flags
    {
        uint8_t prev_frame[8*8] = {0};
        // Set block (2,2) and (3,2) and (2,3) and (3,3) with flags
        prev_frame[2+2*8] = 4;
        prev_frame[3+2*8] = 4;
        prev_frame[2+3*8] = 4;
        prev_frame[3+3*8] = 4;
        
        uint8_t current[4] = {0,0,0,0};
        int mb_offsets[4] = {0,1,2,3};
        // mx=40, my=40 (inside for 64x64), dx=48, dy=48 (both not mult of 16)
        uint8_t mask = propagate_deblock_flags(40, 40, current, prev_frame, mb_offsets, 48, 48, 8, 64, 64);
        // mmvx=2, mmvy=2 -> block index 18
        // pp_prev1=prev[18], pp_prev2=prev[19], pp_prev3=prev[26], pp_prev4=prev[27]
        // All have 4, so current blocks get 4
        assert(mask == 0x0);
        assert(current[0] == 4);
        assert(current[1] == 4);
        assert(current[2] == 4);
        assert(current[3] == 4);
    }
    
    // Test 5: Current block already has deringing, should not be modified
    {
        uint8_t prev_frame[8*8] = {0};
        prev_frame[18] = 0; // no flag
        
        uint8_t current[4] = {4,0,0,0};
        int mb_offsets[4] = {0,1,2,3};
        // mx=32, my=32, dx=0, dy=0 (mult of 16)
        uint8_t mask = propagate_deblock_flags(32, 32, current, prev_frame, mb_offsets, 0, 0, 8, 64, 64);
        assert(mask == 0x3);
        // Block 0 already has 4, should remain 4
        assert(current[0] == 4);
        // Blocks 1-3 have no flags from prev, and mask is 0x3 so not cleared
        // They should remain 0
        assert(current[1] == 0);
        assert(current[2] == 0);
        assert(current[3] == 0);
    }
    
    return 0;
}

#include <cstdint>

// Returns deblocking mask (0x0 or 0x3) after propagating deringing/deblocking semaphores.
// mx, my: half-pixel coordinates of prediction point
// current_row: pointer to 4-byte semaphore array for current MB (one byte per 8x8 block)
// previous_frame: pointer to previous frame's block semaphore array (row-major, one byte per 8x8 block)
// block_offsets: array of 4 ints giving byte offset from current_row to each block's semaphore
// mv_dx, mv_dy: motion vector components in half-pixel units
// blocks_per_row: number of 8x8 blocks per row in previous_frame
// frame_width, frame_height: dimensions in pixels
uint8_t propagate_deblock_flags(
    int mx, int my,
    const uint8_t* current_row,
    const uint8_t* previous_frame,
    const int* block_offsets,
    int mv_dx, int mv_dy,
    int blocks_per_row,
    int frame_width, int frame_height)
{
    constexpr int MB_SIZE = 16; // pixels per macroblock side
    constexpr int BLOCK_SIZE = 8; // pixels per 8x8 block
    
    uint8_t msk_deblock = 0;
    int kk, mmvy, mmvx, nmvx, nmvy;
    const uint8_t *pp_prev1, *pp_prev2, *pp_prev3, *pp_prev4;
    
    // Check if prediction point is inside the valid motion search area
    if (mx >= 0 && mx <= ((frame_width << 1) - (2*MB_SIZE)) &&
        my >= 0 && my <= ((frame_height << 1) - (2*MB_SIZE)))
    {
        // Interior case: use MB-level block selection
        mmvx = mx >> 4;
        mmvy = my >> 4;
        pp_prev1 = previous_frame + mmvx + mmvy * blocks_per_row;
        
        if ((mv_dx & 0xF) != 0) {
            pp_prev2 = pp_prev1 + 1;
            if ((mv_dy & 0xF) != 0) {
                pp_prev3 = pp_prev1 + blocks_per_row;
            } else {
                pp_prev3 = pp_prev1;
            }
            pp_prev4 = pp_prev3 + 1;
        } else {
            pp_prev2 = pp_prev1;
            if ((mv_dy & 0xF) != 0) {
                pp_prev3 = pp_prev1 + blocks_per_row;
            } else {
                pp_prev3 = pp_prev1;
                msk_deblock = 0x3;
            }
            pp_prev4 = pp_prev3;
        }
        
        for (kk = 0; kk < 4; kk++) {
            uint8_t* cur = const_cast<uint8_t*>(current_row) + block_offsets[kk];
            if ((*cur & 4) == 0) {
                *cur |= ((*pp_prev1 | *pp_prev2 | *pp_prev3 | *pp_prev4) & 0x4);
            }
            if (msk_deblock == 0) {
                *cur = 0;
            }
            pp_prev1 += block_offsets[kk];
            pp_prev2 += block_offsets[kk];
            pp_prev3 += block_offsets[kk];
            pp_prev4 += block_offsets[kk];
        }
    }
    else
    {
        // Outside case: per-block handling with clamping
        int max_x_block = blocks_per_row - 1;
        int max_y_block = (frame_height >> 3) - 1;
        
        for (kk = 0; kk < 4; kk++) {
            mmvx = (mx + ((kk & 1) << 3)) >> 4;
            nmvx = mmvx;
            mmvy = (my + ((kk & 2) << 2)) >> 4;
            nmvy = mmvy;
            
            if (nmvx < 0) nmvx = 0;
            else if (nmvx > max_x_block) nmvx = max_x_block;
            if (nmvy < 0) nmvy = 0;
            else if (nmvy > max_y_block) nmvy = max_y_block;
            
            pp_prev1 = previous_frame + nmvx + nmvy * blocks_per_row;
            
            if (((mv_dx & 0xF) != 0) && (mmvx + 1 < blocks_per_row - 1)) {
                pp_prev2 = pp_prev1 + 1;
                if (((mv_dy & 0xF) != 0) && (mmvy + 1 < (frame_height >> 3) - 1)) {
                    pp_prev3 = pp_prev1 + blocks_per_row;
                    msk_deblock = 0x3;
                } else {
                    pp_prev3 = pp_prev1;
                }
                pp_prev4 = pp_prev3 + 1;
            } else {
                pp_prev2 = pp_prev1;
                if (((mv_dy & 0xF) != 0) && (mmvy + 1 < (frame_height >> 3) - 1)) {
                    pp_prev3 = pp_prev1 + blocks_per_row;
                } else {
                    pp_prev3 = pp_prev1;
                }
                pp_prev4 = pp_prev3;
            }
            
            uint8_t* cur = const_cast<uint8_t*>(current_row) + block_offsets[kk];
            if ((*cur & 4) == 0) {
                *cur |= ((*pp_prev1 | *pp_prev2 | *pp_prev3 | *pp_prev4) & 0x4);
            }
            if (msk_deblock == 0) {
                *cur = 0;
            }
        }
    }
    
    return msk_deblock;
}

// The solution requires careful simulation of the original algorithm. The core idea: determine whether the motion vector endpoint lies inside the legal prediction area. If inside, compute the base block indices `mmvx = mx>>4` and `mmvy = my>>4` (since MBs are 16x16 pixels, half-pixel units divide by 16 to get block index, as each block is 8x8 pixels; but the MB coordinate is at half-pixel resolution, so dividing by 16 gives the MB position, and each MB contains 2x2 blocks). The primary reference block is `pp_prev1 = previous_frame + mmvx + mmvy*blocks_per_row`. The neighbor selection depends on whether `mv_dx` and `mv_dy` are multiples of 16 (i.e., motion vector points exactly at a block boundary). If not multiple, the neighbor to the right (`pp_prev1+1`) is used; if dy not multiple, the neighbor below (`pp_prev1+blocks_per_row`) is used; otherwise the same block is reused. The fourth block `pp_prev4` is derived from `pp_prev3` similarly. For the outside case, each of the 4 sub-blocks in the current MB has its own predicted coordinate: for block index `kk` (0..3), the x offset is `(kk&1)<<3` (0 or 8 half-pixel units? Actually since mx is in half-pixel units, a block is 16 half-pixel units wide, so `(kk&1)<<3` gives 0 or 8? Wait: In the original, `mmvx = (xpred + ((kk & 1) << 3)) >> 4`. Here xpred is in half-pixel units, 16 half-pixels = one MB width (16 pixels), and 8 half-pixels = one 8-pixel block. So adding `(kk&1)<<3` (0 or 8) shifts by half a block, then `>>4` converts to block index. Similarly for y: `(kk&2)<<2` gives 0 or 8 (since kk&2 is 0 or 2, shift left by 2 gives 0 or 8).)` After computing mmvx,mmvy, we clamp them to valid block range (0 to blocks_per_row-1 for x, 0 to (frame_height/8)-1 for y). Then the same neighbor selection logic applies but with additional boundary checks: the right neighbor is only allowed if `mmvx+1 < blocks_per_row-1` (some off-by-one from original), and similarly for bottom. For each block, propagate the deringing bit (bit 2, value 4) from the selected previous blocks into the current block if the current doesn't already have it. Then if the deblocking mask is 0, set the current block's byte to 0. The function must preserve the mask logic: the mask becomes 0x3 only when both dx and dy are multiples of 16 in the interior case, or in certain outside cases when both are non-multiples but within bounds. Time complexity is O(1) constant operations, space O(1).

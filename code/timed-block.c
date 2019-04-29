#if !defined(TIMED_BLOCK_C)
#define TIMED_BLOCK_C

#include "defs.h"
#include "intrinsics.c"

typedef struct {
  char const* file_name;
  char const* function_name;
  u32 line_number;
  u64 volatile hit_count_top32_cycle_count_bottom32;
} TimedBlockRecord;

typedef struct {
  TimedBlockRecord* timed_block_record;
  u32 start_cycles;
} TimedBlock;

// IMPORTANT(Ryan): Must use unity build and place definition at the end of a translation unit.
// e.g. TimedBlockRecord translation_unit_timed_block_records[__COUNTER__];
extern TimedBlockRecord translation_unit_timed_block_records[];

void
end_timed_block(TimedBlock* timed_block) 
{
  u64 delta = (read_timestamp_counter() - timed_block->start_cycles) | ((u64)1 << 32);
  atomic_add_u64(&timed_block->timed_block_record->hit_count_top32_cycle_count_bottom32, delta);
}

#define TIMED_BLOCK(line_num) TIMED_BLOCK_(__LINE__)
#define TIMED_BLOCK_(line_num) TIMED_BLOCK__(line_num)
#define TIMED_BLOCK__(line_num) \
 __attribute__((unused, cleanup (end_timed_block))) TimedBlock timed_block##line_num = \
    ({ TimedBlockRecord* timed_block_record = &translation_unit_timed_block_records[__COUNTER__]; \
       timed_block_record->file_name = __FILE__; \
       timed_block_record->line_number = __LINE__; \
       timed_block_record->function_name = __func__; \
       (TimedBlock){timed_block_record, read_timestamp_counter()}; \
    })


#define RESET_AND_READ_TIMED_BLOCK_RECORDS(hit_count, cycle_count) \
  for (u64 counter = 0, hit_cycle_count = 0; \
        hit_cycle_count = atomic_exchange_u64(&translation_unit_timed_block_records[counter].hit_count_top32_cycle_count_bottom32, 0), \
        hit_count = (u32)(hit_cycle_count >> 32), cycle_count = (u32)(hit_cycle_count & 0xFFFFFFFF), \
        counter < ARRAY_COUNT(translation_unit_timed_block_records); \
         ++counter)

#endif

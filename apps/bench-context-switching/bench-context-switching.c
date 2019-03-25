#include "bench-context-switching.h"

/**
 * Struct that stores benchmarking information.
 * 
 * previous_id: The id of the previous thread that performs a ping;
 * new_id: The id of the current thread that has performed a ping;
 * current_time: the timer
 */
struct BContext
{
  uint32_t previous_id;
  uint32_t new_id;
  clock_time_t current_time;
} bench_context;

void bench_init()
{
    GPIO_SOFTWARE_CONTROL(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(2));
    GPIO_SET_OUTPUT(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(2));
}

void bench_on()
{
    GPIO_SET_PIN(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(2));
}

void bench_off()
{
    GPIO_CLR_PIN(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(2));
}

void bench_ping(uint32_t id)
{
  // Save the new id
  bench_context.new_id = id;
  // Save the current time
  // Check for switching context
  if (!check_change())
  {
    bench_context.current_time = clock_time(); // Ticks
  }
}

int check_change(void)
{
  if (bench_context.new_id != bench_context.previous_id)
  {
    // Compute the difference
    clock_time_t current = clock_time();
    clock_time_t previous = bench_context.current_time;
    clock_time_t result = current - previous;

    // Keep the previous id for log
    uint32_t previous_id = bench_context.previous_id;
    // Change previous_id to new_id
    bench_context.previous_id = bench_context.new_id;

    bench_context.current_time = current; // Ticks

    printf("[BENCH_CONTEXT_SWITCHING] %lu %lu %lu %d\n", previous_id, bench_context.new_id, result, CLOCK_SECOND);

    return 1; // Change occurs
  }
  return 0; // No change
}

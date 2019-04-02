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

void bench_init(uint32_t size)
{
    // Define the bench_context_switching_cache size
    bench_context_switching_cache_size = size;
    bench_context_switching_cache_index = 0;
    // Define the bench_context_switching_cache
    bench_context_switching_cache = malloc(sizeof(uint32_t) * bench_context_switching_cache_size);
    // Setup the GPIO
    GPIO_SOFTWARE_CONTROL(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(2));
    GPIO_SET_OUTPUT(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(2));
    // Start the benchmark
    printf("%s Ready\n", BENCH_CONTEXT_SWITCHING_FLAG);
}

int bench_check_data(char* data)
{
    int size = strlen(BENCH_CONTEXT_SWITCHING_FLAG) + strlen(" Ready\n") + 1;
    char str[size];
    sprintf(str, "%s Ready\n", BENCH_CONTEXT_SWITCHING_FLAG);
    return strcmp(str, data);
};

void bench_on(uint32_t pid)
{
    // Set the GPIO up
    GPIO_SET_PIN(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(2));

    // Store in the cache
    if (bench_context_switching_cache_index < bench_context_switching_cache_size)
    {
        bench_context_switching_cache[bench_context_switching_cache_index] = pid;
    } 
}

void bench_off()
{
    // Flush the bench_context_switching_cache
    bench_context_switching_cache_index++;
    if (bench_context_switching_cache_index == bench_context_switching_cache_size) {
        printf("%s Start %lu\n", BENCH_CONTEXT_SWITCHING_FLAG, bench_context_switching_cache_size);
        for(int i = 0; i < bench_context_switching_cache_size; i++)
        {
            printf("%lu\n", bench_context_switching_cache[i]);
        }
        bench_context_switching_cache_index = 0;
    }

    GPIO_CLR_PIN(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(2));
}

// XXX Delete this. Not used anymore.
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

    printf("%s %lu %lu %lu %d\n", BENCH_CONTEXT_SWITCHING_FLAG, previous_id, bench_context.new_id, result, CLOCK_SECOND);

    return 1; // Change occurs
  }
  return 0; // No change
}

#ifndef __BENCH_CONTEXT_SWITCHING_H__
#define __BENCH_CONTEXT_SWITCHING_H__

#include "contiki.h"
#include "board.h"
#include "sys/clock.h"
#include <stdio.h>

/**
 * Ping the benchmark.
 */
void bench_ping(uint32_t id);

/**
 * Check if there was a switching context.
 */
int check_change(void);

#endif
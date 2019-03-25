#ifndef __BENCH_CONTEXT_SWITCHING_H__
#define __BENCH_CONTEXT_SWITCHING_H__

#include "contiki.h"
#include "gpio.h"
#include "board.h"
#include "sys/clock.h"
#include <stdio.h>

/**
 * Init the framework with the GPIO.
 */
void bench_init();

/**
 * Set the GPIO on.
 */
void bench_on();

/**
 * Set the GPIO off.
 */
void bench_off();

/**
 * Ping the benchmark.
 */
void bench_ping(uint32_t id);

/**
 * Check if there was a switching context.
 */
int check_change(void);

#endif

/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "../benchmark.h"
#include "unity/unity.h"

static uint64_t overall = 0;

static void
add(uint64_t i)
{
    overall += i;
}

/**
 * reset returns all values used under test conditions
 * back to their 0 value for use in new tests.
 */
static void
reset()
{
    overall = 0;
}

/*
 * test_benchmark_one_thread validates that the behavior is
 * as expected operating in a single threaded execution
 * context.
 */
void
test_benchmark_one_thread(void)
{
    benchmark(10, 1, add);
    TEST_ASSERT_EQUAL_UINT64(45, overall);
    reset();
    return;
}

/*
 * test_benchmark_two_threads validates that the behavior is
 * as expected operating in a multi threaded execution
 * context.
 */
void
test_benchmark_two_threads(void)
{
    benchmark(10, 2, add);
    TEST_ASSERT_EQUAL_UINT64(70, overall);
    reset();
    return;
}

/*
 * test_benchmark_zero_ops validates the load frunction will
 * return -1 as an error if given 0 as it's ops count.
 */
void
test_benchmark_zero_ops(void)
{
    int res = benchmark(0, 1, add);
    TEST_ASSERT_EQUAL_INT(-1, res);
    reset();
    return;
}

/*
 * test_benchmark_zero_threads validates the load frunction will
 * return -1 as an error if given 0 as it's thread count.
 */
void
test_benchmark_zero_threads(void)
{
    int res = benchmark(1, 0, add);
    TEST_ASSERT_EQUAL_INT(-1, res);
    reset();
    return;
}

int
main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_benchmark_one_thread);
    RUN_TEST(test_benchmark_two_threads);
    RUN_TEST(test_benchmark_zero_ops);
    RUN_TEST(test_benchmark_zero_threads);

    return UNITY_END();
}

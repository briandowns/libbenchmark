/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Brian J. Downs
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

#include <locale.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>

#include "loads.h"

/**
 * function is a variable that is assigned to from the loads function
 * that contains the function passed in for benchmarking. It's defined
 * here so it can be accessed by the run function later on.
 */
static void (*function)(uint64_t iter);

/**
 * thread_args contains a start and end value that
 * indicate to the receiving thread a range of iterations
 * the thread is to execute the given function in.
 */
struct thread_args
{
    uint64_t start;
    uint64_t end;
};

/**
 * run is a function that is passed into a pthread for
 * execution. It receives a points with the number of
 * iterations it should perform in that given thread.
 */
void*
run(void* args)
{
    struct thread_args* ta = (struct thread_args*)args;
    for (uint64_t j = ta->start; j < ta->end; j++) {
        (function)(j);
    }
    pthread_exit(NULL);
}

int
loads(const uint64_t ops, const uint64_t thread_count, void (*f)(uint64_t iter))
{
    if (thread_count < 1) {
        return -1;
    }
    function = f;
    pthread_t* threads = malloc(sizeof(pthread_t) * thread_count);

    struct timeval start, end, result;

    // start the timer
    if (gettimeofday(&start, NULL) != 0) {
        return -1;
    }

    // try to evenly distribute the number of executions across
    // the given number of threads
    for (uint64_t j = 0; j < thread_count; j++) {
        uint64_t start = ops / thread_count * j;
        uint64_t end = ops / thread_count * (j + 1);

        if (j == (thread_count - 1)) {
            end = ops;
        }

        struct thread_args ta = { .start = start, .end = end };
        if (pthread_create(&threads[j], NULL, run, &ta) != 0) {
            return -1;
        }
    }

    // collect threa threads and wait for them all to complete
    for (uint64_t j = 0; j < thread_count; j++) {
        pthread_join(threads[j], NULL);
    }

    // free the memory allocated for the pthread array
    free(threads);

    // stop the timer
    if (gettimeofday(&end, NULL) != 0) {
        return -1;
    }
    timersub(&end, &start, &result);

    // print out the number of threads we're distributing the operations
    // across if more than one thread specified.
    char ss[24];
    if (thread_count > 1) {
        sprintf(ss, "over %llu threads ", thread_count);
    }

    // bring in the ability to use a single quote to format an integer with commas
    setlocale(LC_NUMERIC, "");

    printf("\n%'llu ops %sin %ld.%06ld sec, %0.2f/sec %0.2f ns/op\n",
           ops,
           (strlen(ss) > 0) ? ss : "",
           (long int)result.tv_sec,
           (long int)result.tv_usec,
           (float)ops / (float)result.tv_sec,
           (float)((float)result.tv_usec * 1000) / (float)ops);

    return 0;
}

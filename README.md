# libbenchmark

[![travis](https://img.shields.io/travis/briandowns/libbenchmark.svg)](https://travis-ci.org/briandowns/libbenchmark/)
[![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://opensource.org/licenses/BSD-2-Clause)
![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/briandowns/libbenchmark?style=plastic)

libbenchmark is a small library that provides a single function named `benchmark` to perform micro-benchmarks on a given function spread across a given number of threads. This library was heavily inspired by [Tidwall's](github.com/tidwall) [lotsa](github.com/tidwall/lotsa) Go package.

Operation:

`benchmark` takes 3 arguments. The first is the number of times you want the function executed, the second is the number of threads, and the third, the function you want executed. If either the number of operations or threads is less than 1 `-1` is returned indicating an error state. If the thread count is 1, execution will remain in the current process however if it's larger than 1, execution will be distributed across the threads evenly based on the number of operations given.

## Example 

```c
#include <stdint.h>
#include <stdio.h>

#include "benchmark.h"

void
fizz_buzz(uint64_t i)
{
    if (i % 15 == 0) {
        printf("fizzbuzz\n");
    } else if (i % 5 == 0) {
        printf("buzz\n");
    } else if (i % 3 == 0) {
        printf("fizz\n");
    } else {
        printf("%llu\n", i);
    }
}

int
main(int argc, char** argv)
{
    benchmark(100, 10, fizz_buzz);
    return 0;
}
```

Output: 

```sh
100 ops over 10 threads in 0.001524 sec, inf/sec 15240.00 ns/op
```

## Test

```sh
make test
```

## Installation

Installation is currently supported on Linux and MacOS. There will be an effort to get this installed on FreeBSD soon.

```sh
make install
```

## Contributing

Please feel free to open a PR!

## License

libbenchmark source code is available under the BSD 2 clause [License](/LICENSE).

## Contact

[@bdowns328](http://twitter.com/bdowns328)

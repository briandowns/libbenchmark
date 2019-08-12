# loads

loads is a small library that provides a single function named `loads` to perform micro-benchmarks on a given function, spread across a given number of threads. This library was heavily inspired by [Tidwall's](github.com/tidwall) [lotsa](github.com/tidwall/lotsa) Go package.

## Example 

```c
#include <stdint.h>
#include <stdio.h>

#include "loads.h"

void
fib(uint64_t i)
{
    if (i % 15 == 0) {
        printf("fizzbuzz\n");
    } else if (i % 5 == 0) {
        printf("fizz\n");
    } else if (i % 3 == 0) {
        printf("buzz\n");
    } else {
        printf("%llu\n", i);
    }
}

int
main(int argc, char** argv)
{
    loads(100, 10, fib);
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

loads source code is available under the BSD 2 clause [License](/LICENSE).

## Contact

[@bdowns328](http://twitter.com/bdowns328)

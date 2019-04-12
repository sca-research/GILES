# ELMO2

ELMO2 is designed to help software engineers quickly identify
[side channel](https://en.wikipedia.org/wiki/Side-channel_attack)
security risks during development stages.

This tool simulates instruction-level
[side channel analysis](https://en.wikipedia.org/wiki/Side-channel_attack)
leakage without the need for anything other that a target program by simulating
a processor and gathering leakage traces from this simulated processor.

It can support multiple different processors and multiple different methods of 
generating leakage from these.
Currently the only supported simulator is the
[Thumb Timing Simulator](https://github.com/bristol-sca/thumb-sim).
This will simulate an
[ARM Cortex M0 processor.](https://developer.arm.com/products/processors/cortex-m/cortex-m0)
The [supported leakage models can be found here.](#leakage-generation-models)

Please feel free to add more simulators and models!
There will be documents describing how to do this at some point.

<!-- toc -->

- [Usage](#usage)
- [All command line options](#all-command-line-options)
- [Leakage generation models](#leakage-generation-models)
  * [ELMO Power model](#elmo-power-model)
  * [Hamming weight model](#hamming-weight-model)
  * [Others](#others)
- [Output format](#output-format)
- [API Documentation](#api-documentation)
- [Getting started for development](#getting-started-for-development)
  * [Prerequisites](#prerequisites)
    + [Linux](#linux)
    + [MacOS](#macos)
    + [Windows](#windows)
  * [Building](#building)
  * [Running tests](#running-tests)
    + [Coverage information](#coverage-information)
- [CMake Configuration Options](#cmake-configuration-options)
  * [ELMO2_BUILD_DOCUMENTATION](#elmo2_build_documentation)
  * [ELMO2_CALCULATE_COVERAGE](#elmo2_calculate_coverage)
- [Built with](#built-with)
- [License](#license)

<!-- tocstop -->

## Usage

1) **[Download from the releases page](#releases)**
 🏗️  Coming soon! You'll have to [build it yourself](#building) for now, sorry.

2) **Compile your target program for your chosen simulator.**
Currently only one simulator is supported,
[Thumb Timing Simulator](https://github.com/bristol-sca/thumb-sim),
so compile for this simulator for now.
[Here is an example](https://github.com/bristol-sca/thumb-sim/tree/master/example)
to help you get started.

It is heavily recommended (but not essential) that you make use of the
`elmo-funcs.h` file to provide useful functions, such as trigger points and the
ability to get random data inside the simulator.


The target program should be edited to replace the target
data, such as the cryptographic key, with randomly generated values.

3) **Run ELMO2.**
Here is an example of the most common usage.
```
./ELMO-2 my-program-binary-from-step-2 -o output-file.trs
```
Note that this requires the file coeffs.json to be in the same directory.
This can be overridden like so:
```
./ELMO-2 my-program-binary-from-step-2 some-path/coeffs.json -o output-file.trs
```

4) Done! Perform whatever side channel attacks/analysis you want on the
[output file.](#output-format)

## All command line options
All command line options can be printed using the -h or --help flags shown here:
```
./ELMO-2 --help
```
This should produce the result below.
```
Side channel leakage emulation tool
Usage:
  bin/ELMO-2 [OPTION...] [--input] EXECUTABLE [--file] COEFFICIENTS

  -h, --help                    Print help
  -r, --runs N                  Number of traces to generate (default: 1)
  -f, --file COEFFICIENTS       Coefficients file (default: ./coeffs.json)
  -i, --input EXECUTABLE        Executable to be ran in the simulator
  -o, --output FILE             Generated traces output file
  -s, --simulator SIMULATOR NAME
                                The name of the simulator that should be used
                                (default: Andres)
  -m, --model MODEL NAME        The name of the mathematical model that
                                should be used to generate traces (default: Hamming
                                Weight)
```

## Leakage generation models

There are currently two methods supported for generating leakage supported.

### ELMO Power model

This is the recommend model for generating high quality leakage.

[Details of how it works can be found here.](https://www.usenix.org/conference/usenixsecurity17/technical-sessions/presentation/mccann)

### Hamming weight model

This is the default model and will generate leakage much faster but the
leakage may not be as high quality.

This works by taking the
[Hamming weight](https://en.wikipedia.org/wiki/Hamming_weight)
of the operands of the instructions executed.

### Others

Please help add more if you can!
There will be a document describing how to do this at some point.

## Output format

ELMO2 currently saves traces in the `.trs` format.
This format is designed for use in
[Riscure's Inspector](https://www.riscure.com/security-tools/inspector-sca/),
but can be interpreted in
[other ways](https://github.com/Riscure/python-trsfile).

We hope to support alternative formats in the future.

## API Documentation

Documentation is generated using
[Doxygen.](http://www.doxygen.nl/) In order to generate this
follow these instructions:

1) [**Install Doxygen**](http://www.doxygen.nl/download.html)

2) Firstly follow the instructions in the
[**Getting started for Development section.**](#getting-started-for-development)

3) Documentation can be built with CMake using the "doc" target by **running the
command as shown.**
```
cmake --build . --target doc
```

4) **Open this file**
```
/path/to/build/directory/doc/html/index.html
```

Additionally, enabling the cmake configuration option
`ELMO2_BUILD_DOCUMENTATION` will generate the documentation every
time you compile.

## Getting started for development

### Prerequisites

This uses [CMake.](https://cmake.org/) As such, CMake needs to be
installed first.

#### Linux

The recommended way of installing CMake is through your package manager.

If a CMake package isn't available on your Linux distribution follow the
[official CMake install instructions.](https://cmake.org/install/)

#### MacOS

CMake can be installed using [Homebrew](https://brew.sh/) with the following
command.
```
brew install cmake
```
Alternatively, follow the
[official CMake install instructions.](https://cmake.org/install/)

#### Windows

Follow the [official CMake install instructions.](https://cmake.org/install/)

### Building

1) **Create an empty build directory.**

2) **From the build directory run this command.** This will generate native
build files for your platform. A specific generator can be specified using the
`-G *generator*` flag.
A [list of generators is available here.](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
```
cmake /path/to/source/directory/
```

3) **Run this command to compile the code.** This will compile the program using
your chosen generator e.g. make. Alternatively the native build system can be
used manually from this point on.
```
cmake --build .
```

### Running tests

1) Firstly follow the instructions in the
[**Getting started for Development section.**](#getting-started-for-development)

2) **Build the tests with this command.** This will tell the native build system
to build the target called tests.
```
cmake --build . --target tests
```
3) **Run the tests**
```
/path/to/build/directory/output/tests
```

#### Coverage information

In order to generate code coverage information, [Gcovr](https://gcovr.com/) is
required.

1) **Regenerate the build files** using the option [shown below](#ELMO2_calculate_coverage) from the build
directory. This is needed as specific compile flags need to be added in order to
generate coverage information.
```
cmake /path/to/source/directory/traces_serialiser -DELMO2_CALCULATE_COVERAGE=ON
```

2) **Run this command.** This will tell the native build system to build the
target called coverage.
```
cmake --build . --target coverage
```
Coverage information can be found by opening the file `coverage.html` in the
folder `coverage` which should have been created in your *`build directory`*.
```
/path/to/build/directory/coverage/coverage.html
```

## CMake Configuration Options

These are all CMake options and can be appended to the CMake generate command.
For example, from the build directory:
```
cmake /path/to/source/directory/ -DELMO2_BUILD_DOCUMENTATION
```

### ELMO2_BUILD_DOCUMENTATION

This option will always built the "doc" target when the target "all" is built.
See [API Documentation section.](#api-documentation)

### ELMO2_CALCULATE_COVERAGE

This is needed to generate code coverage information.
See [Coverage information](#coverage-information) for details on how to do this.

**⚠️ Warning ⚠️
This will enable compile flags that are not recommended for normal
purposes. Set this to OFF after you are done generating coverage information.**

## Built with

- C++
- [CMake](https://cmake.org/)
- [JSON for modern C++](https://github.com/nlohmann/json)
- [Catch2](https://github.com/catchorg/Catch2)
- [Traces Serailiser](https://github.com/bristol-sca/Traces-Serialiser)
- [Thumb Timing Simulator](https://github.com/bristol-sca/thumb-sim)
- [Boost](https://www.boost.org/)
- [Gcovr](https://gcovr.com/)
- [Doxygen](http://www.doxygen.nl/)
- [markdown-toc](https://github.com/jonschlinkert/markdown-toc)

Thank you to the creators of all these.
Without these, this would not have been possible.

## License
This program is released under license AGPLv3+.

Get a summary of this license
[here at choosealicense.com](https://choosealicense.com/licenses/agpl-3.0/)

## Acknowledgement
This project has received funding from the European Research Council (ERC) under the European Union’s
Horizon 2020 research and innovation programme (grant agreement No 725042).

![EU Logo](https://github.com/adam-p/markdown-here/raw/master/src/common/images/icon48.png "ERC")

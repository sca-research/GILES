

<!-- toc -->

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

<!-- tocstop -->

## Getting started for development

### Prerequisites

This uses [CMake.](https://cmake.org/) As such, CMake needs to be
installed first.
Additionally, [Boost](https://www.boost.org/) needs to be installed manually.

#### Linux

The recommended way of installing CMake and Boost is through your package
manager.

If either package isn't available on your Linux distribution follow the
[official CMake install instructions](https://cmake.org/install/)
or the [official boost instructions.](https://www.boost.org/users/download/)

#### MacOS

CMake and boost can be installed using [Homebrew](https://brew.sh/) with the
following command.
```
brew install cmake boost-python
```
Alternatively, follow the
[official CMake install instructions](https://cmake.org/install/)
and the [official boost instructions.](https://www.boost.org/users/download/)

#### Windows

Follow the [official CMake install instructions](https://cmake.org/install/)
and the [official boost instructions.](https://www.boost.org/users/download/)

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

# GILES

GILES is designed to help software engineers quickly identify
[side channel](https://en.wikipedia.org/wiki/Side-channel_attack) and
[fault injection](https://en.wikipedia.org/wiki/Fault_injection)
security risks during development stages.

<!-- toc -->

- [Motivation](#motivation)
- [Description](#description)
- [Usage](#usage)
- [All command line options](#all-command-line-options)
- [Leakage generation models](#leakage-generation-models)
  * [ELMO Power model](#elmo-power-model)
  * [Hamming weight model](#hamming-weight-model)
  * [Others](#others)
- [Output format](#output-format)
- [API Documentation](#api-documentation)
- [Building](#building)
- [Built with](#built-with)
- [License](#license)
- [Acknowledgement](#acknowledgement)

<!-- tocstop -->

## Motivation

Developing software for leaky processors is not straightforward and even
experienced developers need to check if the measures that they put in place 
actually work. Rather than having to go through the onerous task of a fully
blown lab evaluation, it would be great if a "push button" solution could be
run over the code that identifies leaks and maybe even helps fix them.
GILES includes instruction level power profiles
(taken on our high lab equipment) and "plugs them into" a simulator,
thereby providing an instruction accurate leakage emulator.

## Description

This tool simulates instruction-level
[side channel analysis](https://en.wikipedia.org/wiki/Side-channel_attack)
leakage without the need for anything other that a target program.
Additionally it allows for fault injection attacks to be performed concurrently.

It can support multiple different processors and multiple different methods of 
generating leakage from these.
Currently the only supported simulator is the
[Thumb Timing Simulator](https://github.com/bristol-sca/thumb-sim).
This will simulate an
[ARM Cortex M0 processor.](https://developer.arm.com/products/processors/cortex-m/cortex-m0)
The [supported leakage models can be found here.](#leakage-generation-models)

Please feel free to add more simulators and models!
There will be documents describing how to do this at some point.

## Usage

1) **[Download from the releases page](https://github.com/bristol-sca/GILES/releases)**
Alternatively, [build it yourself](#building).

2) **Compile your target program for your chosen simulator.**
Currently only one simulator is supported,
[Thumb Timing Simulator](https://github.com/bristol-sca/thumb-sim),
so compile for this simulator for now.
[Here is an example](https://github.com/bristol-sca/thumb-sim/tree/master/example)
to help you get started.

It is heavily recommended (but not essential) that you make use of the
`elmo-funcs.h` file to provide useful functions, such as trigger points and the
ability to get random data inside the simulator. This can be 
[found on the releases page.](https://github.com/bristol-sca/GILES/releases)


The target program should be edited to replace the target
data, such as the cryptographic key, with randomly generated values.

3) **Run GILES.**
Here is an example of the most common usage.
```
GILES my-program-binary-from-step-2 -o output-file.trs
```
This will use the Hamming weight model.
[See here for more on models.](#leakage-generation-models)
The ELMO power model requires the file coeffs.json to be in the same directory 
or specified like so:
```
GILES my-program-binary-from-step-2 --coefficients some-path/coeffs.json -o output-file.trs
```

4) Done! Perform whatever side channel attacks/analysis you want on the
[output file.](#output-format)

## All command line options
All command line options can be printed using the -h or --help flags shown here:
```
./GILES --help
```
This should produce the result below.
```
General instruction leakage simulator
Usage: bin/GILES [--input] EXECUTABLE [--coefficients] COEFFICIENTS
:
  -h [ --help ]                         Print help
  -r [ --runs ] arg (=1)                Number of traces to generate
  -c [ --coefficients ] arg (=./coeffs.json)
                                        Coefficients file
  -i [ --input ] arg                    Executable to be ran in the simulator
  -o [ --output ] arg                   Generated traces output file
  -s [ --simulator ] arg (=Thumb Sim)   The name of the simulator that should 
                                        be used
  -m [ --model ] arg (=Hamming Weight)  The name of the mathematical model that
                                        should be used to generate traces
  -f [ --fault ] arg                    Where to inject a fault. e.g. "--fault 
                                        10 R0 2" is inject a fault before the 
                                        10th clock cycle, by flipping the 
                                        second least significant bit in the 
                                        register R0
  -t [ --timeout ] arg                  The number of clock cycles to force 
                                        stop execution after
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

GILES currently saves traces in the `.trs` format.
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
[**Getting started for Development section.**](BUILDING.md#getting-started-for-development)

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
`GILES_BUILD_DOCUMENTATION` will generate the documentation every
time you compile.

## Building

Refer to [BUILDING.md](BUILDING.md) for developer specific information.

## Built with

- C++
- [CMake](https://cmake.org/)
- [JSON for modern C++](https://github.com/nlohmann/json)
- [OpenMP](https://www.openmp.org)
- [Catch2](https://github.com/catchorg/Catch2)
- [Traces Serailiser](https://github.com/bristol-sca/Traces-Serialiser)
- [Thumb Timing Simulator](https://github.com/bristol-sca/thumb-sim)
- [Boost](https://www.boost.org/)
- [{fmt}](https://github.com/fmtlib/fmt)
- [Gcovr](https://gcovr.com/)
- [Doxygen](http://www.doxygen.nl/)
- [markdown-toc](https://github.com/jonschlinkert/markdown-toc)
- [LinuxDeploy](https://github.com/linuxdeploy/linuxdeploy)

Thank you to the creators of all these.
Without these, this would not have been possible.

## License
This program is released under license AGPLv3+.

Get a summary of this license
[here at choosealicense.com](https://choosealicense.com/licenses/agpl-3.0/)

## Acknowledgement
This project has received funding from the European Research Council (ERC) under the European Union’s
Horizon 2020 research and innovation programme (grant agreement No 725042).

![EU Logo](https://github.com/bristol-sca/GILES/blob/master/LOGO_ERC-FLAG_EU.jpg "ERC")

# All command line options
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

<!-- toc -->

- [--runs/-r](#--runs-r)
- [--coefficients/-c](#--coefficients-c)
- [--input/-i](#--input-i)
- [--output/-o](#--output-o)
- [--simulator/-s](#--simulator-s)
- [--model/-m](#--model-m)
- [--fault/-f](#--fault-f)
- [--timeout/-t](#--timeout-t)

<!-- tocstop -->

## --runs/-r

This controls the number of times the target program will run within the 
simulator. When generating traces, this consequently controls the number of 
traces generated.

This is an optional argument and will default to 1 run.

## --coefficients/-c

Some models such as the ELMO power model require a coefficients file.

This option indicates the path to this file. This is an optional argument and 
if not specified, GILES will look for the file coeffs.json in the currently 
directory and use that. If that is not found then it will continue without 
coefficients as they may not be needed.

## --input/-i

This indicates the path to the target program to be run within the simulator.

This is the only option that isn't optional and GILES will not run without it.

## --output/-o

The path to save generated traces to. This is optional but if not specified a 
warning will be displayed as without this flag, it is possible to have long 
running operations producing no output.

## --simulator/-s

This option can be ignored for now.

The name of the simulator to use. Currently the only supported simulator is the
[Thumb Timing Simulator](https://github.com/bristol-sca/thumb-sim) and this is 
the default option.

## --model/-m

The name of the model to use. Valid names are shown under the 
[Leakage generation models section in the README.](README.md#leakage-generation-models)

If not specified, this will default to "Hamming Weight".

## --fault/-f

This option is used to inject faults. It required three arguments. For example:
`--fault 10 R0 2" is inject a fault before the 10th clock cycle, by flipping the 
second least significant bit in the register R0.

If not specificed, no faults will be injected.

## --timeout/-t

This option is to stop execution after a set number of clock cycles have passed.
This is designed to prevent infinite loops.

If not specificed, no limit will be applied.

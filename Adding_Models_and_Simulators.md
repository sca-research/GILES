

<!-- toc -->

- [Adding new Models](#adding-new-models)
  * [Copy the folder](#copy-the-folder)
  * [Rename all references to TEMPLATE to your new model name](#rename-all-references-to-template-to-your-new-model-name)
  * [Update m_required_terms](#update-m_required_terms)
  * [Implement the function Generate_Traces() (At the bottom of the cpp file)](#implement-the-function-generate_traces-at-the-bottom-of-the-cpp-file)
  * [Add the cpp file to the cmake build](#add-the-cpp-file-to-the-cmake-build)
- [Adding new Simulators](#adding-new-simulators)
  * [Copy the folder](#copy-the-folder-1)
  * [Rename all references to TEMPLATE to your new simulator name](#rename-all-references-to-template-to-your-new-simulator-name)
  * [In the cpp file, **implement the Run_Code() function**](#in-the-cpp-file-implement-the-run_code-function)
  * [Add the cpp file to the cmake build](#add-the-cpp-file-to-the-cmake-build-1)
  * [Optional steps](#optional-steps)
    + [Implement the remaining functions in the cpp file](#implement-the-remaining-functions-in-the-cpp-file)
      - [Implement the Get_Extra_Data() function](#implement-the-get_extra_data-function)
      - [Implement the Inject_Fault() function](#implement-the-inject_fault-function)
      - [Implement the Add_Timeout() function](#implement-the-add_timeout-function)
    + [Implement the functions in elmo-funcs.h](#implement-the-functions-in-elmo-funcsh)
      - [start_trigger()/pause_trigger()](#start_triggerpause_trigger)
      - [get_rand()](#get_rand)
      - [add_byte_to_trace()/add_to_trace()](#add_byte_to_traceadd_to_trace)

<!-- tocstop -->

# Adding new Models

## Copy the folder 
Copy `src/Models/TEMPLATE` to `src/Models/*your-new-model-name*`

## Rename all references to TEMPLATE to your new model name
Update the comments to match this.

## Update m_required_terms
In the cpp file, update m_required_terms to contain a list of all the terms 
your model requires to be loaded from a coefficients file.

**If your Model does not require anything to be loaded** make this an empty
list.

## Implement the function Generate_Traces() (At the bottom of the cpp file)

This function will generate the traces for you. m_execution and m_coefficients 
should contain the data you need. To understand more about these, look at the 
Execution and Coefficients class in the 
[API Documentation.](README.md#api-documentation)

## Add the cpp file to the cmake build
This is done in the file `src/CMakeLists.txt`. The TEMPLATE file is listed in 
here, but commented out. This one line is exactly how your new model needs to 
be added to the build.

# Adding new Simulators

This is slightly harder than adding a new model but largely a similar 
process.

## Copy the folder 
Copy `src/Simulators/TEMPLATE` to `src/Simulators/*your-new-simulator-name*`

## Rename all references to TEMPLATE to your new simulator name
Update the comments to match this.

## In the cpp file, **implement the Run_Code() function**
m_program_path contains the path to the target program. This needs to be loaded 
and ran on whichever simulator is being integrated.

This function needs to return a complete Execution object. This will contain 
a complete recording of the state of all registers and pipeline stages 
(Fetch, Decode, Execute, etc) during every clock cycle. This is the information 
that will later be used to generate traces from.

The simulator may need to be edited in order to record this information.
Take a look at the Execution class in the 
[API Documentation](README.md#api-documentation) for details about what needs 
to be made.

## Add the cpp file to the cmake build
This is done in the file `src/CMakeLists.txt`. The TEMPLATE file is listed in 
here, but commented out. This one line is exactly how your new simulator needs 
to be added to the build.

## Optional steps

**The rest of the steps are optional but highly recommended.** It is very likely
that the rest of the steps will require custom edits to the simulator.

### Implement the remaining functions in the cpp file
These can be left as stubs and these features will be simply unavailable.

The implementation of these functions will be specific to the simulator and may 
require editing the simulators internals.

#### Implement the Get_Extra_Data() function

This requires the [add_to_trace()](#implement-the-get_extra_data-function) 
function to be implemented.

When called, it will return the data stored by the 
[add_to_trace() function.](#implement-the-get_extra_data-function) 

#### Implement the Add_Timeout() function

This function should add a timeout so that the simulator will stop executing 
the target program after the number of cycles given by the parameter have 
passed.

It should still be run through the use of Run_Code() and 
should return normally. It is recommended to print a message indicating that the 
timeout has been reached.

#### Implement the Inject_Fault() function

This function should add a trigger so that after a set number of cycles have 
passed a bit flip should occur within one of the registers. The details of this 
are provided in parameters.

It should still be run through the use of Run_Code() and 
should return normally.

### Implement the functions in elmo-funcs.h

These functions allow special operations to be performed on the simulator from 
within the target program.

#### start_trigger()/pause_trigger()

These functions when called will start or pause the recording of the execution 
of the target program. This is only useful when generating traces and can be 
used to profile a subsection of the target program.

#### get_rand()

This function should simply return a random number. This is useful as often 
simulators do not contain a source of randomness. This is not needed if another 
source of randomness is available.

#### add_byte_to_trace()/add_to_trace()

Extra data can be added to each trace in a trs file. This data can be anything 
that will be useful to record, e.g. the plain text or key of a cryptographic 
algorithm.

This function should place this data somewhere to be retrieved later by the 
[Get_Extra_Data() function created earlier.](#implement-the-get_extra_data-function)

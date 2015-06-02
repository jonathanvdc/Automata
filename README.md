# Automata
A simple finite automata library.

## Usage
This automata library comes with a simple command-line interface.
After compiling all `*.cpp` files in `project/Automata`, it can be used to 
perform the algorithms as listed below, and can be consulted for its usage.

## Functionality
This library implements the following algorithms:
 * subset construction (ssc)
 * modified subset construction (mssc)
 * regex to e-nfa conversion
 * dfa to regex conversion
 * table-filling algorithm (tfa)
 
## Tests
In addition to the manual tests for the library's functionality, automated tests
have also been included for the ssc, mscc and regex to e-nfa functionality.

To run said tests, go to the `tests\auto` folder and run 

    mkdir tests
    AutoTests.exe test nfa ..\..\project\Automata\a.exe
    AutoTests.exe test enfa ..\..\project\Automata\a.exe
    AutoTests.exe test regex ..\..\project\Automata\a.exe
    
(where `a.exe` is the executable obtained by compiling the automata library)

The nfa tests compare executing an nfa to the corresponding dfa, as do the e-nfa tests.
The regex tests compare the compiled regexes to both the resulting dfa and e-nfa, as well as
a reference implementation: the .Net regex engine.

The automated tests require for the .Net framework 4.5 to be present.
A precompiled version of AutoTests.exe is included with the source code, which was compiled
with [dsc](https://github.com/jonathanvdc/Flame/releases).

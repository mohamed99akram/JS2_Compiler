# To run it
- install bison, flex, add to environment variables if windows
- run: `make build` to build the program
- run `make run` to run code existing in `test.js` file
- mingw32-make build_compiler
- ./compiler < test_files/switch.js
# Files
- This program is a compiler for a language that looks like JavaScript.
- lexer.l is the lexer file (input: code file, output: tokens)
- parser.y is the parser file (input: tokens, output: parse tree)
- interpreter.c is the interpreter file (input: parse tree, output: output of program)
- parser.h is the header file for the parser, it contains data structures

# Outputs
- The outputs of the program are printed to the console.

- and there are output log files:
    - calls.log: tells if a statement is used, parsed and executed
    - symbol_table.log: tells the symbol table of the program (declared variables and their values)
    - nodes.log: contains tree of nodes created, nodes can be value / variable or operation
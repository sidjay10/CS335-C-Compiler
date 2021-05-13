# Compiler Design

This repository contains our project for CS335, the course on Compiler Design at IIT Kanpur for the Spring semester 2021.

We build the compiler in stages. Currently, we have a functional compiler that can translate C source code to MIPS target and can be run on `spim` simulator or android devices/routers that have MIPS CPUs.

This project supports translation for a subset of the entire C [grammar](src/grammar.y)

### Basic features supported:

1. Native Data types (integer, boolean, character).  
2. Variables and Expressions.
3. Control structures: 
    - Conditionals (switch,if, if-then-else)
    - Loops (for, while,do-while). 
    - Switch case 
4. Input/Output statements.
5. Arrays (all native types)
6. Functions along with Recursion
7. User defined types (struct, union)
8. Pointers 
9. Simple library functions, such as
    - Integer math functions
    - string functions

### Advanced features supported

1. Multi Level Pointers
2. N-D Array
3. File I/O
4. Dynamic Memory Allocation
5. Code Optimizations: 
    - Dead Code Elimination , Constant folding, Short Circuit Evaluation


## Building the Project 

To build the project, simply run :

```
make
```

To clear the binaries and compiled source : 

```
make clean
```

## Usage

To use the compiler, you must first build it (as above) to generate the binary `./bin/compiler`. Our compiler has a basic CLI and `./bin/compiler --help` should give you usage information. You should specify the input source file using `--input | -i` and optionally specify the output filename as `--output | -o `. 

tl;dr :
```
./bin/compiler -i <src>.c -o <dest>.s
```

If you want intermediate files generated like Three Address Code or additional information like symbol table, there are flags in the CLI that can help you get then. Refer to `--help` for the same.

```
usage: ./bin/compiler --input=string [options] ... 
options:
      --dot-cfg    Generate AST in dot format
      --symtab     Generate the Symbol Table in csv format
      --tac        Generate three address code intermediate
  -v, --all        Verbose generation of all intermediate files/information
  -o, --output     Output filename for the generated assembly. (string [=file.s])
  -i, --input      Specify the input source file (string)
  -?, --help       print this message
```

### Stage Information

## Milestone 1 (Lexer)

This binary lexes the input C file and outputs information of each idenitfied lexeme along with location information.
Usage :

```
git checkout lexer
make 
./bin/scanner <path_to_c_file>
```

## Milestone 2 (Parser)

This binary parses the input C file and generates an Abstract-Syntax-Tree as a `.dot` file. This can either be converted to a different viewing format like `.pdf` or `.jpg` or `.ps`, or it can be viewed using a `.dot` file viewer like `xdot`.

Usage: 
```
git chcekout parser
make
./bin/parser <path_to_c_file> -o <file_name>.dot
```
To view the AST : 
```
xdot <file_name>.dot
```
To generate other file formats (e.g., postscript):

```
dot -Tps <file_name>.dot -o <file_name>.ps
```

## Milestone 3 (Semantic Checking)

This binary also parses the input C file, generates an Abstract-Syntax-Tree as a `.dot` file. Apart from the AST generation, the target at this stage also uses grammar actions to check the program for semantic ambiguities/errors. It performs type checking for expressions etc, and complains on illegal instructions like multiplying pointers etc. At this stage, along with semantic checking we also generate the Symbol Table (local and global) for the source program in a file `symtab.csv`.

Usage: 
```
git chcekout semantics
make
./bin/symtab <path_to_c_file> -o <file_name>.dot
```

## Final Project

The final stage of this project is a functional compiler to translate C source to MIPS and run it. Tip-of-Tree of the `master` branch has code for the final outcome of the project. Usage information is provided above.
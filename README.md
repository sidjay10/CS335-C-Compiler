# Compiler Design

This repository contains our project for CS335, the course on Compiler Design at IIT Kanpur for the Spring semester 2021.

We build the compiler in stages.

## Building the Project 

To build the project, simply run :

```
make
```

To clear the binaries and compiled source : 

```
make clean
```

### Usage Instructions

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
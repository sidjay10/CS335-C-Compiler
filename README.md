# Compiler Design

This repository contains our project for CS335, the course on Compiler Design at IIT Kanpur for the Spring semester 2021.

We build the compiler in stages, with support for building any of the intermediate binaries.

## To build the project :
Use "make" in the code repository: 
make

In general, to build any specific target binary : 
make <target>

To clear the binaries and compiled source : 
make clean

## Milestone 2 (Parser)

Usage: 
./bin/parser <path_to_c_file> -o <file_name>.dot

AST : 
dot -Tps <file_name>.dot -o <file_name>.ps

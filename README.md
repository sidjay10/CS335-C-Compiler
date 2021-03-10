# Compiler Design

This repository contains our project for CS335, the course on Compiler Design at IIT Kanpur for the Spring semester 2021.

We build the compiler in stages, with support for building any of the intermediate binaries.

To build the project :
make

In general, to build any specific target binary :
make <target>

To clear the binaries and compiled source :
make clean

Usage for Milestone 1:
./bin/scanner <path_to_file>

Usage for Milestone 2:
./bin/parser <path_to_file> -o <file_name>.dot


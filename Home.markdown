Ixion is a general purpose formula parser & interpreter that can calculate multiple named targets, or "cells".

# Overview
The goal of this project is to create a library for calculating the results of formula expressions stored in multiple named targets, or "cells".  The cells can be referenced from each other, and the library takes care of resolving their dependencies automatically upon calculation.  The caller can run the calculation routine either in a single-threaded mode, or a multi-threaded mode.  The library also supports re-calculations where the contents of one or more cells have been modified since the last calculation, and a partial calculation of only the affected cells need to be calculated.

# Portability
This library is written with portability in mind; platform specific calls are avoided as much as possible.  It makes extensive use of the [boost library](http://boost.org) to achieve portability.  However, the library does use some of the upcoming C++0x (or TR1) features, which may make it difficult to build it with older compilers that provide no support for C++0x.

# Performance
Achieving good performance is one of the goals of this project.  As much care is taken as humanly possible, to attain reasonable performance.

# Open issues
* Command line options are currently not working on Windows.  Use [Boost.Program_options](http://www.boost.org/doc/libs/1_43_0/doc/html/program_options.html) instead of [GNU getopt](http://www.gnu.org/s/libc/manual/html_node/Getopt.html) to fix this.
* Add support for empty cells, which are not defined but can still be referenced.
* Add support for cell ranges.
* Currently, this project only produces a stand-alone console program.  Split that into a shared library and a front-end executable that links to the library.
* Use autoconf to configure platform specific parameters.  Right now, there is only one Makefile where every parameter is hard-coded.
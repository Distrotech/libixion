Ixion is a general purpose formula parser & interpreter that can calculate multiple named targets, or "cells".

## Overview
The goal of this project is to create a library for calculating the results of formula expressions stored in multiple named targets, or "cells".  The cells can be referenced from each other, and the library takes care of resolving their dependencies automatically upon calculation.  The caller can run the calculation routine either in a single-threaded mode, or a multi-threaded mode.  The library also supports re-calculations where the contents of one or more cells have been modified since the last calculation, and a partial calculation of only the affected cells need to be calculated.

## Portability
This library is written with portability in mind; platform specific calls are avoided as much as possible.  It makes extensive use of the [boost library](http://boost.org) to achieve portability.  However, the library does use some of the upcoming C++0x (or TR1) features, which may make it difficult to build it with older compilers that provide no support for C++0x.

## Performance
Achieving good performance is one of the goals of this project.  As much care is taken as humanly possible, to attain reasonable performance.

## Threaded calculation
Ixion can perform threaded calculation using arbitrary number of threads, for both full and partial calculation modes.

## Features
* Fully threaded calculation.
* Name resolution using A1 naming convention.
* Support 2D cell references and named expressions.
* Dependency tracking during both full calculation and partial re-calculation.

## Ongoing work
* Resolve formula names during formula token construction and classify them into cell references, cell range references, named ranges etc.  This implicitly adds support for empty cells, as cell's validity is inferred from the name itself even if that cell itself is undefined in the model definition.  This is also where cell ranges are picked up.
* Add support for cell ranges.
* Add support for 3D cell references.
* More built-in functions.


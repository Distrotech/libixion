Ixion is a general purpose formula parser & interpreter that works on multiple calculation targets, or "cells".  It can be run in a single-thread mode or multi-thread mode.  When running in a multi-thread mode, the user can specify the number of threads to use for calculation of cells.

# Goal
The goal of this project is to create a library for calculating the results of formula expressions stored in multiple named targets, or "cells".  The cells can be referenced from each other, and the library takes care of resolving their dependencies automatically upon calculation.

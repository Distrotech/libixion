# Introduction
One of the goals of designing the cell calculation framework in Ixion is to allow concurrent execution of multiple formula cells while satisfying correct ordering per cell dependencies.  The framework should also allow a non-parallel execution of cell calculation if the caller so desires.  In parallel execution mode, the caller should be able to specify the number of threads to use at run-time.

# Main execution flow
1. Run topological sort on all formula cells in order to obtain the order of cell interpretation.
2. Reset the states of all formula cells: clear errors, and reset cached cell results.
3. Using a single thread, perform pre-computation on all cells in topological order and mark circular dependent cells with appropriate error conditions.  Pre-computation only involves skimming through cell reference tokens while ignoring all the other tokens.
4. Now, perform full interpretation of cells in topological order using variable number of threads.  Skip those cells marked with error conditions.  During this phase, block on any cells whose results are not yet computed, using thread condition variables.

# Open questions
* The current algorithm requires the whole formula cell list be scanned twice; first to detect circular dependencies and second to perform full computation.  Are there any performance implication for doing this?  Is there any better algorithm for detecting circular dependencies?

# References
## Threaded programming in C++
* [Boost Thread](http://www.boost.org/doc/libs/1_43_0/doc/html/thread.html) - Boost Thread library documentation.
* [Multithreading and Concurrency](http://www.justsoftwaresolutions.co.uk/threading/) by Anthony Williams
* [Thread Interruption in the Boost Thread Library](http://www.justsoftwaresolutions.co.uk/threading/thread-interruption-in-boost-thread-library.html) by Anthony Williams - focuses on how thread interruption works using Boost.Thread library.
* [Enforcing Correct Mutex Usage with Synchronized Values](http://www.drdobbs.com/cpp/225200269) by Anthony Williams, Dr. Dobb's.
# Introduction
One of the goals of designing the cell calculation framework in Ixion is to allow concurrent execution of multiple formula cells while satisfying correct ordering due to cell dependencies.  The framework should also allow a non-parallel execution of cell calculation if the caller so desires.  In parallel execution mode, the caller should be able to specify the number of threads to use at run-time.

# Reference
* [Thread Interruption in the Boost Thread Library](http://www.justsoftwaresolutions.co.uk/threading/thread-interruption-in-boost-thread-library.html) by Anthony Williams focuses on how thread interruption works using Boost.Thread library.
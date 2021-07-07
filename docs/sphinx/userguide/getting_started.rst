

Getting Started with NW Graph
=============================


Overview of the Library
-----------------------



.. Quickstart
.. ----------



Requirements
------------



Compiler
~~~~~~~~

NWGraph requires modern (as of this writing) C++ language support--C++20.  Compilers
that support C++20 and that have been tested with NW Graph include gcc 10.1 and above
and clang 11.0.0 and above.  When using these compilers, make sure to pass the
appropriate flag to set the language level (usually -std=c++20).



Build Tools
~~~~~~~~~~~

Building the tests, benchmarks, and examples that come with NWGraph requires cmake 3.16 or higher.


Other Libraries
~~~~~~~~~~~~~~~

NW Graph requires several external libraries.  For its own execution, it requires
Intel's Threaded Building Blocks (TBB).  You should install this with the appropriate
package manager for your system so that TBB can be found when compiling any programs
using NW Graph.



Installation
------------

NW Graph is a header-only library, so installation means copying the NW Graph include
hierarchy to a place where the compiler can find it, either by copying it to a system
include location such as /usr/local/include, or to some arbitrary location and
pointing the compiler to it with an appropriate -I directive.



Using NW Graph
--------------

NW graph comes with a number of tests, examples, and benchmarks.



cmake
-----





Building Tests and Benchmarks
-----------------------------




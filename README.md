![Build with gcc-11](https://github.com/NWmath/NWgr/workflows/Build%20with%20gcc-11/badge.svg)
![Build with gcc-11](https://github.com/NWmath/NWgr/workflows/Build%20with%20gcc-11/badge.svg?branch=sc_release)
![Build with gcc-11](https://github.com/NWmath/NWgr/workflows/Build%20with%20gcc-11%20(Mac)/badge.svg?branch=sc_release)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/0788903a1d134b47b351e6a346123875)](https://www.codacy.com?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=NWmath/NWgr&amp;utm_campaign=Badge_Grade)

# NWGraph: Northwest Graph Library
NWGraph is a high-performance header-only C++ graph library. It consists of multiple graph algorithms and data structures.


## Organization

The graph algorithms are under include/nwgraph/algorithms/ diretory (some of the experimental algorithms are under include/nwgraph/experimental/algorithms/). The range adaptors are under include/nwgraph/adaptors/ directory. The code for the applications is under bench/ diretory. The abstraction penalty expirements for different containers and a variety of different ways to iterate through a graph (including the use of graphadaptors) are under abp directory. The code for various examples is under example/imdb/ directory.



## How to compile

NWGraph uses [Intel OneTBB](https://github.com/oneapi-src/oneTBB) as the parallel backend.   



### Requirements

* CMake &gt;= 3.20
* g++ &gt;= 11 with support for OneTBB as parallel backend
* oneTBB &gt;= 2021
 

### Compilation

```
$ mkdir build; cd build
$ cmake ..
```

### Useful things to know 
To specify compiler:
```
$ cmake .. -DCMAKE_CXX_COMPILER=g++-11
```
To specify build type as Release or Debug, default is Release:
```
$ cmake .. -DCMAKE_BUILD_TYPE=Release (or Debug)
```
To enable test cases and examples under build/test directory:
```
$ cmake .. -DNW_GRAPH_BUILD_TEST=ON (or OFF)
```
To generate applications under build/bench/ directory:
```
$ cmake .. -DNW_GRAPH_BUILD_BENCH=ON (or OFF)
```
To generate abstraction penalty under build/abp/ directory:
```
$ cmake .. -DNW_GRAPH_BUILD_APBS=OFF (or ON)
```
To generate tools under build/example/ directory:
```
$ cmake .. -DNW_GRAPH_BUILD_EXAMPLES=OFF (or ON)
```
To see verbose information during compilation:
```
$ make VERBOSE=1
```

## Running code in NWGraph

NWGraph uses command-line interface description language [DOCOPT](http://docopt.org/) to define the interface of our command-line applications and abstraction penalty experiments.

A typical interface of the binary looks like this:
```
bfs.exe: breadth first search benchmark driver.
  Usage:
      bfs.exe (-h | --help)
      bfs.exe -f FILE [-r NODE | -s FILE] [-i NUM] [-a NUM] [-b NUM] [-B NUM] [-n NUM] [--seed NUM] [--version ID...] [--log FILE] [--log-header] [-dvV] [THREADS]...

  Options:
      -h, --help              show this screen
      -f FILE                 input file path
      -i NUM                  number of iteration [default: 1]
      -a NUM                  alpha parameter [default: 15]
      -b NUM                  beta parameter [default: 18]
      -B NUM                  number of bins [default: 32]
      -n NUM                  number of trials [default: 1]
      -r NODE                 start from node r (default is random)
      -s, --sources FILE      sources file
      --seed NUM              random seed [default: 27491095]
      --version ID            algorithm version to run [default: 0]
      --log FILE              log times to a file
      --log-header            add a header to the log file
      -d, --debug             run in debug mode
      -v, --verify            verify results
      -V, --verbose           run in verbose mode
```

The applications takes options followed by the arguments of the options as inputs. A minimal example takes a graph as input is as follow:
```
$ bfs.exe -f karate.mtx
```


## Supported graph file format

NWGraph recogonizes the following types of file format:
* [Matrix Market Exchange Formats](https://math.nist.gov/MatrixMarket/formats.html)


## Running benchmarks

We have five main benchmarks: Breadth-first Search, Connected Component Decomposition, Page rank, Single Source Shortest Path, and Triangle Counting. 

### Breadth-first Search
The default sequential version of BFS is version 0 (default). The fastest parallel version of BFS is version 11, the direction-optimizing BFS. 
```
$ bench/bfs.exe -f karate.mtx --seed 0 --version 11
```
### Connected Component Decomposition
The default sequential version of CC is version 0 (default). The fastest parallel version of CC is version 7, Afforest.
```
$ bench/cc.exe -f karate.mtx --relabel --direction ascending
```
### Page rank
The fastest parallel version of PR is version 11 (default).
```
$ bench/pr.exe -f karate.mtx
```
### Single Source Shortest Path
The default sequential version of CC SSSP version 0 (default). The fastest parallel version of SSSP is version 12, Delta-stepping.
```
$ bench/sssp.exe -f karate.mtx
```
### Triangle Counting
The default sequential version of TC is version 0 (default). The fastest parallel version of TC is version 4.
```
$ bench/tc.exe -f karate.mtx --version 4 --relabel --upper
```

### Other useful things

Note that the following features may or may be available to every benchmark.

#### Relabel-by-degree
Relabel vertex by degree (also known as column/row permutation in matrix-matrix multiplication) may speed up the performance of the graph algorithm. It can improve the workload distribution and memory access pattern of the algorithm itself. To enable relabel-by-degree and relabel the degree of vertices in ascending order:
```
$ bench/cc.exe -f karate.mtx --relabel --direction ascending
```
#### Upper Triangular Order
In triangle counting, it allows to relabel the graph in upper/lower triangular order. This will greatly improve the performance of the algorithm. To enable relabel-by-degree and relabel the degree of vertices in upper triangular order:
```
$ bench/tc.exe -f karate.mtx --relabel --upper
```
#### Verifier
We implement a verifier in each benchmark to verify the correctness of the algorithms. To enable the verification of the algorithm:
```
$ bench/cc.exe -f karate.mtx -v
```
or
```
$ bench/cc.exe -f karate.mtx --verify
```
#### Multi-threading
Each algorithm/benchmark has both sequential version and parallel version. When a parallel algorithm is selected, multi-threading is enable by default. The number of threads is set to be the maximum available core on the machine. To enable multi-threading with different thread number, such as 128 threads:
```
$ bench/cc.exe -f karate.mtx 128
```

## Benchmarking abstraction penalties

### What is abstraction penalty?

There are two types of abstraction penalties here.
Using a range-based interface introduces a variety of different ways to iterate through a graph (including the use of graph adaptors).
While ranges and range based for loops are useful programming abstractions, it is important to consider any performance abstraction penalties associated with their use. We benchmark these penalties to ensure they will not significantly limit performance compared to a raw for loop implementation.

We also evaluated the abstraction penalty incurred for storing a graph in different containers. In particular, we have selected `struct_of_array`, `vector_of_vector`, `vector_of_list`, `vector_of_forward_list` containers.

### Running abstraction penalty experiments

For example let us consider the sparse matrix-dense vector multiplication (SpMV) kernel used in page rank, which multiplies the adjacency matrix representation of a graph by a dense vector x and stores the result in another vector y.
To experimentally evaluate the abstraction penalty of different ways to iterate through a graph:
```
$ apb/spmv.exe -f karate.mtx
```

To experimentally evaluate the abstraction penalty of different containers for storing a graph:
```
$ apb/containers -f karate.mtx --format CSR --format VOV --format VOL --format VOF
```
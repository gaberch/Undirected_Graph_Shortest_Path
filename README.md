# Undirected Graph Shortest Path Calculator

Simple program built in C++ that does the following:
* Takes an undirected graph as an input
* Determines shortest path between two vertices
* Stops the program on EOF

## CMake

This program us compiled using CMake. The CMake instructions are contained in `CMakeLists.txt`. To compile the program, do the following, starting in the main repo:
``` 
mkdir build
cd build
cmake ../
make
```

## Resources

The algorithm used to calculate the shortest path is based on the Breadth-First Search method as described in section 22.2 of the book "Introduction to Algorithms", by Cormen, Leiserson, Rivest & Stein, 2nd ed.

## Sample Run

Assume that the executable is called `a2-ece650`. In the following, `$` is the command-prompt.

```
$ ./a2-ece650
V 15
E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}
s 2 10
2-8-10
V 5
E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}
s 4 0
4-1-2-0
```

## Input and output
The program takes input from standard input and output is sent to standard putput. Errors are sent to standard error.



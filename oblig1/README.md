

# IN4200 - Mandatory 1

This repository contains the work for the first mandatory of IN4200 spring 2022. The aim is to read a webgraph from a file and rank the webpages based on their internal links using the PageRank algorithm outlined in ```exercise_description.pdf```. For quick running write ```make default``` or simply ```make run_default``` as the executables are included in this repository also. The compiled code reads the information from a specified file: *filename*, run the algorithm using specified parameters: $d, \epsilon$, and display the top $n$ webpages. The parameters given at execution is described in the following table.

| Parameters| Description | Default settings|
|:---:|---|:---:|
| d  | Damping parameter.  | 1 |
| $\epsilon$ (epsilon)  | Convergence threshold. | $10^{-6}$ |
| n  |  Number of top webpages to show. | 8 |
| filename  | Filename for file containing webgraph. <br> (placed in same directory as the code)  | 8-webpages.txt |
| (read mode)  | Whether to nodes from 0 or 1. <br> 0: Count from 0 (C, C++, python) <br> 1: Count from 1 (Fortran, Matlab)  | 0 |


## Compiling
To compile the code you have the following availbale makefile calls.

|  Use of makefile | Description  |
|---|---|
| *make serial* / *make* | Compiles the serial code producing the executable ```main.out```.   |
| *make default* | Compiles serial code and run executable with default settings (by calling *make run_default*). |
| *make run_default* | Run serial code with default settings (note: executable from serial code should be compiled).  |
| *make omp* | Compiles parallel code producing the executable ```main_omp.out```  |

This is custommized to compile on Apple clang version 12.0.0 (clang-1200.0.32.2) with llvm and libomp packages for running openMP.

## Running the executable
After compiling you can run the executable (either ``main.out`` or ```main_omp.out```) files as

```
/executable d epsilon n filename (read mode)
```

If read mode is not specified it will be set to 0 as default. The remaining parameters must be specified. The following section show some examples of code execution.


```
make default

make serial
./main.out 1 1e-6 8 8-webpages.txt
./main.out 0.98 1e-12 10 web-stanford.txt 1

make omp
./main_omp.out 0.98 1e-12 10 web-stanford.txt 1
```

In order to change the number of threads used in the paralleized code do
```
export OMP_NUM_THREADS=8
```

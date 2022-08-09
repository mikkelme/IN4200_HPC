# IN4200 - Mandatory 2

This repository contains the work for the second mandatory of IN4200 spring 2022. The aim is to run a denoising algorithm on a noisy grey scale image. The details of the algorithm and the project in general is outlinted ```in3200_in4200_oblig2_v22.pdf```. For a quick run do ```make quick_run``` which compiles and run the serial code with default settings.

## Compiling
To compile the code you can use the makefile in the parent folder with the following options.

|  Makefile calls | Description  |
|---|---|
| ```make``` / ```make all ``` | Compiles both the serial code and parallel code.   |
| ```make serial``` | Compiles the serial code producing the executable ```serial_main.out```.   |
| ```make parallel``` | Compiles the parallel code producing the executable ```parallel_main.out```.   |
| ```make serial_run``` | Run the serial code with default settings. |
| ```make parallel_run``` | Run the serial code with default settings. |
| ```make clean``` | Cleans object files, executales and autosaves in the serial_code, parallel code and simple-jpeg folders. |




In order to run the code with different input variables use the command on the following form.

```
 serial_code/serial_main.out 0.2 100 mona_lisa_noisy.jpg 

 mpirun -np 4 ./parallel_code/parallel_main.out 0.2 100 mona_lisa_noisy.jpg
```
 

## Default settings
The default run settings is a follows.

| Parameters| Description | Default settings|
|:---:|---|:---:|
| $\kappa$ (kappa)  | Scalar constant used in denoising algorithm  | 0.2 |
| iters  | Number of iterations to use in denoising algorithm | 100 |
| filename  | Filename for the input image which should be processed | "mona_lisa_noisy.jpg" |


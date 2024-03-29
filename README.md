# Pika Matrix Multiplication
I have implemented Parallel matrix multiplication using the Pika library

# Installation Instructions
- Make sure you have the Pika library installed. Installation instructions for Pika are [here](https://pikacpp.org/usage.html#getting-started)
- Clone the repository
```bash
git clone https://github.com/ujjwal-shekhar/pika_matrix_mult.git
```
- `cd` into the directory
```bash
cd pika_matrix_mult/
```
- Create a build directory for CMake, and `cd` into it (this is where we will build the source)
```bash
mkdir build ; cd build
```
- Run CMake
```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/pika/installation/ ..
```
> NOTE: `/path/to/pika/installation/` is where you might find the the `spack` installation, it will probably be `/home/<user_name>/spack/opt/spack/linux-ubuntu20.04-icelake/gcc-9.4.0/pika-0.23.0-f4jkh5ncx7knvzm6wmgwuuttu5wylbsj`

- Run make
```bash
make
```

- Once the target gets built, run the executable 
```bash
./matrix_multiplication
```

You will get an output that looks like:
```stdout
Matrix A:
4 3 10 
6 6 7 
9 1 2 

Matrix B:
10 10 6 
4 7 5 
3 2 4 

Result Matrix C (A * B):
82 81 79 
105 116 94 
100 101 67
```

To use different settings and/or change the random seed (without `<` or `>`):
```bash
./matrix_multiplication <n> <k> <m> <l> <r> <s>
```

where:

- `n` : Number of rows of matrix $M_1$
- `k` : Number of columns of matrix $M_1$ (should be equal to the number of rows of matrix $M_2$)
- `m` : Number of columns of matrix $M_2$
- `l` : Lower bound on randomized values
- `r` : Upper bound on randomized values
- `s` : Seed for random number generation

> NOTE: If no seed is given, a random seed is selected using : `std::chrono::system_clock::now()`
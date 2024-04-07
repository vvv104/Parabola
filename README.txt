This program accepts integer values from the command line in groups of three.
Each group represents parameters of a quadratic equation. Numbers are
considered as 32-bit integers. If any of the parameters cannot be converted to
int32, all group of three parameters for the equation is nullified.

The program finds the roots of the quadratic equations and the root of the
first derivative of the quadratic equation and outputs the solution for each
equation in the format {(x1[, x2])|no roots} => {Xmin=x0|no Xmin}. Calculations
are performed simultaneously in 4 threads, reading and conversion occur in one
thread, and information output occurs in one thread. The output does not
necessarily have to be in the same order as the data was received.

A circular buffer of size 4 is used for data transfer between the reading
thread and the computational threads, and a buffer of the same size is used for
gathering data from the computational threads into the output thread. Thus,
memory consumption does not exceed 4*O +4*(O+R) where O is size of the object
for quadratic equation definition (3 int32), R is size of result object (3
double). The buffer sizes as well as number of calculation threads can be
parameterized in the program, but currently, the value is fixed.

The producer-consumer approach is chosen as the synchronization mechanism as
the most versatile way to distribute homogeneous computations.
# Parallel-Semi-Ordered-Matrix-Search

The repository contains solution to one of the assignments of the course Parallel Computing in BITS Pilani.

The problem statement is described as follows:

A. Use divide-and-conquer to design a PRAM algorithm for the following problem:
Given a semi-ordered matrix M of numbers find (the position of) a given number K in M. A semi-ordered matrix is one in which
each row is sorted (in increasing order) and each column is sorted (in the same order).

B. Implement your parallel algorithm for (A) using OpenMP in C/C++.

c) Measure the performance for different sizes of M in the range 10 3 x 10 3 to 10 5 x 10 5 . For each size measure the performance for p = 1, 2, .. 2 q where p is the number of cores used.

The design, measurements and inferences are summarized in the design document.

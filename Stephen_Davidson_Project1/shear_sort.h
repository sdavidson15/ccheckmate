#ifndef SHEAR_SORT_H
#define SHEAR_SORT_H

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// mesh_config is a convenience struct carrying the location
// of a mesh and its dimensions. A mesh is a 2D int array.
typedef struct
{
    int num_rows;
    int num_cols;
    int *mesh;
} mesh_config;

// thread_context contains a (expected to be) unique identifier for
// a thread, the total number of threads running, and a mesh_config.
typedef struct
{
    int id;
    int num_threads;
    mesh_config m_cfg;
} thread_context;

// sort_direction is used to specify whether to sort ascending or descending.
typedef enum
{
    ASCENDING,
    DESCENDING
} sort_direction;

// read_mesh_from_file opens the file specified by filepath and parses
// an m x n 2D int array from it.
// If m != n, this will print an error message and exit the program.
mesh_config read_mesh_from_file(char *);

// shear_sort is the start routine run by each thread to perform the shear sort algorithm.
// All synchronization between threads happens in this function.
// shear_sort receives a parameter param whose type is expected to be a thread_context.
void *shear_sort(void *);

// bubble_sort_row sorts a specific row of the provided mesh of width num_cols.
// Takes another parameter direction indicating if it should sort ascending or descending.
void bubble_sort_row(int *, int, int, sort_direction);

// bubble_sort_col sorts a specific col of the provided mesh of height num_rows.
void bubble_sort_col(int *, int, int);

// swap swaps the integer values at the given locations.
void swap(int *, int *);

// mesh_is_sorted iterates over the given mesh of dimensions num_row x num_cols and returns
// whether or not the mesh is sorted in the snakelike pattern of a correct shear sort.
int mesh_is_sorted(int *, int, int);

// print_mesh outputs the mesh of dimensions num_rows x num_cols to the console.
void print_mesh(int *, int, int);

#endif

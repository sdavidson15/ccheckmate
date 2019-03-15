// This file contains a multithreaded implementation of the Shear Sort algorithm.
// Author: Stephen Davidson
// Email: scd@iastate.edu

#include "shear_sort.h"

// done - shared variable that tracks when threads complete phases.
// Protected by mutex and cond.
u_int64_t done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// check - shared variable that tracks when threads complete mesh_is_sorted checks.
// Protected by mutex2 and cond2.
u_int64_t check = 0;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

int main()
{
    mesh_config m_cfg = read_mesh_from_file("input.txt");
    printf("\nINPUT ARRAY:\n--------------------\n");
    print_mesh(m_cfg.mesh, m_cfg.num_rows, m_cfg.num_cols);

    int num_threads = m_cfg.num_rows;

    pthread_t ids[num_threads];
    for (int i = 0; i < num_threads; i++)
    {
        // Prepare parameters to pass into each thread
        thread_context ctx = {i, num_threads, m_cfg};
        thread_context *ptr = malloc(sizeof(*ptr));
        *ptr = ctx;

        pthread_create(&ids[i], NULL, shear_sort, (void *)ptr);
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(ids[i], NULL);

    return 0;
}

// read_mesh_from_file opens the file specified by filepath and parses
// an m x n 2D int array from it.
// If m != n, this will print an error message and exit the program.
mesh_config read_mesh_from_file(char *filepath)
{
    // Get the dimensions of the mesh
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Failed to read from file at: %s\n", filepath);
        exit(0);
    }

    char ch = fgetc(file);
    int rowCount = 0;
    int colCount = 0;
    while (ch != EOF)
    {
        if (ch == ' ')
            colCount++;
        else if (ch == '\n')
            rowCount++;

        ch = fgetc(file);
    }

    fclose(file);

    // Check that the mesh is a square
    int num_rows = rowCount;
    int num_cols = colCount / (rowCount - 1);
    if (num_rows != num_cols)
    {
        printf("Provided mesh in %s is not a square. Has dimensions: %d x %d\n", filepath, rowCount, colCount);
        exit(0);
    }

    // Allocate space for and load the mesh
    int *mesh = (int *)malloc(num_rows * num_cols * sizeof(int));

    fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Failed to read from file at: %s\n", filepath);
        exit(0);
    }

    int i = 0;
    int j = 0;
    while (i < num_rows && j < num_cols)
    {
        int num = 0;
        fscanf(file, "%d", &num);
        *(mesh + i * num_cols + j) = num;

        j++;
        if (j == num_cols)
        {
            j = 0;
            i++;
        }
    }

    fclose(file);

    // Return metadata about the mesh in a mesh_config
    int *mesh_ptr = (int *)mesh;
    mesh_config m_cfg = {num_rows, num_cols, mesh_ptr};
    return m_cfg;
}

// shear_sort is the start routine run by each thread to perform the shear sort algorithm.
// All synchronization between threads happens in this function.
// shear_sort receives a parameter param whose type is expected to be a thread_context.
void *shear_sort(void *param)
{
    thread_context ctx = *((thread_context *)param);
    free(param);

    int *mesh = ctx.m_cfg.mesh;
    int num_rows = ctx.m_cfg.num_rows;
    int num_cols = ctx.m_cfg.num_cols;

    u_int64_t attempts = 0;
    while (attempts < INT_MAX / ctx.num_threads) // to avoid overflows
    {
        // Check if the mesh is sorted
        if (mesh_is_sorted(mesh, num_rows, num_cols))
            break;

        // Wait until all threads are done checking if the mesh is sorted.
        pthread_mutex_lock(&mutex2);
        check++;
        while (check < ctx.num_threads * (attempts + 1))
            pthread_cond_wait(&cond2, &mutex2);

        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mutex2);

        // Sort
        if ((attempts % 2) != 0)
        {
            sort_direction direction = ((ctx.id % 2) == 0) ? ASCENDING : DESCENDING;
            bubble_sort_row(mesh, ctx.id, num_cols, direction);
        }
        else
        {
            bubble_sort_col(mesh, ctx.id, num_rows);
        }

        // Wait until all the threads are done sorting their row/col.
        pthread_mutex_lock(&mutex);
        done++;
        while (done < ctx.num_threads * (attempts + 1))
            pthread_cond_wait(&cond, &mutex);

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        // Give exactly 1 thread the responsibility of printing
        if (ctx.id == 0)
        {
            printf("PHASE %llu\n--------------------\n", attempts + 1);
            print_mesh(mesh, num_rows, num_cols);
        }

        attempts++;
    }

    return 0;
}

// bubble_sort_row sorts a specific row of the provided mesh of width num_cols.
// Takes another parameter direction indicating if it should sort ascending or descending.
void bubble_sort_row(int *mesh, int row, int num_cols, sort_direction direction)
{
    for (int i = 0; i < num_cols - 1; i++)
    {
        for (int j = 0; j < num_cols - i - 1; j++)
        {
            int *current = ((mesh + row * num_cols) + j);
            int *next = ((mesh + row * num_cols) + j + 1);

            if ((direction == ASCENDING && *current > *next) ||
                (direction == DESCENDING && *current < *next))
                swap(current, next);
        }
    }
}

// bubble_sort_col sorts a specific col of the provided mesh of height num_rows.
void bubble_sort_col(int *mesh, int col, int num_rows)
{
    for (int i = 0; i < num_rows - 1; i++)
        for (int j = 0; j < num_rows - i - 1; j++)
        {
            int *current = ((mesh + j * num_rows) + col);
            int *next = ((mesh + (j + 1) * num_rows) + col);

            if (*current > *next)
                swap(current, next);
        }
}

// swap swaps the integer values at the given locations.
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// mesh_is_sorted iterates over the given mesh of dimensions num_row x num_cols and returns
// whether or not the mesh is sorted in the snakelike pattern of a correct shear sort.
int mesh_is_sorted(int *mesh, int num_rows, int num_cols)
{
    for (int i = 0; i < num_rows; i++)
    {
        int direction = ((i % 2) == 0) ? 1 : -1;
        int j = 0;

        while (
            // Weird conditional that comes from handling
            // both directions in the same loop.
            (direction == 1 && j < num_cols) ||
            (direction == -1 && j >= 0))
        {
            int current = *((mesh + i * num_cols) + j);
            int next = 0;

            if (j + direction < 0 || j + direction == num_cols)
            {
                // Handle reaching the end of the column
                if (i + 1 == num_rows)
                    break;

                next = *((mesh + (i + 1) * num_cols) + j);
            }
            else
            {
                next = *((mesh + i * num_cols) + j + direction);
            }

            if (current > next)
                return 0;

            j += direction;
        }
    }

    return 1;
}

// print_mesh outputs the mesh of dimensions num_rows x num_cols to the console.
void print_mesh(int *mesh, int num_rows, int num_cols)
{
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
            printf("%d ", *((mesh + i * num_cols) + j));
        printf("\n");
    }

    printf("\n");
}

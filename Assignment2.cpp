#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>


using namespace std;


// The function of allocating memory for a two-dimensional array
double** malloc_array(long int n) {
    double** matrix = new double*[n];
    for (int i = 0; i < n; i++)
        matrix[i] = new double[n];
    return matrix;
}


// Memory free function
void free_array(double** matrix, long int n) {
    for (int i = 0; i < n; i++)
        delete[] matrix[i];
    delete[] matrix;
}


// The function of initializing a matrix with random numbers
void rand_init_matrix(double** matrix, long int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand() / 1000.0;
}


// Matrix zeroing function
void zero_init_matrix(double** matrix, long int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = 0.0;
}

// // ijk-multiplication
double ijk_multiplication(double** A, double** B, double** C, long int N, int n_threads)
{
    double time = omp_get_wtime();
    #pragma omp parallel num_threads(n_threads)
    {
        #pragma omp for
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    }
    time = omp_get_wtime() - time;
    return time;
}

// jki-multiplication
double jki_multiplication(double** A, double** B, double** C, long int N, int n_threads)
{
    double time = omp_get_wtime();
    #pragma omp parallel num_threads(n_threads)
    {
        #pragma omp for
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                for (int i = 0; i < N; i++)
                    C[i][j] += A[i][k] * B[k][j];
    }
    time = omp_get_wtime() - time;
    return time;
}

// ikj-multiplication
double ikj_multiplication(double** A, double** B, double** C, long int N, int n_threads)
{
    double time = omp_get_wtime();
    #pragma omp parallel num_threads(n_threads)
    {
        #pragma omp for
        for (int i = 0; i < N; i++)
            for (int k = 0; k < N; k++)
                for (int j = 0; j < N; j++)
                    C[i][j] += A[i][k] * B[k][j];
    }
    time = omp_get_wtime() - time;
    return time;
}

// Print array by rows 
void print_array(double** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%f ", matrix[i][j]);
        }
        cout << endl;
    }
}

int main(int argc, char* argv[])
{
    int N = atoi(argv[1]);
    double **A, **B, **C;

    // Memory allocation for matrices A, B, C
    A = malloc_array(N);
    B = malloc_array(N);
    C = malloc_array(N);

    // Initialization of matrices
    rand_init_matrix(A, N);
    rand_init_matrix(B, N);
    zero_init_matrix(C, N);

    // cout << "A:\n";
    // print_array(A, N);
    // cout << "B:\n";
    // print_array(B, N);
    
    // One stream as a baseline for efficiency
    int n_threads = 1;
    cout << "ijk multiplication" << endl;
    double ijk_time_1 = ijk_multiplication(A, B, C, N, n_threads);
    printf("Number of threads: %d; ", n_threads);
    printf("time: %f; ", ijk_time_1);
    // Efficiency is calculated by division of baseline time by obtained time
    printf("efficiency: %f\n", ijk_time_1 / ijk_time_1);

    // cout << "C:\n";
    // print_array(C, N);

    for (n_threads = 2; n_threads <= 10; n_threads++)
    {
        zero_init_matrix(C, N);
        double ijk_time = ijk_multiplication(A, B, C, N, n_threads);
        printf("Number of threads: %d; ", n_threads);
        printf("time: %f s; ", ijk_time);
        printf("efficiency: %f\n", ijk_time_1 / ijk_time);
    }

    n_threads = 1;
    cout << "jki multiplication" << endl;
    zero_init_matrix(C, N);
    double jki_time_1 = jki_multiplication(A, B, C, N, n_threads);
    printf("Number of threads: %d; ", n_threads);
    printf("time: %f; ", jki_time_1);
    printf("efficiency: %f\n", jki_time_1 / jki_time_1);
    for (n_threads = 2; n_threads <= 10; n_threads++)
    {
        zero_init_matrix(C, N);
        double jki_time = jki_multiplication(A, B, C, N, n_threads);
        printf("Number of threads: %d; ", n_threads);
        printf("time: %f s; ", jki_time);
        printf("efficiency: %f\n", jki_time_1 / jki_time);
    }

    n_threads = 1;
    cout << "ikj multiplication" << endl;
    double ikj_time_1 = ikj_multiplication(A, B, C, N, n_threads);
    printf("Number of threads: %d; ", n_threads);
    printf("time: %f; ", ikj_time_1);
    printf("efficiency: %f\n", ikj_time_1 / ikj_time_1);
    for (n_threads = 2; n_threads <= 10; n_threads++)
    {
        zero_init_matrix(C, N);
        double ikj_time = ikj_multiplication(A, B, C, N, n_threads);
        printf("Number of threads: %d; ", n_threads);
        printf("time: %f s; ", ikj_time);
        printf("efficiency: %f\n", ikj_time_1 / ikj_time);
    }

  free_array(A, N);
  free_array(B, N);
  free_array(C, N);

  return 0;
}
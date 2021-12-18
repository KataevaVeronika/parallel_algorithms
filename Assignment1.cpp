#include <iostream>
#include <cstdio>
#include "omp.h"

using namespace std;

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    // initializing the vector of size n with random values from 0 to 32.767
    double* a = new double[n];
    for (long int i = 0; i < n; i++)
    {
        a[i] = rand() / 1000.0;
        // printf("%f ", a[i]);
    }
    cout << endl;

    for (int n_threads = 1; n_threads <= 10; n_threads++)
    {
        // setting max element to the fisrt element of vector
        double max_element = a[0];
        // set timestamp
        double time = omp_get_wtime();
        #pragma omp parallel for num_threads(n_threads) reduction(max : max_element)
        for (int i = 0; i < n; i++)
        {
            // finding max element between current vector element and prev. max value
            max_element = max(max_element, a[i]);
        }
        printf("Number of threads: %d\n", n_threads);
        printf("Max element: %f\n", max_element);
        printf("Time: %f s\n", omp_get_wtime() - time);
    }
    delete[] a;
    return 0;
}
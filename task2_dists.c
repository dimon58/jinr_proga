#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <math.h>


double uniform_dist() {
    // +1 и +2 нужны для невозможности сгенерировать 0 или 1, чтобы не получались nan в normal_dist
    return (double) (rand() + 1) / ((double) RAND_MAX + 2); // NOLINT(cert-msc50-cpp)
}

double normal_dist() {
    double u = uniform_dist();
    double v = uniform_dist();

    return sqrt(-2 * M_PI * log(u)) * cos(2 * M_PI * v);
}


void fill_array(double (dist)(), double *arr, int n) {
    for (int i = 0; i < n; ++i) arr[i] = dist();
}

double calc_mean(const double *arr, int size) {
    double res = 0;
    for (int i = 0; i < size; ++i) res += arr[i];

    return res / size;
}

double calc_mu_k(const double *arr, int size, int k) {
    double mean = calc_mean(arr, size);

    double res = 0;
    for (int i = 0; i < size; ++i) res += pow(mean - arr[i], k);

    return res / size;

}

double calc_std(const double *arr, int size) {
    return sqrt(calc_mu_k(arr, size, 2));
}

double calc_gamma1(const double *arr, int size) {
    return calc_mu_k(arr, size, 3) / pow(calc_std(arr, size), 3);
}

double calc_gamma2(const double *arr, int size) {
    return calc_mu_k(arr, size, 4) / pow(calc_std(arr, size), 4) - 3;
}

double *get_array_from_txt(const char *path, int *size) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Opening file %s fail", path);
        exit(-1);
    }

    *size = 100000;
    double *arr = (double *) malloc(*size * sizeof(double));

    for (int i = 0; i < *size; ++i) {
        int r = fscanf(file, "%lf", arr + i);
        if (r != 1 || r == EOF) {
            printf("Беды на %d", i);
            exit(-1111);
        }
    }

    fclose(file);

    return arr;
}

void print_arr_c(double *arr, int size) {
    printf("mean    %f\n", calc_mean(arr, size));
    printf("std     %f\n", calc_std(arr, size));
    printf("gamma1  %f\n", calc_gamma1(arr, size));
    printf("gamma2  %f\n", calc_gamma2(arr, size));
}

int main(int argc, char *argv[]) {
    srand(time(NULL)); // NOLINT(cert-msc51-cpp)

    for (int i = 0; i < argc; ++i) {
        printf("%d %s\n", i, argv[i]);
    }
    if (argc == 1) {
        printf("Not enough args");
        exit(-1);
    }


    int size = atoi(argv[1]);
    printf("Creating array size %d for uniform dist\n", size);
    double *arr = (double *) malloc(size * sizeof(double));
    fill_array(uniform_dist, arr, size);
    print_arr_c(arr, size);
    printf("\n");

    printf("Creating array size %d for normal dist\n", size);
    fill_array(normal_dist,arr, size);
    print_arr_c(arr, size);
    free(arr);

    const char *path = "tasks/Landau.txt";
    printf("\n\nReading %s\n", path);
    int *size_file = (int *) malloc(sizeof(int));
    double *arr_file = get_array_from_txt(path, size_file);
    print_arr_c(arr_file, *size_file);
    free(arr_file);

    return 0;
}
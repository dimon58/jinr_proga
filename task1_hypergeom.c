#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdbool.h>

#define MAX_ITERS 200
#define STOP_DELTA 1e-12


double hyper_geom_v1(double a, double b, double c, double z);

double hyper_geom_v2(double a, double b, double c, double z);

double hyper_geom_transform4(double a, double b, double c, double z);

double hyper_geom_transform5(double a, double b, double c, double z);


double frac(double x) {
    return x - trunc(x);
}

double hyper_geom_v1(double a, double b, double c, double z) {
    int i = 1;
    double result = 1;
    double el = 1, prev = 0;

    while (true) {
        el *= (a + i - 1) * (b + i - 1) / (c + i - 1) * z / (double) i;
        result += el;
        if (fabs(el - prev) < STOP_DELTA || i > MAX_ITERS) return result;
        prev = el;
        i++;
    }
}

double hyper_geom_transform4(double a, double b, double c, double z) {
    return tgamma(c) * tgamma(c - a - b) / tgamma(c - a) * tgamma(c - b) * hyper_geom_v2(a, b, a + b - c + 1, 1 - z)
           + pow(1 - z, c - a - b) * tgamma(c) * tgamma(a + b - c) / tgamma(a) / tgamma(b) *
             hyper_geom_v2(c - a, c - b, c - a - b + 1, 1 - z);
}


double hyper_geom_transform5(double a, double b, double c, double z) {
    return pow(1 - z, -a) * hyper_geom_v2(a, c - b, c, z / (z - 1));
}


double hyper_geom_v2(double a, double b, double c, double z) {
    if (fabs(z) <= 0.5) return hyper_geom_v1(a, b, c, z);
    if ((0.5 < z) && (z <= 1) && (fabs(frac(c - a - b)) > 1e-12)) return hyper_geom_transform4(a, b, c, z);
    if (z < -0.5) return hyper_geom_transform5(a, b, c, z);

    return hyper_geom_v1(a, b, c, z);
}


double log_func(double z) {
    return -log(1 - z) / z;
}

double log_arg_generator(double z) {
    return z;
}


double asin_func(double z) {
    return asin(z) / z;
}

double asin_arg_generator(double z) {
    return z * z;
}


double atan_func(double z) {
    if (fabs(z) < 1e-12) return 1;
    return atan(z) / z;
}

double atan_arg_generator(double z) {
    return -z * z;
}


double sqrt_func(double z) {
    return 1.0 / 2.0 * (sqrt(sqrt(1 + z * z) + z) + sqrt(sqrt(1 + z * z) - z));
}

double sqrt_arg_generator(double z) {
    return -z * z;
}

void print_table_internal(double (*ref_func)(double), double a, double b, double c, double (*arg_generator)(double),
                          double z) {
    double val, ref, esp;
    val = hyper_geom_v2(a, b, c, arg_generator(z));
    ref = ref_func(z);
    esp = fabs(ref - val) / ref;
    printf("%5.2f    %21.17f    %21.17f    %.2e\n", z, val, ref, esp);
}

void print_table(double start, double end, double step, double (ref_func)(double), double a, double b, double c,
                 double (arg_generator)(double), char *name) {
    char *func_def = (char *) (malloc(255 * sizeof(char)));
    sprintf(func_def, "F(%.2f, %.2f, %.2f, z)", a, b, c);
    printf("%5s    %21s    %21s    %s\n", "z", func_def, name, "eps");
    for (double z = start; z <= end; z += step) { // NOLINT(cert-flp30-c)
        print_table_internal(ref_func, a, b, c, arg_generator, z);
    }
}


void print_table2(double (ref_func)(double), double a, double b, double c,
                  double (arg_generator)(double), char *name) {
    char *func_def = (char *) (malloc(255 * sizeof(char)));
    sprintf(func_def, "F(%.2f, %.2f, %.2f, z)", a, b, c);
    printf("%5s    %21s    %21s    %s\n", "z", func_def, name, "eps");

    for (double z = 0; z < 0.5; z += 0.05) { // NOLINT(cert-flp30-c)
        print_table_internal(ref_func, a, b, c, arg_generator, z);
    }

    for (double z = 0.5; z <= 1.5; z += 0.1) { // NOLINT(cert-flp30-c)
        print_table_internal(ref_func, a, b, c, arg_generator, z);
    }

    for (double z = 1.5; z < 10; z += 0.5) { // NOLINT(cert-flp30-c)
        print_table_internal(ref_func, a, b, c, arg_generator, z);
    }
}

int main() {

    // task 1
    print_table(-0.95, 0.95, 0.1, log_func, 1, 1, 2, log_arg_generator, "-ln(1-z)/z");
    print_table(-0.95, 0.95, 0.1, asin_func, 1.0 / 2.0, 1. / 2, 3. / 2, asin_arg_generator, "asin(z)/z");

    // task 2
    print_table2(atan_func, 1.0 / 2.0, 1.0, 3.0 / 2.0, atan_arg_generator, "atan(z)/z");
    print_table2(sqrt_func, -1.0 / 4.0, 1.0 / 4.0, 1.0 / 2.0, sqrt_arg_generator, "complex_sqrt");

    return 0;


}

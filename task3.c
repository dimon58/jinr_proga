#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include<string.h>

#define sqr(x) (x)*(x)
#define MAXCHAR 1000

struct triangle {
    double x1, y1, x2, y2, x3, y3;
};

void print_triangle(const struct triangle *obj) {
    printf("A(%f, %f), B(%f, %f), C(%f, %f)\n", obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
}

double calc_dist(const double x1, const double y1, const double x2, const double y2) {
    return sqrt(sqr(x1 - x2) + sqr(y1 - y2));
}

double calc_triangle_area(const struct triangle *obj) {
    double a = calc_dist(obj->x1, obj->y1, obj->x2, obj->y2);
    double b = calc_dist(obj->x2, obj->y2, obj->x3, obj->y3);
    double c = calc_dist(obj->x3, obj->y3, obj->x1, obj->y1);
    double p = (a + b + c) / 2;

    return sqrt(p * (p - a) * (p - b) * (p - c));
}


bool is_point_inside_triangle(const struct triangle *obj, double x0, double y0) {
    // https://www.cyberforum.ru/algorithms/thread144722.html
    double v1 = (obj->x1 - x0) * (obj->y2 - obj->y1) - (obj->x2 - obj->x1) * (obj->y1 - y0);
    double v2 = (obj->x2 - x0) * (obj->y3 - obj->y2) - (obj->x3 - obj->x2) * (obj->y2 - y0);
    double v3 = (obj->x3 - x0) * (obj->y1 - obj->y3) - (obj->x1 - obj->x3) * (obj->y3 - y0);

    if (signbit(v1) == signbit(v2) && signbit(v2) == signbit(v3)) return true;

    return false;
}


struct triangle *get_triangles_from_file(const char *path, int *size) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Opening file %s fail", path);
        exit(-1);
    }
    *size = 1000;
    int i = 0;
    struct triangle *arr = (struct triangle *) malloc(*size * sizeof(struct triangle));


    char row[MAXCHAR];
    struct triangle tri;

    while (!feof(file)) {
        fgets(row, MAXCHAR, file);


        // Знаем структуру файла, поэтому без проверок
        tri.x1 = atoi(strtok(row, ","));
        tri.y1 = atoi(strtok(NULL, ","));
        tri.x2 = atoi(strtok(NULL, ","));
        tri.y2 = atoi(strtok(NULL, ","));
        tri.x3 = atoi(strtok(NULL, ","));
        tri.y3 = atoi(strtok(NULL, ","));

        arr[i++] = tri;
    }

    fclose(file);

    return arr;
}

int area_cmp(const void *tri1, const void *tri2) {
    return calc_triangle_area((struct triangle *) tri1) < calc_triangle_area((struct triangle *) tri2);
}

void sort_by_area(struct triangle *arr, int size) {
    qsort(arr, size, sizeof(struct triangle), area_cmp);
}


int count_if_contains(struct triangle *arr, int size, const int x0, const int y0) {
    int count = 0;

    for (int i = 0; i < size; ++i) {
        if (is_point_inside_triangle(&arr[i], x0, y0)) count += 1;
        else count += 0;    // подсказка компилятору, чтобы он использовал векторные инструкции
    }

    return count;
}

int main() {
//    int *size = (int *) malloc(sizeof(int));
    int size;
    struct triangle *arr = get_triangles_from_file("../triangles.txt", &size);

    sort_by_area(arr, size);

    printf("Max area = %.2lf: ", calc_triangle_area(&arr[0]));
    print_triangle(&arr[0]);
    printf("Min area = %.2lf: ", calc_triangle_area(&arr[size - 1]));
    print_triangle(&arr[size - 1]);

    printf("%d/%d triangles contains (0, 0)", count_if_contains(arr, size, 0, 0), size);

    return 0;
}
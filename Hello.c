#include <stdio.h>
#include <locale.h>

#define GET_MAX(arr, n, max_val, i) do { \
    max_val = arr[0]; \
    for (i = 1; i < n; i++) { \
        if (arr[i] > max_val) max_val = arr[i]; \
    } \
} while (0)

#define GET_AVG(arr, n, avg) do { \
    int sum = 0; \
    int _i; \
    for (_i = 0; _i < n; _i++) { \
        sum += arr[_i]; \
    } \
    avg = (double)sum / n; \
} while (0)

int main() {
    setlocale(LC_ALL, "Russian");
    int arr[] = {12, 5, 89, 3, 45, 67};
    int n = sizeof(arr) / sizeof(arr[0]);
    int max_val;
    int i;
    double average;
    int _i;  // переменная для цикла в GET_AVG

    GET_MAX(arr, n, max_val, i);
    GET_AVG(arr, n, average);

    printf("Максимальное число: %d\n", max_val);
    printf("Среднее арифметическое: %.2f\n", average);

    return 0;
}

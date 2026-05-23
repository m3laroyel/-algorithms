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
    int j; \
    for (j = 0; j < n; j++) { \
        sum += arr[j]; \
    } \
    avg = (double)sum / n; \
} while (0)

int main() {
	setlocale(LC_ALL,"Russian");
    int arr[] = {112, 25, 109, 43, 4, 47};
    int n = sizeof(arr) / sizeof(arr[0]);
    int max_val;
    int i; 
    double average;
    int j;  

    GET_MAX(arr, n, max_val, i);
    GET_AVG(arr, n, average);

    printf("Ìàêñèìàëüíîå ÷èñëî: %d\n", max_val);
    printf("Ñðåäíåå àðèôìèòè÷åñêîå: %2.f", average);

    return 0;
}

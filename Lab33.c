#include <stdio.h>
#include <locale.h>

#define GET_MAX(arr, n, max_val, i) do { \
    max_val = arr[0]; \
    for (i = 1; i < n; i++) { \
        if (arr[i] > max_val) max_val = arr[i]; \
    } \
} while (0)

int main() {
    setlocale(LC_ALL, "Russian");
    
    int n;
    printf("Введите количество чисел: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Ошибка: количество чисел должно быть больше 0\n");
        return 1;
    }
    
    int arr[n];  // массив переменного размера (VLA)
    
    printf("Введите %d чисел:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Число %d: ", i + 1);
        scanf("%d", &arr[i]);
    }
    
    int max_val;
    int i;  // переменная для макроса
    
    GET_MAX(arr, n, max_val, i);
    
    printf("Максимальное число: %d\n", max_val);
    
    return 0;
}

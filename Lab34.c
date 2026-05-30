#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char expr[256];
    char tokens[50][50];
    char delimiters[] = " ";
    char *token;
    int token_count = 0;

    printf("enter expression: ");
    fgets(expr, sizeof(expr), stdin);
    expr[strcspn(expr, "\n")] = 0; // Удаляем символ переноса строки

    token = strtok(expr, delimiters);
    while (token != NULL) {
        strcpy(tokens[token_count++], token);
        token = strtok(NULL, delimiters);
    }

    if (token_count == 0) return 0;

    // Массив для хранения промежуточных результатов (стек для чисел)
    double stack[50];
    int top = -1;

    // Сначала обрабатываем первое число
    stack[++top] = atof(tokens[0]);

    // Обработка операций
    for (int i = 1; i < token_count; i += 2) {
        char *op = tokens[i];
        double next_val = atof(tokens[i + 1]);

        if (strcmp(op, "*") == 0) {
            stack[top] *= next_val;
        } else if (strcmp(op, "/") == 0) {
            stack[top] /= next_val;
        } else if (strcmp(op, "+") == 0) {
            stack[++top] = next_val;
        } else if (strcmp(op, "-") == 0) {
            stack[++top] = -next_val;
        }
    }

    // Суммируем всё, что осталось в стеке
    double result = 0.0;
    for (int i = 0; i <= top; i++) {
        result += stack[i];
    }

    printf("result: %g\n", result);

    return 0;
}

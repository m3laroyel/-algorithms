#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct humen {
    char name[50];
    char surname[50];
    int birth_year;
};

int main(void) {
    int n = 0;
    int i, j;
    struct humen *source, temp;
    FILE *inFile, *outFile;

    setlocale(LC_ALL, "Russian");

    /* Открываем файл для чтения */
    inFile = fopen("input.txt", "r");
    if (!inFile) {
        printf("Ошибка: не удалось открыть файл input.txt\n");
        return 1;
    }

    /* Сначала считываем количество записей из первой строки файла */
    if (fscanf(inFile, "%d", &n) != 1 || n <= 0) {
        printf("Ошибка: неверный формат количества людей в файле.\n");
        fclose(inFile);
        return 1;
    }

    /* Выделяем память под один массив */
    source = (struct humen*)malloc(n * sizeof(struct humen));
    if (!source) {
        printf("Ошибка выделения памяти.\n");
        fclose(inFile);
        return 1;
    }

    /* Построчно считываем данные людей из файла */
    for (i = 0; i < n; i++) {
        /* Если файл внезапно закончился раньше времени */
        if (fscanf(inFile, "%49s %49s %d", source[i].name, source[i].surname, &source[i].birth_year) != 3) {
            printf("Ошибка: недостаточно данных в файле или неверный формат на строке %d.\n", i + 2);
            free(source);
            fclose(inFile);
            return 1;
        }
    }
    
    /* Закрываем входной файл, он больше не нужен */
    fclose(inFile);

    /* Сортировка пузырьком (по убыванию года рождения) */
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (source[j].birth_year < source[j + 1].birth_year) {
                temp = source[j];
                source[j] = source[j + 1];
                source[j + 1] = temp;
            }
        }
    }

    /* Открываем файл для записи результата */
    outFile = fopen("output.txt", "w");
    if (!outFile) {
        printf("Ошибка: не удалось создать файл output.txt\n");
        free(source);
        return 1;
    }

    /* Записываем отсортированный список в файл */
    fprintf(outFile, "--- Отсортированный список ---\n");
    for (i = 0; i < n; i++) {
        fprintf(outFile, "%s %s, год рождения: %d\n", source[i].name, source[i].surname, source[i].birth_year);
    }

    /* Освобождаем ресурсы */
    fclose(outFile);
    free(source);

    printf("Данные успешно прочитаны из input.txt и сохранены в output.txt\n");
    return 0;
}


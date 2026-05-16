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


    inFile = fopen("input.txt", "r");
    if (!inFile) {
        printf("Ошибка не удалось открыть файл input.txt\n");
        return 1;
    }
    
    
    
    if (fscanf(inFile, "%d", &n) != 1 || n <= 0) {
        printf("Неверный формат людей в файле.\n");
        fclose(inFile);
        return 1;
    }



    source = (struct humen*)malloc(n * sizeof(struct humen));
    if (!source) {
        printf("Ошибка выделения памяти.\n");
        fclose(inFile);
        return 1;
    }

    for (i = 0; i < n; i++) {
        if (fscanf(inFile, "%49s %49s %d", source[i].name, source[i].surname, &source[i].birth_year) != 3) {
            printf("Ошибка недостаточно данных в строке %d.\n", i + 2);
            free(source);
            fclose(inFile);
            return 1;
        }
    }
    

    fclose(inFile);


    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (source[j].birth_year > source[j + 1].birth_year) {
                temp = source[j];
                source[j] = source[j + 1];
                source[j + 1] = temp;
            }
        }
    }

    outFile = fopen("output.txt", "w");
    if (!outFile) {
        printf("Не удалось сохдать файл output.txt\n");
        free(source);
        return 1;
    }


    fprintf(outFile, "--- Отсартированный список ---\n");
    for (i = 0; i < n; i++) {
        printf( "%s %s, год рождения : %d\n", source[i].name, source[i].surname, source[i].birth_year);
    }


    fclose(outFile);
    free(source);
    return 0;
}

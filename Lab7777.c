#include <stdio.h>
#include <stdlib.h>

struct humen {
    char name[50];
    char surname[50];
    int year;
};

int main(void) {

    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;

    struct humen temp;
    int count = 0;

    while (fscanf(file, "%49s %49s %d", temp.name, temp.surname, &temp.year) == 3) {
        count++;
    }

    struct humen *people = malloc(count * sizeof(struct humen));
    if (!people) {
        fclose(file);
        return 1;
    }

    rewind(file);

    for (int i = 0; i < count; i++) {
        fscanf(file, "%49s %49s %d",
               people[i].name,
               people[i].surname,
               &people[i].year);
    }

    fclose(file);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (people[j].year > people[j + 1].year) {
                struct humen tmp = people[j];
                people[j] = people[j + 1];
                people[j + 1] = tmp;
            }
        }
    }

    printf("%-15s %-15s %s\n", "ÐÐ¼Ñ", "Ð¤Ð°Ð¼Ð¸Ð»Ð¸Ñ", "ÐÐ¾Ð´");

    for (int i = 0; i < count; i++) {
        printf("%-15s %-15s %d\n",
               people[i].name,
               people[i].surname,
               people[i].year);
    }

    free(people);
    return 0;
   

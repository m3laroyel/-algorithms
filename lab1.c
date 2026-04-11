#include <stdio.h>
#include <ctype.h>

int isvowel(int c);
int isconsonant(int c);

int main(int argc, char **argv)
{
    FILE *F;
    int c;
    int total, lines, digits, punct, spaces;
    int letters, lower, upper, vowels, consonants;
    char filename[256];
    int i;
    
    total = 0;
    lines = 0;
    digits = 0;
    punct = 0;
    spaces = 0;
    letters = 0;
    lower = 0;
    upper = 0;
    vowels = 0;
    consonants = 0;
    
    printf("Enter filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    for (i = 0; filename[i] != '\0'; i++) {
        if (filename[i] == '\n') {
            filename[i] = '\0';
            break;
        }
    }
    
    F = fopen(filename, "r");
    
    if (F == NULL) {
        printf("Error: Cannot open file '%s'\n", filename);
        return 1;
    }
    
    while ((c = fgetc(F)) != EOF) {
        total++;
        
        if (c == '\n') {
            lines++;
        }
        
        if (isdigit(c)) {
            digits++;
        }
        
        if (ispunct(c)) {
            punct++;
        }
        
        if (isspace(c)) {
            spaces++;
        }
        
        if (isalpha(c)) {
            letters++;
            
            if (islower(c)) {
                lower++;
            }
            
            if (isupper(c)) {
                upper++;
            }
            
            if (isvowel(c)) {
                vowels++;
            }
            
            if (isconsonant(c)) {
                consonants++;
            }
        }
    }
    
    fclose(F);
    
    printf("\n=== Results for file '%s' ===\n", filename);
    printf("Total characters: %d\n", total);
    printf("Number of lines: %d\n", lines);
    printf("Number of digits: %d\n", digits);
    printf("Number of punctuation marks: %d\n", punct);
    printf("Number of whitespace characters: %d\n", spaces);
    printf("Number of letters: %d\n", letters);
    printf("  - Lowercase letters: %d\n", lower);
    printf("  - Uppercase letters: %d\n", upper);
    printf("  - Vowels: %d\n", vowels);
    printf("  - Consonants: %d\n", consonants);
    
    return 0;
}

int isvowel(int c)
{
    switch(tolower(c))
    {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'y':
            return 1;
        default:
            return 0;
    }
}

int isconsonant(int c)
{
    if (isalpha(c) && !isvowel(c)) {
        return 1;
    }
    return 0;
}

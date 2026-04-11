#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct word {
    int count;
    char *Word;
} WORD;

WORD *words[100000];
int totalw = 0;

char *getword(FILE *F);
int cmpword_alpha(const void *w1, const void *w2);
int cmpword_quant(const void *w1, const void *w2);

int main(int ac, char *av[])
{
    FILE *OUT1, *OUT2, *IN;
    char *pword;
    int i;
    
    if (ac < 2)
    {
        printf("You forgot to specify the list of files!\n");
        return 0;
    }
    
    OUT1 = fopen("dictionary_alpha.txt", "w");
    if (OUT1 == NULL)
    {
        printf("Error: Cannot create output file dictionary_alpha.txt\n");
        return 1;
    }
    
    OUT2 = fopen("dictionary_freq.txt", "w");
    if (OUT2 == NULL)
    {
        printf("Error: Cannot create output file dictionary_freq.txt\n");
        fclose(OUT1);
        return 1;
    }
    
    while (--ac)
    {
        char filename[512];
        sprintf(filename, "texts/%s", av[ac]);
        IN = fopen(filename, "r");
        
        if (IN == NULL)
        {
            printf("Error: Cannot open file %s\n", filename);
            continue;
        }
        
        printf("Processing file %s\n", av[ac]);
        
        while ((pword = getword(IN)) != NULL)
        {
            int found = 0;
            
            for (i = 0; i < totalw; i++)
            {
                if (strcmp(words[i]->Word, pword) == 0)
                {
                    words[i]->count++;
                    found = 1;
                    free(pword);
                    break;
                }
            }
            
            if (!found)
            {
                words[totalw] = (WORD *)malloc(sizeof(WORD));
                words[totalw]->Word = (char *)malloc(strlen(pword) + 1);
                strcpy(words[totalw]->Word, pword);
                words[totalw]->count = 1;
                totalw++;
                free(pword);
            }
        }
        
        fclose(IN);
    }
    
    qsort(words, totalw, sizeof(WORD *), cmpword_alpha);
    
    fprintf(OUT1, "=== Dictionary sorted by alphabet ===\n");
    fprintf(OUT1, "Total unique words: %d\n\n", totalw);
    for (i = 0; i < totalw; i++)
    {
        fprintf(OUT1, "%s: %d\n", words[i]->Word, words[i]->count);
    }
    
    qsort(words, totalw, sizeof(WORD *), cmpword_quant);
    
    fprintf(OUT2, "=== Dictionary sorted by frequency ===\n");
    fprintf(OUT2, "Total unique words: %d\n\n", totalw);
    for (i = 0; i < totalw; i++)
    {
        fprintf(OUT2, "%s: %d\n", words[i]->Word, words[i]->count);
    }
    
    fclose(OUT1);
    fclose(OUT2);
    
    for (i = 0; i < totalw; i++)
    {
        free(words[i]->Word);
        free(words[i]);
    }
    
    printf("\nDone! Output files: dictionary_alpha.txt and dictionary_freq.txt\n");
    
    return 0;
}

char *getword(FILE *F)
{
    char buffer[1024];
    int c;
    int pos = 0;
    int inword = 0;
    
    while ((c = fgetc(F)) != EOF)
    {
        if (isalpha(c))
        {
            inword = 1;
            buffer[pos++] = tolower(c);
        }
        else if (inword && isdigit(c))
        {
            buffer[pos++] = c;
        }
        else if (inword)
        {
            buffer[pos] = '\0';
            char *s = (char *)malloc(strlen(buffer) + 1);
            strcpy(s, buffer);
            return s;
        }
    }
    
    if (inword && pos > 0)
    {
        buffer[pos] = '\0';
        char *s = (char *)malloc(strlen(buffer) + 1);
        strcpy(s, buffer);
        return s;
    }
    
    return NULL;
}

int cmpword_alpha(const void *w1, const void *w2)
{
    WORD *word1 = *(WORD **)w1;
    WORD *word2 = *(WORD **)w2;
    return strcmp(word1->Word, word2->Word);
}

int cmpword_quant(const void *w1, const void *w2)
{
    WORD *word1 = *(WORD **)w1;
    WORD *word2 = *(WORD **)w2;
    return (word2->count - word1->count);
}

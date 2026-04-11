#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_AUTHOR 100
#define MAX_TITLE 200
#define MAX_PUBLISHER 100
#define MAX_YEAR 10

typedef struct book {
    char author[MAX_AUTHOR];
    char title[MAX_TITLE];
    char publisher[MAX_PUBLISHER];
    char year[MAX_YEAR];
} BOOK;

struct tnode {
    BOOK data;
    struct tnode *lchild, *rchild;
};

int compare_by_author_title(BOOK *b1, BOOK *b2);
int compare_first_5_chars(const char *str1, const char *str2);
char* get_bib_field(char *line, char *field);
struct tnode* tree_insert(struct tnode *p, BOOK *newbook);
void tree_print(struct tnode *p);
void tree_free(struct tnode *p);
void process_bib_file(struct tnode **root, char *filename);
void to_lowercase(char *str);

int compare_by_author_title(BOOK *b1, BOOK *b2)
{
    int cmp = strcmp(b1->author, b2->author);
    if (cmp != 0) {
        return cmp;
    }
    return strcmp(b1->title, b2->title);
}

int compare_first_5_chars(const char *str1, const char *str2)
{
    return strncmp(str1, str2, 5);
}

void to_lowercase(char *str)
{
    int i;
    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

char* get_bib_field(char *line, char *field)
{
    char *start, *end;
    
    if (line[0] == field[0] && line[1] == field[1]) {
        start = line + 3;
        end = strchr(start, '\n');
        if (end == NULL) {
            end = strchr(start, '\r');
        }
        if (end != NULL) {
            *end = '\0';
        }
        while (*start == ' ') start++;
        return start;
    }
    return NULL;
}

struct tnode* tree_insert(struct tnode *p, BOOK *newbook)
{
    if (p == NULL) {
        p = (struct tnode *) malloc(sizeof(struct tnode));
        if (p == NULL) {
            printf("Cannot allocate memory\n");
            exit(0);
        }
        p->data = *newbook;
        p->lchild = p->rchild = NULL;
        return p;
    }
    
    if (compare_by_author_title(&p->data, newbook) > 0) {
        p->lchild = tree_insert(p->lchild, newbook);
        return p;
    }
    
    if (compare_by_author_title(&p->data, newbook) < 0) {
        p->rchild = tree_insert(p->rchild, newbook);
        return p;
    }
    
    return p;
}

void tree_print(struct tnode *p)
{
    if (p != NULL) {
        tree_print(p->lchild);
        printf("\n========================================\n");
        printf("Author: %s\n", p->data.author);
        printf("Title: %s\n", p->data.title);
        printf("Publisher: %s\n", p->data.publisher);
        printf("Year: %s\n", p->data.year);
        printf("========================================\n");
        tree_print(p->rchild);
    }
}

void tree_free(struct tnode *p)
{
    if (p != NULL) {
        tree_free(p->lchild);
        tree_free(p->rchild);
        free(p);
    }
}

void process_bib_file(struct tnode **root, char *filename)
{
    FILE *fp;
    char line[500];
    BOOK current_book;
    int in_record = 0;
    int has_fields = 0;
    char *value;
    
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    printf("Processing file: %s\n", filename);
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line[0] == '\n' || line[0] == '\r') {
            if (in_record && has_fields) {
                tree_insert(*root, &current_book);
                if (*root == NULL) {
                    *root = tree_insert(*root, &current_book);
                } else {
                    tree_insert(*root, &current_book);
                }
                has_fields = 0;
            }
            in_record = 0;
            continue;
        }
        
        if (strncmp(line, "@", 1) == 0) {
            in_record = 1;
            memset(&current_book, 0, sizeof(BOOK));
            continue;
        }
        
        if (in_record) {
            value = get_bib_field(line, "AU");
            if (value != NULL) {
                strncpy(current_book.author, value, MAX_AUTHOR - 1);
                to_lowercase(current_book.author);
                has_fields = 1;
            }
            else {
                value = get_bib_field(line, "TI");
                if (value != NULL) {
                    strncpy(current_book.title, value, MAX_TITLE - 1);
                    to_lowercase(current_book.title);
                    has_fields = 1;
                }
                else {
                    value = get_bib_field(line, "PU");
                    if (value != NULL) {
                        strncpy(current_book.publisher, value, MAX_PUBLISHER - 1);
                        to_lowercase(current_book.publisher);
                        has_fields = 1;
                    }
                    else {
                        value = get_bib_field(line, "PY");
                        if (value != NULL) {
                            strncpy(current_book.year, value, MAX_YEAR - 1);
                            has_fields = 1;
                        }
                    }
                }
            }
        }
    }
    
    if (in_record && has_fields) {
        if (*root == NULL) {
            *root = tree_insert(*root, &current_book);
        } else {
            tree_insert(*root, &current_book);
        }
    }
    
    fclose(fp);
}

int main(int argc, char *argv[])
{
    struct tnode *root = NULL;
    FILE *outfile;
    int i;
    
    if (argc < 2) {
        printf("You forgot to specify the list of files!\n");
        printf("Usage: %s file1.bib file2.bib ...\n", argv[0]);
        return 1;
    }
    
    for (i = 1; i < argc; i++) {
        process_bib_file(&root, argv[i]);
    }
    
    outfile = fopen("bibliography_sorted.txt", "w");
    if (outfile == NULL) {
        printf("Error: Cannot create output file\n");
        tree_free(root);
        return 1;
    }
    
    freopen("bibliography_sorted.txt", "w", stdout);
    
    printf("=== BIBLIOGRAPHY SORTED BY AUTHOR AND TITLE ===\n\n");
    tree_print(root);
    
    fclose(outfile);
    
    printf("\nDone! Output saved to bibliography_sorted.txt\n");
    
    tree_free(root);
    
    return 0;
}

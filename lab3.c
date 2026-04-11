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

void to_lowercase(char *str)
{
    int i;
    for (i = 0; str[i]; i++) str[i] = tolower(str[i]);
}

char* get_bib_field(char *line, char *field)
{
    char *start;
    if (line[0] == field[0] && line[1] == field[1]) {
        start = line + 3;
        while (*start == ' ') start++;
        return start;
    }
    return NULL;
}

int compare_books(BOOK *b1, BOOK *b2)
{
    int cmp = strcmp(b1->author, b2->author);
    if (cmp != 0) return cmp;
    return strcmp(b1->title, b2->title);
}

struct tnode* tree_insert(struct tnode *p, BOOK *newbook)
{
    if (p == NULL) {
        p = (struct tnode *) malloc(sizeof(struct tnode));
        if (!p) { printf("No memory\n"); exit(0); }
        p->data = *newbook;
        p->lchild = p->rchild = NULL;
        return p;
    }
    
    if (compare_books(&p->data, newbook) > 0)
        p->lchild = tree_insert(p->lchild, newbook);
    else if (compare_books(&p->data, newbook) < 0)
        p->rchild = tree_insert(p->rchild, newbook);
    
    return p;
}

void tree_print(struct tnode *p)
{
    if (p) {
        tree_print(p->lchild);
        printf("\n========================================\n");
        printf("Author: %s\nTitle: %s\nPublisher: %s\nYear: %s\n", 
               p->data.author, p->data.title, p->data.publisher, p->data.year);
        printf("========================================\n");
        tree_print(p->rchild);
    }
}

void tree_free(struct tnode *p)
{
    if (p) {
        tree_free(p->lchild);
        tree_free(p->rchild);
        free(p);
    }
}

void process_file(struct tnode **root, char *filename)
{
    FILE *fp = fopen(filename, "r");
    char line[500];
    BOOK book;
    int in_record = 0, has_fields = 0;
    char *value;
    
    if (!fp) { printf("Cannot open %s\n", filename); return; }
    printf("Processing: %s\n", filename);
    
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == '\r') {
            if (in_record && has_fields) {
                *root = tree_insert(*root, &book);
                has_fields = 0;
            }
            in_record = 0;
            continue;
        }
        
        if (line[0] == '@') {
            in_record = 1;
            memset(&book, 0, sizeof(BOOK));
            continue;
        }
        
        if (in_record) {
            if ((value = get_bib_field(line, "AU"))) {
                strncpy(book.author, value, MAX_AUTHOR-1);
                to_lowercase(book.author);
                has_fields = 1;
            }
            else if ((value = get_bib_field(line, "TI"))) {
                strncpy(book.title, value, MAX_TITLE-1);
                to_lowercase(book.title);
            }
            else if ((value = get_bib_field(line, "PU"))) {
                strncpy(book.publisher, value, MAX_PUBLISHER-1);
                to_lowercase(book.publisher);
            }
            else if ((value = get_bib_field(line, "PY"))) {
                strncpy(book.year, value, MAX_YEAR-1);
            }
        }
    }
    
    if (in_record && has_fields) *root = tree_insert(*root, &book);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    struct tnode *root = NULL;
    int i;
    
    if (argc < 2) {
        printf("Usage: %s file1.bib file2.bib ...\n", argv[0]);
        return 1;
    }
    
    for (i = 1; i < argc; i++) process_file(&root, argv[i]);
    
    freopen("bibliography_sorted.txt", "w", stdout);
    printf("=== BIBLIOGRAPHY SORTED BY AUTHOR AND TITLE ===\n\n");
    tree_print(root);
    
    printf("\nDone! Output saved to bibliography_sorted.txt\n");
    tree_free(root);
    
    return 0;
}
